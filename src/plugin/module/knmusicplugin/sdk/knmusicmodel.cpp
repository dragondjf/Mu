/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QMimeData>

#include "knglobal.h"
#include "knmusicsearcher.h"
#include "knmusicanalysiscache.h"
#include "knmusicanalysisextend.h"
#include "knmusicratingdelegate.h"

#include "knmusicmodel.h"

#include <QDebug>

KNMusicModel::KNMusicModel(QObject *parent) :
    QStandardItemModel(parent)
{
    //Initial music global.
    m_musicGlobal=KNMusicGlobal::instance();
    //Linked the signal.
    connect(m_musicGlobal, &KNMusicGlobal::musicFilePathChanged,
            this, &KNMusicModel::onActionFileNameChanged);

    //Initial file searcher.
    m_searcher=new KNMusicSearcher;
    m_searcher->moveToThread(m_musicGlobal->searchThread());
    connect(this, &KNMusicModel::requireAnalysisFiles,
            m_searcher, &KNMusicSearcher::analysisUrls);

    //Initial analysis cache.
    m_analysisCache=new KNMusicAnalysisCache;
    m_analysisCache->moveToThread(m_musicGlobal->analysisThread());
    connect(m_searcher, &KNMusicSearcher::fileFound,
            m_analysisCache, &KNMusicAnalysisCache::appendFilePath);
    connect(m_analysisCache, &KNMusicAnalysisCache::requireAppendRow,
            this, &KNMusicModel::appendMusicRow);

    //Initial a default analysis extend.
    setAnalysisExtend(new KNMusicAnalysisExtend);
}

KNMusicModel::~KNMusicModel()
{
    //Delete objects.
    delete m_searcher;
    delete m_analysisCache;
    delete m_analysisExtend;
}

Qt::DropActions KNMusicModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

Qt::ItemFlags KNMusicModel::flags(const QModelIndex &index) const
{
    //Enabled drag and drop flags when the index is valid.
    if(index.isValid())
    {
        return Qt::ItemIsDragEnabled |
                Qt::ItemIsEnabled |
                Qt::ItemNeverHasChildren |
                Qt::ItemIsSelectable |
                (index.column()==Rating || index.column()==AlbumRating?
                     Qt::ItemIsEditable:Qt::NoItemFlags);
    }
    //Or else, just enabled drop.
    return Qt::ItemIsDropEnabled;
}

QStringList KNMusicModel::mimeTypes() const
{
    //Add url list to mimetypes, but I don't know why should add uri.
    //14.08.21: Add music model row foramt for music row.
    QStringList types=QStandardItemModel::mimeTypes();
    types<<"text/uri-list"
         <<KNMusicGlobal::musicRowFormat();
    return types;
}

bool KNMusicModel::dropMimeData(const QMimeData *data,
                                Qt::DropAction action,
                                int row,
                                int column,
                                const QModelIndex &parent)
{
    //When mimedata contains url data, and ensure that move&copy action enabled.
    if((action==Qt::MoveAction || action==Qt::CopyAction))
    {
        if(data->hasUrls())
        {
            addFiles(KNGlobal::urlToPathList(data->urls()));
            return true;
        }
    }
    return QStandardItemModel::dropMimeData(data, action, row, column, parent);
}

qint64 KNMusicModel::totalDuration() const
{
    return m_totalDuration;
}

KNMusicDetailInfo KNMusicModel::detailInfoFromRow(const int &row)
{
    KNMusicDetailInfo detailInfo;
    //Copy the text first.
    for(int i=0; i<MusicDataCount; i++)
    {
        detailInfo.textLists[i]=itemText(row, i);
    }
    //Copy the properties.
    detailInfo.fileName=rowProperty(row, FileNameRole).toString();
    detailInfo.filePath=rowProperty(row, FilePathRole).toString();
    detailInfo.trackFilePath=rowProperty(row, TrackFileRole).toString();
    detailInfo.coverImageHash=rowProperty(row, ArtworkKeyRole).toString();
    detailInfo.startPosition=rowProperty(row, StartPositionRole).toLongLong();
    detailInfo.size=roleData(row, Size, Qt::UserRole).toLongLong();
    detailInfo.dateModified=roleData(row, DateModified, Qt::UserRole).toDateTime();
    detailInfo.dateAdded=roleData(row, DateAdded, Qt::UserRole).toDateTime();
    detailInfo.lastPlayed=roleData(row, LastPlayed, Qt::UserRole).toDateTime();
    detailInfo.duration=roleData(row, Time, Qt::UserRole).toLongLong();
    detailInfo.bitRate=roleData(row, BitRate, Qt::UserRole).toLongLong();
    detailInfo.samplingRate=roleData(row, SampleRate, Qt::UserRole).toLongLong();
    detailInfo.rating=roleData(row, Size, Qt::DisplayRole).toInt();
    //Return the detail info.
    return detailInfo;
}

QPixmap KNMusicModel::songAlbumArt(const int &row)
{
    Q_UNUSED(row)
    //We don't store the album art data for default, models should find their
    //ways to store this thing. This function is only a port.
    return QPixmap();
}

qint64 KNMusicModel::songDuration(const int &row)
{
    Q_ASSERT(row>-1 && row<rowCount());
    //For easy access.
    return roleData(row, Time, Qt::UserRole).toLongLong();
}

int KNMusicModel::playingItemColumn()
{
    return Name;
}

void KNMusicModel::addFiles(const QStringList &fileList)
{
    emit requireAnalysisFiles(fileList);
}

