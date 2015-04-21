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

#include "lib_sequence.h"
#include "lib_midi.h"
#include "lib_labels.h"
#include <iostream>
#include <QDebug>


// ******************************************
Sequence::Sequence() {
// ******************************************
    reset();
}


// ******************************************
Sequence::~Sequence() {
// ******************************************

}


// ******************************************
bool Sequence::parseSysex(const std::vector<unsigned char> *message) {
// ******************************************
    // len should be 75
    if (message->size() != 75) {
        qDebug() << "Sequence::parseFullSysex(): wrong length.";
        return false;
    }

    Message payload;
    if (!Midi::parseSysex(message, &payload)) {
        return false;
    }

    // copy to temporay array:
    unsigned char temp[payload.size()];
    for (unsigned int i = 0; i < payload.size(); i++) {
        temp[i] = payload.at(i);
    }

    unpackData(temp);
    return true;
}


// ******************************************
void Sequence::generateSysex(std::vector<unsigned char> *message) {
// ******************************************
    unsigned char temp[32];
    packData(temp);

    Message payload;
    for (unsigned int i = 0; i < 32; i++) {
        payload.push_back(temp[i]);
    }

    Midi::generateSysex(&payload, 0x02, 0x00, message);
}


// ******************************************
void Sequence::unpackData(const unsigned char *data) {
// ******************************************
    for (unsigned int i = 0; i < 16; i++) {
        active[i] = data[2*i] >> 7; //1==on or 0==silence
        note[i] = data[2*i] & 0x7F; // 0..127
        type[i] = data[2*i+1] >> 7; //0==note or 1==tie/slide
        velocity[i] = (data[2*i+1] & 0x70) >> 4; // 0..7
        value[i] = data[2*i+1] & 0x0F; // 0..F
    }
    print();
}


// ******************************************
void Sequence::packData(unsigned char data[]) {
// ******************************************
    for (unsigned int i = 0; i < 16; i++) {
        data[2*i] = active[i] << 7 | note[i];
        data[2*i+1] = type[i] << 7 | velocity[i] << 4 | value[i];
    }
}


// ******************************************
unsigned char Sequence::INIT_SEQUENCE[] = {
// ******************************************
    0x80 | 48, 0x00 | 0x70 | 0x0,
    0x80 | 48, 0x80 | 0x50 | 0x0,
    0x80 | 60, 0x00 | 0x50 | 0x0,
    0x80 | 60, 0x80 | 0x50 | 0x0,
    0x80 | 48, 0x00 | 0x70 | 0xf,
    0x80 | 48, 0x80 | 0x50 | 0xf,
    0x80 | 60, 0x00 | 0x50 | 0xf,
    0x00 | 60, 0x00 | 0x50 | 0xf,
    0x80 | 48, 0x00 | 0x70 | 0xc,
    0x80 | 48, 0x80 | 0x50 | 0xc,
    0x80 | 60, 0x00 | 0x50 | 0xc,
    0x80 | 60, 0x80 | 0x50 | 0xc,
    0x80 | 48, 0x00 | 0x70 | 0x4,
    0x00 | 48, 0x00 | 0x50 | 0x4,
    0x80 | 60, 0x00 | 0x70 | 0x4,
    0x00 | 60, 0x00 | 0x50 | 0x4
};


// ******************************************
void Sequence::reset() {
// ******************************************
    unpackData(INIT_SEQUENCE);
}


// ******************************************
void Sequence::print() {
// ******************************************
    QString typestr;
    for (unsigned int i = 0; i < 16; i++) {
        if (active[i]) {
            if (type[i]) {
                typestr = "-";
            } else {
                typestr = "♪";
            }
        } else {
            typestr = " ";
        }

        //std::cout << i << ": " << active[i] << " " << note[i] << " " << type[i] << " " << velocity[i] << " " << value[i] << std::endl;
        std::cout << (QString("%1").arg(i, 2)).toUtf8().constData() << ": " << Labels::NoteFormatter(note[i]).toUtf8().constData() << " " << typestr.toUtf8().constData()  << velocity[i]  << " " << Labels::HexValues.at(value[i]).toUtf8().constData() << std::endl;
    }
}


// ******************************************
void Sequence::setParam(int param, int val) {
// ******************************************
    if (param < 0 || param >= 80) {
        return;
    }

    switch(param % 5) {
    case 1: // note
        note[param / 5] = val;
        break;
    case 2: // type
        type[param / 5] = val;
        break;
    case 3: // velocity
        velocity[param / 5] = val;
        break;
    case 4: // value
        value[param / 5] = val;
        break;
    default: // 0:active
        active[param / 5] = val;
        break;
    }
}


// ******************************************
int Sequence::getParam(int param) {
// ******************************************
    if (param < 0 || param >= 80) {
        return 0;
    }

    switch(param % 5) {
    case 1: // note
        return note[param / 5];
    case 2: // type
        return type[param / 5];
    case 3: // velocity
        return velocity[param / 5];
    case 4: // value
        return value[param / 5];
    default: // 0:active
        return active[param / 5];
    }
}

