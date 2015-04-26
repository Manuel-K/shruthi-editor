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

#ifndef SHRUTHI_LIB_EDITOR_H
#define SHRUTHI_LIB_EDITOR_H

#include "lib_patch.h"
#include "lib_midiout.h"
#include "lib_sequence.h"


// ******************************************
enum ACTIONS
// ******************************************
    {PATCH_PARAMETER_CHANGE_EDITOR, SYSEX_FETCH_REQUEST,
    SYSEX_SEND_DATA, PATCH_PARAMETER_CHANGE_MIDI, SYSEX_RECEIVED,
    SET_PATCHNAME, FILEIO_LOAD, FILEIO_SAVE,
    RESET_PATCH, RANDOMIZE_PATCH, NOTE_ON, NOTE_OFF,
    NOTE_PANIC, SYSEX_VERSION_REQUEST, SEQUENCE_PARAMETER_CHANGE_EDITOR,
    SYSEX_FETCH_SEQUENCE, SYSEX_SEND_SEQUENCE, RESET_SEQUENCE};


// ******************************************
struct queueitem_t {
// ******************************************
    ACTIONS action;
    int int0;
    int int1;
    QString string;
    unsigned int size;
    unsigned char *message;
    // constructors:
    queueitem_t() {}
    queueitem_t(ACTIONS a) {
        action=a;
    }
    queueitem_t(ACTIONS a, int i0, int i1) {
        action=a;
        int0=i0;
        int1=i1;
    }
    queueitem_t(ACTIONS a, int i0) {
        action=a;
        int0=i0;
    }
    queueitem_t(ACTIONS a, QString s) {
        action=a;
        string=s;
    }
    queueitem_t(ACTIONS a, QString s, int i0) {
        action = a;
        string = s;
        int0 = i0;
        int1 = 0;
        message = NULL;
    }
    queueitem_t(ACTIONS a, unsigned char *m, unsigned int s) {
        action=a;
        message=m;
        size=s;
    }
};


// ******************************************
class Editor : public QObject {
// ******************************************
    Q_OBJECT
    private:
        Patch patch;
        Sequence sequence;
        MidiOut midiout;
        unsigned char channel;
        int shruthiFilterBoard;
        void actionPatchParameterChangeEditor(int,int);
        void actionFetchRequest(const int &which);
        void actionSendData(const int &which);
        void actionVersionRequest();
        void actionPatchParameterChangeMidi(int,int);
        void actionNoteOn(unsigned char, unsigned char);
        void actionNoteOff(unsigned char);
        void actionNotePanic();
        void actionSysexReceived(unsigned int, unsigned int, unsigned int, unsigned char*);
        void actionSetPatchname(QString);
        void actionFileIOLoad(QString path, const int &what);
        void actionFileIOSave(QString path, const int &what);
        void actionResetPatch(unsigned int version);
        void actionRandomizePatch();
        void actionSequenceParameterChangeEditor(const unsigned &id, const int &value);
        void actionResetSequence();

    public:
        Editor();
        ~Editor();
        int getParam(int);
        QString getName();
        const int &getSequenceParam(const int &step, const SequenceParameter::SequenceParameter &sp);

        static const int FLAG_PATCH = 1;
        static const int FLAG_SEQUENCE = 2;

    public slots:
        void process(queueitem_t);
        bool setMidiOutputPort(int);
        void setMidiChannel(unsigned char);
        void setShruthiFilterBoard(int);
        void run();

    signals:
        void redrawNRPN(int);
        void redrawAll();
        void redrawSequenceParameter(int);
        void redrawAllSequenceParameters();
        void finished();
        void midiOutputStatusChanged(bool);
        void displayStatusbar(QString);
        void setStatusbarVersionLabel(QString);
};
#endif
