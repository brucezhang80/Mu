/*
 * Copyright (C) Kreogist Dev Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include "knmusicplaylistlistdelegate.h"

#include "knmusicplaylistlistview.h"

KNMusicPlaylistListView::KNMusicPlaylistListView(QWidget *parent) :
    KNMusicCategoryListViewBase(parent)
{
    //Enabled drag and drop.
    enabledDragDrop();
    //Set properties.
    setIconSize(QSize(40, 40));
    setDragDropMode(QAbstractItemView::DropOnly);
    //Initial the delegate.
    setItemDelegate(new KNMusicPlaylistListDelegate(this));
}

void KNMusicPlaylistListView::showEvent(QShowEvent *event)
{
    KNMusicCategoryListViewBase::showEvent(event);
    //Check if we have load playlist before.
    if(m_noPlaylistList)
    {
        //Remove the flag.
        m_noPlaylistList=false;
        //Emit loading signal.
        emit requireLoadPlaylistList();
    }
}
