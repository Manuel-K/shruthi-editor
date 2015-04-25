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

#define VERSION "0.21"

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
            } else {
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
            }
        }
    }
    // Setup additional dials for parameters 92 and 93:
    for (int i = 92; i <= 93; i++) {
        if (Patch::hasUI(i)) {
                tmp_d = this->findChild<QDial*>(QString("c%1d").arg(i));
                if (!tmp_d) {
                    qDebug() << "Dial" << QString("c%1d").arg(i) << "could not be found!";
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
                connect(tmp_d, SIGNAL(valueChanged(int)), this, SLOT(dialChanged(int)));
        }
    }

    connect(ui->patch_name, SIGNAL(editingFinished()), this, SLOT(patchNameChanged()));

    // Now that everything is set up, update all UI elements:
    redrawAll();

    // other UI Signals:
    connect(ui->actionLoad_Patch, SIGNAL(triggered()), this, SLOT(loadPatch()));
    connect(ui->actionSave_Patch, SIGNAL(triggered()), this, SLOT(savePatch()));
    connect(ui->actionFetch_Patch, SIGNAL(triggered()), this, SLOT(fetchPatch()));
    connect(ui->actionSend_Patch, SIGNAL(triggered()), this, SLOT(sendPatch()));
    connect(ui->actionOpenSettings, SIGNAL(triggered()), this, SLOT(openSettings()));
    connect(ui->actionReset_Patch, SIGNAL(triggered()), this, SLOT(resetPatch()));
    connect(ui->actionReset_Patch_pre_1_00, SIGNAL(triggered()), this, SLOT(resetPatchPre100()));
    connect(ui->actionQuit,SIGNAL(triggered()), this, SLOT(quitShruthiEditor()));
    connect(ui->actionAbout_Shruthi_Editor, SIGNAL(triggered()), this, SLOT(aboutShruthiEditor()));
    connect(ui->actionAbout_Qt, SIGNAL(triggered()), this, SLOT(aboutQt()));
    connect(ui->actionRandomize_Patch, SIGNAL(triggered()), this, SLOT(randomizePatch()));
    connect(ui->actionKeyboard, SIGNAL(triggered()), this, SIGNAL(showKeyboard()));
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
    ui->c84->setEnabled(false);
    ui->c85->setEnabled(false);
    ui->c92d->setEnabled(false);
    ui->c92->setEnabled(false);
    ui->c93d->setEnabled(false);
    ui->c93->setEnabled(false);

    // Parameter 84:
    bool p84dial = false;
    const param_t p84 = Patch::parameter(84, filter);
    if (p84.name != NULL) {
        ui->l84->setText(p84.name + ":");
        if (p84.dropdown == NULL) {
            p84dial = true;
            ui->c84->setMinimum(p84.min);
            ui->c84->setMaximum(p84.max);
        }
    }

    // dial visibility
    ui->w84->setHidden(!p84dial);


    // Parameter 85:
    bool p85dial = false;
    const param_t p85 = Patch::parameter(85, filter);
    if (p85.name != NULL) {
        ui->l85->setText(p85.name + ":");
        if (p85.dropdown == NULL) {
            p85dial = true;
            ui->c85->setMinimum(p85.min);
            ui->c85->setMaximum(p85.max);
        }
    }

    // dial visibility
    ui->w85->setHidden(!p85dial);


    // Parameter 92:
    QStringList *p92combo = NULL;
    bool p92dial = false;

    const param_t p92 = Patch::parameter(92, filter);
    if (p92.name != NULL) {
        if (p92.dropdown == NULL) {
            ui->l92d->setText(p92.name + ":");
            p92dial = true;
            ui->c92d->setMinimum(p92.min);
            ui->c92d->setMaximum(p92.max);
        } else {
            ui->l92->setText(p92.name + ":");
            p92combo = p92.dropdown;
        }
    }

    // dial visibility
    ui->w92->setHidden(!p92dial);

    // combo box visibility
    bool p92hidden = true;
    if (p92combo) {
        ui->c92->clear();
        ui->c92->addItems(*p92combo);
        // need to set index?
        p92hidden = false;
    }

    ui->w92cb->setHidden(p92hidden);


    // Parameter 93:
    QStringList *p93combo = NULL;
    bool p93dial = false;

    const param_t p93 = Patch::parameter(93, filter);
    if (p93.name != NULL) {
        if (p93.dropdown == NULL) {
            ui->l93d->setText(p93.name + ":");
            p93dial = true;
            ui->c93d->setMinimum(p93.min);
            ui->c93d->setMaximum(p93.max);
        } else {
            ui->l93->setText(p93.name + ":");
            p93combo = p93.dropdown;
        }
    }

    // dial visibility
    ui->w93->setHidden(!p93dial);

    // combo box visibility
    bool p93hidden = true;
    if (p93combo) {
        ui->c93->clear();
        ui->c93->addItems(*p93combo);
        // need to set index?
        p93hidden = false;
    }
    ui->w93cb->setHidden(p93hidden);


    // change p92/p93 page:
    // page 0 contains the dials, page 1 the comboboxes
    ui->sw9293->setCurrentIndex(!p92hidden || !p93hidden);


    // Force display update:
    redrawNRPN(84);
    redrawNRPN(85);
    redrawNRPN(92);
    redrawNRPN(93);


    // Enable active widgets:
    ui->c84->setEnabled(p84dial);
    ui->c85->setEnabled(p85dial);
    ui->c92d->setEnabled(p92dial);
    ui->c92->setEnabled(!p92hidden);
    ui->c93d->setEnabled(p93dial);
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
    emit(enqueue(signal));
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
    emit(enqueue(signal));
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
    QString filename = QFileDialog::getOpenFileName(this, "Open patch", ".sp", "All possible files (*.sp *.syx);;Shruthi-Patches (*.sp);; Sysex-Files (*.syx)");
    if (filename!="") {
        queueitem_t signal(FILEIO_LOAD,filename);
        emit(enqueue(signal));
    }
}


