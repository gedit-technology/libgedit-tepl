/* SPDX-FileCopyrightText: 2022-2023 - Sébastien Wilmet <swilmet@gnome.org>
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

struct _TeplWidgetListItemPrivate
{
	/* Owned. The ref is released on ::destroy. */
	GtkWidget *widget;
	gulong widget_destroy_handler_id;

	gchar *name;
	gchar *title;
	gchar *icon_name;

	guint displayed : 1;
};

/* Forward declarations */
static void set_widget (TeplWidgetListItem *item,
			GtkWidget          *widget);

G_DEFINE_TYPE_WITH_PRIVATE (TeplWidgetListItem, tepl_widget_list_item, G_TYPE_OBJECT)

static void
widget_destroy_cb (GtkWidget          *widget,
		   TeplWidgetListItem *item)
{
	set_widget (item, NULL);
}

static void
set_widget (TeplWidgetListItem *item,
	    GtkWidget          *widget)
{
	if (item->priv->widget == widget)
	{
		return;
	}

	if (item->priv->widget != NULL)
	{
		if (item->priv->widget_destroy_handler_id != 0)
		{
			g_signal_handler_disconnect (item->priv->widget, item->priv->widget_destroy_handler_id);
			item->priv->widget_destroy_handler_id = 0;
		}

		g_clear_object (&item->priv->widget);
	}

	if (widget != NULL)
	{
		item->priv->widget = g_object_ref_sink (widget);

		item->priv->widget_destroy_handler_id =
			g_signal_connect (item->priv->widget,
					  "destroy",
					  G_CALLBACK (widget_destroy_cb),
					  item);
	}
}

static void
tepl_widget_list_item_dispose (GObject *object)
{
	TeplWidgetListItem *item = TEPL_WIDGET_LIST_ITEM (object);

	set_widget (item, NULL);

	G_OBJECT_CLASS (tepl_widget_list_item_parent_class)->dispose (object);
}

static void
tepl_widget_list_item_finalize (GObject *object)
{
	TeplWidgetListItem *item = TEPL_WIDGET_LIST_ITEM (object);

	g_free (item->priv->name);
	g_free (item->priv->title);
	g_free (item->priv->icon_name);

	G_OBJECT_CLASS (tepl_widget_list_item_parent_class)->finalize (object);
}

static void
tepl_widget_list_item_class_init (TeplWidgetListItemClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->dispose = tepl_widget_list_item_dispose;
	object_class->finalize = tepl_widget_list_item_finalize;
}

static void
tepl_widget_list_item_init (TeplWidgetListItem *item)
{
	item->priv = tepl_widget_list_item_get_instance_private (item);
}

/**
 * tepl_widget_list_item_new:
 *
 * Returns: a new #TeplWidgetListItem.
 * Since: 6.4
 */
TeplWidgetListItem *
tepl_widget_list_item_new (void)
{
	return g_object_new (TEPL_TYPE_WIDGET_LIST_ITEM, NULL);
}

/**
 * tepl_widget_list_item_set_widget:
 * @item: a #TeplWidgetListItem.
 * @widget: (nullable): a #GtkWidget, or %NULL to unset.
 *
 * Sets the "widget" property.
 *
 * Since: 6.6
 */
void
tepl_widget_list_item_set_widget (TeplWidgetListItem *item,
				  GtkWidget          *widget)
{
	g_return_if_fail (TEPL_IS_WIDGET_LIST_ITEM (item));
	g_return_if_fail (widget == NULL || GTK_IS_WIDGET (widget));

	set_widget (item, widget);
}

/**
 * tepl_widget_list_item_get_widget:
 * @item: a #TeplWidgetListItem.
 *
 * Returns: (transfer none) (nullable): the "widget" property. Is %NULL when the
 *   property isn't set, or if the widget has been destroyed (see the
 *   #GtkWidget::destroy signal).
 * Since: 6.6
 */
GtkWidget *
tepl_widget_list_item_get_widget (TeplWidgetListItem *item)
{
	g_return_val_if_fail (TEPL_IS_WIDGET_LIST_ITEM (item), NULL);

	return item->priv->widget;
}

/**
 * tepl_widget_list_item_set_displayed:
 * @item: a #TeplWidgetListItem.
 * @displayed: the new value.
 *
 * Sets the "displayed" property.
 *
 * Since: 6.6
 */
void
tepl_widget_list_item_set_displayed (TeplWidgetListItem *item,
				     gboolean            displayed)
{
	g_return_if_fail (TEPL_IS_WIDGET_LIST_ITEM (item));

	item->priv->displayed = displayed != FALSE;
}

/**
 * tepl_widget_list_item_get_displayed:
 * @item: a #TeplWidgetListItem.
 *
 * Returns: the "displayed" property.
 * Since: 6.6
 */
gboolean
tepl_widget_list_item_get_displayed (TeplWidgetListItem *item)
{
	g_return_val_if_fail (TEPL_IS_WIDGET_LIST_ITEM (item), FALSE);

	return item->priv->displayed;
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
 * Since: 6.6
 */
void
tepl_widget_list_item_set_infos (TeplWidgetListItem *item,
				 const gchar        *name,
				 const gchar        *title,
				 const gchar        *icon_name)
{
	g_return_if_fail (TEPL_IS_WIDGET_LIST_ITEM (item));

	g_free (item->priv->name);
	g_free (item->priv->title);
	g_free (item->priv->icon_name);

	item->priv->name = g_strdup (name);
	item->priv->title = g_strdup (title);
	item->priv->icon_name = g_strdup (icon_name);
}

/**
 * tepl_widget_list_item_get_infos:
 * @item: a #TeplWidgetListItem.
 * @name: (out) (optional): the "name" property.
 * @title: (out) (optional): the "title" property.
 * @icon_name: (out) (optional): the "icon-name" property.
 *
 * Gets some pieces of information from @item.
 *
 * Since: 6.6
 */
void
tepl_widget_list_item_get_infos (TeplWidgetListItem  *item,
				 gchar              **name,
				 gchar              **title,
				 gchar              **icon_name)
{
	g_return_if_fail (TEPL_IS_WIDGET_LIST_ITEM (item));

	if (name != NULL)
	{
		*name = g_strdup (item->priv->name);
	}

	if (title != NULL)
	{
		*title = g_strdup (item->priv->title);
	}

	if (icon_name != NULL)
	{
		*icon_name = g_strdup (item->priv->icon_name);
	}
}
