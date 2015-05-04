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


#ifndef LIBRARY_H
#define LIBRARY_H

#include <vector>
#include "lib_patch.h"
#include "lib_sequence.h"
#include "lib_midiout.h"

class QString;
#include <QTime>

// ******************************************
class Library {
// ******************************************

    public:
        Library(MidiOut *out);
        ~Library();

        const Patch &recallPatch(const int &id) const;
        void storePatch(const int &id, const Patch &patch);
        void listPatches() const;
        void movePatch(const int &from, const int &to);

        bool patchHasBeenMoved(const int &id) const;
        bool patchHasBeenEdited(const int &id) const;

        const Sequence &recallSequence(const int &id) const;
        void storeSequence(const int &id, const Sequence &sequence);
        void listSequences() const;
        void moveSequence(const int &from, const int &to);

        bool sequenceHasBeenMoved(const int &id) const;
        bool sequenceHasBeenEdited(const int &id) const;
        bool sequenceIsInit(const int &id) const;
        QString getSequenceIdentifier(const int &id) const;

        bool send(const int &what, const int &from, const int &to);

        bool fetch(const int &from, const int &to);
        void abortFetching();

        bool fetchPatches(const int &from, const int &to);
        bool receivedPatch(const unsigned char *sysex);
        bool isFetchingPatches() const;

        bool fetchSequences(const int &from, const int &to);
        bool receivedSequence(const unsigned char *seq);
        bool isFetchingSequences() const;

        bool saveLibrary(const QString &path);
        bool loadLibrary(const QString &path);

        const unsigned int &getNumberOfPrograms() const;
        void setNumberOfPrograms(const unsigned int &num);

        int nextPatch() const;
        int nextSequence() const;

    private:
        bool keepFetching();

        std::vector<Patch> patches;
        std::vector<bool> patchMoved;
        std::vector<bool> patchEdited;

        std::vector<Sequence> sequences;
        std::vector<bool> sequenceMoved;
        std::vector<bool> sequenceEdited;

        const Sequence init_sequence;

        void growPatchVectors(const int &amount);
        void growSequenceVectors(const int &amount);

        QString calculateSequenceHash(const unsigned int &id) const;
        static QString calculateHash(const unsigned char *key, const unsigned int &len);

        MidiOut *midiout;

        bool fetchPatchMode;
        bool fetchSequenceMode;

        unsigned int fetchStart;
        unsigned int fetchEnd;
        unsigned int fetchNextIncomingPatch;
        unsigned int fetchNextPatchRequest;

        unsigned int fetchNextIncomingSequence;
        unsigned int fetchNextSequenceRequest;

        QTime time;

        unsigned int numberOfPrograms;

};

#endif // LIBRARY_H
