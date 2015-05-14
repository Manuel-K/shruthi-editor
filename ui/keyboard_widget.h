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


#ifndef KEYBOARD_WIDGET_H
#define KEYBOARD_WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <vector>


class KeyboardWidget : public QWidget {
        Q_OBJECT

    public:
        KeyboardWidget(QWidget *parent = 0, unsigned int numberOfKeys = 61);
        ~KeyboardWidget();

        void setNumberOfKeys(const unsigned int &value);
        unsigned int getNumberOfKeys() const;

        bool getAutomaticNumberOfKeys() const;
        void setAutomaticNumberOfKeys(bool value);

        double getKeySizeRatio() const;
        void setKeySizeRatio(double value);

        double getBlackKeyHeightFactor() const;
        void setBlackKeyHeightFactor(double value);

        int getMinimalKeyWidth() const;
        void setMinimalKeyWidth(int value);

        int getBaseNote() const;
        void setBaseNote(int value);

        int getOctave() const;
        void setOctave(int value);

    private:
        void resizeEvent(QResizeEvent * event);
        static bool isBlackKey(int i);
        static int numWhiteKeys(int i);
        void setKeySize();
        void raiseKeys();
        void setKeyVisibility();
        void setupKeys();
        void destroyKeys();

        std::vector <QPushButton*> keys;
        unsigned int visibleNumberOfKeys;

        unsigned int numberOfKeys;
        bool automaticNumberOfKeys;
        double keySizeRatio; // for 25 keys
        double blackKeyHeightFactor;
        int minimalKeyWidth;

        int baseNote;
        int octave;

    private slots:
        void keyPressed();
        void keyReleased();

    signals:
        void keyPressed(int);
        void keyReleased(int);
};

#endif // KEYBOARD_WIDGET_H
