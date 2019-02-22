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


#include "ui/sequence_editor.h"
#include "ui_sequence_editor.h"
#include <QDebug>
#include "flag.h"
#include "patch.h"
#include "queueitem.h"
#include "sequence.h"
#include "sequence_parameter.h"


SequenceEditor::SequenceEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SequenceEditor) {
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

        const QString &name = QCoreApplication::translate("Patch", qPrintable(Patch::parameter(p, 0).short_name)) \
                              + QCoreApplication::translate("ShruthiEditorMainWindow", ":");
        if (Patch::parameter(p, 0).string_values) {
            cb = this->findChild<QComboBox*>(QString("c%1").arg(p));
            if (!cb) {
                continue;
            }
            const QStringList &values = *Patch::parameter(p, 0).string_values;
            for (int i = 0; i < values.size(); i++) {
                cb->addItem(QCoreApplication::translate("Labels", qPrintable(values.at(i))));
            }
            connect(cb, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxChanged(int)));
            l = this->findChild<QLabel*>(QString("l%1").arg(p));
            if (!l) {
                continue;
            }
            l->setText(name);
        } else {
            dial = this->findChild<ShruthiEditorDial*>(QString("c%1").arg(p));
            if (!dial) {
                continue;
            }
            dial->setParameter(p);
            dial->setFormatter(Patch::parameter(p, 0).formatter);
            dial->setName(name);
            dial->setLimits(Patch::parameter(p, 0).min, Patch::parameter(p, 0).max);
            connect(dial, SIGNAL(valueChanged(int,int)), this, SLOT(dialChanged(int,int)));
        }
    }
}


SequenceEditor::~SequenceEditor() {
    delete ui;
}


void SequenceEditor::sendSequenceUpdate() {
    if (ui->autoSendSequence->isChecked()) {
        QueueItem signal(QueueAction::SYSEX_SEND_DATA, Flag::SEQUENCE);
        emit enqueue(signal);
    }
}


void SequenceEditor::redrawPatchParameter(int id, int value) {
#ifdef DEBUGMSGS
    std::cout << "SequenceEditor::redrawPatchParameter();" << id << " " << value << std::endl;
#endif
    if (!Patch::hasUI2(id)) {
        return;
    }

    if (Patch::parameter(id, 0).string_values) {
        QComboBox *cb = this->findChild<QComboBox*>(QString("c%1").arg(id));
        if (!cb) {
            return;
        }
        const bool &temp = cb->isEnabled();
        cb->setEnabled(false);
        cb->setCurrentIndex(value);
        cb->setEnabled(temp);
    } else {
        ShruthiEditorDial *dial = this->findChild<ShruthiEditorDial*>(QString("c%1").arg(id));
        if (!dial) {
            return;
        }
        dial->setValue(value);
    }
}


void SequenceEditor::redrawSequenceStep(int id, int active, int note, int tie, int velocity, int value) {
#ifdef DEBUGMSGS
    std::cout << "SequenceEditor::redrawSequenceStep()" << id << std::endl;
#endif
    if (id < 0 || id >= Sequence::NUMBER_OF_STEPS) {
        return;
    }

    SequenceStep *s = this->findChild<SequenceStep*>(QString("s%1").arg(id));
    if (!s) {
        return;
    }
    s->setActive(active);
    s->setNoteOctave(note);
    s->setTie(tie);
    s->setVelocity(velocity);
    s->setValue(value);
}


void SequenceEditor::activeChanged(int step, int value) {
#ifdef DEBUGMSGS
    std::cout << "SequenceEditor active " << step << " " << value << std::endl;
#endif
    QueueItem signal(QueueAction::SEQUENCE_PARAMETER_CHANGE_EDITOR, Sequence::calculateParamId(step, SequenceParameter::ACTIVE), value);
    emit enqueue(signal);
    sendSequenceUpdate();
}


void SequenceEditor::noteChanged(int step, int value) {
#ifdef DEBUGMSGS
    std::cout << "SequenceEditor note " << step << " " << value << std::endl;
#endif
    QueueItem signal(QueueAction::SEQUENCE_PARAMETER_CHANGE_EDITOR, Sequence::calculateParamId(step, SequenceParameter::NOTE), value);
    emit enqueue(signal);
    sendSequenceUpdate();
}


void SequenceEditor::valueChanged(int step, int value) {
#ifdef DEBUGMSGS
    std::cout << "SequenceEditor value " << step << " " << value << std::endl;
#endif
    QueueItem signal(QueueAction::SEQUENCE_PARAMETER_CHANGE_EDITOR, Sequence::calculateParamId(step, SequenceParameter::VALUE), value);
    emit enqueue(signal);
    sendSequenceUpdate();
}


void SequenceEditor::tieChanged(int step, int value) {
#ifdef DEBUGMSGS
    std::cout << "SequenceEditor tie " <<  step << " " << value << std::endl;
#endif
    QueueItem signal(QueueAction::SEQUENCE_PARAMETER_CHANGE_EDITOR, Sequence::calculateParamId(step, SequenceParameter::TIE), value);
    emit enqueue(signal);
    sendSequenceUpdate();
}


void SequenceEditor::velocityChanged(int step, int value) {
#ifdef DEBUGMSGS
    std::cout << "SequenceEditor velocity " << step << " " << value << std::endl;
#endif
    QueueItem signal(QueueAction::SEQUENCE_PARAMETER_CHANGE_EDITOR, Sequence::calculateParamId(step, SequenceParameter::VELOCITY), value);
    emit enqueue(signal);
    sendSequenceUpdate();
}


void SequenceEditor::comboBoxChanged(int value) {
    // Never send 'no current item set' i.e. -1:
    if (value == -1) {
        return;
    }

    QComboBox* s = (QComboBox*) sender();

    // Don't send changed signal if element is disabled:
    if (!s->isEnabled()) {
        return;
    }

    QString id = s->objectName();
    id.remove(0,1);

#ifdef DEBUGMSGS
    std::cout << "SequenceEditor comboBoxChanged " << id.toInt() << " " << value << std::endl;
#endif
    QueueItem signal(QueueAction::PATCH_PARAMETER_CHANGE_EDITOR, id.toInt(), value);
    emit enqueue(signal);
}


void SequenceEditor::dialChanged(int id, int value) {
#ifdef DEBUGMSGS
    std::cout << "SequenceEditor dialChanged " << id << " " << value << std::endl;
#endif
    emit enqueue(QueueItem(QueueAction::PATCH_PARAMETER_CHANGE_EDITOR, id, value));
}
