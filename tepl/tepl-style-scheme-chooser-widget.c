/* SPDX-FileCopyrightText: 2020-2024 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "tepl-style-scheme-chooser-widget.h"
#include "tepl-style-scheme-row.h"
#include "tepl-utils.h"

/**
 * SECTION:style-scheme-chooser-widget
 * @Title: TeplStyleSchemeChooserWidget
 * @Short_description: A #GtkSourceStyleScheme chooser
 *
 * #TeplStyleSchemeChooserWidget permits to choose a #GtkSourceStyleScheme.
 *
 * It shows a list with the name and description of each #GtkSourceStyleScheme,
 * taken from the default #GtkSourceStyleSchemeManager.
 *
 * #TeplStyleSchemeChooserWidget contains a #GtkScrolledWindow internally. When
 * the #GtkWidget::map signal is emitted, it scrolls to the selected row (useful
 * when the list is long).
 */

struct _TeplStyleSchemeChooserWidgetPrivate
{
	GtkListBox *list_box;

	guint theme_variants : 1;
};

G_DEFINE_TYPE_WITH_PRIVATE (TeplStyleSchemeChooserWidget, tepl_style_scheme_chooser_widget, GTK_TYPE_BIN)

static gboolean
style_scheme_equal (GtkSourceStyleScheme *style_scheme1,
		    GtkSourceStyleScheme *style_scheme2)
{
	const gchar *id1;
	const gchar *id2;

	if (style_scheme1 == style_scheme2)
	{
		return TRUE;
	}

	if (style_scheme1 == NULL || style_scheme2 == NULL)
	{
		return FALSE;
	}

	id1 = gtk_source_style_scheme_get_id (style_scheme1);
	id2 = gtk_source_style_scheme_get_id (style_scheme2);
	return g_strcmp0 (id1, id2) == 0;
}

static GtkSourceStyleScheme *
get_style_scheme (TeplStyleSchemeChooserWidget *chooser)
{
	GtkListBoxRow *selected_row;

	selected_row = gtk_list_box_get_selected_row (chooser->priv->list_box);
	if (selected_row != NULL)
	{
		return _tepl_style_scheme_row_get_style_scheme (TEPL_STYLE_SCHEME_ROW (selected_row));
	}

	return NULL;
}

static void
set_style_scheme (TeplStyleSchemeChooserWidget *chooser,
		  GtkSourceStyleScheme         *style_scheme)
{
	GList *all_rows;
	GList *l;

	if (style_scheme == NULL)
	{
		return;
	}

	all_rows = gtk_container_get_children (GTK_CONTAINER (chooser->priv->list_box));

	for (l = all_rows; l != NULL; l = l->next)
	{
		TeplStyleSchemeRow *cur_row = TEPL_STYLE_SCHEME_ROW (l->data);
		GtkSourceStyleScheme *cur_style_scheme;

		cur_style_scheme = _tepl_style_scheme_row_get_style_scheme (cur_row);
		if (style_scheme_equal (cur_style_scheme, style_scheme))
		{
			gtk_list_box_select_row (chooser->priv->list_box,
						 GTK_LIST_BOX_ROW (cur_row));
			tepl_utils_list_box_scroll_to_row (chooser->priv->list_box,
							   GTK_LIST_BOX_ROW (cur_row));
			break;
		}
	}

	g_list_free (all_rows);
}

static gchar *
get_style_scheme_id (TeplStyleSchemeChooserWidget *chooser)
{
	GtkSourceStyleScheme *style_scheme;
	const gchar *id;

	g_return_val_if_fail (TEPL_IS_STYLE_SCHEME_CHOOSER_WIDGET (chooser), g_strdup (""));

	style_scheme = get_style_scheme (chooser);

	if (style_scheme == NULL)
	{
		return g_strdup ("");
	}

	id = gtk_source_style_scheme_get_id (style_scheme);

	return id != NULL ? g_strdup (id) : g_strdup ("");
}

static void
set_style_scheme_id (TeplStyleSchemeChooserWidget *chooser,
		     const gchar                  *style_scheme_id)
{
	GtkSourceStyleSchemeManager *manager;
	GtkSourceStyleScheme *style_scheme;

	g_return_if_fail (TEPL_IS_STYLE_SCHEME_CHOOSER_WIDGET (chooser));
	g_return_if_fail (style_scheme_id != NULL);

	manager = gtk_source_style_scheme_manager_get_default ();
	style_scheme = gtk_source_style_scheme_manager_get_scheme (manager, style_scheme_id);

	if (style_scheme != NULL)
	{
		set_style_scheme (chooser, style_scheme);
	}
}

static void
tepl_style_scheme_chooser_widget_dispose (GObject *object)
{
	TeplStyleSchemeChooserWidget *chooser = TEPL_STYLE_SCHEME_CHOOSER_WIDGET (object);

	chooser->priv->list_box = NULL;

	G_OBJECT_CLASS (tepl_style_scheme_chooser_widget_parent_class)->dispose (object);
}

static void
tepl_style_scheme_chooser_widget_map (GtkWidget *widget)
{
	TeplStyleSchemeChooserWidget *chooser = TEPL_STYLE_SCHEME_CHOOSER_WIDGET (widget);

	if (GTK_WIDGET_CLASS (tepl_style_scheme_chooser_widget_parent_class)->map != NULL)
	{
		GTK_WIDGET_CLASS (tepl_style_scheme_chooser_widget_parent_class)->map (widget);
	}

	tepl_utils_list_box_scroll_to_selected_row (chooser->priv->list_box);
}

