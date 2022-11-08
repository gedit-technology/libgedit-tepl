/* From gedit-view.c:
 * SPDX-FileCopyrightText: 1998, 1999 - Alex Roberts, Evan Lawrence
 * SPDX-FileCopyrightText: 2000, 2002 - Chema Celorio, Paolo Maggi
 * SPDX-FileCopyrightText: 2003-2005 - Paolo Maggi
 *
 * SPDX-FileCopyrightText: 2016-2022 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "tepl-view.h"
#include "tepl-buffer.h"

/**
 * SECTION:view
 * @Short_description: Subclass of #GtkSourceView
 * @Title: TeplView
 *
 * #TeplView is a subclass of #GtkSourceView, to add more features useful for a
 * text editor.
 */

#define SCROLL_MARGIN 0.02

G_DEFINE_TYPE (TeplView, tepl_view, GTK_SOURCE_TYPE_VIEW)

static GtkTextBuffer *
tepl_view_create_buffer (GtkTextView *view)
{
	return GTK_TEXT_BUFFER (tepl_buffer_new ());
}

static void
set_to_line_end (GtkTextIter *iter)
{
	if (!gtk_text_iter_ends_line (iter))
	{
		gtk_text_iter_forward_to_line_end (iter);
	}
}

static void
get_extents_for_lines_deletion (GtkTextBuffer *buffer,
				GtkTextIter   *start,
				GtkTextIter   *end)
{
	gboolean has_selection;

	has_selection = gtk_text_buffer_get_selection_bounds (buffer, start, end);

	gtk_text_iter_set_line_offset (start, 0);

	if (has_selection && gtk_text_iter_starts_line (end))
	{
		/* In this case, the GtkTextView displays the selection only on
		 * the previous line(s), not on the line where 'end' is, so
		 * don't delete the line located at 'end'.
		 */
		return;
	}

	set_to_line_end (end);

	/* We have now the same situation with 'start' at the start of a line,
	 * and 'end' at the end of a line, with start <= end.
	 * We must just include one more \n (or equivalent) to not leave a blank
	 * line as a result.
	 */
	if (!gtk_text_iter_is_end (end))
	{
		gtk_text_iter_forward_line (end);
	}
	/* We take the \n from the previous line, if there is one. */
	else if (!gtk_text_iter_is_start (start))
	{
		gtk_text_iter_backward_line (start);
		set_to_line_end (start);
	}
}

static void
delete_lines (GtkTextView *text_view)
{
	GtkTextBuffer *buffer;
	GtkTextIter start;
	GtkTextIter end;

	gtk_text_view_reset_im_context (text_view);

	buffer = gtk_text_view_get_buffer (text_view);
	get_extents_for_lines_deletion (buffer, &start, &end);

	if (!gtk_text_iter_equal (&start, &end))
	{
		GtkTextIter cursor = start;
		gtk_text_iter_set_line_offset (&cursor, 0);

		gtk_text_buffer_begin_user_action (buffer);

		gtk_text_buffer_place_cursor (buffer, &cursor);

		gtk_text_buffer_delete_interactive (buffer,
						    &start,
						    &end,
						    gtk_text_view_get_editable (text_view));

		gtk_text_buffer_end_user_action (buffer);

		gtk_text_view_scroll_mark_onscreen (text_view,
						    gtk_text_buffer_get_insert (buffer));
	}
	else
	{
		gtk_widget_error_bell (GTK_WIDGET (text_view));
	}
}

/* We override the standard handler for delete_from_cursor since the
 * GTK_DELETE_PARAGRAPHS case is not implemented as we like (i.e. it does not
 * remove the newline in the previous line).
 * Useful for a keybinding to delete the current line.
 */
static void
tepl_view_delete_from_cursor (GtkTextView   *text_view,
			      GtkDeleteType  type,
			      gint           count)
{
	/* For a keybinding we only care about count==1, so it permits to
	 * simplify the implementation.
	 */
	if (type == GTK_DELETE_PARAGRAPHS && count == 1)
	{
		delete_lines (text_view);
		return;
	}

	if (GTK_TEXT_VIEW_CLASS (tepl_view_parent_class)->delete_from_cursor != NULL)
	{
		GTK_TEXT_VIEW_CLASS (tepl_view_parent_class)->delete_from_cursor (text_view, type, count);
	}
}

static void
tepl_view_class_init (TeplViewClass *klass)
{
	GtkTextViewClass *text_view_class = GTK_TEXT_VIEW_CLASS (klass);

	text_view_class->create_buffer = tepl_view_create_buffer;
	text_view_class->delete_from_cursor = tepl_view_delete_from_cursor;
}

