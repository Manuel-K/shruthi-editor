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


#include "midiin.h"
#include <QDebug>
#include <stddef.h> // for NULL
#include <iostream>
#include <string>
#include "RtMidi.h"
#include "midi.h"
#include "patch.h"


//
// NRPN
//


NRPN::NRPN() {
    nrpnMsb = 0;
    nrpn = -1;
    valueMsb = 0;
    value=-1;
}


int NRPN::getValue(){
    return value;
}


int NRPN::getNRPN(){
    return nrpn;
}


bool MidiIn::isNRPN(const unsigned char &n0, const unsigned char &n1) {
    return n0 == 176 && (n1 == 6 || n1 == 38 || n1 == 98 || n1 == 99);
}


// Format:
//    176 99 0 (NRPN MSB set to 0)
//    176 98 2 (NRPN LSB set to 2, from the table below: Oscillator 1 range)
//    176 6 1 (Data Entry MSB set to 1 -- value above 127 or negative)
//    176 38 116 (Data Entry LSB set to 116, because 116 - 128 = -12)
bool NRPN::parse(int b0, int b1, int b2) {
    if (b0 == 176) {
        switch(b1) {
            case 38: //DATA_LSB
                value = b2 | (valueMsb << 7);
                valueMsb = 0;
                return (nrpn >= 0 && nrpn <= 107);
            case 6: //DATA_MSB
                valueMsb = b2;
                return false;
            case 98: //NRPN_LSB
                nrpn = b2 | (nrpnMsb << 7);
                nrpnMsb = 0;
                return false;
            case 99: //NRPN_MSB
                nrpnMsb = b2;
                return false;
            default:
                break;
        }
    }
#ifdef DEBUGMSGS
    qDebug() << "NRPN_Parser: Received unknown message:" << b0 << "," << b1 << "," << b2;
#endif
    return false;
}


//
// MidiIn
//


void MidiIn::process(const Message *message) {
    int size = message->size();

    if (size >= 4) {
        Message payload;
        if (!Midi::parseSysex(message, &payload)) {
            // We have a problem! Do something. Warn the user or make a cup of tea.
            QueueItem signal(QueueAction::SYSEX_RECEIVED);
            signal.int0 = Midi::getCommand(message);
            signal.int1 = Midi::getArgument(message);
            signal.size = 0;
            signal.message = NULL;
            emit enqueue(signal);
            return;
        }

        // version info has a SysEx size of 15/2 bytes payload
        if (payload.size() == 2 && Midi::getCommand(message) == 0x0c && Midi::getArgument(message) == 0x00) {
            firmwareVersion = payload.at(0) * 1000 + payload.at(1);
#ifdef DEBUGMSGS
            std::cout << "Firmware version: " << firmwareVersion << std::endl;
#endif
        }

        unsigned char *msg = new unsigned char[payload.size()];
        for (unsigned int i=0; i < payload.size(); i++) {
            msg[i] = payload.at(i);
        }

        QueueItem signal(QueueAction::SYSEX_RECEIVED);
        signal.int0 = Midi::getCommand(message);
        signal.int1 = Midi::getArgument(message);
        signal.message = msg;
        signal.size = payload.size();
        emit enqueue(signal);
    } else if (size == 3) {
        if (isNRPN(message->at(0), message->at(1))) { // might want to check if firmwareVersion < 1000
            // Parse as NRPN
            if (nrpn.parse((int)message->at(0),(int)message->at(1),(int)message->at(2))) {
                QueueItem signal(QueueAction::PATCH_PARAMETER_CHANGE_MIDI, nrpn.getNRPN(), nrpn.getValue());
                emit enqueue(signal);
            }
        } else {
            int id = Patch::ccToId(message->at(1), shruthiFilterBoard);
            int value = Patch::convertCCValue(message->at(2), id, shruthiFilterBoard);
            if (!warnedCC) {
                if (id == 25  || id == 29) {
                    std::cout << "Received LFO Rate per CC. That's a bad idea...\nFurther warnings will be suppressed."
                              << std::endl;
                    warnedCC = true;
                }
            }
            QueueItem signal(QueueAction::PATCH_PARAMETER_CHANGE_MIDI, id, value);
            emit enqueue(signal);
        }
    }
}


void mycallback(double deltatime, Message *message, void *userData) {
    Q_UNUSED(deltatime);
    ((MidiIn*)userData)->process(message);
    // Don't try to delete the message!
}


MidiIn::MidiIn() {
#ifdef DEBUGMSGS
    qDebug() << "MidiIn::MidiIn()";
#endif
    opened = false;
    input = -1;
    firmwareVersion = 0;
    initialized = false;

    warnedCC = false;

    shruthiFilterBoard = 0;

    try {
        midiin = new RtMidiIn(RtMidi::UNSPECIFIED, "shruthi-editor");
        midiin->setCallback(&mycallback,this);
        initialized = true;
    }
    catch (RtMidiError &error) {
        error.printMessage();
        qWarning() << "MidiIn::MidiIn(): could not initilize midi port for reading.";
    }
}


MidiIn::~MidiIn() {
#ifdef DEBUGMSGS
    qDebug() << "MidiIn::~MidiIn()";
#endif
    if (initialized) {
        initialized = false;
        delete midiin;
    }
}


void MidiIn::setMidiInputPort(int in) {
#ifdef DEBUGMSGS
    qDebug() << "MidiIn::setMidiPorts";
#endif
    open(in);
    emit midiInputStatusChanged(opened);
}


void MidiIn::setShruthiFilterBoard(int filter) {
#ifdef DEBUGMSGS
    qDebug() << "MidiIn::setShruthiFilterBoard:" << filter;
#endif
    MidiIn::shruthiFilterBoard = filter;
}


bool MidiIn::open(const unsigned int &port) {
#ifdef DEBUGMSGS
    qDebug() << "MidiIn::open(" << port << ")";
#endif
    if (!initialized) {
        qDebug() << "MidiIn::open(): Can't open Midi port, RtMidi was not initialized.";
        return false;
    }

    if (input == port && opened) {
        return true;
    }

    if (opened) {
        midiin->closePort();
    }

    firmwareVersion = 0;

    if (port >= midiin->getPortCount()) {
        qWarning() << "MidiIn::open(): trying to open midi port for reading which doesn't exist.";
        opened = false;
        return false;
    }
    try {
        midiin->openPort(port, "In");
        midiin->ignoreTypes(false, true, true);
        opened = true;
    }
    catch (RtMidiError &error) {
#ifdef DEBUGMSGS
        qDebug() << "MidiIn::open(" << port << "): RtMidiError on openPort().";
#endif
        opened = false;
    }
    if (opened) {
        input = port;
    } else {
        qWarning() << "MidiIn::open(): could not open midi port for reading.";
    }
    return opened;
}
