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


#ifndef LIBRARY_DIALOG_H
#define LIBRARY_DIALOG_H


#include <QDialog>
#include "queueitem.h"
class QListWidget;
class QListWidgetItem;
class QMenu;
class QModelIndex;
class QPoint;
class QWidget;
namespace Ui { class LibraryDialog; }


class LibraryDialog : public QDialog {
        Q_OBJECT

    public:
        explicit LibraryDialog(QWidget *parent = 0);
        ~LibraryDialog();

    private:
        LibraryDialog(const LibraryDialog&); //forbid copying
        LibraryDialog &operator=(const LibraryDialog&); //forbid assignment

        void setFont(QListWidgetItem *item, bool edited, bool moved);
        void libraryRange(const QueueAction::QueueAction &action, const int &flags, const int &from, const int &to);
        void move(const int &flags, const int &from, const int &to);
        void store(const int &flags, const int &id);
        void recall(const int &flags, const int &id);

        void librarySelectedRanges(QListWidget *list, const QueueAction::QueueAction &action, const int &flags);

        void prepareLists(int stop);
        void cleanupLists();

        int numberOfPrograms;

        Ui::LibraryDialog *ui;

        QFont normalFont;
        QFont movedFont;
        QFont editedFont;
        QFont editedMovedFont;

        QColor colorOnHW;
        QColor colorNotOnHW;

        QMenu *patchContextMenu;
        QMenu *sequenceContextMenu;

        bool dontCopyScrollBarPosition; // to prevent scroll bar bouncing
        bool dontCopySelection; // to prevent selection bouncing

    public slots:
        void redrawLibraryPatchItem(int id, QString identifier, bool edited, bool moved, bool onHardware);
        void redrawLibrarySequenceItem(int id, QString identifier, bool edited, bool moved, bool onHardware);
        void setNumberOfLibraryPrograms(int num);

    private slots:
        void patchOpenContextMenu(QPoint p);
        void sequenceOpenContextMenu(QPoint p);
        void patchRecall(QListWidgetItem *item);
        void sequenceRecall(QListWidgetItem *item);
        void patchCMStore();
        void sequenceCMStore();
        void patchCMSend();
        void patchCMSendChanged();
        void sequenceCMSend();
        void sequenceCMSendChanged();
        void patchCMFetch();
        void sequenceCMFetch();
        void patchCMRemove();
        void sequenceCMRemove();
        void patchCMReset();
        void sequenceCMReset();
        void patchCMInsertAfter();
        void sequenceCMInsertAfter();
        void patchMove(const QModelIndex & parent, int start, int end, const QModelIndex & destination, int row);
        void sequenceMove(const QModelIndex & parent, int start, int end, const QModelIndex & destination, int row);

        void fetch();
        void send();
        void sendChanged();
        void loadReplace();
        void loadAppend();
        void save();

        void copyScrollBarPositionFromPatchToSequence(int val);
        void copyScrollBarPositionFromSequenceToPatch(int val);
        void copySelectionFromPatchToSequence();
        void copySelectionFromSequenceToPatch();

    signals:
        void enqueue(QueueItem);

};


#endif // LIBRARY_DIALOG_H