void KNMusicModel::appendMusicRow(const QList<QStandardItem *> &musicRow)
{
    //Clear all the icons.
    musicRow.at(Name)->setData(QPixmap(), Qt::DecorationRole);
    //Calculate new total duration.
    m_totalDuration+=musicRow.at(Time)->data(Qt::UserRole).toInt();
    //Append this row.
    appendRow(musicRow);
    emit rowCountChanged();
}

void KNMusicModel::updateMusicRow(const int &row,
                                  const KNMusicDetailInfo &detailInfo)
{
    //Update the text of the row.
    for(int i=0; i<MusicDataCount; i++)
    {
        switch(i)
        {
        case Rating:
        case AlbumRating:
        case DateAdded:
        case Plays:
            break;
        default:
            setItemText(row, i, detailInfo.textLists[i]);
        }
    }
    //Update the properties.
    setRowProperty(row, FilePathRole, detailInfo.filePath);
    setRowProperty(row, FileNameRole, detailInfo.fileName);
    setRowProperty(row, TrackFileRole, detailInfo.trackFilePath);
    setRowProperty(row, StartPositionRole, detailInfo.startPosition);
    setRoleData(row, Size, Qt::UserRole, detailInfo.size);
    setRoleData(row, DateModified, Qt::UserRole, detailInfo.dateModified);
    setRoleData(row, DateAdded, Qt::UserRole, detailInfo.dateAdded);
    setRoleData(row, LastPlayed, Qt::UserRole, detailInfo.lastPlayed);
    setRoleData(row, Time, Qt::UserRole, detailInfo.duration);
    setRoleData(row, BitRate, Qt::UserRole, detailInfo.bitRate);
    setRoleData(row, SampleRate, Qt::UserRole, detailInfo.samplingRate);
}

void KNMusicModel::removeMusicRow(const int &row)
{
    //We need to do sth before remove a row.
    m_totalDuration-=data(index(row, Time), Qt::UserRole).toInt();
    //Remove that row.
    removeRow(row);
    //Tell other's to update.
    emit rowCountChanged();
}

void KNMusicModel::clearMusicRow()
{
    //Clear the duration.
    m_totalDuration=0;
    //Remove all the rows.
    removeRows(0, rowCount());
    //Tell other's to update.
    emit rowCountChanged();
}

void KNMusicModel::blockAddFile(const QString &filePath)
{
    //WARNING: This function is working in a block way to adding file, may cause
    //performance reduce.
    if(m_searcher->isFilePathAccept(filePath))
    {
        m_analysisCache->analysisFile(filePath);
    }
}

void KNMusicModel::setHeaderSortFlag()
{
    setHeaderData(Time, Qt::Horizontal, SortUserByInt, Qt::UserRole);
    setHeaderData(DiscNumber, Qt::Horizontal, SortByInt, Qt::UserRole);
    setHeaderData(DiscCount, Qt::Horizontal, SortByInt, Qt::UserRole);
    setHeaderData(TrackNumber, Qt::Horizontal, SortByInt, Qt::UserRole);
    setHeaderData(TrackCount, Qt::Horizontal, SortByInt, Qt::UserRole);
    setHeaderData(Size, Qt::Horizontal, SortUserByInt, Qt::UserRole);
    setHeaderData(BitRate, Qt::Horizontal, SortUserByFloat, Qt::UserRole);
    setHeaderData(DateAdded, Qt::Horizontal, SortUserByDate, Qt::UserRole);
    setHeaderData(DateModified, Qt::Horizontal, SortUserByDate, Qt::UserRole);
    setHeaderData(LastPlayed, Qt::Horizontal, SortUserByDate, Qt::UserRole);
}

KNMusicAnalysisExtend *KNMusicModel::analysisExtend() const
{
    return m_analysisExtend;
}

void KNMusicModel::setAnalysisExtend(KNMusicAnalysisExtend *analysisExtend)
{
    if(m_analysisExtend!=nullptr)
    {
        //Disconnect the extended.
        disconnect(m_analysisExtend, &KNMusicAnalysisExtend::requireAppendRow,
                   this, &KNMusicModel::appendMusicRow);
        //Clear the extend in analysis cache.
        m_analysisCache->setExtend(nullptr);
        //Recover the memory.
        delete m_analysisExtend;
    }
    //Save the pointer.
    m_analysisExtend=analysisExtend;
    //Move to the analysis thread.
    m_analysisExtend->moveToThread(m_musicGlobal->analysisThread());
    //Install the extend.
    m_analysisCache->setExtend(m_analysisExtend);
    //Establish connections.
    if(m_analysisExtend!=nullptr)
    {
        connect(m_analysisExtend, &KNMusicAnalysisExtend::requireAppendRow,
                this, &KNMusicModel::appendMusicRow);
    }
}

void KNMusicModel::onActionFileNameChanged(const QString &originalPath,
                                           const QString &currentPath,
                                           const QString &currentFileName)
{
    //Search all the path.
    QModelIndexList originalPathList=match(index(0,0),
                                           FilePathRole,
                                           originalPath,
                                           -1,
                                           Qt::MatchFixedString);
    //Change all the pathes and file names.
    while(!originalPathList.isEmpty())
    {
        //Get the row.
        int currentRow=originalPathList.takeLast().row();
        //Set the new data.
        setRowProperty(currentRow, FilePathRole, currentPath);
        setRowProperty(currentRow, FileNameRole, currentFileName);
    }
}
