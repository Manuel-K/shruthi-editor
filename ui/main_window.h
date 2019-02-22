// Shruthi-Editor: An unofficial Editor for the Shruthi hardware synthesizer. For
// informations about the Shruthi, see <http://www.mutable-instruments.net/shruthi1>.
//
// Copyright (C) 2011-2019 Manuel Krönig
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


#ifndef SHRUTHI_EDITOR_MAIN_WINDOW_H
#define SHRUTHI_EDITOR_MAIN_WINDOW_H


#include "config.h"
#include "queueitem.h"
#include <QMainWindow>
class QFileDialog;
class QLabel;
class QComboBox;
namespace Ui { class MainWindow; }


class ShruthiEditorMainWindow : public QMainWindow {
        Q_OBJECT

    public:
        ShruthiEditorMainWindow(QWidget *parent = 0);
        ~ShruthiEditorMainWindow();

        void setNoTranslation(bool value);

    private:
        ShruthiEditorMainWindow(const ShruthiEditorMainWindow&); //forbid copying
        ShruthiEditorMainWindow &operator=(const ShruthiEditorMainWindow&); //forbid assignment

        void displayMidiStatusChanged(const bool &in, const bool &out);
        void closeEvent(QCloseEvent* event);

        QComboBox *injectQFileDialog(QFileDialog *d);

        Ui::MainWindow *ui;
        int MIDI_INPUT_PORT, MIDI_OUTPUT_PORT, SHRUTHI_FILTER_BOARD;
        unsigned char MIDI_CHANNEL;
        bool MIDI_INPUT_STATUS, MIDI_OUTPUT_STATUS;
        QLabel *statusbarVersionLabel;

        int lastProgramFileMode;
        int parameter84, parameter85, parameter92, parameter93;

        bool noTranslationState;

    public slots:
        // redraw commands:
        void redrawPatchParameter(int id, int value);
        void redrawPatchName(QString name);
        // ui settings:
        void setMidiInputPort(int midiin);
        void setMidiOutputPort(int midiout);
        void setMidiChannel(unsigned char channel);
        void setShruthiFilterBoard(int filter);
        void midiInputStatusChanged(bool st);
        void midiOutputStatusChanged(bool st);
        void displayStatusbar(QString msg);
        void setStatusbarVersionLabel(QString text);

    private slots:
        // local ui actions
        void comboBoxChanged(int val);
        void dialChanged(int val);
        void dialChanged(int id, int val);
        void patchNameChanged();
        void loadProgram();
        void saveProgram();
        void fetchProgram();
        void sendProgram();
        void fetchPatch();
        void sendPatch();
        void fetchSequence();
        void sendSequence();
        void openSettings();
        void resetPatch();
        void resetPatchPre100();
        void resetSequence();
        void randomizePatch();
        void quitShruthiEditor();
        void aboutShruthiEditor();
        void aboutQt();

    signals:
        void enqueue(QueueItem);
        void settingsChanged(Config);
        void showKeyboard();
        void showSequenceEditor();
        void showLibrary();
};


#endif // SHRUTHI_EDITOR_MAIN_WINDOW_H
