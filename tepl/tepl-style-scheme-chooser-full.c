/* SPDX-FileCopyrightText: 2020-2024 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "config.h"
#include "tepl-style-scheme-chooser-full.h"
#include <handy.h>
#include <glib/gi18n-lib.h>
#include "tepl-style-scheme-chooser-simple.h"
#include "tepl-settings.h"

/**
 * SECTION:style-scheme-chooser-full
 * @Title: TeplStyleSchemeChooserFull
 * @Short_description: A #GtkSourceStyleScheme chooser widget for light and dark themes
 *
 * #TeplStyleSchemeChooserFull is a widget that permits to choose a
 * #GtkSourceStyleScheme.
 *
 * It is similar to #TeplStyleSchemeChooserSimple, but supports both light and
 * dark GTK themes in a single widget.
 *
 * #TeplStyleSchemeChooserFull has a title for the component. The title
 * describes whether it is for a light or dark GTK theme. There is only one list
 * of style schemes shown, and it dynamically changes depending on the GTK
 * theme.
 *
 * It connects to the two #GSettings keys provided by
 * tepl_settings_provide_style_scheme_settings().
 */

struct _TeplStyleSchemeChooserFullPrivate
{
	/* The title of the component. */
	GtkLabel *label;

	/* Only one of the two is visible at a time. */
	TeplStyleSchemeChooserSimple *simple_chooser_for_light_theme;
	TeplStyleSchemeChooserSimple *simple_chooser_for_dark_theme;
};

G_DEFINE_TYPE_WITH_PRIVATE (TeplStyleSchemeChooserFull, tepl_style_scheme_chooser_full, GTK_TYPE_BIN)

static GtkLabel *
create_label (void)
{
	GtkLabel *label;

	label = GTK_LABEL (gtk_label_new (NULL));
	gtk_widget_set_halign (GTK_WIDGET (label), GTK_ALIGN_START);

	return label;
}

static TeplStyleSchemeChooserSimple *
create_simple_chooser (gboolean for_dark_theme_variant)
{
	TeplStyleSchemeChooserSimple *simple_chooser;

	simple_chooser = tepl_style_scheme_chooser_simple_new (for_dark_theme_variant);

	/* To do like tepl_utils_get_titled_component(). */
	gtk_widget_set_margin_start (GTK_WIDGET (simple_chooser), 12);

	return simple_chooser;
}

static GtkContainer *
create_vgrid (void)
{
	GtkContainer *vgrid;

	vgrid = GTK_CONTAINER (gtk_grid_new ());
	gtk_orientable_set_orientation (GTK_ORIENTABLE (vgrid), GTK_ORIENTATION_VERTICAL);
	gtk_grid_set_row_spacing (GTK_GRID (vgrid), 6);

	return vgrid;
}

static void
update_label (TeplStyleSchemeChooserFull *chooser)
{
	const gchar *text;
	gchar *markup;

	if (hdy_style_manager_get_dark (hdy_style_manager_get_default ()))
	{
		text = _("Color Scheme for the Dark Theme Variant");
	}
	else
	{
		text = _("Color Scheme for the Light Theme Variant");
	}

	markup = g_strdup_printf ("<b>%s</b>", text);
	gtk_label_set_markup (chooser->priv->label, markup);
	g_free (markup);
}

static void
update_simple_choosers_visibility (TeplStyleSchemeChooserFull *chooser)
{
	gboolean theme_is_dark;

	theme_is_dark = hdy_style_manager_get_dark (hdy_style_manager_get_default ());

	gtk_widget_set_visible (GTK_WIDGET (chooser->priv->simple_chooser_for_light_theme), !theme_is_dark);
	gtk_widget_set_visible (GTK_WIDGET (chooser->priv->simple_chooser_for_dark_theme), theme_is_dark);
}

static void
theme_variant_notify_cb (HdyStyleManager            *style_manager,
			 GParamSpec                 *pspec,
			 TeplStyleSchemeChooserFull *chooser)
{
	update_label (chooser);
	update_simple_choosers_visibility (chooser);
}

static void
bind_settings (TeplStyleSchemeChooserFull *chooser)
{
	GSettings *settings;
	const gchar *key_for_light_theme_variant = NULL;
	const gchar *key_for_dark_theme_variant = NULL;

	settings = _tepl_settings_get_style_scheme_settings (tepl_settings_get_singleton (),
							     &key_for_light_theme_variant,
							     &key_for_dark_theme_variant);

	if (settings == NULL)
	{
		return;
	}

	if (key_for_light_theme_variant != NULL)
	{
		g_settings_bind (settings, key_for_light_theme_variant,
				 chooser->priv->simple_chooser_for_light_theme, "style-scheme-id",
				 G_SETTINGS_BIND_DEFAULT);
	}

	if (key_for_dark_theme_variant != NULL)
	{
		g_settings_bind (settings, key_for_dark_theme_variant,
				 chooser->priv->simple_chooser_for_dark_theme, "style-scheme-id",
				 G_SETTINGS_BIND_DEFAULT);
	}
}

static void
tepl_style_scheme_chooser_full_dispose (GObject *object)
{
	TeplStyleSchemeChooserFull *chooser = TEPL_STYLE_SCHEME_CHOOSER_FULL (object);

	chooser->priv->label = NULL;
	chooser->priv->simple_chooser_for_light_theme = NULL;
	chooser->priv->simple_chooser_for_dark_theme = NULL;

	G_OBJECT_CLASS (tepl_style_scheme_chooser_full_parent_class)->dispose (object);
}

static void
tepl_style_scheme_chooser_full_class_init (TeplStyleSchemeChooserFullClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->dispose = tepl_style_scheme_chooser_full_dispose;
}

static void
tepl_style_scheme_chooser_full_init (TeplStyleSchemeChooserFull *chooser)
{
	GtkContainer *vgrid;

	chooser->priv = tepl_style_scheme_chooser_full_get_instance_private (chooser);

	/* Create individual widgets */
	chooser->priv->label = create_label ();
	chooser->priv->simple_chooser_for_light_theme = create_simple_chooser (FALSE);
	chooser->priv->simple_chooser_for_dark_theme = create_simple_chooser (TRUE);
	vgrid = create_vgrid ();

	/* Packing */
	gtk_container_add (vgrid, GTK_WIDGET (chooser->priv->label));
	gtk_container_add (vgrid, GTK_WIDGET (chooser->priv->simple_chooser_for_light_theme));
	gtk_container_add (vgrid, GTK_WIDGET (chooser->priv->simple_chooser_for_dark_theme));
	gtk_container_add (GTK_CONTAINER (chooser), GTK_WIDGET (vgrid));
	gtk_widget_show_all (GTK_WIDGET (chooser));

	/* Connect to signals */
	g_signal_connect_object (hdy_style_manager_get_default (),
				 "notify::dark",
				 G_CALLBACK (theme_variant_notify_cb),
				 chooser,
				 G_CONNECT_DEFAULT);

	/* Update state */
	update_label (chooser);
	update_simple_choosers_visibility (chooser);

	bind_settings (chooser);
}

/**
 * tepl_style_scheme_chooser_full_new:
 *
 * Returns: (transfer floating): a new #TeplStyleSchemeChooserFull widget.
 * Since: 6.12
 */
TeplStyleSchemeChooserFull *
tepl_style_scheme_chooser_full_new (void)
{
	return g_object_new (TEPL_TYPE_STYLE_SCHEME_CHOOSER_FULL, NULL);
}
