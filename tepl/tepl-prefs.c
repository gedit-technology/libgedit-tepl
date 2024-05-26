/* SPDX-FileCopyrightText: 2022-2024 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "config.h"
#include "tepl-prefs.h"
#include <glib/gi18n-lib.h>
#include <handy.h>
#include "tepl-settings.h"
#include "tepl-style-scheme-chooser-widget.h"
#include "tepl-utils.h"

/**
 * SECTION:prefs
 * @Short_description: Preferences widgets to configure a text editor
 * @Title: TeplPrefs
 *
 * Functions to create GUI elements that are useful for a preferences dialog or
 * panel. In order to configure a text editor.
 */

/* TODO: possibility to use the "dialog-question-symbolic" icon and set a
 * tooltip to it to provide more infos for an item.
 */

#define SETTING_SYSTEM_FONT_KEY "monospace-font-name"

static GtkWidget *
create_checkbutton_simple (GSettings   *settings,
			   const gchar *key,
			   const gchar *mnemonic_text)
{
	GtkWidget *checkbutton;

	g_return_val_if_fail (G_IS_SETTINGS (settings), NULL);
	g_return_val_if_fail (key != NULL, NULL);

	checkbutton = gtk_check_button_new_with_mnemonic (mnemonic_text);
	gtk_widget_show (checkbutton);

	g_settings_bind (settings, key,
			 checkbutton, "active",
			 G_SETTINGS_BIND_DEFAULT);

	return checkbutton;
}

static void
update_default_font_checkbutton_label (GtkCheckButton *checkbutton)
{
	TeplSettings *settings;
	GSettings *desktop_interface_settings;
	gchar *monospace_font_name;
	gchar *label;

	settings = tepl_settings_get_singleton ();
	desktop_interface_settings = tepl_settings_peek_desktop_interface_settings (settings);
	monospace_font_name = g_settings_get_string (desktop_interface_settings, SETTING_SYSTEM_FONT_KEY);

	label = g_strdup_printf (_("_Use the system fixed width font (%s)"), monospace_font_name);

	gtk_button_set_label (GTK_BUTTON (checkbutton), label);
	gtk_button_set_use_underline (GTK_BUTTON (checkbutton), TRUE);

	g_free (monospace_font_name);
	g_free (label);
}

static void
system_font_changed_cb (GSettings *desktop_interface_settings,
			gchar     *key,
			gpointer   user_data)
{
	GtkCheckButton *default_font_checkbutton = GTK_CHECK_BUTTON (user_data);
	update_default_font_checkbutton_label (default_font_checkbutton);
}

static GtkWidget *
create_default_font_checkbutton (void)
{
	GtkWidget *default_font_checkbutton;
	TeplSettings *settings;
	GSettings *desktop_interface_settings;

	default_font_checkbutton = gtk_check_button_new ();
	update_default_font_checkbutton_label (GTK_CHECK_BUTTON (default_font_checkbutton));

	settings = tepl_settings_get_singleton ();
	desktop_interface_settings = tepl_settings_peek_desktop_interface_settings (settings);

	g_signal_connect_object (desktop_interface_settings,
				 "changed::" SETTING_SYSTEM_FONT_KEY,
				 G_CALLBACK (system_font_changed_cb),
				 default_font_checkbutton,
				 0);

	return default_font_checkbutton;
}

/**
 * tepl_prefs_create_font_component:
 * @settings: a #GSettings.
 * @use_default_font_key: a key part of @settings to store whether to use the
 *   system font. The type of the key must be a boolean.
 * @editor_font_key: a key part of @settings to store the editor font. The type
 *   of the key must be a string.
 *
 * Returns: (transfer floating): A component for configuring the font.
 * Since: 6.2
 */
