/* SPDX-FileCopyrightText: 2023 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "tepl-panel-switcher-menu.h"
#include "tepl-panel.h"

/**
 * SECTION:panel-switcher-menu
 * @Short_description: A controller for #TeplPanelSimple
 * @Title: TeplPanelSwitcherMenu
 *
 * Similar to #GtkStackSwitcher, but for a #TeplPanelSimple.
 *
 * This switcher widget is suitable to be put in a #GtkHeaderBar, to choose the
 * component of a side panel (for example).
 */

struct _TeplPanelSwitcherMenuPrivate
{
	TeplPanelSimple *panel;

	GtkPopover *popover;
};

#define TEPL_PANEL_ITEM_KEY "tepl-panel-item-key"

G_DEFINE_TYPE_WITH_PRIVATE (TeplPanelSwitcherMenu, tepl_panel_switcher_menu, GTK_TYPE_BIN)

static const gchar *
get_title (TeplPanelSimple *panel)
{
	TeplPanelItem *active_item;

	active_item = tepl_panel_simple_get_active_item (panel);
	if (active_item != NULL)
	{
		return tepl_panel_item_get_title (active_item);
	}

	return NULL;
}

static GtkLabel *
create_title_label (TeplPanelSwitcherMenu *switcher)
{
	const gchar *title_of_displayed_component;
	GtkLabel *title;
	GtkStyleContext *style_context;

	title_of_displayed_component = get_title (switcher->priv->panel);
	title = GTK_LABEL (gtk_label_new (title_of_displayed_component));

	/* To use a TeplPanelSwitcherMenu in a GtkHeaderBar.
	 * This could be made configurable with an additional API.
	 */
	style_context = gtk_widget_get_style_context (GTK_WIDGET (title));
	gtk_style_context_add_class (style_context, GTK_STYLE_CLASS_TITLE);

	return title;
}

static GtkGrid *
create_menu_button_title (TeplPanelSwitcherMenu *switcher)
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
			TeplPanelSwitcherMenu *switcher)
{
	TeplPanelItem *item;

	item = g_object_get_data (G_OBJECT (button), TEPL_PANEL_ITEM_KEY);
	g_return_if_fail (TEPL_IS_PANEL_ITEM (item));

	/* Directly hide the GtkPopover, i.e. not with a transition. Because
	 * repopulate() will be called, which will destroy everything first.
	 * Closing the popover with a transition and destroying it at the same
	 * time doesn't work well. It would be possible to wait before calling
	 * repopulate(), but we prefer the simpler implementation.
	 */
	gtk_widget_hide (GTK_WIDGET (switcher->priv->popover));

	tepl_panel_set_active (TEPL_PANEL (switcher->priv->panel), item);
}

static GtkToggleButton *
create_item_button (TeplPanelSwitcherMenu *switcher,
		    TeplPanelItem         *item,
		    gboolean               is_active_item)
{
	const gchar *title;
	GtkToggleButton *button;

	title = tepl_panel_item_get_title (item);

	button = GTK_TOGGLE_BUTTON (gtk_toggle_button_new_with_label (title));
	gtk_toggle_button_set_active (button, is_active_item);
	gtk_widget_set_size_request (GTK_WIDGET (button), 100, -1);

	g_object_set_data_full (G_OBJECT (button),
				TEPL_PANEL_ITEM_KEY,
				g_object_ref (item),
				g_object_unref);

	g_signal_connect_object (button,
				 "clicked",
				 G_CALLBACK (item_button_clicked_cb),
				 switcher,
				 G_CONNECT_DEFAULT);

	return button;
}

static GtkPopover *
create_popover (TeplPanelSwitcherMenu *switcher)
{
	GtkPopover *popover;
	GtkGrid *vgrid;
	TeplPanelItem *active_item;
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

	active_item = tepl_panel_simple_get_active_item (switcher->priv->panel);

	items = tepl_panel_simple_get_items (switcher->priv->panel);
	items = g_list_sort (items, (GCompareFunc) tepl_panel_item_compare);
	for (l = items; l != NULL; l = l->next)
	{
		TeplPanelItem *cur_item = TEPL_PANEL_ITEM (l->data);
		GtkToggleButton *button;

		button = create_item_button (switcher,
					     cur_item,
					     cur_item == active_item);

		gtk_container_add (GTK_CONTAINER (vgrid), GTK_WIDGET (button));
	}
	g_list_free_full (items, g_object_unref);

	gtk_widget_show_all (GTK_WIDGET (vgrid));
	gtk_container_add (GTK_CONTAINER (popover),
			   GTK_WIDGET (vgrid));

	return popover;
}

static GtkMenuButton *
create_menu_button (TeplPanelSwitcherMenu *switcher)
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
clear_all (TeplPanelSwitcherMenu *switcher)
{
	GtkWidget *child = gtk_bin_get_child (GTK_BIN (switcher));

	if (child != NULL)
	{
		gtk_widget_destroy (child);
	}

	switcher->priv->popover = NULL;
}

static void
repopulate (TeplPanelSwitcherMenu *switcher)
{
	GtkWidget *child;

	clear_all (switcher);

	if (tepl_panel_simple_has_several_items (switcher->priv->panel))
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
tepl_panel_switcher_menu_dispose (GObject *object)
{
	TeplPanelSwitcherMenu *switcher = TEPL_PANEL_SWITCHER_MENU (object);

	g_clear_object (&switcher->priv->panel);
	switcher->priv->popover = NULL;

	G_OBJECT_CLASS (tepl_panel_switcher_menu_parent_class)->dispose (object);
}

static void
tepl_panel_switcher_menu_class_init (TeplPanelSwitcherMenuClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->dispose = tepl_panel_switcher_menu_dispose;
}

static void
tepl_panel_switcher_menu_init (TeplPanelSwitcherMenu *switcher)
{
	switcher->priv = tepl_panel_switcher_menu_get_instance_private (switcher);
}

static void
panel_changed_cb (TeplPanelSimple       *panel,
		  TeplPanelSwitcherMenu *switcher)
{
	repopulate (switcher);
}

/**
 * tepl_panel_switcher_menu_new:
 * @panel: a #TeplPanelSimple.
 *
 * Returns: (transfer floating): a new #TeplPanelSwitcherMenu widget.
 * Since: 6.12
 */
TeplPanelSwitcherMenu *
tepl_panel_switcher_menu_new (TeplPanelSimple *panel)
{
	TeplPanelSwitcherMenu *switcher;

	g_return_val_if_fail (TEPL_IS_PANEL_SIMPLE (panel), NULL);

	switcher = g_object_new (TEPL_TYPE_PANEL_SWITCHER_MENU, NULL);

	switcher->priv->panel = g_object_ref_sink (panel);

	repopulate (switcher);

	g_signal_connect_object (switcher->priv->panel,
				 "changed",
				 G_CALLBACK (panel_changed_cb),
				 switcher,
				 G_CONNECT_DEFAULT);

	return switcher;
}
