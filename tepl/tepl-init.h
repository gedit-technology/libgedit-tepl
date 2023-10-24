/* SPDX-FileCopyrightText: 2017 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_INIT_H
#define TEPL_INIT_H

#if !defined (TEPL_H_INSIDE) && !defined (TEPL_COMPILATION)
#error "Only <tepl/tepl.h> can be included directly."
#endif

#include <gmodule.h>

G_BEGIN_DECLS

G_MODULE_EXPORT
void	tepl_init		(void);

G_MODULE_EXPORT
void	tepl_finalize		(void);

G_END_DECLS

#endif /* TEPL_INIT_H */
