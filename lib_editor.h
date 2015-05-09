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
#include "lib_library.h"


// ******************************************
enum ACTIONS
// ******************************************
    {PATCH_PARAMETER_CHANGE_EDITOR, SYSEX_FETCH_REQUEST,
    SYSEX_SEND_DATA, PATCH_PARAMETER_CHANGE_MIDI, SYSEX_RECEIVED,
    SET_PATCHNAME, FILEIO_LOAD, FILEIO_SAVE,
    RESET_PATCH, RANDOMIZE_PATCH, NOTE_ON, NOTE_OFF,
    NOTE_PANIC, SYSEX_SHRUTHI_INFO_REQUEST, SEQUENCE_PARAMETER_CHANGE_EDITOR,
    SYSEX_FETCH_SEQUENCE, SYSEX_SEND_SEQUENCE, RESET_SEQUENCE,
    LIBRARY_FETCH, LIBRARY_STORE, LIBRARY_RECALL, LIBRARY_SEND, LIBRARY_MOVE};


// ******************************************
struct queueitem_t {
// ******************************************
    ACTIONS action;
    int int0;
    int int1;
    int int2;
    QString string;
    unsigned int size;
    unsigned char *message;
    // constructors:
    queueitem_t() {
        message = NULL;
    }
    queueitem_t(ACTIONS a) {
        action = a;
        message = NULL;
    }
    queueitem_t(ACTIONS a, int i0, int i1, int i2) {
        action = a;
        int0 = i0;
        int1 = i1;
        int2 = i2;
        message = NULL;
    }
    queueitem_t(ACTIONS a, int i0, int i1) {
        action = a;
        int0 = i0;
        int1 = i1;
        message = NULL;
    }
    queueitem_t(ACTIONS a, int i0) {
        action = a;
        int0 = i0;
        message = NULL;
    }
    queueitem_t(ACTIONS a, QString s) {
        action = a;
        string = s;
        message = NULL;
    }
    queueitem_t(ACTIONS a, QString s, int i0) {
        action = a;
        string = s;
        int0 = i0;
        message = NULL;
    }
    queueitem_t(ACTIONS a, unsigned char *m, unsigned int s) {
        action = a;
        message = m;
        size = s;
    }
};


// ******************************************
class Editor : public QObject {
// ******************************************
    Q_OBJECT

    public:
        Editor();
        ~Editor();
        const int &getParam(int id) const;
        const QString &getName() const;
        const int &getSequenceParam(const int &step, const SequenceParameter::SequenceParameter &sp) const;

        const Library &getLibrary() const;

        // keep in sync with Library::FLAGs
        static const int FLAG_PATCH = 1;
        static const int FLAG_SEQUENCE = 2;

    private:
        void actionPatchParameterChangeEditor(int id, int value);
        void actionFetchRequest(const int &what);
        void actionSendData(const int &what);
        void actionShruthiInfoRequest();
        void actionPatchParameterChangeMidi(int id, int value);
        void actionNoteOn(unsigned char note, unsigned char velocity);
        void actionNoteOff(unsigned char note);
        void actionNotePanic();
        void actionSysexReceived(unsigned int command, unsigned int argument, unsigned int size, unsigned char* message);
        void actionSetPatchname(QString name);
        void actionFileIOLoad(QString path, const int &what);
        void actionFileIOSave(QString path, const int &what);
        void actionResetPatch(unsigned int version);
        void actionRandomizePatch();
        void actionSequenceParameterChangeEditor(const unsigned &id, const int &value);
        void actionResetSequence();

        void actionLibraryFetch(const unsigned int &what, const int &start, const int &stop);
        void actionLibrarySend(const unsigned int &what, const int &start, const int &end);
        void actionLibraryRecall(const unsigned int &what, const unsigned int &id);
        void actionLibraryStore(const unsigned int &what, const unsigned int &id);
        void actionLibraryMove(const unsigned int &what, const unsigned int &start, const unsigned int &target);

        Patch patch;
        Sequence sequence;
        Library library;
        MidiOut midiout;
        unsigned char channel;
        int shruthiFilterBoard;
        int firmwareVersion;
        int numberOfPrograms;

    public slots:
        void process(queueitem_t item);
        bool setMidiOutputPort(int out);
        void setMidiChannel(unsigned char channel);
        void setShruthiFilterBoard(int filter);
        void run();

    signals:
        void redrawPatchParamter(int);
        void redrawPatchParameter2(int);
        void redrawAllPatchParameters();
        void redrawSequenceParameter(int);
        void redrawAllSequenceParameters();
        void finished();
        void midiOutputStatusChanged(bool);
        void displayStatusbar(QString);
        void setStatusbarVersionLabel(QString);
        void redrawLibraryItems(int,int,int);
};
#endif
