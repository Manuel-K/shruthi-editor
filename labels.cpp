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


#include "labels.h"
#include <QStringList>


const QStringList Labels::ModulationSource = QStringList()
        << QT_TR_NOOP("LFO 1")
        << QT_TR_NOOP("LFO 2")
        << QT_TR_NOOP("Step Seq")
        << QT_TR_NOOP("Step Seq 1")
        << QT_TR_NOOP("Step Seq 2")
        << QT_TR_NOOP("Arpeggiator")
        << QT_TR_NOOP("Mod Wheel")
        << QT_TR_NOOP("Aftertouch")
        << QT_TR_NOOP("Pitch-Bend")
        << QT_TR_NOOP("Offset")
        << QT_TR_NOOP("CV 1")
        << QT_TR_NOOP("CV 2")
        << QT_TR_NOOP("CV 3")
        << QT_TR_NOOP("CV 4")
        << QT_TR_NOOP("CC A (16)")
        << QT_TR_NOOP("CC B (17)")
        << QT_TR_NOOP("CC C (18) Breath")
        << QT_TR_NOOP("CC D (19) Pedal")
        << QT_TR_NOOP("Noise")
        << QT_TR_NOOP("Env 1")
        << QT_TR_NOOP("Env 2")
        << QT_TR_NOOP("Velocity")
        << QT_TR_NOOP("Random")
        << QT_TR_NOOP("Note (Pitch)")
        << QT_TR_NOOP("Gate")
        << QT_TR_NOOP("Audio")
        << QT_TR_NOOP("Operator 1")
        << QT_TR_NOOP("Operator 2")
        << QT_TR_NOOP("=4")
        << QT_TR_NOOP("=8")
        << QT_TR_NOOP("=16")
        << QT_TR_NOOP("=32");


const QStringList Labels::ModulationDestination = QStringList()
        << QT_TR_NOOP("Filter cutoff")
        << QT_TR_NOOP("VCA gain")
        << QT_TR_NOOP("Osc 1 Param")
        << QT_TR_NOOP("Osc 2 Param")
        << QT_TR_NOOP("Osc 1 Pitch")
        << QT_TR_NOOP("Osc 2 Pitch")
        << QT_TR_NOOP("Osc 1+2 Pitch")
        << QT_TR_NOOP("Osc 1+2 Fine")
        << QT_TR_NOOP("Oscillator Mix")
        << QT_TR_NOOP("Noise Volume")
        << QT_TR_NOOP("Sub-Osc Vol")
        << QT_TR_NOOP("Resonance")
        << QT_TR_NOOP("CV 1")
        << QT_TR_NOOP("CV 2")
        << QT_TR_NOOP("Attack 1+2")
        << QT_TR_NOOP("LFO 1")
        << QT_TR_NOOP("LFO 2")
        << QT_TR_NOOP("Trig. Env. 1")
        << QT_TR_NOOP("Trig. Env. 2")
        << QT_TR_NOOP("Attack 1")
        << QT_TR_NOOP("Decay 1")
        << QT_TR_NOOP("Sustain 1")
        << QT_TR_NOOP("Release 1")
        << QT_TR_NOOP("Attack 2")
        << QT_TR_NOOP("Decay 2")
        << QT_TR_NOOP("Sustain 2")
        << QT_TR_NOOP("Release 2");


