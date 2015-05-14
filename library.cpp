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


#include "library.h"
#include "fileio.h"
#include "midi.h"
#include "midiout.h"

#include <iostream>
#include <QThread>
#include <QTime>
#include <stdint.h> // needed for hash calculation


Library::Library(MidiOut *out):
    midiout(out),
    time(new QTime),
    numberOfPrograms(0),
    numberOfHWPrograms(16),
    firmwareVersion(0),
    firmwareVersionRequested(false) {
    abortFetching();
    fetchNextIncomingPatch = 0;
    fetchNextPatchRequest = 0;
    fetchNextIncomingSequence = 0;
    fetchNextSequenceRequest = 0;

    mRememberedCurrentShruthiProgram = false;

    growVectorsTo(16);
}


Library::~Library() {
    delete time;
    time = NULL;
}


void Library::setFirmwareVersion(const int &version) {
    firmwareVersion = version;
}

void Library::setFirmwareVersionRequested() {
    firmwareVersionRequested = true;
}


void Library::setMidiChannel(const unsigned char &channel) {
    mMidiChannel = channel;
}


const Patch &Library::recallPatch(const int &id) const {
    return patches.at(id);
}


void Library::storePatch(const int &id, const Patch &patch) {
    if (!patches.at(id).equals(patch)) {
        patches.at(id).set(patch);
        mPatchEdited.at(id) = true;
        mPatchMoved.at(id) = false;
    }
}


void Library::listPatches() const {
    std::cout << "List of patches:" << std::endl;

    const unsigned int &num = patches.size();
    for (unsigned int i = 0; i < num; i++) {
        std::cout << "  " << i << ": "
                  << patches.at(i).getName().toUtf8().constData()
                  << ", changed " << mPatchEdited.at(i)
                  << ", moved " << mPatchMoved.at(i) << std::endl;
    }
}


void Library::movePatch(const int &from, const int &to) {
    if (from == to) {
        return;
    }

    Patch temp = patches.at(from);
    patches.erase(patches.begin() + from);
    patches.insert(patches.begin() + to, temp);

    bool temp2 = mPatchEdited.at(from);
    mPatchEdited.erase(mPatchEdited.begin() + from);
    mPatchEdited.insert(mPatchEdited.begin() + to, temp2);

    // Mark as moved:
    int start = from;
    int end = to;
    if (from > to) {
        start = to;
        end = from;
    }
    for (int i = start; i <= end; i++) {
        mPatchMoved.at(i) = true;
    }
}


bool Library::patchMoved(const int &id) const {
    return mPatchMoved.at(id);
}


bool Library::patchEdited(const int &id) const {
    return mPatchEdited.at(id);
}


QString Library::getPatchIdentifier(const int &id) const {
    const Patch &p = patches.at(id);
    return p.getName().leftJustified(9, ' ') + "(" + p.getVersionString() + ")";
}


const Sequence &Library::recallSequence(const int &id) const {
    return sequences.at(id);
}


void Library::storeSequence(const int &id, const Sequence &sequence) {
    if (!sequences.at(id).equals(sequence)) {
        sequences.at(id).set(sequence);
        mSequenceEdited.at(id) = true;
        mSequenceMoved.at(id) = false;
    }
}


void Library::listSequences() const {
    std::cout << "List of sequences:" << std::endl;

    const unsigned int &num = sequences.size();
    for (unsigned int i = 0; i < num; i++) {
        std::cout << "  " << i << ": "
                  << getSequenceIdentifier(i).toUtf8().constData()
                  << ", changed " << mSequenceEdited.at(i)
                  << ", moved " << mSequenceMoved.at(i) << std::endl;
    }
}


void Library::moveSequence(const int &from, const int &to) {
    if (from == to) {
        return;
    }

    Sequence temp = sequences.at(from);
    sequences.erase(sequences.begin() + from);
    sequences.insert(sequences.begin() + to, temp);

    bool temp2 = mSequenceEdited.at(from);
    mSequenceEdited.erase(mSequenceEdited.begin() + from);
    mSequenceEdited.insert(mSequenceEdited.begin() + to, temp2);

    // Mark as moved:
    int start = from;
    int end = to;
    if (from > to) {
        start = to;
        end = from;
    }
    for (int i = start; i <= end; i++) {
        mSequenceMoved.at(i) = true;
    }
}


