/* SPDX-FileCopyrightText: 2024 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_PANEL_SIMPLE_H
#define TEPL_PANEL_SIMPLE_H

#if !defined (TEPL_H_INSIDE) && !defined (TEPL_COMPILATION)
#error "Only <tepl/tepl.h> can be included directly."
#endif

#include <tepl/tepl-panel-item.h>

G_BEGIN_DECLS

#define TEPL_TYPE_PANEL_SIMPLE             (tepl_panel_simple_get_type ())
#define TEPL_PANEL_SIMPLE(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), TEPL_TYPE_PANEL_SIMPLE, TeplPanelSimple))
#define TEPL_PANEL_SIMPLE_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), TEPL_TYPE_PANEL_SIMPLE, TeplPanelSimpleClass))
#define TEPL_IS_PANEL_SIMPLE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TEPL_TYPE_PANEL_SIMPLE))
#define TEPL_IS_PANEL_SIMPLE_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), TEPL_TYPE_PANEL_SIMPLE))
#define TEPL_PANEL_SIMPLE_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), TEPL_TYPE_PANEL_SIMPLE, TeplPanelSimpleClass))

typedef struct _TeplPanelSimple         TeplPanelSimple;
typedef struct _TeplPanelSimpleClass    TeplPanelSimpleClass;
typedef struct _TeplPanelSimplePrivate  TeplPanelSimplePrivate;

struct _TeplPanelSimple
{
	GObject parent;

	TeplPanelSimplePrivate *priv;
};

struct _TeplPanelSimpleClass
{
	GObjectClass parent_class;

	/* Signals */

	void	(*add_item)		(TeplPanelSimple *panel,
					 TeplPanelItem   *item);

	void	(*remove_item)		(TeplPanelSimple *panel,
					 TeplPanelItem   *item);

	void	(*changed)		(TeplPanelSimple *panel);

	gpointer padding[12];
};

G_MODULE_EXPORT
GType			tepl_panel_simple_get_type		(void);

G_MODULE_EXPORT
TeplPanelSimple *	tepl_panel_simple_new			(void);

G_MODULE_EXPORT
GList *			tepl_panel_simple_get_items		(TeplPanelSimple *panel);

G_MODULE_EXPORT
TeplPanelItem *		tepl_panel_simple_get_active_item	(TeplPanelSimple *panel);

G_MODULE_EXPORT
const gchar *		tepl_panel_simple_get_active_item_name	(TeplPanelSimple *panel);

G_MODULE_EXPORT
void			tepl_panel_simple_set_active_item_name	(TeplPanelSimple *panel,
								 const gchar     *name);

G_MODULE_EXPORT
gboolean		tepl_panel_simple_has_several_items	(TeplPanelSimple *panel);

G_END_DECLS

#endif /* TEPL_PANEL_SIMPLE_H */
