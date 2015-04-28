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

#include <QtWidgets>
#include "shruthi-editor.h"
#include "settings-dialog.h"
#include "lib_labels.h"
#include "lib_patch.h"

#define VERSION "0.22"

// ******************************************
ShruthiEditorMainWindow::ShruthiEditorMainWindow(Editor *edit, QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
// ******************************************
    ui->setupUi(this);
    editor = edit;

    MIDI_CHANNEL = 1;
    MIDI_INPUT_PORT = 0;
    MIDI_OUTPUT_PORT = 0;
    SHRUTHI_FILTER_BOARD = 0;

    statusbarVersionLabel = new QLabel("Version ");
    statusBar()->addPermanentWidget(statusbarVersionLabel);
    // The label is reparented (made a child of the statusbar), so it will get
    // deleted automatically when the statusbar is destroyed.

    // Setup Dials/ComboBoxes:
    QDial* tmp_d;
    QComboBox* tmp_c;
    QLabel *tmp_l;
    ShruthiEditorDial * tmp_sed;
    for (int i=0; i<100; i++) {
        if (Patch::hasUI(i)) {
            if (Patch::parameters[i].dropdown) {
                tmp_c = this->findChild<QComboBox*>(QString("c%1").arg(i));
                if (!tmp_c) {
                    qDebug() << "ComboBox" << QString("c%1").arg(i) << "could not be found!";
                    continue;
                }
                tmp_c->addItems(*(Patch::parameters[i].dropdown));
                connect(tmp_c,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxChanged(int)));
            } else if (Patch::belongsToModMatrix(i)){ // small dials
                tmp_d = this->findChild<QDial*>(QString("c%1").arg(i));
                if (!tmp_d) {
                    qDebug() << "Dial" << QString("c%1").arg(i) << "could not be found!";
                    continue;
                }
                tmp_d->setMinimum(Patch::parameters[i].min);
                tmp_d->setMaximum(Patch::parameters[i].max);
                tmp_l = this->findChild<QLabel*>(QString("d%1").arg(i));
                if (tmp_l) {
                    tmp_l->setText("0");
                } else {
                    qDebug() << "Label" << QString("d%1").arg(i) << "could not be found!";
                    continue;
                }
                connect(tmp_d,SIGNAL(valueChanged(int)), this, SLOT(dialChanged(int)));
            } else {
                tmp_sed = this->findChild<ShruthiEditorDial*>(QString("c%1").arg(i));
                if (!tmp_sed) {
                    qDebug() << "ShruthiEditorDial" << QString("c%1").arg(i) << "could not be found!";
                    continue;
                }
                tmp_sed->setParameter(i);
                tmp_sed->setLimits(Patch::parameters[i].min, Patch::parameters[i].max);
                tmp_sed->setName(Patch::parameters[i].short_name + ":");
                tmp_sed->setFormatter(Patch::parameters[i].formatter);
                connect(tmp_sed, SIGNAL(valueChanged(int,int)), this, SLOT(dialChanged(int,int)));
            }
        }
    }
    // Setup additional dials for parameters 92 and 93:
    for (int i = 92; i <= 93; i++) {
        if (Patch::hasUI(i)) {
            tmp_sed = this->findChild<ShruthiEditorDial*>(QString("c%1d").arg(i));
            if (!tmp_sed) {
                qDebug() << "ShruthiEditorDial" << QString("c%1d").arg(i) << "could not be found!";
                continue;
            }
            tmp_sed->setParameter(i);
            tmp_sed->setLimits(Patch::parameters[i].min, Patch::parameters[i].max);
            tmp_sed->setName(Patch::parameters[i].short_name + ":");
            tmp_sed->setFormatter(Patch::parameters[i].formatter);
            connect(tmp_sed, SIGNAL(valueChanged(int,int)), this, SLOT(dialChanged(int,int)));
        }
    }

    connect(ui->patch_name, SIGNAL(editingFinished()), this, SLOT(patchNameChanged()));

    // Now that everything is set up, update all UI elements:
    redrawAllPatchParameters();

    // other UI Signals:
    connect(ui->actionLoad_Patch, SIGNAL(triggered()), this, SLOT(loadPatch()));
    connect(ui->actionSave_Patch, SIGNAL(triggered()), this, SLOT(savePatch()));
    connect(ui->actionFetch_Patch, SIGNAL(triggered()), this, SLOT(fetchPatch()));
    connect(ui->actionSend_Patch, SIGNAL(triggered()), this, SLOT(sendPatch()));
    connect(ui->actionFetchSequence, SIGNAL(triggered()), this, SLOT(fetchSequence()));
    connect(ui->actionSendSequence, SIGNAL(triggered()), this, SLOT(sendSequence()));
    connect(ui->actionOpenSettings, SIGNAL(triggered()), this, SLOT(openSettings()));
    connect(ui->actionReset_Patch, SIGNAL(triggered()), this, SLOT(resetPatch()));
    connect(ui->actionReset_Patch_pre_1_00, SIGNAL(triggered()), this, SLOT(resetPatchPre100()));
    connect(ui->actionQuit,SIGNAL(triggered()), this, SLOT(quitShruthiEditor()));
    connect(ui->actionAbout_Shruthi_Editor, SIGNAL(triggered()), this, SLOT(aboutShruthiEditor()));
    connect(ui->actionAbout_Qt, SIGNAL(triggered()), this, SLOT(aboutQt()));
    connect(ui->actionRandomize_Patch, SIGNAL(triggered()), this, SLOT(randomizePatch()));
    connect(ui->actionKeyboard, SIGNAL(triggered()), this, SIGNAL(showKeyboard()));
    connect(ui->actionOpenSequenceEditor, SIGNAL(triggered()), this, SIGNAL(showSequenceEditor()));
    connect(ui->actionLoadSequence, SIGNAL(triggered()), this, SLOT(loadSequence()));
    connect(ui->actionSaveSequence, SIGNAL(triggered()), this, SLOT(saveSequence()));
    connect(ui->actionResetSequence, SIGNAL(triggered()), this, SLOT(resetSequence()));
}


