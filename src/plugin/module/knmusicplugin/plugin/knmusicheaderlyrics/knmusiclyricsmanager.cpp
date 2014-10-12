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
#include <QFileInfo>

#include "knmusiclrcparser.h"

#include "knmusiclyricsmanager.h"

#include <QDebug>

KNMusicLyricsManager *KNMusicLyricsManager::m_instance=nullptr;

KNMusicLyricsManager *KNMusicLyricsManager::instance()
{
    return m_instance==nullptr?m_instance=new KNMusicLyricsManager:m_instance;
}

int KNMusicLyricsManager::lines() const
{
    return m_positions.size();
}

qint64 KNMusicLyricsManager::positionAt(const int &index) const
{
    return m_positions.at(index);
}

QString KNMusicLyricsManager::lyricsAt(const int &index) const
{
    return m_lyricsText.at(index);
}

void KNMusicLyricsManager::loadLyricsForFile(const QString &filePath)
{
    //Clear the lyrics.
    clear();
    //Find the lyrics.
    if(!findLyricsForFile(filePath))
    {
        return;
    }
    //Using parser to parse the file.
    m_lrcParser->parseFile(m_currentLyricsPath,
                           m_lyricsProperty,
                           m_positions,
                           m_lyricsText);
}

void KNMusicLyricsManager::clear()
{
    //Remove the old properties.
    m_lyricsProperty.clear();
    //Remove all the positions and the texts.
    m_positions.clear();
    m_lyricsText.clear();
}

bool KNMusicLyricsManager::findLyricsForFile(const QString &filePath)
{
    //Clear the old path.
    m_currentLyricsPath.clear();
    //Get the file info of the music.
    QFileInfo musicInfo(filePath);
    //Search the lyrics using the policy.
    int currentPolicy=0;
    while(currentPolicy<m_policyList.size())
    {
        switch (m_policyList.at(currentPolicy))
        {
        case SameNameInLyricsDir:
            if(checkLyricsFile(m_lyricsFolderPath + "/" +
                               musicInfo.completeBaseName()+".lrc"))
            {
                return true;
            }
            break;
        case SameNameInMusicDir:
            if(checkLyricsFile(musicInfo.absolutePath() + "/" +
                               musicInfo.completeBaseName()+".lrc"))
            {
                return true;
            }
            break;
        default:
            break;
        }
        currentPolicy++;
    }
    return false;
}

bool KNMusicLyricsManager::checkLyricsFile(const QString &lyricsPath)
{
    QFile testFile(lyricsPath);
    if(testFile.exists())
    {
        m_currentLyricsPath=QFileInfo(testFile).absoluteFilePath();
        return true;
    }
    return false;
}

KNMusicLyricsManager::KNMusicLyricsManager(QObject *parent) :
    QObject(parent)
{
    //Set the default loading policy.
    m_policyList.append(SameNameInLyricsDir);
    m_policyList.append(RelateNameInLyricsDir);
    m_policyList.append(SameNameInMusicDir);
    m_policyList.append(RelateNameInMusicDir);
    //Initial the LRC file parser.
    m_lrcParser=new KNMusicLRCParser(this);
}