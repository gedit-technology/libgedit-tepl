/* SPDX-FileCopyrightText: 2024 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "tepl-panel-stack.h"

/**
 * SECTION:panel-stack
 * @Title: TeplPanelStack
 * @Short_description: Side or bottom panel container based on #GtkStack
 *
 * #TeplPanelStack is a side or bottom panel container. It combines
 * #TeplPanelSimple and #GtkStack.
 */

struct _TeplPanelStackPrivate
{
	TeplPanelSimple *panel_simple;
	GtkStack *stack;
};

G_DEFINE_TYPE_WITH_PRIVATE (TeplPanelStack, tepl_panel_stack, G_TYPE_OBJECT)

static void
panel_simple_add_item_cb (TeplPanelSimple *panel_simple,
			  TeplPanelItem   *item,
			  TeplPanelStack  *panel_stack)
{
	GtkWidget *widget;

	widget = tepl_panel_item_get_widget (item);
	gtk_widget_show (widget);

	gtk_container_add_with_properties (GTK_CONTAINER (panel_stack->priv->stack),
					   widget,
					   "name", tepl_panel_item_get_name (item),
					   "title", tepl_panel_item_get_title (item),
					   "icon-name", tepl_panel_item_get_icon_name (item),
					   "position", tepl_panel_item_get_position (item),
					   NULL);
}

static void
panel_simple_remove_item_cb (TeplPanelSimple *panel_simple,
			     TeplPanelItem   *item,
			     TeplPanelStack  *panel_stack)
{
	GtkWidget *widget = tepl_panel_item_get_widget (item);

	if (widget != NULL)
	{
		gtk_container_remove (GTK_CONTAINER (panel_stack->priv->stack), widget);
	}
}

static void
tepl_panel_stack_dispose (GObject *object)
{
	TeplPanelStack *panel_stack = TEPL_PANEL_STACK (object);

	g_clear_object (&panel_stack->priv->panel_simple);
	g_clear_object (&panel_stack->priv->stack);

	G_OBJECT_CLASS (tepl_panel_stack_parent_class)->dispose (object);
}

static void
tepl_panel_stack_class_init (TeplPanelStackClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->dispose = tepl_panel_stack_dispose;
}

static void
tepl_panel_stack_init (TeplPanelStack *panel_stack)
{
	panel_stack->priv = tepl_panel_stack_get_instance_private (panel_stack);
}

/**
 * tepl_panel_stack_new:
 * @panel_simple: (nullable): a #TeplPanelSimple, or %NULL to create one.
 * @stack: (nullable): a #GtkStack, or %NULL to create one.
 *
 * Returns: (transfer full): a new #TeplPanelStack object.
 * Since: 6.11
 */
TeplPanelStack *
tepl_panel_stack_new (TeplPanelSimple *panel_simple,
		      GtkStack        *stack)
{
	TeplPanelStack *panel_stack;

	g_return_val_if_fail (panel_simple == NULL || TEPL_IS_PANEL_SIMPLE (panel_simple), NULL);
	g_return_val_if_fail (stack == NULL || GTK_IS_STACK (stack), NULL);

	panel_stack = g_object_new (TEPL_TYPE_PANEL_STACK, NULL);

	if (panel_simple == NULL)
	{
		panel_stack->priv->panel_simple = tepl_panel_simple_new ();
	}
	else
	{
		panel_stack->priv->panel_simple = g_object_ref (panel_simple);
	}

	if (stack == NULL)
	{
		panel_stack->priv->stack = GTK_STACK (gtk_stack_new ());
		g_object_ref_sink (panel_stack->priv->stack);
	}
	else
	{
		panel_stack->priv->stack = g_object_ref_sink (stack);
	}

	g_signal_connect_object (panel_stack->priv->panel_simple,
				 "add-item",
				 G_CALLBACK (panel_simple_add_item_cb),
				 panel_stack,
				 G_CONNECT_AFTER);

	g_signal_connect_object (panel_stack->priv->panel_simple,
				 "remove-item",
				 G_CALLBACK (panel_simple_remove_item_cb),
				 panel_stack,
				 G_CONNECT_DEFAULT);

	g_object_bind_property (panel_stack->priv->panel_simple, "active-item-name",
				panel_stack->priv->stack, "visible-child-name",
				G_BINDING_BIDIRECTIONAL | G_BINDING_SYNC_CREATE);

	return panel_stack;
}

/**
 * tepl_panel_stack_get_panel_simple:
 * @panel_stack: a #TeplPanelStack.
 *
 * Gets the #TeplPanelSimple of @panel_stack.
 *
 * It permits to alter the #TeplPanelStack content through the #TeplPanel
 * functions.
 *
 * Returns: (transfer none): the #TeplPanelSimple of @panel_stack.
 * Since: 6.11
 */
TeplPanelSimple *
tepl_panel_stack_get_panel_simple (TeplPanelStack *panel_stack)
{
	g_return_val_if_fail (TEPL_IS_PANEL_STACK (panel_stack), NULL);
	return panel_stack->priv->panel_simple;
}

/**
 * tepl_panel_stack_get_stack:
 * @panel_stack: a #TeplPanelStack.
 *
 * Gets the #GtkStack of @panel_stack.
 *
 * The #GtkStack is exposed in the public API because it is useful for creating
 * a #GtkStackSwitcher.
 *
 * If you need to alter the #TeplPanelStack content, you must use
 * tepl_panel_stack_get_panel_simple() instead.
 *
 * Returns: (transfer none): the #GtkStack of @panel_stack.
 * Since: 6.11
 */
GtkStack *
tepl_panel_stack_get_stack (TeplPanelStack *panel_stack)
{
	g_return_val_if_fail (TEPL_IS_PANEL_STACK (panel_stack), NULL);
	return panel_stack->priv->stack;
}