// ******************************************
ShruthiEditorMainWindow::~ShruthiEditorMainWindow()
// ******************************************
{
    delete ui;
}


// ******************************************
void ShruthiEditorMainWindow::setMidiInputPort(int midiin) {
// ******************************************
#ifdef DEBUGMSGS
    qDebug() << "shruthiEditorMainWindow::setMidiInputPort(" << midiin << ")";
#endif
    MIDI_INPUT_PORT = midiin;
}


// ******************************************
void ShruthiEditorMainWindow::setMidiOutputPort(int midiout) {
// ******************************************
#ifdef DEBUGMSGS
    qDebug() << "shruthiEditorMainWindow::setMidiOutputPort(" << midiout << ")";
#endif
    MIDI_OUTPUT_PORT = midiout;
}


// ******************************************
void ShruthiEditorMainWindow::setMidiChannel(unsigned char channel) {
// ******************************************
#ifdef DEBUGMSGS
    qDebug() << "shruthiEditorMainWindow::setMidiChannel(" << channel << ")";
#endif
    MIDI_CHANNEL = channel;
}


// ******************************************
void ShruthiEditorMainWindow::setShruthiFilterBoard(int filter)
// ******************************************
{
#ifdef DEBUGMSGS
    qDebug() << "shruthiEditorMainWindow::setShruthiFilterBoard(" << filter << ")";
#endif
    SHRUTHI_FILTER_BOARD = filter;

    //
    // Hide all unnecessary GUI elements and show the rest:
    //

    // Disable all relevant widgets:
    ui->c92->setEnabled(false);
    ui->c93->setEnabled(false);

    // Parameter 84:
    bool p84dial = false;
    const param_t p84 = Patch::parameter(84, filter);
    if (p84.short_name != NULL) {
        ui->c84->setName(p84.short_name + ":");
        if (p84.dropdown == NULL) {
            p84dial = true;
            ui->c84->setLimits(p84.min, p84.max);
            ui->c84->setFormatter(p84.formatter);
        }
    }

    // dial visibility
    ui->c84->setHidden(!p84dial);


    // Parameter 85:
    bool p85dial = false;
    const param_t p85 = Patch::parameter(85, filter);
    if (p85.short_name != NULL) {
        ui->c85->setName(p85.short_name + ":");
        if (p85.dropdown == NULL) {
            p85dial = true;
            ui->c85->setLimits(p85.min, p85.max);
            ui->c85->setFormatter(p85.formatter);
        }
    }

    // dial visibility
    ui->c85->setHidden(!p85dial);


    // Parameter 92:
    QStringList *p92combo = NULL;
    bool p92dial = false;

    const param_t p92 = Patch::parameter(92, filter);
    if (p92.short_name != NULL) {
        if (p92.dropdown == NULL) {
            ui->c92d->setName(p92.short_name + ":");
            p92dial = true;
            ui->c92d->setLimits(p92.min, p92.max);
            ui->c92d->setFormatter(p92.formatter);
        } else {
            ui->l92->setText(p92.short_name + ":");
            p92combo = p92.dropdown;
        }
    }

    // dial visibility
    ui->c92d->setHidden(!p92dial);

    // combo box visibility
    bool p92hidden = true;
    if (p92combo) {
        ui->c92->clear();
        ui->c92->addItems(*p92combo);
        // need to set index?
        p92hidden = false;
    }

    ui->w92->setHidden(p92hidden);


    // Parameter 93:
    QStringList *p93combo = NULL;
    bool p93dial = false;

    const param_t p93 = Patch::parameter(93, filter);
    if (p93.short_name != NULL) {
        if (p93.dropdown == NULL) {
            ui->c93d->setName(p93.short_name + ":");
            p93dial = true;
            ui->c93d->setLimits(p93.min, p93.max);
            ui->c93d->setFormatter(p93.formatter);
        } else {
            ui->l93->setText(p93.short_name + ":");
            p93combo = p93.dropdown;
        }
    }

    // dial visibility
    ui->c93d->setHidden(!p93dial);

    // combo box visibility
    bool p93hidden = true;
    if (p93combo) {
        ui->c93->clear();
        ui->c93->addItems(*p93combo);
        // need to set index?
        p93hidden = false;
    }
    ui->w93->setHidden(p93hidden);


    // change p92/p93 page:
    // page 0 contains the dials, page 1 the comboboxes
    ui->sw9293->setCurrentIndex(!p92hidden || !p93hidden);


    // Force display update:
    redrawPatchParameter(84);
    redrawPatchParameter(85);
    redrawPatchParameter(92);
    redrawPatchParameter(93);


    // Enable active widgets:
    ui->c92->setEnabled(!p92hidden);
    ui->c93->setEnabled(!p93hidden);
}


