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

#include "lib_midi.h"

// ******************************************
#ifdef PRE094SYSEXHEADER
const unsigned char Midi::sysexHead[6]={0xf0,0x00,0x20,0x77,0x00,0x02}; // pre 0.94
#else
const unsigned char Midi::sysexHead[6]={0xf0,0x00,0x21,0x02,0x00,0x02}; // post 0.94
#endif
// ******************************************


// ******************************************
const unsigned char Midi::sysexFoot=0xf7;
// ******************************************


// ******************************************
unsigned char Midi::calculateChecksum(unsigned char sysex[], unsigned int start, unsigned int end) {
// ******************************************
    unsigned long chk=0;
    for(unsigned int i=start; i<end;i++) {
        chk+=sysex[i];
    }
    return (unsigned char) (chk % 256);
}

// ******************************************
bool Midi::checkSysexHeadFoot(const std::vector<unsigned char> *message) {
// ******************************************
    const unsigned int size = message->size();

    if (size>=7) {
        // Check header:
        for (int i = 0; i < 6; i ++) {
            if ((int)message->at(i) != sysexHead[i]) {
                return false;
            }
        }

        // Check footer:
        if ((int)message->at(size-1)==sysexFoot) {
            return true;
        }
    }

    return false;
}

