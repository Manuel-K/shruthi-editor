#ifndef SEQUENCESTEP_H
#define SEQUENCESTEP_H

#include <QWidget>
#include <QFrame>

namespace Ui {
class SequenceStep;
}

// ******************************************
class SequenceStep : public QWidget
// ******************************************
{
    Q_OBJECT

public:
    explicit SequenceStep(QWidget *parent = 0);
    ~SequenceStep();

    void setNoteLabels(const QStringList &labels);
    void setOctaveLimits(const int &min, const int &max);
    void setStep(const int &value);
    void setValueLabels(const QStringList &labels);
    void setVelocityLimits(const int &min, const int &max);

    void setActive(const int &value);
    void setNote(const int &value);
    void setNoteOctave(const int &value);
    void setOctave(const int &value);
    void setTie(const int &value);
    void setValue(const int &value);
    void setVelocity(const int &value);


private:
    Ui::SequenceStep *ui;
    int step;
    bool sendActive, sendNote, sendTie, sendVelocity, sendValue;

    static const int noteBase = 12;


private slots:
    void activeChanged(int val);
    void noteChanged(int val);
    void octaveChanged(int val);
    void valueChanged(int val);
    void tieChanged(int val);
    void velocityChanged(int val);


signals:
    void activeChanged(int, int);
    void noteChanged(int, int);
    void valueChanged(int, int);
    void tieChanged(int, int);
    void velocityChanged(int, int);
};

#endif // SEQUENCESTEP_H