static void
tepl_style_scheme_chooser_widget_class_init (TeplStyleSchemeChooserWidgetClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

	object_class->dispose = tepl_style_scheme_chooser_widget_dispose;

	widget_class->map = tepl_style_scheme_chooser_widget_map;
}

static void
append_style_scheme_to_list_box (TeplStyleSchemeChooserWidget *chooser,
				 GtkSourceStyleScheme         *style_scheme)
{
	TeplStyleSchemeRow *row;

	row = _tepl_style_scheme_row_new (style_scheme, FALSE);
	gtk_widget_show (GTK_WIDGET (row));

	gtk_list_box_insert (chooser->priv->list_box, GTK_WIDGET (row), -1);
}

static void
populate_list_box (TeplStyleSchemeChooserWidget *chooser)
{
	GtkSourceStyleSchemeManager *manager;
	GList *schemes;
	GList *l;

	manager = gtk_source_style_scheme_manager_get_default ();
	schemes = gtk_source_style_scheme_manager_get_schemes (manager);

	for (l = schemes; l != NULL; l = l->next)
	{
		GtkSourceStyleScheme *style_scheme = GTK_SOURCE_STYLE_SCHEME (l->data);
		append_style_scheme_to_list_box (chooser, style_scheme);
	}

	g_list_free (schemes);
}

static void
list_box_selected_rows_changed_cb (GtkListBox                   *list_box,
				   TeplStyleSchemeChooserWidget *chooser)
{
	/* TODO */
}

static void
style_scheme_manager_changed_cb (GtkSourceStyleSchemeManager  *manager,
				 TeplStyleSchemeChooserWidget *chooser)
{
	gchar *style_scheme_id;

	g_signal_handlers_block_by_func (chooser->priv->list_box,
					 list_box_selected_rows_changed_cb,
					 chooser);

	style_scheme_id = get_style_scheme_id (chooser);

	tepl_utils_list_box_clear (chooser->priv->list_box);
	populate_list_box (chooser);

	/* Note that the style_scheme_id may no longer exist, in which case no
	 * rows will be selected.
	 */
	set_style_scheme_id (chooser, style_scheme_id);

	tepl_utils_list_box_scroll_to_selected_row (chooser->priv->list_box);

	g_signal_handlers_unblock_by_func (chooser->priv->list_box,
					   list_box_selected_rows_changed_cb,
					   chooser);

	g_free (style_scheme_id);
}

static void
listen_to_scheme_manager_changes (TeplStyleSchemeChooserWidget *chooser)
{
	GtkSourceStyleSchemeManager *manager;

	manager = gtk_source_style_scheme_manager_get_default ();

	g_signal_connect_object (manager,
				 "changed",
				 G_CALLBACK (style_scheme_manager_changed_cb),
				 chooser,
				 G_CONNECT_DEFAULT);
}

static void
tepl_style_scheme_chooser_widget_init (TeplStyleSchemeChooserWidget *chooser)
{
	GtkWidget *scrolled_window;

	chooser->priv = tepl_style_scheme_chooser_widget_get_instance_private (chooser);

	chooser->priv->list_box = GTK_LIST_BOX (gtk_list_box_new ());
	gtk_list_box_set_selection_mode (chooser->priv->list_box, GTK_SELECTION_BROWSE);

	populate_list_box (chooser);
	listen_to_scheme_manager_changes (chooser);

	scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_set_hexpand (scrolled_window, TRUE);
	gtk_widget_set_vexpand (scrolled_window, TRUE);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolled_window), GTK_SHADOW_IN);

	/* Improve the minimum height, to avoid the vertical scrollbar by
	 * default (with the default list of style schemes provided by
	 * GtkSourceView).
	 */
	gtk_widget_set_size_request (scrolled_window, -1, 200);

	/* Overlay scrolling gets in the way when trying to select the last row. */
	gtk_scrolled_window_set_overlay_scrolling (GTK_SCROLLED_WINDOW (scrolled_window), FALSE);

	gtk_container_add (GTK_CONTAINER (scrolled_window),
			   GTK_WIDGET (chooser->priv->list_box));
	gtk_container_add (GTK_CONTAINER (chooser), scrolled_window);
	gtk_widget_show_all (GTK_WIDGET (chooser));

	tepl_utils_list_box_setup_scrolling (chooser->priv->list_box,
					     GTK_SCROLLED_WINDOW (scrolled_window));

	g_signal_connect (chooser->priv->list_box,
			  "selected-rows-changed",
			  G_CALLBACK (list_box_selected_rows_changed_cb),
			  chooser);
}

/**
 * tepl_style_scheme_chooser_widget_new:
 * @theme_variants: whether to enable theme variants support.
 *
 * Returns: (transfer floating): a new #TeplStyleSchemeChooserWidget.
 * Since: 5.0
 */
TeplStyleSchemeChooserWidget *
tepl_style_scheme_chooser_widget_new (gboolean theme_variants)
{
	TeplStyleSchemeChooserWidget *chooser;

	chooser = g_object_new (TEPL_TYPE_STYLE_SCHEME_CHOOSER_WIDGET, NULL);
	chooser->priv->theme_variants = theme_variants != FALSE;

	return chooser;
}
