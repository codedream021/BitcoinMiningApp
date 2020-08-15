#include "SettingsDialog.h"

#include <QVBoxLayout>
#include <QSlider>
#include <QDialogButtonBox>

SettingsDialog::SettingsDialog(QWidget* parent) : QDialog(parent)
{
    setLayout(new QVBoxLayout());

    layout()->addWidget(loadSlider = new QSlider());
    loadSlider->setMaximum(loadSliderMax);

    QDialogButtonBox* buttonBox;
    layout()->addWidget(buttonBox = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel));
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

double SettingsDialog::getChosenLoadFraction() {
    return loadSlider->position() / loadSliderMax;
}
