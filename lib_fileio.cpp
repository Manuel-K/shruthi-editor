#include "lib_fileio.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

/*
// ******************************************
bool FileIO::loadFromDisk(const QString &location) {
// ******************************************
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

    char tmp[195];
    unsigned int readBytes = file.read(tmp, 195);
    file.close();

#ifdef DEBUGMSGS
    qDebug() << "Read" << readBytes << "bytes.";
#endif

    // primitive check if patch is valid:
    if (readBytes == 195) {
#ifdef DEBUGMSGS
        qDebug() << "Detected full patch sysex.";
#endif
        Message sysex;
        sysex.reserve(195);
        for (unsigned int i = 0; i < 195; i++) {
            sysex.push_back(tmp[i]);
        }
        return parseSysex(&sysex);
    } else if (readBytes == 92) {
#ifdef DEBUGMSGS
        qDebug() << "Detected light patch files.";
#endif
        unsigned char sysex[92];
        for (unsigned int i=0; i<readBytes; i++) {
            sysex[i] = (char) tmp[i];
    #ifdef DEBUGMSGS
            qDebug() << i << ":" << sysex[i];
    #endif
        }
        return unpackData(sysex);
    } else {
        return false;
    }
}
*/

// ******************************************
bool FileIO::loadFromDisk(const QString &location, std::vector<unsigned char> &data) {
// ******************************************
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


// ******************************************
void FileIO::appendToCharVector(const QByteArray &byteArray, std::vector<unsigned char> &data) {
// ******************************************
    const unsigned int &len = byteArray.length();
    for (unsigned int i = 0; i < len; i++) {
        data.push_back(byteArray.at(i));
    }
}


// ******************************************
void FileIO::appendToByteArray(const std::vector<unsigned char> &data, QByteArray &byteArray) {
// ******************************************
    const unsigned int &len = data.size();
    for (unsigned int i = 0; i < len; i++) {
        byteArray.append(data.at(i));
    }
}


// ******************************************
void FileIO::appendToByteArray(const unsigned char *data, const unsigned int &amount, QByteArray &byteArray) {
// ******************************************
    for (unsigned int i = 0; i < amount; i++) {
        byteArray.append(data[i]);
    }
}


// ******************************************
bool FileIO::saveToDisk(const QString &location, const QByteArray &data) {
// ******************************************
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