bool Library::sequenceMoved(const int &id) const {
    return mSequenceMoved.at(id);
}


bool Library::sequenceEdited(const int &id) const {
    return mSequenceEdited.at(id);
}


bool Library::sequenceIsInit(const int &id) const {
    return sequences.at(id).equals(init_sequence);
}


QString Library::getSequenceIdentifier(const int &id) const {
    return (sequenceIsInit(id) ? QString("init") : QString("custom (%1)").arg(calculateSequenceHash(id).toUtf8().constData()));
}


bool Library::send(const int &what, const int &from, const int &to) {
    QTime t;
    t.start();
    Message temp;
    bool ret = true;

    bool force = !(what&FLAG_CHANGED);

    for (int i = from; i <= to; i++) {
        if ((what&FLAG_PATCH) && (force || patchEdited(i) || patchMoved(i))) {
            std::cout << i << " patch " << std::endl;
            temp.clear();
            patches.at(i).generateSysex(&temp);
            ret = midiout->write(temp);
            if (ret) {
                ret = midiout->patchWriteRequest(i);
            }
            if (ret) {
                mPatchEdited.at(i) = false;
                mPatchMoved.at(i) = false;
            }
            // Don't flood the Shruthi
            QThread::msleep(250);
        }
        if (ret && (what&FLAG_SEQUENCE) && (force || sequenceEdited(i) || sequenceMoved(i))) {
            std::cout << i << " sequence " << std::endl;
            temp.clear();
            sequences.at(i).generateSysex(&temp);
            ret = midiout->write(temp);
            if (ret) {
                ret = midiout->sequenceWriteRequest(i);
            }
            if (ret) {
                mSequenceEdited.at(i) = false;
                mSequenceMoved.at(i) = false;
            }
            // Don't flood the Shruthi
            QThread::msleep(125);
        }

        if (!ret) {
            return false;
        }
    }

    std::cout << "Sending finished. Time elapsed: " << t.elapsed() << " ms." << std::endl;
    return ret;
}


bool Library::startFetching(const int &flags, const int &from, const int &to) {
    if (!(flags&FLAG_PATCH) && !(flags&FLAG_SEQUENCE)) {
        return false;
    }
    // Note:
    // Shruthi displays the first patches number as 1, but calls it 0 internally.
    fetchStart = from;
    fetchEnd = to;

    if (flags&FLAG_PATCH) {
        fetchPatchMode = true;
        fetchNextIncomingPatch = from;
        fetchNextPatchRequest = from;
    }
    if (flags&FLAG_SEQUENCE) {
        fetchSequenceMode = true;
        fetchNextIncomingSequence = from;
        fetchNextSequenceRequest = from;
    }
    time->start();

    return keepFetching();
}


void Library::abortFetching() {
    fetchPatchMode = false;
    fetchSequenceMode = false;
    fetchStart = 0;
    fetchEnd = 0;
}


bool Library::receivedPatch(const unsigned char *sysex) {
    if (!fetchPatchMode || fetchNextIncomingPatch > fetchEnd) {
        abortFetching();
        return false;
    }

#ifdef DEBUGMSGS
    std::cout << "Library::receivedPatch() " << fetchNextIncomingPatch << std::endl;
#endif

    // allocate space in vectors
    growVectorsTo(fetchNextIncomingPatch + 1);

    Patch tempp;
    bool ret = tempp.unpackData(sysex);

    if (ret) {
        patches.at(fetchNextIncomingPatch).set(tempp);
        mPatchEdited.at(fetchNextIncomingPatch) = false;
        mPatchMoved.at(fetchNextIncomingPatch) = false;
        fetchNextIncomingPatch++;

        ret = keepFetching();
    } else {
        abortFetching();
    }
    return ret;
}


bool Library::isFetchingPatches() const {
#ifdef DEBUGMSGS
    std::cout << "Library::isFetchingPatches() " << fetchPatchMode << " " << fetchNextIncomingPatch << " " << fetchEnd << std::endl;
#endif
    return (fetchPatchMode && fetchNextIncomingPatch <= fetchEnd);
}


