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
#ifndef KNMUSICBASSGLOBAL_H
#define KNMUSICBASSGLOBAL_H

#include "bass.h"

#include <QObject>

class KNMusicBassGlobal : public QObject
{
    Q_OBJECT
public:
    static KNMusicBassGlobal *instance();
    static DWORD fdps();
    static void setFdps(const DWORD &fdps);

signals:

public slots:

private:
    static KNMusicBassGlobal *m_instance;
    static DWORD m_fdps;
    explicit KNMusicBassGlobal(QObject *parent = 0);
};

#endif // KNMUSICBASSGLOBAL_H
