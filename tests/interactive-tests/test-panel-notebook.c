/* SPDX-FileCopyrightText: 2024 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <tepl/tepl.h>
#include <stdlib.h>

static TeplPanelSimple *
create_panel_simple (void)
{
	TeplPanelSimple *panel_simple;
	GtkWidget *label;
	TeplPanelItem *item;

	panel_simple = tepl_panel_simple_new ();

	label = gtk_label_new ("Widget 1");
	item = tepl_panel_item_new (label, "name1", "Title 1", NULL, 0);
	tepl_panel_add (TEPL_PANEL (panel_simple), item);
	g_object_unref (item);

	label = gtk_label_new ("Widget 2");
	item = tepl_panel_item_new (label, "name2", "Title 2", NULL, 0);
	tepl_panel_add (TEPL_PANEL (panel_simple), item);
	g_object_unref (item);

	return panel_simple;
}

static GtkNotebook *
create_notebook (void)
{
	GtkNotebook *notebook;

	notebook = GTK_NOTEBOOK (gtk_notebook_new ());
	gtk_notebook_set_tab_pos (notebook, GTK_POS_BOTTOM);

	return notebook;
}

int
main (int    argc,
      char **argv)
{
	GtkWidget *window;
	TeplPanelSimple *panel_simple;
	GtkNotebook *notebook;
	TeplPanelNotebook *panel_notebook;

	tepl_init ();
	gtk_init (&argc, &argv);

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size (GTK_WINDOW (window), 600, 400);

	panel_simple = create_panel_simple ();
	notebook = create_notebook ();
	panel_notebook = tepl_panel_notebook_new (panel_simple, notebook);
	g_object_unref (panel_simple);

	gtk_container_add (GTK_CONTAINER (window), GTK_WIDGET (notebook));
	gtk_widget_show_all (window);

	g_signal_connect (window,
			  "destroy",
			  G_CALLBACK (gtk_main_quit),
			  NULL);

	gtk_main ();

	g_object_unref (panel_notebook);
	tepl_finalize ();

	return EXIT_SUCCESS;
}
