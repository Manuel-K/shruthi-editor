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
#include "lib_fileio.h"
#include <QDebug>


// ******************************************
Editor::Editor() {
// ******************************************
#ifdef DEBUGMSGS
    qDebug("Editor::Editor()");
#endif
    shruthiFilterBoard = 0;
}


// ******************************************
void Editor::run()
// ******************************************
{
#ifdef DEBUGMSGS
    qDebug("Editor::run()");
#endif
    emit setStatusbarVersionLabel(patch.getVersionString());
}


// ******************************************
bool Editor::setMidiOutputPort(int out) {
// ******************************************
#ifdef DEBUGMSGS
    qDebug() << "Editor::setMidiPorts:" << out;
#endif
    bool status = midiout.open(out);
    emit midiOutputStatusChanged(status);
    return status;
}


// ******************************************
void Editor::setMidiChannel(unsigned char channel) {
// ******************************************
#ifdef DEBUGMSGS
    qDebug() << "Editor::setMidiChannel:" << channel;
#endif
    Editor::channel = channel;
}


// ******************************************
void Editor::setShruthiFilterBoard(int filter)
// ******************************************
{
#ifdef DEBUGMSGS
    qDebug() << "Editor::setShruthiFilterBoard:" << filter;
#endif
    Editor::shruthiFilterBoard = filter;
}


// ******************************************
Editor::~Editor() {
// ******************************************
#ifdef DEBUGMSGS
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
        case PATCH_PARAMETER_CHANGE_EDITOR:
            actionPatchParameterChangeEditor(item.int0, item.int1);
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
        case PATCH_PARAMETER_CHANGE_MIDI:
            actionPatchParameterChangeMidi(item.int0, item.int1);
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
            actionSysexReceived(item.int0, item.int1, item.size, item.message);
            break;
        case SET_PATCHNAME:
            actionSetPatchname(item.string);
            break;
        case FILEIO_LOAD:
            actionFileIOLoad(item.string);
            break;
        case FILEIO_SAVE:
            actionFileIOSave(item.string);
            break;
        case RESET_PATCH:
            actionResetPatch(item.int0);
            break;
        case RANDOMIZE_PATCH:
            actionRandomizePatch();
            break;
        default:
#ifdef DEBUGMSGS
            qDebug() << "Editor::process():" << item.action << ":" << item.int0 << "," << item.int1 << "," << item.string;
#endif
            break;
    }
    emit finished();
}


// ******************************************
void Editor::actionPatchParameterChangeEditor(int nrpn, int value) {
// ******************************************
#ifdef DEBUGMSGS
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
#ifdef DEBUGMSGS
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
#ifdef DEBUGMSGS
    qDebug() << "Editor::actionSendPatch()";
#endif
    std::vector<unsigned char> temp;
    patch.generateSysex(&temp);
    if (midiout.write(temp))
        emit displayStatusbar("Patch sent.");
    else
        emit displayStatusbar("Could not send patch.");
}