GtkWidget *
tepl_prefs_create_font_component (GSettings   *settings,
				  const gchar *use_default_font_key,
				  const gchar *editor_font_key)
{
	GtkWidget *default_font_checkbutton;
	GtkWidget *label;
	GtkWidget *font_button;
	GtkWidget *vgrid;
	GtkWidget *hgrid;

	g_return_val_if_fail (G_IS_SETTINGS (settings), NULL);
	g_return_val_if_fail (use_default_font_key != NULL, NULL);
	g_return_val_if_fail (editor_font_key != NULL, NULL);

	/* Widgets */
	default_font_checkbutton = create_default_font_checkbutton ();
	label = gtk_label_new_with_mnemonic (_("Editor _font:"));
	font_button = gtk_font_button_new ();
	gtk_label_set_mnemonic_widget (GTK_LABEL (label), font_button);

	/* GSettings */
	g_settings_bind (settings, use_default_font_key,
			 default_font_checkbutton, "active",
			 G_SETTINGS_BIND_DEFAULT);

	g_settings_bind (settings, editor_font_key,
			 font_button, "font",
			 G_SETTINGS_BIND_DEFAULT);

	/* Packing */
	vgrid = gtk_grid_new ();
	gtk_orientable_set_orientation (GTK_ORIENTABLE (vgrid), GTK_ORIENTATION_VERTICAL);
	gtk_grid_set_row_spacing (GTK_GRID (vgrid), 6);
	gtk_container_add (GTK_CONTAINER (vgrid), default_font_checkbutton);

	hgrid = gtk_grid_new ();
	gtk_grid_set_column_spacing (GTK_GRID (hgrid), 12);
	gtk_container_add (GTK_CONTAINER (hgrid), label);
	gtk_container_add (GTK_CONTAINER (hgrid), font_button);
	gtk_container_add (GTK_CONTAINER (vgrid), hgrid);

	/* Sensitivity */
	g_object_bind_property (default_font_checkbutton, "active",
				hgrid, "sensitive",
				G_BINDING_DEFAULT |
				G_BINDING_SYNC_CREATE |
				G_BINDING_INVERT_BOOLEAN);

	return tepl_utils_get_titled_component (_("Font"), vgrid);
}

/**
 * tepl_prefs_create_color_scheme_component:
 * @settings: a #GSettings.
 * @style_scheme_key: a key part of @settings to store the style scheme ID. The
 *   type of the key must be a string.
 *
 * This function creates a component containing a #TeplStyleSchemeChooserWidget.
 *
 * Returns: (transfer floating): A component for choosing the
 *   #GtkSourceStyleScheme to use.
 * Since: 6.2
 */
GtkWidget *
tepl_prefs_create_color_scheme_component (GSettings   *settings,
					  const gchar *style_scheme_key)
{
	TeplStyleSchemeChooserWidget *style_scheme_chooser;

	g_return_val_if_fail (G_IS_SETTINGS (settings), NULL);
	g_return_val_if_fail (style_scheme_key != NULL, NULL);

	style_scheme_chooser = tepl_style_scheme_chooser_widget_new ();

	g_settings_bind (settings, style_scheme_key,
			 style_scheme_chooser, "tepl-style-scheme-id",
			 G_SETTINGS_BIND_DEFAULT);

	return tepl_utils_get_titled_component (_("Color Scheme"),
						GTK_WIDGET (style_scheme_chooser));
}

/**
 * tepl_prefs_create_display_line_numbers_checkbutton:
 * @settings: a #GSettings.
 * @display_line_numbers_key: a key part of @settings. The type of the key must
 *   be a boolean.
 *
 * Returns: (transfer floating): A #GtkCheckButton intended for
 *   #GtkSourceView:show-line-numbers.
 * Since: 6.2
 */
GtkWidget *
tepl_prefs_create_display_line_numbers_checkbutton (GSettings   *settings,
						    const gchar *display_line_numbers_key)
{
	return create_checkbutton_simple (settings,
					  display_line_numbers_key,
					  _("_Display line numbers"));
}

/**
 * tepl_prefs_create_tab_width_spinbutton:
 * @settings: a #GSettings.
 * @tab_width_key: a key part of @settings. The type of the key must be an
 *   unsigned integer, with a range.
 *
 * Returns: (transfer floating): A widget containing a #GtkSpinButton intended
 *   for #GtkSourceView:tab-width.
 * Since: 6.2
 */
