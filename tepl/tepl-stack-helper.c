/* SPDX-FileCopyrightText: 2022 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "tepl-stack-helper.h"
#include "tepl-signal-group.h"

struct _TeplStackHelperPrivate
{
	TeplSignalGroup *signal_group;
};

enum
{
	SIGNAL_CHANGED,
	N_SIGNALS
};

static guint signals[N_SIGNALS];

G_DEFINE_TYPE_WITH_PRIVATE (TeplStackHelper, _tepl_stack_helper, G_TYPE_OBJECT)

static void
_tepl_stack_helper_dispose (GObject *object)
{
	TeplStackHelper *helper = TEPL_STACK_HELPER (object);

	tepl_signal_group_clear (&helper->priv->signal_group);

	G_OBJECT_CLASS (_tepl_stack_helper_parent_class)->dispose (object);
}

static void
_tepl_stack_helper_class_init (TeplStackHelperClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->dispose = _tepl_stack_helper_dispose;

	/*
	 * TeplStackHelper::changed:
	 * @helper: the #TeplStackHelper emitting the signal.
	 *
	 * ::changed is a convenience signal that is emitted on:
	 * - #GtkContainer::add
	 * - #GtkContainer::remove
	 * - #GObject::notify for the #GtkStack:visible-child property.
	 * - #GtkWidget::child-notify for all children part of the container.
	 *
	 * Possible use-case: implement a #GtkStack switcher, to know when the
	 * switcher content needs to be re-created.
	 */
	signals[SIGNAL_CHANGED] =
		g_signal_new ("changed",
			      G_TYPE_FROM_CLASS (klass),
			      G_SIGNAL_RUN_FIRST,
			      0, NULL, NULL, NULL,
			      G_TYPE_NONE, 0);
}

static void
_tepl_stack_helper_init (TeplStackHelper *helper)
{
	helper->priv = _tepl_stack_helper_get_instance_private (helper);
}

static void
emit_changed (TeplStackHelper *helper)
{
	g_signal_emit (helper, signals[SIGNAL_CHANGED], 0);
}

static void
child_notify_cb (GtkWidget       *widget,
                 GParamSpec      *child_property,
                 TeplStackHelper *helper)
{
	emit_changed (helper);
}

static void
connect_child (TeplStackHelper *helper,
               GtkWidget       *widget)
{
	g_signal_connect (widget,
			  "child-notify",
			  G_CALLBACK (child_notify_cb),
			  helper);
}

static void
connect_all_children (TeplStackHelper *helper,
		      GtkStack        *stack)
{
	GList *children;
	GList *l;

	children = gtk_container_get_children (GTK_CONTAINER (stack));

	for (l = children; l != NULL; l = l->next)
	{
		GtkWidget *child = GTK_WIDGET (l->data);
		connect_child (helper, child);
	}

	g_list_free (children);
}

static void
add_cb (GtkContainer    *container,
        GtkWidget       *widget,
        TeplStackHelper *helper)
{
	connect_child (helper, widget);
	emit_changed (helper);
}

static void
remove_cb (GtkContainer    *container,
           GtkWidget       *widget,
           TeplStackHelper *helper)
{
	g_signal_handlers_disconnect_by_func (widget, child_notify_cb, helper);
	emit_changed (helper);
}

static void
visible_child_notify_cb (GtkStack        *stack,
			 GParamSpec      *pspec,
			 TeplStackHelper *helper)
{
	emit_changed (helper);
}

/* Doesn't hold a reference to @stack. */
TeplStackHelper *
_tepl_stack_helper_new (GtkStack *stack)
{
	TeplStackHelper *helper;

	g_return_val_if_fail (GTK_IS_STACK (stack), NULL);

	helper = g_object_new (TEPL_TYPE_STACK_HELPER, NULL);

	connect_all_children (helper, stack);

	helper->priv->signal_group = tepl_signal_group_new (G_OBJECT (stack));

	tepl_signal_group_add (helper->priv->signal_group,
			       g_signal_connect (stack,
						 "add",
						 G_CALLBACK (add_cb),
						 helper));

	tepl_signal_group_add (helper->priv->signal_group,
			       g_signal_connect (stack,
						 "remove",
						 G_CALLBACK (remove_cb),
						 helper));

	tepl_signal_group_add (helper->priv->signal_group,
			       g_signal_connect (stack,
						 "notify::visible-child",
						 G_CALLBACK (visible_child_notify_cb),
						 helper));

	return helper;
}
