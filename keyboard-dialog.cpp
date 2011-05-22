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
#include "keyboard-dialog.h"

// ******************************************
keyboard::keyboard(QWidget *parent) {
// ******************************************
    setupUi(this);
    QPushButton *tmp;
    
    noteOctave = 0;
    noteVelocity = 64;
    noteChannel = 1;
    channel->setMinimum(1);
    channel->setMaximum(16);
    octave->setMinimum(-4);
    octave->setMaximum(4);
    
    connect(velocity,SIGNAL(sliderMoved(int)), this, SLOT(setVelocity(int)));
    velocity->setValue (noteVelocity);
    connect(octave,SIGNAL(valueChanged(int)), this, SLOT(setOctave(int)));
    octave->setValue (noteOctave);
    connect(channel,SIGNAL(valueChanged(int)), this, SLOT(setChannel(int)));
    channel->setValue (noteChannel);
    for (int i=0; i<=12; i++) {
        tmp = qFindChild<QPushButton*>(this, QString("n")+QString("%1").arg(i));
        connect(tmp,SIGNAL(pressed()), this, SLOT(push()));
        connect(tmp,SIGNAL(released()), this, SLOT(release()));
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
    queueitem_t signal (NOTE_ON,noteChannel,note,noteVelocity);
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
    queueitem_t signal (NOTE_OFF,noteChannel,note,0);
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
void keyboard::setChannel(int ch) {
// ******************************************
    noteChannel = ch;
#ifdef DEBUG
    qDebug() << "setChannel()" << ch;
#endif
}


// ******************************************
unsigned char keyboard::calculateNote(int key) {
// ******************************************
    return 48 + key + 12 * noteOctave;
}