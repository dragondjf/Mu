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
#ifndef KNMUSICLIBRARYCATEGORYTAB_H
#define KNMUSICLIBRARYCATEGORYTAB_H

#include "knmusiclibrarytab.h"

class KNEmptyStateWidget;
class KNMusicLibraryEmptyHint;
class KNMusicCategoryModel;
class KNMusicCategoryProxyModel;
class KNMusicLibraryCategoryTab : public KNMusicLibraryTab
{
    Q_OBJECT
public:
    explicit KNMusicLibraryCategoryTab(QObject *parent = 0);
    QWidget *widget();

signals:

public slots:
    virtual void setCategoryModel(KNMusicCategoryModel *model);
    void setLibraryModel(KNMusicLibraryModel *model);

protected:
    KNMusicCategoryProxyModel *proxyCategoryModel();
    void setContentWidget(QWidget *widget);
    QWidget *viewerWidget();

private:
    KNMusicCategoryProxyModel *m_proxyCategoryModel;
    KNEmptyStateWidget *m_viewer;
    KNMusicLibraryEmptyHint *m_emptyHint;
};

#endif // KNMUSICLIBRARYCATEGORYTAB_H
