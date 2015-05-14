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


#include "editor.h"
#include "ui/sequence_editor.h"
#include "ui_sequence_editor.h"
#include <QDebug>
#include <iostream>

#include "sequence.h"
#include "patch.h"


// ******************************************
SequenceEditor::SequenceEditor(const Editor *edit, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SequenceEditor),
    editor(edit) {
// ******************************************
    ui->setupUi(this);

    SequenceStep *step;
    for (unsigned int s = 0; s < Sequence::NUMBER_OF_STEPS; s++) {
        step = this->findChild<SequenceStep*>(QString("s%1").arg(s));
        if (!step) {
            qDebug() << "Error. Could not find SequenceStep" << QString("s%1").arg(s);
            continue;
        }
        step->setStep(s);
        connect(step, SIGNAL(activeChanged(int,int)), this, SLOT(activeChanged(int,int)));
        connect(step, SIGNAL(noteChanged(int,int)), this, SLOT(noteChanged(int,int)));
        connect(step, SIGNAL(valueChanged(int,int)), this, SLOT(valueChanged(int,int)));
        connect(step, SIGNAL(tieChanged(int,int)), this, SLOT(tieChanged(int,int)));
        connect(step, SIGNAL(velocityChanged(int,int)), this, SLOT(velocityChanged(int,int)));
    }

    ShruthiEditorDial *dial;
    QComboBox *cb;
    QLabel *l;
    for (unsigned int p = 100; p < 110; p++) {
        if (Patch::parameter(p, 0).string_values) {
            cb = this->findChild<QComboBox*>(QString("c%1").arg(p));
            if (!cb) {
                continue;
            }
            cb->addItems(*Patch::parameter(p, 0).string_values);
            connect(cb, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxChanged(int)));
            l = this->findChild<QLabel*>(QString("l%1").arg(p));
            if (!l) {
                continue;
            }
            l->setText(Patch::parameter(p, 0).short_name + ":");
        } else {
            dial = this->findChild<ShruthiEditorDial*>(QString("c%1").arg(p));
            if (!dial) {
                continue;
            }
            dial->setParameter(p);
            dial->setFormatter(Patch::parameter(p, 0).formatter);
            dial->setName(Patch::parameter(p, 0).short_name + ":");
            dial->setLimits(Patch::parameter(p, 0).min, Patch::parameter(p, 0).max);
            connect(dial, SIGNAL(valueChanged(int,int)), this, SLOT(dialChanged(int,int)));
        }
    }
    redrawAllSequenceParameters();
}


// ******************************************
SequenceEditor::~SequenceEditor() {
// ******************************************
    delete ui;

}


// ******************************************
void SequenceEditor::sendSequenceUpdate() {
// ******************************************
    if (ui->autoSendSequence->isChecked()) {
        QueueItem signal(QueueAction::SYSEX_SEND_DATA, Editor::FLAG_SEQUENCE);
        emit enqueue(signal);
    }
}


// ******************************************
void SequenceEditor::redrawPatchParameter(int id) {
// ******************************************
#ifdef DEBUGMSGS
    std::cout << "SequenceEditor::redrawPatchParameter();" << id << std::endl;
#endif
    if (Patch::parameter(id, 0).string_values) {
        QComboBox *cb = this->findChild<QComboBox*>(QString("c%1").arg(id));
        if (!cb) {
            return;
        }
        const bool &temp = cb->isEnabled();
        cb->setEnabled(false);
        cb->setCurrentIndex(editor->getPatchValue(id));
        cb->setEnabled(temp);
    } else {
        ShruthiEditorDial *dial = this->findChild<ShruthiEditorDial*>(QString("c%1").arg(id));
        if (!dial) {
            return;
        }
        dial->setValue(editor->getPatchValue(id));
    }
}


// ******************************************
void SequenceEditor::redrawAllPatchParameters() {
// ******************************************
#ifdef DEBUGMSGS
    std::cout << "SequenceEditor::redrawAllPatchParameters();" << std::endl;
#endif
    for (unsigned int p = 100; p < 110; p++) {
        redrawPatchParameter(p);
    }
}


