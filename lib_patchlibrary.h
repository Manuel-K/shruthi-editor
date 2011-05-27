// Shruthi-Editor: An unofficial Editor for the Shruthi hardware synthesizer. For 
// informations about the Shruthi, see <http://www.mutable-instruments.net/shruthi1>. 
//
// Copyright (C) 2011 Manuel Krönig
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

#ifndef SHRUTHI_PATCH_LIBRARY_H
#define SHRUTHI_PATCH_LIBRARY_H
#include "lib_patch.h"

// ******************************************
class PatchLibrary {
// ******************************************
    private:
        Patch lib[150];
        bool changed[150];
        unsigned int num;
        bool recieving;
        unsigned int max;
    public:
        PatchLibrary();
        ~PatchLibrary();
        unsigned int getNumPatches();
        unsigned int getMaxNumPatches();

        bool isRecieving();
        void reset();
        bool isChanged(unsigned int);
        Patch getPatch(unsigned int);
        void setPatch(unsigned int,Patch);
        void resetPatch(unsigned int);
        QString getName(unsigned int);
        bool parseFullSysex(unsigned char  *sysex, unsigned int len);
};
#endif
