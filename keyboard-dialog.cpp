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
#include "keyboard-dialog.h"

// ******************************************
keyboard::keyboard() {
// ******************************************
    setupUi(this);
    QPushButton *tmp;

    noteOctave = 0;
    noteVelocity = 64;
    octave->setMinimum(-4);
    octave->setMaximum(4);

    connect(velocity,SIGNAL(sliderMoved(int)), this, SLOT(setVelocity(int)));
    velocity->setValue (noteVelocity);
    connect(octave,SIGNAL(valueChanged(int)), this, SLOT(setOctave(int)));
    octave->setValue (noteOctave);
    connect(panic,SIGNAL(pressed()), this, SLOT(panicPushed()));

    for (int i=0; i<=12; i++) {
        tmp = this->findChild<QPushButton*>(QString("n")+QString("%1").arg(i));
        connect(tmp,SIGNAL(pressed()), this, SLOT(push()));
        connect(tmp,SIGNAL(released()), this, SLOT(release()));
        if (i==1||i==3||i==6||i==8||i==10)
            tmp->setStyleSheet("QPushButton { background-color: black }");
        else
            tmp->setStyleSheet("QPushButton { background-color: white }");
    }
}

// ******************************************
void keyboard::showKeyboard() {
// ******************************************
    show();
}

// ******************************************
void keyboard::push() {
// ******************************************
    QPushButton* s = (QPushButton*) sender();
    QString id = s->objectName();
    id.remove(0,1);
    unsigned char note = calculateNote(id.toInt());
#ifdef DEBUG
    qDebug() << "push()" << note;
#endif
    queueitem_t signal (NOTE_ON,note,noteVelocity);
    emit enqueue(signal);
}

// ******************************************
void keyboard::panicPushed() {
// ******************************************
#ifdef DEBUG
    qDebug() << "panicPushed()";
#endif
    queueitem_t signal (NOTE_PANIC);
    emit enqueue(signal);
}

// ******************************************
void keyboard::release() {
// ******************************************
    QPushButton* s = (QPushButton*) sender();
    QString id = s->objectName();
    id.remove(0,1);
    unsigned char note = calculateNote(id.toInt());
#ifdef DEBUG
    qDebug() << "release()" << note;
#endif
    queueitem_t signal (NOTE_OFF,note);
    emit enqueue(signal);
}


// ******************************************
void keyboard::setVelocity(int vel) {
// ******************************************
    noteVelocity = vel;
#ifdef DEBUG
    qDebug() << "setVelocity()" << vel;
#endif
}


// ******************************************
void keyboard::setOctave(int oct) {
// ******************************************
    noteOctave = oct;
#ifdef DEBUG
    qDebug() << "setOctave()" << oct;
#endif
}


// ******************************************
unsigned char keyboard::calculateNote(int key) {
// ******************************************
    return 48 + key + 12 * noteOctave;
}
