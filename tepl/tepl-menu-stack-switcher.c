/* SPDX-FileCopyrightText: 2014 - Steve Frécinaux
 * SPDX-FileCopyrightText: 2022 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "tepl-menu-stack-switcher.h"
#include "tepl-stack-helper.h"

/**
 * SECTION:menu-stack-switcher
 * @Short_description: A #GtkStack switcher based on #GtkMenuButton
 * @Title: TeplMenuStackSwitcher
 *
 * #TeplMenuStackSwitcher is a #GtkStack switcher based on #GtkMenuButton.
 *
 * #TeplMenuStackSwitcher is intended to be used in a #GtkHeaderBar, although
 * the API could be extended to support other use cases.
 *
 * It's an alternative to #GtkStackSwitcher and has a similar API.
 */

struct _TeplMenuStackSwitcherPrivate
{
	GtkStack *stack;
	TeplStackHelper *stack_helper;

	/* The title of the GtkMenuButton (the title of the currently visible
	 * child of the stack).
	 */
	GtkLabel *title;

	/* Part of the GtkPopover. */
	GtkWidget *button_box;
	GHashTable *buttons;

	guint in_visible_child_notify : 1;
};

enum
{
	PROP_0,
	PROP_STACK,
	N_PROPERTIES
};

static GParamSpec *properties[N_PROPERTIES];

G_DEFINE_TYPE_WITH_PRIVATE (TeplMenuStackSwitcher, tepl_menu_stack_switcher, GTK_TYPE_MENU_BUTTON)

#define STACK_CHILD_KEY "stack-child-key"

static void
update_title_label (TeplMenuStackSwitcher *switcher)
{
	gchar *title = NULL;

	if (switcher->priv->stack != NULL)
	{
		GtkWidget *stack_child = gtk_stack_get_visible_child (switcher->priv->stack);

		if (stack_child != NULL)
		{
			gtk_container_child_get (GTK_CONTAINER (switcher->priv->stack),
						 stack_child,
						 "title", &title,
						 NULL);
		}
	}

	gtk_label_set_label (switcher->priv->title, title);
	g_free (title);
}

static void
clear_button_box (TeplMenuStackSwitcher *switcher)
{
	gtk_container_foreach (GTK_CONTAINER (switcher->priv->button_box),
			       (GtkCallback) gtk_widget_destroy,
			       switcher);
}

static void
on_button_clicked (GtkButton             *button,
                   TeplMenuStackSwitcher *switcher)
{
	if (!switcher->priv->in_visible_child_notify)
	{
		GtkWidget *stack_child;
		GtkPopover *popover;

		stack_child = g_object_get_data (G_OBJECT (button), STACK_CHILD_KEY);
		gtk_stack_set_visible_child (switcher->priv->stack, stack_child);

		popover = gtk_menu_button_get_popover (GTK_MENU_BUTTON (switcher));
		gtk_widget_hide (GTK_WIDGET (popover));
	}
}

static void
update_button (TeplMenuStackSwitcher *switcher,
	       GtkWidget             *stack_child,
	       GtkButton             *button)
{
	GList *children;

	/* We get spurious notifications while the stack is being destroyed, so
	 * for now check the child actually exists.
	 */
	children = gtk_container_get_children (GTK_CONTAINER (switcher->priv->stack));
	if (g_list_index (children, stack_child) >= 0)
	{
		gchar *title;
		gboolean visible;

		gtk_container_child_get (GTK_CONTAINER (switcher->priv->stack),
					 stack_child,
					 "title", &title,
					 NULL);

		gtk_button_set_label (button, title);

		visible = (gtk_widget_get_visible (stack_child) &&
			   title != NULL);
		gtk_widget_set_visible (GTK_WIDGET (button), visible);

		gtk_widget_set_size_request (GTK_WIDGET (button), 100, -1);

		g_free (title);
	}

	g_list_free (children);
}

static void
on_title_visible_updated (GtkWidget             *stack_child,
			  GParamSpec            *pspec,
			  TeplMenuStackSwitcher *switcher)
{
	GtkButton *button;

	button = g_hash_table_lookup (switcher->priv->buttons, stack_child);
	update_button (switcher, stack_child, button);
}

