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


#include "patch.h"
#include <math.h> // for ceil, floor
#include <stdlib.h> // for srand, rand
#include <time.h>
#include <QStringList>
#include <iostream>
#include "labels.h"
#include "midi.h"


const unsigned char Patch::parameterCount = 110;


const PatchParameter Patch::parameters [110] = {
    /* Long name, short name, min, max, labels, value formatter, cc */
    /*0*/ {"Oscillator 1 shape", "Shape", 0, 34, &Labels::OscillatorAlgorithm, NULL, 20},
    /*1*/ {"Oscillator 1 parameter", "Param", 0, 127, NULL, NULL, 21},
    /*2*/ {"Oscillator 1 range", "Range",-24, 24, NULL, NULL, 22},
    /*3*/ {"Modulation operator", "Operator", 0, 13, &Labels::Operator, NULL, 28},
    /*4*/ {"Oscillator 2 shape", "Shape", 0, 34, &Labels::OscillatorAlgorithm, NULL, 24},
    /*5*/ {"Oscillator 2 parameter", "Param", 0, 127, NULL, NULL, 25},
    /*6*/ {"Oscillator 2 range", "Range",-24, 24, NULL, NULL, 26},
    /*7*/ {"Oscillator 2 detune", "Detune", 0, 127, NULL, NULL, 27},
    /*8*/ {"Oscillator balance", "Mix", 0, 63, NULL, NULL, 29},
    /*9*/ {"Sub oscillator volume", "Sub. Vol.", 0, 63, NULL, NULL, 30},
    /*10*/ {"Noise volume", "Noise", 0, 63, NULL, NULL, 31},
    /*11*/ {"Sub oscillator/transient generator shape", "Sub-Osc", 0, 10, &Labels::SubOscillatorAlgorithm, NULL, 23},
    /*12*/ {"Filter cutoff", "Cutoff", 0, 127, NULL, NULL, 14}, // alternate cc: 74
    /*13*/ {"Filter resonance", "Resonance", 0, 63, NULL, NULL, 15}, // alternate cc: 71
    /*14*/ {"Envelope->cutoff modulation amount", "Evn Amnt", 0, 63, NULL, NULL, 102},
    /*15*/ {"Lfo->cutoff modulation amount", "Lfo2 Amnt", 0, 63, NULL, NULL, 103},
    /*16*/ {"Envelope 1 attack", "Attack", 0, 127, NULL, NULL, 104},
    /*17*/ {"Envelope 1 decay", "Decay", 0, 127, NULL, NULL, 105},
    /*18*/ {"Envelope 1 sustain", "Sustain", 0, 127, NULL, NULL, 106},
    /*19*/ {"Envelope 1 release", "Release", 0, 127, NULL, NULL, 107},
    /*20*/ {"Envelope 2 attack", "Attack", 0, 127, NULL, NULL, 108}, // alternate cc: 73
    /*21*/ {"Envelope 2 decay", "Decay", 0, 127, NULL, NULL, 109},
    /*22*/ {"Envelope 2 sustain", "Sustain", 0, 127, NULL, NULL, 110},
    /*23*/ {"Envelope 2 release", "Release", 0, 127, NULL, NULL, 111},
    /*24*/ {"LFO 1 waveform", "Shape", 0, 20, &Labels::LfoWave, NULL, 112},
    /*25*/ {"LFO 1 rate", "Rate", 0, 143, NULL, Labels::LfoRateFormatter, 113},
    /*26*/ {"LFO 1 rise time", "Attack", 0, 127, NULL, NULL, 114},
    /*27*/ {"LFO 1 master/slave", "Sync Mode", 0, 3, &Labels::LfoMode, NULL, 115},
    /*28*/ {"LFO 2 waveform", "Shape", 0, 20, &Labels::LfoWave, NULL, 116},
    /*29*/ {"LFO 2 rate", "Rate", 0, 143, NULL, Labels::LfoRateFormatter, 117},
    /*30*/ {"LFO 2 rise time", "Attack", 0, 127, NULL, NULL, 118},
    /*31*/ {"LFO 2 master/slave", "Sync Mode", 0, 3, &Labels::LfoMode, NULL, 119},
    /*32*/ {"Modulation 1 source", NULL, 0, 27, &Labels::ModulationSource, NULL, -1},
    /*33*/ {"Modulation 1 destination", NULL, 0, 26, &Labels::ModulationDestination, NULL, -1},
    /*34*/ {"Modulation 1 amount", NULL,-63, 63, NULL, NULL, -1},
    /*35*/ {"Modulation 2 source", NULL, 0, 27, &Labels::ModulationSource, NULL, -1},
    /*36*/ {"Modulation 2 destination", NULL, 0, 26, &Labels::ModulationDestination, NULL, -1},
    /*37*/ {"Modulation 2 amount", NULL,-63, 63, NULL, NULL, -1},
    /*38*/ {"Modulation 3 source", NULL, 0, 27, &Labels::ModulationSource, NULL, -1},
    /*39*/ {"Modulation 3 destination", NULL, 0, 26, &Labels::ModulationDestination, NULL, -1},
    /*40*/ {"Modulation 3 amount", NULL,-63, 63, NULL, NULL, -1},
    /*41*/ {"Modulation 4 source", NULL, 0, 27, &Labels::ModulationSource, NULL, -1},
    /*42*/ {"Modulation 4 destination", NULL, 0, 26, &Labels::ModulationDestination, NULL, -1},
    /*43*/ {"Modulation 4 amount", NULL,-63, 63, NULL, NULL, -1},
    /*44*/ {"Modulation 5 source", NULL, 0, 27, &Labels::ModulationSource, NULL, -1},
    /*45*/ {"Modulation 5 destination", NULL, 0, 26, &Labels::ModulationDestination, NULL, -1},
    /*46*/ {"Modulation 5 amount", NULL,-63, 63, NULL, NULL, -1},
    /*47*/ {"Modulation 6 source", NULL, 0, 27, &Labels::ModulationSource, NULL, -1},
    /*48*/ {"Modulation 6 destination", NULL, 0, 26, &Labels::ModulationDestination, NULL, -1},
    /*49*/ {"Modulation 6 amount", NULL,-63, 63, NULL, NULL, -1},
    /*50*/ {"Modulation 7 source", NULL, 0, 27, &Labels::ModulationSource, NULL, -1},
    /*51*/ {"Modulation 7 destination", NULL, 0, 26, &Labels::ModulationDestination, NULL, -1},
    /*52*/ {"Modulation 7 amount", NULL,-63, 63, NULL, NULL, -1},
    /*53*/ {"Modulation 8 source", NULL, 0, 27, &Labels::ModulationSource, NULL, -1},
    /*54*/ {"Modulation 8 destination", NULL, 0, 26, &Labels::ModulationDestination, NULL, -1},
    /*55*/ {"Modulation 8 amount", NULL,-63, 63, NULL, NULL, -1},
    /*56*/ {"Modulation 9 source", NULL, 0, 27, &Labels::ModulationSource, NULL, -1},
    /*57*/ {"Modulation 9 destination", NULL, 0, 26, &Labels::ModulationDestination, NULL, -1},
    /*58*/ {"Modulation 9 amount", NULL,-63, 63, NULL, NULL, -1},
    /*59*/ {"Modulation 10 source", NULL, 0, 27, &Labels::ModulationSource, NULL, -1},
    /*60*/ {"Modulation 10 destination", NULL, 0, 26, &Labels::ModulationDestination, NULL, -1},
    /*61*/ {"Modulation 10 amount", NULL,-63, 63, NULL, NULL, -1},
    /*62*/ {"Modulation 11 source", NULL, 0, 27, &Labels::ModulationSource, NULL, -1},
    /*63*/ {"Modulation 11 destination", NULL, 0, 26, &Labels::ModulationDestination, NULL, -1},
    /*64*/ {"Modulation 11 amount", NULL,-63, 63, NULL, NULL, -1},
    /*65*/ {"Modulation 12 source", NULL, 0, 27, &Labels::ModulationSource, NULL, -1},
    /*66*/ {"Modulation 12 destination", NULL, 0, 26, &Labels::ModulationDestination, NULL, -1},
    /*67*/ {"Modulation 12 amount", NULL,-63, 63, NULL, NULL, -1},
    /*68*/ param_blank, //68-75: Name
    /*69*/ param_blank,
    /*70*/ param_blank,
    /*71*/ param_blank,
    /*72*/ param_blank,
    /*73*/ param_blank,
    /*74*/ param_blank,
    /*75*/ param_blank,
    /*76*/ param_blank, // (system_settings.legato ? 0x40 : 0x00) | system_settings.portamento
    /*77*/ param_blank, // seq_mode
    /*78*/ param_blank, // seq_tempo
    /*79*/ param_blank, // seq_groove_template
    /*80*/ param_blank, // seq_groove_amount
    /*81*/ param_blank, // (arp_direction << 4) | arp_range
    /*82*/ param_blank, // arp_pattern
    /*83*/ param_blank, // arp_clock_division
    /*84*/ {"SVF filter cutoff 2", "Cutoff2", 0, 127, NULL, NULL, 12},
    /*85*/ {"SVF filter resonance 2", "Res2", 0, 63, NULL, NULL, 13},
    /*86*/ param_blank, //86: filter_topology_ (92<<4 | 93)
    /*87*/ param_blank, //87-90: op_data_[4] (94, 95<<3 | 96, 97, 98<<3 | 99)
    /*88*/ param_blank,
    /*89*/ param_blank,
    /*90*/ param_blank,
    /*91*/ param_blank, //91: version_
    /*92*/ {"SVF filter mode 1", "Mode 1", 0, 5, &Labels::FilterSVFMode1, NULL, 85},
    /*93*/ {"SVF filter mode 2", "Mode 2", 0, 5, &Labels::FilterSVFMode2, NULL, 86},
    /*94*/ {"Operator 1 in1", NULL, 0, 31, &Labels::ModulationSource, NULL, -1},
    /*95*/ {"Operator 1 in2", NULL, 0, 31, &Labels::ModulationSource, NULL, -1},
    /*96*/ {"Operator 1 out", NULL, 0, 9, &Labels::CvOperator, NULL, -1},
    /*97*/ {"Operator 2 in1", NULL, 0, 31, &Labels::ModulationSource, NULL, -1},
    /*98*/ {"Operator 2 in2", NULL, 0, 31, &Labels::ModulationSource, NULL, -1},
    /*99*/ {"Operator 2 out", NULL, 0, 9, &Labels::CvOperator, NULL, -1},
    /*100*/ {"Sequencer mode", "Seq mode", 0, 2, &Labels::SequencerMode, NULL, 75},
    /*101*/ {"Tempo", "Tempo", 39, 240, NULL, Labels::TempoFormatter, -1}, // 35..248 according to manual; (<40 external); min with encoder is 39, with poti it's 38
    /*102*/ {"Groove template", "Grv tmp", 0, 5, &Labels::GrooveTemplate, NULL, 76},
    /*103*/ {"Groove amount", "Grv amt", 0, 127, NULL, NULL, 77},
    /*104*/ {"Arpeggiator direction", "Arp dir", 0, 4, &Labels::ArpeggiatorDirection, NULL, 78}, // 0..3 according to manual
    /*105*/ {"Arpeggiator range", "Arp rng", 1, 4, NULL, NULL, 79},
    /*106*/ {"Arpeggiator pattern", "Arp pat", 0, 15, NULL, Labels::ArpeggiatorPatternFormatter, 80}, // 0..15 according to manual; (highest==sequence); label range: 1..15,manual
    /*107*/ {"Sequencer Clock Division", "Cld div", 0, 11, &Labels::SequencerClockDivision, NULL, 81},
    /* no NRPN support: */
    /*108*/ {"Portamento", "Portamento", 0, 63, NULL, NULL, 84},
    /*109*/ {"Legato", "Legato", 0, 1, &Labels::OffOn, NULL, 68}
};


