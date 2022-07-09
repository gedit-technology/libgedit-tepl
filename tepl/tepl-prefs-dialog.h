/* SPDX-FileCopyrightText: 2022 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_PREFS_DIALOG_H
#define TEPL_PREFS_DIALOG_H

#if !defined (TEPL_H_INSIDE) && !defined (TEPL_COMPILATION)
#error "Only <tepl/tepl.h> can be included directly."
#endif

#include <gtk/gtk.h>
#include <tepl/tepl-macros.h>

G_BEGIN_DECLS

#define TEPL_TYPE_PREFS_DIALOG             (tepl_prefs_dialog_get_type ())
#define TEPL_PREFS_DIALOG(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), TEPL_TYPE_PREFS_DIALOG, TeplPrefsDialog))
#define TEPL_PREFS_DIALOG_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), TEPL_TYPE_PREFS_DIALOG, TeplPrefsDialogClass))
#define TEPL_IS_PREFS_DIALOG(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TEPL_TYPE_PREFS_DIALOG))
#define TEPL_IS_PREFS_DIALOG_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), TEPL_TYPE_PREFS_DIALOG))
#define TEPL_PREFS_DIALOG_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), TEPL_TYPE_PREFS_DIALOG, TeplPrefsDialogClass))

typedef struct _TeplPrefsDialog         TeplPrefsDialog;
typedef struct _TeplPrefsDialogClass    TeplPrefsDialogClass;
typedef struct _TeplPrefsDialogPrivate  TeplPrefsDialogPrivate;

struct _TeplPrefsDialog
{
	GtkDialog parent;

	TeplPrefsDialogPrivate *priv;
};

struct _TeplPrefsDialogClass
{
	GtkDialogClass parent_class;

	gpointer padding[12];
};

_TEPL_EXTERN
GType			tepl_prefs_dialog_get_type		(void);

_TEPL_EXTERN
TeplPrefsDialog *	tepl_prefs_dialog_get_singleton		(void);

G_GNUC_INTERNAL
void			_tepl_prefs_dialog_unref_singleton	(void);

_TEPL_EXTERN
void			tepl_prefs_dialog_show_for_parent	(TeplPrefsDialog *dialog,
								 GtkWindow       *parent_window);

G_END_DECLS

#endif /* TEPL_PREFS_DIALOG_H */
