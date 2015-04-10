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
#include <QThread>


// ******************************************
enum ACTIONS
// ******************************************
    {NRPN_PROCESS_EDITOR, SYSEX_FETCH_PATCH,
    SYSEX_SEND_PATCH, NRPN_RECEIVED, SYSEX_RECEIVED,
    SET_PATCHNAME, FILEIO_LOAD, FILEIO_SAVE,
    RESET_PATCH, RANDOMIZE_PATCH, NOTE_ON, NOTE_OFF,
    NOTE_PANIC, SYSEX_VERSION_REQUEST};


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
    queueitem_t () {}
    queueitem_t (ACTIONS a) {
        action=a;
    }
    queueitem_t (ACTIONS a, int i0, int i1) {
        action=a;
        int0=i0;
        int1=i1;
    }
    queueitem_t (ACTIONS a, int i0) {
        action=a;
        int0=i0;
    }
    queueitem_t (ACTIONS a, QString s) {
        action=a;
        string=s;
    }
    queueitem_t (ACTIONS a, unsigned char *m, unsigned int s) {
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
        MidiOut midiout;
        unsigned char channel;
        void actionProcessEditor(int,int);
        void actionFetchPatch();
        void actionSendPatch();
        void actionVersionRequest();
        void actionNrpnReceived(int,int);
        void actionNoteOn(unsigned char, unsigned char);
        void actionNoteOff(unsigned char);
        void actionNotePanic();
        void actionSysexReceived(unsigned int, unsigned char*);
        void actionSetPatchname(QString);
        void actionLoadPatch(QString);
        void actionSavePatch(QString);
        void actionResetPatch();
        void actionRandomizePatch();

    public:
        Editor();
        ~Editor();
        int getParam(int);
        QString getName();

    public slots:
        void process(queueitem_t);
        bool setMidiOutputPort(int);
        void setMidiChannel(unsigned char);

    signals:
        void redrawNRPN(int);
        void redrawAll();
        void finished();
        void midiOutputStatusChanged(bool);
        void displayStatusbar(QString);
};
#endif
