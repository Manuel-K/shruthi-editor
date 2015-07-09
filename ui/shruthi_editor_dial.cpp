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


#include "ui/shruthi_editor_dial.h"
#include "ui_shruthi_editor_dial.h"
#include <stddef.h> // for NULL


ShruthiEditorDial::ShruthiEditorDial(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShruthiEditorDial) {
    ui->setupUi(this);

    sendValueChanges = true;
    parameter = 0;
    formatter = NULL;
    connect(ui->dial, SIGNAL(valueChanged(int)), this, SLOT(valueChanged(int)));

    setValue(0);
    setDisplay(0);
}


ShruthiEditorDial::~ShruthiEditorDial() {
    delete ui;
}


void ShruthiEditorDial::setParameter(const int &parameter_) {
    parameter = parameter_;
}


void ShruthiEditorDial::setName(const QString &name) {
    ui->name->setText(name);
}


void ShruthiEditorDial::setLimits(const int &min, const int &max) {
    ui->dial->setMinimum(min);
    ui->dial->setMaximum(max);
}


void ShruthiEditorDial::setFormatter(QString (*formatter_)(int)) {
    formatter = formatter_;
    valueChanged(ui->dial->value());
}


void ShruthiEditorDial::setValue(const int &value) {
    sendValueChanges = false;
    ui->dial->setValue(value);
    sendValueChanges = true;
}


void ShruthiEditorDial::setDisplay(const int &value) {
    if (!formatter) {
        ui->display->setText(QString("%1").arg(value));
    } else {
        ui->display->setText((*formatter)(value));
    }
}


//
// Slots:
//


void ShruthiEditorDial::valueChanged(int value) {
    setDisplay(value);
    if (sendValueChanges) {
        emit valueChanged(parameter, value);
    }
}
