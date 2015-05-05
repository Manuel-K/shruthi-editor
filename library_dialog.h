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



#ifndef LIBRARY_DIALOG_H
#define LIBRARY_DIALOG_H

#include "lib_editor.h"

#include <QDialog>
#include <QListWidgetItem>
namespace Ui {
class LibraryDialog;
}

class QFont;


// ******************************************
class LibraryDialog : public QDialog {
// ******************************************
        Q_OBJECT

    public:
        explicit LibraryDialog(Editor *edit, QWidget *parent = 0);
        ~LibraryDialog();

    private:
        void setFont(QListWidgetItem *item, bool edited, bool moved);
        void sendRange(const int &flags, const int &from, const int &to);
        void move(const int &flags, const int &from, const int &to);
        void store(const int &flags, const int &id);
        void recall(const int &flags, const int &id);

        Ui::LibraryDialog *ui;
        Editor *editor;

        QFont normalFont;
        QFont movedFont;
        QFont editedFont;
        QFont editedMovedFont;

        QMenu *patchContextMenu;
        QMenu *sequenceContextMenu;

        bool dontScroll; // to prevent scroll bar bouncing
        bool dontSyncSelection; // to prevent selection bouncing

    public slots:
        void redrawItems(int what, int start, int stop);

    private slots:
        void patchOpenContextMenu(QPoint p);
        void sequenceOpenContextMenu(QPoint p);
        void patchRecall(QListWidgetItem *item);
        void sequenceRecall(QListWidgetItem *item);
        void patchStore();
        void sequenceStore();
        void patchMove(const QModelIndex & parent, int start, int end, const QModelIndex & destination, int row);
        void sequenceMove(const QModelIndex & parent, int start, int end, const QModelIndex & destination, int row);

        void fetch();
        void sendSelected();
        void syncToSequenceScrollBar(int val);
        void syncToPatchScrollBar(int val);
        void syncToSequenceSelection();
        void syncToPatchSelection();

    signals:
        void enqueue(queueitem_t);

};

#endif // LIBRARY_DIALOG_H
