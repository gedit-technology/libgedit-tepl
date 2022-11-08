/* SPDX-FileCopyrightText: 2022 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "tepl-encoding.h"

const gchar *
tepl_encoding_get_category_name (const TeplEncoding *enc)
{
	g_return_val_if_fail (enc != NULL, NULL);
	return enc->vtable->get_category_name (enc);
}