// ******************************************
void SequenceEditor::redrawAllSequenceParameters() {
// ******************************************
#ifdef DEBUGMSGS
    std::cout << "SequenceEditor::redrawAllSequenceParameters()" << std::endl;
#endif
    SequenceStep *s;
    for (unsigned int i = 0; i < Sequence::NUMBER_OF_STEPS; i++) {
        s = this->findChild<SequenceStep*>(QString("s%1").arg(i));
        if (!s) {
            continue;
        }
        s->setActive(editor->getSequenceValue(i, SequenceParameter::ACTIVE));
        s->setNoteOctave(editor->getSequenceValue(i, SequenceParameter::NOTE));
        s->setTie(editor->getSequenceValue(i, SequenceParameter::TIE));
        s->setVelocity(editor->getSequenceValue(i, SequenceParameter::VELOCITY));
        s->setValue(editor->getSequenceValue(i, SequenceParameter::VALUE));
    }
}


// ******************************************
void SequenceEditor::activeChanged(int step, int value) {
// ******************************************
#ifdef DEBUGMSGS
    std::cout << "SequenceEditor active " << step << " " << value << std::endl;
#endif
    QueueItem signal(QueueAction::SEQUENCE_PARAMETER_CHANGE_EDITOR, Sequence::calculateParamId(step, SequenceParameter::ACTIVE), value);
    emit enqueue(signal);
    sendSequenceUpdate();
}


// ******************************************
void SequenceEditor::noteChanged(int step, int value) {
// ******************************************
#ifdef DEBUGMSGS
    std::cout << "SequenceEditor note " << step << " " << value << std::endl;
#endif
    QueueItem signal(QueueAction::SEQUENCE_PARAMETER_CHANGE_EDITOR, Sequence::calculateParamId(step, SequenceParameter::NOTE), value);
    emit enqueue(signal);
    sendSequenceUpdate();
}


// ******************************************
void SequenceEditor::valueChanged(int step, int value) {
// ******************************************
#ifdef DEBUGMSGS
    std::cout << "SequenceEditor value " << step << " " << value << std::endl;
#endif
    QueueItem signal(QueueAction::SEQUENCE_PARAMETER_CHANGE_EDITOR, Sequence::calculateParamId(step, SequenceParameter::VALUE), value);
    emit enqueue(signal);
    sendSequenceUpdate();
}


// ******************************************
void SequenceEditor::tieChanged(int step, int value) {
// ******************************************
#ifdef DEBUGMSGS
    std::cout << "SequenceEditor tie " <<  step << " " << value << std::endl;
#endif
    QueueItem signal(QueueAction::SEQUENCE_PARAMETER_CHANGE_EDITOR, Sequence::calculateParamId(step, SequenceParameter::TIE), value);
    emit enqueue(signal);
    sendSequenceUpdate();
}


// ******************************************
void SequenceEditor::velocityChanged(int step, int value) {
// ******************************************
#ifdef DEBUGMSGS
    std::cout << "SequenceEditor velocity " << step << " " << value << std::endl;
#endif
    QueueItem signal(QueueAction::SEQUENCE_PARAMETER_CHANGE_EDITOR, Sequence::calculateParamId(step, SequenceParameter::VELOCITY), value);
    emit enqueue(signal);
    sendSequenceUpdate();
}


// ******************************************
void SequenceEditor::comboBoxChanged(int value) {
// ******************************************
    // Never send 'no current item set' i.e. -1:
    if (value == -1)
        return;

    QComboBox* s = (QComboBox*) sender();

    // Don't send changed signal if element is disabled:
    if (!s->isEnabled())
        return;

    QString id = s->objectName();
    id.remove(0,1);


#ifdef DEBUGMSGS
    std::cout << "SequenceEditor comboBoxChanged " << id.toInt() << " " << value << std::endl;
#endif
    QueueItem signal(QueueAction::PATCH_PARAMETER_CHANGE_EDITOR, id.toInt(), value);
    emit enqueue(signal);
}


// ******************************************
void SequenceEditor::dialChanged(int id, int value) {
// ******************************************
#ifdef DEBUGMSGS
    std::cout << "SequenceEditor dialChanged " << id << " " << value << std::endl;
#endif
    emit enqueue(QueueItem(QueueAction::PATCH_PARAMETER_CHANGE_EDITOR, id, value));
}


