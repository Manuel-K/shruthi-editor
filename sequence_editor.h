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


#ifndef SEQUENCEEDITOR_H
#define SEQUENCEEDITOR_H

#include <QDialog>
#include <lib_editor.h>

namespace Ui {
class SequenceEditor;
}

// ******************************************
class SequenceEditor : public QDialog
// ******************************************
{
        Q_OBJECT

    public:
        explicit SequenceEditor(Editor *edit, QWidget *parent = 0);
        ~SequenceEditor();

    private:
        void sendSequenceUpdate();

        Ui::SequenceEditor *ui;
        Editor *editor;

    public slots:
        void showSequenceEditor();
        void redrawPatchParameter(int id);
        void redrawAllPatchParameters();
        void redrawAllSequenceParameters();

    private slots:
        void activeChanged(int step, int value);
        void noteChanged(int step, int value);
        void valueChanged(int step, int value);
        void tieChanged(int step, int value);
        void velocityChanged(int step, int velocity);

        void comboBoxChanged(int value);
        void dialChanged(int id, int value);

    signals:
        void enqueue(queueitem_t);
};

#endif // SEQUENCEEDITOR_H
