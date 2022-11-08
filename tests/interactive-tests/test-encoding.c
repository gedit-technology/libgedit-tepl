/* SPDX-FileCopyrightText: 2022 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <tepl/tepl.h>
#include "tepl/tepl-encoding-iconv.h"
#include <stdlib.h>

int
main (int    argc,
      char **argv)
{
	GSList *list;
	GSList *l;

	tepl_init ();
	gtk_init (&argc, &argv);

	list = tepl_encoding_iconv_get_all ();
	for (l = list; l != NULL; l = l->next)
	{
		const TeplEncodingIconv *encoding_iconv = l->data;
		const TeplEncoding *encoding = tepl_encoding_iconv_to_base_type (encoding_iconv);

		g_print ("Category: '%s' ; Name: '%s'\n",
			 tepl_encoding_get_category_name (encoding),
			 tepl_encoding_iconv_get_charset (encoding_iconv));
	}
	g_slist_free_full (list, (GDestroyNotify) tepl_encoding_iconv_free);

	tepl_finalize ();
	return EXIT_SUCCESS;
}
