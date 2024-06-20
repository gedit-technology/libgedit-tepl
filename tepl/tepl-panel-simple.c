/* SPDX-FileCopyrightText: 2024 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "tepl-panel-simple.h"
#include "tepl-panel.h"

/**
 * SECTION:panel-simple
 * @Title: TeplPanelSimple
 * @Short_description: A set of #TeplPanelItem's
 *
 * #TeplPanelSimple is a simple implementation of the #TeplPanel interface,
 * containing a set of #TeplPanelItem's.
 *
 * By using this class, the #TeplPanelItem widgets are not yet packed into a
 * #GtkContainer. That is a job for other classes.
 */

struct _TeplPanelSimplePrivate
{
	/* Element type: owned "TeplPanelItem *" */
	GList *items;

	TeplPanelItem *active_item;
};

enum
{
	SIGNAL_ADD_ITEM,
	SIGNAL_REMOVE_ITEM,
	SIGNAL_CHANGED,
	N_SIGNALS
};

enum
{
	PROP_0,
	PROP_ACTIVE_ITEM,
	N_PROPERTIES
};

static guint signals[N_SIGNALS];
static GParamSpec *properties[N_PROPERTIES];

static void tepl_panel_interface_init (gpointer g_iface,
				       gpointer iface_data);

G_DEFINE_TYPE_WITH_CODE (TeplPanelSimple,
			 tepl_panel_simple,
			 G_TYPE_OBJECT,
			 G_ADD_PRIVATE (TeplPanelSimple)
			 G_IMPLEMENT_INTERFACE (TEPL_TYPE_PANEL,
						tepl_panel_interface_init))

