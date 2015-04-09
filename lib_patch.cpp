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

#include "lib_patch.h"
#include <QDebug>
#include <iostream>
#include <QFile>
#include <QTextStream>
#include "stdlib.h"
#include "time.h"


// ******************************************
param_t Patch::parameters [108] = {
// ******************************************
    /*0*/ {"Oscillator 1 shape",0, 24,&Labels::OscillatorAlgorithm},
    /*1*/ {"Oscillator 1 parameter",0, 127,NULL},
    /*2*/ {"Oscillator 1 range",-24, 24,NULL},
    /*3*/ {"Modulation operator",0, 4, &Labels::Operator},
    /*4*/ {"Oscillator 2 shape",0, 23,&Labels::OscillatorAlgorithm},
    /*5*/ {"Oscillator 2 parameter",0, 127,NULL},
    /*6*/ {"Oscillator 2 range",-24, 24,NULL},
    /*7*/ {"Oscillator 2 detune",0, 127,NULL},
    /*8*/ {"Oscillator balance",0, 63,NULL},
    /*9*/ {"Sub oscillator volume",0, 63,NULL},
    /*10*/ {"Noise volume",0, 63,NULL},
    /*11*/ {"Sub oscillator/transient generator shape",0, 10, &Labels::SubOscillatorAlgorithm},
    /*12*/ {"Filter cutoff",0, 127,NULL},
    /*13*/ {"Filter resonance",0, 63,NULL},
    /*14*/ {"Envelope->cutoff modulation amount",0, 63,NULL},
    /*15*/ {"Lfo->cutoff modulation amount",0, 63,NULL},
    /*16*/ {"Envelope 1 attack",0, 127,NULL},
    /*17*/ {"Envelope 1 decay",0, 127,NULL},
    /*18*/ {"Envelope 1 sustain",0, 127,NULL},
    /*19*/ {"Envelope 1 release",0, 127,NULL},
    /*20*/ {"Envelope 2 attack",0, 127,NULL},
    /*21*/ {"Envelope 2 decay",0, 127,NULL},
    /*22*/ {"Envelope 2 sustain",0, 127,NULL},
    /*23*/ {"Envelope 2 release",0, 127,NULL},
    /*24*/ {"LFO 1 waveform",0, 20, &Labels::LfoWave},
    /*25*/ {"LFO 1 rate",0, 143, NULL}, //FORMATTER
    /*26*/ {"LFO 1 rise time",0, 127,NULL},
    /*27*/ {"LFO 1 master/slave",0, 2, &Labels::LfoMode},
    /*28*/ {"LFO 2 waveform",0, 20, &Labels::LfoWave},
    /*29*/ {"LFO 2 rate",0, 143, NULL}, // FORMATTER
    /*30*/ {"LFO 2 rise time",0, 127,NULL},
    /*31*/ {"LFO 2 master/slave",0, 2, &Labels::LfoMode},
    /*32*/ {"Modulation 1 source",0, 25, &Labels::ModulationSource},
    /*33*/ {"Modulation 1 destination",0, 16, &Labels::ModulationDestination},
    /*34*/ {"Modulation 1 amount",-63, 63,NULL},
    /*35*/ {"Modulation 2 source",0, 25, &Labels::ModulationSource},
    /*36*/ {"Modulation 2 destination",0, 16, &Labels::ModulationDestination},
    /*37*/ {"Modulation 2 amount",-63, 63,NULL},
    /*38*/ {"Modulation 3 source",0, 25, &Labels::ModulationSource},
    /*39*/ {"Modulation 3 destination",0, 16, &Labels::ModulationDestination},
    /*40*/ {"Modulation 3 amount",-63, 63,NULL},
    /*41*/ {"Modulation 4 source",0, 25, &Labels::ModulationSource},
    /*42*/ {"Modulation 4 destination",0, 16, &Labels::ModulationDestination},
    /*43*/ {"Modulation 4 amount",-63, 63,NULL},
    /*44*/ {"Modulation 5 source",0, 25, &Labels::ModulationSource},
    /*45*/ {"Modulation 5 destination",0, 16, &Labels::ModulationDestination},
    /*46*/ {"Modulation 5 amount",-63, 63,NULL},
    /*47*/ {"Modulation 6 source",0, 25, &Labels::ModulationSource},
    /*48*/ {"Modulation 6 destination",0, 16, &Labels::ModulationDestination},
    /*49*/ {"Modulation 6 amount",-63, 63,NULL},
    /*50*/ {"Modulation 7 source",0, 25, &Labels::ModulationSource},
    /*51*/ {"Modulation 7 destination",0, 16, &Labels::ModulationDestination},
    /*52*/ {"Modulation 7 amount",-63, 63,NULL},
    /*53*/ {"Modulation 8 source",0, 25, &Labels::ModulationSource},
    /*54*/ {"Modulation 8 destination",0, 16, &Labels::ModulationDestination},
    /*55*/ {"Modulation 8 amount",-63, 63,NULL},
    /*56*/ {"Modulation 9 source",0, 25, &Labels::ModulationSource},
    /*57*/ {"Modulation 9 destination",0, 16, &Labels::ModulationDestination},
    /*58*/ {"Modulation 9 amount",-63, 63,NULL},
    /*59*/ {"Modulation 10 source",0, 25, &Labels::ModulationSource},
    /*60*/ {"Modulation 10 destination",0, 16, &Labels::ModulationDestination},
    /*61*/ {"Modulation 10 amount",-63, 63,NULL},
    /*62*/ {"Modulation 11 source",0, 25, &Labels::ModulationSource},
    /*63*/ {"Modulation 11 destination",0, 16, &Labels::ModulationDestination},
    /*64*/ {"Modulation 11 amount",-63, 63,NULL},
    /*65*/ {"Modulation 12 source",0, 25, &Labels::ModulationSource},
    /*66*/ {"Modulation 12 destination",0, 16, &Labels::ModulationDestination},
    /*67*/ {"Modulation 12 amount",-63, 63,NULL},
    /*68*/ {NULL,0,0,NULL}, //68-75: Name
    /*69*/ {NULL,0,0,NULL},
    /*70*/ {NULL,0,0,NULL},
    /*71*/ {NULL,0,0,NULL},
    /*72*/ {NULL,0,0,NULL},
    /*73*/ {NULL,0,0,NULL},
    /*74*/ {NULL,0,0,NULL},
    /*75*/ {NULL,0,0,NULL},
    /*76*/ {"Performance 1 id",0,9, &Labels::PatchParameter}, //check max value
    /*77*/ {"Performance 1 subpage",0,9 ,NULL}, //check max value
    /*78*/ {"Performance 2 id",0,9, &Labels::PatchParameter}, //check max value
    /*79*/ {"Performance 1 subpage",0,9 ,NULL}, //check max value
    /*80*/ {"Performance 3 id",0,9, &Labels::PatchParameter}, //check max value
    /*81*/ {"Performance 1 subpage",0,9 ,NULL}, //check max value
    /*82*/ {"Performance 4 id",0,9, &Labels::PatchParameter}, //check max value
    /*83*/ {"Performance 1 subpage",0,9 ,NULL}, //check max value
    /*84*/ {"Filter cutoff 2",0, 127,NULL},
    /*85*/ {"Filter resonance 2",0, 63,NULL},
    /*86*/ {NULL,0,0,NULL}, //86: filter_topology_ (92<<4 | 93)
    /*87*/ {NULL,0,0,NULL}, //87-90: op_data_[4] (94, 95<<3 | 96, 97, 98<<3 | 99)
    /*88*/ {NULL,0,0,NULL},
    /*89*/ {NULL,0,0,NULL},
    /*90*/ {NULL,0,0,NULL},
    /*91*/ {NULL,0,0,NULL}, //91: exclamation_mark_
    /*92*/ {"Filter 1 mode",0, 2, &Labels::MainFilterMode}, // check
    /*93*/ {"Filter 2 mode",0, 2, &Labels::SecondaryFilterMode}, // check
    /*94*/ {"Operator 1 in1",0, 27, &Labels::ModulationSource},
    /*95*/ {"Operator 1 in2",0, 27, &Labels::ModulationSource},
    /*96*/ {"Operator 1 out",0, 7, &Labels::CvOperator},
    /*97*/ {"Operator 2 in1",0, 27, &Labels::ModulationSource},
    /*98*/ {"Operator 2 in2",0, 27, &Labels::ModulationSource},
    /*99*/ {"Operator 2 out",0, 7, &Labels::CvOperator},
    /*100*/ {"Sequencer mode",0, 6,NULL},
    /*101*/ {"Tempo",35, 248,NULL},
    /*102*/ {"Groove template",0, 5,NULL},
    /*103*/ {"Groove amount",0, 127,NULL},
    /*104*/ {"Arpeggiator direction",0, 3,NULL},
    /*105*/ {"Arpeggiator range", 0,4,NULL},
    /*106*/ {"Arpeggiator pattern",0, 15,NULL},
    /*107*/ {"Time warp",0, 18,NULL}
};


