/*
 * This file is part of Tepl, a text editor library.
 *
 * Copyright 2016, 2017 - Sébastien Wilmet <swilmet@gnome.org>
 *
 * Tepl is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation; either version 2.1 of the License, or (at your
 * option) any later version.
 *
 * Tepl is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#include "tepl-menu-shell.h"

/**
 * SECTION:menu-shell
 * @Short_description: An extension of GtkMenuShell
 * @Title: TeplMenuShell
 *
 * #TeplMenuShell extends the #GtkMenuShell abstract class with the
 * #TeplMenuShell::menu-item-selected and #TeplMenuShell::menu-item-deselected
 * convenience signals.
 *
 * One possible use-case is to push/pop longer descriptions of menu items to a
 * #GtkStatusbar, exactly like
 * tepl_application_window_connect_menu_to_statusbar() does.
 */

struct _TeplMenuShellPrivate
{
	GtkMenuShell *gtk_menu_shell;
};

enum
{
	PROP_0,
	PROP_MENU_SHELL,
	N_PROPERTIES
};

enum
{
	SIGNAL_MENU_ITEM_SELECTED,
	SIGNAL_MENU_ITEM_DESELECTED,
	N_SIGNALS
};

#define TEPL_MENU_SHELL_KEY "tepl-menu-shell-key"

static GParamSpec *properties[N_PROPERTIES];
static guint signals[N_SIGNALS];

G_DEFINE_TYPE_WITH_PRIVATE (TeplMenuShell, tepl_menu_shell, G_TYPE_OBJECT)

/* Prototypes */
static void connect_menu_shell		(TeplMenuShell *tepl_menu_shell,
					 GtkMenuShell  *gtk_menu_shell);

static void disconnect_menu_shell	(TeplMenuShell *tepl_menu_shell,
					 GtkMenuShell  *gtk_menu_shell);

static void
menu_item_select_cb (GtkMenuItem *menu_item,
		     gpointer     user_data)
{
	TeplMenuShell *tepl_menu_shell = TEPL_MENU_SHELL (user_data);
	GtkWidget *submenu;

	submenu = gtk_menu_item_get_submenu (menu_item);

	if (GTK_IS_MENU_SHELL (submenu))
	{
		connect_menu_shell (tepl_menu_shell, GTK_MENU_SHELL (submenu));
	}

	g_signal_emit (tepl_menu_shell,
		       signals[SIGNAL_MENU_ITEM_SELECTED], 0,
		       menu_item);
}

static void
menu_item_deselect_cb (GtkMenuItem *menu_item,
		       gpointer     user_data)
{
	TeplMenuShell *tepl_menu_shell = TEPL_MENU_SHELL (user_data);
	GtkWidget *submenu;

	submenu = gtk_menu_item_get_submenu (menu_item);

	if (GTK_IS_MENU_SHELL (submenu))
	{
		disconnect_menu_shell (tepl_menu_shell, GTK_MENU_SHELL (submenu));
	}

	g_signal_emit (tepl_menu_shell,
		       signals[SIGNAL_MENU_ITEM_DESELECTED], 0,
		       menu_item);
}

static void
connect_menu_item (TeplMenuShell *tepl_menu_shell,
		   GtkMenuItem   *menu_item)
{
	g_signal_connect_object (menu_item,
				 "select",
				 G_CALLBACK (menu_item_select_cb),
				 tepl_menu_shell,
				 0);

	g_signal_connect_object (menu_item,
				 "deselect",
				 G_CALLBACK (menu_item_deselect_cb),
				 tepl_menu_shell,
				 0);
}

static void
disconnect_menu_item (TeplMenuShell *tepl_menu_shell,
		      GtkMenuItem   *menu_item)
{
	g_signal_handlers_disconnect_by_func (menu_item,
					      menu_item_select_cb,
					      tepl_menu_shell);

	g_signal_handlers_disconnect_by_func (menu_item,
					      menu_item_deselect_cb,
					      tepl_menu_shell);
}

static void
insert_cb (GtkMenuShell *gtk_menu_shell,
	   GtkWidget    *child,
	   gint          position,
	   gpointer      user_data)
{
	TeplMenuShell *tepl_menu_shell = TEPL_MENU_SHELL (user_data);

	if (GTK_IS_MENU_ITEM (child))
	{
		connect_menu_item (tepl_menu_shell, GTK_MENU_ITEM (child));
	}
}

