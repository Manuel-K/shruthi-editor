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
    opened=false;
    output=-1;
}

// ******************************************
bool MidiOut::open(int channel) {
// ******************************************
    if (output==channel && opened)
        return true;
    if (opened) 
        Pm_Close(midiout);
    opened = (Pm_OpenOutput(&midiout,channel,NULL,0,NULL,NULL,10)>=0);
    if (opened)
        output=channel;
    else
        qWarning() << "MidiOut::open(): could not open midi device for writing.";
    return opened;
}

// ******************************************
void MidiOut::close() {
// ******************************************
    if (opened) {
        Pm_Close(midiout);
        opened=false;
    }
}

// ******************************************
void MidiOut::writeSysex(unsigned char sysex[]) {
// ******************************************
    if (opened)
        Pm_WriteSysEx(midiout,0,sysex);
    else
        qDebug() << "MidiOut::writeNRPN(): could not send.";
}

// ******************************************
void MidiOut::writeNRPN(int nrpn, int value) {
// ******************************************
    if (opened) {
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
//         std::cout << "nrpn msb: " << nrpn_msb << ", nrpn lsb: " << nrpn_lsb <<", value msb: " << value_msb << ", value lsb:" << value_lsb << "\n";

        buffer[0].message = Pm_Message(176, 99, nrpn_msb);
        buffer[1].message = Pm_Message(176, 98, nrpn_lsb);
        buffer[2].message = Pm_Message(176, 6, value_msb);
        buffer[3].message = Pm_Message(176, 38, value_lsb);
        buffer[0].timestamp = 0;
        buffer[1].timestamp = 10;
        buffer[2].timestamp = 20;
        buffer[3].timestamp = 30;
        
        PmError rv=Pm_Write(midiout, buffer, 4);
        if (rv<0) {
            qDebug()<< "Pm_Write:"<<Pm_GetErrorText(rv);
        }
        if (Pm_HasHostError(midiout)) {
            char error[128];
            Pm_GetHostErrorText(error,128);
            qWarning() << "Pm_GetHostErrorText():"<<error; 
        }
    } else
        qDebug() << "MidiOut::writeNRPN(): could not send.";
}
