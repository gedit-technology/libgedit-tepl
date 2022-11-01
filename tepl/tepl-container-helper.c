/* SPDX-FileCopyrightText: 2022 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "tepl-container-helper.h"
#include "tepl-signal-group.h"

/* To add a convenience signal to GtkContainer. */

struct _TeplContainerHelperPrivate
{
	TeplSignalGroup *signal_group;
};

enum
{
	SIGNAL_CHILDREN_CHANGED,
	N_SIGNALS
};

static guint signals[N_SIGNALS];

G_DEFINE_TYPE_WITH_PRIVATE (TeplContainerHelper, _tepl_container_helper, G_TYPE_OBJECT)

static void
_tepl_container_helper_dispose (GObject *object)
{
	TeplContainerHelper *helper = TEPL_CONTAINER_HELPER (object);

	tepl_signal_group_clear (&helper->priv->signal_group);

	G_OBJECT_CLASS (_tepl_container_helper_parent_class)->dispose (object);
}

static void
_tepl_container_helper_class_init (TeplContainerHelperClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->dispose = _tepl_container_helper_dispose;

	/*
	 * TeplContainerHelper::children-changed:
	 * @helper: the #TeplContainerHelper emitting the signal.
	 *
	 * ::children-changed is a convenience signal that is emitted on:
	 * - #GtkContainer::add
	 * - #GtkContainer::remove
	 * - #GtkWidget::child-notify for all children part of the container.
	 *
	 * Possible use-case: implement a #GtkStack switcher, to know when the
	 * switcher content needs to be re-created.
	 */
	signals[SIGNAL_CHILDREN_CHANGED] =
		g_signal_new ("children-changed",
			      G_TYPE_FROM_CLASS (klass),
			      G_SIGNAL_RUN_FIRST,
			      0, NULL, NULL, NULL,
			      G_TYPE_NONE, 0);
}

static void
_tepl_container_helper_init (TeplContainerHelper *helper)
{
	helper->priv = _tepl_container_helper_get_instance_private (helper);
}

static void
children_changed (TeplContainerHelper *helper)
{
	g_signal_emit (helper, signals[SIGNAL_CHILDREN_CHANGED], 0);
}

static void
child_notify_cb (GtkWidget           *widget,
		 GParamSpec          *child_property,
		 TeplContainerHelper *helper)
{
	children_changed (helper);
}

static void
connect_child (TeplContainerHelper *helper,
	       GtkWidget           *widget)
{
	g_signal_connect (widget,
			  "child-notify",
			  G_CALLBACK (child_notify_cb),
			  helper);
}

static void
connect_all_children (TeplContainerHelper *helper,
		      GtkContainer        *container)
{
	GList *children;
	GList *l;

	children = gtk_container_get_children (container);

	for (l = children; l != NULL; l = l->next)
	{
		GtkWidget *child = GTK_WIDGET (l->data);
		connect_child (helper, child);
	}

	g_list_free (children);
}

static void
add_cb (GtkContainer        *container,
	GtkWidget           *widget,
	TeplContainerHelper *helper)
{
	connect_child (helper, widget);
	children_changed (helper);
}

static void
remove_cb (GtkContainer        *container,
	   GtkWidget           *widget,
	   TeplContainerHelper *helper)
{
	g_signal_handlers_disconnect_by_func (widget, child_notify_cb, helper);
	children_changed (helper);
}

/* Doesn't hold a reference to @container. */
TeplContainerHelper *
_tepl_container_helper_new (GtkContainer *container)
{
	TeplContainerHelper *helper;

	g_return_val_if_fail (GTK_IS_CONTAINER (container), NULL);

	helper = g_object_new (TEPL_TYPE_CONTAINER_HELPER, NULL);

	connect_all_children (helper, container);

	helper->priv->signal_group = tepl_signal_group_new (G_OBJECT (container));

	tepl_signal_group_add (helper->priv->signal_group,
			       g_signal_connect (container,
						 "add",
						 G_CALLBACK (add_cb),
						 helper));

	tepl_signal_group_add (helper->priv->signal_group,
			       g_signal_connect (container,
						 "remove",
						 G_CALLBACK (remove_cb),
						 helper));

	return helper;
}
