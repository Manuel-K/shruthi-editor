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

#include "lib_midiout.h"
#include <QDebug>

// ******************************************
MidiOut::MidiOut() {
// ******************************************
#ifdef DEBUG
    qDebug() << "MidiOut::MidiOut()";
#endif
    opened=false;
    output=-1;
    try {
        midiout = new RtMidiOut();
    }
    catch ( RtError &error ) {
        error.printMessage();
        qWarning() << "MidiOut::MidiOut(): could not initilize midi device for writing.";
//         exit( EXIT_FAILURE );
    }
}


// ******************************************
MidiOut::~MidiOut() {
// ******************************************
#ifdef DEBUG
    qDebug() << "MidiOut::~MidiOut()";
#endif
    delete midiout;
}


// ******************************************
bool MidiOut::open(unsigned int port) {
// ******************************************
#ifdef DEBUG
    qDebug() << "MidiOut::open("<<port<<")";
#endif
    if (output==port && opened)
        return true;

    if (opened)
        midiout->closePort();

    if (port >= midiout->getPortCount() ) {
        qWarning() << "MidiOut::open(): trying to open midi device for writing which doesn't exist.";
        opened = false;
        return false;
    } 
    try {
        midiout->openPort( port );
        opened = true;
    }
    catch ( RtError &error ) {
#ifdef DEBUG
        qDebug() << "MidiOut::open("<<port<<"): RtError on openPort().";
#endif
        opened = false;
    }
    if (opened)
        output = port;
    else
        qWarning() << "MidiOut::open(): could not open midi device for writing.";
    return opened;
}


// ******************************************
void MidiOut::write(unsigned char sysex[]) {
// ******************************************
    if (!opened) {
        qDebug() << "MidiOut::write(unsigned char[]): could not send. Port not opened.";
        return;
    }
    
    std::vector<unsigned char> message;
    int i;
    for (i=0; sysex[i]!=247; i++)
        message.push_back(sysex[i]);
    message.push_back(sysex[i]);
    write(message);
}


// ******************************************
void MidiOut::write(std::vector<unsigned char> message) {
// ******************************************
    if (!opened) {
        qDebug() << "MidiOut::write(std::vector<unsigned char>): could not send. Port not opened.";
        return;
    }
    
    try {
        midiout->sendMessage (&message);
    }
    catch ( RtError &error ) {
        qDebug() << "MidiOut::write(std::vector<unsigned char>): could not send. Error on sending.";
        error.printMessage();
    }
}


// ******************************************
void MidiOut::write(unsigned char c1,unsigned char c2,unsigned char c3) {
// ******************************************
    std::vector<unsigned char> message;
    message.push_back( c1 );
    message.push_back( c2 );
    message.push_back( c3 );
    write( message );
}

// ******************************************
void MidiOut::writeNRPN(int nrpn, int value) {
// ******************************************
    if (!opened) {
        qDebug() << "MidiOut::writeNRPN(): could not send. Port not opened.";
        return;
    }
      
    int32_t nrpn_msb=nrpn>>7;
    int32_t nrpn_lsb=nrpn%128;
    int32_t value_msb;
    int32_t value_lsb;
    if (value<0) {
      value_msb=1;
      value_lsb=(value+128)%128;
    } else {
      value_msb=value>>7;
      value_lsb=value%128;
    }
    write(176, 99, nrpn_msb);
    write(176, 98, nrpn_lsb);
    write(176, 6, value_msb);
    write(176, 38, value_lsb);
}
