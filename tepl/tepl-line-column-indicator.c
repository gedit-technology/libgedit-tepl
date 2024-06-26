/* SPDX-FileCopyrightText: 2022 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "config.h"
#include "tepl-line-column-indicator.h"
#include <glib/gi18n-lib.h>
#include "tepl-signal-group.h"
#include "tepl-utils.h"

/**
 * SECTION:line-column-indicator
 * @Short_description: Indicates the cursor position
 * @Title: TeplLineColumnIndicator
 *
 * #TeplLineColumnIndicator indicates the cursor position by line and column
 * numbers. For example: "Ln 2, Col 3".
 *
 * The usual case is to pack this widget in a #GtkStatusbar.
 */

struct _TeplLineColumnIndicatorPrivate
{
	GtkStack *stack;
	GtkLabel *label;

	/* If @tab_group is set, then @view is always %NULL.
	 * If @tab_group is %NULL, then @view is used but may be %NULL.
	 */
	TeplView *view; /* owned */
	TeplTabGroup *tab_group; /* owned */

	TeplSignalGroup *view_signal_group;
	TeplSignalGroup *buffer_signal_group;
};

G_DEFINE_TYPE_WITH_PRIVATE (TeplLineColumnIndicator, tepl_line_column_indicator, GTK_TYPE_BIN)

static gchar *
get_label_text (gint line,
		gint column)
{
	gchar *base_text;
	gchar *text_with_padding;

	/* Translators: "Ln" is an abbreviation for "Line", Col is an
	 * abbreviation for "Column". Please, use abbreviations if possible.
	 */
	base_text = g_strdup_printf (_("Ln %d, Col %d"), line, column);
	text_with_padding = _tepl_utils_statusbar_add_padding_to_text (base_text);
	g_free (base_text);

	return text_with_padding;
}

static void
set_values (TeplLineColumnIndicator *indicator,
	    gint                     line,
	    gint                     column)
{
	gchar *text;

	g_return_if_fail (line >= 1);
	g_return_if_fail (column >= 1);

	text = get_label_text (line, column);
	gtk_label_set_text (indicator->priv->label, text);
	g_free (text);
}

static void
set_values_for_view (TeplLineColumnIndicator *indicator,
		     TeplView                *view)
{
	GtkTextBuffer *buffer;
	GtkTextIter iter;
	gint line;
	gint column;

	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));
	gtk_text_buffer_get_iter_at_mark (buffer,
					  &iter,
					  gtk_text_buffer_get_insert (buffer));

	line = gtk_text_iter_get_line (&iter);
	column = gtk_source_view_get_visual_column (GTK_SOURCE_VIEW (view), &iter);

	set_values (indicator, line + 1, column + 1);
}

static TeplView *
get_view (TeplLineColumnIndicator *indicator)
{
	if (indicator->priv->tab_group != NULL)
	{
		return tepl_tab_group_get_active_view (indicator->priv->tab_group);
	}

	return indicator->priv->view;
}

static void
update_cursor_position (TeplLineColumnIndicator *indicator)
{
	TeplView *view = get_view (indicator);

	if (view == NULL)
	{
		gtk_widget_hide (GTK_WIDGET (indicator->priv->stack));
	}
	else
	{
		set_values_for_view (indicator, view);
		gtk_widget_show (GTK_WIDGET (indicator->priv->stack));
	}
}

static void
tepl_line_column_indicator_dispose (GObject *object)
{
	TeplLineColumnIndicator *indicator = TEPL_LINE_COLUMN_INDICATOR (object);

	tepl_signal_group_clear (&indicator->priv->view_signal_group);
	tepl_signal_group_clear (&indicator->priv->buffer_signal_group);

	tepl_utils_set_widget ((GtkWidget **) &indicator->priv->view, NULL);
	g_clear_object (&indicator->priv->tab_group);

	/* In case a public function is called after a first dispose. */
	indicator->priv->label = NULL;

	G_OBJECT_CLASS (tepl_line_column_indicator_parent_class)->dispose (object);
}

static void
tepl_line_column_indicator_class_init (TeplLineColumnIndicatorClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->dispose = tepl_line_column_indicator_dispose;
}

static GtkWidget *
create_label_for_size_allocation (void)
{
	GtkWidget *label;
	gchar *text;

	text = get_label_text (9999, 999);
	label = gtk_label_new (text);
	gtk_widget_show (label);
	g_free (text);

	return label;
}

static void
tepl_line_column_indicator_init (TeplLineColumnIndicator *indicator)
{
	indicator->priv = tepl_line_column_indicator_get_instance_private (indicator);

	indicator->priv->stack = GTK_STACK (gtk_stack_new ());
	gtk_widget_show (GTK_WIDGET (indicator->priv->stack));

	indicator->priv->label = GTK_LABEL (gtk_label_new (NULL));
	gtk_widget_show (GTK_WIDGET (indicator->priv->label));

	gtk_container_add (GTK_CONTAINER (indicator->priv->stack),
			   GTK_WIDGET (indicator->priv->label));
	gtk_container_add (GTK_CONTAINER (indicator->priv->stack),
			   create_label_for_size_allocation ());

	/* The visible child never changes. The "label for size allocation" is
	 * used only to reserve some space, to avoid this widget to "push" the
	 * other widgets (that are usually placed on the left in the statusbar).
	 */
	gtk_stack_set_visible_child (indicator->priv->stack,
				     GTK_WIDGET (indicator->priv->label));

	gtk_container_add (GTK_CONTAINER (indicator),
			   GTK_WIDGET (indicator->priv->stack));
}

