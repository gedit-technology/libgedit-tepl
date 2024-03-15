/* SPDX-FileCopyrightText: 2014 - Paolo Borelli
 * SPDX-FileCopyrightText: 2024 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "tepl-panel-switcher-notebook.h"
#include "tepl-panel.h"

/**
 * SECTION:panel-switcher-notebook
 * @Title: TeplPanelSwitcherNotebook
 * @Short_description: A controller for #TeplPanelContainer
 *
 * Similar to #GtkStackSwitcher, but for a #TeplPanelContainer.
 *
 * This switcher widget contains a #GtkNotebook, but only the tab labels are
 * used. It uses empty (zero-sized) pages, because the real content widgets are
 * part of the #TeplPanelContainer.
 *
 * The #TeplPanelContainer is not packed inside the #TeplPanelSwitcherNotebook,
 * the latter just has a reference to the former.
 *
 * This class is useful if you want to expose only #TeplPanel in an API, but you
 * want the same style as #GtkNotebook tab labels.
 */

struct _TeplPanelSwitcherNotebookPrivate
{
	TeplPanelContainer *panel;
	GtkNotebook *notebook;

	gulong signal_handler_id_panel_changed;
	gulong signal_handler_id_notebook_switch_page;
};

#define TEPL_PANEL_ITEM_KEY "tepl-panel-item-key"

G_DEFINE_TYPE_WITH_PRIVATE (TeplPanelSwitcherNotebook, tepl_panel_switcher_notebook, GTK_TYPE_BIN)

/* A blank, zero-sized widget. */
static GtkWidget *
create_empty_widget (void)
{
	GtkWidget *dummy;

	dummy = gtk_grid_new ();

	/* Need to make it visible, as per gtk_notebook_set_current_page() docs:
	 * "GtkNotebook refuses to switch to a page unless the child widget is
	 * visible."
	 */
	gtk_widget_show (dummy);

	return dummy;
}

static void
empty_notebook (GtkNotebook *notebook)
{
	while (gtk_notebook_get_n_pages (notebook) > 0)
	{
		/* Remove the last page. Maybe more efficient than removing the
		 * first page, in order to not affect the indexes of the pages.
		 * But the GtkNotebook docs don't explain which is faster, so
		 * it's just a guess, and it has not been measured.
		 */
		gtk_notebook_remove_page (notebook, -1);
	}
}

static void
set_panel_item_to_notebook_page_widget (GtkWidget     *notebook_page_widget,
					TeplPanelItem *panel_item)
{
	g_object_set_data_full (G_OBJECT (notebook_page_widget),
				TEPL_PANEL_ITEM_KEY,
				g_object_ref (panel_item),
				g_object_unref);
}

static TeplPanelItem *
get_panel_item_from_notebook_page_widget (GtkWidget *notebook_page_widget)
{
	return TEPL_PANEL_ITEM (g_object_get_data (G_OBJECT (notebook_page_widget), TEPL_PANEL_ITEM_KEY));
}

static void
add_item_to_notebook (TeplPanelSwitcherNotebook *switcher,
		      TeplPanelItem             *item,
		      gboolean                   jump_to)
{
	GtkWidget *dummy;
	const gchar *title;

	dummy = create_empty_widget ();
	set_panel_item_to_notebook_page_widget (dummy, item);

	title = tepl_panel_item_get_title (item);

	gtk_notebook_append_page (switcher->priv->notebook, dummy, NULL);
	gtk_notebook_set_tab_label_text (switcher->priv->notebook, dummy, title);

	if (jump_to)
	{
		gint page_num;

		page_num = gtk_notebook_page_num (switcher->priv->notebook, dummy);
		gtk_notebook_set_current_page (switcher->priv->notebook, page_num);
	}
}

static void
fill_notebook (TeplPanelSwitcherNotebook *switcher)
{
	GList *items;
	GList *l;
	TeplPanelItem *active_item;

	items = tepl_panel_container_get_items (switcher->priv->panel);
	items = g_list_sort (items, (GCompareFunc) tepl_panel_item_compare_by_title);

	active_item = tepl_panel_container_get_active_item (switcher->priv->panel);

	for (l = items; l != NULL; l = l->next)
	{
		TeplPanelItem *cur_item = TEPL_PANEL_ITEM (l->data);

		add_item_to_notebook (switcher,
				      cur_item,
				      cur_item == active_item);
	}

	g_list_free_full (items, g_object_unref);
}

