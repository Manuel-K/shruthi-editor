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


    connect(ui->patchList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(recall(QListWidgetItem*)));
    connect(ui->patchList, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customContextMenuRequested(QPoint)));
    connect(ui->patchList->model(), SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)), this, SLOT(move(QModelIndex,int,int,QModelIndex,int)));
    connect(ui->fetch, SIGNAL(clicked(bool)), this, SLOT(fetch()));

    // synchronize list widget scrolling:
    connect(ui->patchList->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(syncToSequenceScrollBar(int)));
    connect(ui->sequenceList->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(syncToPatchScrollBar(int)));
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
            item->setText(QString("%1: ").arg(i+1)+editor->getLibraryName(i));

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
            item->setText(QString("%1: ").arg(i+1)+(editor->isLibrarySequenceInit(i) ? "init" : "custom"));

            const bool &e = editor->getLibrarySequenceEdited(i);
            const bool &m = editor->getLibrarySequenceMoved(i);
            setFont(item, e, m);
        }
    }
}


// ******************************************
void LibraryDialog::recall(QListWidgetItem *item) {
// ******************************************
    Q_UNUSED(item);
    const int &c = ui->patchList->currentRow();

//    std::cout << "recall " << c << std::endl;
    queueitem_t signal(LIBRARY_RECALL);
    signal.int0 = Editor::FLAG_PATCH | Editor::FLAG_SEQUENCE;
    signal.int1 = c;
    emit enqueue(signal);
}

/*
    ui->log->append("customContextMenuRequested");

//    // determine selection manually
//    const QList<QListWidgetItem*> &l = ui->patchList->selectedItems();
//    for (int i = 0; i < l.size(); i++) {
//        ui->log->append(l.at(i)->text());
//    }

    int r = ui->patchList->currentRow();

    // make bold/set background
//    ui->patchList->item(r)->setBackgroundColor(QColor("blue"));
    QFont f = ui->patchList->item(r)->font();
    f.setBold(true);
    f.setItalic(true);
    ui->patchList->item(r)->setFont(f);

//    ui->patchList->item(r)->setText();

    // determine selection manually:
    int rows = ui->patchList->count();
    for (int i = 0; i < rows; i++) {
        if(ui->patchList->item(i)->isSelected()) {
            ui->log->append(QString("%1 selected").arg(i));
        }
    }
//    ui->patchList->model()->qt_check_for_QOBJECT_macro
*/

// ******************************************
void LibraryDialog::customContextMenuRequested(QPoint p) {
// ******************************************
    Q_UNUSED(p);

    const int &c = ui->patchList->currentRow();

//    std::cout << "store " << c << std::endl;
    queueitem_t signal(LIBRARY_STORE);
    signal.int0 = Editor::FLAG_PATCH | Editor::FLAG_SEQUENCE;
    signal.int1 = c;
    emit enqueue(signal);

}


// ******************************************
void LibraryDialog::move(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row) {
// ******************************************
    Q_UNUSED(parent);
    Q_UNUSED(destination);

    if (start != end) {
        qDebug() << "We have a serious problem";
        return;
    }

    queueitem_t signal(LIBRARY_MOVE);
    signal.int0 = Editor::FLAG_PATCH | Editor::FLAG_SEQUENCE;
    signal.int1 = start;
    signal.int2 = row;

    emit enqueue(signal);
}


// ******************************************
void LibraryDialog::fetch() {
// ******************************************
//    ui->patchList->addItem("new");
//    ui->patchList->insertItem(ui->patchList->currentRow(), "new");
    queueitem_t signal(LIBRARY_FETCH);
    signal.int0 = Editor::FLAG_PATCH | Editor::FLAG_SEQUENCE;
    signal.int1 = 0;
    signal.int2 = 15;
    emit enqueue(signal);
}


// ******************************************
void LibraryDialog::syncToSequenceScrollBar(int val) {
// ******************************************
    if (dontScroll) {
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
    if (dontScroll) {
        return;
    }
    //std::cout << "sscroll " << val << std::endl;
    dontScroll = true;
    ui->patchList->verticalScrollBar()->setValue(val);
    dontScroll = false;
}


