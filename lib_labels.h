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

#ifndef SHRUTHI_LABELS_H
#define SHRUTHI_LABELS_H
#include <QStringList>

class Labels {
    public:
        static QStringList ModulationSource;
        static QStringList ModulationDestination;
        static QStringList OscillatorAlgorithm;
        static QStringList SubOscillatorAlgorithm;
        static QStringList LfoWave;
        static QStringList LfoMode;
        static QStringList Status;
        static QStringList Operator;
        static QStringList CvOperator;
        static QString LfoRateFormatter(int value); // used for labels
        static QStringList OffOn;
        static QString OffOnFormatter(int value);
        // The following labels are used for the filter boards:
        static QStringList FilterBoard;
        static QStringList FilterSVFMode1;
        static QStringList FilterSVFMode2;
        static QStringList FilterDSPMode;
        static QStringList FilterDSPFx;
        static QStringList FilterPolivoksMode;
        static QStringList Filter4PMMode;
        static QStringList Filter4PMResonanceFlavor;
        static QStringList FilterSPMode;
        // The following labels are used for the sequencer:
        static QStringList SequencerMode;
        static QStringList ArpeggiatorDirection;
        static QStringList GrooveTemplate;
        static QStringList SequencerClockDivision;
        static QString TempoFormatter(int value); // used for labels
        static QString ArpeggiatorPatternFormatter(int value); // used for labels
        static QStringList Notes;
        static QString NoteFormatter(int value);
        static QStringList HexValues;

    private:
        static QStringList LFORate; // used for LfoRateFormatter
};
#endif
