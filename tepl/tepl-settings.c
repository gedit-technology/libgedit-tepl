/* SPDX-FileCopyrightText: 2022-2024 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "tepl-settings.h"
#include <handy.h>

/**
 * SECTION:settings
 * @title: TeplSettings
 * @short_description: Singleton class containing #GSettings objects
 *
 * #TeplSettings is a singleton class to have a central access to some
 * #GSettings objects and having convenience features that are useful for a text
 * editor.
 *
 * The central access permits to:
 * - Share the same #GSettings objects between different parts of the
 *   application.
 * - Avoid the need to store #GSettings objects a bit everywhere in different
 *   classes.
 */

#define KEY_SYSTEM_FONT "monospace-font-name"
#define KEY_SYSTEM_COLOR_SCHEME "color-scheme"

struct _TeplSettingsPrivate
{
	GSettings *settings_desktop_interface;

	/* Invariant: the following 3 instance variables are either all NULL or
	 * all set.
	 */
	GSettings *settings_font;
	gchar *key_use_default_font;
	gchar *key_editor_font;

	/* Invariant: the following 2 instance variables are either all NULL or
	 * all set.
	 */
	GSettings *settings_theme_variant;
	gchar *key_theme_variant;

	/* Invariant: the following 3 instance variables are either all NULL or
	 * all set.
	 */
	GSettings *settings_style_scheme;
	gchar *key_style_scheme_id_for_light_theme;
	gchar *key_style_scheme_id_for_dark_theme;

	guint handle_theme_variant : 1;
};

enum
{
	SIGNAL_FONT_CHANGED,
	N_SIGNALS
};

enum
{
	PROP_0,
	PROP_STYLE_SCHEME_ID,
	N_PROPERTIES
};

static guint signals[N_SIGNALS];
static GParamSpec *properties[N_PROPERTIES];

/* TeplSettings is a singleton. */
static TeplSettings *singleton = NULL;

G_DEFINE_TYPE_WITH_PRIVATE (TeplSettings, tepl_settings, G_TYPE_OBJECT)