// ******************************************
// ******************************************
// Local UI Signal Handlers
// ******************************************
// ******************************************

// ******************************************
void ShruthiEditorMainWindow::comboBoxChanged(int val) {
// ******************************************
    // Never send 'no current item set' i.e. -1:
    if (val == -1)
        return;

    QComboBox* s = (QComboBox*) sender();

    // Don't send changed signal if element is disabled:
    if (!s->isEnabled())
        return;

    QString id = s->objectName();
    id.remove(0,1);

    queueitem_t signal(PATCH_PARAMETER_CHANGE_EDITOR, id.toInt(), val);
    emit enqueue(signal);
}


// ******************************************
void ShruthiEditorMainWindow::dialChanged(int val) {
// ******************************************
    QDial* s = (QDial*) sender();

    QString id = s->objectName();

    // Fix for additional dials (e.g. parameter 92/93):
    if (id.endsWith('d')) {
        id.chop(1);
    }

    QString temp = id;
    temp.remove(0, 1);
    int param = temp.toInt();

    // Update label:
    id.replace(0,1,"d");

    QLabel *temp2 = this->findChild<QLabel*>(id);;
    if (temp2) {
        temp2->setText(Patch::formatParameterValue(param, val, SHRUTHI_FILTER_BOARD));
    } else {
        qDebug() << "Label" << id << "could not be found!";
    }

    // Don't send changed signal if element is disabled:
    if (!s->isEnabled())
        return;

    queueitem_t signal(PATCH_PARAMETER_CHANGE_EDITOR, param, val);
    emit enqueue(signal);
}


// ******************************************
void ShruthiEditorMainWindow::dialChanged(int id, int val) {
// ******************************************
    queueitem_t signal(PATCH_PARAMETER_CHANGE_EDITOR, id, val);
    emit enqueue(signal);
}


