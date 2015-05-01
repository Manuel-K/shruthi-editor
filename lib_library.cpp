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


#include "lib_library.h"
#include "lib_fileio.h"
#include "lib_midi.h"

#include <iostream>


// ******************************************
Library::Library(MidiOut *out) {
// ******************************************
    patches.reserve(16);
    patchEdited.reserve(16);
    patchMoved.reserve(16);

    midiout = out;

    fetchEnd = 0;
    fetchNextPatchForReceiving = 0;
    fetchNextRequestedPatch = 0;

    numberOfPrograms = 16;
    growPatchVectors(16);

    //loadLibrary("library_all.syx"); //DEBUG
    //listPatches(); //DEBUG
}


// ******************************************
Library::~Library() {
// ******************************************
    //saveLibrary("library.syx"); //DEBUG
}


// ******************************************
const Patch &Library::recallPatch(const int &id) {
// ******************************************
    return patches.at(id);
}


// ******************************************
void Library::storePatch(const int &id, const Patch &patch) {
// ******************************************
    patches.at(id).set(patch);
    patchEdited.at(id) = true;
}


// ******************************************
void Library::listPatches() {
// ******************************************
    std::cout << "List of patches:" << std::endl;

    const unsigned int &num = patches.size();
    for (unsigned int i = 0; i < num; i++) {
        std::cout << "  " << i << ": "
                  << patches.at(i).getName().toUtf8().constData()
                  << ", changed " << patchEdited.at(i)
                  << ", moved " << patchMoved.at(i) << std::endl;
    }
}


// ******************************************
void Library::move(const int &from, const int &to) {
// ******************************************
    return; //this is completely untested!

    if (from == to) {
        return;
    }
    // TODO implement/test me

    patches.insert(patches.begin() + to, patches.at(from));
    patches.erase(patches.begin() + from);

    patchEdited.insert(patchEdited.begin() + to, patchEdited.at(from));
    patchEdited.erase(patchEdited.begin() + from);


    // mark as moved:
    int start = from;
    int end = to;
    if (from > to) {
        start = to;
        end = from;
    }
    for (int i = start; i <= end; i++) {
        patchMoved.at(i) = true;
    }
}


// ******************************************
void Library::fetchPatches(const int &from, const int &to) {
// ******************************************
    // Note:
    // Shruthi displays the first patches number as 1, but calls it 0 internally.
    fetchEnd = to;
    fetchNextPatchForReceiving = from;
    fetchNextRequestedPatch = from;

    keepFetchingPatches();
}


// ******************************************
bool Library::receivedPatch(const unsigned char *sysex) {
// ******************************************
    if (fetchNextPatchForReceiving > fetchEnd) {
        return false;
    }

#ifdef DEBUGMSGS
    std::cout << "Library::receivedPatch() " << fetchNextPatchForReceiving << std::endl;
#endif

    // allocate space in vectors
    const int &temp = (fetchNextPatchForReceiving - patches.size() + 1);
    if (temp > 0) {
        growPatchVectors(temp);
    }

    bool ret = patches.at(fetchNextPatchForReceiving).unpackData(sysex);
    if (ret) {
        fetchNextPatchForReceiving++;

        //listPatches(); //DEBUG
        keepFetchingPatches();
    } else {
        //TODO test if this really stops everything and unlocks the editor
        fetchEnd = 0;
    }
    return ret;
}


// ******************************************
bool Library::isFetchingPatches() {
// ******************************************
#ifdef DEBUGMSGS
    std::cout << "Library::isFetchingPatches() " << fetchNextPatchForReceiving << " " << fetchEnd << std::endl;
#endif
    return (fetchNextPatchForReceiving <= fetchEnd);
}


// ******************************************
bool Library::saveLibrary(const QString &path) {
// ******************************************
    QByteArray ba;

    std::vector<unsigned char> temp;

    const int &size = patches.size();
    for (int i = 0; i < size; i++) {
        temp.clear();
        patches.at(i).generateSysex(&temp);
        FileIO::appendToByteArray(temp, ba);
    }

    bool status = FileIO::saveToDisk(path, ba);
    return status;
}


// ******************************************
bool Library::loadLibrary(const QString &path) {
// ******************************************
    std::vector<unsigned char> temp;
    bool status = FileIO::loadFromDisk(path, temp);

    int lastPosition = 0;
    bool keepGoing = true;
    std::vector<unsigned char> ptc;
    unsigned int patch = 0;

    Patch tempPatch;
    while(keepGoing) {


        //std::cout << "patch " << patch << " " << lastPosition << std::endl;
        const int &ret = Midi::getPatch(&temp, &ptc, lastPosition);
        status &= tempPatch.parseSysex(&ptc);

        //std::cout << "ret " << ret << std::endl;
        if(ret >= 0 && status) {
            lastPosition = ret + 195;
            if (patch >= patches.size()) {
                growPatchVectors(1);
            }
            patches.at(patch).set(tempPatch);
        }

        patch++;
        //            std::vector<unsigned char> seq;
        //            // ignore return value; if it fails, seq is empty:
        //            Midi::getSequence(&temp, &seq);
        //            status &= sequence.parseSysex(&seq);
        //        }

        keepGoing = status && ret >= 0;
    }

    return status;
}


// ******************************************
void Library::setNumberOfPrograms(const unsigned int &num) {
// ******************************************
    growPatchVectors(num - numberOfPrograms);
    numberOfPrograms = num;
}


// ******************************************
bool Library::keepFetchingPatches() {
// ******************************************
#ifdef DEBUGMSGS
    std::cout << "Library::keepFetchingPatches() " << fetchNextRequestedPatch << " " << fetchEnd << std::endl;
#endif
    if (fetchNextRequestedPatch > fetchEnd) {
        return false;
    }

    bool ret = midiout->programChange(0, fetchNextRequestedPatch) && midiout->patchTransferRequest();
    if (ret) {
        fetchNextRequestedPatch ++;
    }
    return ret;
}


// ******************************************
void Library::growPatchVectors(const int &amount) {
// ******************************************
    for (int i = 0; i < amount; i++) {
        patches.push_back(Patch());
        patchEdited.push_back(false);
        patchMoved.push_back(false);
    }
}