// ******************************************
void Editor::actionVersionRequest()
// ******************************************
{
#ifdef DEBUGMSGS
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
void Editor::actionPatchParameterChangeMidi(int nrpn, int value) {
// ******************************************
#ifdef DEBUGMSGS
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
#ifdef DEBUGMSGS
    qDebug() << "Editor::actionNoteOn(" << channel << "," << note << "," << velocity << ")";
#endif
    if (!midiout.noteOn(channel,note,velocity))
        emit displayStatusbar("Could not send note on message.");
}


// ******************************************
void Editor::actionNoteOff(unsigned char note) {
// ******************************************
#ifdef DEBUGMSGS
    qDebug() << "Editor::actionNoteOff(" << channel << "," << note << ")";
#endif
    if (!midiout.noteOff(channel,note))
        emit displayStatusbar("Could not send note off message.");
}


// ******************************************
void Editor::actionNotePanic() {
// ******************************************
#ifdef DEBUGMSGS
    qDebug() << "Editor::actionNotePanic(" << channel << ")";
#endif
    if (midiout.allNotesOff(channel))
        emit displayStatusbar("Sent all notes off message.");
    else
        emit displayStatusbar("Could not send all notes off message.");
}


// ******************************************
void Editor::actionSysexReceived(unsigned int command, unsigned int argument,
                                 unsigned int size, unsigned char* message) {
// ******************************************
#ifdef DEBUGMSGS
    qDebug() << "Editor::actionSysexReceived(" << size << ",...)";
#endif
    if (size == 0 && command == 0 && argument == 0) {
        emit displayStatusbar("Received invalid SysEx.");
    } else if (command == 0x0c && argument == 0x00) {
        // Version info
        //if (size == 2) {
        //    firmwareVersion = message.at(0) * 1000 + message.at(1);
        //}
    } else if (command == 0x01 && argument == 0x00) {
        if (size == 92 && patch.unpackData(message)) {
            emit displayStatusbar("Received valid patch (" + patch.getVersionString() + " format).");
        } else {
            emit displayStatusbar("Received invalid patch.");
        }
    } else {
        emit displayStatusbar("Received unknown sysex.");
#ifdef DEBUGMSGS
        qDebug() << "unknown sysex type";
#endif
    }
    if (message) { // TODO: is it a good idea to check if size > 0
        delete message;
    }
    emit redrawAll();
    emit setStatusbarVersionLabel(patch.getVersionString());
}


// ******************************************
void Editor::actionSetPatchname(QString name) {
// ******************************************
#ifdef DEBUGMSGS
    qDebug() << "Editor::actionSetPatchname(" << name << ")";
#endif
    patch.setName(name);
    emit displayStatusbar("Patch name set.");
}


// ******************************************
void Editor::actionFileIOLoad(QString filename) {
// ******************************************
    std::vector<unsigned char> temp;
    bool status = FileIO::loadFromDisk(filename, temp);

    if (status) {
        const unsigned int &readBytes = temp.size();
        // primitive check if patch is valid:
        if (readBytes == 195) {
#ifdef DEBUGMSGS
            qDebug() << "Detected full patch sysex.";
#endif
            status = patch.parseSysex(&temp);
        } else if (readBytes == 92) {
#ifdef DEBUGMSGS
            qDebug() << "Detected light patch files.";
#endif
            unsigned char data[92];
            for (unsigned int i=0; i<readBytes; i++) {
                data[i] = (char) temp[i];
#ifdef DEBUGMSGS
                qDebug() << i << ":" << sysex[i];
#endif
            }
            status = patch.unpackData(data);
        } else {
            status = false;
        }
    }

#ifdef DEBUGMSGS
    qDebug() << "Editor::actionLoadPatch(" << filename << "):" << status;
#endif
    if (status) {
        emit displayStatusbar("Patch loaded from disk.");
        emit redrawAll();
        emit setStatusbarVersionLabel(patch.getVersionString());
    } else {
        emit displayStatusbar("Could not load patch.");
    }
}


// ******************************************
void Editor::actionFileIOSave(QString filename) {
// ******************************************
    QByteArray ba;

    if (filename.endsWith(".syx")) {
        std::vector<unsigned char> temp;
        patch.generateSysex(&temp);
        FileIO::appendToByteArray(temp, ba);
    } else {
        unsigned char data[92];
        patch.packData(data);
        FileIO::appendToByteArray(data, 92, ba);
    }

    bool status = FileIO::saveToDisk(filename, ba);
#ifdef DEBUGMSGS
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
#ifdef DEBUGMSGS
    qDebug() << "Editor::actionResetPatch()";
#endif
    patch.resetPatch(version);
    emit redrawAll();
    emit displayStatusbar("Patch reset.");
    emit setStatusbarVersionLabel(patch.getVersionString());
}


// ******************************************
void Editor::actionRandomizePatch() {
// ******************************************
#ifdef DEBUGMSGS
    qDebug() << "Editor::actionRandomizePatch()";
#endif
    patch.randomizePatch(shruthiFilterBoard);
    emit redrawAll();
    emit displayStatusbar("Patch randomized.");
    emit setStatusbarVersionLabel(patch.getVersionString());
}

