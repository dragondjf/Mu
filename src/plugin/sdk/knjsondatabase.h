/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNJSONDATABASE_H
#define KNJSONDATABASE_H

#include <QFileInfo>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include <QObject>

class QFile;
class KNJSONDatabase : public QObject
{
    Q_OBJECT
public:
    explicit KNJSONDatabase(QObject *parent = 0);
    void setDatabaseFile(const QString &filePath);
    void read();
    void write();

signals:

public slots:

protected:
    void append(QJsonObject value);
    void replace(int i, QJsonObject value);
    void removeAt(int i);
    QJsonValue at(int i);
    QJsonArray::iterator begin();
    QJsonArray::iterator end();

private:
    inline void addBatchCount();
    inline void checkDatabaseDir();
    QFile *m_databaseFile;
    QFileInfo m_databaseFileInfo;
    QJsonArray m_dataField;
    QJsonDocument m_document;
    QJsonObject m_contentObject;
    QJsonParseError m_lastError;
    static int m_majorVersion;
    static int m_minorVersion;
    int m_batchCount=0;
};

#endif // KNJSONDATABASE_H
