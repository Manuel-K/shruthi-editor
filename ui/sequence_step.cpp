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


#include "ui/sequence_step.h"
#include "ui_sequence_step.h"


SequenceStep::SequenceStep(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SequenceStep) {
    ui->setupUi(this);

    step = 0;

    sendActive = true;
    sendNote = true;
    sendTie = true;
    sendVelocity = true;
    sendValue = true;

    QStringList Notes = QStringList() << "C" << "C#" << "D" << "D#" << "E" << "F" << "F#" << "G" << "G#" << "A" << "A#" << "B";
    //QStringList Notes = QStringList() << "C" << "C♯" << "D" << "D♯" << "E" << "F" << "F♯" << "G" << "G♯" << "A" << "A♯" << "B";
    QStringList HexValues = QStringList() << "0" << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "A" << "B" << "C" << "D" << "E" << "F";

    setNoteLabels(Notes);
    setValueLabels(HexValues);
    setOctaveLimits(0, 8); // -1..9 should be possible, but for 9 the notes A..B have to be disabled!
    setVelocityLimits(0, 7);

    connect(ui->active, SIGNAL(stateChanged(int)), this, SLOT(activeChanged(int)));
    connect(ui->note, SIGNAL(currentIndexChanged(int)), this, SLOT(noteChanged(int)));
    connect(ui->octave, SIGNAL(valueChanged(int)), this, SLOT(octaveChanged(int)));
    connect(ui->value, SIGNAL(currentIndexChanged(int)), this, SLOT(valueChanged(int)));
    connect(ui->tie, SIGNAL(stateChanged(int)), this, SLOT(tieChanged(int)));
    connect(ui->velocity, SIGNAL(valueChanged(int)), this, SLOT(velocityChanged(int)));
}


SequenceStep::~SequenceStep() {
    delete ui;
}


//
// Setup:
//


void SequenceStep::setStep(const int &value) {
    step = value;
    ui->name->setText(QString("%1").arg(value));
}


void SequenceStep::setValueLabels(const QStringList &labels) {
    ui->value->clear();
    ui->value->addItems(labels);
}


void SequenceStep::setNoteLabels(const QStringList &labels) {
    ui->note->clear();
    ui->note->addItems(labels);
}


void SequenceStep::setOctaveLimits(const int &min, const int &max) {
    ui->octave->setMinimum(min);
    ui->octave->setMaximum(max);
}


void SequenceStep::setVelocityLimits(const int &min, const int &max) {
    ui->velocity->setMinimum(min);
    ui->velocity->setMaximum(max);
}


//
// Value handling:
//


void SequenceStep::setActive(const int &value) {
    sendActive = false;
    ui->active->setChecked(value ? true : false);
    sendActive = true;
}


void SequenceStep::setValue(const int &value) {
    sendValue = false;
    ui->value->setCurrentIndex(value);
    sendValue = true;
}


void SequenceStep::setNoteOctave(const int &value) {
    const int &note  = (value - noteBase) % 12;
    const int &octave = (value - noteBase) / 12;
    sendNote = false;
    ui->note->setCurrentIndex(note);
    ui->octave->setValue(octave);
    sendNote = true;
}


void SequenceStep::setNote(const int &value) {
    sendNote = false;
    ui->note->setCurrentIndex(value);
    sendNote = true;
}


void SequenceStep::setOctave(const int &value) {
    sendNote = false;
    ui->octave->setValue(value);
    sendNote = true;
}


void SequenceStep::setTie(const int &value) {
    sendTie = false;
    ui->tie->setChecked(value ? true : false);
    sendTie = true;
}


void SequenceStep::setVelocity(const int &value) {
    sendVelocity = false;
    ui->velocity->setValue(value);
    sendVelocity = true;
}


//
// Slots
//


void SequenceStep::activeChanged(int val) {
    if (!sendActive) {
        return;
    }

    emit activeChanged(step, val ? 1 : 0);
}


void SequenceStep::noteChanged(int val) {
    if (!sendNote) {
        return;
    }
    const int &note = noteBase + ui->octave->value() * 12 + val;
    emit noteChanged(step, note);
}


void SequenceStep::octaveChanged(int val) {
    if (!sendNote) {
        return;
    }
    const int &note = noteBase + val * 12 + ui->note->currentIndex();
    emit noteChanged(step, note);
}



void SequenceStep::valueChanged(int val) {
    if (!sendValue) {
        return;
    }
    emit valueChanged(step, val);
}



void SequenceStep::tieChanged(int val) {
    if (!sendTie) {
        return;
    }
    emit tieChanged(step, val ? 1 : 0);
}


void SequenceStep::velocityChanged(int val) {
    if (!sendVelocity) {
        return;
    }
    emit velocityChanged(step, val);
}
