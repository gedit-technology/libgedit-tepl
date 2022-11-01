/* SPDX-FileCopyrightText: 2022 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "tepl/tepl-container-helper.h"

static void
children_changed_cb (TeplContainerHelper *container_helper,
		     gboolean            *signal_received)
{
	*signal_received = TRUE;
}

static void
test_add_child (void)
{
	GtkStack *stack;
	TeplContainerHelper *container_helper;
	gboolean signal_received = FALSE;

	stack = GTK_STACK (gtk_stack_new ());
	g_object_ref_sink (stack);

	container_helper = _tepl_container_helper_new (GTK_CONTAINER (stack));
	g_signal_connect (container_helper,
			  "children-changed",
			  G_CALLBACK (children_changed_cb),
			  &signal_received);

	g_assert_false (signal_received);
	gtk_stack_add_titled (stack,
			      gtk_label_new (NULL),
			      "label",
			      "Label");
	g_assert_true (signal_received);

	gtk_widget_destroy (GTK_WIDGET (stack));
	g_object_unref (container_helper);
}

static void
test_remove_child (void)
{
	GtkStack *stack;
	TeplContainerHelper *container_helper;
	GtkWidget *label;
	gboolean signal_received = FALSE;

	stack = GTK_STACK (gtk_stack_new ());
	g_object_ref_sink (stack);

	container_helper = _tepl_container_helper_new (GTK_CONTAINER (stack));
	g_signal_connect (container_helper,
			  "children-changed",
			  G_CALLBACK (children_changed_cb),
			  &signal_received);

	label = gtk_label_new (NULL);
	gtk_stack_add_titled (stack, label, "label", "Label");

	signal_received = FALSE; /* reset */
	gtk_widget_destroy (label);
	g_assert_true (signal_received);

	gtk_widget_destroy (GTK_WIDGET (stack));
	g_object_unref (container_helper);
}

static void
test_change_child (void)
{
	GtkStack *stack;
	TeplContainerHelper *container_helper;
	GtkWidget *label;
	gboolean signal_received = FALSE;

	stack = GTK_STACK (gtk_stack_new ());
	g_object_ref_sink (stack);

	container_helper = _tepl_container_helper_new (GTK_CONTAINER (stack));
	g_signal_connect (container_helper,
			  "children-changed",
			  G_CALLBACK (children_changed_cb),
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
	g_object_unref (container_helper);
}

static void
test_change_initially_present_child (void)
{
	GtkStack *stack;
	GtkWidget *label;
	TeplContainerHelper *container_helper;
	gboolean signal_received = FALSE;

	stack = GTK_STACK (gtk_stack_new ());
	g_object_ref_sink (stack);

	label = gtk_label_new (NULL);
	gtk_stack_add_titled (stack, label, "label", "Label");

	/* We create the TeplContainerHelper *after* adding the child. */
	container_helper = _tepl_container_helper_new (GTK_CONTAINER (stack));
	g_signal_connect (container_helper,
			  "children-changed",
			  G_CALLBACK (children_changed_cb),
			  &signal_received);

	/* Now we change one of the GtkContainer child properties. */
	g_assert_false (signal_received);
	gtk_container_child_set (GTK_CONTAINER (stack),
				 label,
				 "title", "Label Changed",
				 NULL);
	g_assert_true (signal_received);

	gtk_widget_destroy (GTK_WIDGET (stack));
	g_object_unref (container_helper);
}

int
main (int    argc,
      char **argv)
{
	gtk_test_init (&argc, &argv);

	g_test_add_func ("/ContainerHelper/add_child", test_add_child);
	g_test_add_func ("/ContainerHelper/remove_child", test_remove_child);
	g_test_add_func ("/ContainerHelper/change_child", test_change_child);
	g_test_add_func ("/ContainerHelper/change_initially_present_child", test_change_initially_present_child);

	return g_test_run ();
}
