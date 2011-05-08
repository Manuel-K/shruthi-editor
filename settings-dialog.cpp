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

#include <QtGui> 
#include "settings-dialog.h"
#include <iostream>
#include "portmidi.h"

// ******************************************
shruthiEditorSettings::shruthiEditorSettings(QWidget *parent) {
// ******************************************
    setupUi(this);
    shruthiEditorSettings::getDeviceInfo();
}


// ******************************************
void shruthiEditorSettings::getDeviceInfo() {
// ******************************************
    int numdev = Pm_CountDevices();
    std::cout << numdev << " midi devices found.\n";

    for (PmDeviceID i=0; i < numdev; i++) {
        const PmDeviceInfo* inf = Pm_GetDeviceInfo(i);
        if (inf->input)
            midi_input_device->addItem(inf->name,i);
        if (inf->output)
            midi_output_device->addItem(inf->name,i);
    }
}

// ******************************************
void shruthiEditorSettings::setChannels(int in, int out) {
// ******************************************
    midi_input_device->setCurrentIndex(midi_input_device->findData(in));
    midi_output_device->setCurrentIndex(midi_output_device->findData(out));
}

// ******************************************
int shruthiEditorSettings::getInputChannel() {
// ******************************************
    return midi_input_device->itemData(midi_input_device->currentIndex()).toInt();
}

// ******************************************
int shruthiEditorSettings::getOutputChannel() {
// ******************************************
    return midi_output_device->itemData(midi_output_device->currentIndex()).toInt();   
}