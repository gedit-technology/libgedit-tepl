/*
 * This file is part of Tepl, a text editor library.
 *
 * Copyright 2020 - Sébastien Wilmet <swilmet@gnome.org>
 *
 * Tepl is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation; either version 2.1 of the License, or (at your
 * option) any later version.
 *
 * Tepl is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#include "tepl-metadata-manager.h"
#include "tepl-metadata-attic.h"
#include "tepl-metadata-parser.h"
#include "tepl-utils.h"

/**
 * SECTION:metadata-manager
 * @Title: TeplMetadataManager
 * @Short_description: To store file metadata on disk
 *
 * #TeplMetadataManager permits to store file metadata on disk. It serves both
 * as an easier replacement for GVfs metadata, and to have metadata support on
 * platforms that don't support GVfs metadata.
 */

/* This code is a modified version of GeditMetadataManager, coming from gedit:
 *
 * Copyright 2003-2007 - Paolo Maggi
 *
 * The XML format is the same. TeplMetadataManager can read a file generated by
 * GeditMetadataManager; but the reverse is maybe not true, it hasn't been
 * tested.
 *
 * A better implementation would be to use a database, so that several processes
 * can read and write to it at the same time, to be able to share metadata
 * between apps.
 */

struct _TeplMetadataManagerPrivate
{
	/* Keys: GFile *
	 * Values: TeplMetadataAttic *
	 * Never NULL.
	 */
	GHashTable *hash_table;

	guint modified : 1;
};

/* TeplMetadataManager is a singleton. */
static TeplMetadataManager *singleton = NULL;

#define DEFAULT_MAX_NUMBER_OF_LOCATIONS (1000)

G_DEFINE_TYPE_WITH_PRIVATE (TeplMetadataManager, tepl_metadata_manager, G_TYPE_OBJECT)

static void
tepl_metadata_manager_finalize (GObject *object)
{
	TeplMetadataManager *manager = TEPL_METADATA_MANAGER (object);

	if (singleton == manager)
	{
		singleton = NULL;
	}

	g_hash_table_unref (manager->priv->hash_table);

	G_OBJECT_CLASS (tepl_metadata_manager_parent_class)->finalize (object);
}

static void
tepl_metadata_manager_class_init (TeplMetadataManagerClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = tepl_metadata_manager_finalize;
}

static void
tepl_metadata_manager_init (TeplMetadataManager *manager)
{
	manager->priv = tepl_metadata_manager_get_instance_private (manager);

	manager->priv->hash_table = g_hash_table_new_full (g_file_hash,
							   (GEqualFunc) g_file_equal,
							   g_object_unref,
							   g_object_unref);
}

/**
 * tepl_metadata_manager_get_singleton:
 *
 * Returns: (transfer none): the #TeplMetadataManager singleton instance.
 * Since: 5.0
 */
TeplMetadataManager *
tepl_metadata_manager_get_singleton (void)
{
	if (singleton == NULL)
	{
		singleton = g_object_new (TEPL_TYPE_METADATA_MANAGER, NULL);
	}

	return singleton;
}

void
_tepl_metadata_manager_unref_singleton (void)
{
	if (singleton != NULL)
	{
		g_object_unref (singleton);
	}

	/* singleton is not set to NULL here, it is set to NULL in
	 * tepl_metadata_manager_finalize() (i.e. when we are sure that the ref
	 * count reaches 0).
	 */
}

/**
 * tepl_metadata_manager_trim:
 * @manager: the #TeplMetadataManager.
 * @max_number_of_locations: the maximum size, or -1 for the default value.
 *
 * The purpose of having a maximum size is to avoid that the file on disk grows
 * indefinitely.
 *
 * @max_number_of_locations is the maximum number of #GFile locations for which
 * metadata are kept. This function discards the least recently accessed
 * metadata if needed.
 *
 * If @max_number_of_locations is -1, a default internal value is used that
 * should fit most applications' needs.
 *
 * Since: 5.0
 */
void
tepl_metadata_manager_trim (TeplMetadataManager *manager,
			    gint                 max_number_of_locations)
{
	guint my_max_number_of_locations;

	g_return_if_fail (TEPL_IS_METADATA_MANAGER (manager));
	g_return_if_fail (max_number_of_locations >= -1);

	if (max_number_of_locations == -1)
	{
		my_max_number_of_locations = DEFAULT_MAX_NUMBER_OF_LOCATIONS;
	}
	else
	{
		my_max_number_of_locations = max_number_of_locations;
	}

	while (g_hash_table_size (manager->priv->hash_table) > my_max_number_of_locations)
	{
		GHashTableIter iter;
		gpointer key;
		gpointer value;
		GFile *oldest_location = NULL;
		TeplMetadataAttic *oldest_metadata_attic = NULL;

		g_hash_table_iter_init (&iter, manager->priv->hash_table);
		while (g_hash_table_iter_next (&iter, &key, &value))
		{
			GFile *location = key;
			TeplMetadataAttic *metadata_attic = value;

			if (oldest_location == NULL ||
			    _tepl_metadata_attic_compare_atime (metadata_attic, oldest_metadata_attic) < 0)
			{
				oldest_location = location;
				oldest_metadata_attic = metadata_attic;
			}
		}

		g_hash_table_remove (manager->priv->hash_table, oldest_location);
		manager->priv->modified = TRUE;
	}
}

