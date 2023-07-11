/* SPDX-FileCopyrightText: 2020 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_PANEL1_H
#define TEPL_PANEL1_H

#if !defined (TEPL_H_INSIDE) && !defined (TEPL_COMPILATION)
#error "Only <tepl/tepl.h> can be included directly."
#endif

#include <gtk/gtk.h>
#include <tepl/tepl-macros.h>

G_BEGIN_DECLS

#define TEPL_TYPE_PANEL1             (tepl_panel1_get_type ())
#define TEPL_PANEL1(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), TEPL_TYPE_PANEL1, TeplPanel1))
#define TEPL_PANEL1_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), TEPL_TYPE_PANEL1, TeplPanel1Class))
#define TEPL_IS_PANEL1(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TEPL_TYPE_PANEL1))
#define TEPL_IS_PANEL1_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), TEPL_TYPE_PANEL1))
#define TEPL_PANEL1_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), TEPL_TYPE_PANEL1, TeplPanel1Class))

typedef struct _TeplPanel1         TeplPanel1;
typedef struct _TeplPanel1Class    TeplPanel1Class;
typedef struct _TeplPanel1Private  TeplPanel1Private;

struct _TeplPanel1
{
	GtkGrid parent;

	TeplPanel1Private *priv;
};

struct _TeplPanel1Class
{
	GtkGridClass parent_class;

	gpointer padding[12];
};

_TEPL_EXTERN
GType		tepl_panel1_get_type				(void);

_TEPL_EXTERN
TeplPanel1 *	tepl_panel1_new					(void);

_TEPL_EXTERN
TeplPanel1 *	tepl_panel1_new_for_left_side_panel		(void);

_TEPL_EXTERN
GtkStack *	tepl_panel1_get_stack				(TeplPanel1 *panel);

_TEPL_EXTERN
void		tepl_panel1_add_component			(TeplPanel1   *panel,
								 GtkWidget   *component,
								 const gchar *name,
								 const gchar *title,
								 const gchar *icon_name);

_TEPL_EXTERN
void		tepl_panel1_provide_active_component_gsetting	(TeplPanel1   *panel,
								 GSettings   *settings,
								 const gchar *setting_key);

_TEPL_EXTERN
void		tepl_panel1_restore_state_from_gsettings	(TeplPanel1 *panel);

_TEPL_EXTERN
void		tepl_panel1_save_state_to_gsettings 		(TeplPanel1 *panel);

G_END_DECLS

#endif /* TEPL_PANEL1_H */
