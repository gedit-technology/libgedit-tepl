/* SPDX-FileCopyrightText: 2024 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_STYLE_SCHEME_CHOOSER_SIMPLE_H
#define TEPL_STYLE_SCHEME_CHOOSER_SIMPLE_H

#if !defined (TEPL_H_INSIDE) && !defined (TEPL_COMPILATION)
#error "Only <tepl/tepl.h> can be included directly."
#endif

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define TEPL_TYPE_STYLE_SCHEME_CHOOSER_SIMPLE             (tepl_style_scheme_chooser_simple_get_type ())
#define TEPL_STYLE_SCHEME_CHOOSER_SIMPLE(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), TEPL_TYPE_STYLE_SCHEME_CHOOSER_SIMPLE, TeplStyleSchemeChooserSimple))
#define TEPL_STYLE_SCHEME_CHOOSER_SIMPLE_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), TEPL_TYPE_STYLE_SCHEME_CHOOSER_SIMPLE, TeplStyleSchemeChooserSimpleClass))
#define TEPL_IS_STYLE_SCHEME_CHOOSER_SIMPLE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TEPL_TYPE_STYLE_SCHEME_CHOOSER_SIMPLE))
#define TEPL_IS_STYLE_SCHEME_CHOOSER_SIMPLE_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), TEPL_TYPE_STYLE_SCHEME_CHOOSER_SIMPLE))
#define TEPL_STYLE_SCHEME_CHOOSER_SIMPLE_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), TEPL_TYPE_STYLE_SCHEME_CHOOSER_SIMPLE, TeplStyleSchemeChooserSimpleClass))

typedef struct _TeplStyleSchemeChooserSimple         TeplStyleSchemeChooserSimple;
typedef struct _TeplStyleSchemeChooserSimpleClass    TeplStyleSchemeChooserSimpleClass;
typedef struct _TeplStyleSchemeChooserSimplePrivate  TeplStyleSchemeChooserSimplePrivate;

struct _TeplStyleSchemeChooserSimple
{
	GtkBin parent;

	TeplStyleSchemeChooserSimplePrivate *priv;
};

struct _TeplStyleSchemeChooserSimpleClass
{
	GtkBinClass parent_class;

	gpointer padding[12];
};

G_MODULE_EXPORT
GType		tepl_style_scheme_chooser_simple_get_type		(void);

G_MODULE_EXPORT
TeplStyleSchemeChooserSimple *
		tepl_style_scheme_chooser_simple_new			(gboolean for_dark_theme_variant);

G_MODULE_EXPORT
const gchar *	tepl_style_scheme_chooser_simple_get_style_scheme_id	(TeplStyleSchemeChooserSimple *chooser);

G_MODULE_EXPORT
void		tepl_style_scheme_chooser_simple_set_style_scheme_id	(TeplStyleSchemeChooserSimple *chooser,
									 const gchar                  *style_scheme_id);

G_END_DECLS

#endif /* TEPL_STYLE_SCHEME_CHOOSER_SIMPLE_H */
