#ifndef FILEIO_H
#define FILEIO_H

#include <QString>
#include <vector>

// ******************************************
class FileIO
// ******************************************
{
public:
    static const int MAX_READ_SIZE = 10240;
    static bool loadFromDisk(const QString &location, std::vector<unsigned char> &data);
    static void appendToCharVector(const QByteArray &byteArray, std::vector<unsigned char> &data);
    static void appendToByteArray(const std::vector<unsigned char> &data, QByteArray &byteArray);
    static void appendToByteArray(const unsigned char *data, const unsigned int &amount, QByteArray &byteArray);
    static bool saveToDisk(const QString &location, const QByteArray &data);
};

#endif // FILEIO_H
