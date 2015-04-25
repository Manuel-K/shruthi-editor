#ifndef KEYBOARD_WIDGET_H
#define KEYBOARD_WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <vector>


// ******************************************
class KeyboardWidget : public QWidget {
// ******************************************
    Q_OBJECT

    public:
        KeyboardWidget(QWidget *parent = 0, unsigned int numberOfKeys = 61);
        ~KeyboardWidget();

        void setNumberOfKeys(const unsigned int &value);
        unsigned int getNumberOfKeys() const;

        bool getAutomaticNumberOfKeys() const;
        void setAutomaticNumberOfKeys(bool value);

        double getKeySizeRatio() const;
        void setKeySizeRatio(double value);

        double getBlackKeyHeightFactor() const;
        void setBlackKeyHeightFactor(double value);

        int getMinimalKeyWidth() const;
        void setMinimalKeyWidth(int value);

        int getBaseNote() const;
        void setBaseNote(int value);

        int getOctave() const;
        void setOctave(int value);

    private:
        void resizeEvent(QResizeEvent * event);
        static bool isBlackKey(int i);
        static int numWhiteKeys(int i);
        void setKeySize();
        void raiseKeys();
        void setKeyVisibility();
        void setupKeys();
        void destroyKeys();

        std::vector <QPushButton*> keys;
        unsigned int visibleNumberOfKeys;

        unsigned int numberOfKeys;
        bool automaticNumberOfKeys;
        double keySizeRatio; // for 25 keys
        double blackKeyHeightFactor;
        int minimalKeyWidth;

        int baseNote;
        int octave;

    private slots:
        void keyPressed();
        void keyReleased();

    signals:
        void keyPressed(int);
        void keyReleased(int);
};

#endif // KEYBOARD_WIDGET_H
