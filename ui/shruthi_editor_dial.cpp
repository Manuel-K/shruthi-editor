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


#include "ui/shruthi_editor_dial.h"
#include <stddef.h> // for NULL


#include <QDial>
#include <QLabel>

ShruthiEditorDial::ShruthiEditorDial(QWidget *parent) :
    QWidget(parent) {
    header = NULL;
    has_header = false;

    display = new QLabel(this);
    display->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    dial = new QDial(this);

    display_width = 27;

    resize();

    parameter = 0;
    formatter = NULL;
    connect(dial, SIGNAL(valueChanged(int)), this, SLOT(valueChanged(int)));
    setValue(0);
}


ShruthiEditorDial::~ShruthiEditorDial() {
    if (has_header) {
        delete header;
    }
    delete display;
    delete dial;
}


void ShruthiEditorDial::setParameter(const int &parameter_) {
    parameter = parameter_;
}


void ShruthiEditorDial::setName(const QString &short_name) {
    if (!has_header) {
        has_header = true;
        header = new QLabel(this);
        header->setAlignment(Qt::AlignCenter|Qt::AlignVCenter);
    }
    header->setText(short_name);
}


void ShruthiEditorDial::setLimits(const int &min, const int &max) {
    dial->setMinimum(min);
    dial->setMaximum(max);
    calculateDisplayWidth();
    resize();
}


void ShruthiEditorDial::setFormatter(QString (*formatter_)(int)) {
    formatter = formatter_;
    setDisplay(dial->value());
    calculateDisplayWidth();
    resize();
}


void ShruthiEditorDial::setValue(const int &value) {
    bool prev = dial->blockSignals(true);
    dial->setValue(value);
    setDisplay(value);
    dial->blockSignals(prev);
}


void ShruthiEditorDial::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event)
    resize();
}


void ShruthiEditorDial::resize() {
    const int &h0 = this->height();
    const int &w0 = this->width();
    int h1 = 0;
    int h2 = h0;

    if (has_header) {
        h1 += 21;
        h2 -= h1;
    }

    const int dial_available_width = std::max(4, this->width() - display_width);
    const int dial_size = std::max(6, std::min(h2, dial_available_width));
    const int x0 = (w0 - display_width - dial_size) / 2;

    const int dh = (h2 - dial_size) / 2;
    if (has_header) {
        header->setGeometry(0, dh, w0, h1);
    }
    display->setGeometry(x0, h1, display_width, h2);
    dial->setGeometry(x0 + display_width, h1 + dh, dial_size, dial_size);
}


void ShruthiEditorDial::setDisplay(const int &value) {
    if (!formatter) {
        display->setText(QString("%1").arg(value));
    } else {
        display->setText((*formatter)(value));
    }
}


void ShruthiEditorDial::calculateDisplayWidth() {
    if (!formatter) {
        int display_width_minval = display->fontMetrics().boundingRect(QString("%1").arg(dial->minimum())).width();
        int display_width_maxval = display->fontMetrics().boundingRect(QString("%1").arg(dial->maximum())).width();
        display_width = 2 + std::max(display_width_minval, display_width_maxval);
    } else {
        int max = 2;
        for (int val = dial->minimum(); val < dial->maximum(); val++) {
            max = std::max(max, display->fontMetrics().boundingRect(formatter(val)).width());
        }
        display_width = max;
    }
    resize();
}


//
// Slots:
//


void ShruthiEditorDial::valueChanged(int value) {
    setDisplay(value);
    valueChanged(parameter, value);
}
