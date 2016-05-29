// Shruthi-Editor: An unofficial Editor for the Shruthi hardware synthesizer. For
// informations about the Shruthi, see <http://www.mutable-instruments.net/shruthi1>.
//
// Copyright (C) 2011-2016 Manuel Krönig
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


#include "midiout.h"
#include <QDebug>
#include <string>
#include "RtMidi.h"
#include "midi.h"


MidiOut::MidiOut() {
#ifdef DEBUGMSGS
    qDebug() << "MidiOut::MidiOut()";
#endif
    opened=false;
    output=-1;
    initialized = false;
    try {
        midiout = new RtMidiOut(RtMidi::UNSPECIFIED, "shruthi-editor");
        initialized = true;
    }
    catch (RtMidiError &error) {
        error.printMessage();
        qWarning() << "MidiOut::MidiOut(): could not initilize midi port for writing.";
    }
}


MidiOut::~MidiOut() {
#ifdef DEBUGMSGS
    qDebug() << "MidiOut::~MidiOut()";
#endif
    if (initialized) {
        initialized = false;
        delete midiout;
    }
}


bool MidiOut::open(const unsigned int &port) {
#ifdef DEBUGMSGS
    qDebug() << "MidiOut::open(" << port << ")";
#endif
    if (!initialized) {
        qDebug() << "MidiOut::open(): Can't open Midi port, RtMidi was not initialized.";
        return false;
    }


    if (output==port && opened) {
        return true;
    }

    if (opened) {
        midiout->closePort();
    }

    if (port >= midiout->getPortCount()) {
        qWarning() << "MidiOut::open(): trying to open midi port for writing which doesn't exist.";
        opened = false;
        return false;
    }
    try {
        midiout->openPort(port, "Out");
        opened = true;
    }
    catch (RtMidiError &error) {
#ifdef DEBUGMSGS
        qDebug() << "MidiOut::open(" << port << "): RtMidiError on openPort().";
#endif
        opened = false;
    }
    if (opened) {
        output = port;
    } else {
        qWarning() << "MidiOut::open(): could not open midi port for writing.";
    }
    return opened;
}


bool MidiOut::write(Message &sysex) {
    if (!opened) {
        qDebug() << "MidiOut::write(Message&): could not send. Port not opened.";
        return false;
    }

    try {
        midiout->sendMessage(&sysex);
        return true;
    }
    catch (RtMidiError &error) {
        qDebug() << "MidiOut::write(Message&): could not send. Error on sending.";
        error.printMessage();
    }
    return false;
}


bool MidiOut::write(const unsigned char &c1, const unsigned char &c2, const unsigned char &c3) {
    Message message;
    message.push_back(c1);
    message.push_back(c2);
    message.push_back(c3);
    return write(message);
}


bool MidiOut::write(const unsigned char &c1, const unsigned char &c2) {
    Message message;
    message.push_back(c1);
    message.push_back(c2);
    return write(message);
}


//
// Wrappers:
//


bool MidiOut::request(const unsigned char &which) {
    Message message;

    for (int i = 0; i < 6; i++) {
        message.push_back(Midi::sysexHead[i]);
    }
    message.push_back(which);
    message.push_back(0);
    message.push_back(0);
    message.push_back(0);
    message.push_back(Midi::sysexFoot);

    return write(message);
}


bool MidiOut::nrpn(const unsigned char &channel, const int &nrpn, const int &value) {
    if (!opened) {
        qDebug() << "MidiOut::nrpn(): could not send. Port not opened.";
        return false;
    }

    int nrpn_msb = nrpn >> 7;
    int nrpn_lsb = nrpn % 128;
    int value_msb = 0;
    int value_lsb = 0;
    if (value < 0) {
        value_msb = 1;
        value_lsb = (value + 128) % 128;
    } else {
        value_msb = value >> 7;
        value_lsb = value % 128;
    }
    return (write(176 | channel, 99, nrpn_msb) &&
            write(176 | channel, 98, nrpn_lsb) &&
            write(176 | channel, 6, value_msb) &&
            write(176 | channel, 38, value_lsb));
}



bool MidiOut::noteOn(const unsigned char &channel, const unsigned char &note, const unsigned char &velocity) {
    return write((144 | channel), note, velocity);
}


bool MidiOut::noteOff(const unsigned char &channel, const unsigned char &note) {
    return write((128 | channel), note, 0);
}


bool MidiOut::allNotesOff(const unsigned char &channel) {
    return controlChange(channel, 123, 0);
}


bool MidiOut::programChange(const unsigned char &channel, const int &program) {
    const int &bank = program / 128;
    const int &p = program % 128;

    return controlChange(channel, 0, bank) && write((0xc0|channel), p);
}


bool MidiOut::programChangeSequence(const unsigned char &channel, const int &sequence) {
    const int &bank = sequence / 128 | 0x40;
    const int &p = sequence % 128;

    return controlChange(channel, 0, bank) && write((0xc0|channel), p);
}


bool MidiOut::controlChange(const unsigned char &channel, const unsigned char &controller, const unsigned char &value) {
    if (!opened) {
        qDebug() << "MidiOut::controlChange(): could not send. Port not opened.";
        return false;
    }
    return write((176|channel),controller,value);
}


//
// Requests:
//


bool MidiOut::patchTransferRequest() {
    return request(0x11);
}


bool MidiOut::sequenceTransferRequest() {
    return request(0x12);
}


bool MidiOut::versionRequest() {
    return request(0x1c);
}


bool MidiOut::numBanksRequest() {
    return request(0x1b);
}


bool MidiOut::currentPatchSequenceRequest() {
    return request(0x1a);
}


bool MidiOut::patchWriteRequest(const int &slot) {
    return writeRequest(slot, 0x21);
}


bool MidiOut::sequenceWriteRequest(const int &slot) {
    return writeRequest(slot, 0x22);
}


bool MidiOut::writeRequest(const int &slot, const unsigned char &which) {
    if (slot < 0) {
        qDebug() << "MidiOut::writeRequest(): Slot" << slot << "is invalid.";
        return false;
    }
    Message payload;
    payload.push_back(slot >> 8);
    payload.push_back(slot & 0xff);

    Message message;
    Midi::generateSysex(&payload, which, 0x00, &message);

    return write(message);
}
