/* SPDX-FileCopyrightText: 2023 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <tepl/tepl.h>
#include "tepl/tepl-panel-item-simple.h"

static void
test_simple (void)
{
	GtkWidget *label;
	TeplPanelItem *item;

	label = gtk_label_new (NULL);
	item = _tepl_panel_item_simple_new (label, "name", "Title", NULL);

	g_assert_true (tepl_panel_item_get_widget (item) == label);
	g_assert_cmpstr (tepl_panel_item_get_name (item), ==, "name");
	g_assert_cmpstr (tepl_panel_item_get_title (item), ==, "Title");
	g_assert_null (tepl_panel_item_get_icon_name (item));

	gtk_widget_destroy (label);
	g_assert_null (tepl_panel_item_get_widget (item));

	g_object_unref (item);
}

int
main (int    argc,
      char **argv)
{
	gtk_test_init (&argc, &argv);

	g_test_add_func ("/PanelItem/simple", test_simple);

	return g_test_run ();
}
