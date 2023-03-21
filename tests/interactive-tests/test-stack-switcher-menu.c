/* SPDX-FileCopyrightText: 2023 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <tepl/tepl.h>
#include <stdlib.h>

static TeplStack *
create_stack (void)
{
	TeplStack *stack;
	GtkWidget *label;
	TeplStackItem *item;

	stack = tepl_stack_new ();

	label = gtk_label_new ("Widget 1");
	item = tepl_stack_item_new (label, "name1", "Title 1", NULL);
	tepl_stack_add_item (stack, item);
	tepl_stack_set_visible_item (stack, item);
	g_object_unref (item);

	label = gtk_label_new ("Widget 2");
	item = tepl_stack_item_new (label, "name2", "Title 2", NULL);
	tepl_stack_add_item (stack, item);
	g_object_unref (item);

	gtk_widget_show (GTK_WIDGET (stack));
	return stack;
}

int
main (int    argc,
      char **argv)
{
	GtkWidget *window;
	TeplStack *stack;
	GtkHeaderBar *headerbar;
	TeplStackSwitcherMenu *stack_switcher_menu;

	tepl_init ();
	gtk_init (&argc, &argv);

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size (GTK_WINDOW (window), 600, 400);

	stack = create_stack ();
	gtk_container_add (GTK_CONTAINER (window), GTK_WIDGET (stack));

	headerbar = GTK_HEADER_BAR (gtk_header_bar_new ());
	gtk_header_bar_set_show_close_button (headerbar, TRUE);
	gtk_header_bar_set_title (headerbar, "TeplStackSwitcherMenu Test");
	stack_switcher_menu = tepl_stack_switcher_menu_new (stack);
	gtk_header_bar_pack_start (headerbar, GTK_WIDGET (stack_switcher_menu));
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
