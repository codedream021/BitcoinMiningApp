#pragma once

#include <QtWidgets/QDialog>

class QPushButton;
class QSlider;

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget* parent, double loadFraction);
    double loadFraction(); 
private slots:
private:  
    size_t loadSliderMax = 16 * 1024 * 1024;
    QSlider* loadSlider;
}; 