const unsigned char Patch::filterBoardCount = 8;


const PatchParameter Patch::parameter84 [8] = {
    /*Filter 0*/ {NULL, NULL, 0, 127, NULL, NULL, -1},
    /*Filter 1*/ {"SSM high pass", "High Pass", 0, 127, NULL, NULL, 12},
    /*Filter 2*/ {"SVF filter cutoff 2", "Cutoff2", 0, 127, NULL, NULL, 12},
    /*Filter 3*/ {"DSP FX Parameter 1", "Param. 1", 0, 127, NULL, NULL, 12},
    /*Filter 4*/ {"Polivoks overdrive", "Overdrive", 0, 1, NULL, Labels::OffOnFormatter, 90}, // &Labels::OffOn
    /*Filter 5*/ {NULL, NULL, 0, 127, NULL, NULL, -1},
    /*Filter 6*/ {"Delay time", "Time", 0, 127, NULL, NULL, 12},
    /*Filter 7*/ {"SP feedback", "Feedback", 0, 127, NULL, NULL, 12}, // TODO: check cc
};


const PatchParameter Patch::parameter85 [8] = {
    /*Filter 0*/ {NULL, NULL, 0, 127, NULL, NULL, -1},
    /*Filter 1*/ {NULL, NULL, 0, 127, NULL, NULL, -1},
    /*Filter 2*/ {"SVF filter resonance 2", "Res2", 0, 127, NULL, NULL, 13},
    /*Filter 3*/ {"DSP FX Parameter 2", "Param. 2", 0, 63, NULL, NULL, 13},
    /*Filter 4*/ {"Polivoks FM feedback", "FM feedbk.", 0, 1, NULL, Labels::OffOnFormatter, 91}, // &Labels::OffOn
    /*Filter 5*/ {NULL, NULL, 0, 127, NULL, NULL, -1},
    /*Filter 6*/ {"Delay level", "Level", 0, 63, NULL, NULL, 13},
    /*Filter 7*/ {NULL, NULL, 0, 127, NULL, NULL, -1},
};


