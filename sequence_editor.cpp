#include "sequence_editor.h"
#include "ui_sequence_editor.h"
#include <QDebug>
#include <iostream>

#include "lib_sequence.h"


// ******************************************
SequenceEditor::SequenceEditor(Editor *edit, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SequenceEditor) {
// ******************************************
    ui->setupUi(this);

    editor = edit;

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

    redrawAllSequenceParameters();
}


// ******************************************
SequenceEditor::~SequenceEditor() {
// ******************************************
    delete ui;

}


// ******************************************
void SequenceEditor::showSequenceEditor() {
// ******************************************
    show();
}


// ******************************************
void SequenceEditor::redrawPatchParameter(int) {
// ******************************************
    //TODO: implement me
}


// ******************************************
void SequenceEditor::redrawAllPatchParameters() {
// ******************************************
    //TODO: implement me
}


// ******************************************
void SequenceEditor::redrawAllSequenceParameters() {
// ******************************************
#ifdef DEBUGMSGS
    std::cout << "SequenceEditor::redrawAllSequenceParameters()" << std::endl;
#endif
    //TODO: cleanup
    SequenceStep *s;
    for (unsigned int i = 0; i < Sequence::NUMBER_OF_STEPS; i++) {
        s = this->findChild<SequenceStep*>(QString("s%1").arg(i));
        if (!s) {
            continue;
        }
        s->setActive(editor->getSequenceParam(i, SequenceParameter::ACTIVE));
        s->setNoteOctave(editor->getSequenceParam(i, SequenceParameter::NOTE));
        s->setTie(editor->getSequenceParam(i, SequenceParameter::TIE));
        s->setVelocity(editor->getSequenceParam(i, SequenceParameter::VELOCITY));
        s->setValue(editor->getSequenceParam(i, SequenceParameter::VALUE));
    }
}


// ******************************************
void SequenceEditor::activeChanged(int step, int value) {
// ******************************************
#ifdef DEBUGMSGS
    std::cout << "SequenceEditor active " << step << " " << value << std::endl;
#endif
    queueitem_t signal(SEQUENCE_PARAMETER_CHANGE_EDITOR, Sequence::calculateParamId(step, SequenceParameter::ACTIVE), value);
    emit(enqueue(signal));
}


// ******************************************
void SequenceEditor::noteChanged(int step, int value) {
// ******************************************
#ifdef DEBUGMSGS
    std::cout << "SequenceEditor note " << step << " " << value << std::endl;
#endif
    queueitem_t signal(SEQUENCE_PARAMETER_CHANGE_EDITOR, Sequence::calculateParamId(step, SequenceParameter::NOTE), value);
    emit(enqueue(signal));
}


// ******************************************
void SequenceEditor::valueChanged(int step, int value) {
// ******************************************
#ifdef DEBUGMSGS
    std::cout << "SequenceEditor value " << step << " " << value << std::endl;
#endif
    queueitem_t signal(SEQUENCE_PARAMETER_CHANGE_EDITOR, Sequence::calculateParamId(step, SequenceParameter::VALUE), value);
    emit(enqueue(signal));
}


// ******************************************
void SequenceEditor::tieChanged(int step, int value) {
// ******************************************
#ifdef DEBUGMSGS
    std::cout << "SequenceEditor tie " <<  step << " " << value << std::endl;
#endif
    queueitem_t signal(SEQUENCE_PARAMETER_CHANGE_EDITOR, Sequence::calculateParamId(step, SequenceParameter::TIE), value);
    emit(enqueue(signal));
}


// ******************************************
void SequenceEditor::velocityChanged(int step, int value) {
// ******************************************
#ifdef DEBUGMSGS
    std::cout << "SequenceEditor velocity " << step << " " << value << std::endl;
#endif
    queueitem_t signal(SEQUENCE_PARAMETER_CHANGE_EDITOR, Sequence::calculateParamId(step, SequenceParameter::VELOCITY), value);
    emit(enqueue(signal));
}
