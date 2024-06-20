/* SPDX-FileCopyrightText: 2023 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <tepl/tepl.h>

static void
test_basics (void)
{
	TeplPanelContainer *container;
	GtkWidget *label1;
	GtkWidget *label2;
	TeplPanelItem *item1;
	TeplPanelItem *item2;
	GList *items;

	container = tepl_panel_container_new ();
	g_object_ref_sink (container);

	/* Initially empty */
	g_assert_null (tepl_panel_container_get_items (container));
	g_assert_null (tepl_panel_container_get_active_item (container));
	g_assert_false (tepl_panel_container_has_several_items (container));

	/* Add an item */
	label1 = gtk_label_new (NULL);
	item1 = tepl_panel_item_new (label1, "name1", "Title 1", NULL, 0);
	tepl_panel_add (TEPL_PANEL (container), item1);

	items = tepl_panel_container_get_items (container);
	g_assert_cmpint (g_list_length (items), ==, 1);
	g_assert_true (items->data == item1);
	g_list_free_full (items, g_object_unref);

	g_assert_true (tepl_panel_container_get_active_item (container) == item1);
	g_assert_false (tepl_panel_container_has_several_items (container));

	/* Add one more item */
	label2 = gtk_label_new (NULL);
	item2 = tepl_panel_item_new (label2, "name2", "Title 2", "icon-name", 0);
	tepl_panel_add (TEPL_PANEL (container), item2);

	items = tepl_panel_container_get_items (container);
	g_assert_cmpint (g_list_length (items), ==, 2);
	g_list_free_full (items, g_object_unref);

	tepl_panel_set_active (TEPL_PANEL (container), item2);
	g_assert_true (tepl_panel_container_get_active_item (container) == item2);

	tepl_panel_set_active (TEPL_PANEL (container), item1);
	g_assert_true (tepl_panel_container_get_active_item (container) == item1);

	g_assert_true (tepl_panel_container_has_several_items (container));

	/* Remove an item */
	tepl_panel_remove (TEPL_PANEL (container), item1);
	g_object_unref (item1);

	items = tepl_panel_container_get_items (container);
	g_assert_cmpint (g_list_length (items), ==, 1);
	g_assert_true (items->data == item2);
	g_list_free_full (items, g_object_unref);

	g_assert_true (tepl_panel_container_get_active_item (container) == item2);
	g_assert_false (tepl_panel_container_has_several_items (container));

	/* Remove last item, empty again */
	tepl_panel_remove (TEPL_PANEL (container), item2);
	g_object_unref (item2);

	g_assert_null (tepl_panel_container_get_items (container));
	g_assert_null (tepl_panel_container_get_active_item (container));
	g_assert_false (tepl_panel_container_has_several_items (container));

	g_object_unref (container);
}

int
main (int    argc,
      char **argv)
{
	gtk_test_init (&argc, &argv);

	g_test_add_func ("/PanelContainer/basics", test_basics);

	return g_test_run ();
}