bool Library::receivedSequence(const unsigned char *seq) {
    if (!fetchSequenceMode || fetchNextIncomingSequence > fetchEnd) {
        abortFetching();
        return false;
    }

#ifdef DEBUGMSGS
    std::cout << "Library::receivedSequence() " << fetchNextIncomingSequence << std::endl;
#endif

    // allocate space in vectors
    growVectorsTo(fetchNextIncomingSequence + 1);

    sequences.at(fetchNextIncomingSequence).unpackData(seq);
    mSequenceEdited.at(fetchNextIncomingSequence) = false;
    mSequenceMoved.at(fetchNextIncomingSequence) = false;
    fetchNextIncomingSequence++;

    return keepFetching();
}


bool Library::isFetchingSequences() const {
#ifdef DEBUGMSGS
    std::cout << "Library::isFetchingSequences() " << fetchSequenceMode << " " << fetchNextIncomingSequence << " " << fetchEnd << std::endl;
#endif
    return (fetchSequenceMode && fetchNextIncomingSequence <= fetchEnd);
}


void Library::deletePrograms(const int &from, const int &to) {
#ifdef DEBUGMSGS
    std::cout << "Library::deletePrograms(" << from << ", " << to << ");" << std::endl;
#endif
    patches.erase(patches.begin() + from, patches.begin() + to + 1);
    mPatchMoved.erase(mPatchMoved.begin() + from, mPatchMoved.begin() + to + 1);
    mPatchEdited.erase(mPatchEdited.begin() + from, mPatchEdited.begin() + to + 1);
    sequences.erase(sequences.begin() + from, sequences.begin() + to + 1);
    mSequenceMoved.erase(mSequenceMoved.begin() + from, mSequenceMoved.begin() + to + 1);
    mSequenceEdited.erase(mSequenceEdited.begin() + from, mSequenceEdited.begin() + to + 1);

    numberOfPrograms -= to - from + 1;

    // make sure that we have at least the number of hw programs
    growVectorsTo(numberOfHWPrograms);

    // Mark as moved:
    for (int i = from; i < numberOfPrograms; i++) {
        mPatchMoved.at(i) = true;
        mSequenceMoved.at(i) = true;
    }
}


void Library::insertProgram(const int &id) {
#ifdef DEBUGMSGS
    std::cout << "Library::insertProgram(" << id << ");" << std::endl;
#endif
    if (firmwareVersionRequested) {
        patches.insert(patches.begin() + id + 1, Patch(firmwareVersion));
    } else {
        patches.insert(patches.begin() + id + 1, Patch());
    }
    mPatchMoved.insert(mPatchMoved.begin() + id + 1, false);
    mPatchEdited.insert(mPatchEdited.begin() + id + 1, true);
    sequences.insert(sequences.begin() + id + 1, Sequence());
    mSequenceMoved.insert(mSequenceMoved.begin() + id + 1, false);
    mSequenceEdited.insert(mSequenceEdited.begin() + id + 1, true);
    numberOfPrograms += 1;

    // Mark as moved:
    for (int i = id + 2; i < numberOfPrograms; i++) {
        mPatchMoved.at(i) = true;
        mSequenceMoved.at(i) = true;
    }
}


