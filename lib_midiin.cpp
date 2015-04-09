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

#include "lib_midiin.h"
#include "lib_midi.h"
#include <QDebug>


// ******************************************
NRPN::NRPN () {
// ******************************************
    nrpnMsb=0;
    nrpn=-1;
    valueMsb=0;
    value=-1;
}


// ******************************************
int NRPN::getValue(){
// ******************************************
    return value;
}


// ******************************************
int NRPN::getNRPN(){
// ******************************************
    return nrpn;
}


// Format:
//    176 99 0 (NRPN MSB set to 0)
//    176 98 2 (NRPN LSB set to 2, from the table below: Oscillator 1 range)
//    176 6 1 (Data Entry MSB set to 1 -- value above 127 or negative)
//    176 38 116 (Data Entry LSB set to 116, because 116 - 128 = -12)
// ******************************************
bool NRPN::parse(int b0, int b1, int b2) {
// ******************************************
    if (b0==176) switch(b1) {
        case 38://DATA_LSB
            value=b2 | (valueMsb<<7);
            valueMsb=0;
            return (nrpn>=0 && nrpn <= 107);
        case 6://DATA_MSB
            valueMsb=b2;
            return false;
        case 98://NRPN_LSB
            nrpn=b2 | (nrpnMsb << 7);
            nrpnMsb=0; 
            return false;
        case 99: //NRPN_MSB
           nrpnMsb=b2;
           return false;
        default:
#ifdef DEBUG
           qDebug() << "NRPN_Parser: Recieved unknown message:"<<b0<<","<<b1<<","<<b2;
#endif
           break;
    } 
#ifdef DEBUG
    qDebug()<< "NRPN_Parser: Recieved unknown message:"<<b0<<","<<b1<<","<<b2;
#endif
    return false;
}


// ******************************************
void MidiIn::process ( std::vector< unsigned char > *message ) {
// ******************************************
    int size = message->size();

    if (size>=4) {
        if (Midi::checkSysexHeadFoot(message)) {
            unsigned char *msg = new unsigned char[size];
            for (int i=0; i<size;i++)
                msg[i]= message->at(i);
            queueitem_t signal (SYSEX_RECIEVED,msg,size);
            signal.message=msg;
            emit enqueue(signal);
        }
    } else if (size>=3) { 
        if (nrpn.parse((int)message->at(0),(int)message->at(1),(int)message->at(2))) {
            queueitem_t signal (NRPN_RECIEVED,nrpn.getNRPN(),nrpn.getValue());
            emit enqueue(signal);
        }
         
    }
}


// ******************************************
void mycallback( double deltatime, std::vector< unsigned char > *message, void *userData ) {
// ******************************************
  ((MidiIn*) userData) -> process(message);
}


// ******************************************
MidiIn::MidiIn() {
// ******************************************
#ifdef DEBUG
    qDebug() << "MidiIn::MidiIn()";
#endif
    opened = false;
    input = -1;
    
    try {
        midiin = new RtMidiIn();
        midiin->setCallback(&mycallback,this);
    }
    catch ( RtMidiError &error ) {
        error.printMessage();
        qWarning() << "MidiOut::MidiIn(): could not initilize midi port for writing.";
//         exit( EXIT_FAILURE );
    }
}


// ******************************************
MidiIn::~MidiIn() {
// ******************************************
#ifdef DEBUG
    qDebug() << "MidiIn::~MidiIn()";
#endif
    delete midiin;
}


// ******************************************
void MidiIn::setMidiInputPort(int in) {
// ******************************************
#ifdef DEBUG
    qDebug() << "MidiIn::setMidiPorts";
#endif
    open(in);
    emit midiInputStatusChanged(opened);
}


// ******************************************
bool MidiIn::open(unsigned int port) {
// ******************************************
#ifdef DEBUG
    qDebug() << "MidiIn::open("<<port<<")";
#endif
    if (input==port && opened)
        return true;

    if (opened) {
        midiin->closePort();
    }
    
    if (port >= midiin->getPortCount() ) {
        qWarning() << "MidiIn::open(): trying to open midi port for reading which doesn't exist.";
        opened = false;
        return false;
    } 
    try {
        midiin->openPort( port );
        midiin->ignoreTypes( false, true, true );
        opened = true;
    }
    catch ( RtMidiError &error ) {
#ifdef DEBUG
        qDebug() << "MidiIn::open("<<port<<"): RtMidiError on openPort().";
#endif
        opened = false;
    }
    if (opened)
        input = port;
    else
        qWarning() << "MidiIn::open(): could not open midi port for reading.";
    return opened;
}
