/* SPDX-FileCopyrightText: 2024 - Sébastien Wilmet
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
	gtk_widget_set_hexpand (GTK_WIDGET (panel), TRUE);
	gtk_widget_set_vexpand (GTK_WIDGET (panel), TRUE);

	label = gtk_label_new ("Widget 1");
	item = tepl_panel_item_new (label, "name1", "Title 1", NULL, 0);
	tepl_panel_add (TEPL_PANEL (panel), item);
	g_object_unref (item);

	label = gtk_label_new ("Widget 2");
	item = tepl_panel_item_new (label, "name2", "Title 2", NULL, 0);
	tepl_panel_add (TEPL_PANEL (panel), item);
	g_object_unref (item);

	gtk_widget_show (GTK_WIDGET (panel));
	return panel;
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
	GtkGrid *vgrid;
	TeplPanelContainer *panel;
	TeplPanelSwitcherNotebook *switcher_notebook;

	tepl_init ();
	gtk_init (&argc, &argv);

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size (GTK_WINDOW (window), 600, 400);

	vgrid = GTK_GRID (gtk_grid_new ());
	gtk_orientable_set_orientation (GTK_ORIENTABLE (vgrid), GTK_ORIENTATION_VERTICAL);

	panel = create_panel ();
	switcher_notebook = tepl_panel_switcher_notebook_new (panel, create_notebook ());

	gtk_container_add (GTK_CONTAINER (vgrid), GTK_WIDGET (panel));
	gtk_container_add (GTK_CONTAINER (vgrid), GTK_WIDGET (switcher_notebook));
	gtk_container_add (GTK_CONTAINER (window), GTK_WIDGET (vgrid));

	gtk_widget_show_all (window);

	g_signal_connect (window,
			  "destroy",
			  G_CALLBACK (gtk_main_quit),
			  NULL);

	gtk_main ();
	tepl_finalize ();
	return EXIT_SUCCESS;
}
