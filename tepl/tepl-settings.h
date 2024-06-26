/* SPDX-FileCopyrightText: 2022-2024 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_SETTINGS_H
#define TEPL_SETTINGS_H

#if !defined (TEPL_H_INSIDE) && !defined (TEPL_COMPILATION)
#error "Only <tepl/tepl.h> can be included directly."
#endif

#include <gio/gio.h>

G_BEGIN_DECLS

#define TEPL_TYPE_SETTINGS             (tepl_settings_get_type ())
#define TEPL_SETTINGS(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), TEPL_TYPE_SETTINGS, TeplSettings))
#define TEPL_SETTINGS_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), TEPL_TYPE_SETTINGS, TeplSettingsClass))
#define TEPL_IS_SETTINGS(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TEPL_TYPE_SETTINGS))
#define TEPL_IS_SETTINGS_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), TEPL_TYPE_SETTINGS))
#define TEPL_SETTINGS_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), TEPL_TYPE_SETTINGS, TeplSettingsClass))

typedef struct _TeplSettings         TeplSettings;
typedef struct _TeplSettingsClass    TeplSettingsClass;
typedef struct _TeplSettingsPrivate  TeplSettingsPrivate;

struct _TeplSettings
{
	GObject parent;

	TeplSettingsPrivate *priv;
};

struct _TeplSettingsClass
{
	GObjectClass parent_class;

	gpointer padding[12];
};

/**
 * TeplSettingsThemeVariant:
 * @TEPL_SETTINGS_THEME_VARIANT_SYSTEM: Follow the system's preference.
 * @TEPL_SETTINGS_THEME_VARIANT_LIGHT: Force a light theme.
 * @TEPL_SETTINGS_THEME_VARIANT_DARK: Force a dark theme.
 *
 * This enum is intended to be used for controlling the GTK theme preference.
 *
 * Since: 6.10
 */
typedef enum
{
	TEPL_SETTINGS_THEME_VARIANT_SYSTEM = 0,
	TEPL_SETTINGS_THEME_VARIANT_LIGHT = 1,
	TEPL_SETTINGS_THEME_VARIANT_DARK = 2,
} TeplSettingsThemeVariant;

G_MODULE_EXPORT
GType		tepl_settings_get_type				(void);

G_MODULE_EXPORT
TeplSettings *	tepl_settings_get_singleton			(void);

G_GNUC_INTERNAL
void		_tepl_settings_unref_singleton			(void);

G_MODULE_EXPORT
GSettings *	tepl_settings_peek_desktop_interface_settings	(TeplSettings *self);

G_MODULE_EXPORT
void		tepl_settings_provide_font_settings		(TeplSettings *self,
								 GSettings    *font_settings,
								 const gchar  *use_default_font_key,
								 const gchar  *editor_font_key);

G_MODULE_EXPORT
gchar *		tepl_settings_get_selected_font			(TeplSettings *self);

G_MODULE_EXPORT
void		tepl_settings_handle_theme_variant		(TeplSettings *self,
								 GSettings    *theme_variant_settings,
								 const gchar  *theme_variant_setting_key);

G_MODULE_EXPORT
void		tepl_settings_provide_style_scheme_settings	(TeplSettings *self,
								 GSettings    *style_scheme_settings,
								 const gchar  *key_for_light_theme_variant,
								 const gchar  *key_for_dark_theme_variant);

G_MODULE_EXPORT
gchar *		tepl_settings_get_style_scheme_id		(TeplSettings *self);

G_GNUC_INTERNAL
gchar *		_tepl_settings_get_default_style_scheme_id	(TeplSettings *self);

G_GNUC_INTERNAL
GSettings *	_tepl_settings_get_style_scheme_settings	(TeplSettings  *self,
								 const gchar  **key_for_light_theme_variant,
								 const gchar  **key_for_dark_theme_variant);

G_MODULE_EXPORT
gboolean	tepl_settings_get_range_uint			(GSettings   *settings,
								 const gchar *key,
								 guint32     *min,
								 guint32     *max);

G_END_DECLS

#endif /* TEPL_SETTINGS_H */
