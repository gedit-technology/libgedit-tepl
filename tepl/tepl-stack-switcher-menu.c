/* SPDX-FileCopyrightText: 2023 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "tepl-stack-switcher-menu.h"

struct _TeplStackSwitcherMenuPrivate
{
	TeplStack *stack;
};

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

	title_label = create_title_label (switcher);
	gtk_container_add (GTK_CONTAINER (hgrid),
			   GTK_WIDGET (title_label));

	down_arrow = gtk_image_new_from_icon_name ("pan-down-symbolic", GTK_ICON_SIZE_BUTTON);
	gtk_container_add (GTK_CONTAINER (hgrid), down_arrow);

	return hgrid;
}

static GtkToggleButton *
create_toggle_button (TeplStackItem *item)
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

	vgrid = GTK_GRID (gtk_grid_new ());
	gtk_orientable_set_orientation (GTK_ORIENTABLE (vgrid), GTK_ORIENTATION_VERTICAL);
	gtk_grid_set_row_spacing (vgrid, 6);

	items = tepl_stack_get_items (switcher->priv->stack);
	for (l = items; l != NULL; l = l->next)
	{
		TeplStackItem *cur_item = TEPL_STACK_ITEM (l->data);

		gtk_container_add (GTK_CONTAINER (vgrid),
				   GTK_WIDGET (create_toggle_button (cur_item)));
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
	GtkPopover *popover;

	menu_button = GTK_MENU_BUTTON (gtk_menu_button_new ());
	gtk_button_set_relief (GTK_BUTTON (menu_button), GTK_RELIEF_NONE);

	gtk_container_add (GTK_CONTAINER (menu_button),
			   GTK_WIDGET (create_menu_button_title (switcher)));

	popover = create_popover (switcher);
	gtk_popover_set_relative_to (popover, GTK_WIDGET (menu_button));
	gtk_menu_button_set_popover (menu_button, GTK_WIDGET (popover));

	return menu_button;
}

static void
populate (TeplStackSwitcherMenu *switcher)
{
	/* TODO: check vertical alignments. */

	if (tepl_stack_has_several_items (switcher->priv->stack))
	{
		gtk_container_add (GTK_CONTAINER (switcher),
				   GTK_WIDGET (create_menu_button (switcher)));
	}
	else
	{
		gtk_container_add (GTK_CONTAINER (switcher),
				   GTK_WIDGET (create_title_label (switcher)));
	}
}

static void
tepl_stack_switcher_menu_dispose (GObject *object)
{
	TeplStackSwitcherMenu *switcher = TEPL_STACK_SWITCHER_MENU (object);

	g_clear_object (&switcher->priv->stack);

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

TeplStackSwitcherMenu *
tepl_stack_switcher_menu_new (TeplStack *stack)
{
	TeplStackSwitcherMenu *switcher;

	g_return_val_if_fail (TEPL_IS_STACK (stack), NULL);

	switcher = g_object_new (TEPL_TYPE_STACK_SWITCHER_MENU, NULL);

	switcher->priv->stack = g_object_ref_sink (stack);

	populate (switcher);
	gtk_widget_show_all (GTK_WIDGET (switcher));

	return switcher;
}
