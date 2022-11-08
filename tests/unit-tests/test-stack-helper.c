/* SPDX-FileCopyrightText: 2022 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "tepl/tepl-stack-helper.h"

static void
stack_changed_cb (TeplStackHelper *stack_helper,
		  gboolean        *signal_received)
{
	*signal_received = TRUE;
}

static void
test_add_child (void)
{
	GtkStack *stack;
	TeplStackHelper *stack_helper;
	gboolean signal_received = FALSE;

	stack = GTK_STACK (gtk_stack_new ());
	g_object_ref_sink (stack);

	stack_helper = _tepl_stack_helper_new (stack);
	g_signal_connect (stack_helper,
			  "changed",
			  G_CALLBACK (stack_changed_cb),
			  &signal_received);

	g_assert_false (signal_received);
	gtk_stack_add_titled (stack,
			      gtk_label_new (NULL),
			      "label",
			      "Label");
	g_assert_true (signal_received);

	gtk_widget_destroy (GTK_WIDGET (stack));
	g_object_unref (stack_helper);
}

static void
test_remove_child (void)
{
	GtkStack *stack;
	TeplStackHelper *stack_helper;
	GtkWidget *label;
	gboolean signal_received = FALSE;

	stack = GTK_STACK (gtk_stack_new ());
	g_object_ref_sink (stack);

	stack_helper = _tepl_stack_helper_new (stack);
	g_signal_connect (stack_helper,
			  "changed",
			  G_CALLBACK (stack_changed_cb),
			  &signal_received);

	label = gtk_label_new (NULL);
	gtk_stack_add_titled (stack, label, "label", "Label");

	signal_received = FALSE; /* reset */
	gtk_widget_destroy (label);
	g_assert_true (signal_received);

	gtk_widget_destroy (GTK_WIDGET (stack));
	g_object_unref (stack_helper);
}

static void
test_change_child (void)
{
	GtkStack *stack;
	TeplStackHelper *stack_helper;
	GtkWidget *label;
	gboolean signal_received = FALSE;

	stack = GTK_STACK (gtk_stack_new ());
	g_object_ref_sink (stack);

	stack_helper = _tepl_stack_helper_new (stack);
	g_signal_connect (stack_helper,
			  "changed",
			  G_CALLBACK (stack_changed_cb),
			  &signal_received);

	label = gtk_label_new (NULL);
	gtk_stack_add_titled (stack, label, "label", "Label");

	signal_received = FALSE; /* reset */
	gtk_container_child_set (GTK_CONTAINER (stack),
				 label,
				 "title", "Label Changed",
				 NULL);
	g_assert_true (signal_received);

	gtk_widget_destroy (GTK_WIDGET (stack));
	g_object_unref (stack_helper);
}

static void
test_change_initially_present_child (void)
{
	GtkStack *stack;
	GtkWidget *label;
	TeplStackHelper *stack_helper;
	gboolean signal_received = FALSE;

	stack = GTK_STACK (gtk_stack_new ());
	g_object_ref_sink (stack);

	label = gtk_label_new (NULL);
	gtk_stack_add_titled (stack, label, "label", "Label");

	/* We create the TeplStackHelper *after* adding the child. */
	stack_helper = _tepl_stack_helper_new (stack);
	g_signal_connect (stack_helper,
			  "changed",
			  G_CALLBACK (stack_changed_cb),
			  &signal_received);

	/* Now we change one of the GtkContainer child properties. */
	g_assert_false (signal_received);
	gtk_container_child_set (GTK_CONTAINER (stack),
				 label,
				 "title", "Label Changed",
				 NULL);
	g_assert_true (signal_received);

	gtk_widget_destroy (GTK_WIDGET (stack));
	g_object_unref (stack_helper);
}

static void
test_set_visible_child (void)
{
	GtkStack *stack;
	GtkWidget *child1;
	GtkWidget *child2;
	TeplStackHelper *stack_helper;
	gboolean signal_received = FALSE;

	stack = GTK_STACK (gtk_stack_new ());
	g_object_ref_sink (stack);

	child1 = gtk_label_new (NULL);
	child2 = gtk_label_new (NULL);
	gtk_widget_show (child1);
	gtk_widget_show (child2);
	gtk_stack_add_titled (stack, child1, "child1", "Child 1");
	gtk_stack_add_titled (stack, child2, "child2", "Child 2");

	gtk_stack_set_visible_child (stack, child1);

	stack_helper = _tepl_stack_helper_new (stack);
	g_signal_connect (stack_helper,
			  "changed",
			  G_CALLBACK (stack_changed_cb),
			  &signal_received);

	/* child1 -> child2 */
	g_assert_false (signal_received);
	gtk_stack_set_visible_child (stack, child2);
	g_assert_true (signal_received);

	/* child2 -> child1 */
	signal_received = FALSE; /* reset */
	gtk_stack_set_visible_child (stack, child1);
	g_assert_true (signal_received);

	/* child1 -> set again child1.
	 * Note: if the signal *is* received, it's not the end of the world.
	 * It's just to have a more complete unit test.
	 */
	signal_received = FALSE; /* reset */
	gtk_stack_set_visible_child (stack, child1);
	g_assert_false (signal_received);
}

int
main (int    argc,
      char **argv)
{
	gtk_test_init (&argc, &argv);

	g_test_add_func ("/StackHelper/add_child", test_add_child);
	g_test_add_func ("/StackHelper/remove_child", test_remove_child);
	g_test_add_func ("/StackHelper/change_child", test_change_child);
	g_test_add_func ("/StackHelper/change_initially_present_child", test_change_initially_present_child);
	g_test_add_func ("/StackHelper/set_visible_child", test_set_visible_child);

	return g_test_run ();
}
