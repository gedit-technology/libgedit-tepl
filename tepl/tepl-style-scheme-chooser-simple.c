/* SPDX-FileCopyrightText: 2024 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "tepl-style-scheme-chooser-simple.h"
#include "tepl-style-scheme-row.h"
#include "tepl-utils.h"

/**
 * SECTION:style-scheme-chooser-simple
 * @Title: TeplStyleSchemeChooserSimple
 * @Short_description: A simple #GtkSourceStyleScheme chooser widget
 *
 * #TeplStyleSchemeChooserSimple is a widget that permits to choose a
 * #GtkSourceStyleScheme.
 *
 * It shows a list with the name and description of each #GtkSourceStyleScheme,
 * taken from the default #GtkSourceStyleSchemeManager.
 *
 * It is either for a light GTK theme, or a dark GTK theme, but not both. If you
 * need both, consider using #TeplStyleSchemeChooserFull or create two instances
 * of #TeplStyleSchemeChooserSimple.
 *
 * #TeplStyleSchemeChooserSimple contains a #GtkScrolledWindow internally. When
 * the #GtkWidget::map signal is emitted, it scrolls to the selected row (useful
 * when the list is long).
 */

struct _TeplStyleSchemeChooserSimplePrivate
{
	/* Contains TeplStyleSchemeRow's. */
	GtkListBox *list_box;

	/* For the property. */
	gchar *style_scheme_id;

	guint for_dark_theme_variant : 1;
};

enum
{
	PROP_0,
	PROP_STYLE_SCHEME_ID,
	N_PROPERTIES
};

static GParamSpec *properties[N_PROPERTIES];

G_DEFINE_TYPE_WITH_PRIVATE (TeplStyleSchemeChooserSimple, tepl_style_scheme_chooser_simple, GTK_TYPE_BIN)

static GtkListBox *
create_list_box (void)
{
	GtkListBox *list_box;

	list_box = GTK_LIST_BOX (gtk_list_box_new ());
	gtk_list_box_set_selection_mode (list_box, GTK_SELECTION_BROWSE);

	return list_box;
}

static GtkScrolledWindow *
create_scrolled_window (void)
{
	GtkScrolledWindow *scrolled_window;

	scrolled_window = GTK_SCROLLED_WINDOW (gtk_scrolled_window_new (NULL, NULL));
	gtk_widget_set_hexpand (GTK_WIDGET (scrolled_window), TRUE);
	gtk_widget_set_vexpand (GTK_WIDGET (scrolled_window), TRUE);
	gtk_scrolled_window_set_shadow_type (scrolled_window, GTK_SHADOW_IN);

	/* Improve the minimum height, to avoid the vertical scrollbar by
	 * default (with the default list of style schemes provided by
	 * libgedit-gtksourceview).
	 */
	gtk_widget_set_size_request (GTK_WIDGET (scrolled_window), -1, 200);

	/* Overlay scrolling gets in the way when trying to select the last row. */
	gtk_scrolled_window_set_overlay_scrolling (scrolled_window, FALSE);

	return scrolled_window;
}

static void
populate_list_box (TeplStyleSchemeChooserSimple *chooser)
{
	GtkSourceStyleSchemeManager *manager;
	GList *schemes;
	GList *l;

	manager = gtk_source_style_scheme_manager_get_default ();
	schemes = gtk_source_style_scheme_manager_get_schemes (manager);

	for (l = schemes; l != NULL; l = l->next)
	{
		GtkSourceStyleScheme *style_scheme = GTK_SOURCE_STYLE_SCHEME (l->data);
		TeplStyleSchemeRow *row;

		row = _tepl_style_scheme_row_new (style_scheme, chooser->priv->for_dark_theme_variant);
		gtk_list_box_insert (chooser->priv->list_box, GTK_WIDGET (row), -1);
	}

	g_list_free (schemes);
}

/* Sync selected-row -> style-scheme-id property. */
static void
update_style_scheme_id_property (TeplStyleSchemeChooserSimple *chooser)
{
	GtkListBoxRow *selected_row;
	const gchar *id = NULL;

	selected_row = gtk_list_box_get_selected_row (chooser->priv->list_box);
	if (selected_row != NULL)
	{
		GtkSourceStyleScheme *style_scheme;

		style_scheme = _tepl_style_scheme_row_get_style_scheme (TEPL_STYLE_SCHEME_ROW (selected_row));
		id = gtk_source_style_scheme_get_id (style_scheme);
	}

	/* Do not call tepl_style_scheme_chooser_simple_set_style_scheme_id()
	 * because we don't want update_selected_row() to be called here.
	 */
	if (g_set_str (&chooser->priv->style_scheme_id, id))
	{
		g_object_notify_by_pspec (G_OBJECT (chooser), properties[PROP_STYLE_SCHEME_ID]);
	}
}

