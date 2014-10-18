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
#include <QTimeLine>
#include <QPaintEvent>
#include <QPainter>

#include "knmusicheaderplayerbase.h"
#include "knmusiclyricsmanager.h"

#include "knmusicheaderlyrics.h"

#include <QDebug>

KNMusicHeaderLyrics::KNMusicHeaderLyrics(QWidget *parent) :
    KNMusicHeaderLyricsBase(parent)
{
    //Initial the lyrics manager.
    m_lyricsManager=KNMusicLyricsManager::instance();

    //Initial the lyrics moving time line.
    m_moveToCurrent=new QTimeLine(m_animationDuration, this);
    m_moveToCurrent->setUpdateInterval(2);
    m_moveToCurrent->setEasingCurve(QEasingCurve::OutCubic);
    m_moveToCurrent->setEndFrame(0);
    connect(m_moveToCurrent, &QTimeLine::frameChanged,
            this, &KNMusicHeaderLyrics::onActionLyricsMoved);
}

KNMusicHeaderLyrics::~KNMusicHeaderLyrics()
{
    delete m_lyricsManager;
}

void KNMusicHeaderLyrics::setHeaderPlayer(KNMusicHeaderPlayerBase *player)
{
    connect(player, &KNMusicHeaderPlayerBase::requireLoadLyrics,
            this, &KNMusicHeaderLyricsBase::loadLyricsForMusic);
    connect(player, &KNMusicHeaderPlayerBase::positionChanged,
            this, &KNMusicHeaderLyrics::onActionPositionChange);
}

void KNMusicHeaderLyrics::reset()
{
    //Get the lyrics lines.
    m_lyricsLines=m_lyricsManager->lines();
    m_noLyrics=(m_lyricsLines==0);
    //Check is there lyrics.
    if(m_noLyrics)
    {
        //Reset the lyrics line.
        m_currentLyricsLine=-1;
    }
    else
    {
        //Initial the current line to the first line.
        m_currentLyricsLine=0;
        //Move the first line to center.
        onActionLyricsMoved(0);
    }
    qDebug()<<"m_currentLyricsLine="<<m_currentLyricsLine;
}

void KNMusicHeaderLyrics::loadLyricsForMusic(const QString &filePath)
{
    //Load the lyrics.
    m_lyricsManager->loadLyricsForFile(filePath);
    //Reset the lyrics viewer.
    reset();
}

void KNMusicHeaderLyrics::onActionPositionChange(const qint64 &position)
{
    //If no lyrics, do nothing.
    if(m_noLyrics)
    {
        return;
    }
    int yOffset=0;
    //Check the position is previous than the current yet.
    if(position<m_lyricsManager->positionAt(m_currentLyricsLine))
    {
        //Find the matching lyrics.
        while(m_lyricsManager->positionAt(m_currentLyricsLine)>position)
        {
            yOffset-=lyricsSize(m_lyricsManager->lyricsAt(m_currentLyricsLine)).height();
            m_currentLyricsLine--;
        }
        update();
        //Start animation.
        startMovingAnime((lyricsLineDuration(m_currentLyricsLine)>>2),
                         yOffset);
        return;
    }
    //Find the next specific position.
    while(m_currentLyricsLine<m_lyricsLines-1 &&
          m_lyricsManager->positionAt(m_currentLyricsLine+1)<position)
    {
        yOffset+=lyricsSize(m_lyricsManager->lyricsAt(m_currentLyricsLine)).height();
        m_currentLyricsLine++;
    }
    if(yOffset>0)
    {
        startMovingAnime((lyricsLineDuration(m_currentLyricsLine)>>2),
                         yOffset);
    }
}

void KNMusicHeaderLyrics::paintEvent(QPaintEvent *event)
{
    //Paint other things.
    QWidget::paintEvent(event);
    //Check is current line available, if not means no lyrics.
    if(m_noLyrics || m_currentLyricsLine==-1)
    {
        return;
    }
    //Initial the painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform,
                           true);
    QSize currentSize;
    QString currentText;
    int centerY=(height()>>1)+m_currentLineOffsetY;
    //Draw the current line.
    painter.setPen(m_highlightColor);
    currentText=m_lyricsManager->lyricsAt(m_currentLyricsLine);
    currentSize=lyricsSize(currentText);
    centerY-=(currentSize.height()>>1);
    painter.drawText(m_leftSpacing,
                     centerY,
                     width(),
                     currentSize.height(),
                     Qt::AlignLeft,
                     currentText);
    //Draw other lyrics.
    painter.setPen(m_normalText);
    //Draw down lines.
    int lineTop=centerY+currentSize.height(),
        paintLine=m_currentLyricsLine+1;
    while(lineTop<height() && paintLine<m_lyricsLines)
    {
        //Draw the line.
        currentText=m_lyricsManager->lyricsAt(paintLine);
        currentSize=lyricsSize(currentText);
        painter.drawText(m_leftSpacing,
                         lineTop,
                         width(),
                         currentSize.height(),
                         Qt::AlignLeft,
                         currentText);
        //To the next line.
        paintLine++;
        lineTop+=currentSize.height();
    }
    //Draw up lines.
    int lineBottom=centerY;
    paintLine=m_currentLyricsLine-1;
    while(lineBottom>0 && paintLine>0)
    {
        //Draw the line.
        currentText=m_lyricsManager->lyricsAt(paintLine);
        currentSize=lyricsSize(currentText);
        //MAGIC: the line bottom is current line's top, so calculate here.
        lineBottom-=currentSize.height();
        painter.drawText(m_leftSpacing,
                         lineBottom,
                         width(),
                         currentSize.height(),
                         Qt::AlignLeft,
                         currentText);
        //To the previous line.
        paintLine--;
    }
}

void KNMusicHeaderLyrics::onActionLyricsMoved(const int &frame)
{
    //Update the offset.
    m_currentLineOffsetY=frame;
    //Redraw the display.
    update();
}

int KNMusicHeaderLyrics::lyricsLineDuration(const int &index)
{
    if(index==-1)
    {
        return 0;
    }
    if(index<m_lyricsLines-1)
    {
        return m_lyricsManager->positionAt(index+1)-
                m_lyricsManager->positionAt(index);
    }
    return m_animationDuration<<2;
}

void KNMusicHeaderLyrics::startMovingAnime(const int &durationOffset,
                                           const int &yOffset)
{
    //Stop the time line.
    m_moveToCurrent->stop();
    //Reset the duration and the start frame.
    m_moveToCurrent->setDuration(qMin(durationOffset, m_animationDuration));
    m_moveToCurrent->setStartFrame(yOffset);
    //Before we start, set the widget to the start frame state.
    onActionLyricsMoved(yOffset);
    //Start the animation.
    m_moveToCurrent->start();
}
