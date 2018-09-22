// Shruthi-Editor: An unofficial Editor for the Shruthi hardware synthesizer. For
// informations about the Shruthi, see <http://www.mutable-instruments.net/shruthi1>.
//
// Copyright (C) 2011-2018 Manuel Krönig
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


#ifndef SHRUTHI_MIDI_H
#define SHRUTHI_MIDI_H


#include "message.h"


class Midi {
    public:
        static const unsigned char sysexHead[6];
        static const unsigned char sysexFoot;
        static unsigned char calculateChecksum(unsigned char sysex[], unsigned int start, unsigned int end);
        static unsigned char calculateChecksum(const Message *message);
        static bool checkSysexHeadFoot(const Message *message);
        static bool checkSysexHeadFoot(const Message *message, const unsigned int start, const unsigned int end);
        static bool parseSysex(const Message *message, Message *data);
        static void generateSysex(const Message *payload, const int command, const int argument, Message *message);
        static int findNextPatch(const Message *message, const unsigned int start = 0);
        static int getPatch(const Message *message, Message *patch, const unsigned int start = 0);
        static int findNextSequence(const Message *message, const unsigned int start = 0);
        static int getSequence(const Message *message, Message *sequence, const unsigned int start = 0);
        static unsigned char nibbleToByte(unsigned char n0, unsigned char n1);
        static unsigned char byteToUpperNibble(unsigned char n);
        static unsigned char byteToLowerNibble(unsigned char n);
        static unsigned char getCommand(const Message *message, const unsigned int start = 0);
        static unsigned char getArgument(const Message *message, const unsigned int start = 0);
};


#endif // SHRUTHI_MIDI_H
