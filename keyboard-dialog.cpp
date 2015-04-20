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
Keyboard::Keyboard(QWidget *parent):
    QDialog(parent),
    ui(new Ui::keyboard) {
// ******************************************
    ui->setupUi(this);
    QPushButton *tmp;

    noteOctave = 0;
    noteVelocity = 64;
    ui->octave->setMinimum(-4);
    ui->octave->setMaximum(4);

    connect(ui->velocity, SIGNAL(sliderMoved(int)), this, SLOT(setVelocity(int)));
    ui->velocity->setValue(noteVelocity);
    connect(ui->octave, SIGNAL(valueChanged(int)), this, SLOT(setOctave(int)));
    ui->octave->setValue(noteOctave);
    connect(ui->panic, SIGNAL(pressed()), this, SLOT(panicPushed()));

    for (int i=0; i<=12; i++) {
        tmp = this->findChild<QPushButton*>(QString("n%1").arg(i));
        if (!tmp) {
            qDebug() << "Keyboard PushButton" << QString("n%1").arg(i) << "could not be found!";
            continue;
        }
        connect(tmp,SIGNAL(pressed()), this, SLOT(push()));
        connect(tmp,SIGNAL(released()), this, SLOT(release()));
        if (i==1||i==3||i==6||i==8||i==10)
            tmp->setStyleSheet("QPushButton { background-color: black }");
        else
            tmp->setStyleSheet("QPushButton { background-color: white }");
    }
}


// ******************************************
Keyboard::~Keyboard()
// ******************************************
{
    delete ui;
}


// ******************************************
void Keyboard::showKeyboard() {
// ******************************************
    show();
}

// ******************************************
void Keyboard::push() {
// ******************************************
    QPushButton* s = (QPushButton*) sender();
    QString id = s->objectName();
    id.remove(0,1);
    unsigned char note = calculateNote(id.toInt());
#ifdef DEBUGMSGS
    qDebug() << "push()" << note;
#endif
    queueitem_t signal(NOTE_ON,note,noteVelocity);
    emit enqueue(signal);
}

// ******************************************
void Keyboard::panicPushed() {
// ******************************************
#ifdef DEBUGMSGS
    qDebug() << "panicPushed()";
#endif
    queueitem_t signal(NOTE_PANIC);
    emit enqueue(signal);
}

// ******************************************
void Keyboard::release() {
// ******************************************
    QPushButton* s = (QPushButton*) sender();
    QString id = s->objectName();
    id.remove(0,1);
    unsigned char note = calculateNote(id.toInt());
#ifdef DEBUGMSGS
    qDebug() << "release()" << note;
#endif
    queueitem_t signal(NOTE_OFF,note);
    emit enqueue(signal);
}


// ******************************************
void Keyboard::setVelocity(int vel) {
// ******************************************
    noteVelocity = vel;
#ifdef DEBUGMSGS
    qDebug() << "setVelocity()" << vel;
#endif
}


// ******************************************
void Keyboard::setOctave(int oct) {
// ******************************************
    noteOctave = oct;
#ifdef DEBUGMSGS
    qDebug() << "setOctave()" << oct;
#endif
}


// ******************************************
unsigned char Keyboard::calculateNote(int key) {
// ******************************************
    return 48 + key + 12 * noteOctave;
}
