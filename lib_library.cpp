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
#include <QThread>
#include <stdint.h> // needed for hash calculation

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

    fetchPatchMode = false;
    fetchSequenceMode = false;
    fetchStart = 0;
    fetchEnd = 0;
    fetchNextIncomingPatch = 0;
    fetchNextPatchRequest = 0;
    fetchNextIncomingSequence = 0;
    fetchNextSequenceRequest = 0;

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
const Patch &Library::recallPatch(const int &id) const {
// ******************************************
    return patches.at(id);
}


// ******************************************
void Library::storePatch(const int &id, const Patch &patch) {
// ******************************************
    if (!patches.at(id).equals(patch)) {
        patches.at(id).set(patch);
        patchEdited.at(id) = true;
        patchMoved.at(id) = false;
    }
}


// ******************************************
void Library::listPatches() const {
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
    if (from == to) {
        return;
    }

    Patch temp = patches.at(from);
    patches.erase(patches.begin() + from);
    patches.insert(patches.begin() + to, temp);

    bool temp2 = patchEdited.at(from);
    patchEdited.erase(patchEdited.begin() + from);
    patchEdited.insert(patchEdited.begin() + to, temp2);

    // Mark as moved:
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
bool Library::patchHasBeenMoved(const int &id) const {
// ******************************************
    return patchMoved.at(id);
}


// ******************************************
bool Library::patchHasBeenEdited(const int &id) const {
// ******************************************
    return patchEdited.at(id);
}


// ******************************************
const Sequence &Library::recallSequence(const int &id) const {
// ******************************************
    return sequences.at(id);
}


// ******************************************
void Library::storeSequence(const int &id, const Sequence &sequence) {
// ******************************************
    if (!sequences.at(id).equals(sequence)) {
        sequences.at(id).set(sequence);
        sequenceEdited.at(id) = true;
        sequenceMoved.at(id) = false;
    }
}


// ******************************************
void Library::listSequences() const {
// ******************************************
    std::cout << "List of sequences:" << std::endl;



    const unsigned int &num = sequences.size();
    for (unsigned int i = 0; i < num; i++) {
        std::cout << "  " << i << ": "
                  << getSequenceIdentifier(i).toUtf8().constData()
                  << ", changed " << sequenceEdited.at(i)
                  << ", moved " << sequenceMoved.at(i) << std::endl;
    }
}


// ******************************************
void Library::moveSequence(const int &from, const int &to) {
// ******************************************
    if (from == to) {
        return;
    }

    Sequence temp = sequences.at(from);
    sequences.erase(sequences.begin() + from);
    sequences.insert(sequences.begin() + to, temp);

    bool temp2 = sequenceEdited.at(from);
    sequenceEdited.erase(sequenceEdited.begin() + from);
    sequenceEdited.insert(sequenceEdited.begin() + to, temp2);

    // Mark as moved:
    int start = from;
    int end = to;
    if (from > to) {
        start = to;
        end = from;
    }
    for (int i = start; i <= end; i++) {
        sequenceMoved.at(i) = true;
    }
}


// ******************************************
bool Library::sequenceHasBeenMoved(const int &id) const {
// ******************************************
    return sequenceMoved.at(id);
}


// ******************************************
bool Library::sequenceHasBeenEdited(const int &id) const {
// ******************************************
    return sequenceEdited.at(id);
}


// ******************************************
bool Library::sequenceIsInit(const int &id) const {
// ******************************************
    return sequences.at(id).equals(init_sequence);
}


// ******************************************
QString Library::getSequenceIdentifier(const int &id) const {
// ******************************************
    return (sequenceIsInit(id) ? QString("init") : QString("custom (%1)").arg(calculateSequenceHash(id).toUtf8().constData()));
}


// ******************************************
bool Library::send(const int &what, const int &from, const int &to) {
// ******************************************
    QTime t;
    t.start();
    std::vector<unsigned char> temp;
    bool ret = true;

    bool force = what&4;

    for (int i = from; i <= to; i++) {
        if ((what&1) && (force || patchHasBeenEdited(i) || patchHasBeenMoved(i))) {
            std::cout << i << " patch " << std::endl;
            temp.clear();
            patches.at(i).generateSysex(&temp);
            ret = midiout->write(temp);
            if (ret) {
                ret = midiout->patchWriteRequest(i);
            }
            if (ret) {
                patchEdited.at(i) = false;
                patchMoved.at(i) = false;
            }
            // Don't flood the Shruthi
            QThread::msleep(250);
        }
        if (ret && (what&2) && (force || sequenceHasBeenEdited(i) || sequenceHasBeenMoved(i))) {
            std::cout << i << " sequence " << std::endl;
            temp.clear();
            sequences.at(i).generateSysex(&temp);
            ret = midiout->write(temp);
            if (ret) {
                ret = midiout->sequenceWriteRequest(i);
            }
            if (ret) {
                sequenceEdited.at(i) = false;
                sequenceMoved.at(i) = false;
            }
            // Don't flood the Shruthi
            QThread::msleep(125);
        }

        if (!ret) {
            return false;
        }
    }

    std::cout << "Sending finished. Time elapsed: " << t.elapsed() << " ms." << std::endl;
    if (ret) {
        ret = midiout->programChange(0, to);
    }
    return ret;

}


// ******************************************
bool Library::fetch(const int &from, const int &to) {
// ******************************************
    // Note:
    // Shruthi displays the first patches number as 1, but calls it 0 internally.
    fetchPatchMode = true;
    fetchSequenceMode = true;
    fetchStart = from;
    fetchEnd = to;
    fetchNextIncomingPatch = from;
    fetchNextPatchRequest = from;
    fetchNextIncomingSequence = from;
    fetchNextSequenceRequest = from;

    time.start();

    return keepFetching();
}


// ******************************************
void Library::abortFetching() {
// ******************************************
    fetchEnd = 0;
    fetchPatchMode = false;
    fetchSequenceMode = false;
}


// ******************************************
bool Library::fetchPatches(const int &from, const int &to) {
// ******************************************
    // Note:
    // Shruthi displays the first patches number as 1, but calls it 0 internally.
    fetchPatchMode = true;
    fetchStart = from;
    fetchEnd = to;
    fetchNextIncomingPatch = from;
    fetchNextPatchRequest = from;

    time.start();

    return keepFetching();
}


// ******************************************
bool Library::receivedPatch(const unsigned char *sysex) {
// ******************************************
    if (!fetchPatchMode || fetchNextIncomingPatch > fetchEnd) {
        abortFetching();
        return false;
    }

#ifdef DEBUGMSGS
    std::cout << "Library::receivedPatch() " << fetchNextIncomingPatch << std::endl;
#endif

    // allocate space in vectors
    const int &temp = (fetchNextIncomingPatch - patches.size() + 1);
    if (temp > 0) {
        growPatchVectors(temp);
    }

    Patch tempp;
    bool ret = tempp.unpackData(sysex);

    if (ret) {
        patches.at(fetchNextIncomingPatch).set(tempp);
        patchEdited.at(fetchNextIncomingPatch) = false;
        patchMoved.at(fetchNextIncomingPatch) = false;
        fetchNextIncomingPatch++;

        //listPatches(); //DEBUG
        ret = keepFetching();
    } else {
        abortFetching();
    }
    return ret;
}


// ******************************************
bool Library::isFetchingPatches() const {
// ******************************************
#ifdef DEBUGMSGS
    std::cout << "Library::isFetchingPatches() " << fetchPatchMode << " " << fetchNextIncomingPatch << " " << fetchEnd << std::endl;
#endif
    return (fetchPatchMode && fetchNextIncomingPatch <= fetchEnd);
}


// ******************************************
bool Library::fetchSequences(const int &from, const int &to) {
// ******************************************
    // Note:
    // Shruthi displays the first patches number as 1, but calls it 0 internally.
    fetchSequenceMode = true;
    fetchStart = from;
    fetchEnd = to;
    fetchNextIncomingSequence = from;
    fetchNextSequenceRequest = from;

    time.start();

    return keepFetching();
}


// ******************************************
bool Library::receivedSequence(const unsigned char *seq) {
// ******************************************
    if (!fetchSequenceMode || fetchNextIncomingSequence > fetchEnd) {
        abortFetching();
        return false;
    }

#ifdef DEBUGMSGS
    std::cout << "Library::receivedSequence() " << fetchNextIncomingSequence << std::endl;
#endif

    // allocate space in vectors
    const int &temp = (fetchNextIncomingSequence - sequences.size() + 1);
    if (temp > 0) {
        growSequenceVectors(temp);
    }

    sequences.at(fetchNextIncomingSequence).unpackData(seq);
    sequenceEdited.at(fetchNextIncomingSequence) = false;
    sequenceMoved.at(fetchNextIncomingSequence) = false;
    fetchNextIncomingSequence++;

    //listSequences(); //DEBUG
    return keepFetching();
}


// ******************************************
bool Library::isFetchingSequences() const {
// ******************************************
#ifdef DEBUGMSGS
    std::cout << "Library::isFetchingSequences() " << fetchSequenceMode << " " << fetchNextIncomingSequence << " " << fetchEnd << std::endl;
#endif
    return (fetchSequenceMode && fetchNextIncomingSequence <= fetchEnd);
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
    unsigned int patch = 0;
    {
        int lastPosition = 0;
        bool keepGoing = true;

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
                patchEdited.at(patch) = false;
                patchMoved.at(patch) = false;
                patch++;
            }
            keepGoing = statusp && retp >= 0;
        }
    }

    // Sequence
    bool statuss = true;
    unsigned int sequence = 0;
    {
        bool keepGoing = true;
        int lastPosition = 0;

        std::vector<unsigned char> seq;
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
                sequenceEdited.at(sequence) = false;
                sequenceMoved.at(sequence) = false;
                sequence++;
            }
            keepGoing = statuss && rets >= 0;
        }
    }

    // Updated number of programs:
    // Note: there are two possible ways to handle this:
    // Either stop loading the programs after numberOfPrograms has been hit or decouple
    // the number of programs inside the library from the number of programs on the Shruthi.
    // Use approach number two!
    numberOfPrograms = std::max(numberOfPrograms, std::max(patch, sequence));

    return statusp && statuss;
}


