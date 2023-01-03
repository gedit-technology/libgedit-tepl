/* SPDX-FileCopyrightText: 2022 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "tepl-widget-list-category.h"

/**
 * SECTION:widget-list-category
 * @Short_description: A category of #TeplWidgetListItem's
 * @Title: TeplWidgetListCategory
 *
 * This API is in a draft state and is incomplete.
 *
 * A #TeplWidgetListCategory object contains:
 * - A name.
 * - A list of #TeplWidgetListItem's.
 *
 * The main idea is to put several #TeplWidgetListItem's in a list. The usual
 * case is to have a single list, with %TEPL_WIDGET_LIST_CATEGORY_DEFAULT. But
 * it's possible to categorize the items, for example to distinguish between
 * core components (always there) and plugins components, with
 * %TEPL_WIDGET_LIST_CATEGORY_CORE and %TEPL_WIDGET_LIST_CATEGORY_PLUGINS. But
 * the name of a category is a simple string, so other schemes are possible too.
 *
 * The use-case for distinguishing between core components and plugins
 * components is to clearly show in the UI that it comes from a plugin. For
 * sorting the items too: first show core components (sorted alphabetically by
 * title for example), then plugins components. If there are no plugins
 * components, show an empty state icon.
 */

struct _TeplWidgetListCategory
{
	gchar *name;

	/* Element types: owned TeplWidgetListItem* */
	GList *items;
};

/**
 * tepl_widget_list_category_new: (skip)
 * @name: the category name.
 *
 * Returns: (transfer full): a new #TeplWidgetListCategory.
 * Since: 6.4
 */
TeplWidgetListCategory *
tepl_widget_list_category_new (const gchar *name)
{
	TeplWidgetListCategory *category;

	g_return_val_if_fail (name != NULL, NULL);

	category = g_new0 (TeplWidgetListCategory, 1);
	category->name = g_strdup (name);

	return category;
}

/**
 * tepl_widget_list_category_free:
 * @category: (nullable): a #TeplWidgetListCategory.
 *
 * Since: 6.4
 */
void
tepl_widget_list_category_free (TeplWidgetListCategory *category)
{
	if (category != NULL)
	{
		g_free (category->name);
		g_list_free_full (category->items, g_object_unref);
		g_free (category);
	}
}

/**
 * tepl_widget_list_category_get_name:
 * @category: a #TeplWidgetListCategory.
 *
 * Returns: the name of @category.
 * Since: 6.4
 */
const gchar *
tepl_widget_list_category_get_name (const TeplWidgetListCategory *category)
{
	g_return_val_if_fail (category != NULL, NULL);

	return category->name;
}

/**
 * tepl_widget_list_category_add_item:
 * @category: a #TeplWidgetListCategory.
 * @item: an item to add to @category.
 *
 * Since: 6.4
 */
void
tepl_widget_list_category_add_item (TeplWidgetListCategory *category,
				    TeplWidgetListItem     *item)
{
	g_return_if_fail (category != NULL);
	g_return_if_fail (TEPL_IS_WIDGET_LIST_ITEM (item));

	category->items = g_list_prepend (category->items, g_object_ref (item));
}

/**
 * tepl_widget_list_category_get_all:
 * @category: a #TeplWidgetListCategory.
 *
 * Returns: (transfer none) (element-type TeplWidgetListItem): the list of items
 *   that are part of @category.
 * Since: 6.4
 */
GList *
tepl_widget_list_category_get_all (TeplWidgetListCategory *category)
{
	g_return_val_if_fail (category != NULL, NULL);

	return category->items;
}
