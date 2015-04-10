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

#ifndef SHRUTHI_EDITOR_H
#define SHRUTHI_EDITOR_H

#include "ui_shruthi-editor.h"

#include "lib_editor.h"


class shruthiEditorMainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

    public:
        shruthiEditorMainWindow(Editor* edit);
    private:
        void displayMidiStatusChanged(bool, bool);
        void closeEvent(QCloseEvent* event);

        Editor* editor;
        int MIDI_INPUT_PORT, MIDI_OUTPUT_PORT;
        unsigned char MIDI_CHANNEL;
        bool MIDI_INPUT_STATUS, MIDI_OUTPUT_STATUS;

    private slots:
        // local ui actions
        void comboBoxChanged(int);
        void dialChanged(int);
        void patchNameChanged();
        void loadPatch();
        void savePatch();
        void fetchPatch();
        void sendPatch();
        void changeMidiPorts();
        void resetPatch();
        void randomizePatch();
        void quitShruthiEditor();
        void aboutShruthiEditor();
        void aboutQt();

    public slots:
        // redraw commands:
        void redrawNRPN(int);
        void redrawAll();
        // ui settings:
        void setMidiInputPort(int);
        void setMidiOutputPort(int);
        void setMidiChannel(unsigned char);
        void midiInputStatusChanged(bool);
        void midiOutputStatusChanged(bool);
        void displayStatusbar(QString msg);

    signals:
        void enqueue(queueitem_t);
        void settingsChanged(int,int,unsigned char);
        void showKeyboard();
};


#endif
