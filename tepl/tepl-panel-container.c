/* SPDX-FileCopyrightText: 2023 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "tepl-panel-container.h"
#include "tepl-panel.h"

/**
 * SECTION:panel-container
 * @Title: TeplPanelContainer
 * @Short_description: Side or bottom panel container
 *
 * #TeplPanelContainer is an implementation of the #TeplPanel interface.
 *
 * #TeplPanelContainer contains API useful to implement a switcher widget, like
 * #TeplPanelSwitcherMenu.
 */

struct _TeplPanelContainerPrivate
{
	GtkStack *stack;

	/* Element type: owned TeplPanelItem */
	GList *items;
};

enum
{
	SIGNAL_CHANGED,
	N_SIGNALS
};

static guint signals[N_SIGNALS];

static void tepl_panel_interface_init (gpointer g_iface,
				       gpointer iface_data);

G_DEFINE_TYPE_WITH_CODE (TeplPanelContainer,
			 tepl_panel_container,
			 GTK_TYPE_BIN,
			 G_ADD_PRIVATE (TeplPanelContainer)
			 G_IMPLEMENT_INTERFACE (TEPL_TYPE_PANEL,
						tepl_panel_interface_init))

static void
emit_changed_signal (TeplPanelContainer *container)
{
	g_signal_emit (container, signals[SIGNAL_CHANGED], 0);
}

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

	/**
	 * TeplPanelContainer::changed:
	 * @container: the #TeplPanelContainer emitting the signal.
	 *
	 * The ::changed signal is emitted when:
	 * - A #TeplPanelItem is added or removed from @container.
	 * - The active #TeplPanelItem of @container has changed.
	 *
	 * Since: 6.8
	 */
	signals[SIGNAL_CHANGED] =
		g_signal_new ("changed",
			      G_TYPE_FROM_CLASS (klass),
			      G_SIGNAL_RUN_FIRST,
			      0, NULL, NULL, NULL,
			      G_TYPE_NONE, 0);
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

	item = tepl_panel_item_new (widget,
				    name,
				    title,
				    icon_name);

	gtk_container_add (GTK_CONTAINER (container->priv->stack), widget);

	container->priv->items = g_list_prepend (container->priv->items,
						 g_object_ref (item));

	emit_changed_signal (container);

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

	emit_changed_signal (container);
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

	emit_changed_signal (container);
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

/**
 * tepl_panel_container_get_items:
 * @container: a #TeplPanelContainer.
 *
 * Returns: (transfer full) (element-type TeplPanelItem): all the items of
 *   @container.
 * Since: 6.8
 */
GList *
tepl_panel_container_get_items (TeplPanelContainer *container)
{
	GList *ret;
	GList *l;

	g_return_val_if_fail (TEPL_IS_PANEL_CONTAINER (container), NULL);

	ret = g_list_copy (container->priv->items);

	for (l = ret; l != NULL; l = l->next)
	{
		g_object_ref (l->data);
	}

	return ret;
}

/**
 * tepl_panel_container_get_active_item:
 * @container: a #TeplPanelContainer.
 *
 * Returns: (transfer none) (nullable): the #TeplPanelItem currently shown in
 *   @container.
 * Since: 6.8
 */
TeplPanelItem *
tepl_panel_container_get_active_item (TeplPanelContainer *container)
{
	GtkWidget *active_widget;
	GList *l;

	g_return_val_if_fail (TEPL_IS_PANEL_CONTAINER (container), NULL);

	active_widget = gtk_stack_get_visible_child (container->priv->stack);

	for (l = container->priv->items; l != NULL; l = l->next)
	{
		TeplPanelItem *cur_item = TEPL_PANEL_ITEM (l->data);
		GtkWidget *cur_widget;

		cur_widget = tepl_panel_item_get_widget (cur_item);
		if (cur_widget == active_widget)
		{
			return cur_item;
		}
	}

	return NULL;
}

/**
 * tepl_panel_container_get_active_item_name:
 * @container: a #TeplPanelContainer.
 *
 * Returns: (nullable): the name of the #TeplPanelItem currently shown in
 *   @container.
 * Since: 6.8
 */
const gchar *
tepl_panel_container_get_active_item_name (TeplPanelContainer *container)
{
	TeplPanelItem *item;

	g_return_val_if_fail (TEPL_IS_PANEL_CONTAINER (container), NULL);

	item = tepl_panel_container_get_active_item (container);
	if (item == NULL)
	{
		return NULL;
	}

	return tepl_panel_item_get_name (item);
}

/**
 * tepl_panel_container_set_active_item_name:
 * @container: a #TeplPanelContainer.
 * @name: a name.
 *
 * Calls tepl_panel_set_active() with the #TeplPanelItem named @name (if found).
 *
 * Since: 6.8
 */
void
tepl_panel_container_set_active_item_name (TeplPanelContainer *container,
					   const gchar        *name)
{
	GList *items;
	GList *l;

	g_return_if_fail (TEPL_IS_PANEL_CONTAINER (container));
	g_return_if_fail (name != NULL);

	items = tepl_panel_container_get_items (container);

	for (l = items; l != NULL; l = l->next)
	{
		TeplPanelItem *cur_item = TEPL_PANEL_ITEM (l->data);
		const gchar *cur_item_name;

		cur_item_name = tepl_panel_item_get_name (cur_item);

		if (g_strcmp0 (cur_item_name, name) == 0)
		{
			tepl_panel_set_active (TEPL_PANEL (container), cur_item);
			break;
		}
	}

	g_list_free_full (items, g_object_unref);
}

/**
 * tepl_panel_container_has_several_items:
 * @container: a #TeplPanelContainer.
 *
 * Convenience function. Useful to implement a switcher widget.
 *
 * Returns: whether @container has more than one #TeplPanelItem.
 * Since: 6.8
 */
gboolean
tepl_panel_container_has_several_items (TeplPanelContainer *container)
{
	g_return_val_if_fail (TEPL_IS_PANEL_CONTAINER (container), FALSE);

	return (container->priv->items != NULL &&
		container->priv->items->next != NULL);
}
