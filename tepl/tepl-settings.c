/* SPDX-FileCopyrightText: 2022 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "tepl-settings.h"

/**
 * SECTION:settings
 * @title: TeplSettings
 * @short_description: Central access to #GSettings objects
 *
 * #TeplSettings is a singleton class to have a central access to #GSettings
 * objects.
 *
 * The central access permits to:
 * - Share the same #GSettings objects between different parts of the
 *   application.
 * - Avoid the need to store #GSettings objects a bit everywhere in different
 *   classes.
 */

struct _TeplSettingsPrivate
{
	GSettings *settings_desktop_interface;
};

/* TeplSettings is a singleton. */
static TeplSettings *singleton = NULL;

G_DEFINE_TYPE_WITH_PRIVATE (TeplSettings, tepl_settings, G_TYPE_OBJECT)

static void
tepl_settings_dispose (GObject *object)
{
	TeplSettings *self = TEPL_SETTINGS (object);

	g_clear_object (&self->priv->settings_desktop_interface);

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

	object_class->dispose = tepl_settings_dispose;
	object_class->finalize = tepl_settings_finalize;
}

static void
tepl_settings_init (TeplSettings *self)
{
	self->priv = tepl_settings_get_instance_private (self);

	self->priv->settings_desktop_interface = g_settings_new ("org.gnome.desktop.interface");
}

/**
 * tepl_settings_get_singleton:
 *
 * Returns: (transfer none): the #TeplSettings singleton instance.
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
 */
GSettings *
tepl_settings_peek_desktop_interface_settings (TeplSettings *self)
{
	g_return_val_if_fail (TEPL_IS_SETTINGS (self), NULL);
	return self->priv->settings_desktop_interface;
}
