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


#ifndef SHRUTHI_CONFIG_H
#define SHRUTHI_CONFIG_H


class Config {
    public:
        Config();
        void load();
        void save();
        const int &midiInputPort() const;
        void setMidiInputPort(int in);
        const int &midiOutputPort() const;
        void setMidiOutputPort(int out);
        const unsigned char &midiChannel() const;
        void setMidiChannel(unsigned char);
        const int &shruthiFilterBoard() const;
        void setShruthiFilterBoard(int value);
        void set(const Config &other);
        bool equals(const Config &other);

    private:
        int mMidiInputPort;
        int mMidiOutputPort;
        unsigned char mMidiChannel;
        int mShruthiFilterBoard;
};


#endif // SHRUTHI_CONFIG_H
