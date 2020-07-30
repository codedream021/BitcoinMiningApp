#include "InfoDialog.h"

#include <QLabel>
#include <QPushButton>
#include <QTextStream>
#include <QFile>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QPlainTextEdit>

InfoDialog::InfoDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Info");
    setLayout(new QVBoxLayout());

    layout()->addWidget(new QLabel("Licenses"));
 //   connect(addLicenseLink("MinGW"), SIGNAL(clicked(bool)), this, SLOT(onShowMinGWLicense()));
    connect(addLicenseLink("Qt framework"), SIGNAL(clicked(bool)), this, SLOT(onShowQtLicense()));
    connect(addLicenseLink("XMR Stak"), SIGNAL(clicked(bool)), this, SLOT(onShowXMRStakLicense()));
    //   connect(addLicenseLink("Curl"), SIGNAL(clicked(bool)), this, SLOT(onShowCurlLicense()));
 //   connect(addLicenseLink("OpenSSL"), SIGNAL(clicked(bool)), this, SLOT(onShowOpenSSLLicense()));

    QDialogButtonBox* buttonBox;
    layout()->addWidget(buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok));
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));

    double f = 0.7;
    resize(f * parent->width(), f * parent->height());
}

void InfoDialog::onShowCurlLicense() { LicenseDialog("Curl", this).exec(); }
void InfoDialog::onShowOpenSSLLicense() { LicenseDialog("OpenSSL", this).exec(); }
void InfoDialog::onShowMinGWLicense() { LicenseDialog("MinGW", this).exec(); }
void InfoDialog::onShowQtLicense() { LicenseDialog("Qt", this).exec(); }
void InfoDialog::onShowXMRStakLicense() { LicenseDialog("XMR Stak", this).exec(); }

QPushButton* InfoDialog::addLicenseLink(const QString& name) {
    QWidget* w; QLayout* l; QPushButton* b;
    layout()->addWidget(w = new QWidget());
    w->setLayout(l = new QHBoxLayout());
    l->addWidget(new QLabel(name));
    l->addWidget(b = new QPushButton("Show"));
    return b;
}

LicenseDialog::LicenseDialog(const QString& library, QWidget *parent) : QDialog(parent)
{
    setWindowTitle("License");
    setLayout(new QVBoxLayout());

    QPlainTextEdit* licenseTextEdit;
    QFile file(library + ".txt");
    QString license;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) license = (QTextStream(&file).readAll());
    layout()->addWidget(licenseTextEdit = new QPlainTextEdit(license));
    licenseTextEdit->setReadOnly(true);

    QDialogButtonBox* buttonBox;
    layout()->addWidget(buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok));
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));

    double f = 0.7;
    resize(f * parent->width(), f * parent->height());
}
