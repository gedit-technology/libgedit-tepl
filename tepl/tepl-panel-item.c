/* SPDX-FileCopyrightText: 2023 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "tepl-panel-item.h"

/**
 * SECTION:panel-item
 * @Title: TeplPanelItem
 * @Short_description: Access to the information of a panel item
 *
 * #TeplPanelItem is an interface to get the information of a panel item.
 */

G_DEFINE_INTERFACE (TeplPanelItem, tepl_panel_item, G_TYPE_OBJECT)

static GtkWidget *
tepl_panel_item_get_widget_default (TeplPanelItem *item)
{
	return NULL;
}

static const gchar *
tepl_panel_item_get_name_default (TeplPanelItem *item)
{
	return NULL;
}

static const gchar *
tepl_panel_item_get_title_default (TeplPanelItem *item)
{
	return NULL;
}

static const gchar *
tepl_panel_item_get_icon_name_default (TeplPanelItem *item)
{
	return NULL;
}

static void
tepl_panel_item_default_init (TeplPanelItemInterface *interface)
{
	interface->get_widget = tepl_panel_item_get_widget_default;
	interface->get_name = tepl_panel_item_get_name_default;
	interface->get_title = tepl_panel_item_get_title_default;
	interface->get_icon_name = tepl_panel_item_get_icon_name_default;
}

/**
 * tepl_panel_item_get_widget:
 * @item: a #TeplPanelItem.
 *
 * Gets the #GtkWidget (the main content).
 *
 * Returns: (transfer none) (nullable): the "widget" attribute. Is %NULL when
 *   the widget has been destroyed.
 * Since: 6.8
 */
GtkWidget *
tepl_panel_item_get_widget (TeplPanelItem *item)
{
	g_return_val_if_fail (TEPL_IS_PANEL_ITEM (item), NULL);

	return TEPL_PANEL_ITEM_GET_INTERFACE (item)->get_widget (item);
}

/**
 * tepl_panel_item_get_name:
 * @item: a #TeplPanelItem.
 *
 * Gets the name. It is an ID as a UTF-8 string. It is not displayed in the UI.
 * It uniquely identifies an item within a list.
 *
 * Returns: (nullable): the "name" attribute.
 * Since: 6.8
 */
const gchar *
tepl_panel_item_get_name (TeplPanelItem *item)
{
	g_return_val_if_fail (TEPL_IS_PANEL_ITEM (item), NULL);

	return TEPL_PANEL_ITEM_GET_INTERFACE (item)->get_name (item);
}

/**
 * tepl_panel_item_get_title:
 * @item: a #TeplPanelItem.
 *
 * Gets the title. It is a human-readable UTF-8 string that can be shown in the
 * UI to choose this item.
 *
 * Returns: (nullable): the "title" attribute.
 * Since: 6.8
 */
const gchar *
tepl_panel_item_get_title (TeplPanelItem *item)
{
	g_return_val_if_fail (TEPL_IS_PANEL_ITEM (item), NULL);

	return TEPL_PANEL_ITEM_GET_INTERFACE (item)->get_title (item);
}

/**
 * tepl_panel_item_get_icon_name:
 * @item: a #TeplPanelItem.
 *
 * Gets the icon name. It can be shown in the UI to choose this item, for
 * example with the #GtkImage's #GtkImage:icon-name property.
 *
 * Returns: (nullable): the "icon-name" attribute.
 * Since: 6.8
 */
const gchar *
tepl_panel_item_get_icon_name (TeplPanelItem *item)
{
	g_return_val_if_fail (TEPL_IS_PANEL_ITEM (item), NULL);

	return TEPL_PANEL_ITEM_GET_INTERFACE (item)->get_icon_name (item);
}

/**
 * tepl_panel_item_compare_by_title:
 * @a: a #TeplPanelItem.
 * @b: a #TeplPanelItem.
 *
 * A #GCompareFunc for the #TeplPanelItem's "title" attribute.
 *
 * Returns: the usual return value for a #GCompareFunc.
 * Since: 6.8
 */
gint
tepl_panel_item_compare_by_title (TeplPanelItem *a,
				  TeplPanelItem *b)
{
	const gchar *title_a;
	const gchar *title_b;
	gchar *normalized_title_a = NULL;
	gchar *normalized_title_b = NULL;
	gint result = 0;

	g_return_val_if_fail (TEPL_IS_PANEL_ITEM (a), 0);
	g_return_val_if_fail (TEPL_IS_PANEL_ITEM (b), 0);

	title_a = tepl_panel_item_get_title (a);
	title_b = tepl_panel_item_get_title (b);

	/* Put untitled items at the end */

	if (title_a == NULL && title_b == NULL)
	{
		result = 0;
		goto out;
	}
	if (title_a == NULL)
	{
		result = 1;
		goto out;
	}
	if (title_b == NULL)
	{
		result = -1;
		goto out;
	}

	/* UTF-8 comparison */

	normalized_title_a = g_utf8_normalize (title_a, -1, G_NORMALIZE_ALL);
	normalized_title_b = g_utf8_normalize (title_b, -1, G_NORMALIZE_ALL);

	result = g_utf8_collate (normalized_title_a, normalized_title_b);

out:
	g_free (normalized_title_a);
	g_free (normalized_title_b);
	return result;
}