const PatchParameter Patch::parameter92 [8] = {
    /*Filter 0*/ {NULL, NULL, 0, 2, NULL, NULL, -1},
    /*Filter 1*/ {NULL, NULL, 0, 2, NULL, NULL, -1},
    /*Filter 2*/ {"SVF filter mode 1", "Mode 1", 0, 5, &Labels::FilterSVFMode1, NULL, 85},
    /*Filter 3*/ {"DSP FX mode", "FX Mode", 0, 4, &Labels::FilterDSPMode, NULL, 87},
    /*Filter 4*/ {"Polivoks filter mode", "Mode", 0, 1, &Labels::FilterPolivoksMode, NULL, 89},
    /*Filter 5*/ {"4PM filter mode", "Mode", 0, 14, &Labels::Filter4PMMode, NULL, 92},
    /*Filter 6*/ {"Delay feedback", "Feedback", 0, 15, NULL, NULL, 94},
    /*Filter 7*/ {"SP mode", "Mode", 0, 3, &Labels::FilterSPMode, NULL, 96},
};


const PatchParameter Patch::parameter93 [8] = {
    /*Filter 0*/ {NULL, NULL, 0, 5, NULL, NULL, -1},
    /*Filter 1*/ {NULL, NULL, 0, 5, NULL, NULL, -1},
    /*Filter 2*/ {"SVF filter mode 2", "Mode 2", 0, 5, &Labels::FilterSVFMode2, NULL, 86},
    /*Filter 3*/ {"DSP FX Program", "FX Program", 0, 15, &Labels::FilterDSPFx, NULL, 88},
    /*Filter 4*/ {NULL, NULL, 0, 127, NULL, NULL, -1},
    /*Filter 5*/ {"4PM resonance flavor", "Resonance Flavor", 0, 3, &Labels::Filter4PMResonanceFlavor, NULL, 93},
    /*Filter 6*/ {"Delay EQ flavor", "EQ Flavor", 0, 15, NULL, NULL, 95},
    /*Filter 7*/ {NULL, NULL, 0, 127, NULL, NULL, -1},
};