// ******************************************
void ShruthiEditorMainWindow::patchNameChanged() {
// ******************************************
    QString name = ui->patch_name->text();
    queueitem_t signal(SET_PATCHNAME, name);
    emit enqueue(signal);
}


// ******************************************
void ShruthiEditorMainWindow::loadPatch() {
// ******************************************
    QString filename = QFileDialog::getOpenFileName(this, "Open patch", ".syx", "All possible files (*.syx *.sp);;Sysex-Files (*.syx);;Shruthi-Patches (*.sp)");
    if (filename != "") {
        queueitem_t signal(FILEIO_LOAD, filename, Editor::FLAG_PATCH);
        emit enqueue(signal);
    }
}


// ******************************************
void ShruthiEditorMainWindow::loadSequence() {
// ******************************************
    QString filename = QFileDialog::getOpenFileName(this, "Open patch", ".syx", "Sysex-Files (*.syx)");
    if (filename != "") {
        queueitem_t signal(FILEIO_LOAD, filename, Editor::FLAG_SEQUENCE);
        emit enqueue(signal);
    }
}


// ******************************************
void ShruthiEditorMainWindow::savePatch() {
// ******************************************
    QString filename = QFileDialog::getSaveFileName(this, "Save patch", ".syx", "Sysex-Files (*.syx);;Shruthi-Patches (*.sp)");
    if (filename != "") {
        queueitem_t signal(FILEIO_SAVE, filename, Editor::FLAG_PATCH);
        emit enqueue(signal);
    }
}


// ******************************************
void ShruthiEditorMainWindow::saveSequence() {
// ******************************************
    QString filename = QFileDialog::getSaveFileName(this, "Save patch", ".syx", "Sysex-Files (*.syx)");
    if (filename != "") {
        queueitem_t signal(FILEIO_SAVE, filename, Editor::FLAG_SEQUENCE);
        emit enqueue(signal);
    }
}


// ******************************************
void ShruthiEditorMainWindow::fetchPatch() {
// ******************************************
    queueitem_t signal(SYSEX_FETCH_REQUEST, Editor::FLAG_PATCH);
    emit enqueue(signal);
}


// ******************************************
void ShruthiEditorMainWindow::sendPatch() {
// ******************************************
    queueitem_t signal(SYSEX_SEND_DATA, Editor::FLAG_PATCH);
    emit enqueue(signal);
}


// ******************************************
void ShruthiEditorMainWindow::fetchSequence() {
// ******************************************
    queueitem_t signal(SYSEX_FETCH_REQUEST, Editor::FLAG_SEQUENCE);
    emit enqueue(signal);
}


// ******************************************
void ShruthiEditorMainWindow::sendSequence() {
// ******************************************
    queueitem_t signal(SYSEX_SEND_DATA, Editor::FLAG_SEQUENCE);
    emit enqueue(signal);
}


// ******************************************
void ShruthiEditorMainWindow::openSettings() {
// ******************************************
    ShruthiEditorSettings prefs(this);
    prefs.setFixedSize(prefs.width(),prefs.height());
    prefs.setMidiPorts(MIDI_INPUT_PORT, MIDI_OUTPUT_PORT);
    prefs.setMidiChannel(MIDI_CHANNEL);
    prefs.setShruthiFilterBoard(SHRUTHI_FILTER_BOARD);
    prefs.setWindowIcon(QIcon(":/shruthi-editor.png"));
    if (prefs.exec()) {
        int in = prefs.getMidiInputPort();
        int out = prefs.getMidiOutputPort();
        unsigned char channel = prefs.getMidiChannel();
        int filterboard = prefs.getShruthiFilterBoard();
        emit settingsChanged(in, out, channel, filterboard);
    }
    prefs.done(1);
}


// ******************************************
void ShruthiEditorMainWindow::resetPatch() {
// ******************************************
    queueitem_t signal(RESET_PATCH);
    signal.int0 = 1000;
    emit enqueue(signal);
}


// ******************************************
void ShruthiEditorMainWindow::resetPatchPre100() {
// ******************************************
    queueitem_t signal(RESET_PATCH);
    signal.int0 = 98;
    emit enqueue(signal);
}


// ******************************************
void ShruthiEditorMainWindow::resetSequence() {
// ******************************************
    queueitem_t signal(RESET_SEQUENCE);
    emit enqueue(signal);
}


