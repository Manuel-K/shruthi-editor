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
#include "lib_labels.h"

// ******************************************
ShruthiEditorSettings::ShruthiEditorSettings(QWidget *parent):
    QDialog(parent),
    ui(new Ui::SettingsDialog) {
// ******************************************
    ui->setupUi(this);
    getPortInfo();

    ui->midiChannel->setMinimum(1);
    ui->midiChannel->setMaximum(16);

    ui->shruthiFilterBoard->addItems(Labels::FilterBoard);
}


// ******************************************
ShruthiEditorSettings::~ShruthiEditorSettings()
// ******************************************
{
    delete ui;
}


// ******************************************
void ShruthiEditorSettings::getPortInfo() {
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
        ui->midiInputPort->addItem(name);
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
        ui->midiOutputPort->addItem(name);
    }

    cleanup:
    delete midiin;
    delete midiout;
}


// ******************************************
void ShruthiEditorSettings::setMidiPorts(const int &in, const int &out) {
// ******************************************
    ui->midiInputPort->setCurrentIndex(in);
    ui->midiOutputPort->setCurrentIndex(out);
}


// ******************************************
int ShruthiEditorSettings::getMidiInputPort() {
// ******************************************
    return ui->midiInputPort->currentIndex();
}


// ******************************************
int ShruthiEditorSettings::getMidiOutputPort() {
// ******************************************
    return ui->midiOutputPort->currentIndex();
}


// ******************************************
void ShruthiEditorSettings::setMidiChannel(const unsigned char &channel) {
// ******************************************
    ui->midiChannel->setValue(channel+1);
}


// ******************************************
unsigned char ShruthiEditorSettings::getMidiChannel() {
// ******************************************
    return ui->midiChannel->value()-1;
}

// ******************************************
void ShruthiEditorSettings::setShruthiFilterBoard(const int &index) {
// ******************************************
    ui->shruthiFilterBoard->setCurrentIndex(index);
}


// ******************************************
int ShruthiEditorSettings::getShruthiFilterBoard() {
// ******************************************
    return ui->shruthiFilterBoard->currentIndex();
}
