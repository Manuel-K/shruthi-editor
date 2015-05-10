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

#include <QSettings>
#include "config.h"


// ******************************************
void Config::save() {
// ******************************************
    QSettings settings("Manuel Kroenig", "Shruthi-Editor");
    settings.setValue("midi/inputPort",midiInputPort);
    settings.setValue("midi/outputPort",midiOutputPort);
    settings.setValue("midi/channel",midiChannel);
    settings.setValue("shruthi/filterBoard", shruthiFilterBoard);
}


// ******************************************
void Config::load() {
// ******************************************
    QSettings settings("Manuel Kroenig", "Shruthi-Editor");
    midiInputPort=settings.value("midi/inputPort",0).toInt();
    midiOutputPort=settings.value("midi/outputPort",0).toInt();
    midiChannel=settings.value("midi/channel",0).toInt();
    shruthiFilterBoard=settings.value("shruthi/filterBoard", 0).toInt();
}


// ******************************************
void Config::setMidiInputPort(int in) {
// ******************************************
    midiInputPort=in;
}


// ******************************************
const int &Config::getMidiInputPort() const {
// ******************************************
    return midiInputPort;
}


// ******************************************
void Config::setMidiOutputPort(int out) {
// ******************************************
    midiOutputPort=out;
}


// ******************************************
const int &Config::getMidiOutputPort() const {
// ******************************************
    return midiOutputPort;
}


// ******************************************
void Config::setMidiChannel(unsigned char channel) {
// ******************************************
    midiChannel=channel;
}


// ******************************************
const unsigned char &Config::getMidiChannel() const {
// ******************************************
    return midiChannel;
}


// ******************************************
const int &Config::getShruthiFilterBoard() const
// ******************************************
{
    return shruthiFilterBoard;
}


// ******************************************
void Config::setShruthiFilterBoard(int value)
// ******************************************
{
    shruthiFilterBoard = value;
}
