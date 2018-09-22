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


#include "ui/settings_dialog.h"
#include "ui_settings_dialog.h"
#include <iostream>
#include <stddef.h> // for NULL
#include <string>
#include "RtMidi.h"
#include "labels.h"


SettingsDialog::SettingsDialog(QWidget *parent):
    QDialog(parent),
    ui(new Ui::SettingsDialog) {
    ui->setupUi(this);

    input_port = 0;
    output_port = 0;
    input_port_error = false;
    output_port_error = false;

    getPortInfo();

    ui->midiChannel->setMinimum(1);
    ui->midiChannel->setMaximum(16);

    ui->shruthiFilterBoard->addItems(Labels::FilterBoard);
}


SettingsDialog::~SettingsDialog() {
    delete ui;
}


void SettingsDialog::getPortInfo() {
    RtMidiIn  *midiin = 0;
    RtMidiOut *midiout = 0;
    QString name;
    unsigned int numdev = 0;

    // Input ports:
    try {
        midiin = new RtMidiIn(RtMidi::UNSPECIFIED, "shruthi-editor port probe");
    }
    catch (RtMidiError &error) {
        error.printMessage();
        ui->midiInputPort->addItem("Error: Could not initialize RtMidi!");
        ui->midiInputPort->setCurrentIndex(0);
        input_port_error = true;
    }

    if (!input_port_error) {
        numdev = midiin->getPortCount();

        std::cout << numdev << " midi input ports found." << std::endl;

        for (unsigned int i=0; i < numdev; i++) {
            try {
                name = QString::fromStdString(midiin->getPortName(i));
            }
            catch (RtMidiError &error) {
                name = "Unknown Port";
                error.printMessage();
            }

            // don't add editor instances:
            if (!name.startsWith("shruthi-editor")) {
                ui->midiInputPort->addItem(name, i);
            }
        }
        delete midiin;
        midiin = NULL;
    }


    // Output ports:
    try {
        midiout = new RtMidiOut(RtMidi::UNSPECIFIED, "shruthi-editor port probe");
    }
    catch (RtMidiError &error) {
        error.printMessage();
        ui->midiOutputPort->addItem("Error: Could not initialize RtMidi!");
        ui->midiOutputPort->setCurrentIndex(0);
        output_port_error = true;
    }

    if (!output_port_error) {
        numdev = midiout->getPortCount();

        std::cout << numdev << " midi output ports found." << std::endl;

        for (unsigned int i=0; i < numdev; i++) {
            try {
                name = QString::fromStdString(midiout->getPortName(i));
            }
            catch (RtMidiError &error) {
                name = "Unknown Port";
                error.printMessage();
            }

            // don't add editor instances:
            if (!name.startsWith("shruthi-editor")) {
                ui->midiOutputPort->addItem(name, i);
            }
        }
        delete midiout;
        midiout = NULL;
    }
}


void SettingsDialog::setMidiPorts(const int &in, const int &out) {
    input_port = in;
    output_port = out;

    if (!input_port_error) {
        ui->midiInputPort->setCurrentIndex(in);
    }
    if (!output_port_error) {
        ui->midiOutputPort->setCurrentIndex(out);
    }
}


int SettingsDialog::getMidiInputPort() {
    if (input_port_error) {
        return input_port;
    }
    return ui->midiInputPort->currentData().toInt();
}


int SettingsDialog::getMidiOutputPort() {
    if (output_port_error) {
        return output_port;
    }
    return ui->midiOutputPort->currentData().toInt();
}


void SettingsDialog::setMidiChannel(const unsigned char &channel) {
    ui->midiChannel->setValue(channel+1);
}


unsigned char SettingsDialog::getMidiChannel() {
    return ui->midiChannel->value()-1;
}


void SettingsDialog::setShruthiFilterBoard(const int &index) {
    ui->shruthiFilterBoard->setCurrentIndex(index);
}


int SettingsDialog::getShruthiFilterBoard() {
    return ui->shruthiFilterBoard->currentIndex();
}
