/* SPDX-FileCopyrightText: 2022 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <tepl/tepl.h>
#include "tepl/tepl-menu-stack-switcher.h"
#include <stdlib.h>

#define N_STACK_CHILDREN (3)

static gchar *
get_child_name (gint num)
{
	return g_strdup_printf ("child%d", num);
}

static void
destroy_nth_stack_child (GtkStack *stack,
			 gint      num)
{
	gchar *name;
	GtkWidget *child;

	name = get_child_name (num);
	child = gtk_stack_get_child_by_name (stack, name);
	if (child != NULL)
	{
		gtk_widget_destroy (child);
	}

	g_free (name);
}

static void
add_stack_child (GtkStack *stack,
		 gint      num)
{
	gchar *name;
	gchar *title;
	GtkWidget *label;

	destroy_nth_stack_child (stack, num);

	name = get_child_name (num);
	title = g_strdup_printf ("Label %d", num);

	label = gtk_label_new (title);
	gtk_widget_show (label);

	gtk_container_add_with_properties (GTK_CONTAINER (stack),
					   label,
					   "name", name,
					   "title", title,
					   "position", num,
					   NULL);

	g_free (name);
	g_free (title);
}

static GtkStack *
create_stack (void)
{
	GtkStack *stack;
	gint num;

	stack = GTK_STACK (gtk_stack_new ());

	for (num = 1; num <= N_STACK_CHILDREN; num++)
	{
		add_stack_child (stack, num);
	}

	return stack;
}

static GtkHeaderBar *
create_header_bar (TeplMenuStackSwitcher *switcher)
{
	GtkHeaderBar *header_bar;
	GtkWidget *other_button;

	header_bar = GTK_HEADER_BAR (gtk_header_bar_new ());
	gtk_header_bar_set_title (header_bar, "Application Title");
	gtk_header_bar_set_show_close_button (header_bar, TRUE);

	gtk_header_bar_pack_start (header_bar, GTK_WIDGET (switcher));

	/* To more easily check the vertical alignment of the text and arrow in
	 * the TeplMenuStackSwitcher.
	 */
	other_button = gtk_button_new_with_label ("Other Button");
	gtk_header_bar_pack_start (header_bar, other_button);

	gtk_widget_show_all (GTK_WIDGET (header_bar));
	return header_bar;
}

static void
check_button_toggled_cb (GtkCheckButton *check_button,
			 GtkStack       *stack)
{
	gpointer data;
	gint num;

	data = g_object_get_data (G_OBJECT (check_button), "num");
	g_assert (data != NULL);
	num = GPOINTER_TO_INT (data);

	if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (check_button)))
	{
		add_stack_child (stack, num);
	}
	else
	{
		destroy_nth_stack_child (stack, num);
	}
}

static GtkCheckButton *
create_check_button (GtkStack *stack,
		     gint      num)
{
	gchar *title;
	GtkCheckButton *check_button;

	title = g_strdup_printf ("Stack child %d present", num);
	check_button = GTK_CHECK_BUTTON (gtk_check_button_new_with_label (title));
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (check_button), TRUE);
	g_object_set_data (G_OBJECT (check_button), "num", GINT_TO_POINTER (num));

	g_signal_connect (check_button,
			  "toggled",
			  G_CALLBACK (check_button_toggled_cb),
			  stack);

	g_free (title);
	return check_button;
}

static GtkGrid *
create_control_panel (GtkStack *stack)
{
	GtkGrid *vgrid;
	gint num;

	vgrid = GTK_GRID (gtk_grid_new ());
	gtk_orientable_set_orientation (GTK_ORIENTABLE (vgrid), GTK_ORIENTATION_VERTICAL);
	gtk_grid_set_row_spacing (vgrid, 6);

	for (num = 1; num <= N_STACK_CHILDREN; num++)
	{
		gtk_container_add (GTK_CONTAINER (vgrid),
				   GTK_WIDGET (create_check_button (stack, num)));
	}

	return vgrid;
}

static GtkWidget *
create_window_content (GtkStack *stack)
{
	GtkGrid *hgrid;

	hgrid = GTK_GRID (gtk_grid_new ());
	gtk_grid_set_column_spacing (hgrid, 18);

	gtk_container_add (GTK_CONTAINER (hgrid),
			   GTK_WIDGET (stack));

	gtk_container_add (GTK_CONTAINER (hgrid),
			   GTK_WIDGET (create_control_panel (stack)));

	gtk_widget_show_all (GTK_WIDGET (hgrid));
	return GTK_WIDGET (hgrid);
}

int
main (int    argc,
      char **argv)
{
	TeplMenuStackSwitcher *switcher;
	GtkStack *stack;
	GtkWindow *window;

	tepl_init ();
	gtk_init (&argc, &argv);

	switcher = tepl_menu_stack_switcher_new ();
	stack = create_stack ();
	tepl_menu_stack_switcher_set_stack (switcher, stack);

	window = GTK_WINDOW (gtk_window_new (GTK_WINDOW_TOPLEVEL));
	gtk_window_set_default_size (window, 800, 600);
	gtk_window_set_titlebar (window, GTK_WIDGET (create_header_bar (switcher)));
	gtk_container_add (GTK_CONTAINER (window), create_window_content (stack));
	gtk_widget_show (GTK_WIDGET (window));

	g_signal_connect (window,
			  "destroy",
			  G_CALLBACK (gtk_main_quit),
			  NULL);

	gtk_main ();
	tepl_finalize ();
	return EXIT_SUCCESS;
}
