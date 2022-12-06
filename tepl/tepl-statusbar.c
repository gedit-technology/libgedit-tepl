/* SPDX-FileCopyrightText: 2020-2022 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "tepl-statusbar.h"

/**
 * SECTION:statusbar
 * @Title: TeplStatusbar
 * @Short_description: Subclass of #GtkStatusbar
 *
 * #TeplStatusbar is a subclass of #GtkStatusbar with a better look by default.
 */

struct _TeplStatusbarPrivate
{
	gint something;
};

G_DEFINE_TYPE_WITH_PRIVATE (TeplStatusbar, tepl_statusbar, GTK_TYPE_STATUSBAR)

static void
tepl_statusbar_class_init (TeplStatusbarClass *klass)
{
}

static void
tepl_statusbar_init (TeplStatusbar *statusbar)
{
	statusbar->priv = tepl_statusbar_get_instance_private (statusbar);

	gtk_widget_set_margin_top (GTK_WIDGET (statusbar), 0);
	gtk_widget_set_margin_bottom (GTK_WIDGET (statusbar), 0);
}

/**
 * tepl_statusbar_new:
 *
 * Returns: (transfer floating): a new #TeplStatusbar.
 * Since: 5.0
 */
TeplStatusbar *
tepl_statusbar_new (void)
{
	return g_object_new (TEPL_TYPE_STATUSBAR, NULL);
}