/**
 * tepl_metadata_manager_load_from_disk:
 * @manager: the #TeplMetadataManager.
 * @from_file: the #GFile to load metadata from.
 * @error: location to a %NULL #GError, or %NULL.
 *
 * Loads synchronously all the metadata from @from_file into @manager.
 *
 * A good moment to call this function is on application startup, see the
 * #GApplication::startup signal.
 *
 * Returns: whether the operation was successful.
 * Since: 5.0
 */
gboolean
tepl_metadata_manager_load_from_disk (TeplMetadataManager  *manager,
				      GFile                *from_file,
				      GError              **error)
{
	g_return_val_if_fail (TEPL_IS_METADATA_MANAGER (manager), FALSE);
	g_return_val_if_fail (G_IS_FILE (from_file), FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	return _tepl_metadata_parser_read_file (from_file,
						manager->priv->hash_table,
						error);
}

static GBytes *
to_string (TeplMetadataManager *manager)
{
	GString *string;
	GHashTableIter iter;
	gpointer key;
	gpointer value;

	string = g_string_new (NULL);
	g_string_append (string, "<metadata>\n");

	g_hash_table_iter_init (&iter, manager->priv->hash_table);
	while (g_hash_table_iter_next (&iter, &key, &value))
	{
		GFile *location = key;
		TeplMetadataAttic *metadata_attic = value;

		_tepl_metadata_attic_append_xml_to_string (metadata_attic, location, string);
	}

	g_string_append (string, "</metadata>\n");

	return g_string_free_to_bytes (string);
}

/**
 * tepl_metadata_manager_save_to_disk:
 * @manager: the #TeplMetadataManager.
 * @to_file: the #GFile to save metadata to.
 * @trim: if %TRUE, tepl_metadata_manager_trim() is called with -1.
 * @error: location to a %NULL #GError, or %NULL.
 *
 * Saves synchronously all the metadata from @manager to @to_file. The parent
 * directories of @to_file are created if needed.
 *
 * A good moment to call this function is on application shutdown, see the
 * #GApplication::shutdown signal.
 *
 * Returns: whether the operation was successful.
 * Since: 5.0
 */
gboolean
tepl_metadata_manager_save_to_disk (TeplMetadataManager  *manager,
				    GFile                *to_file,
				    gboolean              trim,
				    GError              **error)
{
	GBytes *bytes;
	gboolean ok;

	g_return_val_if_fail (TEPL_IS_METADATA_MANAGER (manager), FALSE);
	g_return_val_if_fail (G_IS_FILE (to_file), FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	if (trim)
	{
		tepl_metadata_manager_trim (manager, -1);
	}

	if (!manager->priv->modified)
	{
		return TRUE;
	}

	if (!tepl_utils_create_parent_directories (to_file, NULL, error))
	{
		return FALSE;
	}

	bytes = to_string (manager);

	ok = g_file_replace_contents (to_file,
				      g_bytes_get_data (bytes, NULL),
				      g_bytes_get_size (bytes),
				      NULL,
				      FALSE,
				      G_FILE_CREATE_NONE,
				      NULL,
				      NULL,
				      error);

	if (ok)
	{
		manager->priv->modified = FALSE;
	}

	g_bytes_unref (bytes);
	return ok;
}

/**
 * tepl_metadata_manager_copy_from:
 * @from_manager: the #TeplMetadataManager.
 * @for_location: a #GFile.
 * @to_metadata: a #TeplMetadata.
 *
 * Copies the metadata stored in @from_manager for @for_location into
 * @to_metadata.
 *
 * Since: 5.0
 */
void
tepl_metadata_manager_copy_from (TeplMetadataManager *from_manager,
				 GFile               *for_location,
				 TeplMetadata        *to_metadata)
{
	TeplMetadataAttic *from_metadata_attic;

	g_return_if_fail (TEPL_IS_METADATA_MANAGER (from_manager));
	g_return_if_fail (G_IS_FILE (for_location));
	g_return_if_fail (TEPL_IS_METADATA (to_metadata));

	from_metadata_attic = g_hash_table_lookup (from_manager->priv->hash_table, for_location);

	if (from_metadata_attic != NULL)
	{
		_tepl_metadata_attic_copy_from (from_metadata_attic, to_metadata);
	}
}

/**
 * tepl_metadata_manager_merge_into:
 * @into_manager: the #TeplMetadataManager.
 * @for_location: a #GFile.
 * @from_metadata: a #TeplMetadata.
 *
 * Merges the metadata from @from_metadata into @into_manager for @for_location.
 *
 * Since: 5.0
 */
void
tepl_metadata_manager_merge_into (TeplMetadataManager *into_manager,
				  GFile               *for_location,
				  TeplMetadata        *from_metadata)
{
	TeplMetadataAttic *into_metadata_attic;

	g_return_if_fail (TEPL_IS_METADATA_MANAGER (into_manager));
	g_return_if_fail (G_IS_FILE (for_location));
	g_return_if_fail (TEPL_IS_METADATA (from_metadata));

	into_metadata_attic = g_hash_table_lookup (into_manager->priv->hash_table, for_location);

	if (into_metadata_attic == NULL)
	{
		into_metadata_attic = _tepl_metadata_attic_new ();

		g_hash_table_replace (into_manager->priv->hash_table,
				      g_object_ref (for_location),
				      into_metadata_attic);
	}

	_tepl_metadata_attic_merge_into (into_metadata_attic, from_metadata);

	into_manager->priv->modified = TRUE;
}
