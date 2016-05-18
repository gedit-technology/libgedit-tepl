/*
 * This file is part of Gtef, a text editor library.
 *
 * Copyright 2016 - Sébastien Wilmet <swilmet@gnome.org>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#include "gtef-tab.h"

/**
 * SECTION:tab
 * @Short_description: Contains a GtefView with GtkInfoBars on top
 * @Title: GtefTab
 */

typedef struct _GtefTabPrivate GtefTabPrivate;

struct _GtefTabPrivate
{
	gint something;
};

G_DEFINE_TYPE_WITH_PRIVATE (GtefTab, gtef_tab, GTK_TYPE_GRID)

static void
gtef_tab_dispose (GObject *object)
{

	G_OBJECT_CLASS (gtef_tab_parent_class)->dispose (object);
}

static void
gtef_tab_class_init (GtefTabClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->dispose = gtef_tab_dispose;
}

static void
gtef_tab_init (GtefTab *tab)
{
}
