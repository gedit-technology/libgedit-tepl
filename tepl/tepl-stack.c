/* SPDX-FileCopyrightText: 2023 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "tepl-stack.h"

struct _TeplStackPrivate
{
	GtkGrid *grid;

	/* Element-type: owned TeplStackItem */
	GList *items;
};

enum
{
	SIGNAL_CHANGED,
	N_SIGNALS
};

static guint signals[N_SIGNALS];

G_DEFINE_TYPE_WITH_PRIVATE (TeplStack, tepl_stack, GTK_TYPE_BIN)

static void
tepl_stack_dispose (GObject *object)
{
	TeplStack *stack = TEPL_STACK (object);

	stack->priv->grid = NULL;

	g_list_free_full (stack->priv->items, g_object_unref);
	stack->priv->items = NULL;

	G_OBJECT_CLASS (tepl_stack_parent_class)->dispose (object);
}

static void
tepl_stack_class_init (TeplStackClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->dispose = tepl_stack_dispose;

	/**
	 * TeplStack::changed:
	 * @stack: the #TeplStack emitting the signal.
	 *
	 * The ::changed signal is emitted when BLAH BLAH BLAH.
	 *
	 * Since: 6.6
	 */
	signals[SIGNAL_CHANGED] =
		g_signal_new ("changed",
			      G_TYPE_FROM_CLASS (klass),
			      G_SIGNAL_RUN_FIRST,
			      0, NULL, NULL, NULL,
			      G_TYPE_NONE, 0);
}

static void
tepl_stack_init (TeplStack *stack)
{
	stack->priv = tepl_stack_get_instance_private (stack);

	stack->priv->grid = GTK_GRID (gtk_grid_new ());
	gtk_widget_show (GTK_WIDGET (stack->priv->grid));

	gtk_container_add (GTK_CONTAINER (stack),
			   GTK_WIDGET (stack->priv->grid));
}

TeplStack *
tepl_stack_new (void)
{
	return g_object_new (TEPL_TYPE_STACK, NULL);
}

void
tepl_stack_add_item (TeplStack     *stack,
		     TeplStackItem *item)
{
	GList *node;
	GtkWidget *item_widget;

	g_return_if_fail (TEPL_IS_STACK (stack));
	g_return_if_fail (TEPL_IS_STACK_ITEM (item));

	node = g_list_find (stack->priv->items, item);
	if (node != NULL)
	{
		return;
	}

	stack->priv->items = g_list_prepend (stack->priv->items, g_object_ref (item));

	item_widget = tepl_stack_item_get_widget (item);
	if (item_widget != NULL)
	{
		gtk_container_add (GTK_CONTAINER (stack->priv->grid), item_widget);
	}

	tepl_stack_changed (stack);
}

void
tepl_stack_remove_item (TeplStack     *stack,
			TeplStackItem *item)
{
	GList *node;
	GtkWidget *item_widget;

	g_return_if_fail (TEPL_IS_STACK (stack));
	g_return_if_fail (TEPL_IS_STACK_ITEM (item));

	node = g_list_find (stack->priv->items, item);
	if (node == NULL)
	{
		return;
	}

	item_widget = tepl_stack_item_get_widget (item);
	if (item_widget != NULL)
	{
		gtk_container_remove (GTK_CONTAINER (stack->priv->grid), item_widget);
	}

	stack->priv->items = g_list_delete_link (stack->priv->items, node);
	g_object_unref (item);

	tepl_stack_changed (stack);
}

/**
 * tepl_stack_get_items:
 * @stack: a #TeplStack.
 *
 * Returns: (transfer full) (element-type TeplStackItem): all the items of
 *   @stack.
 * Since: 6.6
 */
GList *
tepl_stack_get_items (TeplStack *stack)
{
	g_return_val_if_fail (TEPL_IS_STACK (stack), NULL);

	return g_list_copy_deep (stack->priv->items, (GCopyFunc) g_object_ref, NULL);
}

void
tepl_stack_changed (TeplStack *stack)
{
	g_return_if_fail (TEPL_IS_STACK (stack));

	g_signal_emit (stack, signals[SIGNAL_CHANGED], 0);
}

/**
 * tepl_stack_has_several_items:
 * @stack: a #TeplStack.
 *
 * Convenience function.
 *
 * Returns: whether @stack has more than one #TeplStackItem.
 * Since: 6.6
 */
gboolean
tepl_stack_has_several_items (TeplStack *stack)
{
	g_return_val_if_fail (TEPL_IS_STACK (stack), FALSE);

	return (stack->priv->items != NULL &&
		stack->priv->items->next != NULL);
}

guint
tepl_stack_get_n_visible_items (TeplStack *stack)
{
	guint n_visible_items = 0;
	GList *l;

	g_return_val_if_fail (TEPL_IS_STACK (stack), 0);

	for (l = stack->priv->items; l != NULL; l = l->next)
	{
		TeplStackItem *item = TEPL_STACK_ITEM (l->data);
		GtkWidget *widget;

		widget = tepl_stack_item_get_widget (item);

		if (widget != NULL && gtk_widget_get_visible (widget))
		{
			n_visible_items++;
		}
	}

	return n_visible_items;
}

/**
 * tepl_stack_get_visible_item:
 * @stack: a #TeplStack.
 *
 * Returns: (transfer none) (nullable):
 * Since: 6.6
 */
TeplStackItem *
tepl_stack_get_visible_item (TeplStack *stack)
{
	GList *l;

	g_return_val_if_fail (TEPL_IS_STACK (stack), NULL);

	for (l = stack->priv->items; l != NULL; l = l->next)
	{
		TeplStackItem *item = TEPL_STACK_ITEM (l->data);
		GtkWidget *widget;

		widget = tepl_stack_item_get_widget (item);

		if (widget != NULL && gtk_widget_get_visible (widget))
		{
			return item;
		}
	}

	return NULL;
}
