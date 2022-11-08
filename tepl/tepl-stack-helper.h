/* SPDX-FileCopyrightText: 2022 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_STACK_HELPER_H
#define TEPL_STACK_HELPER_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define TEPL_TYPE_STACK_HELPER             (_tepl_stack_helper_get_type ())
#define TEPL_STACK_HELPER(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), TEPL_TYPE_STACK_HELPER, TeplStackHelper))
#define TEPL_STACK_HELPER_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), TEPL_TYPE_STACK_HELPER, TeplStackHelperClass))
#define TEPL_IS_STACK_HELPER(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TEPL_TYPE_STACK_HELPER))
#define TEPL_IS_STACK_HELPER_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), TEPL_TYPE_STACK_HELPER))
#define TEPL_STACK_HELPER_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), TEPL_TYPE_STACK_HELPER, TeplStackHelperClass))

typedef struct _TeplStackHelper         TeplStackHelper;
typedef struct _TeplStackHelperClass    TeplStackHelperClass;
typedef struct _TeplStackHelperPrivate  TeplStackHelperPrivate;

struct _TeplStackHelper
{
	GObject parent;

	TeplStackHelperPrivate *priv;
};

struct _TeplStackHelperClass
{
	GObjectClass parent_class;
};

GType			_tepl_stack_helper_get_type	(void);

TeplStackHelper *	_tepl_stack_helper_new		(GtkContainer *container);

G_END_DECLS

#endif /* TEPL_STACK_HELPER_H */
