/* SPDX-FileCopyrightText: 2023 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <tepl/tepl.h>
#include <stdlib.h>

static TeplPanelStack *
create_panel_stack (void)
{
	TeplPanelStack *panel_stack;
	TeplPanelSimple *panel_simple;
	GtkWidget *label;
	TeplPanelItem *item;

	panel_stack = tepl_panel_stack_new ();
	panel_simple = tepl_panel_stack_get_panel_simple (panel_stack);

	label = gtk_label_new ("Widget 1");
	item = tepl_panel_item_new (label, "name1", "Title 1", NULL);
	tepl_panel_add (TEPL_PANEL (panel_simple), item);
	tepl_panel_set_active (TEPL_PANEL (panel_simple), item);
	g_object_unref (item);

	label = gtk_label_new ("Widget 2");
	item = tepl_panel_item_new (label, "name2", "Title 2", NULL);
	tepl_panel_add (TEPL_PANEL (panel_simple), item);
	g_object_unref (item);

	gtk_widget_show (GTK_WIDGET (tepl_panel_stack_get_stack (panel_stack)));
	return panel_stack;
}

int
main (int    argc,
      char **argv)
{
	GtkWidget *window;
	TeplPanelStack *panel_stack;
	GtkHeaderBar *headerbar;
	TeplPanelSwitcherMenu *switcher_menu;

	tepl_init ();
	gtk_init (&argc, &argv);

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size (GTK_WINDOW (window), 600, 400);

	panel_stack = create_panel_stack ();
	gtk_container_add (GTK_CONTAINER (window),
			   GTK_WIDGET (tepl_panel_stack_get_stack (panel_stack)));

	headerbar = GTK_HEADER_BAR (gtk_header_bar_new ());
	gtk_header_bar_set_show_close_button (headerbar, TRUE);
	gtk_header_bar_set_title (headerbar, "TeplPanelSwitcherMenu Test");
	switcher_menu = tepl_panel_switcher_menu_new (tepl_panel_stack_get_panel_simple (panel_stack));
	gtk_header_bar_pack_start (headerbar, GTK_WIDGET (switcher_menu));
	gtk_widget_show_all (GTK_WIDGET (headerbar));
	gtk_window_set_titlebar (GTK_WINDOW (window), GTK_WIDGET (headerbar));

	gtk_widget_show (window);

	g_signal_connect (window,
			  "destroy",
			  G_CALLBACK (gtk_main_quit),
			  NULL);

	gtk_main ();

	g_object_unref (panel_stack);
	tepl_finalize ();
	return EXIT_SUCCESS;
}
