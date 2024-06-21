/* SPDX-FileCopyrightText: 2014 - Paolo Borelli
 * SPDX-FileCopyrightText: 2024 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "tepl-panel-notebook.h"
#include "tepl-panel.h"

/**
 * SECTION:panel-notebook
 * @Title: TeplPanelNotebook
 * @Short_description: Side or bottom panel container based on #GtkNotebook
 *
 * Side or bottom panel container based on #GtkNotebook.
 *
 * This class is useful if you want to expose only #TeplPanel in an API.
 */

struct _TeplPanelNotebookPrivate
{
	TeplPanelSimple *panel_simple;
	GtkNotebook *notebook;
};

#define TEPL_PANEL_ITEM_KEY "tepl-panel-item-key"

G_DEFINE_TYPE_WITH_PRIVATE (TeplPanelNotebook, tepl_panel_notebook, G_TYPE_OBJECT)

/* Prototypes */
static void notebook_switch_page_cb (GtkNotebook       *notebook,
				     GtkWidget         *page,
				     guint              page_num,
				     TeplPanelNotebook *self);

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
notebook_page_widget_set_panel_item (GtkWidget     *notebook_page_widget,
				     TeplPanelItem *panel_item)
{
	g_object_set_data_full (G_OBJECT (notebook_page_widget),
				TEPL_PANEL_ITEM_KEY,
				g_object_ref (panel_item),
				g_object_unref);
}

static TeplPanelItem *
notebook_page_widget_get_panel_item (GtkWidget *notebook_page_widget)
{
	return TEPL_PANEL_ITEM (g_object_get_data (G_OBJECT (notebook_page_widget), TEPL_PANEL_ITEM_KEY));
}

static void
add_item_to_notebook (TeplPanelNotebook *self,
		      TeplPanelItem     *item,
		      gboolean           jump_to)
{
	GtkWidget *widget;
	const gchar *title;

	widget = tepl_panel_item_get_widget (item);
	if (widget == NULL)
	{
		return;
	}

	notebook_page_widget_set_panel_item (widget, item);

	title = tepl_panel_item_get_title (item);

	gtk_notebook_append_page (self->priv->notebook, widget, NULL);
	gtk_notebook_set_tab_label_text (self->priv->notebook, widget, title);

	if (jump_to)
	{
		gint page_num;

		page_num = gtk_notebook_page_num (self->priv->notebook, widget);
		gtk_notebook_set_current_page (self->priv->notebook, page_num);
	}
}

static void
fill_notebook (TeplPanelNotebook *self)
{
	GList *items;
	GList *l;
	TeplPanelItem *active_item;

	items = tepl_panel_simple_get_items (self->priv->panel_simple);
	items = g_list_sort (items, (GCompareFunc) tepl_panel_item_compare);

	active_item = tepl_panel_simple_get_active_item (self->priv->panel_simple);

	for (l = items; l != NULL; l = l->next)
	{
		TeplPanelItem *cur_item = TEPL_PANEL_ITEM (l->data);

		add_item_to_notebook (self,
				      cur_item,
				      cur_item == active_item);
	}

	g_list_free_full (items, g_object_unref);
}

static void
repopulate_notebook (TeplPanelNotebook *self)
{
	g_signal_handlers_block_by_func (self->priv->notebook,
					 notebook_switch_page_cb,
					 self);

	empty_notebook (self->priv->notebook);
	fill_notebook (self);

	g_signal_handlers_unblock_by_func (self->priv->notebook,
					   notebook_switch_page_cb,
					   self);
}

static void
panel_simple_changed_cb (TeplPanelSimple   *panel_simple,
			 TeplPanelNotebook *self)
{
	repopulate_notebook (self);
}

static void
notebook_switch_page_cb (GtkNotebook       *notebook,
			 GtkWidget         *page,
			 guint              page_num,
			 TeplPanelNotebook *self)
{
	TeplPanelItem *panel_item;

	panel_item = notebook_page_widget_get_panel_item (page);

	g_signal_handlers_block_by_func (self->priv->panel_simple,
					 panel_simple_changed_cb,
					 self);

	tepl_panel_set_active (TEPL_PANEL (self->priv->panel_simple), panel_item);

	g_signal_handlers_unblock_by_func (self->priv->panel_simple,
					   panel_simple_changed_cb,
					   self);
}

static void
tepl_panel_notebook_dispose (GObject *object)
{
	TeplPanelNotebook *self = TEPL_PANEL_NOTEBOOK (object);

	g_clear_object (&self->priv->panel_simple);
	g_clear_object (&self->priv->notebook);

	G_OBJECT_CLASS (tepl_panel_notebook_parent_class)->dispose (object);
}

static void
tepl_panel_notebook_class_init (TeplPanelNotebookClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->dispose = tepl_panel_notebook_dispose;
}

static void
tepl_panel_notebook_init (TeplPanelNotebook *self)
{
	self->priv = tepl_panel_notebook_get_instance_private (self);
}

/**
 * tepl_panel_notebook_new:
 * @panel_simple: a #TeplPanelSimple.
 * @notebook: a #GtkNotebook.
 *
 * Creates a new #TeplPanelNotebook.
 *
 * @notebook needs to be empty (no pages), and should be pre-configured. Then
 * the #TeplPanelNotebook will handle @notebook, so you should not modify it
 * afterwards.
 *
 * Returns: (transfer full): a new #TeplPanelNotebook object.
 * Since: 6.11
 */
TeplPanelNotebook *
tepl_panel_notebook_new (TeplPanelSimple *panel_simple,
			 GtkNotebook     *notebook)
{
	TeplPanelNotebook *self;

	g_return_val_if_fail (TEPL_IS_PANEL_SIMPLE (panel_simple), NULL);
	g_return_val_if_fail (GTK_IS_NOTEBOOK (notebook), NULL);
	g_return_val_if_fail (gtk_notebook_get_n_pages (notebook) == 0, NULL);

	self = g_object_new (TEPL_TYPE_PANEL_NOTEBOOK, NULL);

	self->priv->panel_simple = g_object_ref (panel_simple);
	self->priv->notebook = g_object_ref_sink (notebook);

	g_signal_connect_object (self->priv->panel_simple,
				 "changed",
				 G_CALLBACK (panel_simple_changed_cb),
				 self,
				 G_CONNECT_DEFAULT);

	g_signal_connect_object (self->priv->notebook,
				 "switch-page",
				 G_CALLBACK (notebook_switch_page_cb),
				 self,
				 G_CONNECT_AFTER);

	repopulate_notebook (self);

	return self;
}
