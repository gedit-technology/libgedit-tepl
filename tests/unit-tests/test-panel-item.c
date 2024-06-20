/* SPDX-FileCopyrightText: 2023-2024 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <tepl/tepl.h>

static void
test_basics (void)
{
	GtkWidget *label;
	TeplPanelItem *item;

	label = gtk_label_new (NULL);
	item = tepl_panel_item_new (label, "name", "Title", NULL, 0);

	g_assert_true (tepl_panel_item_get_widget (item) == label);
	g_assert_cmpstr (tepl_panel_item_get_name (item), ==, "name");
	g_assert_cmpstr (tepl_panel_item_get_title (item), ==, "Title");
	g_assert_null (tepl_panel_item_get_icon_name (item));

	gtk_widget_destroy (label);
	g_assert_null (tepl_panel_item_get_widget (item));

	g_object_unref (item);
}

static void
test_compare (void)
{
	GtkWidget *label1;
	GtkWidget *label2;
	TeplPanelItem *item1;
	TeplPanelItem *item2;

	label1 = gtk_label_new (NULL);
	label2 = gtk_label_new (NULL);
	g_object_ref_sink (label1);
	g_object_ref_sink (label2);

	/* All posititons at 0 */
	item1 = tepl_panel_item_new (label1, "name1", "A", NULL, 0);
	item2 = tepl_panel_item_new (label2, "name2", "B", NULL, 0);
	g_assert_cmpint (tepl_panel_item_compare_by_title (item1, item2), <, 0);
	g_assert_cmpint (tepl_panel_item_compare (item1, item2), <, 0);
	g_assert_cmpint (tepl_panel_item_compare_by_title (item2, item1), >, 0);
	g_assert_cmpint (tepl_panel_item_compare (item2, item1), >, 0);
	g_assert_cmpint (tepl_panel_item_compare_by_title (item1, item1), ==, 0);
	g_assert_cmpint (tepl_panel_item_compare (item1, item1), ==, 0);
	g_object_unref (item1);
	g_object_unref (item2);

	/* With different positions */
	item1 = tepl_panel_item_new (label1, "name1", "A", NULL, 1);
	item2 = tepl_panel_item_new (label2, "name2", "B", NULL, 0);
	g_assert_cmpint (tepl_panel_item_compare_by_title (item1, item2), <, 0);
	g_assert_cmpint (tepl_panel_item_compare (item1, item2), >, 0);
	g_assert_cmpint (tepl_panel_item_compare_by_title (item2, item1), >, 0);
	g_assert_cmpint (tepl_panel_item_compare (item2, item1), <, 0);
	g_assert_cmpint (tepl_panel_item_compare_by_title (item1, item1), ==, 0);
	g_assert_cmpint (tepl_panel_item_compare (item1, item1), ==, 0);
	g_object_unref (item1);
	g_object_unref (item2);

	g_object_unref (label1);
	g_object_unref (label2);
}

int
main (int    argc,
      char **argv)
{
	gtk_test_init (&argc, &argv);

	g_test_add_func ("/PanelItem/basics", test_basics);
	g_test_add_func ("/PanelItem/compare", test_compare);

	return g_test_run ();
}
