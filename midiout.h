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


#ifndef SHRUTHI_MIDIOUT_H
#define SHRUTHI_MIDIOUT_H


#include "message.h"
class RtMidiOut;


class MidiOut {
    public:
        MidiOut();
        ~MidiOut();
        bool open(const unsigned int &channel);
        bool write(Message &sysex);

        // Wrappers:
        bool nrpn(const int &nrpn, const int &value);
        bool noteOn(const unsigned char &channel, const unsigned char &note, const unsigned char &velocity);
        bool noteOff(const unsigned char &channel, const unsigned char &note);
        bool allNotesOff(const unsigned char &channel);
        bool programChange(const unsigned char &channel, const int &program);
        bool programChangeSequence(const unsigned char &channel, const int &sequence);
        bool controlChange(const unsigned char &channel, const unsigned char &controller, const unsigned char &value);

        // Requests:
        bool patchTransferRequest();
        bool sequenceTransferRequest();
        bool versionRequest();
        bool numBanksRequest();
        bool currentPatchSequenceRequest();
        bool patchWriteRequest(const int &slot);
        bool sequenceWriteRequest(const int &slot);

    private:
        MidiOut(const MidiOut&); //forbid copying
        MidiOut &operator=(const MidiOut&); //forbid assignment

        // Wrappers:
        bool write(const unsigned char &c1, const unsigned char &c2, const unsigned char &c3);
        bool write(const unsigned char &c1, const unsigned char &c2);
        bool request(const unsigned char &which);
        bool writeRequest(const int &slot, const unsigned char &which);

        RtMidiOut* midiout;
        bool opened;
        unsigned int output;
        bool initialized;
};


#endif // SHRUTHI_MIDIOUT_H
