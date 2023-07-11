/* SPDX-FileCopyrightText: 2023 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "tepl-panel.h"

/**
 * SECTION:panel
 * @Title: TeplPanel
 * @Short_description: Side or bottom panel interface
 *
 * #TeplPanel represents a panel that is usually shown on the side or at the
 * bottom (inside a main window), and can contain several items.
 */

G_DEFINE_INTERFACE (TeplPanel, tepl_panel, G_TYPE_OBJECT)

static TeplPanelItem *
tepl_panel_add_default (TeplPanel   *panel,
			GtkWidget   *widget,
			const gchar *name,
			const gchar *title,
			const gchar *icon_name)
{
	return NULL;
}

static void
tepl_panel_remove_default (TeplPanel     *panel,
			   TeplPanelItem *item)
{
}

static void
tepl_panel_set_active_default (TeplPanel     *panel,
			       TeplPanelItem *item)
{
}

static void
tepl_panel_default_init (TeplPanelInterface *interface)
{
	interface->add = tepl_panel_add_default;
	interface->remove = tepl_panel_remove_default;
	interface->set_active = tepl_panel_set_active_default;
}

/**
 * tepl_panel_add:
 * @panel: a #TeplPanel.
 * @widget: a #GtkWidget.
 * @name: (nullable): the name.
 * @title: (nullable): the title.
 * @icon_name: (nullable): the icon-name.
 *
 * Adds an item to the panel. See #TeplPanelItem for the meaning of the parameters.
 *
 * @widget should have its #GtkWidget:visible property set to %TRUE in order to
 * be correctly shown in the panel.
 *
 * A #TeplPanelItem object is returned, which can be used as a parameter to
 * other #TeplPanel functions (#TeplPanelItem serves as a kind of handle).
 *
 * Note that #TeplPanel also owns a reference to the #TeplPanelItem. So either
 * store the #TeplPanelItem for later use, or unref it directly after calling
 * this function.
 *
 * Returns: (transfer full) (nullable): a new #TeplPanelItem object.
 * Since: 6.8
 */
TeplPanelItem *
tepl_panel_add (TeplPanel   *panel,
		GtkWidget   *widget,
		const gchar *name,
		const gchar *title,
		const gchar *icon_name)
{
	g_return_val_if_fail (TEPL_IS_PANEL (panel), NULL);
	g_return_val_if_fail (GTK_IS_WIDGET (widget), NULL);

	return TEPL_PANEL_GET_INTERFACE (panel)->add (panel,
						      widget,
						      name,
						      title,
						      icon_name);
}

/**
 * tepl_panel_remove:
 * @panel: a #TeplPanel.
 * @item: the #TeplPanelItem to remove.
 *
 * Removes an item from @panel.
 *
 * Since: 6.8
 */
void
tepl_panel_remove (TeplPanel     *panel,
		   TeplPanelItem *item)
{
	g_return_if_fail (TEPL_IS_PANEL (panel));
	g_return_if_fail (TEPL_IS_PANEL_ITEM (item));

	TEPL_PANEL_GET_INTERFACE (panel)->remove (panel, item);
}

/**
 * tepl_panel_set_active:
 * @panel: a #TeplPanel.
 * @item: a #TeplPanelItem.
 *
 * Sets @item to be the one that is currently shown in @panel.
 *
 * This is different than setting the #GtkWidget:visible property to %TRUE. All
 * #GtkWidget's of a @panel should have their #GtkWidget:visible property set to
 * %TRUE all the time, but only one #TeplPanelItem is shown at a time.
 *
 * Since: 6.8
 */
void
tepl_panel_set_active (TeplPanel     *panel,
		       TeplPanelItem *item)
{
	g_return_if_fail (TEPL_IS_PANEL (panel));
	g_return_if_fail (TEPL_IS_PANEL_ITEM (item));

	TEPL_PANEL_GET_INTERFACE (panel)->set_active (panel, item);
}
