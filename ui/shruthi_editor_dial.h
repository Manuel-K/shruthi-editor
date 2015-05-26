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


#ifndef SHRUTHI_EDITOR_DIAL_H
#define SHRUTHI_EDITOR_DIAL_H

#include <QWidget>

namespace Ui {
class ShruthiEditorDial;
}


class ShruthiEditorDial : public QWidget {
        Q_OBJECT

    public:
        explicit ShruthiEditorDial(QWidget *parent = 0);
        ~ShruthiEditorDial();

        void setParameter(const int &parameter_);
        void setName(const QString &name);
        void setLimits(const int &min, const int &max);
        void setFormatter(QString (*formatter_)(int));

        void setValue(const int &value);


    private:
        ShruthiEditorDial(const ShruthiEditorDial&); //forbid copying
        ShruthiEditorDial &operator=(const ShruthiEditorDial&); //forbid assignment

        Ui::ShruthiEditorDial *ui;
        void setDisplay(const int &value);

        int parameter;
        bool sendValueChanges;
        QString (*formatter)(int);

    private slots:
        void valueChanged(int);

    signals:
        void valueChanged(int, int);
};

#endif // SHRUTHI_EDITOR_DIAL_H
