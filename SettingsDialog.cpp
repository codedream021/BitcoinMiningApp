#include "SettingsDialog.h"

#include <QVBoxLayout>
#include <QSlider>
#include <QLabel>
#include <QDialogButtonBox>
#include <QCheckBox>

SettingsDialog::SettingsDialog(QWidget* parent, double loadFraction, bool autoStart) : QDialog(parent)
{
    setLayout(new QVBoxLayout());

    QWidget* widgetLoadFraction;
    layout()->addWidget(widgetLoadFraction = new QWidget());
    widgetLoadFraction->setLayout(new QHBoxLayout());

    widgetLoadFraction->layout()->addWidget(new QLabel("Load fraction"));
    widgetLoadFraction->layout()->addWidget(loadSlider = new QSlider());
    loadSlider->setMaximum(loadSliderMax);
    loadSlider->setValue(loadFraction * loadSliderMax);
    loadSlider->setOrientation(Qt::Horizontal);

    layout()->addWidget(autoStartCheckBox = new QCheckBox("Autorun on startup"));
    autoStartCheckBox->setCheckState(autoStart ? Qt::Checked : Qt::Unchecked);

    QDialogButtonBox* buttonBox;
    layout()->addWidget(buttonBox = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel));
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

double SettingsDialog::loadFraction() {
    return loadSlider->value() / loadSliderMax;
}

bool SettingsDialog::autoStart() {
    return autoStartCheckBox->checkState() == Qt::Checked;
}
