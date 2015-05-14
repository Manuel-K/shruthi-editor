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

#include <QApplication>
#include <QtCore>
#include <QDebug>
#include <QStatusBar>

#include "ui/main_window.h"
#include "ui/keyboard_dialog.h"
#include "ui/sequence_editor.h"
#include "ui/library_dialog.h"

#include "editor.h"
#include "midiin.h"
#include "signalrouter.h"

#ifdef FUSION
#include <QStyleFactory>
#endif

// ******************************************
int main(int argc, char *argv[]) {
// ******************************************
    qRegisterMetaType<QueueItem>("QueueItem");
    qRegisterMetaType<Config>("Config");

    int retVal;

    {
        QApplication app(argc, argv);
#ifdef FUSION
        app.setStyle(QStyleFactory::create("Fusion"));
#endif

        // Setup signalrouter
        SignalRouter sr;
        QThread srThread;
        sr.connect(&srThread, SIGNAL(started()), SLOT(run()));
        sr.moveToThread(&srThread);

        // Setup editor
        Editor editor;
        QThread editorThread;
        editor.connect(&editorThread, SIGNAL(started()), SLOT(run()));
        editor.moveToThread(&editorThread);

        // editor: incoming signals
        editor.connect(&sr, SIGNAL(editorProcess(QueueItem)), SLOT(process(QueueItem)));
        editor.connect(&sr, SIGNAL(setMidiOutputPort(int)), SLOT(setMidiOutputPort(int)));
        editor.connect(&sr, SIGNAL(setMidiChannel(unsigned char)), SLOT(setMidiChannel(unsigned char)));
        editor.connect(&sr, SIGNAL(setShruthiFilterBoard(int)), SLOT(setShruthiFilterBoard(int)));


        // Setup midiin
        MidiIn midiin;
        QThread midiinThread;
        midiin.moveToThread(&midiinThread);
        midiinThread.start();
        // midiin: incoming signals
        midiin.connect(&sr, SIGNAL(setMidiInputPort(int)), SLOT(setMidiInputPort(int)));
        midiin.connect(&sr, SIGNAL(setShruthiFilterBoard(int)), SLOT(setShruthiFilterBoard(int)));

        // Setup main_window
        ShruthiEditorMainWindow *main_window = new ShruthiEditorMainWindow(&editor);
        main_window->setWindowIcon(QIcon(":/shruthi_editor.png"));
        main_window->setFixedSize(main_window->width(), main_window->height());
        main_window->statusBar()->setSizeGripEnabled(false);
        main_window->setAttribute(Qt::WA_DeleteOnClose, true);
        // main_window: incoming signals
        main_window->connect(&editor, SIGNAL(redrawPatchParamter(int)), SLOT(redrawPatchParameter(int)));
        main_window->connect(&editor, SIGNAL(redrawAllPatchParameters()), SLOT(redrawAllPatchParameters()));
        main_window->connect(&sr, SIGNAL(setMidiInputPort(int)), SLOT(setMidiInputPort(int)));
        main_window->connect(&sr, SIGNAL(setMidiOutputPort(int)), SLOT(setMidiOutputPort(int)));
        main_window->connect(&sr, SIGNAL(setMidiChannel(unsigned char)), SLOT(setMidiChannel(unsigned char)));
        main_window->connect(&sr, SIGNAL(setShruthiFilterBoard(int)), SLOT(setShruthiFilterBoard(int)));
        main_window->connect(&editor, SIGNAL(midiOutputStatusChanged(bool)), SLOT(midiOutputStatusChanged(bool)));
        main_window->connect(&editor, SIGNAL(displayStatusbar(QString)), SLOT(displayStatusbar(QString)));
        main_window->connect(&editor, SIGNAL(setStatusbarVersionLabel(QString)), SLOT(setStatusbarVersionLabel(QString)));
        main_window->connect(&midiin, SIGNAL(midiInputStatusChanged(bool)), SLOT(midiInputStatusChanged(bool)));

        // Setup keyboard
        KeyboardDialog keys;
        keys.connect(main_window, SIGNAL(showKeyboard()), SLOT(show()));
        keys.setWindowIcon(QIcon(":/shruthi_editor.png"));

        // Setup SequenceEditor
        SequenceEditor sequence_editor(&editor);
        sequence_editor.connect(main_window, SIGNAL(showSequenceEditor()), SLOT(show()));
        sequence_editor.setWindowIcon(QIcon(":/shruthi_editor.png"));
        sequence_editor.connect(&editor, SIGNAL(redrawAllSequenceParameters()), SLOT(redrawAllSequenceParameters()));
        sequence_editor.connect(&editor, SIGNAL(redrawAllPatchParameters()), SLOT(redrawAllPatchParameters()));
        sequence_editor.connect(&editor, SIGNAL(redrawPatchParameter2(int)), SLOT(redrawPatchParameter(int)));

        // Setup LibraryDialog
        LibraryDialog lib(editor.getLibrary());
        lib.setWindowIcon(QIcon(":/shruthi_editor.png"));
        lib.connect(main_window, SIGNAL(showLibrary()), SLOT(show()));
        lib.connect(&editor, SIGNAL(redrawLibraryItems(int,int,int)), SLOT(redrawItems(int,int,int)));


        // Start editor
        editorThread.start();

        // signalrouter: incoming signals
        sr.connect(&editor, SIGNAL(finished()), SLOT(editorFinished()));
        sr.connect(main_window, SIGNAL(enqueue(QueueItem)), SLOT(enqueue(QueueItem)));
        sr.connect(&sequence_editor, SIGNAL(enqueue(QueueItem)), SLOT(enqueue(QueueItem)));
        sr.connect(&midiin, SIGNAL(enqueue(QueueItem)), SLOT(enqueue(QueueItem)));
        sr.connect(&keys, SIGNAL(enqueue(QueueItem)), SLOT(enqueue(QueueItem)));
        sr.connect(&lib, SIGNAL(enqueue(QueueItem)), SLOT(enqueue(QueueItem)));
        sr.connect(main_window, SIGNAL(settingsChanged(Config)), SLOT(settingsChanged(Config)));

        // start signal router
        srThread.start();

        main_window->show();
        retVal= app.exec();
#ifdef DEBUGMSGS
        qDebug() << "main: shutting down...";

        qDebug() << "srThread.quit()";
#endif
        srThread.quit();
        srThread.wait(1000);

#ifdef DEBUGMSGS
        qDebug() << "editorThread.quit()";
#endif
        editorThread.quit();
        editorThread.wait(1000);
#ifdef DEBUGMSGS
        qDebug() << "midiinThread.quit()";
#endif
        midiinThread.quit();
        midiinThread.wait(1000);
    }

#ifdef DEBUGMSGS
    qDebug() << "return"<< retVal;
#endif
    return retVal;
}
