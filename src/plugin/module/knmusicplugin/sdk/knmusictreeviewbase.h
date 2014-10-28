/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICTREEVIEWBASE_H
#define KNMUSICTREEVIEWBASE_H

#include "knmusicglobal.h"

#include <QTreeView>

using namespace KNMusic;

class QTimeLine;
class KNConnectionHandler;
class KNMusicModel;
class KNMusicProxyModel;
class KNMusicProxyModelPool;
class KNMusicTreeViewBase : public QTreeView
{
    Q_OBJECT
public:
    explicit KNMusicTreeViewBase(QWidget *parent = 0);
    KNMusicModel *musicModel();
    void setMusicModel(KNMusicModel *musicModel);
    void backupHeader();

signals:

public slots:
    virtual void resetHeaderState();

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void moveToFirst(const int &logicalIndex);

private slots:
    void onActionMouseInOut(const int &frame);
    void playIndex(const QModelIndex &index);
    void removeIndex(const QModelIndex &index);
    void removeSelections();

    void playCurrent();
    void removeCurrent();

private:
    void configureTimeLine(QTimeLine *timeLine);
    void showSoloMenu(QMouseEvent *event);
    void showMultiMenu(QMouseEvent *event);
    QTimeLine *m_mouseIn, *m_mouseOut;
    KNMusicProxyModel *m_proxyModel=nullptr;
    KNMusicProxyModelPool *m_proxyModelPool;
    KNConnectionHandler *m_soloConnections,
                        *m_multiConnections;
    QDrag *m_drag;
    QMimeData *m_mimeData;
    QByteArray m_headerState;
    QColor m_alternateColor=QColor(255,255,255,0),
    m_fontColor=QColor(255,255,255),
    m_buttonColor=QColor(255,255,255);
    int m_maxOpacity=0x30,
    m_fontBase=0x9f,
    m_buttonBase=0x10;
    bool m_pressed=false, m_initialLoad=true;
};

#endif // KNMUSICTREEVIEWBASE_H
