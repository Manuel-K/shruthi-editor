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

#include "lib_labels.h"
#include <QStringList>
#include <QDebug>

// ******************************************
QStringList Labels::ModulationSource = QStringList()
// ******************************************
  << "Lfo 1"
  << "Lfo 2"
  << "Step Seq"
  << "Step Seq 1"
  << "Step Seq 2"
  << "Arpeggiator"
  << "Mod Wheel"
  << "Aftertouch"
  << "Pitch-Bend"
  << "Offset"
  << "CV 1"
  << "CV 2"
  << "CV 3"
  << "CV 4"
  << "CC A (16)"
  << "CC B (17)"
  << "CC C (18) Breath"
  << "CC D (19) Pedal"
  << "Noise"
  << "Env 1"
  << "Env 2"
  << "Velocity"
  << "Random"
  << "Note (Pitch)"
  << "Gate"
  << "Audio"
  << "Operator 1"
  << "Operator 2"
  << "=4"
  << "=8"
  << "=16"
  << "=32";


// ******************************************
QStringList Labels::ModulationDestination = QStringList()
// ******************************************
  << "Filter cutoff"
  << "VCA gain"
  << "Osc 1 Param"
  << "Osc 2 Param"
  << "Osc 1 Pitch"
  << "Osc 2 Pitch"
  << "Osc 1+2 Pitch"
  << "Osc 1+2 Fine"
  << "Oscillator Mix"
  << "Noise Volume"
  << "Sub-Osc Vol"
  << "Resonance"
  << "CV 1"
  << "CV 2"
  << "Attack 1+2"
  << "Lfo 1"
  << "Lfo 2"
  << "Trig. Env. 1"
  << "Trig. Env. 2"
  << "Attack 1"
  << "Decay 1"
  << "Sustain 1"
  << "Release 1"
  << "Attack 2"
  << "Decay 2"
  << "Sustain 2"
  << "Release 2";


// ******************************************
QStringList Labels::OscillatorAlgorithm = QStringList()
// ******************************************
  << "Silence"
  << "Sawtooth"
  << "Square wave"
  << "Triangle"
  << "Phase-dist saw (filter sweep)"
  << "Phase-dist saw (res filter sweep)"
  << "Phase-dist resonant triangle"
  << "Phase-dist trapezoidal creature"
  << "Phase-distortion/self-sync trick"
  << "4 detuned sawtooths" //Stack of 4 detuned sawtooth waves
  << "Minimal 2-operators FM"
  << "Waves"
  << "Tampur"
  << "Digital"
  << "Metal"
  << "Bowed"
  << "Slap"
  << "Organ"
  << "Male"
  << "User wavetable"
  << "Bitwise anarchy"
  << "Bit-crushed" // sine and triangle wave
  << "Naive square wave"
  << "Filtered noise generator"
  << "Vowel" //Low-tech formant synthesis
  << "Bellish"
  << "Polated"
  << "Cello"
  << "Clipsweep"
  << "Female"
  << "Fmntvocal"
  << "Formant2"
  << "Reso3hp"
  << "Electp"
  << "Vibes";

// ******************************************
QStringList Labels::SubOscillatorAlgorithm = QStringList()
// ******************************************
  << "square, 1 octave below"
  << "triangle, 1 octave below"
  << "25% pulse, 1 octave below"
  << "square, 2 octaves below"
  << "triangle, 2 octaves below"
  << "25% pulse, 2 octaves below"
  << "discrete click"
  << "glitch"
  << "blow"
  << "high-pitched click"
  << "low-pitched discontinuity";

// ******************************************
QStringList Labels::LfoWave = QStringList()
// ******************************************
  << "Triangle"
  << "Square"
  << "Sample&Hold"
  << "Ramp"
  << "Step"
  << "Wave 1"
  << "Wave 2"
  << "Wave 3"
  << "Wave 4"
  << "Wave 5"
  << "Wave 6"
  << "Wave 7"
  << "Wave 8"
  << "Wave 9"
  << "Wave 10"
  << "Wave 11"
  << "Wave 12"
  << "Wave 13"
  << "Wave 14"
  << "Wave 15"
  << "Wave 16";

// ******************************************
QStringList Labels::LfoMode = QStringList()
// ******************************************
  << "Free"
  << "Slave"
  << "Master"
  << "Oneshot";

// ******************************************
QStringList Labels::Status = QStringList()
// ******************************************
  << "OFF"
  << "ON";

// ******************************************
QStringList Labels::Operator = QStringList()
// ******************************************
  << "Sum"
  << "Sync"
  << "Ring mod."
  << "XOR"
  << "Fuzz"
  << ">>4"
  << ">>8"
  << "Fold"
  << "Bits"
  << "Duo"
  << "Ping Pong 2"
  << "Ping Pong 4"
  << "Ping Pong 8"
  << "Ping Png Seq";

// ******************************************
QStringList Labels::CvOperator = QStringList()
// ******************************************
  << "Off"
  << "Sum"
  << "Product"
  << "Max"
  << "Min"
  << "XOR"
  << ">="
  << "<="
  << "qtz"
  << "lag";


// ******************************************
QStringList Labels::LFORate = QStringList()
// ******************************************
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

