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

#include "lib_signalrouter.h"

// #ifdef DEBUG
#include <QDebug>
// #endif


// ******************************************
SignalRouter::SignalRouter() {
// ******************************************
#ifdef DEBUG
    qDebug() << "SignalRouter::SignalRouter()";
#endif
    // load config
    config = Config();
    config.load();
}


// ******************************************
SignalRouter::~SignalRouter() {
// ******************************************
#ifdef DEBUG
    qDebug() << "SignalRouter::~SignalRouter()";
#endif
    editorEnabled=false;
}


// ******************************************
void SignalRouter::run() {
// ******************************************
#ifdef DEBUG
    qDebug() << "SignalRouter::run()";
#endif
    emit setMidiInputPort(config.getMidiInputPort());
    emit setMidiOutputPort(config.getMidiOutputPort());
    emit setMidiChannel(config.getMidiChannel());
    editorEnabled=true;
    editorWorking=false;
}


// ******************************************
void SignalRouter::enqueue(queueitem_t item) {
// ******************************************
    if (editorWorking)
        queue.enqueue(item);
    else if (editorEnabled) {
        editorWorking=true;

        emit editorProcess(item);
    }
}


// ******************************************
void SignalRouter::editorFinished() {
// ******************************************
    if (editorEnabled) {
        if (!queue.isEmpty())
            emit editorProcess(queue.dequeue());
        else
            editorWorking=false;
    }
}


// ******************************************
void SignalRouter::settingsChanged(int in, int out, unsigned char channel) {
// ******************************************
#ifdef DEBUG
    qDebug() << "SignalRouter::settingsChanged: in" << in << ", out:" << out << ", channel:" << channel;
#endif
    // setMidiInputPort and setMidiOutputPort have to be emited, even if the value didn't change.
    emit setMidiInputPort(in);
    emit setMidiOutputPort(out);
    emit setMidiChannel(channel);

    bool writeConfig = false;
    if (config.getMidiInputPort() != in || config.getMidiOutputPort() != out) {
        config.setMidiInputPort(in);
        config.setMidiOutputPort(out);
        writeConfig = true;
    }
    if (config.getMidiChannel() != channel) {
        config.setMidiChannel(channel);
        writeConfig = true;
    }
    if (writeConfig) {
#ifdef DEBUG
        qDebug() << "Config was changed. Saving.";
#endif
        config.save();
    }
}