GtkWidget *
tepl_prefs_create_tab_width_spinbutton (GSettings   *settings,
					const gchar *tab_width_key)
{
	guint32 min = 0;
	guint32 max = 0;
	gboolean success;
	GtkWidget *label;
	GtkWidget *spinbutton;
	GtkWidget *hgrid;

	g_return_val_if_fail (G_IS_SETTINGS (settings), NULL);
	g_return_val_if_fail (tab_width_key != NULL, NULL);

	success = tepl_settings_get_range_uint (settings, tab_width_key, &min, &max);
	g_return_val_if_fail (success, NULL);

	label = gtk_label_new_with_mnemonic (_("_Tab width:"));
	spinbutton = gtk_spin_button_new_with_range (min, max, 1.0);
	gtk_label_set_mnemonic_widget (GTK_LABEL (label), spinbutton);

	g_settings_bind (settings, tab_width_key,
			 spinbutton, "value",
			 G_SETTINGS_BIND_DEFAULT);

	/* Packing */
	hgrid = gtk_grid_new ();
	gtk_grid_set_column_spacing (GTK_GRID (hgrid), 6);
	gtk_container_add (GTK_CONTAINER (hgrid), label);
	gtk_container_add (GTK_CONTAINER (hgrid), spinbutton);
	gtk_widget_show_all (hgrid);

	return hgrid;
}

/**
 * tepl_prefs_create_insert_spaces_component:
 * @settings: a #GSettings.
 * @insert_spaces_key: a key part of @settings. The type of the key must be a
 *   boolean.
 * @smart_backspace_key: a key part of @settings. The type of the key must be a
 *   boolean.
 *
 * Returns: (transfer floating): a component intended for
 *   #GtkSourceView:insert-spaces-instead-of-tabs and
 *   #GtkSourceView:smart-backspace.
 * Since: 6.2
 */
GtkWidget *
tepl_prefs_create_insert_spaces_component (GSettings   *settings,
					   const gchar *insert_spaces_key,
					   const gchar *smart_backspace_key)
{
	GtkWidget *insert_spaces_checkbutton;
	GtkWidget *smart_backspace_checkbutton;
	GtkWidget *vgrid;

	g_return_val_if_fail (G_IS_SETTINGS (settings), NULL);
	g_return_val_if_fail (insert_spaces_key != NULL, NULL);
	g_return_val_if_fail (smart_backspace_key != NULL, NULL);

	insert_spaces_checkbutton = create_checkbutton_simple (settings,
							       insert_spaces_key,
							       _("Insert _spaces instead of tabs"));

	smart_backspace_checkbutton = create_checkbutton_simple (settings,
								 smart_backspace_key,
								 _("_Forget you are not using tabulations"));
	gtk_widget_set_margin_start (smart_backspace_checkbutton, 12);

	g_object_bind_property (insert_spaces_checkbutton, "active",
				smart_backspace_checkbutton, "sensitive",
				G_BINDING_DEFAULT | G_BINDING_SYNC_CREATE);

	/* Packing */
	vgrid = gtk_grid_new ();
	gtk_orientable_set_orientation (GTK_ORIENTABLE (vgrid), GTK_ORIENTATION_VERTICAL);
	gtk_grid_set_row_spacing (GTK_GRID (vgrid), 7);
	gtk_container_add (GTK_CONTAINER (vgrid), insert_spaces_checkbutton);
	gtk_container_add (GTK_CONTAINER (vgrid), smart_backspace_checkbutton);
	gtk_widget_show_all (vgrid);

	return vgrid;
}

/**
 * tepl_prefs_create_highlighting_component:
 * @settings: a #GSettings.
 * @highlight_current_line_key: a key part of @settings. The type of the key
 *   must be a boolean.
 * @highlight_matching_brackets_key: a key part of @settings. The type of the
 *   key must be a boolean.
 *
 * Returns: (transfer floating): a component intended for
 *   #GtkSourceView:highlight-current-line and
 *   #GtkSourceBuffer:highlight-matching-brackets.
 * Since: 6.2
 */
