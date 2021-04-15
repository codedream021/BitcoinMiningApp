#include "SettingsDialog.h"

#include <QVBoxLayout>
#include <QSlider>
#include <QDialogButtonBox>

SettingsDialog::SettingsDialog(QWidget* parent) : QDialog(parent)
{
    setLayout(new QVBoxLayout());

    layout()->addWidget(loadSlider = new QSlider());
    loadSlider->setMaximum(loadSliderMax);
<<<<<<< HEAD
    loadSlider->setValue(1);
    loadSlider->setOrientation(Qt::Horizontal);
=======
>>>>>>> 51ea25f571f17f36eb4fb357672fc1b40b3a23e5

    QDialogButtonBox* buttonBox;
    layout()->addWidget(buttonBox = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel));
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

double SettingsDialog::getChosenLoadFraction() {
<<<<<<< HEAD
    return loadSlider->value() / loadSliderMax;
=======
    return loadSlider->position() / loadSliderMax;
>>>>>>> 51ea25f571f17f36eb4fb357672fc1b40b3a23e5
}
