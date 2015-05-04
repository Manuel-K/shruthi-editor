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


#include "library_dialog.h"
#include "ui_library_dialog.h"

#include <QDebug>
#include <QScrollBar>

// ******************************************
LibraryDialog::LibraryDialog(Editor *edit, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LibraryDialog) {
// ******************************************
    ui->setupUi(this);

    editor = edit;

    ui->patchList->addItem("If you can read this,");
    ui->patchList->addItem("something is wrong!");

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


    connect(ui->patchList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(patchRecall(QListWidgetItem*)));
    connect(ui->patchList, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(patchStore(QPoint)));
    connect(ui->patchList->model(), SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)), this, SLOT(patchMove(QModelIndex,int,int,QModelIndex,int)));
    connect(ui->sequenceList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(sequenceRecall(QListWidgetItem*)));
    connect(ui->sequenceList, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(sequenceStore(QPoint)));
    connect(ui->sequenceList->model(), SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)), this, SLOT(sequenceMove(QModelIndex,int,int,QModelIndex,int)));

    connect(ui->fetch, SIGNAL(clicked(bool)), this, SLOT(fetch()));
    connect(ui->sendSelected, SIGNAL(clicked()), this, SLOT(sendSelected()));

    // synchronize list widget scrolling:
    connect(ui->patchList->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(syncToSequenceScrollBar(int)));
    connect(ui->sequenceList->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(syncToPatchScrollBar(int)));
    // synchronize selection changes:
    connect(ui->patchList, SIGNAL(itemSelectionChanged()), this, SLOT(syncToSequenceSelection()));
    connect(ui->sequenceList, SIGNAL(itemSelectionChanged()), this, SLOT(syncToPatchSelection()));

}


// ******************************************
LibraryDialog::~LibraryDialog() {
// ******************************************
    delete ui;
}


