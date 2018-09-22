// Shruthi-Editor: An unofficial Editor for the Shruthi hardware synthesizer. For
// informations about the Shruthi, see <http://www.mutable-instruments.net/shruthi1>.
//
// Copyright (C) 2011-2018 Manuel Krönig
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


#include "signalrouter.h"
#ifdef DEBUGMSGS
#include <QDebug>
#endif


SignalRouter::SignalRouter() {
#ifdef DEBUGMSGS
    qDebug() << "SignalRouter::SignalRouter()";
#endif
    // load config
    config = Config();
    config.load();
}


SignalRouter::~SignalRouter() {
#ifdef DEBUGMSGS
    qDebug() << "SignalRouter::~SignalRouter()";
#endif
    editorEnabled = false;
}


void SignalRouter::run() {
#ifdef DEBUGMSGS
    qDebug() << "SignalRouter::run()";
#endif
    emit setMidiInputPort(config.midiInputPort());
    emit setMidiOutputPort(config.midiOutputPort());
    emit setMidiChannel(config.midiChannel());
    emit setShruthiFilterBoard(config.shruthiFilterBoard());
    editorEnabled = true;
    editorWorking = false;
}


void SignalRouter::enqueue(QueueItem item) {
    if (editorWorking) {
        queue.enqueue(item);
    } else if (editorEnabled) {
        editorWorking = true;

        emit editorProcess(item);
    }
}


void SignalRouter::editorFinished() {
    if (editorEnabled) {
        if (!queue.isEmpty()) {
            emit editorProcess(queue.dequeue());
        } else {
            editorWorking = false;
        }
    }
}


void SignalRouter::settingsChanged(Config conf) {
#ifdef DEBUGMSGS
    qDebug() << "SignalRouter::settingsChanged: in" << conf.midiInputPort() << ", out:" << conf.midiOutputPort() << ", channel:" << conf.midiChannel() << ", filter:" << conf.shruthiFilterBoard();
#endif
    // setMidiInputPort and setMidiOutputPort have to be emited, even if the value didn't change.
    emit setMidiInputPort(conf.midiInputPort());
    emit setMidiOutputPort(conf.midiOutputPort());
    emit setMidiChannel(conf.midiChannel());

    if (config.shruthiFilterBoard() != conf.shruthiFilterBoard()) {
        emit setShruthiFilterBoard(conf.shruthiFilterBoard());
    }

    if (!config.equals(conf)) {
#ifdef DEBUGMSGS
        qDebug() << "Config was changed. Saving.";
#endif
        config.set(conf);
        config.save();
    }
}
