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
#include "editor.h"
#include "ui/main_window.h"
#include "ui_main_window.h"
#include "ui/settings_dialog.h"
#include "labels.h"
#include "patch.h"
#include <iostream>

#define VERSION "0.23"

// ******************************************
ShruthiEditorMainWindow::ShruthiEditorMainWindow(const Editor *edit, QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    editor(edit) {
// ******************************************
    ui->setupUi(this);

    MIDI_CHANNEL = 1;
    MIDI_INPUT_PORT = 0;
    MIDI_OUTPUT_PORT = 0;
    SHRUTHI_FILTER_BOARD = 0;
    MIDI_INPUT_STATUS = false;
    MIDI_OUTPUT_STATUS = false;

    lastProgramFileMode = 2;

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
    connect(ui->actionLoadProgram, SIGNAL(triggered()), this, SLOT(loadProgram()));
    connect(ui->actionSaveProgram, SIGNAL(triggered()), this, SLOT(saveProgram()));
    connect(ui->actionFetchProgram, SIGNAL(triggered()), this, SLOT(fetchProgram()));
    connect(ui->actionSendProgram, SIGNAL(triggered()), this, SLOT(sendProgram()));
    connect(ui->actionFetchPatch, SIGNAL(triggered()), this, SLOT(fetchPatch()));
    connect(ui->actionSendPatch, SIGNAL(triggered()), this, SLOT(sendPatch()));
    connect(ui->actionFetchSequence, SIGNAL(triggered()), this, SLOT(fetchSequence()));
    connect(ui->actionSendSequence, SIGNAL(triggered()), this, SLOT(sendSequence()));
    connect(ui->actionOpenSettings, SIGNAL(triggered()), this, SLOT(openSettings()));
    connect(ui->actionResetPatch, SIGNAL(triggered()), this, SLOT(resetPatch()));
    connect(ui->actionResetPatchPre100, SIGNAL(triggered()), this, SLOT(resetPatchPre100()));
    connect(ui->actionQuit,SIGNAL(triggered()), this, SLOT(quitShruthiEditor()));
    connect(ui->actionAboutShruthiEditor, SIGNAL(triggered()), this, SLOT(aboutShruthiEditor()));
    connect(ui->actionAboutQt, SIGNAL(triggered()), this, SLOT(aboutQt()));
    connect(ui->actionRandomizePatch, SIGNAL(triggered()), this, SLOT(randomizePatch()));
    connect(ui->actionKeyboard, SIGNAL(triggered()), this, SIGNAL(showKeyboard()));
    connect(ui->actionOpenSequenceEditor, SIGNAL(triggered()), this, SIGNAL(showSequenceEditor()));
    connect(ui->actionOpenLibrary, SIGNAL(triggered()), this, SIGNAL(showLibrary()));
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
    const QStringList *p92combo = NULL;
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
    const QStringList *p93combo = NULL;
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

    QueueItem signal(QueueAction::PATCH_PARAMETER_CHANGE_EDITOR, id.toInt(), val);
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

    QueueItem signal(QueueAction::PATCH_PARAMETER_CHANGE_EDITOR, param, val);
    emit enqueue(signal);
}


// ******************************************
void ShruthiEditorMainWindow::dialChanged(int id, int val) {
// ******************************************
    QueueItem signal(QueueAction::PATCH_PARAMETER_CHANGE_EDITOR, id, val);
    emit enqueue(signal);
}


// ******************************************
void ShruthiEditorMainWindow::patchNameChanged() {
// ******************************************
    QString name = ui->patch_name->text();
    QueueItem signal(QueueAction::SET_PATCHNAME, name);
    emit enqueue(signal);
}


// ******************************************
void ShruthiEditorMainWindow::loadProgram() {
// ******************************************
    QFileDialog *d = new QFileDialog(this);
    d->setWindowTitle("Open program");
    d->setNameFilter("All possible files (*.syx *.sp);;SysEx files (*.syx);;Shruthi patches (*.sp)");
    d->setAcceptMode(QFileDialog::AcceptOpen);
    d->setFileMode(QFileDialog::ExistingFile);

    QComboBox *cb = injectQFileDialog(d);

    int success = d->exec();

    QStringList filenames = d->selectedFiles();
    const int &index = cb->currentIndex();
    int flag = cb->currentIndex() + 1; // combo box and flags have to be kept in sync!


    delete d;
    d = NULL;

    if (success && filenames.size() > 0) {
        lastProgramFileMode = index;
        if (filenames.first().endsWith(".sp", Qt::CaseInsensitive)) {
            flag = Editor::FLAG_PATCH;
        }
        QueueItem signal(QueueAction::FILEIO_LOAD, filenames.first(), flag);
        emit enqueue(signal);
    }
}


// ******************************************
QComboBox *ShruthiEditorMainWindow::injectQFileDialog(QFileDialog *d) {
// ******************************************
    QComboBox *cb = new QComboBox(d);
    cb->addItem("Patch");
    cb->addItem("Sequence");
    cb->addItem("Patch/Sequence");
    cb->setCurrentIndex(lastProgramFileMode);

    // Insert type combo box into layout:

    QGridLayout *gl = d->findChild<QGridLayout*>();

    if (!gl) {
        qDebug() << "Could not find QGridLayout of the QFileDialog! Type file dialog will look ugly!";
    } else {
        QLabel *l = new QLabel(d);
        l->setText("Type:");

        const int &row = gl->rowCount();
        gl->addWidget(l, row, 0);
        gl->addWidget(cb, row, 1);
    }
    return cb;
}


// ******************************************
void ShruthiEditorMainWindow::saveProgram() {
// ******************************************
    QFileDialog *d = new QFileDialog(this);
    d->setWindowTitle("Save program");
    //d->setNameFilter("SysEx files (*.syx);;Shruthi patches (*.sp);;Shruthi patches (*.sp)");
    d->setNameFilter("SysEx files (*.syx)");
    d->setAcceptMode(QFileDialog::AcceptSave);
    d->setFileMode(QFileDialog::AnyFile);
    d->setDefaultSuffix("syx");

    QComboBox *cb = injectQFileDialog(d);

    int success = d->exec();

    QStringList filenames = d->selectedFiles();
    const int &index = cb->currentIndex();
    int flag = cb->currentIndex() + 1; // combo box and flags have to be kept in sync!


    delete d;
    d = NULL;

    if (success && filenames.size() > 0) {
        lastProgramFileMode = index;
        std::cout << filenames.at(0).toUtf8().constData() << " " << flag << std::endl;
        if (filenames.first().endsWith(".sp", Qt::CaseInsensitive)) {
            flag = Editor::FLAG_PATCH;
        }
        QueueItem signal(QueueAction::FILEIO_SAVE, filenames.first(), flag);
        emit enqueue(signal);
    }
}


// ******************************************
void ShruthiEditorMainWindow::fetchProgram() {
// ******************************************
    QueueItem signal(QueueAction::SYSEX_FETCH_REQUEST, Editor::FLAG_PATCH | Editor::FLAG_SEQUENCE);
    emit enqueue(signal);
}


// ******************************************
void ShruthiEditorMainWindow::sendProgram() {
// ******************************************
    QueueItem signal(QueueAction::SYSEX_SEND_DATA, Editor::FLAG_PATCH | Editor::FLAG_SEQUENCE);
    emit enqueue(signal);
}


// ******************************************
void ShruthiEditorMainWindow::fetchPatch() {
// ******************************************
    QueueItem signal(QueueAction::SYSEX_FETCH_REQUEST, Editor::FLAG_PATCH);
    emit enqueue(signal);
}


// ******************************************
void ShruthiEditorMainWindow::sendPatch() {
// ******************************************
    QueueItem signal(QueueAction::SYSEX_SEND_DATA, Editor::FLAG_PATCH);
    emit enqueue(signal);
}


// ******************************************
void ShruthiEditorMainWindow::fetchSequence() {
// ******************************************
    QueueItem signal(QueueAction::SYSEX_FETCH_REQUEST, Editor::FLAG_SEQUENCE);
    emit enqueue(signal);
}


// ******************************************
void ShruthiEditorMainWindow::sendSequence() {
// ******************************************
    QueueItem signal(QueueAction::SYSEX_SEND_DATA, Editor::FLAG_SEQUENCE);
    emit enqueue(signal);
}


// ******************************************
void ShruthiEditorMainWindow::openSettings() {
// ******************************************
    SettingsDialog prefs(this);
    prefs.setFixedSize(prefs.width(),prefs.height());
    prefs.setMidiPorts(MIDI_INPUT_PORT, MIDI_OUTPUT_PORT);
    prefs.setMidiChannel(MIDI_CHANNEL);
    prefs.setShruthiFilterBoard(SHRUTHI_FILTER_BOARD);
    prefs.setWindowIcon(QIcon(":/shruthi_editor.png"));
    if (prefs.exec()) {
        Config conf;
        conf.setMidiInputPort(prefs.getMidiInputPort());
        conf.setMidiOutputPort(prefs.getMidiOutputPort());
        conf.setMidiChannel(prefs.getMidiChannel());
        conf.setShruthiFilterBoard(prefs.getShruthiFilterBoard());
        emit settingsChanged(conf);
    }
    prefs.done(1);
}


// ******************************************
void ShruthiEditorMainWindow::resetPatch() {
// ******************************************
    QueueItem signal(QueueAction::RESET_PATCH);
    signal.int0 = 1000;
    emit enqueue(signal);
}


// ******************************************
void ShruthiEditorMainWindow::resetPatchPre100() {
// ******************************************
    QueueItem signal(QueueAction::RESET_PATCH);
    signal.int0 = 98;
    emit enqueue(signal);
}


// ******************************************
void ShruthiEditorMainWindow::resetSequence() {
// ******************************************
    QueueItem signal(QueueAction::RESET_SEQUENCE);
    emit enqueue(signal);
}


// ******************************************
void ShruthiEditorMainWindow::randomizePatch() {
// ******************************************
    QueueItem signal(QueueAction::RANDOMIZE_PATCH);
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
        QueueItem signal(QueueAction::SYSEX_SHRUTHI_INFO_REQUEST);
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

