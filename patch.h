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

#ifndef SHRUTHI_PATCH_H
#define SHRUTHI_PATCH_H
#include "labels.h"
#include "message.h"


// ******************************************
struct PatchParameter {
// ******************************************
    QString name;
    QString short_name;
    int min;
    int max;
    const QStringList* dropdown;
    QString (*formatter)(int);
    int cc;
};


// ******************************************
static const PatchParameter param_blank = {NULL, NULL, 0, 0, NULL, NULL, -1};
// ******************************************


// ******************************************
class Patch {
// ******************************************
    public:
        Patch();

        static PatchParameter parameter(const int &id, int filter = 0);

        static bool enabled(const int &id);
        static bool hasUI(const int &id);
        static bool sendAsNRPN(const int &id);
        static bool belongsToModMatrix(const int &id);

        void setValue(const int &id, int value);
        const int &getValue(const int &id) const;
        static QString formatParameterValue(const int &id, const int &value, int filter = 0);
        void setName(const QString &name);
        const QString &getName() const;
        QString getVersionString() const;

        void resetPatch(unsigned int version = 1000);
        void randomizePatch(const int &filter);

        bool unpackData(const unsigned char *sysex);
        void packData(unsigned char res[]) const;
        bool parseSysex(const Message *message);
        void generateSysex(Message *message) const;

        bool equals(const Patch &other) const;
        void set(const Patch &other);

        static unsigned char ccToId(const unsigned char &cc, const int &filter);
        static int convertCCValue(const unsigned int &val, int &id, const int &filter);

        static const unsigned char parameterCount;
        static const unsigned char filterBoardCount;

    private:
        void printPatch() const;

        QString name;
        int data[110];
        unsigned char version;

        static const PatchParameter parameters[];
        static const PatchParameter parameter84[];
        static const PatchParameter parameter85[];
        static const PatchParameter parameter92[];
        static const PatchParameter parameter93[];

        static const unsigned char INIT_PATCH[];
        static const unsigned char INIT_PATCH_PRE100[];
};
#endif
