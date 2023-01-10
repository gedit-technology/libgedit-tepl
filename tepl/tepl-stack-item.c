/* SPDX-FileCopyrightText: 2023 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "tepl-stack-item.h"

/**
 * SECTION:stack-item
 * @Short_description: Contains a #GtkWidget plus additional infos
 * @Title: TeplStackItem
 *
 * #TeplStackItem contains a #GtkWidget plus additional pieces of information.
 *
 * The semantics of the different attributes:
 *
 * - "widget": The main content, as a #GtkWidget.
 *
 * - "name": An ID as a UTF-8 string. Not displayed in the UI. Uniquely
 *   identifies an item within a list.
 *
 * - "title": A human-readable title that can be shown in the UI to choose this
 *   item (a UTF-8 string).
 *
 * - "icon-name": An icon-name representing this item, can be shown in the UI to
 *   choose this item. Can be used for #GtkImage:icon-name.
 */

struct _TeplStackItemPrivate
{
	/* Owned. The ref is released on ::destroy. */
	GtkWidget *widget;
	gulong widget_destroy_handler_id;

	gchar *name;
	gchar *title;
	gchar *icon_name;
};

/* Forward declarations */
static void set_widget (TeplStackItem *item,
			GtkWidget     *widget);

G_DEFINE_TYPE_WITH_PRIVATE (TeplStackItem, tepl_stack_item, G_TYPE_OBJECT)

static gboolean
is_null_or_valid_utf8 (const gchar *str)
{
	return (str == NULL || g_utf8_validate (str, -1, NULL));
}

static void
widget_destroy_cb (GtkWidget     *widget,
		   TeplStackItem *item)
{
	set_widget (item, NULL);
}

static void
set_widget (TeplStackItem *item,
	    GtkWidget     *widget)
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
tepl_stack_item_dispose (GObject *object)
{
	TeplStackItem *item = TEPL_STACK_ITEM (object);

	set_widget (item, NULL);

	G_OBJECT_CLASS (tepl_stack_item_parent_class)->dispose (object);
}

static void
tepl_stack_item_finalize (GObject *object)
{
	TeplStackItem *item = TEPL_STACK_ITEM (object);

	g_free (item->priv->name);
	g_free (item->priv->title);
	g_free (item->priv->icon_name);

	G_OBJECT_CLASS (tepl_stack_item_parent_class)->finalize (object);
}

static void
tepl_stack_item_class_init (TeplStackItemClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->dispose = tepl_stack_item_dispose;
	object_class->finalize = tepl_stack_item_finalize;
}

static void
tepl_stack_item_init (TeplStackItem *item)
{
	item->priv = tepl_stack_item_get_instance_private (item);
}

/**
 * tepl_stack_item_new:
 * @widget: for the "widget" attribute.
 * @name: (nullable): for the "name" attribute.
 * @title: (nullable): for the "title" attribute.
 * @icon_name: (nullable): for the "icon-name" attribute.
 *
 * Returns: a new #TeplStackItem.
 * Since: 6.6
 */
TeplStackItem *
tepl_stack_item_new (GtkWidget   *widget,
		     const gchar *name,
		     const gchar *title,
		     const gchar *icon_name)
{
	TeplStackItem *item;

	g_return_val_if_fail (GTK_IS_WIDGET (widget), NULL);
	g_return_val_if_fail (is_null_or_valid_utf8 (name), NULL);
	g_return_val_if_fail (is_null_or_valid_utf8 (title), NULL);

	item = g_object_new (TEPL_TYPE_STACK_ITEM, NULL);

	set_widget (item, widget);

	item->priv->name = g_strdup (name);
	item->priv->title = g_strdup (title);
	item->priv->icon_name = g_strdup (icon_name);

	return item;
}

/**
 * tepl_stack_item_get_widget:
 * @item: a #TeplStackItem.
 *
 * Returns: (transfer none) (nullable): the "widget" attribute. Is %NULL when
 *   the widget has been destroyed.
 * Since: 6.6
 */
GtkWidget *
tepl_stack_item_get_widget (TeplStackItem *item)
{
	g_return_val_if_fail (TEPL_IS_STACK_ITEM (item), NULL);

	return item->priv->widget;
}

/**
 * tepl_stack_item_get_infos:
 * @item: a #TeplStackItem.
 * @name: (out) (optional) (nullable): the "name" attribute.
 * @title: (out) (optional) (nullable): the "title" attribute.
 * @icon_name: (out) (optional) (nullable): the "icon-name" attribute.
 *
 * Gets some pieces of information from @item.
 *
 * Since: 6.6
 */
void
tepl_stack_item_get_infos (TeplStackItem  *item,
			   gchar         **name,
			   gchar         **title,
			   gchar         **icon_name)
{
	g_return_if_fail (TEPL_IS_STACK_ITEM (item));

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
