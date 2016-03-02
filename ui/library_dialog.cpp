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


#include "ui/library_dialog.h"
#include "ui_library_dialog.h"
#include <QDebug>
#include <QFileDialog>
#include <QMenu>
#include <QMessageBox>
#include <QScrollBar>
#include "flag.h"


LibraryDialog::LibraryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LibraryDialog) {
    ui->setupUi(this);

    ui->patchList->addItem("If you can read this,");
    ui->sequenceList->addItem("something is wrong!");

    dontCopyScrollBarPosition = false;
    dontCopySelection = false;

    numberOfPrograms = 0;

    // Setup fonts
    normalFont = QFont("Monospace");
    normalFont.setStyleHint(QFont::Monospace);
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
    patchContextMenu->addAction("Insert Program After", this, SLOT(patchCMInsertAfter()));
    patchContextMenu->addAction("Delete Program", this, SLOT(patchCMRemove()));
    patchContextMenu->addAction("Reset", this, SLOT(patchCMReset()));
    patchContextMenu->addSeparator();
    patchContextMenu->addAction("Fetch", this, SLOT(patchCMFetch()));
    patchContextMenu->addAction("Send", this, SLOT(patchCMSend()));
    patchContextMenu->addAction("Send Changed", this, SLOT(patchCMSendChanged()));

    sequenceContextMenu = new QMenu(this);
    sequenceContextMenu->addAction("Store", this, SLOT(sequenceCMStore()));
    sequenceContextMenu->addSeparator();
    sequenceContextMenu->addAction("Insert Program After", this, SLOT(sequenceCMInsertAfter()));
    sequenceContextMenu->addAction("Delete Program", this, SLOT(sequenceCMRemove()));
    sequenceContextMenu->addAction("Reset", this, SLOT(sequenceCMReset()));
    sequenceContextMenu->addSeparator();
    sequenceContextMenu->addAction("Fetch", this, SLOT(sequenceCMFetch()));
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
    connect(ui->patchList->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(copyScrollBarPositionFromPatchToSequence(int)));
    connect(ui->sequenceList->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(copyScrollBarPositionFromSequenceToPatch(int)));
    // synchronize selection changes:
    connect(ui->patchList, SIGNAL(itemSelectionChanged()), this, SLOT(copySelectionFromPatchToSequence()));
    connect(ui->sequenceList, SIGNAL(itemSelectionChanged()), this, SLOT(copySelectionFromSequenceToPatch()));
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


void LibraryDialog::prepareLists(int stop) {
    // add missing items:
    const int &amount = stop - ui->patchList->count() + 1;
    if (amount > 0) {
        //std::cout << "amount: " << amount << std::endl;
        for (int i = 0; i < amount; i++) {
            ui->patchList->addItem(" ");
            ui->sequenceList->addItem(" ");
        }
    }
}


void LibraryDialog::cleanupLists() {
    // cleanup items:
    if (ui->patchList->count() - numberOfPrograms > 0) {
        for (int i = ui->patchList->count() - 1; i >= numberOfPrograms; i--) {
            delete ui->patchList->takeItem(i);
            delete ui->sequenceList->takeItem(i);
        }
    }
}


void LibraryDialog::redrawLibraryPatchItem(int id, QString identifier, bool edited, bool moved, bool onHardware) {
    prepareLists(id);

    QListWidgetItem *item = ui->patchList->item(id);
    item->setText(QString("%1: ").arg(id+1, 3, 10, QChar(' ')) + identifier);

    setFont(item, edited, moved);
    item->setTextColor(onHardware ? colorOnHW : colorNotOnHW);

    cleanupLists();
}


void LibraryDialog::redrawLibrarySequenceItem(int id, QString identifier, bool edited, bool moved, bool onHardware) {
    prepareLists(id);

    QListWidgetItem *item = ui->sequenceList->item(id);
    item->setText(QString("%1: ").arg(id+1, 3, 10, QChar(' ')) + identifier);

    setFont(item, edited, moved);
    item->setTextColor(onHardware ? colorOnHW : colorNotOnHW);

    cleanupLists();
}


void LibraryDialog::setNumberOfLibraryPrograms(int num) {
    numberOfPrograms = num;
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
    signal.int0 = Flag::PATCH | Flag::SEQUENCE;
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
    signal.int0 = Flag::PATCH | Flag::SEQUENCE;
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
    signal.int0 = Flag::PATCH | Flag::SEQUENCE | Flag::CHANGED;
    signal.int1 = 0;
    signal.int2 = -1;
    emit enqueue(signal);
}


void LibraryDialog::loadReplace() {
    QString path = QFileDialog::getOpenFileName(this, "Load Library", ".", "SysEx files (*.syx)");
    if (path != "") {
        QueueItem signal(QueueAction::LIBRARY_LOAD, path, Flag::PATCH | Flag::SEQUENCE);
        emit enqueue(signal);
    }
}


void LibraryDialog::loadAppend() {
    QString path = QFileDialog::getOpenFileName(this, "Load Library", ".", "SysEx files (*.syx)");
    if (path != "") {
        QueueItem signal(QueueAction::LIBRARY_LOAD, path, Flag::PATCH | Flag::SEQUENCE | Flag::APPEND);
        emit enqueue(signal);
    }
}


void LibraryDialog::save() {
    QString path = QFileDialog::getSaveFileName(this, "Save Library", ".", "SysEx files (*.syx)");
    if (path != "") {
        if (!path.endsWith(".syx", Qt::CaseInsensitive)) {
            path.append(".syx");
        }

        QueueItem signal(QueueAction::LIBRARY_SAVE, path, Flag::PATCH | Flag::SEQUENCE);
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

    int flag = Flag::PATCH;
    if (ui->sync->isChecked()) {
        flag |= Flag::SEQUENCE;
    }
    recall(flag, c);
}


void LibraryDialog::sequenceRecall(QListWidgetItem *item) {
    Q_UNUSED(item);
    const int &c = ui->sequenceList->currentRow();

    int flag = Flag::SEQUENCE;
    if (ui->sync->isChecked()) {
        flag |= Flag::PATCH;
    }
    recall(flag, c);

}


void LibraryDialog::patchCMStore() {
    const int &c = ui->patchList->currentRow();

    int flag = Flag::PATCH;
    if (ui->sync->isChecked()) {
        flag |= Flag::SEQUENCE;
    }
    store(flag, c);
}


void LibraryDialog::sequenceCMStore() {
    const int &c = ui->sequenceList->currentRow();

    int flag = Flag::SEQUENCE;
    if (ui->sync->isChecked()) {
        flag |= Flag::PATCH;
    }
    store(flag, c);
}


void LibraryDialog::patchCMSend() {
    int flag = Flag::PATCH;
    if (ui->sync->isChecked()) {
        flag |= Flag::SEQUENCE;
    }
    librarySelectedRanges(ui->patchList, QueueAction::LIBRARY_SEND, flag);
}


void LibraryDialog::patchCMSendChanged() {
    int flag = Flag::PATCH | Flag::CHANGED;
    if (ui->sync->isChecked()) {
        flag |= Flag::SEQUENCE;
    }
    librarySelectedRanges(ui->patchList, QueueAction::LIBRARY_SEND, flag);
}


void LibraryDialog::sequenceCMSend() {
    int flag = Flag::SEQUENCE;
    if (ui->sync->isChecked()) {
        flag |= Flag::PATCH;
    }
    librarySelectedRanges(ui->sequenceList, QueueAction::LIBRARY_SEND, flag);
}


void LibraryDialog::sequenceCMSendChanged() {
    int flag = Flag::SEQUENCE | Flag::CHANGED;
    if (ui->sync->isChecked()) {
        flag |= Flag::PATCH;
    }
    librarySelectedRanges(ui->sequenceList, QueueAction::LIBRARY_SEND, flag);
}


void LibraryDialog::patchCMFetch() {
    int flag = Flag::PATCH;
    if (ui->sync->isChecked()) {
        flag |= Flag::SEQUENCE;
    }
    librarySelectedRanges(ui->patchList, QueueAction::LIBRARY_FETCH, flag);
}


void LibraryDialog::sequenceCMFetch() {
    int flag = Flag::SEQUENCE;
    if (ui->sync->isChecked()) {
        flag |= Flag::PATCH;
    }
    librarySelectedRanges(ui->sequenceList, QueueAction::LIBRARY_FETCH, flag);
}


void LibraryDialog::patchCMRemove() {
#ifdef DEBUGMSGS
    qDebug() << "LibraryDialog::patchCMDelete()";
#endif
    librarySelectedRanges(ui->patchList, QueueAction::LIBRARY_REMOVE, 0);
}


void LibraryDialog::sequenceCMRemove() {
#ifdef DEBUGMSGS
    qDebug() << "LibraryDialog::patchCMDelete()";
#endif
    librarySelectedRanges(ui->sequenceList, QueueAction::LIBRARY_REMOVE, 0);
}


void LibraryDialog::patchCMReset() {
#ifdef DEBUGMSGS
    qDebug() << "LibraryDialog::patchCMReset()";
#endif
    int flag = Flag::PATCH;
    if (ui->sync->isChecked()) {
        flag |= Flag::SEQUENCE;
    }
    librarySelectedRanges(ui->patchList, QueueAction::LIBRARY_RESET, flag);
}


void LibraryDialog::sequenceCMReset() {
#ifdef DEBUGMSGS
    qDebug() << "LibraryDialog::sequenceCMReset()";
#endif
    int flag = Flag::SEQUENCE;
    if (ui->sync->isChecked()) {
        flag |= Flag::PATCH;
    }
    librarySelectedRanges(ui->sequenceList, QueueAction::LIBRARY_RESET, flag);
}


void LibraryDialog::patchCMInsertAfter() {
#ifdef DEBUGMSGS
    qDebug() << "LibraryDialog::patchCMInsertAfter()";
#endif
    QueueItem item(QueueAction::LIBRARY_INSERT, ui->patchList->currentRow() + 1);
    emit enqueue(item);
}


void LibraryDialog::sequenceCMInsertAfter() {
#ifdef DEBUGMSGS
    qDebug() << "LibraryDialog::sequenceCMInsertAfter()";
#endif
    QueueItem item(QueueAction::LIBRARY_INSERT, ui->sequenceList->currentRow() + 1);
    emit enqueue(item);
}


void LibraryDialog::patchMove(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row) {
    Q_UNUSED(parent);
    Q_UNUSED(destination);

    if (start != end) {
        qDebug() << "We have a serious problem";
        return;
    }

    copySelectionFromPatchToSequence();

    int flag = Flag::PATCH;
    if (ui->sync->isChecked()) {
        flag |= Flag::SEQUENCE;
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

    copySelectionFromSequenceToPatch();

    int flag = Flag::SEQUENCE;
    if (ui->sync->isChecked()) {
        flag |= Flag::PATCH;
    }
    move(flag, start, row);
}


void LibraryDialog::copyScrollBarPositionFromPatchToSequence(int val) {
    if (dontCopyScrollBarPosition || !ui->sync->isChecked()) {
        return;
    }
    dontCopyScrollBarPosition = true;
    ui->sequenceList->verticalScrollBar()->setValue(val);
    dontCopyScrollBarPosition = false;
}


void LibraryDialog::copyScrollBarPositionFromSequenceToPatch(int val) {
    if (dontCopyScrollBarPosition || !ui->sync->isChecked()) {
        return;
    }
    dontCopyScrollBarPosition = true;
    ui->patchList->verticalScrollBar()->setValue(val);
    dontCopyScrollBarPosition = false;
}


void LibraryDialog::copySelectionFromPatchToSequence() {
    if (dontCopySelection || !ui->sync->isChecked()) {
        return;
    }

    dontCopySelection = true;
    int rows = ui->patchList->count();
    for (int i = 0; i < rows; i++) {
        ui->sequenceList->item(i)->setSelected(ui->patchList->item(i)->isSelected());
    }
    dontCopySelection = false;
}


void LibraryDialog::copySelectionFromSequenceToPatch() {
    if (dontCopySelection || !ui->sync->isChecked()) {
        return;
    }

    dontCopySelection = true;
    int rows = ui->sequenceList->count();
    for (int i = 0; i < rows; i++) {
        ui->patchList->item(i)->setSelected(ui->sequenceList->item(i)->isSelected());
    }
    dontCopySelection = false;
}
