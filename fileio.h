// Shruthi-Editor: An unofficial Editor for the Shruthi hardware synthesizer. For
// informations about the Shruthi, see <http://www.mutable-instruments.net/shruthi1>.
//
// Copyright (C) 2011-2018 Manuel Krönig
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


#ifndef SHRUTHI_FILEIO_H
#define SHRUTHI_FILEIO_H


#include "message.h"
class QByteArray;
class QString;


class FileIO {
    public:
        static const int MAX_READ_SIZE = 1048576; // 1MB
        static bool loadFromDisk(const QString &location, Message &data);
        static void appendToCharVector(const QByteArray &byteArray, Message &data);
        static void appendToByteArray(const Message &data, QByteArray &byteArray);
        static void appendToByteArray(const unsigned char *data, const unsigned int &amount, QByteArray &byteArray);
        static bool saveToDisk(const QString &location, const QByteArray &data);
};


#endif // SHRUTHI_FILEIO_H