static void
tepl_view_init (TeplView *view)
{
}

/**
 * tepl_view_new:
 *
 * Creates a new #TeplView.
 *
 * By default, an empty #TeplBuffer will be lazily created and can be retrieved
 * with gtk_text_view_get_buffer().
 *
 * If you want to specify your own buffer, either override the
 * #GtkTextViewClass create_buffer factory method, or use
 * tepl_view_new_with_buffer().
 *
 * Returns: a new #TeplView.
 * Since: 1.0
 */
GtkWidget *
tepl_view_new (void)
{
	return g_object_new (TEPL_TYPE_VIEW, NULL);
}

/**
 * tepl_view_new_with_buffer:
 * @buffer: a #GtkSourceBuffer or a subclass of it.
 *
 * Creates a new #TeplView widget displaying the buffer @buffer.
 *
 * Returns: a new #TeplView.
 * Since: 5.0
 */
GtkWidget *
tepl_view_new_with_buffer (GtkSourceBuffer *buffer)
{
	g_return_val_if_fail (GTK_SOURCE_IS_BUFFER (buffer), NULL);

	return g_object_new (TEPL_TYPE_VIEW,
			     "buffer", buffer,
			     NULL);
}

/**
 * tepl_view_cut_clipboard:
 * @view: a #TeplView.
 *
 * Cuts the clipboard and then scrolls to the cursor position.
 *
 * Since: 1.0
 */
void
tepl_view_cut_clipboard (TeplView *view)
{
	GtkTextBuffer *buffer;
	GtkClipboard *clipboard;

	g_return_if_fail (TEPL_IS_VIEW (view));

	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));

	clipboard = gtk_widget_get_clipboard (GTK_WIDGET (view),
					      GDK_SELECTION_CLIPBOARD);

	gtk_text_buffer_cut_clipboard (buffer,
				       clipboard,
				       gtk_text_view_get_editable (GTK_TEXT_VIEW (view)));

	gtk_text_view_scroll_to_mark (GTK_TEXT_VIEW (view),
				      gtk_text_buffer_get_insert (buffer),
				      SCROLL_MARGIN,
				      FALSE,
				      0.0,
				      0.0);
}

/**
 * tepl_view_copy_clipboard:
 * @view: a #TeplView.
 *
 * Copies the clipboard.
 *
 * Since: 1.0
 */
void
tepl_view_copy_clipboard (TeplView *view)
{
	GtkTextBuffer *buffer;
	GtkClipboard *clipboard;

	g_return_if_fail (TEPL_IS_VIEW (view));

	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));

	clipboard = gtk_widget_get_clipboard (GTK_WIDGET (view),
					      GDK_SELECTION_CLIPBOARD);

	gtk_text_buffer_copy_clipboard (buffer, clipboard);

	/* On copy do not scroll, we are already on screen. */
}

/**
 * tepl_view_paste_clipboard:
 * @view: a #TeplView.
 *
 * Pastes the clipboard and then scrolls to the cursor position.
 *
 * Since: 1.0
 */
void
tepl_view_paste_clipboard (TeplView *view)
{
	GtkTextBuffer *buffer;
	GtkClipboard *clipboard;

	g_return_if_fail (TEPL_IS_VIEW (view));

	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));

	clipboard = gtk_widget_get_clipboard (GTK_WIDGET (view),
					      GDK_SELECTION_CLIPBOARD);

	gtk_text_buffer_paste_clipboard (buffer,
					 clipboard,
					 NULL,
					 gtk_text_view_get_editable (GTK_TEXT_VIEW (view)));

	/* GtkTextView already connects to the GtkTextBuffer::paste-done signal
	 * to scroll to the cursor position.
	 */
}

/**
 * tepl_view_delete_selection:
 * @view: a #TeplView.
 *
 * Deletes the text currently selected in the #GtkTextBuffer associated
 * to the view and then scrolls to the cursor position.
 *
 * Since: 1.0
 */
void
tepl_view_delete_selection (TeplView *view)
{
	GtkTextBuffer *buffer;

	g_return_if_fail (TEPL_IS_VIEW (view));

	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));

	gtk_text_buffer_delete_selection (buffer,
					  TRUE,
					  gtk_text_view_get_editable (GTK_TEXT_VIEW (view)));

	gtk_text_view_scroll_to_mark (GTK_TEXT_VIEW (view),
				      gtk_text_buffer_get_insert (buffer),
				      SCROLL_MARGIN,
				      FALSE,
				      0.0,
				      0.0);
}