// ******************************************
unsigned char Patch::INIT_PATCH[] = 
// ******************************************
    {1, 0, 0, 0, 2, 16, 244, 12, 32, 0, 0, 0, 96, 0, 32, 0, 0, 50, 20, 60, 0, 40,
    90, 30, 0, 80, 0, 0, 0, 3, 0, 0, 0, 4, 0, 19, 5, 0, 19, 2, 0, 0, 3, 0, 1, 8, 0,
    2, 8, 0, 10, 2, 0, 11, 3, 0, 20, 1, 63, 21, 1, 16, 8, 6, 32, 0, 6, 16, 105, 110,
    105, 116, 32, 32, 32, 32, 1, 0, 12, 0, 13, 0, 14, 0, 0, 0, 0, 0, 0, 0, 0, 33};


// ******************************************
bool Patch::enabled(int i) {
// ******************************************
    return (i<68 || i==84 || i==85 || i>=92);
}


// ******************************************
Patch::Patch() {
// ******************************************
    resetPatch();
    srand(time(NULL));
}


// ******************************************
void Patch::setParam(int param, int value) {
// ******************************************
    data[param] = value;
}


// ******************************************
int Patch::getParam(int param) {
// ******************************************
    return data[param];
}


// ******************************************
void Patch::setName(QString new_name) {
// ******************************************
    name=new_name;
}