static void
on_position_updated (GtkWidget             *stack_child,
		     GParamSpec            *pspec,
		     TeplMenuStackSwitcher *switcher)
{
	GtkWidget *button;
	gint position;

	button = g_hash_table_lookup (switcher->priv->buttons, stack_child);

	gtk_container_child_get (GTK_CONTAINER (switcher->priv->stack),
				 stack_child,
				 "position", &position,
				 NULL);

	/* TODO: fix bug with the position not taken into account *initially*
	 * when populating the button_box.
	 */
	gtk_box_reorder_child (GTK_BOX (switcher->priv->button_box), button, position);
}

static void
add_child (TeplMenuStackSwitcher *switcher,
	   GtkWidget             *stack_child)
{
	GtkRadioButton *button;
	GList *group;

	button = GTK_RADIO_BUTTON (gtk_radio_button_new (NULL));
	gtk_toggle_button_set_mode (GTK_TOGGLE_BUTTON (button), FALSE);
	gtk_widget_set_valign (GTK_WIDGET (button), GTK_ALIGN_CENTER);

	update_button (switcher, stack_child, GTK_BUTTON (button));

	group = gtk_container_get_children (GTK_CONTAINER (switcher->priv->button_box));
	if (group != NULL)
	{
		gtk_radio_button_join_group (button, GTK_RADIO_BUTTON (group->data));
		g_list_free (group);
	}

	gtk_container_add (GTK_CONTAINER (switcher->priv->button_box),
			   GTK_WIDGET (button));

	g_object_set_data (G_OBJECT (button), STACK_CHILD_KEY, stack_child);
	g_signal_connect (button, "clicked", G_CALLBACK (on_button_clicked), switcher);
	g_signal_connect (stack_child, "notify::visible", G_CALLBACK (on_title_visible_updated), switcher);
	g_signal_connect (stack_child, "child-notify::title", G_CALLBACK (on_title_visible_updated), switcher);
	g_signal_connect (stack_child, "child-notify::position", G_CALLBACK (on_position_updated), switcher);

	g_hash_table_insert (switcher->priv->buttons, stack_child, button);
}

static void
populate_button_box (TeplMenuStackSwitcher *switcher)
{
	GList *children;
	GList *l;

	children = gtk_container_get_children (GTK_CONTAINER (switcher->priv->stack));
	for (l = children; l != NULL; l = l->next)
	{
		GtkWidget *stack_child = GTK_WIDGET (l->data);

		add_child (switcher, stack_child);
	}
	g_list_free (children);
}

static void
stack_visible_child_notify_cb (GtkStack              *stack,
			       GParamSpec            *pspec,
			       TeplMenuStackSwitcher *switcher)
{
	GtkWidget *visible_child;
	GtkToggleButton *button;

	visible_child = gtk_stack_get_visible_child (stack);
	if (visible_child == NULL)
	{
		return;
	}

	button = g_hash_table_lookup (switcher->priv->buttons, visible_child);
	if (button != NULL)
	{
		switcher->priv->in_visible_child_notify = TRUE;
		gtk_toggle_button_set_active (button, TRUE);
		switcher->priv->in_visible_child_notify = FALSE;
	}
}

static void
on_stack_child_added (GtkStack              *stack,
		      GtkWidget             *stack_child,
		      TeplMenuStackSwitcher *switcher)
{
	add_child (switcher, stack_child);
}

static void
on_stack_child_removed (GtkStack              *stack,
			GtkWidget             *stack_child,
			TeplMenuStackSwitcher *switcher)
{
	GtkButton *button;

	g_signal_handlers_disconnect_by_func (stack_child, on_title_visible_updated, switcher);
	g_signal_handlers_disconnect_by_func (stack_child, on_position_updated, switcher);

	button = g_hash_table_lookup (switcher->priv->buttons, stack_child);
	gtk_container_remove (GTK_CONTAINER (switcher->priv->button_box),
			      GTK_WIDGET (button));
	g_hash_table_remove (switcher->priv->buttons, stack_child);
}

static void
disconnect_stack_signals (TeplMenuStackSwitcher *switcher)
{
	g_signal_handlers_disconnect_by_func (switcher->priv->stack, on_stack_child_added, switcher);
	g_signal_handlers_disconnect_by_func (switcher->priv->stack, on_stack_child_removed, switcher);
	g_signal_handlers_disconnect_by_func (switcher->priv->stack, stack_visible_child_notify_cb, switcher);
	g_signal_handlers_disconnect_by_func (switcher->priv->stack, disconnect_stack_signals, switcher);
}