/**
 * tepl_view_select_all:
 * @view: a #TeplView.
 *
 * Selects all the text.
 *
 * Since: 1.0
 */
void
tepl_view_select_all (TeplView *view)
{
	GtkTextBuffer *buffer;
	GtkTextIter start;
	GtkTextIter end;

	g_return_if_fail (TEPL_IS_VIEW (view));

	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));

	gtk_text_buffer_get_bounds (buffer, &start, &end);
	gtk_text_buffer_select_range (buffer, &start, &end);
}

/**
 * tepl_view_scroll_to_cursor:
 * @view: a #TeplView.
 *
 * Scrolls the @view to the cursor position.
 *
 * Since: 1.0
 */
void
tepl_view_scroll_to_cursor (TeplView *view)
{
	GtkTextBuffer *buffer;

	g_return_if_fail (TEPL_IS_VIEW (view));

	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));

	gtk_text_view_scroll_to_mark (GTK_TEXT_VIEW (view),
				      gtk_text_buffer_get_insert (buffer),
				      0.25,
				      FALSE,
				      0.0,
				      0.0);
}

/**
 * tepl_view_goto_line:
 * @view: a #TeplView.
 * @line: a line number, counting from 0.
 *
 * Places the cursor at the position returned by
 * gtk_text_buffer_get_iter_at_line(), and scrolls to that position.
 *
 * Returns: %TRUE if the cursor has been moved exactly to @line, %FALSE if that
 *   line didn't exist.
 * Since: 2.0
 */
gboolean
tepl_view_goto_line (TeplView *view,
		     gint      line)
{
	GtkTextBuffer *buffer;
	GtkTextIter iter;
	gboolean line_exists;

	g_return_val_if_fail (TEPL_IS_VIEW (view), FALSE);

	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));

	gtk_text_buffer_get_iter_at_line (buffer, &iter, line);
	line_exists = gtk_text_iter_get_line (&iter) == line;

	gtk_text_buffer_place_cursor (buffer, &iter);
	tepl_view_scroll_to_cursor (view);

	return line_exists;
}

/**
 * tepl_view_goto_line_offset:
 * @view: a #TeplView.
 * @line: a line number, counting from 0.
 * @line_offset: the line offset, in characters (not bytes).
 *
 * Places the cursor at the position returned by
 * gtk_text_buffer_get_iter_at_line_offset(), and scrolls to that position.
 *
 * Returns: %TRUE if the cursor has been moved exactly to @line and
 *   @line_offset, %FALSE if that position didn't exist.
 * Since: 2.0
 */
gboolean
tepl_view_goto_line_offset (TeplView *view,
			    gint      line,
			    gint      line_offset)
{
	GtkTextBuffer *buffer;
	GtkTextIter iter;
	gboolean pos_exists;

	g_return_val_if_fail (TEPL_IS_VIEW (view), FALSE);

	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));

	gtk_text_buffer_get_iter_at_line_offset (buffer,
						 &iter,
						 line,
						 line_offset);

	pos_exists = (gtk_text_iter_get_line (&iter) == line &&
		      gtk_text_iter_get_line_offset (&iter) == line_offset);

	gtk_text_buffer_place_cursor (buffer, &iter);
	tepl_view_scroll_to_cursor (view);

	return pos_exists;
}

/**
 * tepl_view_select_lines:
 * @view: a #TeplView.
 * @start_line: start of the region to select.
 * @end_line: end of the region to select.
 *
 * Selects the lines between @start_line and @end_line included, counting from
 * zero. And then scrolls to the cursor.
 *
 * Possible use-case: line numbers coming from a compilation output, to go to
 * the place where a warning or error occurred.
 *
 * Since: 2.0
 */
void
tepl_view_select_lines (TeplView *view,
			gint      start_line,
			gint      end_line)
{
	GtkTextBuffer *buffer;
	GtkTextIter start_iter;
	GtkTextIter end_iter;

	g_return_if_fail (TEPL_IS_VIEW (view));

	if (end_line < start_line)
	{
		gint start_line_copy;

		/* Swap start_line and end_line */
		start_line_copy = start_line;
		start_line = end_line;
		end_line = start_line_copy;
	}

	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));

	gtk_text_buffer_get_iter_at_line (buffer, &start_iter, start_line);
	gtk_text_buffer_get_iter_at_line (buffer, &end_iter, end_line);

	if (!gtk_text_iter_ends_line (&end_iter))
	{
		gtk_text_iter_forward_to_line_end (&end_iter);
	}

	gtk_text_buffer_select_range (buffer, &start_iter, &end_iter);

	tepl_view_scroll_to_cursor (view);
}
