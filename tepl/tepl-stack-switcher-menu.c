/* SPDX-FileCopyrightText: 2023 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "tepl-stack-switcher-menu.h"

/**
 * SECTION:stack-switcher-menu
 * @Short_description: A controller for #TeplStack
 * @Title: TeplStackSwitcherMenu
 *
 * Similar to #GtkStackSwitcher, but for a #TeplStack.
 *
 * This switcher widget is suitable to be put in a #GtkHeaderBar, to choose the
 * component of a side panel (for example).
 */

struct _TeplStackSwitcherMenuPrivate
{
	TeplStack *stack;

	GtkPopover *popover;
};

#define TEPL_STACK_ITEM_KEY "tepl-stack-item-key"

G_DEFINE_TYPE_WITH_PRIVATE (TeplStackSwitcherMenu, tepl_stack_switcher_menu, GTK_TYPE_BIN)

static gchar *
get_title (TeplStack *stack)
{
	TeplStackItem *visible_item;
	gchar *title = NULL;

	if (tepl_stack_get_n_visible_items (stack) > 1)
	{
		g_warn_if_reached ();
	}

	visible_item = tepl_stack_get_visible_item (stack);
	if (visible_item != NULL)
	{
		tepl_stack_item_get_infos (visible_item, NULL, &title, NULL);
	}

	return title;
}

static GtkLabel *
create_title_label (TeplStackSwitcherMenu *switcher)
{
	gchar *title_of_displayed_component;
	GtkLabel *title;
	GtkStyleContext *style_context;

	title_of_displayed_component = get_title (switcher->priv->stack);
	title = GTK_LABEL (gtk_label_new (title_of_displayed_component));

	/* To use a TeplStackSwitcherMenu in a GtkHeaderBar.
	 * This could be made configurable with an additional API.
	 */
	style_context = gtk_widget_get_style_context (GTK_WIDGET (title));
	gtk_style_context_add_class (style_context, GTK_STYLE_CLASS_TITLE);

	g_free (title_of_displayed_component);
	return title;
}

static GtkGrid *
create_menu_button_title (TeplStackSwitcherMenu *switcher)
{
	GtkGrid *hgrid;
	GtkLabel *title_label;
	GtkWidget *down_arrow;

	hgrid = GTK_GRID (gtk_grid_new ());
	gtk_grid_set_column_spacing (hgrid, 6);
	gtk_widget_set_valign (GTK_WIDGET (hgrid), GTK_ALIGN_CENTER);

	title_label = create_title_label (switcher);
	gtk_widget_set_valign (GTK_WIDGET (title_label), GTK_ALIGN_BASELINE);
	gtk_container_add (GTK_CONTAINER (hgrid),
			   GTK_WIDGET (title_label));

	down_arrow = gtk_image_new_from_icon_name ("pan-down-symbolic", GTK_ICON_SIZE_BUTTON);
	gtk_widget_set_valign (down_arrow, GTK_ALIGN_BASELINE);
	gtk_container_add (GTK_CONTAINER (hgrid), down_arrow);

	return hgrid;
}

static void
item_button_clicked_cb (GtkToggleButton       *button,
			TeplStackSwitcherMenu *switcher)
{
	TeplStackItem *item;

	item = g_object_get_data (G_OBJECT (button), TEPL_STACK_ITEM_KEY);
	g_return_if_fail (TEPL_IS_STACK_ITEM (item));

	/* Directly hide the GtkPopover, i.e. not with a transition. Because
	 * repopulate() will be called, which will destroy everything first.
	 * Closing the popover with a transition and destroying it at the same
	 * time doesn't work well. It would be possible to wait before calling
	 * repopulate(), but we prefer the simpler implementation.
	 */
	gtk_widget_hide (GTK_WIDGET (switcher->priv->popover));

	tepl_stack_set_visible_item (switcher->priv->stack, item);
}

static GtkToggleButton *
create_item_button (TeplStackSwitcherMenu *switcher,
		    TeplStackItem         *item)
{
	gchar *title = NULL;
	GtkWidget *widget;
	gboolean visible;
	GtkToggleButton *button;

	tepl_stack_item_get_infos (item, NULL, &title, NULL);

	widget = tepl_stack_item_get_widget (item);
	visible = widget != NULL && gtk_widget_get_visible (widget);

	button = GTK_TOGGLE_BUTTON (gtk_toggle_button_new_with_label (title));
	gtk_toggle_button_set_active (button, visible);
	gtk_widget_set_size_request (GTK_WIDGET (button), 100, -1);

	g_object_set_data_full (G_OBJECT (button),
				TEPL_STACK_ITEM_KEY,
				g_object_ref (item),
				g_object_unref);

	g_signal_connect_object (button,
				 "clicked",
				 G_CALLBACK (item_button_clicked_cb),
				 switcher,
				 0);

	g_free (title);
	return button;
}