// ******************************************
QString Patch::getName() {
// ******************************************
    return name;
}


// ******************************************
void Patch::printPatch() {
// ******************************************
    qDebug() << "name: " << name;
    for (int i=0; i < 108; i++) if (enabled(i)) {
        const param_t param = parameters[i];
        if (parameters[i].dropdown)
            qDebug() << param.name << ": " << (*param.dropdown).at(data[i]);
        else
            qDebug() << param.name << ": " << data[i];
    }
}


// ******************************************
void Patch::resetPatch() {
// ******************************************
    parseSysex(INIT_PATCH);
}


// ******************************************
void Patch::randomizePatch() {
// ******************************************
    parseSysex(INIT_PATCH);
    for (int i=0; i < 108; i++) if (enabled(i)) {
        const param_t param = parameters[i];
        data[i]=(rand() % (param.max-param.min))+param.min;
    }
    name="random";
}


// ******************************************
void Patch::parseSysex(unsigned char sysex[]) {
// ******************************************
    int temp[108];
    
    // copy sysex data:
    for (int i=0; i<92 ;i++)
        temp[i] = sysex[i];
    for (int i=100; i<108 ;i++)
        temp[i] = 0;
    
    // uncompress data:
    temp[92] = temp[86]>>4; // filter_1_mode_
    temp[93] = temp[86]%16; // filter_2_mode_
    temp[94] = temp[87]; // op 1 in 1
    temp[95] = temp[88]>>3; // op 1 in 2
    temp[96] = temp[88]%8; // op 1 out
    temp[97] = temp[89]; // op 2 in 1
    temp[98] = temp[90]>>3; // op 2 in 2
    temp[99] = temp[90]%8; // op 2 out

    // fix negative values (2s complement):
    for (int i=0; i < 108; i++) if (enabled(i)) 
        if (parameters[i].min < 0)
            temp[i] -= (temp[i]>>7)*256;
    // store patch
    for (int i=0; i<108 ;i++)
        data[i] = temp[i];
    // read name:
    QStringList tmp_name = QStringList();
    for (int i=68; i<76; i++)
        tmp_name << QString("%1").arg((char)data[i]);
    name=tmp_name.join("").trimmed();
}


// ******************************************
void Patch::generateSysex (unsigned char res[]) {
// ******************************************
    int temp[108];
    // copy data:
    for (int i=0; i<108 ;i++)
        temp[i] = data[i];
    
    // fix negative values (2s complement):
    for (unsigned int i=0; i < 92; i++) if (enabled(i)) 
        if (parameters[i].min < 0 && temp[i]<0)        
            temp[i]+=256;
    // compress data
    temp[86] = (temp[92]<<4) | temp[93];
    temp[87] = temp[94];
    temp[88] = (temp[95]<<3) | temp[96];
    temp[89] = temp[97];
    temp[90] = (temp[98]<<3) | temp[99];
    temp[91] = 33; // ascii value of !
    
    // set name:
    QString temp_name = QString("%1").arg(name, -8, ' '); // pad name
    for (unsigned int i=0; i<8; i++) //->(68,76):
        temp[68+i]=temp_name.at(i).toLatin1();
    
    // copy data
    for (unsigned int i=0; i<92 ;i++)
        res[i] = (char) temp[i];
}


