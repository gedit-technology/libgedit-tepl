/* SPDX-FileCopyrightText: 2020-2024 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_STYLE_SCHEME_CHOOSER_FULL_H
#define TEPL_STYLE_SCHEME_CHOOSER_FULL_H

#if !defined (TEPL_H_INSIDE) && !defined (TEPL_COMPILATION)
#error "Only <tepl/tepl.h> can be included directly."
#endif

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define TEPL_TYPE_STYLE_SCHEME_CHOOSER_FULL             (tepl_style_scheme_chooser_full_get_type ())
#define TEPL_STYLE_SCHEME_CHOOSER_FULL(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), TEPL_TYPE_STYLE_SCHEME_CHOOSER_FULL, TeplStyleSchemeChooserFull))
#define TEPL_STYLE_SCHEME_CHOOSER_FULL_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), TEPL_TYPE_STYLE_SCHEME_CHOOSER_FULL, TeplStyleSchemeChooserFullClass))
#define TEPL_IS_STYLE_SCHEME_CHOOSER_FULL(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TEPL_TYPE_STYLE_SCHEME_CHOOSER_FULL))
#define TEPL_IS_STYLE_SCHEME_CHOOSER_FULL_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), TEPL_TYPE_STYLE_SCHEME_CHOOSER_FULL))
#define TEPL_STYLE_SCHEME_CHOOSER_FULL_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), TEPL_TYPE_STYLE_SCHEME_CHOOSER_FULL, TeplStyleSchemeChooserFullClass))

typedef struct _TeplStyleSchemeChooserFull         TeplStyleSchemeChooserFull;
typedef struct _TeplStyleSchemeChooserFullClass    TeplStyleSchemeChooserFullClass;
typedef struct _TeplStyleSchemeChooserFullPrivate  TeplStyleSchemeChooserFullPrivate;

struct _TeplStyleSchemeChooserFull
{
	GtkBin parent;

	TeplStyleSchemeChooserFullPrivate *priv;
};

struct _TeplStyleSchemeChooserFullClass
{
	GtkBinClass parent_class;

	gpointer padding[12];
};

G_MODULE_EXPORT
GType		tepl_style_scheme_chooser_full_get_type	(void);

G_MODULE_EXPORT
TeplStyleSchemeChooserFull *
		tepl_style_scheme_chooser_full_new	(void);

G_END_DECLS

#endif /* TEPL_STYLE_SCHEME_CHOOSER_FULL_H */
