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
Editor::Editor() {
// ******************************************
#ifdef DEBUG
    qDebug("Editor::Editor()");
#endif
};


// ******************************************
bool Editor::setMidiOutputPort(int out) {
// ******************************************
#ifdef DEBUG
    qDebug() << "Editor::setMidiPorts:" << out;
#endif
    bool status = midiout.open(out);
    emit midiOutputStatusChanged(status);
    return status;
}


// ******************************************
void Editor::setMidiChannel(unsigned char channel) {
// ******************************************
#ifdef DEBUG
    qDebug() << "Editor::setMidiChannel:" << channel;
#endif
    Editor::channel = channel;
}


// ******************************************
Editor::~Editor() {
// ******************************************
#ifdef DEBUG
    qDebug() << "Editor::~Editor()";
#endif
}


// ******************************************
int Editor::getParam(unsigned int nrpn) {
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
            actionProcessEditor(item.int0,item.int1);
            break;
         case SYSEX_FETCH_PATCH:
            actionFetchPatch();
            break;
        case SYSEX_SEND_PATCH:
            actionSendPatch();
            break;
        case NRPN_RECIEVED:
            actionNrpnRecieved(item.int0,item.int1);
            break;
        case NOTE_ON:
            actionNoteOn(item.int0,item.int1);
            break;
        case NOTE_OFF:
            actionNoteOff(item.int0);
            break;
        case NOTE_PANIC:
            actionNotePanic();
            break;
        case SYSEX_RECIEVED:
            actionSysexRecieved(item.size,item.message);
            break;
        case SET_PATCHNAME:
            actionSetPatchname(item.string);
            break; 
        case LIBRARY_EDIT:
            actionLibraryEdit(item.int0);
            break;
        case LIBRARY_STORE:
            actionLibraryStore(item.int0);
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
        case LIBRARY_FETCH_PATCHES:
            actionLibraryFetchPatches();
            break;
        default:
#ifdef DEBUG
            qDebug() << "Editor::process():" << item.action << ":" << item.int0 << "," << item.int1 << "," << item.string;
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
        if (!midiout.nrpn(nrpn,value))
            emit displayStatusbar("Could not send changes.");
    }
}

// ******************************************
void Editor::actionFetchPatch() {
// ******************************************
#ifdef DEBUG
    qDebug() << "Editor::actionFetchPatch()";
#endif
    if (midiout.patchTransferRequest())
        emit displayStatusbar("Patch transfer request sent.");
    else
        emit displayStatusbar("Could not send patch transfer request.");
}


// ******************************************
void Editor::actionSendPatch() {
// ******************************************
#ifdef DEBUG
    qDebug() << "Editor::actionSendPatch()";
#endif
    std::vector<unsigned char> temp;
    patch.generateFullSysex(&temp);
    if (midiout.write(temp))
        emit displayStatusbar("Patch sent.");
    else
        emit displayStatusbar("Could not send patch.");
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
    emit redrawNRPN(nrpn);
}


// ******************************************
void Editor::actionNoteOn(unsigned char note, unsigned char velocity) {
// ******************************************
#ifdef DEBUG
    qDebug() << "Editor::actionNoteOn(" << channel << "," << note << "," << velocity << ")";
#endif
    if(!midiout.noteOn(channel,note,velocity))
        emit displayStatusbar("Could not send note on message.");
}


// ******************************************
void Editor::actionNoteOff(unsigned char note) {
// ******************************************
#ifdef DEBUG
    qDebug() << "Editor::actionNoteOff(" << channel << "," << note << ")";
#endif
    if(!midiout.noteOff(channel,note))
        emit displayStatusbar("Could not send note off message.");
}


// ******************************************
void Editor::actionNotePanic() {
// ******************************************
#ifdef DEBUG
    qDebug() << "Editor::actionNotePanic(" << channel <<")";
#endif
    if (midiout.allNotesOff(channel))
        emit displayStatusbar("Sent all notes off message.");
    else
        emit displayStatusbar("Could not send all notes off message.");
}


// ******************************************
void Editor::actionSysexRecieved(unsigned int size, unsigned char* message) {
// ******************************************
#ifdef DEBUG
    qDebug() << "Editor::actionSysexRecieved(" << size << ",...)";
#endif
    bool status;
    bool libraryMode = lib.isRecieving();
    if (7<size && message[6]==1 && message[7]==0) {
      
        if (libraryMode) {
            status = lib.parseFullSysex(message,size);
            if (lib.isRecieving()) {
                midiout.programChange(channel,lib.getNumPatches());
                actionFetchPatch();
            } else {
                emit redrawPatches();
            }
        } else {
            status = patch.parseFullSysex(message,size);
        }
        
        if (status) {
            if (libraryMode)
                emit displayStatusbar("Fetching patch library: Recieved valid patch. ("+QString("%1").arg(100*lib.getNumPatches()/lib.getMaxNumPatches())+"% done).");
            else
                emit displayStatusbar("Recieved valid patch.");
        } else {
            emit displayStatusbar("Recieved invalid patch.");
        }
    } else {
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
    emit displayStatusbar("Patch name set.");
}


// ******************************************
void Editor::actionLoadPatch(QString filename) {
// ******************************************
    bool status = patch.loadFromDisk(filename);
#ifdef DEBUG
    qDebug() << "Editor::actionLoadPatch(" << filename << "):" << status;
#endif
    if (status)
        emit displayStatusbar("Patch loaded from disk.");
    else
        emit displayStatusbar("Could not load patch.");
    emit redrawAll();
}


// ******************************************
void Editor::actionSavePatch(QString filename) {
// ******************************************
    bool status = patch.saveToDisk(filename);
#ifdef DEBUG
    qDebug() << "Editor::actionSavePatch(" << filename << "):" << status;
#endif
    if (status)
        emit displayStatusbar("Patch saved to disk.");
    else
        emit displayStatusbar("Could not save patch.");
}


// ******************************************
void Editor::actionResetPatch() {
// ******************************************
#ifdef DEBUG
    qDebug() << "Editor::actionResetPatch()";
#endif
    patch.resetPatch();
    emit redrawAll();
    emit displayStatusbar("Patch reset.");
}


// ******************************************
void Editor::actionRandomizePatch() {
// ******************************************
#ifdef DEBUG
    qDebug() << "Editor::actionRandomizePatch()";
#endif
    patch.randomizePatch();
    emit redrawAll();
    emit displayStatusbar("Patch randomized.");
}


// ******************************************
void Editor::actionLibraryFetchPatches() {
// ******************************************
#ifdef DEBUG
    qDebug() << "Editor::actionLibraryFetchPatches()";
#endif
    midiout.programChange(channel,0);
    lib.reset();
    actionFetchPatch();
}


// ******************************************
void Editor::actionLibraryEdit(unsigned int n) {
// ******************************************
#ifdef DEBUG
    qDebug() << "Editor::actionLibraryEdit(" << n << ")";
#endif
    midiout.programChange(channel,n);
    patch = lib.getPatch(n);
    emit redrawAll();
}


// ******************************************
void Editor::actionLibraryStore(unsigned int n) {
// ******************************************
#ifdef DEBUG
    qDebug() << "Editor::actionLibraryStore(" << n << ")";
#endif
    lib.setPatch(n,patch);
    emit redrawPatches();
    emit redrawAll();
}


// ******************************************
QString Editor::libraryGetPatchName(unsigned int num) {
// ******************************************
    return lib.getName(num);
}


// ******************************************
unsigned int Editor::libraryGetNumPatches() {
// ******************************************
    return lib.getNumPatches();
}
