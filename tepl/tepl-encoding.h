/* SPDX-FileCopyrightText: 2022 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_ENCODING_H
#define TEPL_ENCODING_H

#if !defined (TEPL_H_INSIDE) && !defined (TEPL_COMPILATION)
#error "Only <tepl/tepl.h> can be included directly."
#endif

#include <glib.h>

G_BEGIN_DECLS

typedef struct _TeplEncoding TeplEncoding;
typedef struct _TeplEncodingVtable TeplEncodingVtable;

/**
 * TeplEncoding:
 * @vtable: the #TeplEncodingVtable of the object.
 *
 * To implement the #TeplEncoding interface.
 *
 * Don't access the fields of this struct unless you are writing a class that
 * implements this interface.
 *
 * Since: 6.4
 */
struct _TeplEncoding
{
	TeplEncodingVtable *vtable;
};

/**
 * TeplEncodingVtable:
 * @get_name: For tepl_encoding_get_name().
 * @get_category_name: For tepl_encoding_get_category_name().
 *
 * The virtual function table of the #TeplEncoding interface.
 *
 * Since: 6.4
 */
struct _TeplEncodingVtable
{
	const gchar *	(* get_name)		(const TeplEncoding *enc);

	const gchar *	(* get_category_name)	(const TeplEncoding *enc);
};

G_GNUC_INTERNAL
const gchar *	tepl_encoding_get_name			(const TeplEncoding *enc);

G_GNUC_INTERNAL
const gchar *	tepl_encoding_get_category_name		(const TeplEncoding *enc);

G_END_DECLS

#endif /* TEPL_ENCODING_H */
