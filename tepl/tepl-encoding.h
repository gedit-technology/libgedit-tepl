/* SPDX-FileCopyrightText: 2002-2005 - Paolo Maggi
 * SPDX-FileCopyrightText: 2014, 2015, 2017 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_ENCODING_H
#define TEPL_ENCODING_H

#include <glib-object.h>

G_BEGIN_DECLS

#define TEPL_TYPE_ENCODING (tepl_encoding_get_type ())

typedef struct _TeplEncoding TeplEncoding;

GType			tepl_encoding_get_type			(void) G_GNUC_CONST;

TeplEncoding *		tepl_encoding_new			(const gchar *charset);

TeplEncoding *		tepl_encoding_new_utf8			(void);

TeplEncoding *		tepl_encoding_new_from_locale		(void);

TeplEncoding *		tepl_encoding_copy			(const TeplEncoding *enc);

void			tepl_encoding_free			(TeplEncoding *enc);

const gchar *		tepl_encoding_get_charset		(const TeplEncoding *enc);

const gchar *		tepl_encoding_get_name			(const TeplEncoding *enc);

gchar *			tepl_encoding_to_string			(const TeplEncoding *enc);

gboolean		tepl_encoding_is_utf8			(const TeplEncoding *enc);

gboolean		tepl_encoding_equals			(const TeplEncoding *enc1,
								 const TeplEncoding *enc2);

GSList *		tepl_encoding_get_all			(void);

GSList *		tepl_encoding_get_default_candidates	(void);

G_END_DECLS

#endif  /* TEPL_ENCODING_H */
