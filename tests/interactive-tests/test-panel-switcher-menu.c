/* SPDX-FileCopyrightText: 2023 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <tepl/tepl.h>
#include <stdlib.h>

static TeplPanelContainer *
create_panel (void)
{
	TeplPanelContainer *panel;
	GtkWidget *label;
	TeplPanelItem *item;

	panel = tepl_panel_container_new ();

	label = gtk_label_new ("Widget 1");
	gtk_widget_show (label);
	item = tepl_panel_add (TEPL_PANEL (panel), label, "name1", "Title 1", NULL);
	g_object_unref (item);

	label = gtk_label_new ("Widget 2");
	gtk_widget_show (label);
	item = tepl_panel_add (TEPL_PANEL (panel), label, "name2", "Title 2", NULL);
	g_object_unref (item);

	gtk_widget_show (GTK_WIDGET (panel));
	return panel;
}

int
main (int    argc,
      char **argv)
{
	GtkWidget *window;
	TeplPanelContainer *panel;
	GtkHeaderBar *headerbar;
	TeplPanelSwitcherMenu *switcher_menu;

	tepl_init ();
	gtk_init (&argc, &argv);

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size (GTK_WINDOW (window), 600, 400);

	panel = create_panel ();
	gtk_container_add (GTK_CONTAINER (window), GTK_WIDGET (panel));

	headerbar = GTK_HEADER_BAR (gtk_header_bar_new ());
	gtk_header_bar_set_show_close_button (headerbar, TRUE);
	gtk_header_bar_set_title (headerbar, "TeplPanelSwitcherMenu Test");
	switcher_menu = tepl_panel_switcher_menu_new (panel);
	gtk_header_bar_pack_start (headerbar, GTK_WIDGET (switcher_menu));
	gtk_widget_show_all (GTK_WIDGET (headerbar));
	gtk_window_set_titlebar (GTK_WINDOW (window), GTK_WIDGET (headerbar));

	gtk_widget_show (window);

	g_signal_connect (window,
			  "destroy",
			  G_CALLBACK (gtk_main_quit),
			  NULL);

	gtk_main ();
	tepl_finalize ();
	return EXIT_SUCCESS;
}