// ******************************************
void ShruthiEditorMainWindow::savePatch() {
// ******************************************
    QString filename = QFileDialog::getSaveFileName(this, "Save patch", ".sp", "Shruthi-Patches (*.sp);; Sysex-Files (*.syx)");
    if (filename!="") {
        queueitem_t signal(FILEIO_SAVE,filename);
        emit(enqueue(signal));
    }
}


// ******************************************
void ShruthiEditorMainWindow::fetchPatch() {
// ******************************************
    queueitem_t signal(SYSEX_FETCH_PATCH);
    emit(enqueue(signal));
}


// ******************************************
void ShruthiEditorMainWindow::sendPatch() {
// ******************************************
    queueitem_t signal(SYSEX_SEND_PATCH);
    emit(enqueue(signal));
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
    emit(enqueue(signal));
}


// ******************************************
void ShruthiEditorMainWindow::resetPatchPre100() {
// ******************************************
    queueitem_t signal(RESET_PATCH);
    signal.int0 = 98;
    emit(enqueue(signal));
}


// ******************************************
void ShruthiEditorMainWindow::randomizePatch() {
// ******************************************
    queueitem_t signal(RANDOMIZE_PATCH);
    emit(enqueue(signal));
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
void ShruthiEditorMainWindow::redrawNRPN(int nrpn) {
// ******************************************
    const param_t param = Patch::parameter(nrpn, SHRUTHI_FILTER_BOARD);

    QString id = QString("c%1").arg(nrpn);

    bool forceDial = false;

    // Fix for additional dials (parameter 92/93):
    if ((nrpn == 92 || nrpn == 93) && param.dropdown == NULL) {
        id.append("d");
        forceDial = true;
    }

    // Deactivate element before setting value to prevent sending
    // the change back (i.e. debouncing)!
    if (!forceDial && param.dropdown) {
        QComboBox* temp = this->findChild<QComboBox*>(id);
        if (!temp) {
            qDebug() << "ComboBox" << id << "could not be found!";
            return;
        }
        bool wasEnabled = temp->isEnabled();
        if (wasEnabled)
            temp->setEnabled(false);
        temp->setCurrentIndex(editor->getParam(nrpn));
        if (wasEnabled)
            temp->setEnabled(true);
    } else {
        QDial* temp = this->findChild<QDial*>(id);
        if (!temp) {
            qDebug() << "Dial" << id << "could not be found!";
            return;
        }
        bool wasEnabled = temp->isEnabled();
        if (wasEnabled)
            temp->setEnabled(false);
        temp->setValue(editor->getParam(nrpn));
        if (wasEnabled)
            temp->setEnabled(true);
    }
}

// ******************************************
void ShruthiEditorMainWindow::redrawAll() {
// ******************************************
    for (int i=0; i<100; i++) {
        if (Patch::hasUI(i)) {
            redrawNRPN(i);
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
void ShruthiEditorMainWindow::displayMidiStatusChanged(bool in, bool out) {
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
        emit(enqueue(signal));
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