bool Library::saveLibrary(const QString &path) {
    QByteArray ba;

    Message temp;

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


bool Library::loadLibrary(const QString &path, bool append) {
    Message temp;

    if (!FileIO::loadFromDisk(path, temp)) {
        return false;
    }

    bool statusp = true;
    bool statuss = true;
    int patch = 0;
    int sequence = 0;

    if (append) {
        patch = patches.size();
        sequence = sequences.size();
    }

    // Patch:
    int lastPosition = 0;
    bool keepGoing = true;

    Message ptc;
    Patch tempPatch;
    while(keepGoing) {
        //std::cout << "patch " << patch << " " << lastPosition << std::endl;
        const int &retp = Midi::getPatch(&temp, &ptc, lastPosition);
        statusp &= tempPatch.parseSysex(&ptc);

        //std::cout << "ret " << ret << std::endl;
        if(retp >= 0 && statusp) {
            lastPosition = retp + 195;
            growVectorsTo(patch + 1);
            patches.at(patch).set(tempPatch);
            mPatchEdited.at(patch) = false;
            mPatchMoved.at(patch) = false;
            patch++;
        }
        keepGoing = statusp && retp >= 0;
    }

    // Sequence
    keepGoing = true;
    lastPosition = 0;

    Message seq;
    Sequence tempSequence;

    while(keepGoing) {
        //std::cout << "seq" << sequence << std::endl;
        const int &rets = Midi::getSequence(&temp, &seq, lastPosition);
        statuss &= tempSequence.parseSysex(&seq);

        //std::cout << "rets" << rets << " status " << statuss << std::endl;

        if (rets >= 0 && statuss) {
            lastPosition = rets + 95;
            growVectorsTo(sequence + 1);
            sequences.at(sequence).set(tempSequence);
            mSequenceEdited.at(sequence) = false;
            mSequenceMoved.at(sequence) = false;
            sequence++;
        }
        keepGoing = statuss && rets >= 0;
    }

    // Updated number of programs:
    // Note: there are two possible ways to handle this:
    // Either stop loading the programs after numberOfPrograms has been hit or decouple
    // the number of programs inside the library from the number of programs on the Shruthi.
    // Use approach number two!
    numberOfPrograms = std::max(numberOfPrograms, std::max(patch, sequence));

    return statusp && statuss;
}


const int &Library::getNumberOfPrograms() const {
    return numberOfPrograms;
}


const int &Library::getNumberOfHWPrograms() const {
    return numberOfHWPrograms;
}


void Library::setNumberOfHWPrograms(const int &num) {
    numberOfHWPrograms = num;
    growVectorsTo(num);
}


const unsigned int &Library::nextPatch() const {
    return fetchNextIncomingPatch;
}


const unsigned int &Library::nextSequence() const {
    return fetchNextIncomingSequence;
}


void Library::rememberShruthiProgram(const int &patch, const int &sequence) {
    mCurrentShruthiPatch = patch;
    mCurrentShruthiSequence = sequence;
    mRememberedCurrentShruthiProgram = true;
}


bool Library::recallShruthiProgramm() {
    if (mRememberedCurrentShruthiProgram) {
        bool response = midiout->automaticProgramChange(mMidiChannel,
                                                        firmwareVersion,
                                                        mCurrentShruthiPatch,
                                                        mCurrentShruthiSequence);
        if (response) {
            mRememberedCurrentShruthiProgram = false;
        }
        return response;
    }
    return true; // don't need to do anything
}



bool Library::keepFetching() {
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
        std::cout << ". It took " << time->elapsed() << " ms to fetch " << fetchEnd - fetchStart + 1 << " programs." << std::endl;

        abortFetching();
        return recallShruthiProgramm();
    }

    const bool prefer_patch = fetchNextPatchRequest <= fetchNextSequenceRequest;

    bool ret;

    if ((ptc_enabled && prefer_patch) || (ptc_enabled && !seq_enabled)) {
        ret = midiout->programChange(mMidiChannel, fetchNextPatchRequest) && midiout->patchTransferRequest();
        if (ret) {
            fetchNextPatchRequest++;
        }
    } else {
        ret = midiout->programChange(mMidiChannel, fetchNextSequenceRequest) && midiout->sequenceTransferRequest();
        if (ret) {
            fetchNextSequenceRequest++;
        }
    }

    if (!ret) {
        abortFetching();
    }

    return ret;
}


void Library::growVectorsTo(const int &num) {
    const int &amount = num - numberOfPrograms;
    if (amount > 0) {
        numberOfPrograms = num;
        patches.reserve(num);
        mPatchEdited.reserve(num);
        mPatchMoved.reserve(num);
        sequences.reserve(num);
        mSequenceEdited.reserve(num);
        mSequenceMoved.reserve(num);


        for (int i = 0; i < amount; i++) {
            if (firmwareVersionRequested) {
                patches.push_back(Patch(firmwareVersion));
            } else {
                patches.push_back(Patch());
            }
            mPatchEdited.push_back(false);
            mPatchMoved.push_back(false);
            sequences.push_back(Sequence());
            mSequenceMoved.push_back(false);
            mSequenceEdited.push_back(false);
        }
    }
}


QString Library::calculateSequenceHash(const unsigned int &id) const {
    unsigned char key[32];
    sequences.at(id).packData(key);
    return calculateHash(key, 32);
}


QString Library::calculateHash(const unsigned char *key, const unsigned int &len) {
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
