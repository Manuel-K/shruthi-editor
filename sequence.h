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

#ifndef SHRUTHI_SEQUENCE_H
#define SHRUTHI_SEQUENCE_H

#include "message.h"
#include "sequence_parameter.h"


// ******************************************
class Sequence {
// ******************************************

public:
    Sequence();
    ~Sequence();

    void unpackData(const unsigned char *data);
    void packData(unsigned char data[]) const;
    bool parseSysex(const Message *message);
    void generateSysex(Message *message) const;

    const int &getValueByID(const int &id) const;
    const int &getValue(const int &step, const SequenceParameter::SequenceParameter &sp) const;
    void setValueById(const int &id, const int &val);
    static int calculateParamId(const int &step, const SequenceParameter::SequenceParameter &sp);
    void reset();

    bool equals(const Sequence &other) const;
    void set(const Sequence &other);

    const int &getActive(const int &step) const;
    const int &getNote(const int &step) const;
    const int &getTie(const int &step) const;
    const int &getVelocity(const int &step) const;
    const int &getValue(const int &step) const;

    static const int NUMBER_OF_STEPS = 16;
    static const int ERROR_RETURN;

private:
    void print() const;

    int stepsActive[16];
    int stepsNote[16];
    int stepsTie[16];
    int stepsVelocity[16];
    int stepsValue[16];
    static unsigned char INIT_SEQUENCE[];
};

#endif // SHRUTHI_SEQUENCE_H