GtkWidget *
tepl_prefs_create_highlighting_component (GSettings   *settings,
					  const gchar *highlight_current_line_key,
					  const gchar *highlight_matching_brackets_key)
{
	GtkWidget *vgrid;

	g_return_val_if_fail (G_IS_SETTINGS (settings), NULL);
	g_return_val_if_fail (highlight_current_line_key != NULL, NULL);
	g_return_val_if_fail (highlight_matching_brackets_key != NULL, NULL);

	vgrid = gtk_grid_new ();
	gtk_orientable_set_orientation (GTK_ORIENTABLE (vgrid), GTK_ORIENTATION_VERTICAL);
	gtk_grid_set_row_spacing (GTK_GRID (vgrid), 7);

	gtk_container_add (GTK_CONTAINER (vgrid),
			   create_checkbutton_simple (settings,
						      highlight_current_line_key,
						      _("Highlight current _line")));
	gtk_container_add (GTK_CONTAINER (vgrid),
			   create_checkbutton_simple (settings,
						      highlight_matching_brackets_key,
						      _("Highlight matching _brackets")));

	return tepl_utils_get_titled_component (_("Highlighting"), vgrid);
}

static GtkWidget *
create_auto_save_component (GSettings   *settings,
			    const gchar *auto_save_key,
			    const gchar *auto_save_interval)
{
	guint32 min = 0;
	guint32 max = 0;
	gboolean success;
	GtkWidget *checkbutton;
	GtkWidget *label;
	GtkWidget *spinbutton;
	GtkWidget *vgrid;
	GtkWidget *hgrid;

	/* Widgets */

	success = tepl_settings_get_range_uint (settings, auto_save_interval, &min, &max);
	g_return_val_if_fail (success, NULL);

	checkbutton = create_checkbutton_simple (settings, auto_save_key, _("_Autosave files"));

	label = gtk_label_new_with_mnemonic (_("Time in _minutes between each autosave:"));
	spinbutton = gtk_spin_button_new_with_range (min, max, 1.0);
	gtk_label_set_mnemonic_widget (GTK_LABEL (label), spinbutton);

	g_settings_bind (settings, auto_save_interval,
			 spinbutton, "value",
			 G_SETTINGS_BIND_DEFAULT);

	/* Packing */

	vgrid = gtk_grid_new ();
	gtk_orientable_set_orientation (GTK_ORIENTABLE (vgrid), GTK_ORIENTATION_VERTICAL);
	gtk_grid_set_row_spacing (GTK_GRID (vgrid), 7);
	gtk_container_add (GTK_CONTAINER (vgrid), checkbutton);

	hgrid = gtk_grid_new ();
	gtk_grid_set_column_spacing (GTK_GRID (hgrid), 6);
	gtk_widget_set_margin_start (hgrid, 12);
	gtk_container_add (GTK_CONTAINER (hgrid), label);
	gtk_container_add (GTK_CONTAINER (hgrid), spinbutton);
	gtk_container_add (GTK_CONTAINER (vgrid), hgrid);
	gtk_widget_show_all (vgrid);

	/* Sensitivity */

	g_object_bind_property (checkbutton, "active",
				hgrid, "sensitive",
				G_BINDING_DEFAULT | G_BINDING_SYNC_CREATE);

	return vgrid;
}

/**
 * tepl_prefs_create_files_component:
 * @settings: a #GSettings.
 * @create_backup_copy_key: a key part of @settings. The type of the key must be
 *   a boolean. Intended to be used with %TEPL_FILE_SAVER_FLAGS_CREATE_BACKUP
 *   (for example).
 * @auto_save_key: a key part of @settings. The type of the key must be a
 *   boolean. Whether to autosave files.
 * @auto_save_interval: a key part of @settings. The type of the key must be an
 *   unsigned integer, with a range. The interval is in minutes.
 *
 * Returns: (transfer floating): a component for some files preferences.
 * Since: 6.2
 */
