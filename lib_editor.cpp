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

#include "lib_editor.h"
#include <QDebug>

// ******************************************
unsigned char Editor::transferRequest[]={240,0,32,119,0,2,17,0,0,0,247};
// ******************************************


// ******************************************
Editor::Editor() {
// ******************************************
#ifdef DEBUG
    qDebug("Editor::Editor()");
#endif
};


// ******************************************
bool Editor::setMidiDevices(int in, int out) {
// ******************************************
#ifdef DEBUG
    qDebug() << "Editor::setMidiDevices:" << out;
#endif
    bool status = midiout.open(out);
    emit midiOutputStatusChanged(status);
    return status;
}


// ******************************************
Editor::~Editor() {
// ******************************************
#ifdef DEBUG
    qDebug() << "Editor::~Editor()";
#endif
}


// ******************************************
int Editor::getParam(int nrpn) {
// ******************************************
    return patch.getParam(nrpn);
};


// ******************************************
QString Editor::getName() {
// ******************************************
    return patch.getName();
};


// ******************************************
void Editor::process(queueitem_t item) {
// ******************************************
    switch (item.action) {
        case NRPN_PROCESS_EDITOR:
            actionProcessEditor(item.nrpn,item.value);
            break;
         case SYSEX_FETCH_PATCH:
            actionFetchPatch();
            break;
        case SYSEX_SEND_PATCH:
            actionSendPatch();
            break;
        case NRPN_RECIEVED:
            actionNrpnRecieved(item.nrpn,item.value);
            break;
        case NOTE_ON:
            actionNoteOn(item.noteChannel,item.note,item.noteVelocity);
            break;
        case NOTE_OFF:
            actionNoteOff(item.noteChannel,item.note,item.noteVelocity);
            break;
        case NOTE_PANIC:
            actionNotePanic(item.noteChannel);
            break;
        case SYSEX_RECIEVED:
            actionSysexRecieved(item.size,item.message);
            break;
        case SET_PATCHNAME:
            actionSetPatchname(item.string);
            break; 
        case FILEIO_LOAD:
            actionLoadPatch(item.string);
            break;
        case FILEIO_SAVE:
            actionSavePatch(item.string);
            break;
        case RESET_PATCH:
            actionResetPatch();
            break;
        case RANDOMIZE_PATCH:
            actionRandomizePatch();
            break;
        default:
#ifdef DEBUG
            qDebug() << "Editor::process():" << item.action << ":" << item.nrpn << "," << item.value << "," << item.string;
#endif
            break;
    }
    emit finished();
};


// ******************************************
void Editor::actionProcessEditor(int nrpn, int value) {
// ******************************************
#ifdef DEBUG
    qDebug() << "Editor::actionProcessEditor(" << nrpn << "," << value << ")";
#endif
    if (patch.getParam(nrpn) != value) {
        patch.setParam(nrpn,value);
        midiout.writeNRPN(nrpn,value);
    }
}

// ******************************************
void Editor::actionFetchPatch() {
// ******************************************
#ifdef DEBUG
    qDebug() << "Editor::actionFetchPatch()";
#endif
    midiout.write(Editor::transferRequest);
}


// ******************************************
void Editor::actionSendPatch() {
// ******************************************
#ifdef DEBUG
    qDebug() << "Editor::actionSendPatch()";
#endif
    std::vector<unsigned char> temp;
    patch.generateFullSysex(&temp);
    midiout.write(temp);
}


// ******************************************
void Editor::actionNrpnRecieved(int nrpn, int value) {
// ******************************************
#ifdef DEBUG
    qDebug() << "Editor::actionNrpnRecieved("<<nrpn<<","<<value<<")";
#endif
    if (Patch::parameters[nrpn].min<0 && value>=127)
        value-=256; //2s complement
    patch.setParam(nrpn,value);
    emit(redrawNRPN(nrpn));
}


// ******************************************
void Editor::actionNoteOn(unsigned char channel, unsigned char note, unsigned char velocity) {
// ******************************************
#ifdef DEBUG
    qDebug() << "Editor::actionNoteOn(" << channel << "," << note << "," << velocity << ")";
#endif
    midiout.write((144|(channel-1)),note,velocity);
}


// ******************************************
void Editor::actionNoteOff(unsigned char channel, unsigned char note, unsigned char velocity) {
// ******************************************
#ifdef DEBUG
    qDebug() << "Editor::actionNoteOff(" << channel << "," << note << "," << velocity << ")";
#endif
    midiout.write((128|(channel-1)),note,velocity);
}


// ******************************************
void Editor::actionNotePanic(unsigned char channel) {
// ******************************************
#ifdef DEBUG
    qDebug() << "Editor::actionNotePanic(" << channel <<")";
#endif
    midiout.write((176|(channel-1)),123,0);
}


// ******************************************
void Editor::actionSysexRecieved(unsigned int size, unsigned char* message) {
// ******************************************
#ifdef DEBUG
    qDebug() << "Editor::actionSysexRecieved(" << size << ",...)";
#endif
    if (7<size && message[6]==1 && message[7]==0)
        patch.parseFullSysex(message,size);
    else {
        emit displayStatusbar("Recieved unknown sysex.");
#ifdef DEBUG
        qDebug() << "unknown sysex type";
#endif
    }
    delete message;
    emit redrawAll();
}


// ******************************************
void Editor::actionSetPatchname(QString name) {
// ******************************************
#ifdef DEBUG
    qDebug() << "Editor::actionSetPatchname(" << name << ")";
#endif
    patch.setName(name);
}


// ******************************************
void Editor::actionLoadPatch(QString filename) {
// ******************************************
    bool status = patch.loadFromDisk(filename);
#ifdef DEBUG
    qDebug() << "Editor::actionLoadPatch(" << filename << "):" << status;
#endif
    emit redrawAll();
}


// ******************************************
void Editor::actionSavePatch(QString filename) {
// ******************************************
    bool status = patch.saveToDisk(filename);
#ifdef DEBUG
    qDebug() << "Editor::actionSavePatch(" << filename << "):" << status;
#endif
}


// ******************************************
void Editor::actionResetPatch() {
// ******************************************
#ifdef DEBUG
    qDebug() << "Editor::actionResetPatch()";
#endif
    patch.resetPatch();
    emit redrawAll();
}


// ******************************************
void Editor::actionRandomizePatch() {
// ******************************************
#ifdef DEBUG
    qDebug() << "Editor::actionRandomizePatch()";
#endif
    patch.randomizePatch();
    emit redrawAll();
}