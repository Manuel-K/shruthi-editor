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


#ifndef SEQUENCESTEP_H
#define SEQUENCESTEP_H


#include <QWidget>
namespace Ui { class SequenceStep; }


class SequenceStep : public QWidget {
        Q_OBJECT

    public:
        explicit SequenceStep(QWidget *parent = 0);
        ~SequenceStep();

        void setNoteLabels(const QStringList &labels);
        void setOctaveLimits(const int &min, const int &max);
        void setStep(const int &value);
        void setValueLabels(const QStringList &labels);
        void setVelocityLimits(const int &min, const int &max);

        void setActive(const int &value);
        void setNote(const int &value);
        void setNoteOctave(const int &value);
        void setOctave(const int &value);
        void setTie(const int &value);
        void setValue(const int &value);
        void setVelocity(const int &value);


    private:
        SequenceStep(const SequenceStep&); //forbid copying
        SequenceStep &operator=(const SequenceStep&); //forbid assignment

        Ui::SequenceStep *ui;
        int step;
        bool sendActive, sendNote, sendTie, sendVelocity, sendValue;

        static const int noteBase = 12;


    private slots:
        void activeChanged(int val);
        void noteChanged(int val);
        void octaveChanged(int val);
        void valueChanged(int val);
        void tieChanged(int val);
        void velocityChanged(int val);


    signals:
        void activeChanged(int, int);
        void noteChanged(int, int);
        void valueChanged(int, int);
        void tieChanged(int, int);
        void velocityChanged(int, int);
};

#endif // SEQUENCESTEP_H
