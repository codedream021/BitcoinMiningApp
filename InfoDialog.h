#pragma once

#include <QtWidgets/QDialog>

class QPushButton;

class InfoDialog : public QDialog
{
    Q_OBJECT
public:
    explicit InfoDialog(QWidget* parent);
private slots:
    void onShowOpenSSLLicense();
    void onShowCurlLicense();
    void onShowMinGWLicense();
    void onShowQtLicense();
    void onShowXMRStakLicense();
private:
    QPushButton* addLicenseLink(const QString& name);
};

class LicenseDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LicenseDialog(const QString& library, QWidget* parent);
private:
}; 