// ******************************************
QString Labels::LfoRateFormatter(int value) {  // used for labels
// ******************************************
    if (value < 16)
        return LFORate.at(value);
    else
        return QString("%1").arg(value-16);
}


// ******************************************
QString Labels::OffOnFormatter(int value) {
// ******************************************
    if (value > 0) {
        return "On";
    } else {
        return "Off";
    }
}


// ******************************************
QStringList Labels::OffOn = QStringList()
// ******************************************
        << "Off"
        << "On";


// ******************************************
QStringList Labels::FilterBoard = QStringList()
// ******************************************
/* Filter 0*/ << "SMR4 (lpf)"
/* Filter 1*/ << "SSM2044 (ssm)"
/* Filter 2*/ << "Dual SVF (svf)"
/* Filter 3*/ << "Digital (dsp)"
/* Filter 4*/ << "Polivoks (pvk)"
/* Filter 5*/ << "4-Pole Mission (4pm)"
/* Filter 6*/ << "LP2+Delay"
/* Filter 7*/ << "Shruthacon (sp)";


// ******************************************
QStringList Labels::FilterSVFMode1 = QStringList()
// ******************************************
  << "Lowpass"
  << "Bandpass"
  << "Highpass"
  << "Lowpass <"
  << "Bandpass <"
  << "Highpass <";


// ******************************************
QStringList Labels::FilterSVFMode2 = QStringList()
// ******************************************
  << "Lowpass (parallel)"
  << "Bandpass (parallel)"
  << "Highpass (parallel)"
  << "Lowpass > (serial)"
  << "Bandpass > (serial)"
  << "Highpass > (serial)";


// ******************************************
QStringList Labels::FilterDSPMode = QStringList()
// ******************************************
        << "LP > FX"
        << "HP > FX"
        << "FX > LP"
        << "FX > HP"
        << "FX";


// ******************************************
QStringList Labels::FilterDSPFx = QStringList()
// ******************************************
        << "Distortion (distrt)"
        << "Bit crusher (crushr)"
        << "Comb/pos. fb. (+cmbfl)"
        << "Comb/neg. fb. (-cmbfl)"
        << "Ring modulator (ringmd)"
        << "Simple delay"
        << "Delay/30% fb. (fb dly)"
        << "Delay/80% fb. (dubdly)"
        << "Delay/30%/no AA (!fbdly)"
        << "Delay/80%/no AA (!dbdly)"
        << "1/16 tempo-sync. delay"
        << "1/12 tempo-sync. delay"
        << "1/8 tempo-sync. delay"
        << "3/16 tempo-sync. delay"
        << "Looper"
        << "Pitch shifter";


// ******************************************
QStringList Labels::FilterPolivoksMode = QStringList()
// ******************************************
        << "Lowpass"
        << "Bandpass";


// ******************************************
QStringList Labels::Filter4PMMode = QStringList()
// ******************************************
        << "4-pole LP"
        << "3-pole LP"
        << "2-pole LP"
        << "1-pole LP"
        << "1-pole HP"
        << "2-pole HP"
        << "3-pole HP"
        << "2-pole BP"
        << "4-pole BP"
        << "Notch filter"
        << "Phaser"
        << "2-p HP + 1-p LP (h21p)"
        << "3-p HP + 1-p LP (h31p)"
        << "Notch + 1-p LP (n+lp)"
        << "Allpass + 1-p LP (p+lp)";


// ******************************************
QStringList Labels::Filter4PMResonanceFlavor = QStringList()
// ******************************************
        << "Liquid"
        << "MS"
        << "Wobbly"
        << "Whacky";


// ******************************************
QStringList Labels::FilterSPMode = QStringList()
// ******************************************
        << "Lowpass"
        << "Bandpass"
        << "Highpass"
        << "Allpass";


// ******************************************
QStringList Labels::SequencerMode = QStringList()
// ******************************************
        << "Step"
        << "Arpeggiator"
        << "Sequence";


// ******************************************
QStringList Labels::ArpeggiatorDirection = QStringList()
// ******************************************
        << "Up"
        << "Down"
        << "Up/Down"
        << "?"
        << "Played";


// ******************************************
QStringList Labels::GrooveTemplate = QStringList()
// ******************************************
        << "Swing"
        << "Shuffle"
        << "Push"
        << "Lag"
        << "Human"
        << "Monkey";


// ******************************************
QStringList Labels::SequencerClockDivision = QStringList()
// ******************************************
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



// ******************************************
QString Labels::TempoFormatter(int value)
// ******************************************
{
    if (value < 40) {
        return QString("External %1").arg(value);
    } else {
        return QString("%1").arg(value);
    }
}


// ******************************************
QString Labels::ArpeggiatorPatternFormatter(int value)
// ******************************************
{
    if (value < 15) {
        return QString("%1").arg(value + 1);
    } else {
        return "Sequence";
    }
}


// ******************************************
QStringList Labels::Notes = QStringList()
// ******************************************
        << "C"
        << "C#"
        << "D"
        << "D#"
        << "E"
        << "F"
        << "F#"
        << "G"
        << "G#"
        << "A"
        << "A#"
        << "B";


// ******************************************
QString Labels::NoteFormatter(int value)
// ******************************************
{
    return Notes.at(value%12) + QString("%1").arg(value/12 - 1);
}


// ******************************************
QStringList Labels::HexValues = QStringList()
// ******************************************
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