static void
tepl_panel_simple_get_property (GObject    *object,
                                guint       prop_id,
                                GValue     *value,
                                GParamSpec *pspec)
{
	TeplPanelSimple *panel = TEPL_PANEL_SIMPLE (object);

	switch (prop_id)
	{
		case PROP_ACTIVE_ITEM:
			g_value_set_object (value, tepl_panel_simple_get_active_item (panel));
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
tepl_panel_simple_set_property (GObject      *object,
                                guint         prop_id,
                                const GValue *value,
                                GParamSpec   *pspec)
{
	TeplPanel *panel = TEPL_PANEL (object);

	switch (prop_id)
	{
		case PROP_ACTIVE_ITEM:
			tepl_panel_set_active (panel, g_value_get_object (value));
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
tepl_panel_simple_dispose (GObject *object)
{
	TeplPanelSimple *panel = TEPL_PANEL_SIMPLE (object);

	g_list_free_full (panel->priv->items, g_object_unref);
	panel->priv->items = NULL;

	g_clear_object (&panel->priv->active_item);

	G_OBJECT_CLASS (tepl_panel_simple_parent_class)->dispose (object);
}

static void
tepl_panel_simple_add_item (TeplPanelSimple *panel,
			    TeplPanelItem   *item)
{
	g_return_if_fail (g_list_find (panel->priv->items, item) == NULL);

	panel->priv->items = g_list_prepend (panel->priv->items,
					     g_object_ref (item));
}

static void
tepl_panel_simple_remove_item (TeplPanelSimple *panel,
			       TeplPanelItem   *item)
{
	GList *node;

	if (panel->priv->active_item == item)
	{
		tepl_panel_set_active (TEPL_PANEL (panel), NULL);
	}

	node = g_list_find (panel->priv->items, item);
	if (node == NULL)
	{
		return;
	}

	g_object_unref (item);
	panel->priv->items = g_list_delete_link (panel->priv->items, node);
}

static void
tepl_panel_simple_changed (TeplPanelSimple *panel)
{
	/* Do nothing. */
}

static void
tepl_panel_simple_class_init (TeplPanelSimpleClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	klass->add_item = tepl_panel_simple_add_item;
	klass->remove_item = tepl_panel_simple_remove_item;
	klass->changed = tepl_panel_simple_changed;

	object_class->get_property = tepl_panel_simple_get_property;
	object_class->set_property = tepl_panel_simple_set_property;
	object_class->dispose = tepl_panel_simple_dispose;

	/**
	 * TeplPanelSimple::add-item:
	 * @panel: the #TeplPanelSimple emitting the signal.
	 * @item: the #TeplPanelItem being added.
	 *
	 * The ::add-item signal is emitted when a #TeplPanelItem needs to be
	 * added.
	 *
	 * It is a %G_SIGNAL_RUN_LAST signal: the item is added by the
	 * #TeplPanelSimple's method handler for this signal.
	 *
	 * Since: 6.11
	 */
	signals[SIGNAL_ADD_ITEM] =
		g_signal_new ("add-item",
			      G_TYPE_FROM_CLASS (klass),
			      G_SIGNAL_RUN_LAST,
			      G_STRUCT_OFFSET (TeplPanelSimpleClass, add_item),
			      NULL, NULL, NULL,
			      G_TYPE_NONE, 1, TEPL_TYPE_PANEL_ITEM);

	/**
	 * TeplPanelSimple::remove-item:
	 * @panel: the #TeplPanelSimple emitting the signal.
	 * @item: the #TeplPanelItem being removed.
	 *
	 * The ::remove-item signal is emitted when a #TeplPanelItem needs to be
	 * removed.
	 *
	 * It is a %G_SIGNAL_RUN_LAST signal: the item is removed by the
	 * #TeplPanelSimple's method handler for this signal.
	 *
	 * Since: 6.11
	 */
	signals[SIGNAL_REMOVE_ITEM] =
		g_signal_new ("remove-item",
			      G_TYPE_FROM_CLASS (klass),
			      G_SIGNAL_RUN_LAST,
			      G_STRUCT_OFFSET (TeplPanelSimpleClass, remove_item),
			      NULL, NULL, NULL,
			      G_TYPE_NONE, 1, TEPL_TYPE_PANEL_ITEM);

	/**
	 * TeplPanelSimple::changed:
	 * @panel: the #TeplPanelSimple emitting the signal.
	 *
	 * The ::changed signal is for convenience: it is emitted when the
	 * #TeplPanelSimple::add-item and #TeplPanelSimple::remove-item signals
	 * are emitted, and also when the #TeplPanelSimple:active-item property
	 * changes.
	 *
	 * It is a %G_SIGNAL_RUN_FIRST signal: the action is already done.
	 *
	 * This signal is useful if you don't need the more detailed information
	 * on what has changed.
	 *
	 * Since: 6.11
	 */
	signals[SIGNAL_CHANGED] =
		g_signal_new ("changed",
			      G_TYPE_FROM_CLASS (klass),
			      G_SIGNAL_RUN_FIRST,
			      G_STRUCT_OFFSET (TeplPanelSimpleClass, changed),
			      NULL, NULL, NULL,
			      G_TYPE_NONE, 0);

	/**
	 * TeplPanelSimple:active-item:
	 *
	 * The #TeplPanelItem currently shown.
	 *
	 * Since: 6.11
	 */
	properties[PROP_ACTIVE_ITEM] =
		g_param_spec_object ("active-item",
				     "active-item",
				     "",
				     TEPL_TYPE_PANEL_ITEM,
				     G_PARAM_READWRITE |
				     G_PARAM_STATIC_STRINGS);

	g_object_class_install_properties (object_class, N_PROPERTIES, properties);
}

static void
tepl_panel_simple_init (TeplPanelSimple *panel)
{
	panel->priv = tepl_panel_simple_get_instance_private (panel);
}

static void
tepl_panel_simple_add (TeplPanel     *panel,
		       TeplPanelItem *item)
{
	g_object_ref (item);
	g_signal_emit (panel, signals[SIGNAL_ADD_ITEM], 0, item);
	g_signal_emit (panel, signals[SIGNAL_CHANGED], 0);
	g_object_unref (item);
}

static void
tepl_panel_simple_remove (TeplPanel     *panel,
			  TeplPanelItem *item)
{
	g_object_ref (item);
	g_signal_emit (panel, signals[SIGNAL_REMOVE_ITEM], 0, item);
	g_signal_emit (panel, signals[SIGNAL_CHANGED], 0);
	g_object_unref (item);
}

static void
tepl_panel_simple_set_active (TeplPanel     *panel,
			      TeplPanelItem *item)
{
	TeplPanelSimple *panel_simple = TEPL_PANEL_SIMPLE (panel);

	if (g_set_object (&panel_simple->priv->active_item, item))
	{
		g_object_notify_by_pspec (G_OBJECT (panel_simple),
					  properties[PROP_ACTIVE_ITEM]);

		g_signal_emit (panel, signals[SIGNAL_CHANGED], 0);
	}
}

static void
tepl_panel_interface_init (gpointer g_iface,
			   gpointer iface_data)
{
	TeplPanelInterface *interface = g_iface;

	interface->add = tepl_panel_simple_add;
	interface->remove = tepl_panel_simple_remove;
	interface->set_active = tepl_panel_simple_set_active;
}

/**
 * tepl_panel_simple_new:
 *
 * Returns: (transfer full): a new #TeplPanelSimple object.
 * Since: 6.11
 */
TeplPanelSimple *
tepl_panel_simple_new (void)
{
	return g_object_new (TEPL_TYPE_PANEL_SIMPLE, NULL);
}

/**
 * tepl_panel_simple_get_items:
 * @panel: a #TeplPanelSimple.
 *
 * Returns: (transfer full) (element-type TeplPanelItem): all the items of
 *   @panel.
 * Since: 6.11
 */
GList *
tepl_panel_simple_get_items (TeplPanelSimple *panel)
{
	GList *ret;
	GList *l;

	g_return_val_if_fail (TEPL_IS_PANEL_SIMPLE (panel), NULL);

	ret = g_list_copy (panel->priv->items);

	for (l = ret; l != NULL; l = l->next)
	{
		g_object_ref (l->data);
	}

	return ret;
}

/**
 * tepl_panel_simple_get_active_item:
 * @panel: a #TeplPanelSimple.
 *
 * Returns: (transfer none) (nullable): the value of the
 *   #TeplPanelSimple:active-item property.
 * Since: 6.11
 */
TeplPanelItem *
tepl_panel_simple_get_active_item (TeplPanelSimple *panel)
{
	g_return_val_if_fail (TEPL_IS_PANEL_SIMPLE (panel), NULL);
	return panel->priv->active_item;
}

/**
 * tepl_panel_simple_get_active_item_name:
 * @panel: a #TeplPanelSimple.
 *
 * A convenience function.
 *
 * Returns: (nullable): the name of the #TeplPanelSimple:active-item.
 * Since: 6.11
 */
const gchar *
tepl_panel_simple_get_active_item_name (TeplPanelSimple *panel)
{
	TeplPanelItem *item;

	g_return_val_if_fail (TEPL_IS_PANEL_SIMPLE (panel), NULL);

	item = tepl_panel_simple_get_active_item (panel);

	return item != NULL ? tepl_panel_item_get_name (item) : NULL;
}

/**
 * tepl_panel_simple_set_active_item_name:
 * @panel: a #TeplPanelSimple.
 * @name: (nullable): a name.
 *
 * A convenience function that calls tepl_panel_set_active() with the
 * #TeplPanelItem named @name (if found).
 *
 * Since: 6.11
 */
void
tepl_panel_simple_set_active_item_name (TeplPanelSimple *panel,
					const gchar     *name)
{
	GList *items;
	GList *l;

	g_return_if_fail (TEPL_IS_PANEL_SIMPLE (panel));

	if (name == NULL)
	{
		tepl_panel_set_active (TEPL_PANEL (panel), NULL);
		return;
	}

	items = tepl_panel_simple_get_items (panel);

	for (l = items; l != NULL; l = l->next)
	{
		TeplPanelItem *cur_item = TEPL_PANEL_ITEM (l->data);
		const gchar *cur_item_name;

		cur_item_name = tepl_panel_item_get_name (cur_item);

		if (g_strcmp0 (cur_item_name, name) == 0)
		{
			tepl_panel_set_active (TEPL_PANEL (panel), cur_item);
			break;
		}
	}

	g_list_free_full (items, g_object_unref);
}

/**
 * tepl_panel_simple_has_several_items:
 * @panel: a #TeplPanelSimple.
 *
 * A convenience function. Useful to implement a switcher widget.
 *
 * Returns: whether @panel has more than one #TeplPanelItem.
 * Since: 6.11
 */
gboolean
tepl_panel_simple_has_several_items (TeplPanelSimple *panel)
{
	g_return_val_if_fail (TEPL_IS_PANEL_SIMPLE (panel), FALSE);

	return (panel->priv->items != NULL &&
		panel->priv->items->next != NULL);
}