// ******************************************
void LibraryDialog::setFont(QListWidgetItem *item, bool edited, bool moved) {
// ******************************************

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


// ******************************************
void LibraryDialog::redrawItems(int what, int start, int stop) {
// ******************************************
    if ((what&Editor::FLAG_PATCH) || (what&Editor::FLAG_SEQUENCE)) {
        // add missing items
        const int &amount = stop - ui->patchList->count() + 1;
        if (amount > 0) {
            //std::cout << "amount: " << amount << std::endl;
            for (int i = 0; i < amount; i++) {
                ui->patchList->addItem("temp");
            }
        }
        // add missing items
        const int &amount2 = stop - ui->sequenceList->count() + 1;
        if (amount2 > 0) {
            //std::cout << "amountS: " << amount2 << std::endl;
            for (int i = 0; i < amount2; i++) {
                ui->sequenceList->addItem("temp");
            }
        }
    }

    if (what&Editor::FLAG_PATCH) {
        QListWidgetItem *item;
        // setup items
        //std::cout << "redrawPatchNames " <<  start << " " << stop << std::endl;
        for (int i = start; i <= stop; i++) {
            item = ui->patchList->item(i);
            item->setText(QString("%1: ").arg(i+1, 3, 10, QChar(' '))+editor->getLibraryPatchName(i));

            const bool &e = editor->getLibraryPatchEdited(i);
            const bool &m = editor->getLibraryPatchMoved(i);
            setFont(item, e, m);
        }
    }
    if (what&Editor::FLAG_SEQUENCE) {
        QListWidgetItem *item;
        // setup items
        //std::cout << "redrawSequenceInfo " <<  start << " " << stop << std::endl;
        for (int i = start; i <= stop; i++) {
            item = ui->sequenceList->item(i);
            item->setText(QString("%1: ").arg(i+1, 3, 10, QChar(' '))+editor->getLibrarySequenceName(i));

            const bool &e = editor->getLibrarySequenceEdited(i);
            const bool &m = editor->getLibrarySequenceMoved(i);
            setFont(item, e, m);
        }
    }
}


// ******************************************
void LibraryDialog::recall(const int &flags, const int &id) {
// ******************************************
    queueitem_t signal(LIBRARY_RECALL);
    signal.int0 = flags;
    signal.int1 = id;
    emit enqueue(signal);
}


// ******************************************
void LibraryDialog::move(const int &flags, const int &from, const int &to) {
// ******************************************
    queueitem_t signal(LIBRARY_MOVE);
    signal.int0 = flags;
    signal.int1 = from;
    signal.int2 = to;

    emit enqueue(signal);
}


// ******************************************
void LibraryDialog::store(const int &flags, const int &id) {
// ******************************************
    queueitem_t signal(LIBRARY_STORE);
    signal.int0 = flags;
    signal.int1 = id;
    emit enqueue(signal);
}


// ******************************************
void LibraryDialog::fetch() {
// ******************************************
    queueitem_t signal(LIBRARY_FETCH);
    signal.int0 = Editor::FLAG_PATCH | Editor::FLAG_SEQUENCE;
    signal.int1 = 0;
    signal.int2 = 15;
    emit enqueue(signal);
}



// ******************************************
void LibraryDialog::sendRange(const int &flags, const int &from, const int &to) {
// ******************************************
    std::cout << "LibraryDialog::sendRange() " << flags << " " << from << " " << to << std::endl;
    queueitem_t signal(LIBRARY_SEND);
    signal.int0 = flags;
    signal.int1 = from;
    signal.int2 = to;
    emit enqueue(signal);
}





//
// UI Slots:
//


// ******************************************
void LibraryDialog::patchRecall(QListWidgetItem *item) {
// ******************************************
    Q_UNUSED(item);
    const int &c = ui->patchList->currentRow();

//    std::cout << "recall " << c << std::endl;
    int flag = Editor::FLAG_PATCH;
    if (ui->sync->isChecked()) {
        flag |= Editor::FLAG_SEQUENCE;
    }
    recall(flag, c);
}


// ******************************************
void LibraryDialog::sequenceRecall(QListWidgetItem *item) {
// ******************************************
    Q_UNUSED(item);
    const int &c = ui->sequenceList->currentRow();

//    std::cout << "recall " << c << std::endl;
    int flag = Editor::FLAG_SEQUENCE;
    if (ui->sync->isChecked()) {
        flag |= Editor::FLAG_PATCH;
    }
    recall(flag, c);

}


// ******************************************
void LibraryDialog::patchStore(QPoint p) {
// ******************************************
    Q_UNUSED(p);

    const int &c = ui->patchList->currentRow();

//    std::cout << "store " << c << std::endl;
    int flag = Editor::FLAG_PATCH;
    if (ui->sync->isChecked()) {
        flag |= Editor::FLAG_SEQUENCE;
    }
    store(flag, c);
}


// ******************************************
void LibraryDialog::sequenceStore(QPoint p) {
// ******************************************
    Q_UNUSED(p);

    const int &c = ui->sequenceList->currentRow();

//    std::cout << "store " << c << std::endl;
    int flag = Editor::FLAG_SEQUENCE;
    if (ui->sync->isChecked()) {
        flag |= Editor::FLAG_PATCH;
    }
    store(flag, c);
}


// ******************************************
void LibraryDialog::sendSelected() {
// ******************************************
    //std::cout << "LibraryDialog::sendSelected()" << std::endl;

    if (ui->patchList->currentRow() < 0) {
        // no selection
        return;
    }

    // determine selection manually:
    int rows = ui->patchList->count();
    if (rows < 16) {
        return; // should have at least the internal patches!
    }

    const int &flags = Editor::FLAG_PATCH | Editor::FLAG_SEQUENCE;

    bool last = ui->patchList->item(0)->isSelected();
    bool inside = false;
    int start = 0;
    for (int i = 1; i < rows; i++) {
        const bool &current = ui->patchList->item(i)->isSelected();
        if (current && !last) {
            inside = true;
            start = i;
        }

        if (!current && last) {
            inside = false;
            sendRange(flags, start, i - 1);
        }

        last = current;
    }
    if (inside) {
        sendRange(flags, start, rows - 1);
    }
}


// ******************************************
void LibraryDialog::patchMove(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row) {
// ******************************************
    Q_UNUSED(parent);
    Q_UNUSED(destination);

    if (start != end) {
        qDebug() << "We have a serious problem";
        return;
    }

    //std::cout << "patchMove" << std::endl;

    syncToSequenceSelection();

    int flag = Editor::FLAG_PATCH;
    if (ui->sync->isChecked()) {
        flag |= Editor::FLAG_SEQUENCE;
    }
    move(flag, start, row);
}


// ******************************************
void LibraryDialog::sequenceMove(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row) {
// ******************************************
    Q_UNUSED(parent);
    Q_UNUSED(destination);

    if (start != end) {
        qDebug() << "We have a serious problem";
        return;
    }

    //std::cout << "seqMove" << std::endl;

    syncToPatchSelection();

    int flag = Editor::FLAG_SEQUENCE;
    if (ui->sync->isChecked()) {
        flag |= Editor::FLAG_PATCH;
    }
    move(flag, start, row);
}


// ******************************************
void LibraryDialog::syncToSequenceScrollBar(int val) {
// ******************************************
    if (dontScroll || !ui->sync->isChecked()) {
        return;
    }
    //std::cout << "pscroll " << val << std::endl;
    dontScroll = true;
    ui->sequenceList->verticalScrollBar()->setValue(val);
    dontScroll = false;
}


// ******************************************
void LibraryDialog::syncToPatchScrollBar(int val) {
// ******************************************
    if (dontScroll || !ui->sync->isChecked()) {
        return;
    }
    //std::cout << "sscroll " << val << std::endl;
    dontScroll = true;
    ui->patchList->verticalScrollBar()->setValue(val);
    dontScroll = false;
}


// ******************************************
void LibraryDialog::syncToSequenceSelection() {
// ******************************************
    if (dontSyncSelection || !ui->sync->isChecked()) {
        return;
    }

    //std::cout << "LibraryDialog::syncToSequenceSelection();" << std::endl;

    dontSyncSelection = true;
    int rows = ui->patchList->count();
    for (int i = 0; i < rows; i++) {
        ui->sequenceList->item(i)->setSelected(ui->patchList->item(i)->isSelected());
    }
    dontSyncSelection = false;
}


// ******************************************
void LibraryDialog::syncToPatchSelection() {
// ******************************************
    if (dontSyncSelection || !ui->sync->isChecked()) {
        return;
    }

    //std::cout << "LibraryDialog::syncToPatchSelection();" << std::endl;

    dontSyncSelection = true;
    int rows = ui->sequenceList->count();
    for (int i = 0; i < rows; i++) {
        ui->patchList->item(i)->setSelected(ui->sequenceList->item(i)->isSelected());
    }
    dontSyncSelection = false;
}


