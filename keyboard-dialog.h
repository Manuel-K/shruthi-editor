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

#ifndef KEYBOARD_DIALOG_H
#define KEYBOARD_DIALOG_H

#include "ui_keyboard.h"
#include "lib_editor.h"

// ******************************************
class Keyboard : public QDialog {
// ******************************************
    Q_OBJECT

    public:
        explicit Keyboard(QWidget *parent = 0);
        ~Keyboard();

    private:
        Ui::keyboard *ui;
        unsigned char noteVelocity;
        static const int defaultOctave = 0;

    private slots:
        void keyPressed(int key);
        void keyReleased(int key);
        void setVelocity(int vel);
        void setOctave(int oct);
        void panicPushed();

    signals:
        void enqueue(queueitem_t);
};
#endif
