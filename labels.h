// Shruthi-Editor: An unofficial Editor for the Shruthi hardware synthesizer. For
// informations about the Shruthi, see <http://www.mutable-instruments.net/shruthi1>.
//
// Copyright (C) 2011-2016 Manuel Krönig
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


class QString;
class QStringList;


class Labels {
    public:
        static const QStringList ModulationSource;
        static const QStringList ModulationDestination;
        static const QStringList OscillatorAlgorithm;
        static const QStringList SubOscillatorAlgorithm;
        static const QStringList LfoWave;
        static const QStringList LfoMode;
        static const QStringList Status;
        static const QStringList Operator;
        static const QStringList CvOperator;
        static QString LfoRateFormatter(int value); // used for labels
        static const QStringList OffOn;
        static QString OffOnFormatter(int value);
        // The following labels are used for the filter boards:
        static const QStringList FilterBoard;
        static const QStringList FilterSVFMode1;
        static const QStringList FilterSVFMode2;
        static const QStringList FilterDSPMode;
        static const QStringList FilterDSPFx;
        static const QStringList FilterPolivoksMode;
        static const QStringList Filter4PMMode;
        static const QStringList Filter4PMResonanceFlavor;
        static const QStringList FilterSPMode;
        // The following labels are used for the sequencer:
        static const QStringList SequencerMode;
        static const QStringList ArpeggiatorDirection;
        static const QStringList GrooveTemplate;
        static const QStringList SequencerClockDivision;
        static QString TempoFormatter(int value); // used for labels
        static QString ArpeggiatorPatternFormatter(int value); // used for labels
        static const QStringList Notes;
        static QString NoteFormatter(int value);
        static const QStringList HexValues;

    private:
        static const QStringList LFORate; // used for LfoRateFormatter
};


#endif // SHRUTHI_LABELS_H
