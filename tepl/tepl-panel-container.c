/* SPDX-FileCopyrightText: 2023 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "tepl-panel-container.h"
#include "tepl-panel.h"
#include "tepl-panel-item-simple.h"

/**
 * SECTION:panel-container
 * @Title: TeplPanelContainer
 * @Short_description: Side or bottom panel container
 */

struct _TeplPanelContainerPrivate
{
	GtkStack *stack;

	/* Element type: owned TeplPanelItem */
	GList *items;
};

static void tepl_panel_interface_init (gpointer g_iface,
				       gpointer iface_data);

G_DEFINE_TYPE_WITH_CODE (TeplPanelContainer,
			 tepl_panel_container,
			 GTK_TYPE_BIN,
			 G_ADD_PRIVATE (TeplPanelContainer)
			 G_IMPLEMENT_INTERFACE (TEPL_TYPE_PANEL,
						tepl_panel_interface_init))

static void
tepl_panel_container_dispose (GObject *object)
{
	TeplPanelContainer *container = TEPL_PANEL_CONTAINER (object);

	container->priv->stack = NULL;

	g_list_free_full (container->priv->items, g_object_unref);
	container->priv->items = NULL;

	G_OBJECT_CLASS (tepl_panel_container_parent_class)->dispose (object);
}

static void
tepl_panel_container_class_init (TeplPanelContainerClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->dispose = tepl_panel_container_dispose;
}

static void
tepl_panel_container_init (TeplPanelContainer *container)
{
	container->priv = tepl_panel_container_get_instance_private (container);

	container->priv->stack = GTK_STACK (gtk_stack_new ());
	gtk_widget_show (GTK_WIDGET (container->priv->stack));
	gtk_container_add (GTK_CONTAINER (container),
			   GTK_WIDGET (container->priv->stack));
}

static TeplPanelItem *
tepl_panel_container_add (TeplPanel   *panel,
			  GtkWidget   *widget,
			  const gchar *name,
			  const gchar *title,
			  const gchar *icon_name)
{
	TeplPanelContainer *container = TEPL_PANEL_CONTAINER (panel);
	TeplPanelItem *item;

	if (gtk_widget_get_parent (widget) != NULL)
	{
		g_warn_if_reached ();
		return NULL;
	}

	item = _tepl_panel_item_simple_new (widget,
					    name,
					    title,
					    icon_name);

	gtk_container_add (GTK_CONTAINER (container->priv->stack), widget);

	container->priv->items = g_list_prepend (container->priv->items,
						 g_object_ref (item));

	return item;
}

static void
tepl_panel_container_remove (TeplPanel     *panel,
			     TeplPanelItem *item)
{
	TeplPanelContainer *container = TEPL_PANEL_CONTAINER (panel);
	GList *node;
	GtkWidget *widget;

	node = g_list_find (container->priv->items, item);
	if (node == NULL)
	{
		return;
	}

	widget = tepl_panel_item_get_widget (item);
	if (widget != NULL)
	{
		gtk_container_remove (GTK_CONTAINER (container->priv->stack),
				      widget);
	}

	g_object_unref (node->data);
	container->priv->items = g_list_delete_link (container->priv->items, node);
}

static void
tepl_panel_container_set_active (TeplPanel     *panel,
				 TeplPanelItem *item)
{
	TeplPanelContainer *container = TEPL_PANEL_CONTAINER (panel);
	GtkWidget *widget;

	widget = tepl_panel_item_get_widget (item);
	if (widget != NULL)
	{
		gtk_stack_set_visible_child (container->priv->stack, widget);
	}
}

static void
tepl_panel_interface_init (gpointer g_iface,
			   gpointer iface_data)
{
	TeplPanelInterface *interface = g_iface;

	interface->add = tepl_panel_container_add;
	interface->remove = tepl_panel_container_remove;
	interface->set_active = tepl_panel_container_set_active;
}

/**
 * tepl_panel_container_new:
 *
 * Returns: (transfer floating): a new #TeplPanelContainer.
 * Since: 6.8
 */
TeplPanelContainer *
tepl_panel_container_new (void)
{
	return g_object_new (TEPL_TYPE_PANEL_CONTAINER, NULL);
}