// ******************************************
bool Patch::parseFullSysex(unsigned char sysex[], unsigned int len) {
// ******************************************
    // check if valid:
    if (!(sysex[0]==Midi::sysexHead[0] && sysex[1]==Midi::sysexHead[1] && 
          sysex[2]==Midi::sysexHead[2] && sysex[3]==Midi::sysexHead[3] && 
          sysex[4]==Midi::sysexHead[4] && sysex[5]==Midi::sysexHead[5])) {
        qDebug() << "Invalid sysex header.";
        return false;
    }
    if (!(Midi::sysexFoot==sysex[len-1])) {
        qDebug() << "Invalid sysex footer.";
        return false;
    }
    if (!(1==sysex[6]&&0==sysex[7])) {
        qDebug() << "Sysex is not a patch.";
        return false;
    }
    // combine nibbles to bytes:
    int j=8;
    for (unsigned int i=8;i<len-1;i+=2)
        sysex[j++] = sysex[i]<<4|sysex[i+1];
    sysex[j]=sysex[len-1];
    len=j+1;
    
    if (!(33==sysex[len-3])) {
        qDebug() << "Invalid patch data.";
        return false;
    }
    if (!Midi::calculateChecksum(sysex,8,100)==sysex[len-2]) {
        qDebug() << "Invalid checksum.";
        return false;
    }
    
    // throw padding away:   
    unsigned char tmp[92];
    for (int i=0; i<92;i++)
        tmp[i]=sysex[8+i];

    parseSysex(tmp);
    return true;
}


// ******************************************
bool Patch::parseFullSysex(std::vector<unsigned char> message) {
// ******************************************
    // copy to temporay array:
    unsigned char *sysex = new unsigned char[message.size()];
    for (unsigned int i=0; i<message.size();i++)
        sysex[i] = message.at(i); 
    return parseFullSysex(sysex, message.size());
}


// ******************************************
void Patch::generateFullSysex(std::vector<unsigned char> *message) {
// ******************************************
    message->reserve(195); // Note: must have at least 195 entries.
    
    unsigned char temp[93];
    generateSysex(temp);
    
    for (unsigned int i=0; i<6; i++)
        message->push_back(Midi::sysexHead[i]);
    message->push_back(1);
    message->push_back(0);
    temp[92]=Midi::calculateChecksum(temp,0,92);
    
    // expand bytes to nibbles:
    for (unsigned int i=0;i<93;i++) {
        message->push_back((temp[i]>>4)&0x0F);
        message->push_back(temp[i]&0x0F);
    }
    message->push_back(Midi::sysexFoot);
}


// ******************************************
bool Patch::loadFromDisk(QString location) {
// ******************************************
#ifdef DEBUG
    qDebug() << "Patch::loadFromDisk(" << location << ")";
#endif
    QFile file(location);

    if(!file.exists()) {
        qDebug() << "The file does not exist.";
        return false;
    }

    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open.";
        return false;
    }

    char tmp [195];
    unsigned int readBytes = file.read(tmp,195);
    file.close();

#ifdef DEBUG
    qDebug() << "Read" << readBytes << "bytes.";   
#endif

    unsigned char sysex[195] = {};
    for (unsigned int i=0; i<readBytes; i++) {
         sysex[i] = (char) tmp[i];
#ifdef DEBUG
        qDebug() << i << ":" << sysex[i];
#endif
    }

    // primitive check if patch is valid:
    if (readBytes==195) {
#ifdef DEBUG
        qDebug() << "Detected full patch sysex.";   
#endif
        return parseFullSysex(sysex,195);
    } else if (readBytes==92 && tmp[91]==33) { // last field is !
#ifdef DEBUG
        qDebug() << "Detected light patch files.";   
#endif
        parseSysex(sysex);
        return true;
    } else {
        return false;
    }
};


// ******************************************
bool Patch::saveToDisk(QString location) {
// ******************************************
#ifdef DEBUG
    qDebug() << "Patch::saveToDisk(" << location << ")";
#endif
    QFile file(location);

    if(!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Could not open file for saving.";
        return false;
    }

    char temp[195];
    unsigned int len;
    if (location.endsWith(".syx")) {
        std::vector<unsigned char> sysex;
        generateFullSysex(&sysex);
        len = 195;
        for (unsigned int i=0; i<len; i++)
            temp[i]=(char) sysex[i];
    } else {
        unsigned char sysex[92] = {};
        generateSysex(sysex);
        len = 92;
        for (unsigned int i=0; i<len; i++)
            temp[i]=(char) sysex[i];
    }
    
    bool status = file.write(temp,len)==len;

    file.close();
    return status;
};
