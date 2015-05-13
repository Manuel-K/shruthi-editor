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


#include "ui/keyboard_widget.h"


// ******************************************
KeyboardWidget::KeyboardWidget(QWidget *parent, unsigned int numberOfKeys_) : QWidget(parent) {
// ******************************************
    numberOfKeys = numberOfKeys_;
    automaticNumberOfKeys = true;
    keySizeRatio = 0.35; // for 25 keys
    blackKeyHeightFactor = 0.6;
    minimalKeyWidth = 15;
    octave = 0;
    setupKeys();
}


// ******************************************
KeyboardWidget::~KeyboardWidget() {
// ******************************************
    destroyKeys();
}


// ******************************************
void KeyboardWidget::resizeEvent(QResizeEvent *event) {
// ******************************************
    Q_UNUSED(event);
    setKeySize();
}


// ******************************************
bool KeyboardWidget::isBlackKey(int i) {
// ******************************************
    i = i % 12;
    return (i == 2 || i == 4 || i == 7 || i == 9 || i == 11);
}


// ******************************************
int KeyboardWidget::numWhiteKeys(int i) {
// ******************************************
    const int octaves = i / 12;
    const int base = octaves * 7;
    switch(i % 12) {
    case 1:
        return 1 + base;
    case 2:
        return 1 + base;
    case 3:
        return 2 + base;
    case 4:
        return 2 + base;
    case 5:
        return 3 + base;
    case 6:
        return 4 + base;
    case 7:
        return 4 + base;
    case 8:
        return 5 + base;
    case 9:
        return 5 + base;
    case 10:
        return 6 + base;
    case 11:
        return 6 + base;
    default: // 0
        return 0 + base;
    }
}


// ******************************************
void KeyboardWidget::setKeySize() {
// ******************************************
    unsigned int newVisibleNumberOfKeys = numberOfKeys;
    int w = this->width();
    int h = this->height();


    // determine number of visible keys:
    int octaves = newVisibleNumberOfKeys / 12;
    int nwhite = numWhiteKeys(newVisibleNumberOfKeys);
    double key_width = w / (double)nwhite;
    while (automaticNumberOfKeys && key_width < minimalKeyWidth && octaves > 1) {
        newVisibleNumberOfKeys -= 12;
        octaves = newVisibleNumberOfKeys / 12;
        nwhite = numWhiteKeys(newVisibleNumberOfKeys);
        key_width = w / (double)nwhite;
    }


    // correct aspect ratio/center vertically:
    double dy = 0;
    double key_height = h;

    double ratio = keySizeRatio * 25 / (double)newVisibleNumberOfKeys;
    if (key_height / (double) w > ratio) {
        dy = (key_height - w * ratio) / 2.0;
        key_height -= 2 * dy;
    }

    double black_key_height = key_height * blackKeyHeightFactor;

    int xlast = 0;
    int xlast_bk = 0;
    unsigned int ilast_bk = 0;

    for (unsigned int i = 1; i <= newVisibleNumberOfKeys; i++) {
        int xpos = numWhiteKeys(i);
        if (isBlackKey(i)) {
            int xi = (xpos - 0.5) * key_width;
            int wi = key_width;

            // close the gaps between buttons
            if (ilast_bk > 0 && ilast_bk + 2 == i) {
                int dxi = xlast_bk - xi;
                xi += dxi;
                wi -= dxi;
            }

            keys.at(i - 1)->setGeometry(QRect(xi, dy, wi, black_key_height));
            xlast_bk = xi + wi;
            ilast_bk = i;
        } else {
            int xi = (xpos - 1) * key_width;
            int wi = key_width;

            // close the gaps between buttons
            int dxi = xlast - xi;
            xi += dxi;
            wi -= dxi;

            keys.at(i - 1)->setGeometry(QRect(xi, dy, wi, key_height));
            xlast = xi + wi;
        }
    }
    if (visibleNumberOfKeys != newVisibleNumberOfKeys) {
        visibleNumberOfKeys = newVisibleNumberOfKeys;
        setKeyVisibility();
        raiseKeys();
    }
}


