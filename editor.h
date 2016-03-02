// Shruthi-Editor: An unofficial Editor for the Shruthi hardware synthesizer. For
// informations about the Shruthi, see <http://www.mutable-instruments.net/shruthi1>.
//
// Copyright (C) 2011-2016 Manuel Krönig
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


#ifndef SHRUTHI_EDITOR_H
#define SHRUTHI_EDITOR_H


#include <QObject>
#include "queueitem.h"
class Library;
class MidiOut;
class Patch;
class Sequence;


class Editor : public QObject {
        Q_OBJECT

    public:
        Editor();
        ~Editor();

    private:
        Editor(const Editor&); //forbid copying
        Editor &operator=(const Editor&); //forbid assignment

        void actionPatchParameterChangeEditor(int id, int value);
        void actionFetchRequest(const int &what);
        void actionSendData(const int &what);
        void actionShruthiInfoRequest();
        void actionPatchParameterChangeMidi(int id, int value);
        void actionNoteOn(unsigned char note, unsigned char velocity);
        void actionNoteOff(unsigned char note);
        void actionNotePanic();
        void actionSysexReceived(unsigned int command, unsigned int argument, unsigned int size, unsigned char* message);
        void actionSetPatchname(QString name);
        void actionFileIOLoad(QString path, const int &what);
        void actionFileIOSave(QString path, const int &what);
        void actionResetPatch(unsigned int version);
        void actionRandomizePatch();
        void actionSequenceParameterChangeEditor(const unsigned &id, const int &value);
        void actionResetSequence();

        void actionLibraryFetch(const unsigned int &what, const int &start, const int &stop);
        void actionLibrarySend(const unsigned int &what, const int &start, const int &end);
        void actionLibrarySendReturnHandler(const bool &ret);
        void actionLibraryRecall(const unsigned int &what, const unsigned int &id);
        void actionLibraryStore(const unsigned int &what, const unsigned int &id);
        void actionLibraryMove(const unsigned int &what, const unsigned int &start, const unsigned int &target);
        void actionLibraryLoad(const QString &path, const int &flags);
        void actionLibrarySave(const QString &path, const int &flags);
        void actionLibraryRemove(const unsigned int &start, const unsigned int &end);
        void actionLibraryInsert(const unsigned int &id);
        void actionLibraryReset(const unsigned int &flags, const unsigned int &start, const unsigned int &end);

        void redrawAllPatchParameters();
        void redrawAllSequenceParameters();
        void redrawLibraryItems(int what, int start, int stop);

        MidiOut *midiout;
        Patch *patch;
        Sequence *sequence;
        Library *library;
        unsigned char channel;
        int shruthiFilterBoard;
        int firmwareVersion;

    public slots:
        void process(QueueItem item);
        bool setMidiOutputPort(int out);
        void setMidiChannel(unsigned char channel);
        void setShruthiFilterBoard(int filter);
        void run();
        void librarySendNext();

    signals:
        void redrawPatchParameter(int,int);
        void redrawPatchName(QString);
        void redrawSequenceParameter(int);
        void redrawSequenceStep(int,int,int,int,int,int);
        void finished();
        void midiOutputStatusChanged(bool);
        void displayStatusbar(QString);
        void setStatusbarVersionLabel(QString);
        void redrawLibraryPatchItem(int,QString,bool,bool,bool);
        void redrawLibrarySequenceItem(int,QString,bool,bool,bool);
        void setNumberOfLibraryPrograms(int);
};


#endif // SHRUTHI_EDITOR_H
