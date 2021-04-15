#include "SettingsDialog.h"

#include <QVBoxLayout>
#include <QSlider>
#include <QLabel>
#include <QDialogButtonBox>

SettingsDialog::SettingsDialog(QWidget* parent, double loadFraction) : QDialog(parent)
{
    setLayout(new QVBoxLayout());

    layout()->addWidget(new QLabel("Load fraction"));
    layout()->addWidget(loadSlider = new QSlider());
    loadSlider->setMaximum(loadSliderMax);
    loadSlider->setValue(loadFraction * loadSliderMax);
    loadSlider->setOrientation(Qt::Horizontal);

    QDialogButtonBox* buttonBox;
    layout()->addWidget(buttonBox = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel));
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

double SettingsDialog::loadFraction() {
    return loadSlider->value() / loadSliderMax;
}
