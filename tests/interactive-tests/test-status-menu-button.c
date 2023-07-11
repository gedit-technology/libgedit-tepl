/* SPDX-FileCopyrightText: 2023 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <tepl/tepl.h>
#include <stdlib.h>

static TeplStatusMenuButton *
create_status_button (void)
{
	TeplStatusMenuButton *status_button;
	GtkWidget *popover_label;
	GtkWidget *popover;

	status_button = tepl_status_menu_button_new ();
	tepl_status_menu_button_set_label_text (status_button, "Text");

	popover_label = gtk_label_new ("Popover");
	gtk_widget_show (popover_label);

	popover = gtk_popover_new (GTK_WIDGET (status_button));
	gtk_container_add (GTK_CONTAINER (popover), popover_label);

	gtk_menu_button_set_popover (GTK_MENU_BUTTON (status_button), popover);

	return status_button;
}

static GtkWidget *
create_statusbar (void)
{
	TeplStatusbar *statusbar;

	statusbar = tepl_statusbar_new ();

	gtk_statusbar_push (GTK_STATUSBAR (statusbar),
			    gtk_statusbar_get_context_id (GTK_STATUSBAR (statusbar), "foo"),
			    "Status bar");

	gtk_box_pack_end (GTK_BOX (statusbar),
			  GTK_WIDGET (create_status_button ()),
			  FALSE, FALSE, 0);

	return GTK_WIDGET (statusbar);
}

static GtkFrame *
create_frame (void)
{
	GtkFrame *frame;
	GtkCssProvider *css_provider;
	GtkStyleContext *style_context;
	GError *error = NULL;

	/* To remove unwanted borders.
	 * Keep the one at the bottom, to see where the statusbar starts.
	 */
	const gchar *frame_border_css =
		"frame border {\n"
		"  border-top: none;\n"
		"  border-left: none;\n"
		"  border-right: none;\n"
		"}\n";

	frame = GTK_FRAME (gtk_frame_new (NULL));

	css_provider = gtk_css_provider_new ();
	gtk_css_provider_load_from_data (css_provider, frame_border_css, -1, &error);
	if (error != NULL)
	{
		g_warning ("CSS error: %s", error->message);
		g_clear_error (&error);
	}

	style_context = gtk_widget_get_style_context (GTK_WIDGET (frame));
	gtk_style_context_add_provider (style_context,
					GTK_STYLE_PROVIDER (css_provider),
					GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
	g_object_unref (css_provider);

	return frame;
}

static GtkWidget *
create_content (void)
{
	GtkWidget *content;
	GtkFrame *frame;

	content= gtk_label_new ("Content");
	gtk_widget_set_hexpand (content, TRUE);
	gtk_widget_set_vexpand (content, TRUE);

	frame = create_frame ();
	gtk_container_add (GTK_CONTAINER (frame), content);

	return GTK_WIDGET (frame);
}

int
main (int    argc,
      char **argv)
{
	GtkWidget *window;
	GtkGrid *vgrid;

	gtk_init (&argc, &argv);

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size (GTK_WINDOW (window), 600, 400);
	g_signal_connect (window, "destroy", gtk_main_quit, NULL);

	vgrid = GTK_GRID (gtk_grid_new ());
	gtk_orientable_set_orientation (GTK_ORIENTABLE (vgrid), GTK_ORIENTATION_VERTICAL);

	gtk_container_add (GTK_CONTAINER (vgrid), create_content ());
	gtk_container_add (GTK_CONTAINER (vgrid), create_statusbar ());

	gtk_container_add (GTK_CONTAINER (window), GTK_WIDGET (vgrid));

	gtk_widget_show_all (window);

	gtk_main ();
	return EXIT_SUCCESS;
}