static void
remove_cb (GtkContainer *container,
	   GtkWidget    *child,
	   gpointer      user_data)
{
	TeplMenuShell *tepl_menu_shell = TEPL_MENU_SHELL (user_data);

	if (GTK_IS_MENU_ITEM (child))
	{
		disconnect_menu_item (tepl_menu_shell, GTK_MENU_ITEM (child));
	}
}

static void
connect_menu_shell (TeplMenuShell *tepl_menu_shell,
		    GtkMenuShell  *gtk_menu_shell)
{
	GList *children;
	GList *l;

	children = gtk_container_get_children (GTK_CONTAINER (gtk_menu_shell));

	for (l = children; l != NULL; l = l->next)
	{
		GtkMenuItem *menu_item = l->data;

		if (GTK_IS_MENU_ITEM (menu_item))
		{
			connect_menu_item (tepl_menu_shell, menu_item);
		}
	}

	g_list_free (children);

	g_signal_connect_object (gtk_menu_shell,
				 "insert",
				 G_CALLBACK (insert_cb),
				 tepl_menu_shell,
				 0);

	g_signal_connect_object (gtk_menu_shell,
				 "remove",
				 G_CALLBACK (remove_cb),
				 tepl_menu_shell,
				 0);
}

static void
disconnect_menu_shell (TeplMenuShell *tepl_menu_shell,
		       GtkMenuShell  *gtk_menu_shell)
{
	GList *children;
	GList *l;

	children = gtk_container_get_children (GTK_CONTAINER (gtk_menu_shell));

	for (l = children; l != NULL; l = l->next)
	{
		GtkMenuItem *menu_item = l->data;

		if (GTK_IS_MENU_ITEM (menu_item))
		{
			disconnect_menu_item (tepl_menu_shell, menu_item);
		}
	}

	g_list_free (children);

	g_signal_handlers_disconnect_by_func (gtk_menu_shell,
					      insert_cb,
					      tepl_menu_shell);

	g_signal_handlers_disconnect_by_func (gtk_menu_shell,
					      remove_cb,
					      tepl_menu_shell);
}

static void
set_menu_shell (TeplMenuShell *tepl_menu_shell,
		GtkMenuShell  *gtk_menu_shell)
{
	g_assert (tepl_menu_shell->priv->gtk_menu_shell == NULL);
	g_return_if_fail (GTK_IS_MENU_SHELL (gtk_menu_shell));

	tepl_menu_shell->priv->gtk_menu_shell = gtk_menu_shell;
	connect_menu_shell (tepl_menu_shell, gtk_menu_shell);
}

