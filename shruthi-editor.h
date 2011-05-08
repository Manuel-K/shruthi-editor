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

#ifndef SHRUTHI_EDITOR_H
#define SHRUTHI_EDITOR_H
 
#include "ui_shruthi-editor.h"

#include "lib_editor.h"

 
class shruthiEditorMainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT
 
    public:
        shruthiEditorMainWindow(Editor* edit, QWidget *parent = 0);
    private:
        void displayMidiStatusChanged(bool, bool);
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
        // redraw commands
        void redrawNRPN(int);
        void redrawAll();
        void setMidiDevices(int midiin, int midiout);
        void midiInputStatusChanged(bool);
        void midiOutputStatusChanged(bool);
        
    signals:
        void enqueue(queueitem_t);
        void midiDeviceChanged(int,int);

};
 
 
#endif