const QStringList Labels::OscillatorAlgorithm = QStringList()
        << QT_TR_NOOP("Silence")
        << QT_TR_NOOP("Sawtooth")
        << QT_TR_NOOP("Square wave")
        << QT_TR_NOOP("Triangle")
        << QT_TR_NOOP("Phase-dist saw (filter sweep)")
        << QT_TR_NOOP("Phase-dist saw (res filter sweep)")
        << QT_TR_NOOP("Phase-dist resonant triangle")
        << QT_TR_NOOP("Phase-dist trapezoidal creature")
        << QT_TR_NOOP("Phase-distortion/self-sync trick")
        << QT_TR_NOOP("4 detuned sawtooths") //Stack of 4 detuned sawtooth waves
        << QT_TR_NOOP("Minimal 2-operators FM")
        << QT_TR_NOOP("Waves")
        << QT_TR_NOOP("Tampur")
        << QT_TR_NOOP("Digital")
        << QT_TR_NOOP("Metal")
        << QT_TR_NOOP("Bowed")
        << QT_TR_NOOP("Slap")
        << QT_TR_NOOP("Organ")
        << QT_TR_NOOP("Male")
        << QT_TR_NOOP("User wavetable")
        << QT_TR_NOOP("Bitwise anarchy")
        << QT_TR_NOOP("Bit-crushed") // sine and triangle wave
        << QT_TR_NOOP("Naive square wave")
        << QT_TR_NOOP("Filtered noise generator")
        << QT_TR_NOOP("Vowel") //Low-tech formant synthesis
        << QT_TR_NOOP("Bellish")
        << QT_TR_NOOP("Polated")
        << QT_TR_NOOP("Cello")
        << QT_TR_NOOP("Clipsweep")
        << QT_TR_NOOP("Female")
        << QT_TR_NOOP("Fmntvocal")
        << QT_TR_NOOP("Formant2")
        << QT_TR_NOOP("Reso3hp")
        << QT_TR_NOOP("Electp")
        << QT_TR_NOOP("Vibes");


const QStringList Labels::SubOscillatorAlgorithm = QStringList()
        << QT_TR_NOOP("square, 1 octave below")
        << QT_TR_NOOP("triangle, 1 octave below")
        << QT_TR_NOOP("25% pulse, 1 octave below")
        << QT_TR_NOOP("square, 2 octaves below")
        << QT_TR_NOOP("triangle, 2 octaves below")
        << QT_TR_NOOP("25% pulse, 2 octaves below")
        << QT_TR_NOOP("discrete click")
        << QT_TR_NOOP("glitch")
        << QT_TR_NOOP("blow")
        << QT_TR_NOOP("high-pitched click")
        << QT_TR_NOOP("low-pitched discontinuity");


const QStringList Labels::LfoWave = QStringList()
        << QT_TR_NOOP("Triangle")
        << QT_TR_NOOP("Square")
        << QT_TR_NOOP("Sample&Hold")
        << QT_TR_NOOP("Ramp")
        << QT_TR_NOOP("Step")
        << QT_TR_NOOP("Wave 1")
        << QT_TR_NOOP("Wave 2")
        << QT_TR_NOOP("Wave 3")
        << QT_TR_NOOP("Wave 4")
        << QT_TR_NOOP("Wave 5")
        << QT_TR_NOOP("Wave 6")
        << QT_TR_NOOP("Wave 7")
        << QT_TR_NOOP("Wave 8")
        << QT_TR_NOOP("Wave 9")
        << QT_TR_NOOP("Wave 10")
        << QT_TR_NOOP("Wave 11")
        << QT_TR_NOOP("Wave 12")
        << QT_TR_NOOP("Wave 13")
        << QT_TR_NOOP("Wave 14")
        << QT_TR_NOOP("Wave 15")
        << QT_TR_NOOP("Wave 16");


const QStringList Labels::LfoMode = QStringList()
        << QT_TR_NOOP("Free")
        << QT_TR_NOOP("Slave")
        << QT_TR_NOOP("Master")
        << QT_TR_NOOP("Oneshot");


const QStringList Labels::Status = QStringList()
        << QT_TR_NOOP("OFF")
        << QT_TR_NOOP("ON");


const QStringList Labels::Operator = QStringList()
        << QT_TR_NOOP("Sum")
        << QT_TR_NOOP("Sync")
        << QT_TR_NOOP("Ring mod.")
        << QT_TR_NOOP("XOR")
        << QT_TR_NOOP("Fuzz")
        << QT_TR_NOOP(">>4")
        << QT_TR_NOOP(">>8")
        << QT_TR_NOOP("Fold")
        << QT_TR_NOOP("Bits")
        << QT_TR_NOOP("Duo")
        << QT_TR_NOOP("Ping Pong 2")
        << QT_TR_NOOP("Ping Pong 4")
        << QT_TR_NOOP("Ping Pong 8")
        << QT_TR_NOOP("Ping Png Seq");


