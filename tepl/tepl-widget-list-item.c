/* SPDX-FileCopyrightText: 2022 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "tepl-widget-list-item.h"

/**
 * SECTION:widget-list-item
 * @Short_description: Contains a #GtkWidget plus additional infos
 * @Title: TeplWidgetListItem
 *
 * #TeplWidgetListItem contains a #GtkWidget plus additional pieces of
 * information.
 *
 * All the values are controlled by the owner of the #TeplWidgetListItem object.
 * The only exception is when receiving the #GtkWidget::destroy signal of the
 * currently associated widget, the "widget" property becomes %NULL.
 *
 * # The semantics of the different properties
 *
 * - "widget": The main content, as a #GtkWidget.
 *
 * - "displayed": Whether the "widget" is currently displayed in the UI.
 *
 * - "name": An ID as a string. Not displayed in the UI. Uniquely identifies an
 *   item within a list.
 *
 * - "title": A human-readable title that can be shown in the UI to choose this
 *   item.
 *
 * - "icon-name": An icon-name representing this item, can be shown in the UI to
 *   choose this item. Can be used for #GtkImage:icon-name.
 *
 * # Relation to #GtkStack
 *
 * #TeplWidgetListItem is similar to the #GtkStack child properties. Some
 * differences:
 * - This class is more abstract. It can be used in combination with #GtkStack
 *   but also other types of #GtkContainer's.
 * - This class contains the "displayed" property, which permits to have several
 *   items displayed at the same time. Use-cases: split-views, or showing
 *   several documents side-by-side, or showing several components in a side
 *   panel, etc.
 * - This class has maximum flexibility with regards to the #GtkWidget: the
 *   usual case is to set it just after calling tepl_widget_list_item_new(), and
 *   to show/hide it when displayed/undisplayed. But lazy initialization is
 *   possible too, or destroying/re-creating the widget when needed, while still
 *   having the other infos available and the item present in a list.
 * - #GtkStack emits signals when child properties change, while this class
 *   defer the responsibility of updating the state of other objects to the
 *   owner of the #TeplWidgetListItem.
 */

struct _TeplWidgetListItem
{
	/* Owned. The ref is released on ::destroy. */
	GtkWidget *widget;
	gulong widget_destroy_handler_id;

	gchar *name;
	gchar *title;
	gchar *icon_name;

	guint displayed : 1;
};

/**
 * tepl_widget_list_item_new: (skip)
 *
 * Returns: a new #TeplWidgetListItem. Free with tepl_widget_list_item_free().
 * Since: 6.4
 */
TeplWidgetListItem *
tepl_widget_list_item_new (void)
{
	return g_new0 (TeplWidgetListItem, 1);
}

/**
 * tepl_widget_list_item_free:
 * @item: (nullable): the #TeplWidgetListItem to free.
 *
 * Since: 6.4
 */
void
tepl_widget_list_item_free (TeplWidgetListItem *item)
{
	if (item != NULL)
	{
		tepl_widget_list_item_set_widget (item, NULL);

		g_free (item->name);
		g_free (item->title);
		g_free (item->icon_name);

		g_free (item);
	}
}

static void
widget_destroy_cb (GtkWidget          *widget,
		   TeplWidgetListItem *item)
{
	tepl_widget_list_item_set_widget (item, NULL);
}

/**
 * tepl_widget_list_item_set_widget:
 * @item: a #TeplWidgetListItem.
 * @widget: (nullable): a #GtkWidget, or %NULL to unset.
 *
 * Sets the "widget" property.
 *
 * Since: 6.4
 */
void
tepl_widget_list_item_set_widget (TeplWidgetListItem *item,
				  GtkWidget          *widget)
{
	g_return_if_fail (item != NULL);
	g_return_if_fail (widget == NULL || GTK_IS_WIDGET (widget));

	if (item->widget == widget)
	{
		return;
	}

	if (item->widget != NULL)
	{
		if (item->widget_destroy_handler_id != 0)
		{
			g_signal_handler_disconnect (item->widget, item->widget_destroy_handler_id);
			item->widget_destroy_handler_id = 0;
		}

		g_clear_object (&item->widget);
	}

	if (widget != NULL)
	{
		item->widget = g_object_ref_sink (widget);

		item->widget_destroy_handler_id =
			g_signal_connect (item->widget,
					  "destroy",
					  G_CALLBACK (widget_destroy_cb),
					  item);
	}
}

/**
 * tepl_widget_list_item_get_widget:
 * @item: a #TeplWidgetListItem.
 *
 * Returns: (transfer none) (nullable): the "widget" property. Is %NULL when the
 *   property isn't set, or if the widget has been destroyed (see the
 *   #GtkWidget::destroy signal).
 * Since: 6.4
 */
GtkWidget *
tepl_widget_list_item_get_widget (const TeplWidgetListItem *item)
{
	g_return_val_if_fail (item != NULL, NULL);

	return item->widget;
}

/**
 * tepl_widget_list_item_set_displayed:
 * @item: a #TeplWidgetListItem.
 * @displayed: the new value.
 *
 * Sets the "displayed" property.
 *
 * Since: 6.4
 */
void
tepl_widget_list_item_set_displayed (TeplWidgetListItem *item,
				     gboolean            displayed)
{
	g_return_if_fail (item != NULL);

	item->displayed = displayed != FALSE;
}

/**
 * tepl_widget_list_item_get_displayed:
 * @item: a #TeplWidgetListItem.
 *
 * Returns: the "displayed" property.
 * Since: 6.4
 */
gboolean
tepl_widget_list_item_get_displayed (const TeplWidgetListItem *item)
{
	g_return_val_if_fail (item != NULL, FALSE);

	return item->displayed;
}

/**
 * tepl_widget_list_item_set_infos:
 * @item: a #TeplWidgetListItem.
 * @name: (nullable): for the "name" property.
 * @title: (nullable): for the "title" property.
 * @icon_name: (nullable): for the "icon-name" property.
 *
 * Sets some pieces of information to @item.
 *
 * Since: 6.4
 */
void
tepl_widget_list_item_set_infos (TeplWidgetListItem *item,
				 const gchar        *name,
				 const gchar        *title,
				 const gchar        *icon_name)
{
	g_return_if_fail (item != NULL);

	g_free (item->name);
	g_free (item->title);
	g_free (item->icon_name);

	item->name = g_strdup (name);
	item->title = g_strdup (title);
	item->icon_name = g_strdup (icon_name);
}

/**
 * tepl_widget_list_item_get_infos:
 * @item: a #TeplWidgetListItem.
 * @name: (out) (transfer none) (optional): the "name" property.
 * @title: (out) (transfer none) (optional): the "title" property.
 * @icon_name: (out) (transfer none) (optional): the "icon-name" property.
 *
 * Gets some pieces of information from @item.
 *
 * Since: 6.4
 */
void
tepl_widget_list_item_get_infos (const TeplWidgetListItem  *item,
				 const gchar              **name,
				 const gchar              **title,
				 const gchar              **icon_name)
{
	g_return_if_fail (item != NULL);

	if (name != NULL)
	{
		*name = item->name;
	}

	if (title != NULL)
	{
		*title = item->title;
	}

	if (icon_name != NULL)
	{
		*icon_name = item->icon_name;
	}
}