/* Sync style-scheme-id property -> selected-row. */
static void
update_selected_row (TeplStyleSchemeChooserSimple *chooser)
{
	GList *all_rows;
	GList *l;

	gtk_list_box_unselect_all (chooser->priv->list_box);

	if (chooser->priv->style_scheme_id == NULL)
	{
		return;
	}

	all_rows = gtk_container_get_children (GTK_CONTAINER (chooser->priv->list_box));

	for (l = all_rows; l != NULL; l = l->next)
	{
		TeplStyleSchemeRow *row = TEPL_STYLE_SCHEME_ROW (l->data);
		GtkSourceStyleScheme *style_scheme;
		const gchar *id;

		style_scheme = _tepl_style_scheme_row_get_style_scheme (row);
		id = gtk_source_style_scheme_get_id (style_scheme);

		if (g_strcmp0 (chooser->priv->style_scheme_id, id) == 0 &&
		    gtk_list_box_row_get_selectable (GTK_LIST_BOX_ROW (row)))
		{
			gtk_list_box_select_row (chooser->priv->list_box,
						 GTK_LIST_BOX_ROW (row));
			tepl_utils_list_box_scroll_to_row (chooser->priv->list_box,
							   GTK_LIST_BOX_ROW (row));
			break;
		}
	}

	g_list_free (all_rows);
}

static void
list_box_selected_rows_changed_cb (GtkListBox                   *list_box,
				   TeplStyleSchemeChooserSimple *chooser)
{
	update_style_scheme_id_property (chooser);
}

static void
full_repopulate (TeplStyleSchemeChooserSimple *chooser)
{
	g_signal_handlers_block_by_func (chooser->priv->list_box,
					 list_box_selected_rows_changed_cb,
					 chooser);

	tepl_utils_list_box_clear (chooser->priv->list_box);
	populate_list_box (chooser);
	update_selected_row (chooser);

	g_signal_handlers_unblock_by_func (chooser->priv->list_box,
					   list_box_selected_rows_changed_cb,
					   chooser);
}

static void
style_scheme_manager_changed_cb (GtkSourceStyleSchemeManager  *manager,
				 TeplStyleSchemeChooserSimple *chooser)
{
	full_repopulate (chooser);
}

