#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class FileDownloader : public QObject
{
	Q_OBJECT
public:
	explicit FileDownloader(QUrl imageUrl, QObject* parent = 0);
	virtual ~FileDownloader();
	QByteArray downloadedData() const;

signals:
	void downloaded();
	void error(std::string error);

private slots:
	void fileDownloaded(QNetworkReply* pReply);
	void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);

private:
	QNetworkAccessManager m_WebCtrl;
	QByteArray m_DownloadedData;
};

#endif // FILEDOWNLOADER_H