static void
repopulate_notebook (TeplPanelSwitcherNotebook *switcher)
{
	g_signal_handler_block (switcher->priv->notebook,
				switcher->priv->signal_handler_id_notebook_switch_page);

	empty_notebook (switcher->priv->notebook);
	fill_notebook (switcher);

	g_signal_handler_unblock (switcher->priv->notebook,
				  switcher->priv->signal_handler_id_notebook_switch_page);
}

static void
panel_changed_cb (TeplPanelContainer        *panel,
		  TeplPanelSwitcherNotebook *switcher)
{
	repopulate_notebook (switcher);
}

static void
notebook_switch_page_cb (GtkNotebook               *notebook,
			 GtkWidget                 *page,
			 guint                      page_num,
			 TeplPanelSwitcherNotebook *switcher)
{
	TeplPanelItem *panel_item;

	panel_item = get_panel_item_from_notebook_page_widget (page);

	g_signal_handler_block (switcher->priv->panel,
				switcher->priv->signal_handler_id_panel_changed);

	tepl_panel_set_active (TEPL_PANEL (switcher->priv->panel), panel_item);

	g_signal_handler_unblock (switcher->priv->panel,
				  switcher->priv->signal_handler_id_panel_changed);
}

static void
tepl_panel_switcher_notebook_dispose (GObject *object)
{
	TeplPanelSwitcherNotebook *switcher = TEPL_PANEL_SWITCHER_NOTEBOOK (object);

	g_clear_signal_handler (&switcher->priv->signal_handler_id_panel_changed,
				switcher->priv->panel);
	g_clear_signal_handler (&switcher->priv->signal_handler_id_notebook_switch_page,
				switcher->priv->notebook);

	g_clear_object (&switcher->priv->panel);
	g_clear_object (&switcher->priv->notebook);

	G_OBJECT_CLASS (tepl_panel_switcher_notebook_parent_class)->dispose (object);
}

static void
tepl_panel_switcher_notebook_class_init (TeplPanelSwitcherNotebookClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->dispose = tepl_panel_switcher_notebook_dispose;
}

static void
tepl_panel_switcher_notebook_init (TeplPanelSwitcherNotebook *switcher)
{
	switcher->priv = tepl_panel_switcher_notebook_get_instance_private (switcher);
}

/**
 * tepl_panel_switcher_notebook_new:
 * @panel: a #TeplPanelContainer.
 * @notebook: a #GtkNotebook.
 *
 * Creates a new #TeplPanelSwitcherNotebook.
 *
 * @notebook needs to be empty (no pages), and should be pre-configured. Then
 * the #TeplPanelSwitcherNotebook will handle @notebook, so you should not
 * modify @notebook afterwards. The goal is to configure the #GtkNotebook in the
 * way it is desired in your application.
 *
 * Returns: (transfer floating): a new #TeplPanelSwitcherNotebook widget.
 * Since: 6.10
 */
TeplPanelSwitcherNotebook *
tepl_panel_switcher_notebook_new (TeplPanelContainer *panel,
				  GtkNotebook        *notebook)
{
	TeplPanelSwitcherNotebook *switcher;

	g_return_val_if_fail (TEPL_IS_PANEL_CONTAINER (panel), NULL);
	g_return_val_if_fail (GTK_IS_NOTEBOOK (notebook), NULL);
	g_return_val_if_fail (gtk_notebook_get_n_pages (notebook) == 0, NULL);

	switcher = g_object_new (TEPL_TYPE_PANEL_SWITCHER_NOTEBOOK, NULL);

	switcher->priv->panel = g_object_ref_sink (panel);
	switcher->priv->notebook = g_object_ref_sink (notebook);

	gtk_container_add (GTK_CONTAINER (switcher), GTK_WIDGET (notebook));
	gtk_widget_show_all (GTK_WIDGET (switcher));

	switcher->priv->signal_handler_id_panel_changed =
		g_signal_connect (switcher->priv->panel,
				  "changed",
				  G_CALLBACK (panel_changed_cb),
				  switcher);

	switcher->priv->signal_handler_id_notebook_switch_page =
		g_signal_connect_after (switcher->priv->notebook,
					"switch-page",
					G_CALLBACK (notebook_switch_page_cb),
					switcher);

	repopulate_notebook (switcher);

	return switcher;
}