static void
tepl_menu_shell_get_property (GObject    *object,
			      guint       prop_id,
			      GValue     *value,
			      GParamSpec *pspec)
{
	TeplMenuShell *tepl_menu_shell = TEPL_MENU_SHELL (object);

	switch (prop_id)
	{
		case PROP_MENU_SHELL:
			g_value_set_object (value, tepl_menu_shell_get_menu_shell (tepl_menu_shell));
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
tepl_menu_shell_set_property (GObject      *object,
			      guint         prop_id,
			      const GValue *value,
			      GParamSpec   *pspec)
{
	TeplMenuShell *tepl_menu_shell = TEPL_MENU_SHELL (object);

	switch (prop_id)
	{
		case PROP_MENU_SHELL:
			set_menu_shell (tepl_menu_shell, g_value_get_object (value));
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
tepl_menu_shell_dispose (GObject *object)
{
	TeplMenuShell *tepl_menu_shell = TEPL_MENU_SHELL (object);

	tepl_menu_shell->priv->gtk_menu_shell = NULL;

	G_OBJECT_CLASS (tepl_menu_shell_parent_class)->dispose (object);
}

static void
tepl_menu_shell_class_init (TeplMenuShellClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->get_property = tepl_menu_shell_get_property;
	object_class->set_property = tepl_menu_shell_set_property;
	object_class->dispose = tepl_menu_shell_dispose;

	/**
	 * TeplMenuShell:menu-shell:
	 *
	 * The #GtkMenuShell.
	 *
	 * Since: 2.0
	 */
	properties[PROP_MENU_SHELL] =
		g_param_spec_object ("menu-shell",
				     "GtkMenuShell",
				     "",
				     GTK_TYPE_MENU_SHELL,
				     G_PARAM_READWRITE |
				     G_PARAM_CONSTRUCT_ONLY |
				     G_PARAM_STATIC_STRINGS);

	g_object_class_install_properties (object_class, N_PROPERTIES, properties);

	/**
	 * TeplMenuShell::menu-item-selected:
	 * @tepl_menu_shell: the #TeplMenuShell emitting the signal.
	 * @menu_item: the #GtkMenuItem that has been selected.
	 *
	 * The ::menu-item-selected signal is emitted when the
	 * #GtkMenuItem::select signal is emitted on a #GtkMenuItem belonging
	 * (directly or indirectly through submenus) to @tepl_menu_shell.
	 *
	 * Since: 2.0
	 */
	signals[SIGNAL_MENU_ITEM_SELECTED] =
		g_signal_new ("menu-item-selected",
			      G_TYPE_FROM_CLASS (klass),
			      G_SIGNAL_RUN_FIRST,
			      G_STRUCT_OFFSET (TeplMenuShellClass, menu_item_selected),
			      NULL, NULL, NULL,
			      G_TYPE_NONE,
			      1, GTK_TYPE_MENU_ITEM);

	/**
	 * TeplMenuShell::menu-item-deselected:
	 * @tepl_menu_shell: the #TeplMenuShell emitting the signal.
	 * @menu_item: the #GtkMenuItem that has been deselected.
	 *
	 * The ::menu-item-deselected signal is emitted when the
	 * #GtkMenuItem::deselect signal is emitted on a #GtkMenuItem belonging
	 * (directly or indirectly through submenus) to @tepl_menu_shell.
	 *
	 * Since: 2.0
	 */
	signals[SIGNAL_MENU_ITEM_DESELECTED] =
		g_signal_new ("menu-item-deselected",
			      G_TYPE_FROM_CLASS (klass),
			      G_SIGNAL_RUN_FIRST,
			      G_STRUCT_OFFSET (TeplMenuShellClass, menu_item_deselected),
			      NULL, NULL, NULL,
			      G_TYPE_NONE,
			      1, GTK_TYPE_MENU_ITEM);
}

static void
tepl_menu_shell_init (TeplMenuShell *tepl_menu_shell)
{
	tepl_menu_shell->priv = tepl_menu_shell_get_instance_private (tepl_menu_shell);
}

/**
 * tepl_menu_shell_get_from_gtk_menu_shell:
 * @gtk_menu_shell: a #GtkMenuShell.
 *
 * Returns the #TeplMenuShell of @gtk_menu_shell. The returned object is
 * guaranteed to be the same for the lifetime of @gtk_menu_shell.
 *
 * Returns: (transfer none): the #TeplMenuShell of @gtk_menu_shell.
 * Since: 2.0
 */
TeplMenuShell *
tepl_menu_shell_get_from_gtk_menu_shell (GtkMenuShell *gtk_menu_shell)
{
	TeplMenuShell *tepl_menu_shell;

	g_return_val_if_fail (GTK_IS_MENU_SHELL (gtk_menu_shell), NULL);

	tepl_menu_shell = g_object_get_data (G_OBJECT (gtk_menu_shell), TEPL_MENU_SHELL_KEY);

	if (tepl_menu_shell == NULL)
	{
		tepl_menu_shell = g_object_new (TEPL_TYPE_MENU_SHELL,
						"menu-shell", gtk_menu_shell,
						NULL);

		g_object_set_data_full (G_OBJECT (gtk_menu_shell),
					TEPL_MENU_SHELL_KEY,
					tepl_menu_shell,
					g_object_unref);
	}

	g_return_val_if_fail (TEPL_IS_MENU_SHELL (tepl_menu_shell), NULL);
	return tepl_menu_shell;
}

/**
 * tepl_menu_shell_get_menu_shell:
 * @tepl_menu_shell: a #TeplMenuShell.
 *
 * Returns: (transfer none): the #GtkMenuShell of @tepl_menu_shell.
 * Since: 2.0
 */
GtkMenuShell *
tepl_menu_shell_get_menu_shell (TeplMenuShell *tepl_menu_shell)
{
	g_return_val_if_fail (TEPL_IS_MENU_SHELL (tepl_menu_shell), NULL);

	return tepl_menu_shell->priv->gtk_menu_shell;
}

/* ex:set ts=8 noet: */
