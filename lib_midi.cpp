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
    for (unsigned int i=start; i<end;i++) {
        chk+=sysex[i];
    }
    return (unsigned char)(chk % 256);
}


// ******************************************
unsigned char Midi::calculateChecksum(const std::vector<unsigned char> *message)
// ******************************************
{
    unsigned long chk = 0;
    for (unsigned int i = 0; i < message->size(); i++) {
        chk += message->at(i);
    }
    return (unsigned char)(chk % 256);
}


// ******************************************
bool Midi::checkSysexHeadFoot(const std::vector<unsigned char> *message) {
// ******************************************
    const unsigned int size = message->size();

    // Check if data is even (note: header + footer are uneven)
    if (size % 2 == 0) {
        return false;
    }

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


// ******************************************
unsigned char Midi::nibbleToByte(unsigned char n0, unsigned char n1)
// ******************************************
{
    return n0<<4 | n1;
}


// ******************************************
unsigned char Midi::byteToUpperNibble(unsigned char n)
// ******************************************
{
    return (n >> 4) & 0x0F;
}


// ******************************************
unsigned char Midi::byteToLowerNibble(unsigned char n)
// ******************************************
{
    return n & 0x0F;
}


// ******************************************
unsigned char Midi::getCommand(const Message *message)
// ******************************************
{
    if (message->size() < 6) {
        return 0;
    }
    return message->at(6);
}


// ******************************************
unsigned char Midi::getArgument(const Message *message)
// ******************************************
{
    if (message->size() < 7) {
        return 0;
    }
    return message->at(7);
}


// ******************************************
bool Midi::parseSysex(const std::vector<unsigned char> *message, std::vector<unsigned char> *data)
// ******************************************
{
    const unsigned int size = message->size();

    if (!checkSysexHeadFoot(message)) {
        return false;
    }

    for (unsigned int i = 8; i < size - 3; i += 2) {
        const unsigned char temp = nibbleToByte(message->at(i), message->at(i+1));
        data->push_back(temp);
    }

    const unsigned char checksum = nibbleToByte(message->at(size-3), message->at(size-2));
    const unsigned char calculated_checksum = calculateChecksum(data);

    return checksum == calculated_checksum;
}


// ******************************************
void Midi::generateSysex(const Message *payload, const int command, const int argument, Message *message)
// ******************************************
{
    const unsigned int size = payload->size();
    message->reserve(2 * size + 11);


    for (unsigned int i=0; i<6; i++) {
        message->push_back(sysexHead[i]);
    }

    message->push_back(command);
    message->push_back(argument);
    const unsigned char checksum = calculateChecksum(payload);

    // expand bytes to nibbles:
    for (unsigned int i = 0; i < size; i++) {
        message->push_back((payload->at(i)>>4)&0x0F);
        message->push_back(payload->at(i)&0x0F);
    }
    message->push_back((checksum>>4)&0x0F);
    message->push_back(checksum&0x0F);

    message->push_back(sysexFoot);
}

