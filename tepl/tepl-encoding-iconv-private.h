/* SPDX-FileCopyrightText: 2014, 2017 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_ENCODING_ICONV_PRIVATE_H
#define TEPL_ENCODING_ICONV_PRIVATE_H

#include <glib.h>

G_BEGIN_DECLS

/*
 * TeplEncodingIconvDuplicates:
 * @TEPL_ENCODING_ICONV_DUPLICATES_KEEP_FIRST: Keep the first occurrence.
 * @TEPL_ENCODING_ICONV_DUPLICATES_KEEP_LAST: Keep the last occurrence.
 *
 * Specifies which encoding occurrence to keep when removing duplicated
 * encodings in a list with _tepl_encoding_remove_duplicates().
 */
typedef enum _TeplEncodingIconvDuplicates
{
	TEPL_ENCODING_ICONV_DUPLICATES_KEEP_FIRST,
	TEPL_ENCODING_ICONV_DUPLICATES_KEEP_LAST
} TeplEncodingIconvDuplicates;

G_GNUC_INTERNAL
GSList *	_tepl_encoding_iconv_remove_duplicates	(GSList                      *encodings,
							 TeplEncodingIconvDuplicates  removal_type);

G_END_DECLS

#endif /* TEPL_ENCODING_ICONV_PRIVATE_H */
