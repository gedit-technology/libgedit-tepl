/* SPDX-FileCopyrightText: 2022 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_ENCODING_H
#define TEPL_ENCODING_H

#include <glib.h>

G_BEGIN_DECLS

typedef struct _TeplEncoding TeplEncoding;
typedef struct _TeplEncodingVtable TeplEncodingVtable;

struct _TeplEncoding
{
	TeplEncodingVtable *vtable;
};

struct _TeplEncodingVtable
{
	const gchar *	(* get_category_name)	(const TeplEncoding *enc);
};

G_GNUC_INTERNAL
const gchar *	tepl_encoding_get_category_name		(const TeplEncoding *enc);

G_END_DECLS

#endif /* TEPL_ENCODING_H */