// ******************************************
void KeyboardWidget::raiseKeys() {
// ******************************************
    for (unsigned int i = 1; i <= visibleNumberOfKeys; i++) {
        if (isBlackKey(i)) {
            keys.at(i - 1)->raise();
        }
    }
}


// ******************************************
void KeyboardWidget::setKeyVisibility() {
// ******************************************
    for (unsigned int i = 1; i <= keys.size(); i++) {
        keys.at(i - 1)->setHidden(i > visibleNumberOfKeys);
    }
}


// ******************************************
void KeyboardWidget::setupKeys() {
// ******************************************
    visibleNumberOfKeys = 0;

    QPushButton *b;
    for (unsigned int i = 1; i <= numberOfKeys; i++) {
        b = new QPushButton(this);
        b->setObjectName(QString("%1").arg(i - 1));
        connect(b, SIGNAL(pressed()), this, SLOT(keyPressed()));
        connect(b, SIGNAL(released()), this, SLOT(keyReleased()));

        if (isBlackKey(i)) {
            b->setStyleSheet("QPushButton { background-color: #404040 }");
        } else {
            b->setStyleSheet("QPushButton { background-color: white }");
        }
        keys.push_back(b);
    }
    setKeySize();
    raiseKeys();
}


// ******************************************
void KeyboardWidget::destroyKeys()
// ******************************************
{
    QPushButton *temp;
    unsigned int len = keys.size();
    for (unsigned int i = 0; i < len; i++) {
        temp = keys.back();
        keys.pop_back();
        delete temp;
    }
}


// ******************************************
void KeyboardWidget::setNumberOfKeys(const unsigned int &value) {
// ******************************************
    numberOfKeys = value;

    // It would be nice to reuse buttons, but it is simpler to delete them all.
    destroyKeys();

    setupKeys();
}


// ******************************************
unsigned int KeyboardWidget::getNumberOfKeys() const {
// ******************************************
    return numberOfKeys;
}


// ******************************************
int KeyboardWidget::getBaseNote() const {
// ******************************************
    return baseNote;
}


// ******************************************
void KeyboardWidget::setBaseNote(int value) {
// ******************************************
    baseNote = value;
}


// ******************************************
int KeyboardWidget::getMinimalKeyWidth() const {
// ******************************************
    return minimalKeyWidth;
}


// ******************************************
void KeyboardWidget::setMinimalKeyWidth(int value) {
// ******************************************
    minimalKeyWidth = value;
}


// ******************************************
double KeyboardWidget::getBlackKeyHeightFactor() const {
// ******************************************
    return blackKeyHeightFactor;
}


// ******************************************
void KeyboardWidget::setBlackKeyHeightFactor(double value) {
// ******************************************
    blackKeyHeightFactor = value;
}


// ******************************************
double KeyboardWidget::getKeySizeRatio() const {
// ******************************************
    return keySizeRatio;
}


// ******************************************
void KeyboardWidget::setKeySizeRatio(double value) {
// ******************************************
    keySizeRatio = value;
}


// ******************************************
bool KeyboardWidget::getAutomaticNumberOfKeys() const {
// ******************************************
    return automaticNumberOfKeys;
}


// ******************************************
void KeyboardWidget::setAutomaticNumberOfKeys(bool value) {
// ******************************************
    automaticNumberOfKeys = value;
}


// ******************************************
int KeyboardWidget::getOctave() const {
// ******************************************
    return octave;
}


// ******************************************
void KeyboardWidget::setOctave(int value) {
// ******************************************
    octave = value;
}


//
// Slots:
//


// ******************************************
void KeyboardWidget::keyPressed() {
// ******************************************
    QPushButton* s = (QPushButton*) sender();
    QString id = s->objectName();
    emit keyPressed(baseNote + 12 * octave + id.toInt());
}


// ******************************************
void KeyboardWidget::keyReleased() {
// ******************************************
    QPushButton* s = (QPushButton*) sender();
    QString id = s->objectName();
    emit keyReleased(baseNote + 12 * octave + id.toInt());
}
