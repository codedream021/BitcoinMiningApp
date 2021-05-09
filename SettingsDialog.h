#pragma once

#include <QtWidgets/QDialog>

class QPushButton;
class QSlider;
class QCheckBox;

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget* parent, double loadFraction, bool autoStart);
    double loadFraction(); 
    bool autoStart();
private slots:
private:  
    double loadSliderMax = 1000000;
    QSlider* loadSlider;
    QCheckBox* autoStartCheckBox;
}; 