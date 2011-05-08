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
#include <QDebug>

// ******************************************
parserNRPN::parserNRPN () {
// ******************************************
    nrpnMsb=0;
    nrpn=-1;
    valueMsb=0;
    value=-1;
}

// ******************************************
int parserNRPN::getValue(){
// ******************************************
    return value;
}
// ******************************************
int parserNRPN::getNRPN(){
// ******************************************
    return nrpn;
}


// Format:
//    176 99 0 (NRPN MSB set to 0)
//    176 98 2 (NRPN LSB set to 2, from the table below: Oscillator 1 range)
//    176 6 1 (Data Entry MSB set to 1 -- value above 127 or negative)
//    176 38 116 (Data Entry LSB set to 116, because 116 - 128 = -12)
// ******************************************
bool parserNRPN::parse(int b0, int b1, int b2) {
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
parserSysex::parserSysex () {
// ******************************************
    recieving=false;
    pos=0;
}

// ******************************************
bool parserSysex::isRecieving() {
// ******************************************
    return recieving;
}
// ******************************************
void parserSysex::setRecieving() {
// ******************************************
    recieving=true;
    pos=0;
}

// ******************************************
bool parserSysex::parse(int b0, int b1, int b2, int b3) {
// ******************************************
    buffer[pos++]=b0;
    if(b0!=247)
        buffer[pos++]=b1;
    if(b1!=247)
        buffer[pos++]=b2;
    if(b2!=247)
        buffer[pos++]=b3;
    if (b0==247||b1==247||b2==247||b3==247)  // end of sysex message
        recieving=false;
    return !recieving;
}
// ******************************************
int parserSysex::getSysex(int* sysex) {
// ******************************************
    for (int i=0; i<pos; i++)
        sysex[i]=buffer[i];
    return pos;
}

// ******************************************
int parserSysex::getLen() {
// ******************************************
    return pos;
}

// ******************************************
void MidiIn::listen() {
// ******************************************
    if (!opened || !Pm_Poll(midiin)) 
        return;
    Pm_Read( midiin, buffer, 1 );
    int b0 = (buffer->message & 0xFF);
    int b1 = (buffer->message >> 8)& 0xFF;
    int b2 = (buffer->message >> 16)& 0xFF;
    int b3 = (buffer->message >> 24);
    
    if (b0==240 && b1==0 && b2==32 && b3==119) {//SYSEX_HEAD
        sysex.setRecieving();
#ifdef DEBUG
        qDebug() << "detected start of sysex message";
#endif
    }
    if (sysex.isRecieving()){
        if (sysex.parse(b0,b1,b2,b3)) {
            int len = sysex.getLen();
            int *msg = new int[len];
            sysex.getSysex(msg);
            queueitem_t signal = {SYSEX_RECIEVED,len,0,NULL,NULL};
            signal.intp=msg;
            emit enqueue(signal);
        }
    } else {
        if (nrpn.parse(b0,b1,b2)) {
            queueitem_t signal = {NRPN_RECIEVED,nrpn.getNRPN(),nrpn.getValue(),NULL,NULL};
            emit enqueue(signal);
        }
    }
    QTimer::singleShot(0, this, SLOT(listen()));
}

// ******************************************
MidiIn::~MidiIn() {
// ******************************************
#ifdef DEBUG
    qDebug() << "MidiIn::~MidiIn()";
#endif
    timer->stop();
    delete timer;
    if (opened)
        Pm_Close(midiin);
}
// ******************************************
MidiIn::MidiIn() {
// ******************************************
#ifdef DEBUG
    qDebug() << "MidiIn::MidiIn()";
#endif
    opened = false;
    timer = new QTimer(this);
    input = -1;
    connect(timer, SIGNAL(timeout()), this, SLOT(listen()));
}

// ******************************************
void MidiIn::setMidiDevices(int in, int out) {
// ******************************************
#ifdef DEBUG
    qDebug() << "MidiIn::setMidiDevices";
#endif
    if (in==input && opened)
        return;
    
    if (opened) {
        timer->stop();
        opened=false;
        Pm_Close(midiin);
    }
    opened = (Pm_OpenInput(&midiin,in,NULL,0,NULL,NULL)>=0);
    if (opened) {
        timer->start(100);
        input = in;
    } else {
        qWarning() << "MidiIn::setMidiDevices(): could not open midi device for writing.";
    }
    emit midiInputStatusChanged(opened);
}
