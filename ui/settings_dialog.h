// Shruthi-Editor: An unofficial Editor for the Shruthi hardware synthesizer. For
// informations about the Shruthi, see <http://www.mutable-instruments.net/shruthi1>.
//
// Copyright (C) 2011-2019 Manuel Krönig
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


#ifndef SETTINGS_DIALOG_H
#define SETTINGS_DIALOG_H


#include <QDialog>
namespace Ui { class SettingsDialog; }


class SettingsDialog : public QDialog {
        Q_OBJECT

    public:
        SettingsDialog(QWidget *parent = 0);
        ~SettingsDialog();

        int getMidiInputPort();
        int getMidiOutputPort();
        void setMidiPorts(const int &in, const int &out);
        void setMidiChannel(const unsigned char &channel);
        unsigned char getMidiChannel();
        int getShruthiFilterBoard();
        void setShruthiFilterBoard(const int &index);

    private:
        SettingsDialog(const SettingsDialog&); //forbid copying
        SettingsDialog &operator=(const SettingsDialog&); //forbid assignment

        void getPortInfo();

        int input_port;
        int output_port;
        bool input_port_error;
        bool output_port_error;

        Ui::SettingsDialog *ui;
};


#endif // SETTINGS_DIALOG_H
