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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KNMUSICALBUMLISTDELEGATE_H
#define KNMUSICALBUMLISTDELEGATE_H

#include <QStyledItemDelegate>

/*!
 * \brief The KNMusicAlbumListDelegate class is the item delegate which is used
 * in the album list view. It will simply shows the data of a song instead of
 * using a complex tree view.
 */
class KNMusicAlbumListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicAlbumListDelegate object.
     * \param parent The parent object pointer.
     */
    explicit KNMusicAlbumListDelegate(QWidget *parent = 0);

    /*!
     * \brief Reimplemented from QStyledItemDelegate::paint().
     */
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QStyledItemDelegate::sizeHint().
     */
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const Q_DECL_OVERRIDE;
};

#endif // KNMUSICALBUMLISTDELEGATE_H
