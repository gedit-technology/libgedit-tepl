/* SPDX-FileCopyrightText: 2023-2024 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "tepl-panel-item.h"
#include "tepl-utils.h"

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

	gchar *name;
	gchar *title;
	gchar *icon_name;
	gint position;
};

enum
{
	PROP_0,
	PROP_WIDGET,
	PROP_NAME,
	PROP_TITLE,
	PROP_ICON_NAME,
	PROP_POSITION,
	N_PROPERTIES
};

static GParamSpec *properties[N_PROPERTIES];

G_DEFINE_TYPE_WITH_PRIVATE (TeplPanelItem, tepl_panel_item, G_TYPE_OBJECT)

static gboolean
is_null_or_valid_utf8 (const gchar *str)
{
	return (str == NULL || g_utf8_validate (str, -1, NULL));
}

static void
set_name (TeplPanelItem *item,
	  const gchar   *name)
{
	g_return_if_fail (name != NULL);
	g_return_if_fail (g_utf8_validate (name, -1, NULL));

	g_set_str (&item->priv->name, name);
}

static void
set_title (TeplPanelItem *item,
	   const gchar   *title)
{
	g_return_if_fail (is_null_or_valid_utf8 (title));
	g_set_str (&item->priv->title, title);
}

static void
tepl_panel_item_get_property (GObject    *object,
			      guint       prop_id,
			      GValue     *value,
			      GParamSpec *pspec)
{
	TeplPanelItem *item = TEPL_PANEL_ITEM (object);

	switch (prop_id)
	{
		case PROP_WIDGET:
			g_value_set_object (value, tepl_panel_item_get_widget (item));
			break;

		case PROP_NAME:
			g_value_set_string (value, tepl_panel_item_get_name (item));
			break;

		case PROP_TITLE:
			g_value_set_string (value, tepl_panel_item_get_title (item));
			break;

		case PROP_ICON_NAME:
			g_value_set_string (value, tepl_panel_item_get_icon_name (item));
			break;

		case PROP_POSITION:
			g_value_set_int (value, tepl_panel_item_get_position (item));
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
tepl_panel_item_set_property (GObject      *object,
			      guint         prop_id,
			      const GValue *value,
			      GParamSpec   *pspec)
{
	TeplPanelItem *item = TEPL_PANEL_ITEM (object);

	switch (prop_id)
	{
		case PROP_WIDGET:
			tepl_utils_set_widget (&item->priv->widget, g_value_get_object (value));
			break;

		case PROP_NAME:
			set_name (item, g_value_get_string (value));
			break;

		case PROP_TITLE:
			set_title (item, g_value_get_string (value));
			break;

		case PROP_ICON_NAME:
			g_set_str (&item->priv->icon_name, g_value_get_string (value));
			break;

		case PROP_POSITION:
			item->priv->position = g_value_get_int (value);
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
tepl_panel_item_dispose (GObject *object)
{
	TeplPanelItem *item = TEPL_PANEL_ITEM (object);

	tepl_utils_set_widget (&item->priv->widget, NULL);

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

	object_class->get_property = tepl_panel_item_get_property;
	object_class->set_property = tepl_panel_item_set_property;
	object_class->dispose = tepl_panel_item_dispose;
	object_class->finalize = tepl_panel_item_finalize;

	/**
	 * TeplPanelItem:widget:
	 *
	 * The #GtkWidget (the main content).
	 *
	 * Since: 6.11
	 */
	properties[PROP_WIDGET] =
		g_param_spec_object ("widget",
				     "widget",
				     "",
				     GTK_TYPE_WIDGET,
				     G_PARAM_READWRITE |
				     G_PARAM_CONSTRUCT_ONLY |
				     G_PARAM_STATIC_STRINGS);

	/**
	 * TeplPanelItem:name:
	 *
	 * The name.
	 *
	 * It is an ID as a UTF-8 string. It is not displayed in the UI. It
	 * uniquely identifies an item within a list.
	 *
	 * Since: 6.11
	 */
	properties[PROP_NAME] =
		g_param_spec_string ("name",
				     "name",
				     "",
				     NULL,
				     G_PARAM_READWRITE |
				     G_PARAM_CONSTRUCT_ONLY |
				     G_PARAM_STATIC_STRINGS);

	/**
	 * TeplPanelItem:title:
	 *
	 * The title.
	 *
	 * It is a human-readable UTF-8 string that can be shown in the UI to
	 * choose this item.
	 *
	 * Since: 6.11
	 */
	properties[PROP_TITLE] =
		g_param_spec_string ("title",
				     "title",
				     "",
				     NULL,
				     G_PARAM_READWRITE |
				     G_PARAM_CONSTRUCT_ONLY |
				     G_PARAM_STATIC_STRINGS);

	/**
	 * TeplPanelItem:icon-name:
	 *
	 * The icon name.
	 *
	 * It can be shown in the UI to choose this item, for example with the
	 * #GtkImage's #GtkImage:icon-name property.
	 *
	 * Since: 6.11
	 */
	properties[PROP_ICON_NAME] =
		g_param_spec_string ("icon-name",
				     "icon-name",
				     "",
				     NULL,
				     G_PARAM_READWRITE |
				     G_PARAM_CONSTRUCT_ONLY |
				     G_PARAM_STATIC_STRINGS);

	/**
	 * TeplPanelItem:position:
	 *
	 * The position.
	 *
	 * Used to sort items in the desired order.
	 *
	 * Since: 6.11
	 */
	properties[PROP_POSITION] =
		g_param_spec_int ("position",
				  "position",
				  "",
				  G_MININT, G_MAXINT, 0,
				  G_PARAM_READWRITE |
				  G_PARAM_CONSTRUCT_ONLY |
				  G_PARAM_STATIC_STRINGS);

	g_object_class_install_properties (object_class, N_PROPERTIES, properties);
}

