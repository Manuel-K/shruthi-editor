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

#ifndef QUEUEITEM_H
#define QUEUEITEM_H

#include <QString>

// ******************************************
enum ACTIONS
// ******************************************
    {PATCH_PARAMETER_CHANGE_EDITOR, SYSEX_FETCH_REQUEST,
    SYSEX_SEND_DATA, PATCH_PARAMETER_CHANGE_MIDI, SYSEX_RECEIVED,
    SET_PATCHNAME, FILEIO_LOAD, FILEIO_SAVE,
    RESET_PATCH, RANDOMIZE_PATCH, NOTE_ON, NOTE_OFF,
    NOTE_PANIC, SYSEX_SHRUTHI_INFO_REQUEST, SEQUENCE_PARAMETER_CHANGE_EDITOR,
    SYSEX_FETCH_SEQUENCE, SYSEX_SEND_SEQUENCE, RESET_SEQUENCE,
    LIBRARY_FETCH, LIBRARY_STORE, LIBRARY_RECALL, LIBRARY_SEND, LIBRARY_MOVE,
    LIBRARY_LOAD, LIBRARY_SAVE, LIBRARY_DELETE};


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

#endif