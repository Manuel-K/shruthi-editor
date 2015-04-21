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

#ifndef SHRUTHI_MIDI_H
#define SHRUTHI_MIDI_H

#include <vector>
typedef std::vector<unsigned char> Message;


class Midi {
    public:
        static const unsigned char sysexHead[6];
        static const unsigned char sysexFoot;
        static unsigned char calculateChecksum(unsigned char sysex[], unsigned int start, unsigned int end);
        static unsigned char calculateChecksum(const std::vector<unsigned char> *message);
        static bool checkSysexHeadFoot(const std::vector<unsigned char> *message);
        static bool parseSysex(const std::vector<unsigned char> *message, std::vector<unsigned char> *data);
        static unsigned char nibbleToByte(unsigned char n0, unsigned char n1);
        static unsigned char getCommand(const Message *message);
        static unsigned char getArgument(const Message *message);
};

#endif