static void
tepl_style_scheme_chooser_simple_get_property (GObject    *object,
                                               guint       prop_id,
                                               GValue     *value,
                                               GParamSpec *pspec)
{
	TeplStyleSchemeChooserSimple *chooser = TEPL_STYLE_SCHEME_CHOOSER_SIMPLE (object);

	switch (prop_id)
	{
		case PROP_STYLE_SCHEME_ID:
			g_value_set_string (value, tepl_style_scheme_chooser_simple_get_style_scheme_id (chooser));
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
tepl_style_scheme_chooser_simple_set_property (GObject      *object,
                                               guint         prop_id,
                                               const GValue *value,
                                               GParamSpec   *pspec)
{
	TeplStyleSchemeChooserSimple *chooser = TEPL_STYLE_SCHEME_CHOOSER_SIMPLE (object);

	switch (prop_id)
	{
		case PROP_STYLE_SCHEME_ID:
			tepl_style_scheme_chooser_simple_set_style_scheme_id (chooser, g_value_get_string (value));
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
tepl_style_scheme_chooser_simple_dispose (GObject *object)
{
	TeplStyleSchemeChooserSimple *chooser = TEPL_STYLE_SCHEME_CHOOSER_SIMPLE (object);

	chooser->priv->list_box = NULL;

	G_OBJECT_CLASS (tepl_style_scheme_chooser_simple_parent_class)->dispose (object);
}

static void
tepl_style_scheme_chooser_simple_finalize (GObject *object)
{
	TeplStyleSchemeChooserSimple *chooser = TEPL_STYLE_SCHEME_CHOOSER_SIMPLE (object);

	g_free (chooser->priv->style_scheme_id);

	G_OBJECT_CLASS (tepl_style_scheme_chooser_simple_parent_class)->finalize (object);
}

static void
tepl_style_scheme_chooser_simple_map (GtkWidget *widget)
{
	TeplStyleSchemeChooserSimple *chooser = TEPL_STYLE_SCHEME_CHOOSER_SIMPLE (widget);

	if (GTK_WIDGET_CLASS (tepl_style_scheme_chooser_simple_parent_class)->map != NULL)
	{
		GTK_WIDGET_CLASS (tepl_style_scheme_chooser_simple_parent_class)->map (widget);
	}

	tepl_utils_list_box_scroll_to_selected_row (chooser->priv->list_box);
}

static void
tepl_style_scheme_chooser_simple_class_init (TeplStyleSchemeChooserSimpleClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

	object_class->get_property = tepl_style_scheme_chooser_simple_get_property;
	object_class->set_property = tepl_style_scheme_chooser_simple_set_property;
	object_class->dispose = tepl_style_scheme_chooser_simple_dispose;
	object_class->finalize = tepl_style_scheme_chooser_simple_finalize;

	widget_class->map = tepl_style_scheme_chooser_simple_map;

	/**
	 * TeplStyleSchemeChooserSimple:style-scheme-id:
	 *
	 * The selected #GtkSourceStyleScheme ID.
	 *
	 * Its value changes when the user selects another row in the list.
	 *
	 * When set, if the ID doesn't exist, no row is selected.
	 *
	 * Since: 6.11
	 */
	properties[PROP_STYLE_SCHEME_ID] =
		g_param_spec_string ("style-scheme-id",
				     "style-scheme-id",
				     "",
				     NULL,
				     G_PARAM_READWRITE |
				     G_PARAM_STATIC_STRINGS);

	g_object_class_install_properties (object_class, N_PROPERTIES, properties);
}

static void
tepl_style_scheme_chooser_simple_init (TeplStyleSchemeChooserSimple *chooser)
{
	GtkScrolledWindow *scrolled_window;

	chooser->priv = tepl_style_scheme_chooser_simple_get_instance_private (chooser);

	chooser->priv->list_box = create_list_box ();
	scrolled_window = create_scrolled_window ();

	gtk_container_add (GTK_CONTAINER (scrolled_window),
			   GTK_WIDGET (chooser->priv->list_box));
	tepl_utils_list_box_setup_scrolling (chooser->priv->list_box, scrolled_window);

	gtk_container_add (GTK_CONTAINER (chooser),
			   GTK_WIDGET (scrolled_window));
	gtk_widget_show_all (GTK_WIDGET (chooser));
}

/**
 * tepl_style_scheme_chooser_simple_new:
 * @for_dark_theme_variant: whether it's for choosing a style scheme for a dark
 *   GTK theme variant.
 *
 * Creates a new #TeplStyleSchemeChooserSimple widget.
 *
 * The content of the widget will be different depending on
 * @for_dark_theme_variant. That configuration cannot change once the widget is
 * created. If your application supports both the light and dark GTK theme
 * variants, consider using #TeplStyleSchemeChooserFull instead.
 *
 * Returns: (transfer floating): a new #TeplStyleSchemeChooserSimple widget.
 * Since: 6.11
 */
TeplStyleSchemeChooserSimple *
tepl_style_scheme_chooser_simple_new (gboolean for_dark_theme_variant)
{
	TeplStyleSchemeChooserSimple *chooser;

	chooser = g_object_new (TEPL_TYPE_STYLE_SCHEME_CHOOSER_SIMPLE, NULL);
	chooser->priv->for_dark_theme_variant = for_dark_theme_variant != FALSE;

	g_signal_connect (chooser->priv->list_box,
			  "selected-rows-changed",
			  G_CALLBACK (list_box_selected_rows_changed_cb),
			  chooser);

	g_signal_connect_object (gtk_source_style_scheme_manager_get_default (),
				 "changed",
				 G_CALLBACK (style_scheme_manager_changed_cb),
				 chooser,
				 G_CONNECT_DEFAULT);

	full_repopulate (chooser);

	return chooser;
}

/**
 * tepl_style_scheme_chooser_simple_get_style_scheme_id:
 * @chooser: a #TeplStyleSchemeChooserSimple.
 *
 * Returns: (nullable): the value of the
 *   #TeplStyleSchemeChooserSimple:style-scheme-id property.
 * Since: 6.11
 */
const gchar *
tepl_style_scheme_chooser_simple_get_style_scheme_id (TeplStyleSchemeChooserSimple *chooser)
{
	g_return_val_if_fail (TEPL_IS_STYLE_SCHEME_CHOOSER_SIMPLE (chooser), NULL);
	return chooser->priv->style_scheme_id;
}

/**
 * tepl_style_scheme_chooser_simple_set_style_scheme_id:
 * @chooser: a #TeplStyleSchemeChooserSimple.
 * @style_scheme_id: the new value.
 *
 * Sets the #TeplStyleSchemeChooserSimple:style-scheme-id property.
 *
 * Since: 6.11
 */
void
tepl_style_scheme_chooser_simple_set_style_scheme_id (TeplStyleSchemeChooserSimple *chooser,
						      const gchar                  *style_scheme_id)
{
	g_return_if_fail (TEPL_IS_STYLE_SCHEME_CHOOSER_SIMPLE (chooser));

	if (g_set_str (&chooser->priv->style_scheme_id, style_scheme_id))
	{
		g_signal_handlers_block_by_func (chooser->priv->list_box,
						 list_box_selected_rows_changed_cb,
						 chooser);
		update_selected_row (chooser);
		g_signal_handlers_unblock_by_func (chooser->priv->list_box,
						   list_box_selected_rows_changed_cb,
						   chooser);

		g_object_notify_by_pspec (G_OBJECT (chooser), properties[PROP_STYLE_SCHEME_ID]);
	}
}
