/* SPDX-FileCopyrightText: 2022 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <tepl/tepl.h>

static void
delete_current_line (TeplView *view)
{
	g_signal_emit_by_name (view, "delete-from-cursor", GTK_DELETE_PARAGRAPHS, 1);
}

static void
select_range (GtkTextBuffer *buffer,
	      gint           selection_start_offset,
	      gint           selection_end_offset)
{
	GtkTextIter selection_start_iter;
	GtkTextIter selection_end_iter;

	gtk_text_buffer_get_iter_at_offset (buffer, &selection_start_iter, selection_start_offset);
	gtk_text_buffer_get_iter_at_offset (buffer, &selection_end_iter, selection_end_offset);

	gtk_text_buffer_select_range (buffer, &selection_start_iter, &selection_end_iter);
}

static void
check_buffer_content (GtkTextBuffer *buffer,
		      const gchar   *expected_content)
{
	GtkTextIter start;
	GtkTextIter end;
	gchar *content;

	gtk_text_buffer_get_bounds (buffer, &start, &end);
	content = gtk_text_buffer_get_slice (buffer, &start, &end, TRUE);
	g_assert_cmpstr (content, ==, expected_content);

	g_free (content);
}

static void
check_delete_current_line (TeplView    *view,
			   const gchar *content_before,
			   gint         selection_start_offset_before,
			   gint         selection_end_offset_before,
			   const gchar *content_after,
			   gint         cursor_offset_after)
{
	GtkTextBuffer *buffer;
	GtkTextIter cursor_pos;

	/* Prepa */

	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));
	gtk_text_buffer_set_text (buffer, content_before, -1);
	select_range (buffer, selection_start_offset_before, selection_end_offset_before);

	/* Operation */

	delete_current_line (view);

	/* Checks */

	check_buffer_content (buffer, content_after);
	g_assert_false (gtk_text_buffer_get_has_selection (buffer));

	gtk_text_buffer_get_iter_at_mark (buffer, &cursor_pos, gtk_text_buffer_get_insert (buffer));
	g_assert_cmpint (cursor_offset_after, ==, gtk_text_iter_get_offset (&cursor_pos));
}

static void
test_delete_current_line (void)
{
	TeplView *view;

	view = TEPL_VIEW (tepl_view_new ());
	g_object_ref_sink (view);

	/* Empty buffer */
	check_delete_current_line (view, "", 0, 0, "", 0);

	/* Single line without trailing \n */
	// Without selection
	check_delete_current_line (view, "ab", 0, 0, "", 0);
	check_delete_current_line (view, "ab", 1, 1, "", 0);
	check_delete_current_line (view, "ab", 2, 2, "", 0);
	// With selection
	check_delete_current_line (view, "ab", 0, 1, "", 0);
	check_delete_current_line (view, "ab", 0, 2, "", 0);
	check_delete_current_line (view, "ab", 1, 2, "", 0);

	/* Single line with trailing \n (so two lines with the second line
	 * empty).
	 */
	// Without selection
	check_delete_current_line (view, "ab\n", 0, 0, "", 0);
	check_delete_current_line (view, "ab\n", 1, 1, "", 0);
	check_delete_current_line (view, "ab\n", 2, 2, "", 0);
	check_delete_current_line (view, "ab\n", 3, 3, "ab", 0);
	// With selection
	check_delete_current_line (view, "ab\n", 0, 1, "", 0);
	check_delete_current_line (view, "ab\n", 0, 2, "", 0);
	check_delete_current_line (view, "ab\n", 1, 2, "", 0);
	check_delete_current_line (view, "ab\n", 0, 3, "", 0);
	check_delete_current_line (view, "ab\n", 1, 3, "", 0);
	check_delete_current_line (view, "ab\n", 2, 3, "", 0);

	/* Two lines, delete second line */
	// Without selection
	check_delete_current_line (view, "ab\nc", 3, 3, "ab", 0);
	check_delete_current_line (view, "ab\nc", 4, 4, "ab", 0);
	// With selection
	check_delete_current_line (view, "ab\nc", 3, 4, "ab", 0);

	/* Two lines with selection */
	check_delete_current_line (view, "a\nb", 0, 1, "b", 0);
	check_delete_current_line (view, "a\nb", 0, 2, "b", 0);
	check_delete_current_line (view, "a\nb", 0, 3, "", 0);

	// This one is a bit more difficult. We select only the \n, but in the
	// TextView the selection is visible only on the first line, after the
	// 'a'. So 'b' remains.
	check_delete_current_line (view, "a\nb", 1, 2, "b", 0);

	check_delete_current_line (view, "a\nb", 1, 3, "", 0);
	check_delete_current_line (view, "a\nb", 2, 3, "a", 0);

	/* Three lines */
	// Without selection, delete second line
	check_delete_current_line (view, "ab\nc\nde", 3, 3, "ab\nde", 3);
	check_delete_current_line (view, "ab\nc\nde", 4, 4, "ab\nde", 3);
	// With selection
	check_delete_current_line (view, "ab\nc\nde", 3, 4, "ab\nde", 3);
	check_delete_current_line (view, "ab\nc\nde", 3, 5, "ab\nde", 3);
	check_delete_current_line (view, "ab\nc\nde", 2, 3, "c\nde", 0);
	check_delete_current_line (view, "ab\nc\nde", 2, 4, "de", 0);

	g_object_unref (view);
}

int
main (int    argc,
      char **argv)
{
	gtk_test_init (&argc, &argv);

	g_test_add_func ("/view/delete_current_line", test_delete_current_line);

	return g_test_run ();
}