const QStringList Labels::CvOperator = QStringList()
        << QT_TR_NOOP("Off")
        << QT_TR_NOOP("Sum")
        << QT_TR_NOOP("Product")
        << QT_TR_NOOP("Max")
        << QT_TR_NOOP("Min")
        << QT_TR_NOOP("XOR")
        << QT_TR_NOOP(">=")
        << QT_TR_NOOP("<=")
        << QT_TR_NOOP("qtz")
        << QT_TR_NOOP("lag");


const QStringList Labels::LFORate = QStringList()
        << "x1"
        << "x2"
        << "x3"
        << "x4"
        << "x5"
        << "x6"
        << "x7"
        << "x8"
        << "x9"
        << "x10"
        << "x11"
        << "x12"
        << "x13"
        << "x14"
        << "x15"
        << "x16";


QString Labels::LfoRateFormatter(int value) {  // used for labels
    if (value < 16)
        return LFORate.at(value);
    else
        return QString("%1").arg(value-16);
}


QString Labels::OffOnFormatter(int value) {
    if (value > 0) {
        return tr("On");
    } else {
        return tr("Off");
    }
}


const QStringList Labels::OffOn = QStringList()
        << QT_TR_NOOP("Off")
        << QT_TR_NOOP("On");


const QStringList Labels::FilterBoard = QStringList()
        << /* Filter 0*/ QT_TR_NOOP("SMR4 (lpf)")
        << /* Filter 1*/ QT_TR_NOOP("SSM2044 (ssm)")
        << /* Filter 2*/ QT_TR_NOOP("Dual SVF (svf)")
        << /* Filter 3*/ QT_TR_NOOP("Digital (dsp)")
        << /* Filter 4*/ QT_TR_NOOP("Polivoks (pvk)")
        << /* Filter 5*/ QT_TR_NOOP("4-Pole Mission (4pm)")
        << /* Filter 6*/ QT_TR_NOOP("LP2+Delay")
        << /* Filter 7*/ QT_TR_NOOP("Shruthacon (sp)");


const QStringList Labels::FilterSVFMode1 = QStringList()
        << QT_TR_NOOP("Lowpass")
        << QT_TR_NOOP("Bandpass")
        << QT_TR_NOOP("Highpass")
        << QT_TR_NOOP("Lowpass <")
        << QT_TR_NOOP("Bandpass <")
        << QT_TR_NOOP("Highpass <");


const QStringList Labels::FilterSVFMode2 = QStringList()
        << QT_TR_NOOP("Lowpass (parallel)")
        << QT_TR_NOOP("Bandpass (parallel)")
        << QT_TR_NOOP("Highpass (parallel)")
        << QT_TR_NOOP("Lowpass > (serial)")
        << QT_TR_NOOP("Bandpass > (serial)")
        << QT_TR_NOOP("Highpass > (serial)");


const QStringList Labels::FilterDSPMode = QStringList()
        << QT_TR_NOOP("LP > FX")
        << QT_TR_NOOP("HP > FX")
        << QT_TR_NOOP("FX > LP")
        << QT_TR_NOOP("FX > HP")
        << QT_TR_NOOP("FX");


const QStringList Labels::FilterDSPFx = QStringList()
        << QT_TR_NOOP("Distortion (distrt)")
        << QT_TR_NOOP("Bit crusher (crushr)")
        << QT_TR_NOOP("Comb/pos. fb. (+cmbfl)")
        << QT_TR_NOOP("Comb/neg. fb. (-cmbfl)")
        << QT_TR_NOOP("Ring modulator (ringmd)")
        << QT_TR_NOOP("Simple delay")
        << QT_TR_NOOP("Delay/30% fb. (fb dly)")
        << QT_TR_NOOP("Delay/80% fb. (dubdly)")
        << QT_TR_NOOP("Delay/30%/no AA (!fbdly)")
        << QT_TR_NOOP("Delay/80%/no AA (!dbdly)")
        << QT_TR_NOOP("1/16 tempo-sync. delay")
        << QT_TR_NOOP("1/12 tempo-sync. delay")
        << QT_TR_NOOP("1/8 tempo-sync. delay")
        << QT_TR_NOOP("3/16 tempo-sync. delay")
        << QT_TR_NOOP("Looper")
        << QT_TR_NOOP("Pitch shifter");


