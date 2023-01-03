/* SPDX-FileCopyrightText: 2023 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "tepl-stack-switcher-menu.h"

struct _TeplStackSwitcherMenuPrivate
{
	TeplStack *stack;
};

G_DEFINE_TYPE_WITH_PRIVATE (TeplStackSwitcherMenu, tepl_stack_switcher_menu, GTK_TYPE_BIN)

static GList *
get_components_titles (TeplStack *stack)
{
	GList *titles = NULL;
	GList *items;
	GList *l;

	items = tepl_stack_get_items (stack);
	for (l = items; l != NULL; l = l->next)
	{
		TeplStackItem *cur_item = TEPL_STACK_ITEM (l->data);
		gchar *cur_title = NULL;

		tepl_stack_item_get_infos (cur_item, NULL, &cur_title, NULL);
		titles = g_list_prepend (titles, cur_title);
	}
	g_list_free_full (items, g_object_unref);

	return g_list_reverse (titles);
}

static gchar *
get_title_of_displayed_component (TeplStack *stack)
{
	GList *titles;
	gchar *ret = NULL;

	/* Dummy implementation */

	titles = get_components_titles (stack);
	if (titles != NULL)
	{
		ret = g_strdup (titles->data);
	}
	g_list_free_full (titles, g_free);

	return ret;
}

static GtkLabel *
create_title_label (TeplStackSwitcherMenu *switcher)
{
	gchar *title_of_displayed_component;
	GtkLabel *title;
	GtkStyleContext *style_context;

	title_of_displayed_component = get_title_of_displayed_component (switcher->priv->stack);
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

static GtkPopover *
create_popover (TeplStackSwitcherMenu *switcher)
{
	GtkPopover *popover;
	GtkGrid *vgrid;
	GList *components_titles;
	GList *l;

	popover = GTK_POPOVER (gtk_popover_new (NULL));

	vgrid = GTK_GRID (gtk_grid_new ());
	gtk_orientable_set_orientation (GTK_ORIENTABLE (vgrid), GTK_ORIENTATION_VERTICAL);
	gtk_grid_set_row_spacing (vgrid, 6);

	components_titles = get_components_titles (switcher->priv->stack);
	for (l = components_titles; l != NULL; l = l->next)
	{
		const gchar *cur_component_title = l->data;
		GtkToggleButton *button;

		button = GTK_TOGGLE_BUTTON (gtk_toggle_button_new_with_label (cur_component_title));

		gtk_container_add (GTK_CONTAINER (vgrid),
				   GTK_WIDGET (button));
	}
	g_list_free_full (components_titles, g_free);

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
