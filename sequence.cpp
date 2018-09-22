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


#include "sequence.h"
#include <QDebug>
#include <QStringList>
#include <iostream>
#include "labels.h"
#include "midi.h"


Sequence::Sequence() {
    reset();
}


const int Sequence::ERROR_RETURN = -111;


bool Sequence::parseSysex(const Message *message) {
    // len should be 75
    if (message->size() != 75) {
        qDebug() << "Sequence::parseFullSysex(): wrong length.";
        return false;
    }

    Message payload;
    if (!Midi::parseSysex(message, &payload)) {
        return false;
    }

    // copy to temporary array:
    unsigned char temp[payload.size()];
    for (unsigned int i = 0; i < payload.size(); i++) {
        temp[i] = payload.at(i);
    }

    unpackData(temp);
    return true;
}


void Sequence::generateSysex(Message *message) const {
    unsigned char temp[32];
    packData(temp);

    Message payload;
    for (unsigned int i = 0; i < 32; i++) {
        payload.push_back(temp[i]);
    }

    Midi::generateSysex(&payload, 0x02, 0x00, message);
}


const int &Sequence::getValue(const int &step, const SequenceParameter::SequenceParameter &sp) const {
    if (step < 0 || step >= NUMBER_OF_STEPS) {
        return ERROR_RETURN;
    }

    switch (sp) {
        case SequenceParameter::ACTIVE:
            return mActive[step];
        case SequenceParameter::NOTE:
            return mNote[step];
        case SequenceParameter::TIE:
            return mTie[step];
        case SequenceParameter::VALUE:
            return mValue[step];
        case SequenceParameter::VELOCITY:
            return mVelocity[step];
    }
    return ERROR_RETURN;
}


void Sequence::setValue(const int &step, const SequenceParameter::SequenceParameter &sp, const int &val) {
    if (step < 0 || step >= NUMBER_OF_STEPS) {
        return;
    }

    switch (sp) {
        case SequenceParameter::ACTIVE:
            mActive[step] = val;
            break;
        case SequenceParameter::NOTE:
            mNote[step] = val;
            break;
        case SequenceParameter::TIE:
            mTie[step] = val;
            break;
        case SequenceParameter::VALUE:
            mValue[step] = val;
            break;
        case SequenceParameter::VELOCITY:
            mVelocity[step] = val;
            break;
    }
}


void Sequence::unpackData(const unsigned char *data) {
    for (unsigned int i = 0; i < 16; i++) {
        mActive[i] = data[2*i] >> 7; //1==on or 0==silence
        mNote[i] = data[2*i] & 0x7F; // 0..127
        mTie[i] = data[2*i+1] >> 7; //0==note or 1==tie/slide
        mVelocity[i] = (data[2*i+1] & 0x70) >> 4; // 0..7
        mValue[i] = data[2*i+1] & 0x0F; // 0..F
    }
#ifdef DEBUGMSGS
    print();
#endif
}


void Sequence::packData(unsigned char data[]) const {
    for (unsigned int i = 0; i < 16; i++) {
        data[2*i] = mActive[i] << 7 | mNote[i];
        data[2*i+1] = mTie[i] << 7 | mVelocity[i] << 4 | mValue[i];
    }
}


unsigned char Sequence::INIT_SEQUENCE[] = {
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


void Sequence::reset() {
    unpackData(INIT_SEQUENCE);
}


bool Sequence::equals(const Sequence &other) const {
    for (int s = 0; s < NUMBER_OF_STEPS; s++) {
        if(mActive[s] != other.mActive[s] ||
                mNote[s] != other.mNote[s] ||
                mTie[s] != other.mTie[s] ||
                mValue[s] != other.mValue[s] ||
                mVelocity[s] != other.mVelocity[s]) {
            return false;
        }
    }

    return true;
}


void Sequence::set(const Sequence &other) {
    for (int s = 0; s < NUMBER_OF_STEPS; s++) {
        mActive[s] = other.mActive[s];
        mNote[s] = other.mNote[s];
        mTie[s] = other.mTie[s];
        mValue[s] = other.mValue[s];
        mVelocity[s] = other.mVelocity[s];
    }
}


void Sequence::print() const {
    QString typestr;
    for (unsigned int i = 0; i < 16; i++) {
        if (mActive[i]) {
            if (mTie[i]) {
                typestr = "-";
            } else {
                typestr = "♪";
            }
        } else {
            typestr = " ";
        }

        std::cout << (QString("%1").arg(i, 2)).toUtf8().constData() << ": " << Labels::NoteFormatter(mNote[i]).toUtf8().constData() << " " << typestr.toUtf8().constData() << mVelocity[i] << " " << Labels::HexValues.at(mValue[i]).toUtf8().constData() << std::endl;
    }
}


void Sequence::setValueById(const int &id, const int &val) {
    if (id < 0 || id >= 80) {
        return;
    }

    const int &step = id / 5;
    switch(id % 5) {
        case 1: // note
            setValue(step, SequenceParameter::NOTE, val);
            break;
        case 2: // tie
            setValue(step, SequenceParameter::TIE, val);
            break;
        case 3: // velocity
            setValue(step, SequenceParameter::VELOCITY, val);
            break;
        case 4: // value
            setValue(step, SequenceParameter::VALUE, val);
            break;
        default: // 0:active
            setValue(step, SequenceParameter::ACTIVE, val);
            break;
    }
}


int Sequence::calculateParamId(const int &step, const SequenceParameter::SequenceParameter &sp) {
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
    }
    return ERROR_RETURN;
}
