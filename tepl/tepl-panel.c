/* SPDX-FileCopyrightText: 2023-2024 - Sébastien Wilmet
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

static void
tepl_panel_add_default (TeplPanel     *panel,
			TeplPanelItem *item)
{
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
 * @item: a #TeplPanelItem.
 *
 * Adds an item to the panel.
 *
 * Since: 6.12
 */
void
tepl_panel_add (TeplPanel     *panel,
		TeplPanelItem *item)
{
	g_return_if_fail (TEPL_IS_PANEL (panel));
	g_return_if_fail (TEPL_IS_PANEL_ITEM (item));

	return TEPL_PANEL_GET_INTERFACE (panel)->add (panel, item);
}

/**
 * tepl_panel_remove:
 * @panel: a #TeplPanel.
 * @item: the #TeplPanelItem to remove.
 *
 * Removes an item from the panel.
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
 * Only one #TeplPanelItem is shown at a time in a #TeplPanel.
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