// ******************************************
void ShruthiEditorMainWindow::randomizePatch() {
// ******************************************
    queueitem_t signal(RANDOMIZE_PATCH);
    emit enqueue(signal);
}


// ******************************************
void ShruthiEditorMainWindow::quitShruthiEditor() {
// ******************************************
    QApplication::exit(0);
}


// ******************************************
void ShruthiEditorMainWindow::aboutShruthiEditor() {
// ******************************************
    QMessageBox::about(this,"About Shruthi-Editor",
        QString::fromUtf8("Shruti-Editor Version "VERSION".\n\n Copyright (C) 2011-2015 Manuel Krönig."));
}


// ******************************************
void ShruthiEditorMainWindow::aboutQt() {
// ******************************************
    QMessageBox::aboutQt(this);
}


// ******************************************
void ShruthiEditorMainWindow::closeEvent(QCloseEvent* event) {
// ******************************************
    Q_UNUSED(event);
    quitShruthiEditor();
}


// ******************************************
// ******************************************
// Redraw Signal Handlers
// ******************************************
// ******************************************

// ******************************************
void ShruthiEditorMainWindow::redrawPatchParameter(int id) {
// ******************************************
    const param_t param = Patch::parameter(id, SHRUTHI_FILTER_BOARD);

    QString wid = QString("c%1").arg(id);

    // Fix for additional dials (parameter 92/93):
    if ((id == 92 || id == 93) && param.dropdown == NULL) {
        wid.append("d");
    }

    // Deactivate element before setting value to prevent sending
    // the change back (i.e. debouncing)!
    if (param.dropdown) {
        QComboBox* temp = this->findChild<QComboBox*>(wid);
        if (!temp) {
            return;
        }
        const bool &wasEnabled = temp->isEnabled();
        temp->setEnabled(false);
        temp->setCurrentIndex(editor->getParam(id));
        temp->setEnabled(wasEnabled);
    } else if (Patch::belongsToModMatrix(id)) {
        QDial* temp = this->findChild<QDial*>(wid);
        if (!temp) {
            return;
        }
        const bool &wasEnabled = temp->isEnabled();
        temp->setEnabled(false);
        temp->setValue(editor->getParam(id));
        temp->setEnabled(wasEnabled);
    } else {
        ShruthiEditorDial* temp = this->findChild<ShruthiEditorDial*>(wid);
        if (!temp) {
            return;
        }
        temp->setValue(editor->getParam(id));
    }
}

// ******************************************
void ShruthiEditorMainWindow::redrawAllPatchParameters() {
// ******************************************
    for (int i=0; i<100; i++) {
        if (Patch::hasUI(i)) {
            redrawPatchParameter(i);
        }
    }
    ui->patch_name->setText(editor->getName());
}


// ******************************************
// ******************************************
// Statusbar
// ******************************************
// ******************************************

// ******************************************
void ShruthiEditorMainWindow::midiInputStatusChanged(bool st) {
// ******************************************
    MIDI_INPUT_STATUS=st;
    displayMidiStatusChanged(st, MIDI_OUTPUT_STATUS);
}


// ******************************************
void ShruthiEditorMainWindow::midiOutputStatusChanged(bool st) {
// ******************************************
    MIDI_OUTPUT_STATUS=st;
    displayMidiStatusChanged(MIDI_INPUT_STATUS,st);
}


// ******************************************
void ShruthiEditorMainWindow::displayMidiStatusChanged(const bool &in, const bool &out) {
// ******************************************
    QString status = "MidiIn: ";
    if (!in)
        status += "not ";
    status += "ready. MidiOut: ";
    if (!out)
        status += "not ";
    status += "ready.";
    statusBar()->showMessage(status);

    // It's rather hacky to do this here, but it works:
    if (in && out) {
        queueitem_t signal(SYSEX_VERSION_REQUEST);
        emit enqueue(signal);
    }

}


// ******************************************
void ShruthiEditorMainWindow::displayStatusbar(QString msg) {
// ******************************************
    statusBar()->showMessage(msg);
}


// ******************************************
void ShruthiEditorMainWindow::setStatusbarVersionLabel(QString text)
// ******************************************
{
    statusbarVersionLabel->setText(text + " ");
}

