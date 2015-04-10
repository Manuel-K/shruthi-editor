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

#include <QtWidgets>
#include "settings-dialog.h"
#include <iostream>
#include "RtMidi.h"

// ******************************************
shruthiEditorSettings::shruthiEditorSettings(QWidget *parent) {
// ******************************************
    setupUi(this);
    getPortInfo();
    
    midiChannel->setMinimum(1);
    midiChannel->setMaximum(16);
}


// ******************************************
void shruthiEditorSettings::getPortInfo() {
// ******************************************
    RtMidiIn  *midiin = 0;
    RtMidiOut *midiout = 0;
    QString name;
  
    // Input ports:
    try {
        midiin = new RtMidiIn();
    }
    catch (RtMidiError &error) {
        error.printMessage();
        exit(EXIT_FAILURE);
    }
    unsigned int numdev = midiin->getPortCount();
    
    std::cout << numdev << " midi input ports found." << std::endl;

    for (unsigned int i=0; i < numdev; i++) {
        try {
            name = QString::fromStdString(midiin->getPortName(i));
        }
        catch (RtMidiError &error) {
            error.printMessage();
            goto cleanup;
        }
        midiInputPort->addItem(name);
    }
    
    // Output ports:
    try {
        midiout = new RtMidiOut();
    }
    catch (RtMidiError &error) {
        error.printMessage();
        exit(EXIT_FAILURE);
    }
    numdev = midiout->getPortCount();
    
    std::cout << numdev << " midi output ports found." << std::endl;

    for (unsigned int i=0; i < numdev; i++) {
        try {
            name = QString::fromStdString(midiout->getPortName(i));
        }
        catch (RtMidiError &error) {
            error.printMessage();
            goto cleanup;
        }
        midiOutputPort->addItem(name);
    }
    
    cleanup:
    delete midiin;
    delete midiout;
}


// ******************************************
void shruthiEditorSettings::setMidiPorts(int in, int out) {
// ******************************************
    midiInputPort->setCurrentIndex(in);
    midiOutputPort->setCurrentIndex(out);
}


// ******************************************
int shruthiEditorSettings::getMidiInputPort() {
// ******************************************
    return midiInputPort->currentIndex();
}


// ******************************************
int shruthiEditorSettings::getMidiOutputPort() {
// ******************************************
    return midiOutputPort->currentIndex();   
}


// ******************************************
void shruthiEditorSettings::setMidiChannel(unsigned char channel) {
// ******************************************
    midiChannel->setValue(channel+1);
}


// ******************************************
unsigned char shruthiEditorSettings::getMidiChannel() {
// ******************************************
    return midiChannel->value()-1;
}
