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


#include "ui/library_dialog.h"
#include "ui_library_dialog.h"
#include "library.h"

#include <QDebug>
#include <QScrollBar>
#include <QMenu>
#include <QFileDialog>
#include <QMessageBox>


LibraryDialog::LibraryDialog(const Library *lib, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LibraryDialog),
    library(lib) {
    ui->setupUi(this);

    ui->patchList->addItem("If you can read this,");
    ui->sequenceList->addItem("something is wrong!");

    dontScroll = false;
    dontSyncSelection = false;

    // Setup fonts
    normalFont = ui->patchList->item(0)->font();
    normalFont.setBold(false);
    normalFont.setItalic(false);
    movedFont = normalFont;
    movedFont.setItalic(true);
    editedFont = normalFont;
    editedFont.setBold(true);
    editedMovedFont = editedFont;
    editedMovedFont.setItalic(true);

    colorOnHW.setNamedColor("black");
    colorNotOnHW.setNamedColor("grey");

    patchContextMenu = new QMenu(this);
    patchContextMenu->addAction("Store", this, SLOT(patchCMStore()));
    patchContextMenu->addSeparator();
    patchContextMenu->addAction("Insert Program After", this, SLOT(patchCMInsert()));
    patchContextMenu->addAction("Delete Program", this, SLOT(patchCMDelete()));
    patchContextMenu->addSeparator();
    patchContextMenu->addAction("Send", this, SLOT(patchCMSend()));
    patchContextMenu->addAction("Send Changed", this, SLOT(patchCMSendChanged()));

    sequenceContextMenu = new QMenu(this);
    sequenceContextMenu->addAction("Store", this, SLOT(sequenceCMStore()));
    sequenceContextMenu->addSeparator();
    sequenceContextMenu->addAction("Insert Program After", this, SLOT(sequenceCMInsert()));
    sequenceContextMenu->addAction("Delete Program", this, SLOT(sequenceCMDelete()));
    sequenceContextMenu->addSeparator();
    sequenceContextMenu->addAction("Send", this, SLOT(sequenceCMSend()));
    sequenceContextMenu->addAction("Send Changed", this, SLOT(sequenceCMSendChanged()));

    connect(ui->patchList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(patchRecall(QListWidgetItem*)));
    connect(ui->patchList, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(patchOpenContextMenu(QPoint)));
    connect(ui->patchList->model(), SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)), this, SLOT(patchMove(QModelIndex,int,int,QModelIndex,int)));
    connect(ui->sequenceList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(sequenceRecall(QListWidgetItem*)));
    connect(ui->sequenceList, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(sequenceOpenContextMenu(QPoint)));
    connect(ui->sequenceList->model(), SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)), this, SLOT(sequenceMove(QModelIndex,int,int,QModelIndex,int)));

    connect(ui->fetch, SIGNAL(clicked(bool)), this, SLOT(fetch()));
    connect(ui->send, SIGNAL(clicked()), this, SLOT(send()));
    connect(ui->sendChanged, SIGNAL(clicked()), this, SLOT(sendChanged()));
    connect(ui->loadReplace, SIGNAL(clicked()), this, SLOT(loadReplace()));
    connect(ui->loadAppend, SIGNAL(clicked()), this, SLOT(loadAppend()));
    connect(ui->save, SIGNAL(clicked()), this, SLOT(save()));

    // synchronize list widget scrolling:
    connect(ui->patchList->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(syncToSequenceScrollBar(int)));
    connect(ui->sequenceList->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(syncToPatchScrollBar(int)));
    // synchronize selection changes:
    connect(ui->patchList, SIGNAL(itemSelectionChanged()), this, SLOT(syncToSequenceSelection()));
    connect(ui->sequenceList, SIGNAL(itemSelectionChanged()), this, SLOT(syncToPatchSelection()));
}


LibraryDialog::~LibraryDialog() {
    delete ui;
}


void LibraryDialog::setFont(QListWidgetItem *item, bool edited, bool moved) {
    if (edited) {
        if (moved) {
            item->setFont(editedMovedFont);
        } else {
            item->setFont(editedFont);
        }
    } else {
        if (moved) {
            item->setFont(movedFont);
        } else {
            item->setFont(normalFont);
        }
    }
}