static void
connect_stack_signals (TeplMenuStackSwitcher *switcher)
{
	g_signal_connect (switcher->priv->stack,
			  "add",
			  G_CALLBACK (on_stack_child_added),
			  switcher);

	g_signal_connect (switcher->priv->stack,
			  "remove",
			  G_CALLBACK (on_stack_child_removed),
			  switcher);

	g_signal_connect (switcher->priv->stack,
			  "notify::visible-child",
			  G_CALLBACK (stack_visible_child_notify_cb),
			  switcher);

	g_signal_connect_swapped (switcher->priv->stack,
				  "destroy",
				  G_CALLBACK (disconnect_stack_signals),
				  switcher);
}

static void
create_menu_button_title (TeplMenuStackSwitcher *switcher)
{
	GtkGrid *hgrid;
	GtkStyleContext *style_context;
	GtkWidget *down_arrow;

	hgrid = GTK_GRID (gtk_grid_new ());
	gtk_grid_set_column_spacing (hgrid, 6);

	switcher->priv->title = GTK_LABEL (gtk_label_new (NULL));
	gtk_container_add (GTK_CONTAINER (hgrid),
			   GTK_WIDGET (switcher->priv->title));

	/* To use a TeplMenuStackSwitcher in a GtkHeaderBar.
	 * If we want to use it outside an headerbar, this should be made
	 * configurable with additional API.
	 */
	style_context = gtk_widget_get_style_context (GTK_WIDGET (switcher->priv->title));
	gtk_style_context_add_class (style_context, GTK_STYLE_CLASS_TITLE);

	down_arrow = gtk_image_new_from_icon_name ("pan-down-symbolic", GTK_ICON_SIZE_BUTTON);
	gtk_container_add (GTK_CONTAINER (hgrid), down_arrow);

	gtk_widget_set_valign (GTK_WIDGET (hgrid), GTK_ALIGN_CENTER);
	gtk_widget_show_all (GTK_WIDGET (hgrid));
	gtk_container_add (GTK_CONTAINER (switcher),
			   GTK_WIDGET (hgrid));
}

static void
create_popover (TeplMenuStackSwitcher *switcher)
{
	GtkPopover *popover;

	popover = GTK_POPOVER (gtk_popover_new (GTK_WIDGET (switcher)));
	gtk_popover_set_position (popover, GTK_POS_BOTTOM);

	switcher->priv->button_box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 6);
	gtk_widget_show (switcher->priv->button_box);
	gtk_container_add (GTK_CONTAINER (popover), switcher->priv->button_box);

	switcher->priv->buttons = g_hash_table_new (g_direct_hash, g_direct_equal);

	gtk_menu_button_set_popover (GTK_MENU_BUTTON (switcher),
				     GTK_WIDGET (popover));
}

static void
stack_changed_cb (TeplStackHelper       *stack_helper,
		  TeplMenuStackSwitcher *switcher)
{
	update_title_label (switcher);
}

static void
update_stack_helper (TeplMenuStackSwitcher *switcher)
{
	if (switcher->priv->stack_helper != NULL)
	{
		g_signal_handlers_disconnect_by_func (switcher->priv->stack_helper,
						      stack_changed_cb,
						      switcher);

		g_clear_object (&switcher->priv->stack_helper);
	}

	if (switcher->priv->stack != NULL)
	{
		switcher->priv->stack_helper = _tepl_stack_helper_new (switcher->priv->stack);

		g_signal_connect (switcher->priv->stack_helper,
				  "changed",
				  G_CALLBACK (stack_changed_cb),
				  switcher);
	}
}

