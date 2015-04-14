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
#include "lib_labels.h"
#include <vector>
#include "lib_midi.h"

// ******************************************
struct param_t {
// ******************************************
    QString name;
    int min;
    int max;
    QStringList* dropdown;
};


// ******************************************
static const param_t param_blank = {NULL, 0, 0, NULL};
// ******************************************


// ******************************************
class Patch {
// ******************************************
    private:
        QString name;
        int data[108];
        unsigned char version;
        void parseSysex(unsigned char *sysex);
        void generateSysex(unsigned char res[]);

        static param_t parameter84[];
        static param_t parameter85[];
        static param_t parameter92[];
        static param_t parameter93[];

    public:
        static const unsigned char parameterCount;
        static unsigned char INIT_PATCH[];
        static bool enabled(int);
        static bool hasUI(int);

        static param_t parameters[]; // use this with caution
        static param_t parameter(int id, int filter = 0);

        Patch();

        void setParam(int param, int value);
        int getParam(int param);
        void setName(QString name);
        QString getName();
        QString getVersionString();

        void printPatch();
        void resetPatch();
        void randomizePatch(int);
        bool parseFullSysex(unsigned char  *sysex, unsigned int len);
        bool parseFullSysex(std::vector<unsigned char> message);
        void generateFullSysex(std::vector<unsigned char> *message);

        bool loadFromDisk(QString location);
        bool saveToDisk(QString location);

        static unsigned char ccToNrpn(const unsigned char cc, const int filter);
        static int parseCcValue(const unsigned int val, int nrpn, const int filter);
};
#endif
