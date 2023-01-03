/* SPDX-FileCopyrightText: 2023 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "tepl-panel-chooser-menu.h"

struct _TeplPanelChooserMenuPrivate
{
	gint something;
};

G_DEFINE_TYPE_WITH_PRIVATE (TeplPanelChooserMenu, tepl_panel_chooser_menu, GTK_TYPE_BIN)

static gboolean
zero_or_one_component (void)
{
	return FALSE;
}

static GList *
get_components_titles (void)
{
	GList *list = NULL;

	list = g_list_prepend (list, g_strdup ("TeplPanelChooser 2"));
	list = g_list_prepend (list, g_strdup ("TeplPanelChooser 1"));

	return list;
}

static gchar *
get_title_of_displayed_component (void)
{
	return g_strdup ("TeplPanelChooser 1");
}

static GtkLabel *
create_title_label (void)
{
	gchar *title_of_displayed_component;
	GtkLabel *title;
	GtkStyleContext *style_context;

	title_of_displayed_component = get_title_of_displayed_component ();
	title = GTK_LABEL (gtk_label_new (title_of_displayed_component));

	/* To use a TeplPanelChooserMenu in a GtkHeaderBar.
	 * This could be made configurable with an additional API.
	 */
	style_context = gtk_widget_get_style_context (GTK_WIDGET (title));
	gtk_style_context_add_class (style_context, GTK_STYLE_CLASS_TITLE);

	g_free (title_of_displayed_component);
	return title;
}

static GtkGrid *
create_menu_button_title (void)
{
	GtkGrid *hgrid;
	GtkLabel *title_label;
	GtkWidget *down_arrow;

	hgrid = GTK_GRID (gtk_grid_new ());
	gtk_grid_set_column_spacing (hgrid, 6);

	title_label = create_title_label ();
	gtk_container_add (GTK_CONTAINER (hgrid),
			   GTK_WIDGET (title_label));

	down_arrow = gtk_image_new_from_icon_name ("pan-down-symbolic", GTK_ICON_SIZE_BUTTON);
	gtk_container_add (GTK_CONTAINER (hgrid), down_arrow);

	return hgrid;
}

static GtkPopover *
create_popover (void)
{
	GtkPopover *popover;
	GtkGrid *vgrid;
	GList *components_titles;
	GList *l;

	popover = GTK_POPOVER (gtk_popover_new (NULL));

	vgrid = GTK_GRID (gtk_grid_new ());
	gtk_orientable_set_orientation (GTK_ORIENTABLE (vgrid), GTK_ORIENTATION_VERTICAL);
	gtk_grid_set_row_spacing (vgrid, 6);

	components_titles = get_components_titles ();
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
create_menu_button (void)
{
	GtkMenuButton *menu_button;
	GtkPopover *popover;

	menu_button = GTK_MENU_BUTTON (gtk_menu_button_new ());
	gtk_button_set_relief (GTK_BUTTON (menu_button), GTK_RELIEF_NONE);

	gtk_container_add (GTK_CONTAINER (menu_button),
			   GTK_WIDGET (create_menu_button_title ()));

	popover = create_popover ();
	gtk_popover_set_relative_to (popover, GTK_WIDGET (menu_button));
	gtk_menu_button_set_popover (menu_button, GTK_WIDGET (popover));

	return menu_button;
}

static void
populate (TeplPanelChooserMenu *chooser)
{
	/* TODO: check vertical alignments. */

	if (zero_or_one_component ())
	{
		gtk_container_add (GTK_CONTAINER (chooser),
				   GTK_WIDGET (create_title_label ()));
	}
	else
	{
		gtk_container_add (GTK_CONTAINER (chooser),
				   GTK_WIDGET (create_menu_button ()));
	}
}

#if 0
static void
tepl_panel_chooser_menu_finalize (GObject *object)
{

	G_OBJECT_CLASS (tepl_panel_chooser_menu_parent_class)->finalize (object);
}
#endif

static void
tepl_panel_chooser_menu_class_init (TeplPanelChooserMenuClass *klass)
{
#if 0
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = tepl_panel_chooser_menu_finalize;
#endif
}

static void
tepl_panel_chooser_menu_init (TeplPanelChooserMenu *chooser)
{
	chooser->priv = tepl_panel_chooser_menu_get_instance_private (chooser);

	populate (chooser);
	gtk_widget_show_all (GTK_WIDGET (chooser));
}

TeplPanelChooserMenu *
tepl_panel_chooser_menu_new (void)
{
	return g_object_new (TEPL_TYPE_PANEL_CHOOSER_MENU, NULL);
}
