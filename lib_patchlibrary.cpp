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

#include "lib_patchlibrary.h"
#include <QDebug>


// ******************************************
PatchLibrary::PatchLibrary() {
// ******************************************
    num=0;
    recieving=false;
//     max = 144;
    max = 4;
}


// ******************************************
PatchLibrary::~PatchLibrary() {
// ******************************************
}


// ******************************************
unsigned int PatchLibrary::getNumPatches() {
// ******************************************
    return num;
}


// ******************************************
unsigned int PatchLibrary::getMaxNumPatches() {
// ******************************************
    return max; 
}


// ******************************************
bool PatchLibrary::isRecieving() {
// ******************************************
    return recieving;
}


// ******************************************
void PatchLibrary::reset() {
// ******************************************
#ifdef DEBUG
    qDebug() << "PatchLibrary::reset()";
#endif
    recieving = true;
    num=0;
}


// ******************************************
bool PatchLibrary::isChanged(unsigned int i) {
// ******************************************
    return changed[i];
}


// ******************************************
Patch PatchLibrary::getPatch(unsigned int i) {
// ******************************************
    return lib[i];
}


// ******************************************
void PatchLibrary::setPatch(unsigned int i, Patch p) {
// ******************************************
    if (!lib[i].equals(p)) {
        changed[i]=true;
        lib[i]=p;
    }
}


// ******************************************
void PatchLibrary::resetPatch(unsigned int i) {
// ******************************************
    lib[i].resetPatch();
    changed[i]=true;
}


// ******************************************
QString PatchLibrary::getName(unsigned int i) {
// ******************************************
    return lib[i].getName();
}


// ******************************************
bool PatchLibrary::parseFullSysex(unsigned char  *sysex, unsigned int len) {
// ******************************************
    bool status = lib[num].parseFullSysex(sysex,len);
    changed[num] = false;
    num++;
    if (num>=max)
        recieving = false;
    return status;
}