/*
 * Notes on other filter boards:
 * DSP FX param 1 (cc 12) uses param 84
 * DSP FX param 2 (cc 12) uses param 85
 * DSP FX mode (cc 87) uses param 92
 * DSP FX program (cc 88) uses param 93
 * Polivoks filter mode (cc 89) uses param 92
 * Polivoks overdrive (cc 90) uses param 84
 * Polivoks Fm feedback (cc 91) uses param 85
 * 4PM filter mode (cc 92) uses param 92
 * 4PM resonance flavor (cc 93) uses param 93
 * Delay time (cc 12) uses SVF param 84
 * Delay level (cc 13) uses param 85
 * Delay feedback (cc 94) uses param 92
 * Delay EQ flavor (cc 95) uses param 93
 * SP Mode (cc 96) uses param 92
 * SP Feedback uses param 84
 */


PatchParameter Patch::parameter(const int &id, int filter) {
    if (id >= parameterCount || filter >= filterBoardCount) {
#ifdef DEBUGMSGS
        qDebug() << "Patch::parameter() called with invalid arguments." << id << filter;
#endif
        return param_blank;
    }

    // Handle different filters:
    switch (id) {
        case 84:
            return parameter84[filter];
        case 85:
            return parameter85[filter];
        case 92:
            return parameter92[filter];
        case 93:
            return parameter93[filter];
        default:
            return parameters[id];
    }
}


