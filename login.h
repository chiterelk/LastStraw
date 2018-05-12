#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>

namespace Ui {
class LogIn;
}

class LogIn : public QMainWindow
{
	Q_OBJECT

public:
	explicit LogIn(QWidget *parent = 0);
	~LogIn();

private slots:
	void on_pushButton_clicked();
	void on_pushButtonLogIn_clicked();

private:
	Ui::LogIn *ui;

signals:
	tookKeys(QString apiKey, QString secretKey);
};

#endif // LOGIN_H
