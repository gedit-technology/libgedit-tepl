/* SPDX-FileCopyrightText: 2023 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "tepl-object-counters.h"

/**
 * SECTION:object-counters
 * @Title: TeplObjectCounters
 * @Short_description: Utility to detect memory leaks
 *
 * TeplObjectCounters is a utility to detect memory leaks, by counting the
 * number of objects constructed and destructed, per type.
 *
 * It must be used manually by calling its functions explicitly.
 *
 * It is not intended to be used in performance-sensitive code areas.
 */

/* Keys: owned gchar* (the type as a string).
 * Values: owned gint* (pointer to the counter).
 */
static GHashTable *hash_table;

/**
 * tepl_object_counters_init:
 *
 * Must be called before using the other TeplObjectCounters functions, usually
 * early in the `main()` function.
 *
 * Since: 6.10
 */
void
tepl_object_counters_init (void)
{
	if (hash_table == NULL)
	{
		hash_table = g_hash_table_new_full (g_str_hash,
						    g_str_equal,
						    g_free,
						    g_free);
	}
}

/**
 * tepl_object_counters_increment:
 * @type: the object type, as a string.
 *
 * Increments the counter for @type.
 *
 * For a #GObject class, this function must be called in the `_init()` function,
 * when an instance is created.
 *
 * Since: 6.10
 */
void
tepl_object_counters_increment (const gchar *type)
{
	gint *counter;

	g_return_if_fail (type != NULL);
	g_return_if_fail (hash_table != NULL);

	counter = g_hash_table_lookup (hash_table, type);

	if (counter == NULL)
	{
		counter = g_malloc (sizeof (gint));
		*counter = 1;

		g_hash_table_insert (hash_table,
				     g_strdup (type),
				     counter);
	}
	else
	{
		*counter = *counter + 1;
	}
}

/**
 * tepl_object_counters_decrement:
 * @type: the object type, as a string.
 *
 * Decrements the counter for @type.
 *
 * For a #GObject class, this function must be called in the `_finalize()`
 * destructor function.
 *
 * Since: 6.10
 */
void
tepl_object_counters_decrement (const gchar *type)
{
	gint *counter;

	g_return_if_fail (type != NULL);
	g_return_if_fail (hash_table != NULL);

	counter = g_hash_table_lookup (hash_table, type);
	g_return_if_fail (counter != NULL);
	g_return_if_fail (*counter > 0);

	*counter = *counter - 1;
}

static void
final_check (gpointer key,
	     gpointer value,
	     gpointer user_data)
{
	const gchar *type = key;
	const gint *counter = value;

	g_return_if_fail (type != NULL);
	g_return_if_fail (counter != NULL);

	if (*counter != 0)
	{
		g_printerr ("%s: %d remaining instance(s).\n", type, *counter);
	}
}

/**
 * tepl_object_counters_final_check:
 *
 * Prints a warning for each type for which the counter hasn't reached zero.
 *
 * This function is intended to be called once, towards the end of the `main()`
 * function.
 *
 * After calling this function, the other TeplObjectCounters functions must not
 * be called anymore.
 *
 * Since: 6.10
 */
void
tepl_object_counters_final_check (void)
{
	g_return_if_fail (hash_table != NULL);

	g_hash_table_foreach (hash_table,
			      final_check,
			      NULL);

	g_hash_table_unref (hash_table);
	hash_table = NULL;
}