// ******************************************
const unsigned int &Library::getNumberOfPrograms() const {
// ******************************************
    return numberOfPrograms;
}


// ******************************************
void Library::increaseNumberOfProgramsTo(const unsigned int &num) {
// ******************************************
    if (num > numberOfPrograms) {
        setNumberOfPrograms(num);
    }
}


// ******************************************
void Library::setNumberOfPrograms(const unsigned int &num) {
// ******************************************
    // Be really careful with this function. Most of the times
    // increaseNumberOfProgramsTo() has more suitable behavior.
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
int Library::nextPatch() const {
// ******************************************
    return fetchNextIncomingPatch;
}


// ******************************************
int Library::nextSequence() const {
// ******************************************
    return fetchNextIncomingSequence;
}


// ******************************************
bool Library::keepFetching() {
// ******************************************
#ifdef DEBUGMSGS
    std::cout << "Library::keepFetching(): Patches " << fetchPatchMode << " " << fetchNextPatchRequest << " " << fetchEnd << std::endl;
    std::cout << "Library::keepFetching(): Sequences " << fetchSequenceMode << " " << fetchNextSequenceRequest << " " << fetchEnd << std::endl;
#endif
    const bool ptc_enabled = fetchPatchMode && fetchNextPatchRequest <= fetchEnd;
    const bool seq_enabled = fetchSequenceMode && fetchNextSequenceRequest <= fetchEnd;

    if (!ptc_enabled && !seq_enabled) {
        // Finished fetching. Display statistics:
        std::cout << "Finished fetching ";

        if (fetchPatchMode) {
            std::cout << "patches";
            if (fetchSequenceMode) {
                std::cout << " and ";
            }
        }
        if (fetchSequenceMode) {
            std::cout << "sequences";
        }
        std::cout << ". It took " << time.elapsed() << " ms to fetch " << fetchEnd - fetchStart + 1 << " programs." << std::endl;

        abortFetching();
        return true;
    }

    const bool prefer_patch = fetchNextPatchRequest <= fetchNextSequenceRequest;

    bool ret;

    if ((ptc_enabled && prefer_patch) || (ptc_enabled && !seq_enabled)) {
        ret = midiout->programChange(0, fetchNextPatchRequest) && midiout->patchTransferRequest();
        if (ret) {
            fetchNextPatchRequest++;
        }
    } else {
        ret = midiout->programChange(0, fetchNextSequenceRequest) && midiout->sequenceTransferRequest();
        if (ret) {
            fetchNextSequenceRequest++;
        }
    }

    if (!ret) {
        abortFetching();
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


// ******************************************
QString Library::calculateSequenceHash(const unsigned int &id) const {
// ******************************************
    // uses public domain code for Bob Jenkins' One-at-a-Time Hash
    // source: http://burtleburtle.net/bob/hash/doobs.html
    unsigned char key[32];
    sequences.at(id).packData(key);
    return calculateHash(key, 32);
}


// ******************************************
QString Library::calculateHash(const unsigned char *key, const unsigned int &len) {
// ******************************************
    // uses public domain code for Bob Jenkins' One-at-a-Time Hash
    // source: http://burtleburtle.net/bob/hash/doobs.html
    uint32_t hash;
    unsigned int j;
    for(hash = j = 0; j < len; ++j) {
        hash += key[j];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return QString("%1").arg(hash, 8, 16, QChar('0'));
}
