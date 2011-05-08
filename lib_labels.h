// Shruthi-Editor: An unofficial Editor for the Shruthi hardware synthesizer. For 
// informations about the Shruthi, see <http://www.mutable-instruments.net/shruthi1>. 
//
// Copyright (C) 2011 Manuel Krönig
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

#ifndef SHRUTHI_LABELS_H
#define SHRUTHI_LABELS_H
#include <QStringList>

class Labels {
    private:
        static QStringList LFORate; // used for LfoRateFormatter
    public:
        static QStringList ModulationSource;
        static QStringList ModulationDestination;
        static QStringList PatchParameter;
        static QStringList OscillatorAlgorithm;
        static QStringList SubOscillatorAlgorithm;
        static QStringList LfoWave;
        static QStringList LfoMode;
        static QStringList Status;
        static QStringList Operator;
        static QStringList CvOperator;
        static QStringList MainFilterMode;
        static QStringList SecondaryFilterMode;
        static QString LfoRateFormatter(int value); // used for labels
};
#endif