static void
tepl_settings_get_property (GObject    *object,
			    guint       prop_id,
			    GValue     *value,
			    GParamSpec *pspec)
{
	TeplSettings *settings = TEPL_SETTINGS (object);

	switch (prop_id)
	{
		case PROP_STYLE_SCHEME_ID:
			g_value_take_string (value, tepl_settings_get_style_scheme_id (settings));
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
tepl_settings_dispose (GObject *object)
{
	TeplSettings *self = TEPL_SETTINGS (object);

	g_clear_object (&self->priv->settings_desktop_interface);

	g_clear_object (&self->priv->settings_font);
	g_clear_pointer (&self->priv->key_use_default_font, g_free);
	g_clear_pointer (&self->priv->key_editor_font, g_free);

	g_clear_object (&self->priv->settings_theme_variant);
	g_clear_pointer (&self->priv->key_theme_variant, g_free);

	g_clear_object (&self->priv->settings_style_scheme);
	g_clear_pointer (&self->priv->key_style_scheme_id_for_light_theme, g_free);
	g_clear_pointer (&self->priv->key_style_scheme_id_for_dark_theme, g_free);

	G_OBJECT_CLASS (tepl_settings_parent_class)->dispose (object);
}

static void
tepl_settings_finalize (GObject *object)
{
	if (singleton == TEPL_SETTINGS (object))
	{
		singleton = NULL;
	}

	G_OBJECT_CLASS (tepl_settings_parent_class)->finalize (object);
}

static void
tepl_settings_class_init (TeplSettingsClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->get_property = tepl_settings_get_property;
	object_class->dispose = tepl_settings_dispose;
	object_class->finalize = tepl_settings_finalize;

	/**
	 * TeplSettings::font-changed:
	 * @settings: the #TeplSettings emitting the signal.
	 *
	 * The ::font-changed signal is emitted when the return value of
	 * tepl_settings_get_selected_font() has potentially changed.
	 *
	 * It takes into account the @use_default_font_key provided with
	 * tepl_settings_provide_font_settings() to avoid unnecessary signal
	 * emission. In other words, for example if @use_default_font_key is
	 * %FALSE, the signal is not emitted even if the default (system's) font
	 * has been modified.
	 *
	 * The relation between this signal and a potential zoom in and zoom out
	 * feature, to temporarily make the font larger or smaller (so the zoom
	 * level not being stored in #GSettings): when this signal is emitted,
	 * it means that the user has explicitly changed the font setting, and
	 * as such he or she probably wants to use that font instead. So the
	 * expected behavior in that case is to reset the zoom level. But it's
	 * left as an exercise for another class, since #TeplSettings is only
	 * for #GSettings.
	 *
	 * Since: 6.2
	 */
	signals[SIGNAL_FONT_CHANGED] =
		g_signal_new ("font-changed",
			      G_TYPE_FROM_CLASS (klass),
			      G_SIGNAL_RUN_FIRST,
			      0, NULL, NULL, NULL,
			      G_TYPE_NONE, 0);

	/**
	 * TeplSettings:style-scheme-id:
	 *
	 * See tepl_settings_provide_style_scheme_settings().
	 *
	 * The value of this property depends on whether the GTK theme is light
	 * or dark, and uses the corresponding #GSettings key.
	 *
	 * Since: 6.12
	 */
	properties[PROP_STYLE_SCHEME_ID] =
		g_param_spec_string ("style-scheme-id",
				     "style-scheme-id",
				     "",
				     NULL,
				     G_PARAM_READABLE |
				     G_PARAM_STATIC_STRINGS);

	g_object_class_install_properties (object_class, N_PROPERTIES, properties);
}

static gboolean
use_default_font (TeplSettings *self)
{
	if (self->priv->settings_font == NULL)
	{
		return TRUE;
	}

	return g_settings_get_boolean (self->priv->settings_font,
				       self->priv->key_use_default_font);
}

static void
system_font_changed_cb (GSettings *settings_desktop_interface,
			gchar     *key,
			gpointer   user_data)
{
	TeplSettings *self = TEPL_SETTINGS (user_data);

	if (use_default_font (self))
	{
		g_signal_emit (self, signals[SIGNAL_FONT_CHANGED], 0);
	}
}

static void
use_default_font_changed_cb (GSettings *settings_font,
			     gchar     *key,
			     gpointer   user_data)
{
	TeplSettings *self = TEPL_SETTINGS (user_data);

	g_signal_emit (self, signals[SIGNAL_FONT_CHANGED], 0);
}

static void
editor_font_changed_cb (GSettings *settings_font,
			gchar     *key,
			gpointer   user_data)
{
	TeplSettings *self = TEPL_SETTINGS (user_data);

	if (!use_default_font (self))
	{
		g_signal_emit (self, signals[SIGNAL_FONT_CHANGED], 0);
	}
}

static void
tepl_settings_init (TeplSettings *self)
{
	self->priv = tepl_settings_get_instance_private (self);

	self->priv->settings_desktop_interface = g_settings_new ("org.gnome.desktop.interface");

	g_signal_connect_object (self->priv->settings_desktop_interface,
				 "changed::" KEY_SYSTEM_FONT,
				 G_CALLBACK (system_font_changed_cb),
				 self,
				 0);
}

/**
 * tepl_settings_get_singleton:
 *
 * Returns: (transfer none): the #TeplSettings singleton instance.
 * Since: 6.2
 */
TeplSettings *
tepl_settings_get_singleton (void)
{
	if (singleton == NULL)
	{
		singleton = g_object_new (TEPL_TYPE_SETTINGS, NULL);
	}

	return singleton;
}

void
_tepl_settings_unref_singleton (void)
{
	if (singleton != NULL)
	{
		g_object_unref (singleton);
	}

	/* singleton is not set to NULL here, it is set to NULL in
	 * tepl_settings_finalize() (i.e. when we are sure that the ref count
	 * reaches 0).
	 */
}

/**
 * tepl_settings_peek_desktop_interface_settings:
 * @self: the #TeplSettings instance.
 *
 * Returns: (transfer none): the #GSettings for `"org.gnome.desktop.interface"`.
 * Since: 6.2
 */
GSettings *
tepl_settings_peek_desktop_interface_settings (TeplSettings *self)
{
	g_return_val_if_fail (TEPL_IS_SETTINGS (self), NULL);
	return self->priv->settings_desktop_interface;
}

/**
 * tepl_settings_provide_font_settings:
 * @self: the #TeplSettings instance.
 * @font_settings: the #GSettings object containing the keys.
 * @use_default_font_key: a key of type boolean.
 * @editor_font_key: a key of type string.
 *
 * This function can only be called once, to provide two keys:
 * - @use_default_font_key: must be of type boolean, to know whether to use the
 *   system's font (the default font), or the editor font.
 * - @editor_font_key: must be of type string, containing the font name to have
 *   a different font for the text editor, rather than using the system's font.
 *
 * See tepl_settings_get_selected_font().
 *
 * Since: 6.2
 */
void
tepl_settings_provide_font_settings (TeplSettings *self,
				     GSettings    *font_settings,
				     const gchar  *use_default_font_key,
				     const gchar  *editor_font_key)
{
	gchar *detailed_signal = NULL;

	g_return_if_fail (TEPL_IS_SETTINGS (self));
	g_return_if_fail (G_IS_SETTINGS (font_settings));
	g_return_if_fail (use_default_font_key != NULL);
	g_return_if_fail (editor_font_key != NULL);

	g_return_if_fail (self->priv->settings_font == NULL);
	g_return_if_fail (self->priv->key_use_default_font == NULL);
	g_return_if_fail (self->priv->key_editor_font == NULL);

	self->priv->settings_font = g_object_ref (font_settings);
	self->priv->key_use_default_font = g_strdup (use_default_font_key);
	self->priv->key_editor_font = g_strdup (editor_font_key);

	detailed_signal = g_strconcat ("changed::", use_default_font_key, NULL);
	g_signal_connect_object (font_settings,
				 detailed_signal,
				 G_CALLBACK (use_default_font_changed_cb),
				 self,
				 0);
	g_free (detailed_signal);

	detailed_signal = g_strconcat ("changed::", editor_font_key, NULL);
	g_signal_connect_object (font_settings,
				 detailed_signal,
				 G_CALLBACK (editor_font_changed_cb),
				 self,
				 0);
	g_free (detailed_signal);

	if (!use_default_font (self))
	{
		g_signal_emit (self, signals[SIGNAL_FONT_CHANGED], 0);
	}
}

/**
 * tepl_settings_get_selected_font:
 * @self: the #TeplSettings instance.
 *
 * If tepl_settings_provide_font_settings() has *not* been called, this function
 * always returns the current value of the system's fixed width (monospace)
 * font.
 *
 * If tepl_settings_provide_font_settings() *has* been called, this function
 * returns the font name depending on the boolean value of
 * @use_default_font_key.
 *
 * See also the #TeplSettings::font-changed signal,
 * tepl_utils_override_font_string() and tepl_prefs_create_font_component().
 *
 * Returns: the selected font name.
 * Since: 6.2
 */
gchar *
tepl_settings_get_selected_font (TeplSettings *self)
{
	g_return_val_if_fail (TEPL_IS_SETTINGS (self), NULL);

	if (use_default_font (self))
	{
		return g_settings_get_string (self->priv->settings_desktop_interface,
					      KEY_SYSTEM_FONT);
	}

	return g_settings_get_string (self->priv->settings_font,
				      self->priv->key_editor_font);
}

static void
update_theme_variant (TeplSettings *self)
{
	HdyStyleManager *style_manager = hdy_style_manager_get_default ();

	/* For a text editor, prefer a light theme. See the documentation of the
	 * GtkSettings:gtk-application-prefer-dark-theme property:
	 *
	 * "Dark themes should not be used for documents, where large spaces are
	 * white/light and the dark chrome creates too much contrast (web
	 * browser, text editor...)."
	 */
	HdyColorScheme color_scheme = HDY_COLOR_SCHEME_PREFER_LIGHT;

	if (self->priv->settings_theme_variant != NULL)
	{
		TeplSettingsThemeVariant variant;

		variant = g_settings_get_enum (self->priv->settings_theme_variant,
					       self->priv->key_theme_variant);

		switch (variant)
		{
			case TEPL_SETTINGS_THEME_VARIANT_SYSTEM:
				if (!hdy_style_manager_get_system_supports_color_schemes (style_manager))
				{
					/* Do as if the 'system' enum value didn't exist,
					 * with 'light' as the default.
					 */
					color_scheme = HDY_COLOR_SCHEME_FORCE_LIGHT;
				}
				break;

			case TEPL_SETTINGS_THEME_VARIANT_LIGHT:
				color_scheme = HDY_COLOR_SCHEME_FORCE_LIGHT;
				break;

			case TEPL_SETTINGS_THEME_VARIANT_DARK:
				color_scheme = HDY_COLOR_SCHEME_FORCE_DARK;
				break;

			default:
				g_return_if_reached ();
		}
	}

	hdy_style_manager_set_color_scheme (style_manager, color_scheme);
}

static void
theme_variant_changed_cb (GSettings    *settings,
			  const gchar  *key,
			  TeplSettings *self)
{
	update_theme_variant (self);
}

/**
 * tepl_settings_handle_theme_variant:
 * @self: the #TeplSettings instance.
 * @theme_variant_settings: (nullable): a #GSettings object, or %NULL.
 * @theme_variant_setting_key: (nullable): a #GSettings key of type enum
 *   #TeplSettingsThemeVariant, or %NULL.
 *
 * This function permits to setup correctly the GTK theme variant to use.
 *
 * If @theme_variant_settings and @theme_variant_setting_key are %NULL, only the
 * system's settings will be taken into account.
 *
 * If @theme_variant_settings and @theme_variant_setting_key are provided, they
 * are taken into account according to #TeplSettingsThemeVariant.
 *
 * You should call this function only once, as subsequent calls are ignored (the
 * setting used cannot be reconfigured).
 *
 * Since: 6.10
 */
void
tepl_settings_handle_theme_variant (TeplSettings *self,
				    GSettings    *theme_variant_settings,
				    const gchar  *theme_variant_setting_key)
{
	g_return_if_fail (TEPL_IS_SETTINGS (self));
	g_return_if_fail (theme_variant_settings == NULL || G_IS_SETTINGS (theme_variant_settings));

	if (self->priv->handle_theme_variant)
	{
		/* Already done. Cannot be re-configured. */
		return;
	}

	g_return_if_fail (self->priv->settings_theme_variant == NULL);
	g_return_if_fail (self->priv->key_theme_variant == NULL);

	hdy_init ();

	if (theme_variant_settings != NULL &&
	    theme_variant_setting_key != NULL)
	{
		gchar *detailed_signal;

		self->priv->settings_theme_variant = g_object_ref (theme_variant_settings);
		self->priv->key_theme_variant = g_strdup (theme_variant_setting_key);

		detailed_signal = g_strconcat ("changed::", theme_variant_setting_key, NULL);
		g_signal_connect_object (theme_variant_settings,
					 detailed_signal,
					 G_CALLBACK (theme_variant_changed_cb),
					 self,
					 G_CONNECT_DEFAULT);
		g_free (detailed_signal);
	}

	update_theme_variant (self);

	self->priv->handle_theme_variant = TRUE;
}

static void
style_scheme_setting_changed_cb (GSettings    *style_scheme_settings,
				 const gchar  *key,
				 TeplSettings *self)
{
	g_object_notify_by_pspec (G_OBJECT (self), properties[PROP_STYLE_SCHEME_ID]);
}

static void
style_manager_dark_notify_cb (HdyStyleManager *style_manager,
			      GParamSpec      *pspec,
			      TeplSettings    *self)
{
	g_object_notify_by_pspec (G_OBJECT (self), properties[PROP_STYLE_SCHEME_ID]);
}

/**
 * tepl_settings_provide_style_scheme_settings:
 * @self: the #TeplSettings instance.
 * @style_scheme_settings: the #GSettings object containing the keys.
 * @key_for_light_theme_variant: a key of type string.
 * @key_for_dark_theme_variant: a key of type string.
 *
 * This function can only be called once, to provide two keys:
 * - @key_for_light_theme_variant: must be of type string, containing the ID of
 *   a #GtkSourceStyleScheme that should be used when the GTK theme variant is
 *   light.
 * - @key_for_dark_theme_variant: the same, but when the GTK theme variant is
 *   dark.
 *
 * See tepl_settings_get_style_scheme_id().
 *
 * Since: 6.12
 */
void
tepl_settings_provide_style_scheme_settings (TeplSettings *self,
					     GSettings    *style_scheme_settings,
					     const gchar  *key_for_light_theme_variant,
					     const gchar  *key_for_dark_theme_variant)
{
	gchar *detailed_signal;
	HdyStyleManager *style_manager;

	g_return_if_fail (TEPL_IS_SETTINGS (self));
	g_return_if_fail (G_IS_SETTINGS (style_scheme_settings));
	g_return_if_fail (key_for_light_theme_variant != NULL);
	g_return_if_fail (key_for_dark_theme_variant != NULL);

	g_return_if_fail (self->priv->settings_style_scheme == NULL);
	g_return_if_fail (self->priv->key_style_scheme_id_for_light_theme == NULL);
	g_return_if_fail (self->priv->key_style_scheme_id_for_dark_theme == NULL);

	self->priv->settings_style_scheme = g_object_ref (style_scheme_settings);
	self->priv->key_style_scheme_id_for_light_theme = g_strdup (key_for_light_theme_variant);
	self->priv->key_style_scheme_id_for_dark_theme = g_strdup (key_for_dark_theme_variant);

	detailed_signal = g_strconcat ("changed::", key_for_light_theme_variant, NULL);
	g_signal_connect_object (style_scheme_settings,
				 detailed_signal,
				 G_CALLBACK (style_scheme_setting_changed_cb),
				 self,
				 G_CONNECT_DEFAULT);
	g_free (detailed_signal);

	detailed_signal = g_strconcat ("changed::", key_for_dark_theme_variant, NULL);
	g_signal_connect_object (style_scheme_settings,
				 detailed_signal,
				 G_CALLBACK (style_scheme_setting_changed_cb),
				 self,
				 G_CONNECT_DEFAULT);
	g_free (detailed_signal);

	style_manager = hdy_style_manager_get_default ();
	g_signal_connect_object (style_manager,
				 "notify::dark",
				 G_CALLBACK (style_manager_dark_notify_cb),
				 self,
				 G_CONNECT_DEFAULT);

	g_object_notify_by_pspec (G_OBJECT (self), properties[PROP_STYLE_SCHEME_ID]);
}

/**
 * tepl_settings_get_style_scheme_id:
 * @self: the #TeplSettings instance.
 *
 * Returns: (transfer full) (nullable): the value of the
 *   #TeplSettings:style-scheme-id property.
 * Since: 6.12
 */
gchar *
tepl_settings_get_style_scheme_id (TeplSettings *self)
{
	HdyStyleManager *style_manager;

	g_return_val_if_fail (TEPL_IS_SETTINGS (self), NULL);

	if (self->priv->settings_style_scheme == NULL)
	{
		return NULL;
	}

	style_manager = hdy_style_manager_get_default ();
	if (hdy_style_manager_get_dark (style_manager))
	{
		return g_settings_get_string (self->priv->settings_style_scheme,
					      self->priv->key_style_scheme_id_for_dark_theme);
	}

	return g_settings_get_string (self->priv->settings_style_scheme,
				      self->priv->key_style_scheme_id_for_light_theme);
}

gchar *
_tepl_settings_get_default_style_scheme_id (TeplSettings *self)
{
	g_return_val_if_fail (TEPL_IS_SETTINGS (self), NULL);

	if (self->priv->settings_style_scheme != NULL)
	{
		HdyStyleManager *style_manager = hdy_style_manager_get_default ();
		const gchar *key_style_scheme_id;
		GVariant *default_value;
		gchar *default_style_scheme_id = NULL;

		if (hdy_style_manager_get_dark (style_manager))
		{
			key_style_scheme_id = self->priv->key_style_scheme_id_for_dark_theme;
		}
		else
		{
			key_style_scheme_id = self->priv->key_style_scheme_id_for_light_theme;
		}

		default_value = g_settings_get_default_value (self->priv->settings_style_scheme,
							      key_style_scheme_id);

		if (default_value != NULL)
		{
			default_style_scheme_id = g_variant_dup_string (default_value, NULL);
			g_variant_unref (default_value);
		}

		if (default_style_scheme_id != NULL)
		{
			return default_style_scheme_id;
		}
	}

	return g_strdup ("tango");
}

/**
 * tepl_settings_get_range_uint:
 * @settings: a #GSettings.
 * @key: a key part of @settings.
 * @min: (out): the minimum value allowed by the range.
 * @max: (out): the maximum value allowed by the range.
 *
 * This function introspects a #GSettings key to get its range, if the key as a
 * "u" type (unsigned integer). %FALSE is returned if the introspection failed.
 *
 * See g_settings_schema_key_get_range() for more flexibility.
 *
 * Returns: whether the operation was successful.
 * Since: 6.2
 */
gboolean
tepl_settings_get_range_uint (GSettings   *settings,
			      const gchar *key,
			      guint32     *min,
			      guint32     *max)
{
	GSettingsSchema *schema = NULL;
	GSettingsSchemaKey *schema_key = NULL;
	GVariant *range = NULL;
	gchar *range_type = NULL;
	GVariant *range_values = NULL;
	const gchar *range_values_format;
	gboolean success = FALSE;

	g_return_val_if_fail (G_IS_SETTINGS (settings), FALSE);
	g_return_val_if_fail (key != NULL, FALSE);
	g_return_val_if_fail (min != NULL, FALSE);
	g_return_val_if_fail (max != NULL, FALSE);

	*min = 0;
	*max = 0;

	g_object_get (settings,
		      "settings-schema", &schema,
		      NULL);

	g_return_val_if_fail (schema != NULL, FALSE);

	if (!g_settings_schema_has_key (schema, key))
	{
		goto out;
	}

	schema_key = g_settings_schema_get_key (schema, key);
	range = g_settings_schema_key_get_range (schema_key);
	g_variant_get (range, "(sv)", &range_type, &range_values);

	if (g_strcmp0 (range_type, "range") != 0)
	{
		goto out;
	}

	range_values_format = g_variant_get_type_string (range_values);
	if (g_strcmp0 (range_values_format, "(uu)") != 0)
	{
		goto out;
	}

	g_variant_get (range_values, "(uu)", min, max);
	success = TRUE;

out:
	g_clear_pointer (&schema, g_settings_schema_unref);
	g_clear_pointer (&schema_key, g_settings_schema_key_unref);
	g_clear_pointer (&range, g_variant_unref);
	g_free (range_type);
	g_clear_pointer (&range_values, g_variant_unref);

	return success;
}
