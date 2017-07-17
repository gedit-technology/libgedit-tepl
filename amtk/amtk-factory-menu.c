/*
 * This file is part of Amtk - Actions, Menus and Toolbars Kit
 *
 * Copyright 2017 - Sébastien Wilmet <swilmet@gnome.org>
 *
 * Amtk is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation; either version 2.1 of the License, or (at your
 * option) any later version.
 *
 * Amtk is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#include "amtk-factory-menu.h"
#include "amtk-action-info.h"
#include "amtk-action-info-central-store.h"
#include "amtk-menu-item.h"

/**
 * SECTION:amtk-factory-menu
 * @Short_description: A factory that creates #GtkMenuItem's
 * @Title: AmtkFactoryMenu
 *
 * #AmtkFactoryMenu permits to create #GtkMenuItem's from #AmtkActionInfo's.
 */

struct _AmtkFactoryMenuPrivate
{
	gint something;
};

G_DEFINE_TYPE_WITH_PRIVATE (AmtkFactoryMenu, amtk_factory_menu, AMTK_TYPE_FACTORY)

static void
amtk_factory_menu_class_init (AmtkFactoryMenuClass *klass)
{
}

static void
amtk_factory_menu_init (AmtkFactoryMenu *factory)
{
	factory->priv = amtk_factory_menu_get_instance_private (factory);
}

/**
 * amtk_factory_menu_new:
 * @application: (nullable): a #GtkApplication, or %NULL.
 *
 * Creates a new #AmtkFactoryMenu object. Associating a #GtkApplication is
 * optional.
 *
 * Returns: a new #AmtkFactoryMenu.
 * Since: 3.0
 */
AmtkFactoryMenu *
amtk_factory_menu_new (GtkApplication *application)
{
	g_return_val_if_fail (application == NULL || GTK_IS_APPLICATION (application), NULL);

	return g_object_new (AMTK_TYPE_FACTORY_MENU,
			     "application", application,
			     NULL);
}

/**
 * amtk_factory_menu_new_with_default_application:
 *
 * Calls amtk_factory_menu_new() with g_application_get_default() (it must be a
 * #GtkApplication).
 *
 * Returns: a new #AmtkFactoryMenu with the default #GtkApplication.
 * Since: 3.0
 */
AmtkFactoryMenu *
amtk_factory_menu_new_with_default_application (void)
{
	return amtk_factory_menu_new (GTK_APPLICATION (g_application_get_default ()));
}

/**
 * amtk_factory_menu_create_menu_item:
 * @factory: an #AmtkFactoryMenu.
 * @action_name: an action name.
 *
 * Creates a new #GtkMenuItem for @action_name. The #AmtkActionInfoCentralStore
 * must contain an #AmtkActionInfo for @action_name.
 *
 * gtk_actionable_set_action_name() is called on the menu item with
 * @action_name. The label is set with the #GtkMenuItem:use-underline property
 * enabled. The first accelerator is set to the #GtkAccelLabel of the menu item.
 * The icon is set. And the tooltip is set with
 * amtk_menu_item_set_long_description().
 *
 * If the #AmtkFactory:application is non-%NULL, this function also calls
 * gtk_application_set_accels_for_action() with the accelerators returned by
 * amtk_action_info_get_accels() (this will erase previously set accelerators
 * for that action, if any).
 *
 * Returns: (transfer floating): a new #GtkMenuItem for @action_name.
 * Since: 3.0
 */
GtkWidget *
amtk_factory_menu_create_menu_item (AmtkFactoryMenu *factory,
				    const gchar     *action_name)
{
	AmtkActionInfoCentralStore *central_store;
	AmtkActionInfo *action_info;
	GtkMenuItem *menu_item;
	const gchar * const *accels;
	const gchar *icon_name;
	const gchar *tooltip;
	GtkApplication *app;

	g_return_val_if_fail (AMTK_IS_FACTORY_MENU (factory), NULL);
	g_return_val_if_fail (action_name != NULL, NULL);

	central_store = amtk_action_info_central_store_get_instance ();
	action_info = amtk_action_info_central_store_lookup (central_store, action_name);

	if (action_info == NULL)
	{
		g_warning ("%s(): action name '%s' not found.",
			   G_STRFUNC,
			   action_name);

		return NULL;
	}

	menu_item = GTK_MENU_ITEM (gtk_menu_item_new ());

	gtk_actionable_set_action_name (GTK_ACTIONABLE (menu_item), action_name);

	gtk_menu_item_set_use_underline (menu_item, TRUE);
	gtk_menu_item_set_label (menu_item, amtk_action_info_get_label (action_info));

	/* Set accel before setting icon, because
	 * amtk_menu_item_set_icon_name() adds a GtkBox.
	 */
	accels = amtk_action_info_get_accels (action_info);
	if (accels != NULL && accels[0] != NULL)
	{
		guint accel_key;
		GdkModifierType accel_mods;

		gtk_accelerator_parse (accels[0], &accel_key, &accel_mods);

		if (accel_key != 0 || accel_mods != 0)
		{
			GtkWidget *child;

			child = gtk_bin_get_child (GTK_BIN (menu_item));

			gtk_accel_label_set_accel (GTK_ACCEL_LABEL (child),
						   accel_key,
						   accel_mods);
		}
	}

	icon_name = amtk_action_info_get_icon_name (action_info);
	if (icon_name != NULL)
	{
		amtk_menu_item_set_icon_name (menu_item, icon_name);
	}

	tooltip = amtk_action_info_get_tooltip (action_info);
	if (tooltip != NULL)
	{
		amtk_menu_item_set_long_description (menu_item, tooltip);
	}

	app = amtk_factory_get_application (AMTK_FACTORY (factory));
	if (app != NULL)
	{
		gtk_application_set_accels_for_action (app, action_name, accels);
	}

	amtk_action_info_mark_as_used (action_info);

	return GTK_WIDGET (menu_item);
}
