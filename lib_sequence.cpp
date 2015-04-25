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
const int Sequence::ERROR_RETURN = -111;
// ******************************************


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
const int &Sequence::getParam(const int &step, const SequenceParameter::SequenceParameter &sp) {
// ******************************************
    if (step < 0 || step >= NUMBER_OF_STEPS) {
        return ERROR_RETURN;
    }

    switch (sp) {
    case SequenceParameter::ACTIVE:
        return stepsActive[step];
    case SequenceParameter::NOTE:
        return stepsNote[step];
    case SequenceParameter::TIE:
        return stepsTie[step];
    case SequenceParameter::VALUE:
        return stepsValue[step];
    case SequenceParameter::VELOCITY:
        return stepsVelocity[step];
    default:
        return ERROR_RETURN;
    }
}


// ******************************************
void Sequence::unpackData(const unsigned char *data) {
// ******************************************
    for (unsigned int i = 0; i < 16; i++) {
        stepsActive[i] = data[2*i] >> 7; //1==on or 0==silence
        stepsNote[i] = data[2*i] & 0x7F; // 0..127
        stepsTie[i] = data[2*i+1] >> 7; //0==note or 1==tie/slide
        stepsVelocity[i] = (data[2*i+1] & 0x70) >> 4; // 0..7
        stepsValue[i] = data[2*i+1] & 0x0F; // 0..F
    }
#ifdef DEBUGMSGS
    print();
#endif
}


// ******************************************
void Sequence::packData(unsigned char data[]) {
// ******************************************
    for (unsigned int i = 0; i < 16; i++) {
        data[2*i] = stepsActive[i] << 7 | stepsNote[i];
        data[2*i+1] = stepsTie[i] << 7 | stepsVelocity[i] << 4 | stepsValue[i];
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
const int &Sequence::getActive(const int &step) {
// ******************************************
    if (step < 0 || step >= NUMBER_OF_STEPS) {
        return ERROR_RETURN;
    }
    return stepsActive[step];
}


// ******************************************
const int &Sequence::getNote(const int &step) {
// ******************************************
    if (step < 0 || step >= NUMBER_OF_STEPS) {
        return ERROR_RETURN;
    }
    return stepsNote[step];
}


// ******************************************
const int &Sequence::getTie(const int &step) {
// ******************************************
    if (step < 0 || step >= NUMBER_OF_STEPS) {
        return ERROR_RETURN;
    }
    return stepsTie[step];

}


// ******************************************
const int &Sequence::getVelocity(const int &step) {
// ******************************************
    if (step < 0 || step >= NUMBER_OF_STEPS) {
        return ERROR_RETURN;
    }
    return stepsVelocity[step];
}


// ******************************************
const int &Sequence::getValue(const int &step) {
// ******************************************
    if (step < 0 || step >= NUMBER_OF_STEPS) {
        return ERROR_RETURN;
    }
    return stepsValue[step];
}




// ******************************************
void Sequence::print() {
// ******************************************
    QString typestr;
    for (unsigned int i = 0; i < 16; i++) {
        if (stepsActive[i]) {
            if (stepsTie[i]) {
                typestr = "-";
            } else {
                typestr = "♪";
            }
        } else {
            typestr = " ";
        }

        //std::cout << i << ": " << active[i] << " " << note[i] << " " << tie[i] << " " << velocity[i] << " " << value[i] << std::endl;
        std::cout << (QString("%1").arg(i, 2)).toUtf8().constData() << ": " << Labels::NoteFormatter(stepsNote[i]).toUtf8().constData() << " " << typestr.toUtf8().constData()  << stepsVelocity[i]  << " " << Labels::HexValues.at(stepsValue[i]).toUtf8().constData() << std::endl;
    }
}


// ******************************************
void Sequence::setParamById(const int &id, const int &val) {
// ******************************************
    if (id < 0 || id >= 80) {
        return;
    }

    switch(id % 5) {
    case 1: // note
        stepsNote[id / 5] = val;
        break;
    case 2: // tie
        stepsTie[id / 5] = val;
        break;
    case 3: // velocity
        stepsVelocity[id / 5] = val;
        break;
    case 4: // value
        stepsValue[id / 5] = val;
        break;
    default: // 0:active
        stepsActive[id / 5] = val;
        break;
    }
}


// ******************************************
int Sequence::calculateParamId(const int &step, const SequenceParameter::SequenceParameter &sp) {
// ******************************************
    if (step < 0 || step >= NUMBER_OF_STEPS) {
        return ERROR_RETURN;
    }

    switch(sp) {
    case SequenceParameter::ACTIVE:
        return 5 * step;
    case SequenceParameter::NOTE:
        return 5 * step + 1;
    case SequenceParameter::TIE:
        return 5 * step + 2;
    case SequenceParameter::VELOCITY:
        return 5 * step + 3;
    case SequenceParameter::VALUE:
        return 5 * step + 4;
    default:
        return ERROR_RETURN;
    }
}


// ******************************************
const int &Sequence::getParamByID(const int &id) {
// ******************************************
    if (id < 0 || id >= 80) {
        return ERROR_RETURN;
    }

    switch(id % 5) {
    case 1: // note
        return stepsNote[id / 5];
    case 2: // tie
        return stepsTie[id / 5];
    case 3: // velocity
        return stepsVelocity[id / 5];
    case 4: // value
        return stepsValue[id / 5];
    default: // 0:active
        return stepsActive[id / 5];
    }
}

