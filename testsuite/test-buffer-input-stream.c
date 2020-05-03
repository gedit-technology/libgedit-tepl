/* Copyright 2010 - Ignacio Casal Quinteiro
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <tepl/tepl.h>
#include "tepl/tepl-buffer-input-stream.h"
#include <string.h>

static void
test_consecutive_read (const gchar     *inbuf,
		       const gchar     *outbuf,
		       TeplNewlineType  type,
		       gsize            read_chunk_len)
{
	GtkTextBuffer *buf;
	TeplBufferInputStream *in;
	gsize outlen;
	gssize n, r;
	GError *err = NULL;
	gchar *b;
	gboolean close;

	buf = gtk_text_buffer_new (NULL);
	gtk_text_buffer_set_text (buf, inbuf, -1);

	b = g_malloc (200);
	in = _tepl_buffer_input_stream_new (buf, type, TRUE);

	outlen = strlen (outbuf);
	n = 0;

	do
	{
		r = g_input_stream_read (G_INPUT_STREAM (in), b + n, read_chunk_len, NULL, &err);
		g_assert_cmpint (r, >=, 0);
		g_assert_no_error (err);

		n += r;
	} while (r != 0);

	g_assert_cmpint (n, ==, outlen);

	b[n] = '\0';

	g_assert_cmpstr (b, ==, outbuf);

	close = g_input_stream_close (G_INPUT_STREAM (in), NULL, &err);
	g_assert_true (close);
	g_assert_no_error (err);

	g_object_unref (buf);
	g_object_unref (in);
	g_free (b);
}

static void
test_empty (void)
{
	/* empty file should not have a trailing newline */
	test_consecutive_read ("", "", TEPL_NEWLINE_TYPE_CR_LF, 10);
}

static void
test_consecutive_cut_char (void)
{
	/* first \n is read then fo and then is added \r but not \n */
	test_consecutive_read ("\nfo\nbar\n\nblah\n", "\r\nfo\r\nbar\r\n\r\nblah\r\n\r\n", TEPL_NEWLINE_TYPE_CR_LF, 8);
	test_consecutive_read ("\nfo\nbar\n\nblah", "\r\nfo\r\nbar\r\n\r\nblah\r\n", TEPL_NEWLINE_TYPE_CR_LF, 8);
}

static void
test_consecutive_big_read (void)
{
	test_consecutive_read ("\nfo\nbar\n\nblah\n", "\rfo\rbar\r\rblah\r\r", TEPL_NEWLINE_TYPE_CR, 200);
	test_consecutive_read ("\nfo\nbar\n\nblah", "\rfo\rbar\r\rblah\r", TEPL_NEWLINE_TYPE_CR, 200);

	test_consecutive_read ("\rfo\rbar\r\rblah\r", "\nfo\nbar\n\nblah\n\n", TEPL_NEWLINE_TYPE_LF, 200);
	test_consecutive_read ("\rfo\rbar\r\rblah", "\nfo\nbar\n\nblah\n", TEPL_NEWLINE_TYPE_LF, 200);

	test_consecutive_read ("\r\nfo\r\nbar\r\n\r\nblah\r\n", "\nfo\nbar\n\nblah\n\n", TEPL_NEWLINE_TYPE_LF, 200);
	test_consecutive_read ("\r\nfo\r\nbar\r\n\r\nblah", "\nfo\nbar\n\nblah\n", TEPL_NEWLINE_TYPE_LF, 200);

	test_consecutive_read ("\nfo\nbar\n\nblah\n", "\r\nfo\r\nbar\r\n\r\nblah\r\n\r\n", TEPL_NEWLINE_TYPE_CR_LF, 200);
	test_consecutive_read ("\nfo\nbar\n\nblah", "\r\nfo\r\nbar\r\n\r\nblah\r\n", TEPL_NEWLINE_TYPE_CR_LF, 200);
}

static void
test_consecutive_middle_read (void)
{
	test_consecutive_read ("\nfo\nbar\n\nblah\n", "\rfo\rbar\r\rblah\r\r", TEPL_NEWLINE_TYPE_CR, 6);
	test_consecutive_read ("\nfo\nbar\n\nblah", "\rfo\rbar\r\rblah\r", TEPL_NEWLINE_TYPE_CR, 6);

	test_consecutive_read ("\rfo\rbar\r\rblah\r", "\nfo\nbar\n\nblah\n\n", TEPL_NEWLINE_TYPE_LF, 6);
	test_consecutive_read ("\rfo\rbar\r\rblah", "\nfo\nbar\n\nblah\n", TEPL_NEWLINE_TYPE_LF, 6);

	test_consecutive_read ("\r\nfo\r\nbar\r\n\r\nblah\r\n", "\nfo\nbar\n\nblah\n\n", TEPL_NEWLINE_TYPE_LF, 6);
	test_consecutive_read ("\r\nfo\r\nbar\r\n\r\nblah", "\nfo\nbar\n\nblah\n", TEPL_NEWLINE_TYPE_LF, 6);

	test_consecutive_read ("\nfo\nbar\n\nblah\n", "\r\nfo\r\nbar\r\n\r\nblah\r\n\r\n", TEPL_NEWLINE_TYPE_CR_LF, 6);
	test_consecutive_read ("\nfo\nbar\n\nblah", "\r\nfo\r\nbar\r\n\r\nblah\r\n", TEPL_NEWLINE_TYPE_CR_LF, 6);
}

static void
test_consecutive_multibyte_cut (void)
{
	test_consecutive_read ("hello\nhello\xe6\x96\x87\nworld\n", "hello\rhello\xe6\x96\x87\rworld\r\r", TEPL_NEWLINE_TYPE_CR, 6);
	test_consecutive_read ("hello\rhello\xe6\x96\x87\rworld\r", "hello\rhello\xe6\x96\x87\rworld\r\r", TEPL_NEWLINE_TYPE_CR, 6);
	test_consecutive_read ("hello\nhello\xe6\x96\x87\nworld\n", "hello\nhello\xe6\x96\x87\nworld\n\n", TEPL_NEWLINE_TYPE_LF, 6);
}

static void
test_consecutive_multibyte_big_read (void)
{
	test_consecutive_read ("hello\nhello\xe6\x96\x87\nworld\n", "hello\rhello\xe6\x96\x87\rworld\r\r", TEPL_NEWLINE_TYPE_CR, 200);
	test_consecutive_read ("hello\rhello\xe6\x96\x87\rworld\r", "hello\rhello\xe6\x96\x87\rworld\r\r", TEPL_NEWLINE_TYPE_CR, 200);
	test_consecutive_read ("hello\nhello\xe6\x96\x87\nworld\n", "hello\nhello\xe6\x96\x87\nworld\n\n", TEPL_NEWLINE_TYPE_LF, 200);
}

gint
main (gint   argc,
      gchar *argv[])
{
	g_test_init (&argc, &argv, NULL);

	g_test_add_func ("/buffer-input-stream/empty", test_empty);

	g_test_add_func ("/buffer-input-stream/consecutive_cut_char", test_consecutive_cut_char);
	g_test_add_func ("/buffer-input-stream/consecutive_big_read", test_consecutive_big_read);
	g_test_add_func ("/buffer-input-stream/consecutive_middle_read", test_consecutive_middle_read);

	g_test_add_func ("/buffer-input-stream/consecutive_multibyte_cut", test_consecutive_multibyte_cut);
	g_test_add_func ("/buffer-input-stream/consecutive_multibyte_big_read", test_consecutive_multibyte_big_read);

	return g_test_run ();
}
