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

#include <vector>

// ******************************************
class Sequence {
// ******************************************

public:
    Sequence();
    ~Sequence();
    void unpackData(const unsigned char *data);
    void generateSysex(std::vector<unsigned char> *message);
    void reset();

private:
    bool parseSysex(const std::vector<unsigned char> *message);
    void packData(unsigned char data[]);
    void print();

    void setParam(int param, int val);
    int getParam(int param);

    int active[16];
    int note[16];
    int type[16];
    int velocity[16];
    int value[16];
    static unsigned char INIT_SEQUENCE[];

};

#endif // SHRUTHI_SEQUENCE_H
