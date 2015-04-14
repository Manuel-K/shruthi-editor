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
QStringList Labels::PatchParameter = QStringList();
// ******************************************
//   << "PRM_OSC_SHAPE_1"
//   << "PRM_OSC_PARAMETER_1"
//   << "PRM_OSC_RANGE_1"
//   << "PRM_OSC_OPTION_1"
//
//   << "PRM_OSC_SHAPE_2"
//   << "PRM_OSC_PARAMETER_2"
//   << "PRM_OSC_RANGE_2"
//   << "PRM_OSC_OPTION_2"
//
//   << "PRM_MIX_BALANCE"
//   << "PRM_MIX_SUB_OSC"
//   << "PRM_MIX_NOISE"
//   << "PRM_MIX_SUB_OSC_SHAPE"
//
//   << "PRM_FILTER_CUTOFF"
//   << "PRM_FILTER_RESONANCE"
//   << "PRM_FILTER_ENV"
//   << "PRM_FILTER_LFO"
//
//   << "PRM_ENV_ATTACK_1"
//   << "PRM_ENV_DECAY_1"
//   << "PRM_ENV_SUSTAIN_1"
//   << "PRM_ENV_RELEASE_1"
//
//   << "PRM_ENV_ATTACK_2"
//   << "PRM_ENV_DECAY_2"
//   << "PRM_ENV_SUSTAIN_2"
//   << "PRM_ENV_RELEASE_2"
//
//   << "PRM_LFO_WAVE_1"
//   << "PRM_LFO_RATE_1"
//   << "PRM_LFO_ATTACK_1"
//   << "PRM_LFO_RETRIGGER_1"
//
//   << "PRM_LFO_WAVE_2"
//   << "PRM_LFO_RATE_2"
//   << "PRM_LFO_ATTACK_2"
//   << "PRM_LFO_RETRIGGER_2"
//
//   << "PRM_MOD_SOURCE"
//   << "PRM_MOD_DESTINATION"
//   << "PRM_MOD_AMOUNT"
//   << "PRM_MOD_ROW"
//
//   << "PRM_FILTER_CUTOFF_2 = 84"
//   << "PRM_FILTER_RESONANCE_2 = 85"
//   << "PRM_FILTER_MODE_1 = 92"
//   << "PRM_FILTER_MODE_2 = 93"
//
//   << "PRM_OP_OP1 = 94"
//   << "PRM_OP_OP2 = 95"
//   << "PRM_OP_OPERATOR = 96"
//   << "PRM_OP_ROW = 97";


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
QStringList Labels::FilterBoard = QStringList()
// ******************************************
/* Filter 0*/ << "SMR4 (lpf)"
/* Filter 1*/ << "SSM2044 (ssm)"
/* Filter 2*/ << "Dual SVF (svf)"
/* Filter 3*/ << "Digital (dsp)"
/* Filter 4*/ << "Polivoks (pvk)"
/* Filter 5*/ << "4pm"
/* Filter 6*/ << "LP2+delay"
/* Filter 7*/ << "sp";


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
        << "l>f"
        << "h>f"
        << "f>l"
        << "f>h"
        << ">fx";


// ******************************************
QStringList Labels::FilterDSPFx = QStringList()
// ******************************************
        << "distrt"
        << "crushr"
        << "+cmbfl"
        << "-cmbfl"
        << "ringmd"
        << "delay"
        << "fb dly"
        << "dubdly"
        << "!fbdly"
        << "!dbdly"
        << "/16 dl"
        << "/12 dl"
        << "/8 dly"
        << "3/16dl"
        << "looper"
        << "pitch";


// ******************************************
QStringList Labels::FilterPolivoksMode = QStringList()
// ******************************************
        << "Lowpass"
        << "Bandpass";


// ******************************************
QStringList Labels::Filter4PMMode = QStringList()
// ******************************************
        << "lp4"
        << "lp3"
        << "lp2"
        << "lp1"
        << "hp1"
        << "hp2"
        << "hp3"
        << "bp2"
        << "bp4"
        << "notch"
        << "phaser"
        << "h21p"
        << "h31p"
        << "n+lp"
        << "p+lp";


// ******************************************
QStringList Labels::Filter4PMFeedbackFlavor = QStringList()
// ******************************************
        << "liquid"
        << "ms"
        << "wobbly"
        << "whacky";


// ******************************************
QStringList Labels::FilterSPMode = QStringList()
// ******************************************
        << "Lowpass"
        << "Bandpass"
        << "Highpass"
        << "ap";


// ******************************************
QStringList Labels::OffOn = QStringList()
// ******************************************
        << "Off"
        << "On";
