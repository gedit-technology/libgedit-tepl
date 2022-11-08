/* SPDX-FileCopyrightText: 2002-2005 - Paolo Maggi
 * SPDX-FileCopyrightText: 2014, 2015, 2017 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_ENCODING_ICONV_H
#define TEPL_ENCODING_ICONV_H

#include "tepl-encoding.h"

G_BEGIN_DECLS

typedef struct _TeplEncodingIconv TeplEncodingIconv;

G_GNUC_INTERNAL
const TeplEncoding *	tepl_encoding_iconv_to_base_type	(const TeplEncodingIconv *enc);

G_GNUC_INTERNAL
TeplEncodingIconv *	tepl_encoding_iconv_new			(const gchar *charset);

G_GNUC_INTERNAL
TeplEncodingIconv *	tepl_encoding_iconv_new_utf8		(void);

G_GNUC_INTERNAL
TeplEncodingIconv *	tepl_encoding_iconv_new_from_locale	(void);

G_GNUC_INTERNAL
TeplEncodingIconv *	tepl_encoding_iconv_copy		(const TeplEncodingIconv *enc);

G_GNUC_INTERNAL
void			tepl_encoding_iconv_free		(TeplEncodingIconv *enc);

G_GNUC_INTERNAL
gchar *			tepl_encoding_iconv_to_string		(const TeplEncodingIconv *enc);

G_GNUC_INTERNAL
gboolean		tepl_encoding_iconv_is_utf8		(const TeplEncodingIconv *enc);

G_GNUC_INTERNAL
gboolean		tepl_encoding_iconv_equals		(const TeplEncodingIconv *enc1,
								 const TeplEncodingIconv *enc2);

G_GNUC_INTERNAL
GSList *		tepl_encoding_iconv_get_all		(void);

G_GNUC_INTERNAL
GSList *		tepl_encoding_iconv_get_default_candidates (void);

G_END_DECLS

#endif /* TEPL_ENCODING_ICONV_H */