const unsigned char Patch::INIT_PATCH[] =
{1, 0, 0, 0, 1, 16, 244, 12, 32, 0, 0, 0, 96, 0, 32, 0, 0, 50, 20, 60, 0, 40,
 90, 30, 0, 80, 0, 0, 0, 3, 0, 0, 0, 4, 0, 19, 5, 0, 19, 2, 0, 0, 3, 0, 1, 8, 0,
 2, 8, 0, 21, 2, 0, 21, 3, 0, 20, 1, 63, 21, 1, 16, 8, 6, 32, 0, 6, 16, 105, 110,
 105, 116, 32, 32, 32, 32, 0, 0, 120, 0, 0, 1, 0, 7, 0, 0, 0, 0, 0, 0, 0, 37};


const unsigned char Patch::INIT_PATCH_PRE100[] =
{1, 0, 0, 0, 2, 16, 244, 12, 32, 0, 0, 0, 96, 0, 32, 0, 0, 50, 20, 60, 0, 40,
 90, 30, 0, 80, 0, 0, 0, 3, 0, 0, 0, 4, 0, 19, 5, 0, 19, 2, 0, 0, 3, 0, 1, 8, 0,
 2, 8, 0, 10, 2, 0, 11, 3, 0, 20, 1, 63, 21, 1, 16, 8, 6, 32, 0, 6, 16, 105, 110,
 105, 116, 32, 32, 32, 32, 1, 0, 12, 0, 13, 0, 14, 0, 0, 0, 0, 0, 0, 0, 0, 33};
// Note: pairwise equal for 0.93 and 0.98


bool Patch::enabled(const int &id) {
    return (id < 68 || id == 84 || id == 85 || (id >= 92 && id < parameterCount));
}


bool Patch::hasUI(const int &id) {
    return (id < 68 || id == 84 || id == 85 || (id >= 92 && id <= 99));
}


bool Patch::hasUI2(const int &id) {
    return (id >= 100 && id < 110);
}


bool Patch::sendAsNRPN(const int &id) {
    return (id < 68 || id == 84 || id == 85 || (id >= 92 && id < 108));
}


bool Patch::belongsToModMatrix(const int &id) {
    return (32 <= id && id <= 67);
}


Patch::Patch() {
    reset();
    srand(time(NULL));
}


Patch::Patch(const unsigned int &version) {
    reset(version);
    srand(time(NULL));
}


void Patch::setValue(const int &id, int const value) {
    data[id] = value;
}


const int &Patch::getValue(const int &id) const {
    return data[id];
}


QString Patch::formatParameterValue(const int &id, const int &value, int filter) {
    const PatchParameter &param_entry = parameter(id, filter);

    if (param_entry.string_values) {
        return (*param_entry.string_values).at(value);
    } else {
        if (param_entry.formatter) {
            return (*param_entry.formatter)(value);
        }
        return QString("%1").arg(value);
    }
}


void Patch::setName(const QString &new_name) {
    name=new_name;
}


const QString &Patch::getName() const {
    return name;
}

QString Patch::getVersionString() const {
    if (version == 33) {
        return "0.9x";
    } else if (version == 37) {
        return "1.xx";
    }
    return "Unknown";
}


void Patch::printPatch() const {
    std::cout << "name: " << name.toUtf8().constData() << std::endl;
    for (int i=0; i < parameterCount; i++) {
        if (enabled(i)) {
            std::cout << parameters[i].name.toUtf8().constData() << ": "
                      << formatParameterValue(i, data[i]).toUtf8().constData() << std::endl;
        }
    }
}


void Patch::reset(unsigned int version) {
    if (version >= 1000) {
        unpackData(INIT_PATCH);
    } else {
        unpackData(INIT_PATCH_PRE100);
    }
}


void Patch::randomize(const int &filter) {
    unpackData(INIT_PATCH);
    for (int i=0; i < parameterCount; i++) {
        if (enabled(i)) { // do we want to randomize sequencer settings?
            const PatchParameter param = parameter(i, filter);
            data[i] = (rand() %(param.max-param.min))+param.min;
        }
    }
    name = "random";
}