GtkWidget *
tepl_prefs_create_files_component (GSettings   *settings,
				   const gchar *create_backup_copy_key,
				   const gchar *auto_save_key,
				   const gchar *auto_save_interval)
{
	GtkWidget *vgrid;

	g_return_val_if_fail (G_IS_SETTINGS (settings), NULL);
	g_return_val_if_fail (create_backup_copy_key != NULL, NULL);
	g_return_val_if_fail (auto_save_key != NULL, NULL);
	g_return_val_if_fail (auto_save_interval != NULL, NULL);

	vgrid = gtk_grid_new ();
	gtk_orientable_set_orientation (GTK_ORIENTABLE (vgrid), GTK_ORIENTATION_VERTICAL);
	gtk_grid_set_row_spacing (GTK_GRID (vgrid), 7);

	gtk_container_add (GTK_CONTAINER (vgrid),
			   create_checkbutton_simple (settings,
						      create_backup_copy_key,
						      _("_Create a backup copy of files before saving")));
	gtk_container_add (GTK_CONTAINER (vgrid),
			   create_auto_save_component (settings,
						       auto_save_key,
						       auto_save_interval));

	return tepl_utils_get_titled_component (_("Files"), vgrid);
}

/**
 * tepl_prefs_create_theme_variant_combo_box:
 * @settings: a #GSettings.
 * @theme_variant_key: a key part of @settings. Its type must be an enum for
 *   #TeplSettingsThemeVariant.
 *
 * Returns: (transfer floating): A widget containing a #GtkComboBoxText intended
 *   to choose a #TeplSettingsThemeVariant.
 * Since: 6.10
 */
GtkWidget *
tepl_prefs_create_theme_variant_combo_box (GSettings   *settings,
					   const gchar *theme_variant_key)
{
	GtkComboBoxText *combo_box_text;
	HdyStyleManager *style_manager;

	g_return_val_if_fail (G_IS_SETTINGS (settings), NULL);
	g_return_val_if_fail (theme_variant_key != NULL, NULL);

	combo_box_text = GTK_COMBO_BOX_TEXT (gtk_combo_box_text_new ());
	gtk_widget_show (GTK_WIDGET (combo_box_text));
	gtk_widget_set_halign (GTK_WIDGET (combo_box_text), GTK_ALIGN_START);

	style_manager = hdy_style_manager_get_default ();
	if (hdy_style_manager_get_system_supports_color_schemes (style_manager))
	{
		gtk_combo_box_text_append (combo_box_text, "system", _("Follow system style"));
	}

	gtk_combo_box_text_append (combo_box_text, "light", _("Light style"));
	gtk_combo_box_text_append (combo_box_text, "dark", _("Dark style"));

	g_settings_bind (settings, theme_variant_key,
			 combo_box_text, "active-id",
			 G_SETTINGS_BIND_DEFAULT);

	return tepl_utils_get_titled_component (_("Theme Variant"),
						GTK_WIDGET (combo_box_text));
}

/**
 * tepl_prefs_create_right_margin_component:
 * @settings: a #GSettings.
 * @display_right_margin_key: a key part of @settings. The type of the key must
 *   be a boolean.
 * @right_margin_position_key: a key part of @settings. The type of the key must
 *   be an unsigned integer.
 *
 * Returns: (transfer floating): a component intended for
 *   #GtkSourceView:show-right-margin and #GtkSourceView:right-margin-position.
 * Since: 6.10
 */
GtkWidget *
tepl_prefs_create_right_margin_component (GSettings   *settings,
					  const gchar *display_right_margin_key,
					  const gchar *right_margin_position_key)
{
	GtkWidget *check_button;
	GtkWidget *spin_button;
	GtkGrid *spin_button_wrapper;
	GtkGrid *hgrid;

	g_return_val_if_fail (G_IS_SETTINGS (settings), NULL);
	g_return_val_if_fail (display_right_margin_key != NULL, NULL);
	g_return_val_if_fail (right_margin_position_key != NULL, NULL);

	/* Check button */
	check_button = create_checkbutton_simple (settings,
						  display_right_margin_key,
						  _("Display right _margin at column:"));

	/* Spin button */
	spin_button = gtk_spin_button_new_with_range (1.0, 1000.0, 1.0);

	// Note that this creates a binding between the writability of the
	// GSettings key with the "sensitive" property of the spin_button.
	// Hence the need for spin_button_wrapper.
	g_settings_bind (settings, right_margin_position_key,
			 spin_button, "value",
			 G_SETTINGS_BIND_DEFAULT);

	/* Sensitivity */
	spin_button_wrapper = GTK_GRID (gtk_grid_new ());
	gtk_container_add (GTK_CONTAINER (spin_button_wrapper), spin_button);

	g_object_bind_property (check_button, "active",
				spin_button_wrapper, "sensitive",
				G_BINDING_DEFAULT | G_BINDING_SYNC_CREATE);

	/* Main packing */
	hgrid = GTK_GRID (gtk_grid_new ());
	gtk_grid_set_column_spacing (hgrid, 6);

	gtk_container_add (GTK_CONTAINER (hgrid), check_button);
	gtk_container_add (GTK_CONTAINER (hgrid), GTK_WIDGET (spin_button_wrapper));
	gtk_widget_show_all (GTK_WIDGET (hgrid));

	return GTK_WIDGET (hgrid);
}

