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

#include "lib_editor.h"
#include <QDebug>


// ******************************************
Editor::Editor() {
// ******************************************
#ifdef DEBUG
    qDebug("Editor::Editor()");
#endif
    shruthiFilterBoard = 0;
}


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
void Editor::setShruthiFilterBoard(int filter)
// ******************************************
{
#ifdef DEBUG
    qDebug() << "Editor::setShruthiFilterBoard:" << filter;
#endif
    Editor::shruthiFilterBoard = filter;
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
}


// ******************************************
QString Editor::getName() {
// ******************************************
    return patch.getName();
}


// ******************************************
void Editor::process(queueitem_t item) {
// ******************************************
    switch(item.action) {
        case NRPN_PROCESS_EDITOR:
            actionProcessEditor(item.int0,item.int1);
            break;
         case SYSEX_FETCH_PATCH:
            actionFetchPatch();
            break;
        case SYSEX_SEND_PATCH:
            actionSendPatch();
            break;
        case SYSEX_VERSION_REQUEST:
            actionVersionRequest();
            break;
        case NRPN_RECEIVED:
            actionNrpnReceived(item.int0,item.int1);
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
        case SYSEX_RECEIVED:
            actionSysexReceived(item.size,item.message);
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
            actionResetPatch(item.int0);
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
}


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
void Editor::actionVersionRequest()
// ******************************************
{
#ifdef DEBUG
    qDebug() << "Editor::actionVersionRequest()";
#endif
    if (midiout.versionRequest()) {
        //emit displayStatusbar("Version request sent.");
        std::cout  << "Version request sent." << std::endl;
    } else {
        //emit displayStatusbar("Could not send version request.");
        std::cout  << "Could not send version request." << std::endl;
    }
}


// ******************************************
void Editor::actionNrpnReceived(int nrpn, int value) {
// ******************************************
#ifdef DEBUG
    qDebug() << "Editor::actionNrpnReceived(" << nrpn << "," << value << ")";
#endif
    if (!Patch::enabled(nrpn)) {
        return;
    }

    if (Patch::parameters[nrpn].min<0 && value>=127)
        value-=256; //2s complement
    patch.setParam(nrpn,value);
    if (Patch::hasUI(nrpn)) {
        emit redrawNRPN(nrpn);
    }
}


// ******************************************
void Editor::actionNoteOn(unsigned char note, unsigned char velocity) {
// ******************************************
#ifdef DEBUG
    qDebug() << "Editor::actionNoteOn(" << channel << "," << note << "," << velocity << ")";
#endif
    if (!midiout.noteOn(channel,note,velocity))
        emit displayStatusbar("Could not send note on message.");
}


// ******************************************
void Editor::actionNoteOff(unsigned char note) {
// ******************************************
#ifdef DEBUG
    qDebug() << "Editor::actionNoteOff(" << channel << "," << note << ")";
#endif
    if (!midiout.noteOff(channel,note))
        emit displayStatusbar("Could not send note off message.");
}


// ******************************************
void Editor::actionNotePanic() {
// ******************************************
#ifdef DEBUG
    qDebug() << "Editor::actionNotePanic(" << channel << ")";
#endif
    if (midiout.allNotesOff(channel))
        emit displayStatusbar("Sent all notes off message.");
    else
        emit displayStatusbar("Could not send all notes off message.");
}


// ******************************************
void Editor::actionSysexReceived(unsigned int size, unsigned char* message) {
// ******************************************
#ifdef DEBUG
    qDebug() << "Editor::actionSysexReceived(" << size << ",...)";
#endif
    if (7<size && message[6]==1 && message[7]==0)
        if (patch.parseFullSysex(message,size))
            emit displayStatusbar("Received valid patch (" + patch.getVersionString() + " format).");
        else
            emit displayStatusbar("Received invalid patch.");
    else {
        emit displayStatusbar("Received unknown sysex.");
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
void Editor::actionResetPatch(unsigned int version) {
// ******************************************
#ifdef DEBUG
    qDebug() << "Editor::actionResetPatch()";
#endif
    patch.resetPatch(version);
    emit redrawAll();
    emit displayStatusbar("Patch reset.");
}


// ******************************************
void Editor::actionRandomizePatch() {
// ******************************************
#ifdef DEBUG
    qDebug() << "Editor::actionRandomizePatch()";
#endif
    patch.randomizePatch(shruthiFilterBoard);
    emit redrawAll();
    emit displayStatusbar("Patch randomized.");
}