bool Patch::unpackData(const unsigned char sysex[]) {
    // check if version field contains valid entry
    if (!(sysex[91] == 33 || sysex[91] == 37)) {
        return false;
    }

    int temp[parameterCount];

    // copy sysex data:
    for (int i=0; i<92 ;i++)
        temp[i] = sysex[i];
    for (int i = 100; i < parameterCount; i++) {
        temp[i] = 0;
    }

    // uncompress data:
    temp[92] = temp[86]>>4; // filter_1_mode_
    temp[93] = temp[86]%16; // filter_2_mode_
    temp[94] = temp[87]; // op 1 in 1
    temp[95] = temp[88]>>3; // op 1 in 2
    temp[96] = temp[88]%8; // op 1 out
    temp[97] = temp[89]; // op 2 in 1
    temp[98] = temp[90]>>3; // op 2 in 2
    temp[99] = temp[90]%8; // op 2 out
    version = temp[91];
#ifdef DEBUGMSGS
    std::cout << "Parsed patch with version " << version << "/" << (int)version << "." << std::endl;
#endif
    // uncompress v1.00 data:
    if (version == 37) {
        // temp[76] holds (system_settings.legato ? 0x40 : 0x00) | system_settings.portamento
        temp[109] = (temp[76] & 0x40) > 0;  // legato
        temp[108] = temp[76] % 64; // portamento
        temp[100] = temp[77]; // seq_mode
        temp[101] = temp[78]; // seq_tempo
        temp[102] = temp[79]; // seq_groove_template
        temp[103] = temp[80]; // seq_groove_amount
        // temp[81] holds (arp_direction << 4) | arp_range;
        temp[104] = temp[81] >> 4; // arp_direction
        temp[105] = temp[81] % 16; // arp_range
        temp[106] = temp[82]; // arp_pattern
        temp[107] = temp[83]; // arp_clock_division
    }

    // fix negative values (2s complement):
    for (int i = 0; i < parameterCount; i++) {
        if (enabled(i) && parameters[i].min < 0) {
            temp[i] -= (temp[i]>>7)*256;
        }
    }
    // store patch
    for (int i = 0; i < parameterCount; i++) {
        data[i] = temp[i];
    }
    // read name:
    QStringList tmp_name = QStringList();
    for (int i=68; i<76; i++)
        tmp_name << QString("%1").arg((char)data[i]);
    name = tmp_name.join("").trimmed();

    return true;
}


void Patch::packData(unsigned char res[]) const {
    int temp[parameterCount];
    // copy data:
    for (int i = 0; i < parameterCount; i++)
        temp[i] = data[i];

    // fix negative values (2s complement):
    for (unsigned int i=0; i < parameterCount; i++) {
        if (enabled(i) && parameters[i].min < 0 && temp[i] < 0) {
            temp[i]+=256;
        }
    }
    // compress data
    temp[86] = (temp[92]<<4) | temp[93];
    temp[87] = temp[94];
    temp[88] = (temp[95]<<3) | temp[96];
    temp[89] = temp[97];
    temp[90] = (temp[98]<<3) | temp[99];
    temp[91] = version; // ascii value of ! or %
    // compress v1.00 data:
    if (version == 37) {
        temp[76] = (temp[109] ? 0x40 : 0x00) | temp[108]; // (legato ? 0x40 : 0x00) | portamento;
        temp[77] = temp[100]; // seq_mode
        temp[78] = temp[101]; // seq_tempo
        temp[79] = temp[102]; // seq_groove_template
        temp[80] = temp[103]; // seq_groove_amount
        temp[81] = (temp[104] << 4) | temp[105]; // (arp_direction << 4) | arp_range
        temp[82] = temp[106]; // arp_pattern
        temp[83] = temp[107]; // arp_clock_division
    }

    // set name:
    QString temp_name = QString("%1").arg(name, -8, ' '); // pad name
    for (unsigned int i = 0; i < 8; i++) { //->(68,76):
        temp[68 + i] = temp_name.at(i).toLatin1();
    }

    // copy data
    for (unsigned int i=0; i<92; i++) {
        res[i] = (char) temp[i];
    }
}