void LibraryDialog::redrawItems(int what, int start, int stop) {
    // TODO: set text immediately or ignore what
    if ((what&Library::FLAG_PATCH) || (what&Library::FLAG_SEQUENCE)) {
        // add missing items
        const int &amount = stop - ui->patchList->count() + 1;
        if (amount > 0) {
            //std::cout << "amount: " << amount << std::endl;
            for (int i = 0; i < amount; i++) {
                ui->patchList->addItem(" ");
                ui->sequenceList->addItem(" ");
            }
        }
    }

    if (what&Library::FLAG_PATCH) {
        QListWidgetItem *item;
        // setup items
        //std::cout << "redrawPatchNames " <<  start << " " << stop << std::endl;
        for (int i = start; i <= stop; i++) {
            item = ui->patchList->item(i);
            item->setText(QString("%1: ").arg(i+1, 3, 10, QChar(' ')) + library->getPatchIdentifier(i));

            const bool &e = library->patchEdited(i);
            const bool &m = library->patchMoved(i);
            setFont(item, e, m);
            item->setTextColor(i < library->getNumberOfHWPrograms() ? colorOnHW : colorNotOnHW);
        }
    }
    if (what&Library::FLAG_SEQUENCE) {
        QListWidgetItem *item;
        // setup items
        //std::cout << "redrawSequenceInfo " <<  start << " " << stop << std::endl;
        for (int i = start; i <= stop; i++) {
            item = ui->sequenceList->item(i);
            item->setText(QString("%1: ").arg(i+1, 3, 10, QChar(' ')) + library->getSequenceIdentifier(i));

            const bool &e = library->sequenceEdited(i);
            const bool &m = library->sequenceMoved(i);
            setFont(item, e, m);
            item->setTextColor(i < library->getNumberOfHWPrograms() ? colorOnHW : colorNotOnHW);
        }
    }

    // cleanup items:
    if (ui->patchList->count() - library->getNumberOfPrograms() > 0) {
        for (int i = ui->patchList->count() - 1; i >= library->getNumberOfPrograms(); i--) {
            delete ui->patchList->takeItem(i);
            delete ui->sequenceList->takeItem(i);
        }
    }
}


void LibraryDialog::recall(const int &flags, const int &id) {
    QueueItem signal(QueueAction::LIBRARY_RECALL);
    signal.int0 = flags;
    signal.int1 = id;
    emit enqueue(signal);
}


void LibraryDialog::move(const int &flags, const int &from, const int &to) {
    QueueItem signal(QueueAction::LIBRARY_MOVE);
    signal.int0 = flags;
    signal.int1 = from;
    signal.int2 = to;

    emit enqueue(signal);
}


void LibraryDialog::store(const int &flags, const int &id) {
    QueueItem signal(QueueAction::LIBRARY_STORE);
    signal.int0 = flags;
    signal.int1 = id;
    emit enqueue(signal);
}


void LibraryDialog::fetch() {
    QueueItem signal(QueueAction::LIBRARY_FETCH);
    signal.int0 = Library::FLAG_PATCH | Library::FLAG_SEQUENCE;
    signal.int1 = 0;
    signal.int2 = -1;
    emit enqueue(signal);
}


void LibraryDialog::send() {
    const int &b = QMessageBox::question(this, "Send Programs", "This will overwrite the programs on the connected Shruthi.\nAre you sure?", "Yes", "No");
    if (b) {
        return;
    }
    QueueItem signal(QueueAction::LIBRARY_SEND);
    signal.int0 = Library::FLAG_PATCH | Library::FLAG_SEQUENCE;
    signal.int1 = 0;
    signal.int2 = -1;
    emit enqueue(signal);
}


void LibraryDialog::sendChanged(){
    const int &b = QMessageBox::question(this, "Send Changed Programs", "This will overwrite the programs on the connected Shruthi.\nAre you sure?", "Yes", "No");
    if (b) {
        return;
    }
    QueueItem signal(QueueAction::LIBRARY_SEND);
    signal.int0 = Library::FLAG_PATCH | Library::FLAG_SEQUENCE | Library::FLAG_CHANGED;
    signal.int1 = 0;
    signal.int2 = -1;
    emit enqueue(signal);
}


void LibraryDialog::loadReplace() {
    QString path = QFileDialog::getOpenFileName(this, "Load Library", ".", "SysEx files (*.syx)");
    if (path != "") {
        QueueItem signal(QueueAction::LIBRARY_LOAD, path, Library::FLAG_PATCH | Library::FLAG_SEQUENCE);
        emit enqueue(signal);
    }
}


