// Shruthi-Editor: An unofficial Editor for the Shruthi hardware synthesizer. For
// informations about the Shruthi, see <http://www.mutable-instruments.net/shruthi1>.
//
// Copyright (C) 2011-2019 Manuel Krönig
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


#include "config.h"
#include <QSettings>


Config::Config() {
    mMidiInputPort = 0;
    mMidiOutputPort = 0;
    mMidiChannel = 0;
    mShruthiFilterBoard = 0;
}


void Config::save() {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "shruthi-editor", "Shruthi-Editor");
    settings.setValue("midi/inputPort", mMidiInputPort);
    settings.setValue("midi/outputPort", mMidiOutputPort);
    settings.setValue("midi/channel", mMidiChannel);
    settings.setValue("shruthi/filterBoard", mShruthiFilterBoard);
}


void Config::load() {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "shruthi-editor", "Shruthi-Editor");
    mMidiInputPort = settings.value("midi/inputPort", 0).toInt();
    mMidiOutputPort = settings.value("midi/outputPort", 0).toInt();
    mMidiChannel = settings.value("midi/channel", 0).toInt();
    mShruthiFilterBoard = settings.value("shruthi/filterBoard", 0).toInt();
}


void Config::setMidiInputPort(int in) {
    mMidiInputPort = in;
}


const int &Config::midiInputPort() const {
    return mMidiInputPort;
}


void Config::setMidiOutputPort(int out) {
    mMidiOutputPort = out;
}


const int &Config::midiOutputPort() const {
    return mMidiOutputPort;
}


void Config::setMidiChannel(unsigned char channel) {
    mMidiChannel = channel;
}


const unsigned char &Config::midiChannel() const {
    return mMidiChannel;
}


const int &Config::shruthiFilterBoard() const {
    return mShruthiFilterBoard;
}


void Config::setShruthiFilterBoard(int value) {
    mShruthiFilterBoard = value;
}


void Config::set(const Config &other) {
    mMidiChannel = other.mMidiChannel;
    mMidiInputPort = other.mMidiInputPort;
    mMidiOutputPort = other.mMidiOutputPort;
    mShruthiFilterBoard = other.mShruthiFilterBoard;
}


bool Config::equals(const Config &other) {
    return mMidiChannel == other.mMidiChannel &&
            mMidiInputPort == other.mMidiInputPort &&
            mMidiOutputPort == other.mMidiOutputPort &&
            mShruthiFilterBoard == other.mShruthiFilterBoard;
}