/**
 * tepl_line_column_indicator_new:
 *
 * Returns: (transfer floating): a new #TeplLineColumnIndicator widget.
 * Since: 6.4
 */
TeplLineColumnIndicator *
tepl_line_column_indicator_new (void)
{
	return g_object_new (TEPL_TYPE_LINE_COLUMN_INDICATOR, NULL);
}

static void
cursor_moved_cb (TeplBuffer              *buffer,
		 TeplLineColumnIndicator *indicator)
{
	update_cursor_position (indicator);
}

static void
connect_to_buffer (TeplLineColumnIndicator *indicator,
		   TeplBuffer              *buffer)
{
	tepl_signal_group_clear (&indicator->priv->buffer_signal_group);

	if (buffer == NULL)
	{
		return;
	}

	indicator->priv->buffer_signal_group = tepl_signal_group_new (G_OBJECT (buffer));

	tepl_signal_group_add (indicator->priv->buffer_signal_group,
			       g_signal_connect (buffer,
						 "tepl-cursor-moved",
						 G_CALLBACK (cursor_moved_cb),
						 indicator));
}

static void
buffer_notify_cb (TeplView                *view,
		  GParamSpec              *pspec,
		  TeplLineColumnIndicator *indicator)
{
	TeplBuffer *buffer;

	buffer = TEPL_BUFFER (gtk_text_view_get_buffer (GTK_TEXT_VIEW (view)));
	connect_to_buffer (indicator, buffer);

	update_cursor_position (indicator);
}

static void
tab_width_notify_cb (TeplView                *view,
		     GParamSpec              *pspec,
		     TeplLineColumnIndicator *indicator)
{
	/* gtk_source_view_get_visual_column() depends on tab-width. */
	update_cursor_position (indicator);
}

static void
connect_to_view (TeplLineColumnIndicator *indicator,
		 TeplView                *view)
{
	tepl_signal_group_clear (&indicator->priv->view_signal_group);

	if (view == NULL)
	{
		return;
	}

	indicator->priv->view_signal_group = tepl_signal_group_new (G_OBJECT (view));

	tepl_signal_group_add (indicator->priv->view_signal_group,
			       g_signal_connect (view,
						 "notify::buffer",
						 G_CALLBACK (buffer_notify_cb),
						 indicator));

	tepl_signal_group_add (indicator->priv->view_signal_group,
			       g_signal_connect (view,
						 "notify::tab-width",
						 G_CALLBACK (tab_width_notify_cb),
						 indicator));
}

static void
view_changed (TeplLineColumnIndicator *indicator)
{
	TeplView *view;
	TeplBuffer *buffer = NULL;

	view = get_view (indicator);

	if (view != NULL)
	{
		buffer = TEPL_BUFFER (gtk_text_view_get_buffer (GTK_TEXT_VIEW (view)));
	}

	connect_to_view (indicator, view);
	connect_to_buffer (indicator, buffer);

	update_cursor_position (indicator);
}

/**
 * tepl_line_column_indicator_set_view:
 * @indicator: a #TeplLineColumnIndicator.
 * @view: (nullable): a #TeplView, or %NULL.
 *
 * Sets a #TeplView to update automatically the values for the line and column
 * of the current cursor position. If @view is %NULL, then @indicator will be
 * empty.
 *
 * Use tepl_line_column_indicator_set_view() or
 * tepl_line_column_indicator_set_tab_group(), but not both.
 *
 * Since: 6.4
 */
void
tepl_line_column_indicator_set_view (TeplLineColumnIndicator *indicator,
				     TeplView                *view)
{
	g_return_if_fail (TEPL_IS_LINE_COLUMN_INDICATOR (indicator));
	g_return_if_fail (view == NULL || TEPL_IS_VIEW (view));
	g_return_if_fail (indicator->priv->tab_group == NULL);

	tepl_utils_set_widget ((GtkWidget **) &indicator->priv->view,
			       (GtkWidget *) view);
	view_changed (indicator);
}

static void
active_view_notify_cb (TeplTabGroup            *tab_group,
		       GParamSpec              *pspec,
		       TeplLineColumnIndicator *indicator)
{
	view_changed (indicator);
}

/**
 * tepl_line_column_indicator_set_tab_group:
 * @indicator: a #TeplLineColumnIndicator.
 * @tab_group: a #TeplTabGroup.
 *
 * Sets a #TeplTabGroup to update automatically the values for the line and
 * column of the current cursor position, for the current
 * #TeplTabGroup:active-tab.
 *
 * This function can be called only once per @indicator.
 *
 * Use tepl_line_column_indicator_set_view() or
 * tepl_line_column_indicator_set_tab_group(), but not both.
 *
 * Since: 6.4
 */
void
tepl_line_column_indicator_set_tab_group (TeplLineColumnIndicator *indicator,
					  TeplTabGroup            *tab_group)
{
	g_return_if_fail (TEPL_IS_LINE_COLUMN_INDICATOR (indicator));
	g_return_if_fail (TEPL_IS_TAB_GROUP (tab_group));
	g_return_if_fail (indicator->priv->tab_group == NULL);
	g_return_if_fail (indicator->priv->view == NULL);

	indicator->priv->tab_group = g_object_ref_sink (tab_group);

	g_signal_connect_object (tab_group,
				 "notify::active-view",
				 G_CALLBACK (active_view_notify_cb),
				 indicator,
				 0);

	view_changed (indicator);
}