unsigned char Patch::ccToId(const unsigned char &cc, const int &filter) {
    // Oscillator 1
    if (cc >= 20 && cc <= 22) {
        return cc - 20;
    }
    // Modulation operator
    if (cc == 28) {
        return 3;
    }
    // Oscillator 2
    if (cc >= 24 && cc <= 27) {
        return cc - 20;
    }
    // Oscillator ballance, Sub volume, Noise volume
    if (cc >= 29 && cc <= 31) {
        return cc - 21;
    }
    // Sub shape
    if (cc == 23) {
        return 11;
    }
    // Filter cutoff
    if (cc == 14 || cc == 74) {
        return 12;
    }
    // Filter resonance
    if (cc == 15 || cc == 71) {
        return 13;
    }
    // Envelope 1/Envelope 2/LFO
    if (cc >= 102 && cc <= 119) {
        return cc - 88;
    }
    // Envelope 2 attack alias
    if (cc == 73) {
        return 20;
    }
    // Sequencer Mode
    if (cc == 75) {
        return 100;
    }
    // Groove template/amount, Arpeggiator dir/range/pattern, Seq clock devision
    if (cc >= 76 && cc <= 81) {
        return cc + 26;
    }
    // Portamento
    if (cc == 84) {
        return 108; // no NRPN support!
    }
    // Legato
    if (cc == 68) {
        return 109; // no NRPN support!
    }
    //SVF filter cutoff2/resonance2
    if (cc == 12 || cc == 13) {
        return cc + 72;
    }
    //SVF filter mode 1/2
    if (cc == 85 || cc == 86) {
        return cc + 7;
    }
    // other filters (see Labels::FilterBoard for indices):
    // DSP FX mode, Polivoks filter mode, 4PM filter mode, Delay feedback, SP mode
    if ((filter == 3 && cc == 87) || (filter == 4 && cc == 89) || (filter == 5 && cc == 92) || (filter == 6 && cc == 94) || (filter == 7 && cc == 96)) {
        return 92;
    }
    // DSP FX program, 4PM resonance flavor, Delay EQ flavor
    if ((filter == 3 && cc == 88) || (filter == 5 && cc == 93) || (filter == 6 && cc == 95)) {
        return 93;
    }
    // Polivoks overdrive
    if ((filter == 4 && cc == 90)) {
        return 84;
    }
    // Polivoks Fm feedback
    if ((filter == 4 && cc == 91)) {
        return 85;
    }

    return 255; // Not supported
}


int Patch::convertCCValue(const unsigned int &val, int &id, const int &filter) {
    if (id >= parameterCount) {
        return 255; // Not supported
    }
    const int min = parameter(id, filter).min;
    const int max = parameter(id, filter).max;
    const double perc = val / 127.0;

    // Try to emulate Shruthi's LFO rate extrapolation:
    if (id == 25  || id == 29) {
        int temp = min + floor(perc * (max - min));
        if (temp == 143) {
            return 142;
        }
        return temp;
    }

    return min + ceil(perc * (max - min));
}


bool Patch::parseSysex(const Message *message) {
    Message payload;
    if (!Midi::parseSysex(message, &payload)) {
        return false;
    }

    // copy to temporay array:
    unsigned char temp[payload.size()];
    for (unsigned int i = 0; i < payload.size(); i++) {
        temp[i] = payload.at(i);
    }

    return unpackData(temp);
}


void Patch::generateSysex(Message *message) const {
    unsigned char temp[92];
    packData(temp);

    Message payload;
    for (unsigned int i = 0; i < 92; i++) {
        payload.push_back(temp[i]);
    }

    Midi::generateSysex(&payload, 0x01, 0x00, message);
}


bool Patch::equals(const Patch &other) const {
    for (unsigned int i = 0; i < parameterCount; i++) {
        if (enabled(i) && data[i] != other.data[i]) {
            return false;
        }
    }
    if (version != other.version ||
            name.compare(other.name) != 0) {
        return false;
    }
    // compare the performance page settings:
    if (version == 33) {
        for (unsigned int i = 76; i <= 83; i++) {
            if (data[i] != other.data[i]) {
                return false;
            }
        }
    }
    return true;
}


void Patch::set(const Patch &other) {
    for (unsigned int i = 0; i < parameterCount; i++) {
        data[i] = other.data[i];
    }
    version = other.version;
    name = other.name;
}
