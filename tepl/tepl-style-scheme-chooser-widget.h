/* SPDX-FileCopyrightText: 2020 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_STYLE_SCHEME_CHOOSER_WIDGET_H
#define TEPL_STYLE_SCHEME_CHOOSER_WIDGET_H

#if !defined (TEPL_H_INSIDE) && !defined (TEPL_COMPILATION)
#error "Only <tepl/tepl.h> can be included directly."
#endif

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define TEPL_TYPE_STYLE_SCHEME_CHOOSER_WIDGET             (tepl_style_scheme_chooser_widget_get_type ())
#define TEPL_STYLE_SCHEME_CHOOSER_WIDGET(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), TEPL_TYPE_STYLE_SCHEME_CHOOSER_WIDGET, TeplStyleSchemeChooserWidget))
#define TEPL_STYLE_SCHEME_CHOOSER_WIDGET_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), TEPL_TYPE_STYLE_SCHEME_CHOOSER_WIDGET, TeplStyleSchemeChooserWidgetClass))
#define TEPL_IS_STYLE_SCHEME_CHOOSER_WIDGET(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TEPL_TYPE_STYLE_SCHEME_CHOOSER_WIDGET))
#define TEPL_IS_STYLE_SCHEME_CHOOSER_WIDGET_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), TEPL_TYPE_STYLE_SCHEME_CHOOSER_WIDGET))
#define TEPL_STYLE_SCHEME_CHOOSER_WIDGET_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), TEPL_TYPE_STYLE_SCHEME_CHOOSER_WIDGET, TeplStyleSchemeChooserWidgetClass))

typedef struct _TeplStyleSchemeChooserWidget         TeplStyleSchemeChooserWidget;
typedef struct _TeplStyleSchemeChooserWidgetClass    TeplStyleSchemeChooserWidgetClass;
typedef struct _TeplStyleSchemeChooserWidgetPrivate  TeplStyleSchemeChooserWidgetPrivate;

struct _TeplStyleSchemeChooserWidget
{
	GtkBin parent;

	TeplStyleSchemeChooserWidgetPrivate *priv;
};

struct _TeplStyleSchemeChooserWidgetClass
{
	GtkBinClass parent_class;

	gpointer padding[12];
};

G_MODULE_EXPORT
GType		tepl_style_scheme_chooser_widget_get_type	(void);

G_MODULE_EXPORT
TeplStyleSchemeChooserWidget *
		tepl_style_scheme_chooser_widget_new		(gboolean theme_variants);

G_END_DECLS

#endif /* TEPL_STYLE_SCHEME_CHOOSER_WIDGET_H */