/**
 * tepl_prefs_create_display_statusbar_checkbutton:
 * @settings: a #GSettings.
 * @display_statusbar_key: a key part of @settings. The type of the key must be
 *   a boolean.
 *
 * Returns: (transfer floating): A #GtkCheckButton intended for configuring the
 *   visibility of a #GtkStatusbar.
 * Since: 6.10
 */
GtkWidget *
tepl_prefs_create_display_statusbar_checkbutton (GSettings   *settings,
						 const gchar *display_statusbar_key)
{
	g_return_val_if_fail (G_IS_SETTINGS (settings), NULL);
	g_return_val_if_fail (display_statusbar_key != NULL, NULL);

	return create_checkbutton_simple (settings,
					  display_statusbar_key,
					  _("Display _statusbar"));
}

static gboolean
background_pattern_get_mapping (GValue   *to_value,
				GVariant *from_variant,
				gpointer  user_data)
{
	const gchar *enum_value_str;
	gboolean is_grid;

	g_return_val_if_fail (g_variant_is_of_type (from_variant, G_VARIANT_TYPE_STRING), FALSE);
	g_return_val_if_fail (G_VALUE_TYPE (to_value) == G_TYPE_BOOLEAN, FALSE);

	enum_value_str = g_variant_get_string (from_variant, NULL);
	is_grid = g_strcmp0 (enum_value_str, "grid") == 0;

	g_value_set_boolean (to_value, is_grid);

	return TRUE;
}

static GVariant *
background_pattern_set_mapping (const GValue       *from_value,
				const GVariantType *to_expected_type,
				gpointer            user_data)
{
	gboolean bool_value;

	g_return_val_if_fail (G_VALUE_TYPE (from_value) == G_TYPE_BOOLEAN, NULL);
	g_return_val_if_fail (g_variant_type_equal (to_expected_type, G_VARIANT_TYPE_STRING), NULL);

	bool_value = g_value_get_boolean (from_value);

	return g_variant_new_string (bool_value ? "grid" : "none");
}

/**
 * tepl_prefs_create_display_grid_checkbutton:
 * @settings: a #GSettings.
 * @background_pattern_key: a key part of @settings. Its type must be an enum
 *   for #GtkSourceBackgroundPatternType.
 *
 * Returns: (transfer floating): a #GtkCheckButton intended for
 *   %GTK_SOURCE_BACKGROUND_PATTERN_TYPE_GRID.
 * Since: 6.10
 */
GtkWidget *
tepl_prefs_create_display_grid_checkbutton (GSettings   *settings,
					    const gchar *background_pattern_key)
{
	GtkWidget *display_grid_checkbutton;

	g_return_val_if_fail (G_IS_SETTINGS (settings), NULL);
	g_return_val_if_fail (background_pattern_key != NULL, NULL);

	display_grid_checkbutton = gtk_check_button_new_with_mnemonic (_("Display _grid pattern"));
	gtk_widget_show (display_grid_checkbutton);

	g_settings_bind_with_mapping (settings, background_pattern_key,
				      display_grid_checkbutton, "active",
				      G_SETTINGS_BIND_DEFAULT,
				      background_pattern_get_mapping,
				      background_pattern_set_mapping,
				      NULL, NULL);

	return display_grid_checkbutton;
}