static void
tepl_panel_item_init (TeplPanelItem *item)
{
	item->priv = tepl_panel_item_get_instance_private (item);
}

/**
 * tepl_panel_item_new:
 * @widget: a #GtkWidget.
 * @name: the name.
 * @title: (nullable): the title.
 * @icon_name: (nullable): the icon-name.
 *
 * Creates a new #TeplPanelItem object.
 *
 * See the properties for the meaning of the parameters.
 *
 * Returns: (transfer full): a new #TeplPanelItem object.
 * Since: 6.11
 */
TeplPanelItem *
tepl_panel_item_new (GtkWidget   *widget,
		     const gchar *name,
		     const gchar *title,
		     const gchar *icon_name)
{
	g_return_val_if_fail (GTK_IS_WIDGET (widget), NULL);
	g_return_val_if_fail (name != NULL, NULL);

	return g_object_new (TEPL_TYPE_PANEL_ITEM,
			     "widget", widget,
			     "name", name,
			     "title", title,
			     "icon-name", icon_name,
			     NULL);
}

/**
 * tepl_panel_item_get_widget:
 * @item: a #TeplPanelItem.
 *
 * Returns: (transfer none) (nullable): the value of the #TeplPanelItem:widget
 *   property.
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
 * Returns: the value of the #TeplPanelItem:name property.
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
 * Returns: (nullable): the value of the #TeplPanelItem:title property.
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
 * Returns: (nullable): the value of the #TeplPanelItem:icon-name property.
 * Since: 6.8
 */
const gchar *
tepl_panel_item_get_icon_name (TeplPanelItem *item)
{
	g_return_val_if_fail (TEPL_IS_PANEL_ITEM (item), NULL);
	return item->priv->icon_name;
}

/**
 * tepl_panel_item_get_position:
 * @item: a #TeplPanelItem.
 *
 * Returns: the value of the #TeplPanelItem:position property.
 * Since: 6.11
 */
gint
tepl_panel_item_get_position (TeplPanelItem *item)
{
	g_return_val_if_fail (TEPL_IS_PANEL_ITEM (item), 0);
	return item->priv->position;
}

/**
 * tepl_panel_item_compare_by_title:
 * @a: a #TeplPanelItem.
 * @b: a #TeplPanelItem.
 *
 * A #GCompareFunc that takes into account only the #TeplPanelItem:title
 * property.
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

/**
 * tepl_panel_item_compare:
 * @a: a #TeplPanelItem.
 * @b: a #TeplPanelItem.
 *
 * A #GCompareFunc that takes into account the #TeplPanelItem:position property
 * in priority, and falls back to the #TeplPanelItem:title property if two items
 * have the same position.
 *
 * Returns: the usual return value for a #GCompareFunc.
 * Since: 6.11
 */
gint
tepl_panel_item_compare (TeplPanelItem *a,
			 TeplPanelItem *b)
{
	g_return_val_if_fail (TEPL_IS_PANEL_ITEM (a), 0);
	g_return_val_if_fail (TEPL_IS_PANEL_ITEM (b), 0);

	if (a->priv->position == b->priv->position)
	{
		return tepl_panel_item_compare_by_title (a, b);
	}

	return a->priv->position - b->priv->position;
}
