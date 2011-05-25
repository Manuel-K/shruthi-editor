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

#include "lib_signalrouter.h"



SignalRouter::SignalRouter() {
#ifdef DEBUG
    qDebug() << "SignalRouter::SignalRouter()";
#endif
    // load config
    config = Config();
    config.load();
}
SignalRouter::~SignalRouter() {
#ifdef DEBUG
    qDebug() << "SignalRouter::~SignalRouter()";
#endif
    editorEnabled=false;
}


void SignalRouter::run() {
    emit setMidiDevices(config.getMidiInputDevice(),config.getMidiOutputDevice());
    editorEnabled=true;
    editorWorking=false;
}
void SignalRouter::enqueue(queueitem_t item) {
    if (editorWorking)
        queue.enqueue(item);
    else if (editorEnabled) {
        editorWorking=true;
        
        emit editorProcess(item);
    }
}
void SignalRouter::editorFinished() {
    if (editorEnabled) {
        if (!queue.isEmpty())
            emit editorProcess(queue.dequeue());
        else
            editorWorking=false;
    }
}


void SignalRouter::midiDeviceChanged(int in, int out) {
#ifdef DEBUG
    qDebug() << "midiDeviceChanged: in" << in << ", out:" <<out;
#endif
    emit setMidiDevices(in,out);
    bool config_changed = (config.getMidiInputDevice() != in) || (config.getMidiOutputDevice() != out);
    if (config_changed) {
        config.setMidiInputDevice(in);
        config.setMidiOutputDevice(out);
#ifdef DEBUG
        qDebug() << "Config was changed. Saving.";
#endif
        config.save();
    }
}
