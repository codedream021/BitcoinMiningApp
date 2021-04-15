#include "FileDownloader.h"

#include <iostream>

FileDownloader::FileDownloader(QUrl url, QObject* parent) : QObject(parent) {
	connect(&m_WebCtrl, SIGNAL(finished(QNetworkReply*)), this, SLOT(fileDownloaded(QNetworkReply*)));
	connect(&m_WebCtrl, SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError>)), this, SLOT(sslErrors(QNetworkReply*, const QList<QSslError>)));
	m_WebCtrl.get(QNetworkRequest(url));
}

FileDownloader::~FileDownloader() { }

void FileDownloader::fileDownloaded(QNetworkReply* pReply) {
	if (pReply->error() == 0) {
		m_DownloadedData = pReply->readAll(); 
	    emit downloaded();
	}
	pReply->deleteLater();	
}

void FileDownloader::sslErrors(QNetworkReply *reply, const QList<QSslError> &errors) {
	std::cout << "[SSL Errors]";
}

QByteArray FileDownloader::downloadedData() const {
	return m_DownloadedData;
}