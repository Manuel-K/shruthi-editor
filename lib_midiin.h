// Shruthi-Editor: An unofficial Editor for the Shruthi hardware synthesizer. For 
// informations about the Shruthi, see <http://www.mutable-instruments.net/shruthi1>. 
//
// Copyright (C) 2011 Manuel Krönig
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

#ifndef SHRUTHI_MIDIIN_H
#define SHRUTHI_MIDIIN_H

#include <QObject>
#include "lib_editor.h" // queueitem_t definition
#include "RtMidi.h"


// ******************************************
class NRPN {
// ******************************************
    private:
        int nrpnMsb;
        int nrpn;
        int valueMsb;
        int value;
    public:
        NRPN();
        int getValue();
        int getNRPN();
        bool parse(int,int,int);
};

// ******************************************
class MidiIn : public QObject {
// ******************************************
    Q_OBJECT
    private:
        NRPN nrpn;
        
        RtMidiIn* midiin;
        bool opened;
        unsigned int input;

        bool open(unsigned int);
        
    public:
        ~MidiIn();
        MidiIn();
        void process(std::vector< unsigned char > *message);
    public slots:
        void setMidiInputPort(int);
    signals:
        void enqueue(queueitem_t);
        void midiInputStatusChanged(bool);
};


#endif