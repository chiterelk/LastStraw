#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QLabel>
#include <QGridLayout>
#include <QMessageBox>
#include <QDateTime>
#include <QProgressBar>

#include "jlaststraw.h"
#include "login.h"
#include "JBittrex/jbalance.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();


public slots:
	void showPing(int _ping);
	void logined();
	void showWallet(QList <JBalance*> _wallet);
	void chengedShowZero(int);
	void started(int _countMarkets);
	void chengeCountWorkMarket(QString _market);
	void marketNotFound();
	void selectMarketForBTC(QString _market);
	void selectMarketForETH(QString _market);
	void selectMarketForUSDT(QString _market);

	void processBTC(int _process);
	void processETH(int _process);
	void processUSDT(int _process);


private slots:

	void on_pushButtonUpdateWallet_clicked();

	void on_pushButtonStart_clicked();

private:
	Ui::MainWindow *ui;
	JLastStraw *LastStraw = new JLastStraw(this);
	QLabel *ping = new QLabel("ping: 0ms",this,0);
	LogIn *logIn = new LogIn(this);
	QList <QLabel*> labelCurrensy;
	QList <QLabel*> labelBalance;
	QList <QLabel*> labelAvailable;
	QMessageBox * selectCurrency = new QMessageBox("Select currence!","<b>Select currence!</b>",QMessageBox::Information,QMessageBox::NoButton,QMessageBox::NoButton,QMessageBox::Ok,this,Qt::Dialog);
	QList <QProgressBar*> progress;
};

#endif // MAINWINDOW_H
