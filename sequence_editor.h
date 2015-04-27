#ifndef SEQUENCEEDITOR_H
#define SEQUENCEEDITOR_H

#include <QDialog>
#include <lib_editor.h>

namespace Ui {
class SequenceEditor;
}

// ******************************************
class SequenceEditor : public QDialog
// ******************************************
{
        Q_OBJECT

    public:
        explicit SequenceEditor(Editor *edit, QWidget *parent = 0);
        ~SequenceEditor();

    private:
        Ui::SequenceEditor *ui;
        Editor *editor;

        void sendSequenceUpdate();

    public slots:
        void showSequenceEditor();
        void redrawPatchParameter(int id);
        void redrawAllPatchParameters();
        void redrawAllSequenceParameters();

    private slots:
        void activeChanged(int step, int value);
        void noteChanged(int step, int value);
        void valueChanged(int step, int value);
        void tieChanged(int step, int value);
        void velocityChanged(int step, int velocity);

        void comboBoxChanged(int value);
        void dialChanged(int id, int value);

    signals:
        void enqueue(queueitem_t);
};

#endif // SEQUENCEEDITOR_H
