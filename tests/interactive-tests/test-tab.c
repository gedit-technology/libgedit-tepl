/* SPDX-FileCopyrightText: 2016-2022 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <tepl/tepl.h>
#include <stdlib.h>

static void
info_bar_response_cb (GtkInfoBar *info_bar,
		      gint        response_id,
		      gpointer    user_data)
{
	gtk_widget_destroy (GTK_WIDGET (info_bar));
}

static void
basic_cb (GtkButton *button,
	  TeplTab   *tab)
{
	TeplInfoBar *info_bar;
	GtkWidget *entry;

	info_bar = tepl_info_bar_new_simple (GTK_MESSAGE_WARNING,
					     "Primary message.",
					     "Secondary message.");

	entry = gtk_entry_new ();
	gtk_widget_show (entry);
	tepl_info_bar_add_content_widget (info_bar, entry, TEPL_INFO_BAR_LOCATION_ALONGSIDE_ICON);
	//tepl_info_bar_add_content_widget (info_bar, entry, TEPL_INFO_BAR_LOCATION_BELOW_ICON);

	tepl_info_bar_setup_close_button (info_bar);

	// Test changing the :message-type property. Is the icon correctly
	// updated?
	gtk_info_bar_set_message_type (GTK_INFO_BAR (info_bar), GTK_MESSAGE_QUESTION);

	// Test setting the :icon-name property. It should have a higher
	// priority.
	tepl_info_bar_set_icon_name (info_bar, "document-print");

	tepl_tab_add_info_bar (tab, GTK_INFO_BAR (info_bar));
	gtk_widget_show (GTK_WIDGET (info_bar));
}

static void
progress_cb (GtkButton *button,
	     TeplTab   *tab)
{
	TeplProgressInfoBar *info_bar;

	info_bar = tepl_progress_info_bar_new ("document-save",
					       "File loading... The full and very long path is: "
					       "/home/seb/a/very/long/path/like/this/is/beautiful"
					       "/but/is/it/correctly/wrapped/in/the/info/bar/that"
					       "/is/the/question",
					       TRUE);

	tepl_progress_info_bar_set_fraction (info_bar, 0.3);

	g_signal_connect (info_bar,
			  "response",
			  G_CALLBACK (info_bar_response_cb),
			  NULL);

	tepl_tab_add_info_bar (tab, GTK_INFO_BAR (info_bar));
	gtk_widget_show (GTK_WIDGET (info_bar));
}

static GtkWidget *
create_side_panel (TeplTab *tab)
{
	GtkGrid *vgrid;
	GtkWidget *basic;
	GtkWidget *progress;

	vgrid = GTK_GRID (gtk_grid_new ());
	gtk_orientable_set_orientation (GTK_ORIENTABLE (vgrid), GTK_ORIENTATION_VERTICAL);
	gtk_grid_set_row_spacing (vgrid, 6);

	basic = gtk_button_new_with_label ("Basic");
	gtk_container_add (GTK_CONTAINER (vgrid), basic);
	g_signal_connect_object (basic,
				 "clicked",
				 G_CALLBACK (basic_cb),
				 tab,
				 0);

	progress = gtk_button_new_with_label ("Progress");
	gtk_container_add (GTK_CONTAINER (vgrid), progress);
	g_signal_connect_object (progress,
				 "clicked",
				 G_CALLBACK (progress_cb),
				 tab,
				 0);

	return GTK_WIDGET (vgrid);
}

static GtkWidget *
create_window_content (void)
{
	GtkGrid *hgrid;
	TeplTab *tab;
	GtkWidget *side_panel;

	hgrid = GTK_GRID (gtk_grid_new ());
	gtk_orientable_set_orientation (GTK_ORIENTABLE (hgrid), GTK_ORIENTATION_HORIZONTAL);
	gtk_grid_set_column_spacing (hgrid, 6);
	g_object_set (hgrid,
		      "margin", 6,
		      NULL);

	tab = tepl_tab_new ();
	side_panel = create_side_panel (tab);

	gtk_container_add (GTK_CONTAINER (hgrid), side_panel);
	gtk_container_add (GTK_CONTAINER (hgrid), GTK_WIDGET (tab));

	gtk_widget_show_all (GTK_WIDGET (hgrid));

	return GTK_WIDGET (hgrid);
}

int
main (int    argc,
      char **argv)
{
	GtkWidget *window;

	tepl_init ();
	gtk_init (&argc, &argv);

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size (GTK_WINDOW (window), 800, 600);
	gtk_container_add (GTK_CONTAINER (window), create_window_content ());
	gtk_widget_show (window);

	g_signal_connect (window,
			  "destroy",
			  G_CALLBACK (gtk_main_quit),
			  NULL);

	gtk_main ();
	tepl_finalize ();
	return EXIT_SUCCESS;
}
