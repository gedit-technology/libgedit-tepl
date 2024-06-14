/* SPDX-FileCopyrightText: 2023-2024 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "tepl-panel-item.h"

/**
 * SECTION:panel-item
 * @Title: TeplPanelItem
 * @Short_description: Panel item data
 *
 * A #TeplPanelItem object contains a #GtkWidget plus associated information.
 */

struct _TeplPanelItemPrivate
{
	/* Owned. The ref is released on ::destroy. */
	GtkWidget *widget;
	gulong widget_destroy_handler_id;

	gchar *name;
	gchar *title;
	gchar *icon_name;
};

/* Forward declarations */
static void set_widget (TeplPanelItem *item,
			GtkWidget     *widget);

G_DEFINE_TYPE_WITH_PRIVATE (TeplPanelItem, tepl_panel_item, G_TYPE_OBJECT)

static gboolean
is_null_or_valid_utf8 (const gchar *str)
{
	return (str == NULL || g_utf8_validate (str, -1, NULL));
}

static void
widget_destroy_cb (GtkWidget     *widget,
		   TeplPanelItem *item)
{
	set_widget (item, NULL);
}

static void
set_widget (TeplPanelItem *item,
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
tepl_panel_item_dispose (GObject *object)
{
	TeplPanelItem *item = TEPL_PANEL_ITEM (object);

	set_widget (item, NULL);

	G_OBJECT_CLASS (tepl_panel_item_parent_class)->dispose (object);
}

static void
tepl_panel_item_finalize (GObject *object)
{
	TeplPanelItem *item = TEPL_PANEL_ITEM (object);

	g_free (item->priv->name);
	g_free (item->priv->title);
	g_free (item->priv->icon_name);

	G_OBJECT_CLASS (tepl_panel_item_parent_class)->finalize (object);
}

static void
tepl_panel_item_class_init (TeplPanelItemClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->dispose = tepl_panel_item_dispose;
	object_class->finalize = tepl_panel_item_finalize;
}

static void
tepl_panel_item_init (TeplPanelItem *item)
{
	item->priv = tepl_panel_item_get_instance_private (item);
}

/**
 * tepl_panel_item_new:
 * @widget: a #GtkWidget.
 * @name: (nullable): the name.
 * @title: (nullable): the title.
 * @icon_name: (nullable): the icon-name.
 *
 * Creates a new #TeplPanelItem object.
 *
 * See the other #TeplPanelItem functions for the meaning of the parameters.
 *
 * Returns: (transfer full): a new #TeplPanelItem object.
 * Since: 6.12
 */
TeplPanelItem *
tepl_panel_item_new (GtkWidget   *widget,
		     const gchar *name,
		     const gchar *title,
		     const gchar *icon_name)
{
	TeplPanelItem *item;

	g_return_val_if_fail (GTK_IS_WIDGET (widget), NULL);
	g_return_val_if_fail (is_null_or_valid_utf8 (name), NULL);
	g_return_val_if_fail (is_null_or_valid_utf8 (title), NULL);

	item = g_object_new (TEPL_TYPE_PANEL_ITEM, NULL);

	set_widget (item, widget);

	item->priv->name = g_strdup (name);
	item->priv->title = g_strdup (title);
	item->priv->icon_name = g_strdup (icon_name);

	return item;
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
	return item->priv->widget;
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
	return item->priv->name;
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
	return item->priv->title;
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
	return item->priv->icon_name;
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
