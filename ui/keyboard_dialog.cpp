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


#include "ui/keyboard_dialog.h"
#include "ui_keyboard_dialog.h"
#ifdef DEBUGMSGS
#include <QDebug>
#endif


KeyboardDialog::KeyboardDialog(QWidget *parent):
    QDialog(parent),
    ui(new Ui::KeyboardDialog) {
    ui->setupUi(this);

    noteVelocity = 64;
    ui->octave->setMinimum(-4);
    ui->octave->setMaximum(4);
    ui->keys->setBaseNote(48);

    connect(ui->velocity, SIGNAL(sliderMoved(int)), this, SLOT(setVelocity(int)));
    ui->velocity->setValue(noteVelocity);
    connect(ui->octave, SIGNAL(valueChanged(int)), this, SLOT(setOctave(int)));
    ui->octave->setValue(defaultOctave);
    connect(ui->panic, SIGNAL(pressed()), this, SLOT(panicPushed()));
    connect(ui->keys, SIGNAL(keyPressed(int)), this, SLOT(keyPressed(int)));
    connect(ui->keys, SIGNAL(keyReleased(int)), this, SLOT(keyReleased(int)));
}


KeyboardDialog::~KeyboardDialog() {
    delete ui;
}

//
// Slots:
//


void KeyboardDialog::keyPressed(int key) {
#ifdef DEBUGMSGS
    qDebug() << "Keyboard::keyPressed()" << key;
#endif
    QueueItem signal(QueueAction::NOTE_ON, key, noteVelocity);
    emit enqueue(signal);
}


void KeyboardDialog::panicPushed() {
#ifdef DEBUGMSGS
    qDebug() << "Keyboard::panicPushed()";
#endif
    QueueItem signal(QueueAction::NOTE_PANIC);
    emit enqueue(signal);
}


void KeyboardDialog::keyReleased(int key) {
#ifdef DEBUGMSGS
    qDebug() << "Keyboard::keyReleased()" << key;
#endif
    QueueItem signal(QueueAction::NOTE_OFF, key);
    emit enqueue(signal);
}


void KeyboardDialog::setVelocity(int vel) {
    noteVelocity = vel;
#ifdef DEBUGMSGS
    qDebug() << "Keyboard::setVelocity()" << vel;
#endif
}


void KeyboardDialog::setOctave(int oct) {
    ui->keys->setOctave(oct);
#ifdef DEBUGMSGS
    qDebug() << "setOctave()" << oct;
#endif
}
