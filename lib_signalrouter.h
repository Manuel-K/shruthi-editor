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

#ifndef SHRUTHI_SIGNALROUTER_H
#define SHRUTHI_SIGNALROUTER_H

#include "lib_config.h"
#include "lib_editor.h"
#include <QQueue>

// ******************************************
class SignalRouter : public QObject {
// ******************************************
    Q_OBJECT

    private:
        Config config;

    public:
        SignalRouter();
        ~SignalRouter();

        bool editorWorking;
        bool editorEnabled;
        QQueue<queueitem_t> queue;

    public slots:
        void run();
        void enqueue(queueitem_t);
        void editorFinished();
        void settingsChanged(int, int, unsigned char, int);

    signals:
        void editorProcess(queueitem_t);
        void setMidiInputPort(int);
        void setMidiOutputPort(int);
        void setMidiChannel(unsigned char);
        void setShruthiFilterBoard(int);
};

#endif
