/* SPDX-FileCopyrightText: 2023 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "tepl-panel-item-simple.h"

struct _TeplPanelItemSimplePrivate
{
	/* Owned. The ref is released on ::destroy. */
	GtkWidget *widget;
	gulong widget_destroy_handler_id;

	gchar *name;
	gchar *title;
	gchar *icon_name;
};

/* Forward declarations */
static void set_widget (TeplPanelItemSimple *item,
			GtkWidget           *widget);

static void tepl_panel_item_interface_init (gpointer g_iface,
					    gpointer iface_data);

G_DEFINE_TYPE_WITH_CODE (TeplPanelItemSimple,
			 _tepl_panel_item_simple,
			 G_TYPE_OBJECT,
			 G_ADD_PRIVATE (TeplPanelItemSimple)
			 G_IMPLEMENT_INTERFACE (TEPL_TYPE_PANEL_ITEM,
						tepl_panel_item_interface_init))

static gboolean
is_null_or_valid_utf8 (const gchar *str)
{
	return (str == NULL || g_utf8_validate (str, -1, NULL));
}

static void
widget_destroy_cb (GtkWidget           *widget,
		   TeplPanelItemSimple *item)
{
	set_widget (item, NULL);
}

static void
set_widget (TeplPanelItemSimple *item,
	    GtkWidget           *widget)
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
_tepl_panel_item_simple_dispose (GObject *object)
{
	TeplPanelItemSimple *item = TEPL_PANEL_ITEM_SIMPLE (object);

	set_widget (item, NULL);

	G_OBJECT_CLASS (_tepl_panel_item_simple_parent_class)->dispose (object);
}

static void
_tepl_panel_item_simple_finalize (GObject *object)
{
	TeplPanelItemSimple *item = TEPL_PANEL_ITEM_SIMPLE (object);

	g_free (item->priv->name);
	g_free (item->priv->title);
	g_free (item->priv->icon_name);

	G_OBJECT_CLASS (_tepl_panel_item_simple_parent_class)->finalize (object);
}

static void
_tepl_panel_item_simple_class_init (TeplPanelItemSimpleClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->dispose = _tepl_panel_item_simple_dispose;
	object_class->finalize = _tepl_panel_item_simple_finalize;
}

static void
_tepl_panel_item_simple_init (TeplPanelItemSimple *item)
{
	item->priv = _tepl_panel_item_simple_get_instance_private (item);
}

static GtkWidget *
tepl_panel_item_simple_get_widget (TeplPanelItem *item)
{
	TeplPanelItemSimple *item_simple = TEPL_PANEL_ITEM_SIMPLE (item);

	return item_simple->priv->widget;
}

static const gchar *
tepl_panel_item_simple_get_name (TeplPanelItem *item)
{
	TeplPanelItemSimple *item_simple = TEPL_PANEL_ITEM_SIMPLE (item);

	return item_simple->priv->name;
}

static const gchar *
tepl_panel_item_simple_get_title (TeplPanelItem *item)
{
	TeplPanelItemSimple *item_simple = TEPL_PANEL_ITEM_SIMPLE (item);

	return item_simple->priv->title;
}

static const gchar *
tepl_panel_item_simple_get_icon_name (TeplPanelItem *item)
{
	TeplPanelItemSimple *item_simple = TEPL_PANEL_ITEM_SIMPLE (item);

	return item_simple->priv->icon_name;
}

static void
tepl_panel_item_interface_init (gpointer g_iface,
				gpointer iface_data)
{
	TeplPanelItemInterface *interface = g_iface;

	interface->get_widget = tepl_panel_item_simple_get_widget;
	interface->get_name = tepl_panel_item_simple_get_name;
	interface->get_title = tepl_panel_item_simple_get_title;
	interface->get_icon_name = tepl_panel_item_simple_get_icon_name;
}

TeplPanelItem *
_tepl_panel_item_simple_new (GtkWidget   *widget,
			     const gchar *name,
			     const gchar *title,
			     const gchar *icon_name)
{
	TeplPanelItemSimple *item;

	g_return_val_if_fail (GTK_IS_WIDGET (widget), NULL);
	g_return_val_if_fail (is_null_or_valid_utf8 (name), NULL);
	g_return_val_if_fail (is_null_or_valid_utf8 (title), NULL);

	item = g_object_new (TEPL_TYPE_PANEL_ITEM_SIMPLE, NULL);

	set_widget (item, widget);

	item->priv->name = g_strdup (name);
	item->priv->title = g_strdup (title);
	item->priv->icon_name = g_strdup (icon_name);

	return TEPL_PANEL_ITEM (item);
}
