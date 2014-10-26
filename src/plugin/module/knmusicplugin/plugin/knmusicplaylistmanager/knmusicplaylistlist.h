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
#ifndef KNMUSICPLAYLISTLIST_H
#define KNMUSICPLAYLISTLIST_H

#include <QJsonArray>

#include <QStandardItemModel>

class KNMusicPlaylistModel;
class KNMusicPlaylistListItem;
class KNMusicPlaylistList : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit KNMusicPlaylistList(QObject *parent = 0);
    void loadDataFile();
    KNMusicPlaylistModel *playlistModel(const int &row);
    KNMusicPlaylistListItem *playlistItem(const int &row);
    KNMusicPlaylistListItem *playlistItemFromIndex(const QModelIndex &index);

signals:

public slots:

private slots:
    void onActionRowInserted(const QModelIndex &parent, int first, int last);
    void onActionRowRemoved(const QModelIndex &parent, int first, int last);

private:
    QJsonArray m_playlistListData;
};

#endif // KNMUSICPLAYLISTLIST_H