static void
tepl_menu_stack_switcher_get_property (GObject    *object,
				       guint       prop_id,
				       GValue     *value,
				       GParamSpec *pspec)
{
	TeplMenuStackSwitcher *switcher = TEPL_MENU_STACK_SWITCHER (object);

	switch (prop_id)
	{
		case PROP_STACK:
			g_value_set_object (value, tepl_menu_stack_switcher_get_stack (switcher));
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
tepl_menu_stack_switcher_set_property (GObject      *object,
				       guint         prop_id,
				       const GValue *value,
				       GParamSpec   *pspec)
{
	TeplMenuStackSwitcher *switcher = TEPL_MENU_STACK_SWITCHER (object);

	switch (prop_id)
	{
		case PROP_STACK:
			tepl_menu_stack_switcher_set_stack (switcher, g_value_get_object (value));
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
tepl_menu_stack_switcher_dispose (GObject *object)
{
	TeplMenuStackSwitcher *switcher = TEPL_MENU_STACK_SWITCHER (object);

	/* TODO: don't call a public function inside dispose(). */
	tepl_menu_stack_switcher_set_stack (switcher, NULL);

	G_OBJECT_CLASS (tepl_menu_stack_switcher_parent_class)->dispose (object);
}

static void
tepl_menu_stack_switcher_finalize (GObject *object)
{
	TeplMenuStackSwitcher *switcher = TEPL_MENU_STACK_SWITCHER (object);

	g_hash_table_destroy (switcher->priv->buttons);

	G_OBJECT_CLASS (tepl_menu_stack_switcher_parent_class)->finalize (object);
}

static void
tepl_menu_stack_switcher_class_init (TeplMenuStackSwitcherClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->get_property = tepl_menu_stack_switcher_get_property;
	object_class->set_property = tepl_menu_stack_switcher_set_property;
	object_class->dispose = tepl_menu_stack_switcher_dispose;
	object_class->finalize = tepl_menu_stack_switcher_finalize;

	/**
	 * TeplMenuStackSwitcher:stack:
	 *
	 * The current #GtkStack that the switcher controls.
	 *
	 * Since: 6.4
	 */
	properties[PROP_STACK] =
		g_param_spec_object ("stack",
				     "stack",
				     "",
				     GTK_TYPE_STACK,
				     G_PARAM_READWRITE |
				     G_PARAM_CONSTRUCT |
				     G_PARAM_STATIC_STRINGS);

	g_object_class_install_properties (object_class, N_PROPERTIES, properties);
}

static void
tepl_menu_stack_switcher_init (TeplMenuStackSwitcher *switcher)
{
	switcher->priv = tepl_menu_stack_switcher_get_instance_private (switcher);

	create_menu_button_title (switcher);
	create_popover (switcher);
}

/**
 * tepl_menu_stack_switcher_new:
 *
 * Returns: (transfer floating): a new #TeplMenuStackSwitcher widget.
 * Since: 6.4
 */
TeplMenuStackSwitcher *
tepl_menu_stack_switcher_new (void)
{
	return g_object_new (TEPL_TYPE_MENU_STACK_SWITCHER, NULL);
}

/**
 * tepl_menu_stack_switcher_set_stack:
 * @switcher: a #TeplMenuStackSwitcher.
 * @stack: (nullable): a #GtkStack, or %NULL.
 *
 * Sets the #TeplMenuStackSwitcher:stack property.
 *
 * Since: 6.4
 */
void
tepl_menu_stack_switcher_set_stack (TeplMenuStackSwitcher *switcher,
				    GtkStack              *stack)
{
	g_return_if_fail (TEPL_IS_MENU_STACK_SWITCHER (switcher));
	g_return_if_fail (stack == NULL || GTK_IS_STACK (stack));

	if (switcher->priv->stack == stack)
	{
		return;
	}

	if (switcher->priv->stack != NULL)
	{
		disconnect_stack_signals (switcher);
		clear_button_box (switcher);
		g_clear_object (&switcher->priv->stack);
	}

	if (stack != NULL)
	{
		switcher->priv->stack = g_object_ref_sink (stack);
		populate_button_box (switcher);
		connect_stack_signals (switcher);
	}

	update_stack_helper (switcher);
	update_title_label (switcher);

	/* TODO: look if it is really necessary, since the widgets inside anyway
	 * change.
	 */
	gtk_widget_queue_resize (GTK_WIDGET (switcher));

	g_object_notify_by_pspec (G_OBJECT (switcher), properties[PROP_STACK]);
}

/**
 * tepl_menu_stack_switcher_get_stack:
 * @switcher: a #TeplMenuStackSwitcher.
 *
 * Returns: (transfer none) (nullable): the value of the
 *   #TeplMenuStackSwitcher:stack property.
 * Since: 6.4
 */
GtkStack *
tepl_menu_stack_switcher_get_stack (TeplMenuStackSwitcher *switcher)
{
	g_return_val_if_fail (TEPL_IS_MENU_STACK_SWITCHER (switcher), NULL);

	return switcher->priv->stack;
}