void LibraryDialog::loadAppend() {
    QString path = QFileDialog::getOpenFileName(this, "Load Library", ".", "SysEx files (*.syx)");
    if (path != "") {
        QueueItem signal(QueueAction::LIBRARY_LOAD, path, Library::FLAG_PATCH | Library::FLAG_SEQUENCE | Library::FLAG_APPEND);
        emit enqueue(signal);
    }
}


void LibraryDialog::save() {
    QString path = QFileDialog::getSaveFileName(this, "Save Library", ".", "SysEx files (*.syx)");
    if (path != "") {
        if (!path.endsWith(".syx", Qt::CaseInsensitive)) {
            path.append(".syx");
        }

        QueueItem signal(QueueAction::LIBRARY_SAVE, path, Library::FLAG_PATCH | Library::FLAG_SEQUENCE);
        emit enqueue(signal);
    }
}


void LibraryDialog::libraryRange(const QueueAction::QueueAction &action, const int &flags, const int &from, const int &to) {
#ifdef DEBUGMSGS
    qDebug() << "LibraryDialog::libraryRange() " << flags << from << to;
#endif
    QueueItem signal(action);
    signal.int0 = flags;
    signal.int1 = from;
    signal.int2 = to;
    emit enqueue(signal);
}


void LibraryDialog::librarySelectedRanges(QListWidget *list, const QueueAction::QueueAction &action, const int &flags) {
    if (list->currentRow() < 0) {
        // no selection
        return;
    }

    // determine selection manually:
    int rows = list->count();
    if (rows < 16) {
        return; // should have at least the internal patches!
    }

    bool last = list->item(0)->isSelected();
    bool inside = false;
    int start = 0;
    for (int i = 1; i < rows; i++) {
        const bool &current = list->item(i)->isSelected();
        if (current && !last) {
            inside = true;
            start = i;
        }

        if (!current && last) {
            inside = false;
            libraryRange(action, flags, start, i - 1);
        }

        last = current;
    }
    if (inside) {
        libraryRange(action, flags, start, rows - 1);
    }
}


//
// UI Slots:
//


void LibraryDialog::patchOpenContextMenu(QPoint p) {
#ifdef DEBUGMSGS
    qDebug() << "LibraryDialog::patchOpenContextMenu";
#endif
    patchContextMenu->popup(ui->patchList->mapToGlobal(p));
}


void LibraryDialog::sequenceOpenContextMenu(QPoint p) {
#ifdef DEBUGMSGS
    qDebug() << "LibraryDialog::sequenceOpenContextMenu";
#endif
    sequenceContextMenu->popup(ui->sequenceList->mapToGlobal(p));
}


void LibraryDialog::patchRecall(QListWidgetItem *item) {
    Q_UNUSED(item);
    const int &c = ui->patchList->currentRow();

    int flag = Library::FLAG_PATCH;
    if (ui->sync->isChecked()) {
        flag |= Library::FLAG_SEQUENCE;
    }
    recall(flag, c);
}


void LibraryDialog::sequenceRecall(QListWidgetItem *item) {
    Q_UNUSED(item);
    const int &c = ui->sequenceList->currentRow();

    int flag = Library::FLAG_SEQUENCE;
    if (ui->sync->isChecked()) {
        flag |= Library::FLAG_PATCH;
    }
    recall(flag, c);

}


void LibraryDialog::patchCMStore() {
    const int &c = ui->patchList->currentRow();

    int flag = Library::FLAG_PATCH;
    if (ui->sync->isChecked()) {
        flag |= Library::FLAG_SEQUENCE;
    }
    store(flag, c);
}


void LibraryDialog::sequenceCMStore() {
    const int &c = ui->sequenceList->currentRow();

    int flag = Library::FLAG_SEQUENCE;
    if (ui->sync->isChecked()) {
        flag |= Library::FLAG_PATCH;
    }
    store(flag, c);
}


void LibraryDialog::patchCMSend() {
    int flag = Library::FLAG_PATCH;
    if (ui->sync->isChecked()) {
        flag |= Library::FLAG_SEQUENCE;
    }
    librarySelectedRanges(ui->patchList, QueueAction::LIBRARY_SEND, flag);
}


