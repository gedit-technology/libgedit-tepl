/* SPDX-FileCopyrightText: 2023-2024 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <tepl/tepl.h>

static void
test_basics (void)
{
	TeplPanelSimple *panel;
	GtkWidget *label1;
	GtkWidget *label2;
	TeplPanelItem *item1;
	TeplPanelItem *item2;
	GList *items;

	panel = tepl_panel_simple_new ();

	/* Initially empty */
	g_assert_null (tepl_panel_simple_get_items (panel));
	g_assert_null (tepl_panel_simple_get_active_item (panel));
	g_assert_false (tepl_panel_simple_has_several_items (panel));

	/* Add an item */
	label1 = gtk_label_new (NULL);
	item1 = tepl_panel_item_new (label1, "name1", "Title 1", NULL, 0);
	tepl_panel_add (TEPL_PANEL (panel), item1);

	items = tepl_panel_simple_get_items (panel);
	g_assert_cmpint (g_list_length (items), ==, 1);
	g_assert_true (items->data == item1);
	g_list_free_full (items, g_object_unref);

	g_assert_null (tepl_panel_simple_get_active_item (panel));
	g_assert_false (tepl_panel_simple_has_several_items (panel));

	tepl_panel_set_active (TEPL_PANEL (panel), item1);
	g_assert_true (tepl_panel_simple_get_active_item (panel) == item1);

	/* Add one more item */
	label2 = gtk_label_new (NULL);
	item2 = tepl_panel_item_new (label2, "name2", "Title 2", "icon-name", 0);
	tepl_panel_add (TEPL_PANEL (panel), item2);

	items = tepl_panel_simple_get_items (panel);
	g_assert_cmpint (g_list_length (items), ==, 2);
	g_list_free_full (items, g_object_unref);

	g_assert_true (tepl_panel_simple_get_active_item (panel) == item1);
	tepl_panel_set_active (TEPL_PANEL (panel), item2);
	g_assert_true (tepl_panel_simple_get_active_item (panel) == item2);

	tepl_panel_set_active (TEPL_PANEL (panel), item1);
	g_assert_true (tepl_panel_simple_get_active_item (panel) == item1);

	g_assert_true (tepl_panel_simple_has_several_items (panel));

	/* Remove an item */
	tepl_panel_remove (TEPL_PANEL (panel), item1);
	g_object_unref (item1);

	items = tepl_panel_simple_get_items (panel);
	g_assert_cmpint (g_list_length (items), ==, 1);
	g_assert_true (items->data == item2);
	g_list_free_full (items, g_object_unref);

	g_assert_null (tepl_panel_simple_get_active_item (panel));
	g_assert_false (tepl_panel_simple_has_several_items (panel));

	/* Remove last item, empty again */
	tepl_panel_remove (TEPL_PANEL (panel), item2);
	g_object_unref (item2);

	g_assert_null (tepl_panel_simple_get_items (panel));
	g_assert_null (tepl_panel_simple_get_active_item (panel));
	g_assert_false (tepl_panel_simple_has_several_items (panel));

	g_object_unref (panel);
}

int
main (int    argc,
      char **argv)
{
	gtk_test_init (&argc, &argv);

	g_test_add_func ("/PanelSimple/basics", test_basics);

	return g_test_run ();
}
