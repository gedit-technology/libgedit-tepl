/* SPDX-FileCopyrightText: 2024 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_PANEL_SWITCHER_NOTEBOOK_H
#define TEPL_PANEL_SWITCHER_NOTEBOOK_H

#if !defined (TEPL_H_INSIDE) && !defined (TEPL_COMPILATION)
#error "Only <tepl/tepl.h> can be included directly."
#endif

#include <tepl/tepl-panel-container.h>

G_BEGIN_DECLS

#define TEPL_TYPE_PANEL_SWITCHER_NOTEBOOK             (tepl_panel_switcher_notebook_get_type ())
#define TEPL_PANEL_SWITCHER_NOTEBOOK(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), TEPL_TYPE_PANEL_SWITCHER_NOTEBOOK, TeplPanelSwitcherNotebook))
#define TEPL_PANEL_SWITCHER_NOTEBOOK_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), TEPL_TYPE_PANEL_SWITCHER_NOTEBOOK, TeplPanelSwitcherNotebookClass))
#define TEPL_IS_PANEL_SWITCHER_NOTEBOOK(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TEPL_TYPE_PANEL_SWITCHER_NOTEBOOK))
#define TEPL_IS_PANEL_SWITCHER_NOTEBOOK_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), TEPL_TYPE_PANEL_SWITCHER_NOTEBOOK))
#define TEPL_PANEL_SWITCHER_NOTEBOOK_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), TEPL_TYPE_PANEL_SWITCHER_NOTEBOOK, TeplPanelSwitcherNotebookClass))

typedef struct _TeplPanelSwitcherNotebook         TeplPanelSwitcherNotebook;
typedef struct _TeplPanelSwitcherNotebookClass    TeplPanelSwitcherNotebookClass;
typedef struct _TeplPanelSwitcherNotebookPrivate  TeplPanelSwitcherNotebookPrivate;

struct _TeplPanelSwitcherNotebook
{
	GtkBin parent;

	TeplPanelSwitcherNotebookPrivate *priv;
};

struct _TeplPanelSwitcherNotebookClass
{
	GtkBinClass parent_class;

	gpointer padding[12];
};

G_MODULE_EXPORT
GType				tepl_panel_switcher_notebook_get_type	(void);

G_MODULE_EXPORT
TeplPanelSwitcherNotebook *	tepl_panel_switcher_notebook_new	(TeplPanelContainer *panel,
									 GtkNotebook        *notebook);

G_END_DECLS

#endif /* TEPL_PANEL_SWITCHER_NOTEBOOK_H */