void LibraryDialog::patchCMSendChanged() {
    int flag = Library::FLAG_PATCH | Library::FLAG_CHANGED;
    if (ui->sync->isChecked()) {
        flag |= Library::FLAG_SEQUENCE;
    }
    librarySelectedRanges(ui->patchList, QueueAction::LIBRARY_SEND, flag);
}


void LibraryDialog::sequenceCMSend() {
    int flag = Library::FLAG_SEQUENCE;
    if (ui->sync->isChecked()) {
        flag |= Library::FLAG_PATCH;
    }
    librarySelectedRanges(ui->patchList, QueueAction::LIBRARY_SEND, flag);
}


void LibraryDialog::sequenceCMSendChanged() {
    int flag = Library::FLAG_SEQUENCE | Library::FLAG_CHANGED;
    if (ui->sync->isChecked()) {
        flag |= Library::FLAG_PATCH;
    }
    librarySelectedRanges(ui->patchList, QueueAction::LIBRARY_SEND, flag);
}


void LibraryDialog::patchCMDelete() {
#ifdef DEBUGMSGS
    qDebug() << "LibraryDialog::patchCMDelete()";
#endif
    librarySelectedRanges(ui->patchList, QueueAction::LIBRARY_DELETE, 0);
}


void LibraryDialog::sequenceCMDelete() {
#ifdef DEBUGMSGS
    qDebug() << "LibraryDialog::patchCMDelete()";
#endif
    librarySelectedRanges(ui->sequenceList, QueueAction::LIBRARY_DELETE, 0);
}


void LibraryDialog::patchCMInsert() {
#ifdef DEBUGMSGS
    qDebug() << "LibraryDialog::patchCMInsert()";
#endif
    QueueItem item(QueueAction::LIBRARY_INSERT, ui->patchList->currentRow());
    emit enqueue(item);
}


void LibraryDialog::sequenceCMInsert() {
#ifdef DEBUGMSGS
    qDebug() << "LibraryDialog::sequenceCMInsert()";
#endif
    QueueItem item(QueueAction::LIBRARY_INSERT, ui->patchList->currentRow());
    emit enqueue(item);
}


void LibraryDialog::patchMove(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row) {
    Q_UNUSED(parent);
    Q_UNUSED(destination);

    if (start != end) {
        qDebug() << "We have a serious problem";
        return;
    }

    syncToSequenceSelection();

    int flag = Library::FLAG_PATCH;
    if (ui->sync->isChecked()) {
        flag |= Library::FLAG_SEQUENCE;
    }
    move(flag, start, row);
}


void LibraryDialog::sequenceMove(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row) {
    Q_UNUSED(parent);
    Q_UNUSED(destination);

    if (start != end) {
        qDebug() << "We have a serious problem";
        return;
    }

    syncToPatchSelection();

    int flag = Library::FLAG_SEQUENCE;
    if (ui->sync->isChecked()) {
        flag |= Library::FLAG_PATCH;
    }
    move(flag, start, row);
}


void LibraryDialog::syncToSequenceScrollBar(int val) {
    if (dontScroll || !ui->sync->isChecked()) {
        return;
    }
    dontScroll = true;
    ui->sequenceList->verticalScrollBar()->setValue(val);
    dontScroll = false;
}


void LibraryDialog::syncToPatchScrollBar(int val) {
    if (dontScroll || !ui->sync->isChecked()) {
        return;
    }
    dontScroll = true;
    ui->patchList->verticalScrollBar()->setValue(val);
    dontScroll = false;
}


void LibraryDialog::syncToSequenceSelection() {
    if (dontSyncSelection || !ui->sync->isChecked()) {
        return;
    }

    dontSyncSelection = true;
    int rows = ui->patchList->count();
    for (int i = 0; i < rows; i++) {
        ui->sequenceList->item(i)->setSelected(ui->patchList->item(i)->isSelected());
    }
    dontSyncSelection = false;
}


void LibraryDialog::syncToPatchSelection() {
    if (dontSyncSelection || !ui->sync->isChecked()) {
        return;
    }

    dontSyncSelection = true;
    int rows = ui->sequenceList->count();
    for (int i = 0; i < rows; i++) {
        ui->patchList->item(i)->setSelected(ui->sequenceList->item(i)->isSelected());
    }
    dontSyncSelection = false;
}
