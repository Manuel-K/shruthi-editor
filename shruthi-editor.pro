# Shruthi-Editor: An unofficial Editor for the Shruthi hardware synthesizer. For 
# informations about the Shruthi, see <http:#www.mutable-instruments.net/shruthi1>. 
#
# Copyright (C) 2011 Manuel Krönig
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

HEADERS     = RtMidi.h settings-dialog.h shruthi-editor.h lib_patch.h lib_labels.h lib_editor.h lib_config.h lib_midiout.h lib_midiin.h keyboard-dialog.h
SOURCES     = RtMidi.cpp settings-dialog.cpp shruthi-editor.cpp main.cpp lib_patch.cpp lib_labels.cpp lib_editor.cpp lib_config.cpp lib_midiout.cpp lib_midiin.cpp  keyboard-dialog.cpp
FORMS       = settings-dialog.ui shruthi-editor.ui keyboard.ui
RC_FILE     = shruthi-editor.rc

CONFIG += qt #debug
#DEFINES+=DEBUG

LIBS += -L. 
target.path = release/
icons.path = release/
icons.files = shruthi-editor.png


unix {
    # alsa:
    DEFINES+=__LINUX_ALSASEQ__
    LIBS+=-lasound
    # jack;
    #DEFINES+=__LINUX_JACK__
    #LIBS+=-ljack
}
macx {
    DEFINES+=__MACOSX_CORE__
    LIBS+=-framework CoreMidi -framework CoreAudio -framework CoreFoundation
    ICON = shruthi-editor.icns
}
win32 {
    DEFINES+=__WINDOWS_MM__
    LIBS+=-lwinmm
    DEFINES+=CLEANLOOKS
    icons.files+=shruthi-editor.ico
}


# install
INSTALLS += target icons