const QStringList Labels::FilterPolivoksMode = QStringList()
        << QT_TR_NOOP("Lowpass")
        << QT_TR_NOOP("Bandpass");


const QStringList Labels::Filter4PMMode = QStringList()
        << QT_TR_NOOP("4-pole LP")
        << QT_TR_NOOP("3-pole LP")
        << QT_TR_NOOP("2-pole LP")
        << QT_TR_NOOP("1-pole LP")
        << QT_TR_NOOP("1-pole HP")
        << QT_TR_NOOP("2-pole HP")
        << QT_TR_NOOP("3-pole HP")
        << QT_TR_NOOP("2-pole BP")
        << QT_TR_NOOP("4-pole BP")
        << QT_TR_NOOP("Notch filter")
        << QT_TR_NOOP("Phaser")
        << QT_TR_NOOP("2-p HP + 1-p LP (h21p)")
        << QT_TR_NOOP("3-p HP + 1-p LP (h31p)")
        << QT_TR_NOOP("Notch + 1-p LP (n+lp)")
        << QT_TR_NOOP("Allpass + 1-p LP (p+lp)");


const QStringList Labels::Filter4PMResonanceFlavor = QStringList()
        << QT_TR_NOOP("Liquid")
        << QT_TR_NOOP("MS")
        << QT_TR_NOOP("Wobbly")
        << QT_TR_NOOP("Whacky");


const QStringList Labels::FilterSPMode = QStringList()
        << QT_TR_NOOP("Lowpass")
        << QT_TR_NOOP("Bandpass")
        << QT_TR_NOOP("Highpass")
        << QT_TR_NOOP("Allpass");


const QStringList Labels::SequencerMode = QStringList()
        << QT_TR_NOOP("Step")
        << QT_TR_NOOP("Arpeggiator")
        << QT_TR_NOOP("Sequence");


const QStringList Labels::ArpeggiatorDirection = QStringList()
        << QT_TR_NOOP("Up")
        << QT_TR_NOOP("Down")
        << QT_TR_NOOP("Up/Down")
        << QT_TR_NOOP("?")
        << QT_TR_NOOP("Played");


const QStringList Labels::GrooveTemplate = QStringList()
        << QT_TR_NOOP("Swing")
        << QT_TR_NOOP("Shuffle")
        << QT_TR_NOOP("Push")
        << QT_TR_NOOP("Lag")
        << QT_TR_NOOP("Human")
        << QT_TR_NOOP("Monkey");


const QStringList Labels::SequencerClockDivision = QStringList()
        << "1/1"
        << "1/2"
        << "1/3"
        << "1/4"
        << "1/6"
        << "1/8"
        << "1/12"
        << "1/16"
        << "1/24"
        << "1/32"
        << "1/48"
        << "1/96";



QString Labels::TempoFormatter(int value) {
    if (value < 40) {
        return QString("ext");
    } else {
        return QString("%1").arg(value);
    }
}


QString Labels::ArpeggiatorPatternFormatter(int value) {
    if (value < 15) {
        return QString("%1").arg(value + 1);
    } else {
        return tr("sq");
    }
}


const QStringList Labels::Notes = QStringList()
        << QT_TR_NOOP("C")
        << QT_TR_NOOP("C#")
        << QT_TR_NOOP("D")
        << QT_TR_NOOP("D#")
        << QT_TR_NOOP("E")
        << QT_TR_NOOP("F")
        << QT_TR_NOOP("F#")
        << QT_TR_NOOP("G")
        << QT_TR_NOOP("G#")
        << QT_TR_NOOP("A")
        << QT_TR_NOOP("A#")
        << QT_TR_NOOP("B");


QString Labels::NoteFormatter(int value) {
    return Notes.at(value%12) + QString("%1").arg(value/12 - 1);
}


const QStringList Labels::HexValues = QStringList()
        << "0"
        << "1"
        << "2"
        << "3"
        << "4"
        << "5"
        << "6"
        << "7"
        << "8"
        << "9"
        << "A"
        << "B"
        << "C"
        << "D"
        << "E"
        << "F";
