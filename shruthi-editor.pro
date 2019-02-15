# Shruthi-Editor: An unofficial Editor for the Shruthi hardware synthesizer. For
# informations about the Shruthi, see <http:#www.mutable-instruments.net/shruthi1>.
#
# Copyright (C) 2011-2019 Manuel Krönig
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


isEmpty(PREFIX) {
    win32:PREFIX = C:/shruti-editor
    else:PREFIX = /usr/local
}


#
# Source files
#


HEADERS = \
    ui/keyboard_dialog.h \
    ui/keyboard_widget.h \
    ui/library_dialog.h \
    ui/main_window.h \
    ui/sequence_editor.h \
    ui/sequence_step.h \
    ui/settings_dialog.h \
    ui/shruthi_editor_dial.h \
    RtMidi.h \
    config.h \
    editor.h \
    fileio.h \
    flag.h \
    labels.h \
    library.h \
    message.h \
    midi.h \
    midiin.h \
    midiout.h \
    patch.h \
    queueitem.h \
    sequence.h \
    sequence_parameter.h \
    signalrouter.h \
    version.h

SOURCES = \
    ui/keyboard_dialog.cpp \
    ui/keyboard_widget.cpp \
    ui/library_dialog.cpp \
    ui/main_window.cpp \
    ui/sequence_editor.cpp \
    ui/sequence_step.cpp \
    ui/settings_dialog.cpp \
    ui/shruthi_editor_dial.cpp \
    RtMidi.cpp \
    config.cpp \
    editor.cpp \
    fileio.cpp \
    labels.cpp \
    library.cpp \
    main.cpp \
    midi.cpp \
    midiin.cpp \
    midiout.cpp \
    patch.cpp \
    sequence.cpp \
    signalrouter.cpp

FORMS = \
    ui/keyboard_dialog.ui \
    ui/library_dialog.ui \
    ui/main_window.ui \
    ui/sequence_editor.ui \
    ui/sequence_step.ui \
    ui/settings_dialog.ui \
    ui/shruthi_editor_dial.ui

RESOURCES = \
    ui/shruthi_editor.qrc

TRANSLATIONS = \
    translations/shruthi_editor_fr_FR.ts


QM_FILES_INSTALL_PATH = $$PREFIX/share/shruthi_editor/translations
include(translations.pri)


#
# Settings
#

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += qt

#DEFINES += DEBUGMSGS

# Uncomment the following line if you use a firmware version prior to 0.94
#DEFINES += PRE094SYSEXHEADER

LIBS += -L.
target.path = release/

# Comment the following line if you don't want to force the Fusion style for Qt 5
greaterThan(QT_MAJOR_VERSION, 4): DEFINES += FUSION

# Comment the following line if you don't want to force the Cleanlooks style for Qt 4
equals(QT_MAJOR_VERSION, 4): DEFINES += CLEANLOOKS

# Generate and embed translations
CONFIG += lrelease embed_translations

unix:!macx {
    message(RtMidi will use linux alsaseq.)
    # alsa:
    DEFINES += __LINUX_ALSA__
    LIBS += -lasound
    # jack:
    #DEFINES += __LINUX_JACK__
    #LIBS += -ljack
}

macx {
    message(RtMidi will use macosx core.)
    DEFINES += __MACOSX_CORE__
    LIBS += -framework CoreMidi -framework CoreAudio -framework CoreFoundation
    ICON = ui/shruthi_editor.icns
}

win32 {
    message(RtMidi will use windows mm.)
    DEFINES += __WINDOWS_MM__
    LIBS += -lwinmm
    RC_FILE = ui/shruthi_editor.rc
}


# install
INSTALLS += target
target.path = $$PREFIX/bin
