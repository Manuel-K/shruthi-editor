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
    sequences.reserve(16);
    sequenceEdited.reserve(16);
    sequenceMoved.reserve(16);

    midiout = out;

    fetchPatch = false;
    fetchSequence = false;
    fetchEnd = 0;
    fetchNextPatchForReceiving = 0;
    fetchNextRequestedPatch = 0;
    fetchNextSequenceForReceiving = 0;
    fetchNextRequestedSequence = 0;

    numberOfPrograms = 16;
    growPatchVectors(16);
    growSequenceVectors(16);

    //loadLibrary("library_complete.syx"); //DEBUG
    //listPatches(); //DEBUG
    //listSequences(); //DEBUG
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
void Library::movePatch(const int &from, const int &to) {
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
const Sequence &Library::recallSequence(const int &id) {
// ******************************************
    return sequences.at(id);
}


// ******************************************
void Library::storeSequence(const int &id, const Sequence &sequence) {
// ******************************************
    sequences.at(id).set(sequence);
}


// ******************************************
void Library::listSequences() {
// ******************************************
    std::cout << "List of sequences:" << std::endl;



    const unsigned int &num = sequences.size();
    for (unsigned int i = 0; i < num; i++) {
        std::cout << "  " << i << ": "
                  << (sequences.at(i).equals(init_sequence) ? "init" : "custom")
                  << ", changed " << sequenceEdited.at(i)
                  << ", moved " << sequenceMoved.at(i) << std::endl;
    }
}


// ******************************************
bool Library::moveSequence(const int &from, const int &to) {
// ******************************************
    //TODO: implement me
    return false;
}

// ******************************************
void Library::fetch(const int &from, const int &to) {
// ******************************************
    // Note:
    // Shruthi displays the first patches number as 1, but calls it 0 internally.
    fetchPatch = true;
    fetchSequence = true;
    fetchEnd = to;
    fetchNextPatchForReceiving = from;
    fetchNextRequestedPatch = from;
    fetchNextSequenceForReceiving = from;
    fetchNextRequestedSequence = from;

    keepFetching();
}


// ******************************************
void Library::fetchPatches(const int &from, const int &to) {
// ******************************************
    // Note:
    // Shruthi displays the first patches number as 1, but calls it 0 internally.
    fetchPatch = true;
    fetchEnd = to;
    fetchNextPatchForReceiving = from;
    fetchNextRequestedPatch = from;

    keepFetching();
}


// ******************************************
bool Library::receivedPatch(const unsigned char *sysex) {
// ******************************************
    if (!fetchPatch || fetchNextPatchForReceiving > fetchEnd) {
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
        keepFetching();
    } else {
        //TODO test if this really stops everything and unlocks the editor
        fetchEnd = 0;
        fetchPatch = false;
    }
    return ret;
}


// ******************************************
bool Library::isFetchingPatches() {
// ******************************************
#ifdef DEBUGMSGS
    std::cout << "Library::isFetchingPatches() " << fetchPatch << " " << fetchNextPatchForReceiving << " " << fetchEnd << std::endl;
#endif
    return (fetchPatch && fetchNextPatchForReceiving <= fetchEnd);
}


// ******************************************
void Library::fetchSequences(const int &from, const int &to) {
// ******************************************
    // Note:
    // Shruthi displays the first patches number as 1, but calls it 0 internally.
    fetchSequence = true;
    fetchEnd = to;
    fetchNextSequenceForReceiving = from;
    fetchNextRequestedSequence = from;

    keepFetching();
}


// ******************************************
bool Library::receivedSequence(const unsigned char *seq) {
// ******************************************
    if (!fetchSequence || fetchNextSequenceForReceiving > fetchEnd) {
        return false;
    }

#ifdef DEBUGMSGS
    std::cout << "Library::receivedSequence() " << fetchNextSequenceForReceiving << std::endl;
#endif

    // allocate space in vectors
    const int &temp = (fetchNextSequenceForReceiving - sequences.size() + 1);
    if (temp > 0) {
        growSequenceVectors(temp);
    }

    sequences.at(fetchNextSequenceForReceiving).unpackData(seq);
    fetchNextSequenceForReceiving++;

    //listSequences(); //DEBUG
    keepFetching();
    //TODO add function to stop everything and to unlock the editor
    return true;
}


// ******************************************
bool Library::isFetchingSequences() {
// ******************************************
#ifdef DEBUGMSGS
    std::cout << "Library::isFetchingSequences() " << fetchSequence << " " << fetchNextSequenceForReceiving << " " << fetchEnd << std::endl;
#endif
    return (fetchSequence && fetchNextSequenceForReceiving <= fetchEnd);
}


// ******************************************
bool Library::saveLibrary(const QString &path) {
// ******************************************
    QByteArray ba;

    std::vector<unsigned char> temp;

    const int &psize = patches.size();
    const int &ssize = sequences.size();
    const int &size = std::max(psize, ssize);

    for (int i = 0; i < size; i++) {
        // Patch
        if (i < psize) {
            temp.clear();
            patches.at(i).generateSysex(&temp);
            FileIO::appendToByteArray(temp, ba);
        }
        // Sequence
        if (i < ssize) {
            temp.clear();
            sequences.at(i).generateSysex(&temp);
            FileIO::appendToByteArray(temp, ba);
        }
    }

    bool status = FileIO::saveToDisk(path, ba);
    // CHECK: does this replace the file or only parts of it?
    return status;
}


// ******************************************
bool Library::loadLibrary(const QString &path) {
// ******************************************
    std::vector<unsigned char> temp;

    if (!FileIO::loadFromDisk(path, temp)) {
        return false;
    }

    // Patch:
    bool statusp = true;
    {
        int lastPosition = 0;
        bool keepGoing = true;

        unsigned int patch = 0;
        std::vector<unsigned char> ptc;
        Patch tempPatch;
        while(keepGoing) {
            //std::cout << "patch " << patch << " " << lastPosition << std::endl;
            const int &retp = Midi::getPatch(&temp, &ptc, lastPosition);
            statusp &= tempPatch.parseSysex(&ptc);

            //std::cout << "ret " << ret << std::endl;
            if(retp >= 0 && statusp) {
                lastPosition = retp + 195;
                if (patch >= patches.size()) {
                    growPatchVectors(1);
                }
                patches.at(patch).set(tempPatch);
                patch++;
            }
            keepGoing = statusp && retp >= 0;
        }
    }

    // Sequence
    bool statuss = true;
    {
        bool keepGoing = true;
        int lastPosition = 0;

        std::vector<unsigned char> seq;
        unsigned int sequence = 0;
        Sequence tempSequence;

        while(keepGoing) {
            //std::cout << "seq" << sequence << std::endl;
            const int &rets = Midi::getSequence(&temp, &seq, lastPosition);
            statuss &= tempSequence.parseSysex(&seq);

            //std::cout << "rets" << rets << " status " << statuss << std::endl;

            if (rets >= 0 && statuss) {
                lastPosition = rets + 95;
                if (sequence >= sequences.size()) {
                    growSequenceVectors(1);
                }
                sequences.at(sequence).set(tempSequence);
                sequence++;
            }
            keepGoing = statuss && rets >= 0;
        }
    }

    return statusp && statuss;
}


// ******************************************
void Library::setNumberOfPrograms(const unsigned int &num) {
// ******************************************
    patches.reserve(num);
    patchEdited.reserve(num);
    patchMoved.reserve(num);
    sequences.reserve(num);
    sequenceEdited.reserve(num);
    sequenceMoved.reserve(num);

    growPatchVectors(num - numberOfPrograms);
    growSequenceVectors(num - numberOfPrograms);
    numberOfPrograms = num;
}


// ******************************************
bool Library::keepFetching() {
// ******************************************
#ifdef DEBUGMSGS
    std::cout << "Library::keepFetching(): Patches " << fetchPatch << " " << fetchNextRequestedPatch << " " << fetchEnd << std::endl;
    std::cout << "Library::keepFetching(): Sequnces " << fetchSequence << " " << fetchNextSequenceForReceiving << " " << fetchEnd << std::endl;
#endif
    const bool ptc_enabled = fetchPatch && fetchNextRequestedPatch <= fetchEnd;
    const bool seq_enabled = fetchSequence && fetchNextRequestedSequence <= fetchEnd;

    if (!ptc_enabled && !seq_enabled) {
        return false;
    }

    const bool prefer_patch = fetchNextRequestedPatch <= fetchNextRequestedSequence;

    bool ret;

    if ((ptc_enabled && prefer_patch) || (ptc_enabled && !seq_enabled)) {
        ret = midiout->programChange(0, fetchNextRequestedPatch) && midiout->patchTransferRequest();
        if (ret) {
            fetchNextRequestedPatch++;
        }
    } else {
        ret = midiout->programChange(0, fetchNextRequestedSequence) && midiout->sequenceTransferRequest();
        if (ret) {
            fetchNextRequestedSequence++;
        }
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


// ******************************************
void Library::growSequenceVectors(const int &amount) {
// ******************************************
    for (int i = 0; i < amount; i++) {
        sequences.push_back(Sequence());
        sequenceMoved.push_back(false);
        sequenceEdited.push_back(false);
    }

}

