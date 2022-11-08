/* SPDX-FileCopyrightText: 2022 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "tepl-encoding.h"

/**
 * tepl_encoding_get_name:
 * @enc: a #TeplEncoding.
 *
 * Gets the name of the #TeplEncoding, such as "UTF-8".
 *
 * Returns: the name of the #TeplEncoding.
 * Since: 6.4
 */
const gchar *
tepl_encoding_get_name (const TeplEncoding *enc)
{
	g_return_val_if_fail (enc != NULL, NULL);
	return enc->vtable->get_name (enc);
}

/**
 * tepl_encoding_get_category_name:
 * @enc: a #TeplEncoding.
 *
 * #TeplEncoding's are grouped into several categories, such as "Unicode" or
 * "Western".
 *
 * This function returns the category name of the #TeplEncoding, or %NULL if the
 * category is unknown.
 *
 * The returned string is localized (already translated).
 *
 * Returns: (nullable): the category name of the #TeplEncoding, or %NULL.
 * Since: 6.4
 */
const gchar *
tepl_encoding_get_category_name (const TeplEncoding *enc)
{
	g_return_val_if_fail (enc != NULL, NULL);
	return enc->vtable->get_category_name (enc);
}
