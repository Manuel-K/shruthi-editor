// Shruthi-Editor: An unofficial Editor for the Shruthi hardware synthesizer. For
// informations about the Shruthi, see <http://www.mutable-instruments.net/shruthi1>.
//
// Copyright (C) 2011-2015 Manuel Krönig
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.


#include "fileio.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>


bool FileIO::loadFromDisk(const QString &location, Message &data) {
#ifdef DEBUGMSGS
    qDebug() << "Patch::loadFromDisk(" << location << ")";
#endif
    QFile file(location);

    if (!file.exists()) {
        qDebug() << "The file does not exist.";
        return false;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open.";
        return false;
    }

    QByteArray tmp = file.read(MAX_READ_SIZE);

    file.close();

#ifdef DEBUGMSGS
    qDebug() << "Read" << tmp.length() << "bytes.";
#endif

    appendToCharVector(tmp, data);
    return true;
}


void FileIO::appendToCharVector(const QByteArray &byteArray, Message &data) {
    const unsigned int &len = byteArray.length();
    for (unsigned int i = 0; i < len; i++) {
        data.push_back(byteArray.at(i));
    }
}


void FileIO::appendToByteArray(const Message &data, QByteArray &byteArray) {
    const unsigned int &len = data.size();
    for (unsigned int i = 0; i < len; i++) {
        byteArray.append(data.at(i));
    }
}


void FileIO::appendToByteArray(const unsigned char *data, const unsigned int &amount, QByteArray &byteArray) {
    for (unsigned int i = 0; i < amount; i++) {
        byteArray.append(data[i]);
    }
}


bool FileIO::saveToDisk(const QString &location, const QByteArray &data) {
#ifdef DEBUGMSGS
    qDebug() << "FileIO::saveToDisk(" << location << ")";
#endif
    QFile file(location);

    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Could not open file for saving.";
        return false;
    }

    bool status = file.write(data) == data.length();

    file.close();
    return status;
}