static GtkPopover *
create_popover (TeplStackSwitcherMenu *switcher)
{
	GtkPopover *popover;
	GtkGrid *vgrid;
	GList *items;
	GList *l;

	popover = GTK_POPOVER (gtk_popover_new (NULL));
	gtk_popover_set_position (popover, GTK_POS_BOTTOM);

	vgrid = GTK_GRID (gtk_grid_new ());
	gtk_orientable_set_orientation (GTK_ORIENTABLE (vgrid), GTK_ORIENTATION_VERTICAL);
	gtk_grid_set_row_spacing (vgrid, 6);
	g_object_set (vgrid,
		      "margin", 10,
		      NULL);

	items = tepl_stack_get_items (switcher->priv->stack);
	items = g_list_sort (items, (GCompareFunc) tepl_stack_item_compare_by_title);
	for (l = items; l != NULL; l = l->next)
	{
		TeplStackItem *cur_item = TEPL_STACK_ITEM (l->data);

		gtk_container_add (GTK_CONTAINER (vgrid),
				   GTK_WIDGET (create_item_button (switcher, cur_item)));
	}
	g_list_free_full (items, g_object_unref);

	gtk_widget_show_all (GTK_WIDGET (vgrid));
	gtk_container_add (GTK_CONTAINER (popover),
			   GTK_WIDGET (vgrid));

	return popover;
}

static GtkMenuButton *
create_menu_button (TeplStackSwitcherMenu *switcher)
{
	GtkMenuButton *menu_button;

	menu_button = GTK_MENU_BUTTON (gtk_menu_button_new ());
	gtk_button_set_relief (GTK_BUTTON (menu_button), GTK_RELIEF_NONE);

	gtk_container_add (GTK_CONTAINER (menu_button),
			   GTK_WIDGET (create_menu_button_title (switcher)));

	g_assert (switcher->priv->popover == NULL);
	switcher->priv->popover = create_popover (switcher);
	gtk_popover_set_relative_to (switcher->priv->popover, GTK_WIDGET (menu_button));
	gtk_menu_button_set_popover (menu_button, GTK_WIDGET (switcher->priv->popover));

	return menu_button;
}

static void
clear_all (TeplStackSwitcherMenu *switcher)
{
	GtkWidget *child = gtk_bin_get_child (GTK_BIN (switcher));

	if (child != NULL)
	{
		gtk_widget_destroy (child);
	}

	switcher->priv->popover = NULL;
}

static void
repopulate (TeplStackSwitcherMenu *switcher)
{
	GtkWidget *child;

	clear_all (switcher);

	/* TODO: check vertical alignments. */

	if (tepl_stack_has_several_items (switcher->priv->stack))
	{
		child = GTK_WIDGET (create_menu_button (switcher));
	}
	else
	{
		child = GTK_WIDGET (create_title_label (switcher));
	}

	gtk_widget_show_all (child);
	gtk_container_add (GTK_CONTAINER (switcher), child);
}

static void
tepl_stack_switcher_menu_dispose (GObject *object)
{
	TeplStackSwitcherMenu *switcher = TEPL_STACK_SWITCHER_MENU (object);

	g_clear_object (&switcher->priv->stack);
	switcher->priv->popover = NULL;

	G_OBJECT_CLASS (tepl_stack_switcher_menu_parent_class)->dispose (object);
}

static void
tepl_stack_switcher_menu_class_init (TeplStackSwitcherMenuClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->dispose = tepl_stack_switcher_menu_dispose;
}

static void
tepl_stack_switcher_menu_init (TeplStackSwitcherMenu *switcher)
{
	switcher->priv = tepl_stack_switcher_menu_get_instance_private (switcher);
}

static void
stack_changed_cb (TeplStack             *stack,
		  TeplStackSwitcherMenu *switcher)
{
	repopulate (switcher);
}

/**
 * tepl_stack_switcher_menu_new:
 * @stack: a #TeplStack.
 *
 * Returns: (transfer floating): a new #TeplStackSwitcherMenu widget.
 * Since: 6.6
 */
TeplStackSwitcherMenu *
tepl_stack_switcher_menu_new (TeplStack *stack)
{
	TeplStackSwitcherMenu *switcher;

	g_return_val_if_fail (TEPL_IS_STACK (stack), NULL);

	switcher = g_object_new (TEPL_TYPE_STACK_SWITCHER_MENU, NULL);

	switcher->priv->stack = g_object_ref_sink (stack);

	repopulate (switcher);

	g_signal_connect_object (switcher->priv->stack,
				 "changed",
				 G_CALLBACK (stack_changed_cb),
				 switcher,
				 0);

	return switcher;
}
