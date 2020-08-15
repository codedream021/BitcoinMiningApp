#pragma once

#include <QtWidgets/QDialog>

class QPushButton;

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget* parent);
    double getChosenLoadFraction();
private slots:
private: 
    size_t loadSliderMax = 16 * 1024 * 1024;
    QSlider* loadSlider;
}; 