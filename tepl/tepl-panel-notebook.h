/* SPDX-FileCopyrightText: 2024 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_PANEL_NOTEBOOK_H
#define TEPL_PANEL_NOTEBOOK_H

#if !defined (TEPL_H_INSIDE) && !defined (TEPL_COMPILATION)
#error "Only <tepl/tepl.h> can be included directly."
#endif

#include <tepl/tepl-panel-simple.h>

G_BEGIN_DECLS

#define TEPL_TYPE_PANEL_NOTEBOOK             (tepl_panel_notebook_get_type ())
#define TEPL_PANEL_NOTEBOOK(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), TEPL_TYPE_PANEL_NOTEBOOK, TeplPanelNotebook))
#define TEPL_PANEL_NOTEBOOK_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), TEPL_TYPE_PANEL_NOTEBOOK, TeplPanelNotebookClass))
#define TEPL_IS_PANEL_NOTEBOOK(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TEPL_TYPE_PANEL_NOTEBOOK))
#define TEPL_IS_PANEL_NOTEBOOK_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), TEPL_TYPE_PANEL_NOTEBOOK))
#define TEPL_PANEL_NOTEBOOK_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), TEPL_TYPE_PANEL_NOTEBOOK, TeplPanelNotebookClass))

typedef struct _TeplPanelNotebook         TeplPanelNotebook;
typedef struct _TeplPanelNotebookClass    TeplPanelNotebookClass;
typedef struct _TeplPanelNotebookPrivate  TeplPanelNotebookPrivate;

struct _TeplPanelNotebook
{
	GObject parent;

	TeplPanelNotebookPrivate *priv;
};

struct _TeplPanelNotebookClass
{
	GObjectClass parent_class;

	gpointer padding[12];
};

G_MODULE_EXPORT
GType			tepl_panel_notebook_get_type	(void);

G_MODULE_EXPORT
TeplPanelNotebook *	tepl_panel_notebook_new		(TeplPanelSimple *panel_simple,
							 GtkNotebook     *notebook);

G_END_DECLS

#endif /* TEPL_PANEL_NOTEBOOK_H */
