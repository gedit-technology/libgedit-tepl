/* SPDX-FileCopyrightText: 2022 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_SETTINGS_H
#define TEPL_SETTINGS_H

#include <gio/gio.h>

G_BEGIN_DECLS

#define TEPL_TYPE_SETTINGS             (_tepl_settings_get_type ())
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
};

G_GNUC_INTERNAL
GType		_tepl_settings_get_type				(void);

G_GNUC_INTERNAL
TeplSettings *	_tepl_settings_get_singleton			(void);

G_GNUC_INTERNAL
void		_tepl_settings_unref_singleton			(void);

G_GNUC_INTERNAL
GSettings *	_tepl_settings_peek_desktop_interface_settings	(TeplSettings *self);

G_END_DECLS

#endif /* TEPL_SETTINGS_H */
