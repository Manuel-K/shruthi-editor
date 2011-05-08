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

#include <QSettings>
#include "lib_config.h"

// ******************************************
void Config::save() {
// ******************************************
    QSettings settings("Manuel Kroenig", "Shruthi-Editor");
    settings.setValue("midi/inputDevice",midiInputDevice);
    settings.setValue("midi/outputDevice",midiOutputDevice);
};

// ******************************************
void Config::load() {
// ******************************************
    QSettings settings("Manuel Kroenig", "Shruthi-Editor");
    midiInputDevice=settings.value("midi/inputDevice",1).toInt();
    midiOutputDevice=settings.value("midi/outputDevice",0).toInt();
};

// ******************************************
void Config::setMidiInputDevice(int in) {
// ******************************************
    midiInputDevice=in;
};

// ******************************************
void Config::setMidiOutputDevice(int out) {
// ******************************************
    midiOutputDevice=out;
};

// ******************************************
int Config::getMidiInputDevice() {
// ******************************************
    return midiInputDevice;
};

// ******************************************
int Config::getMidiOutputDevice() {
// ******************************************
    return midiOutputDevice;
};