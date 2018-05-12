#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QString>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	logIn->show();
	ui->groupBoxStop->setHidden(1);
	ui->statusBar->addWidget(ping,0);
	connect(LastStraw,&JLastStraw::ping,this,&MainWindow::showPing);
	connect(logIn,&LogIn::tookKeys,LastStraw,&JLastStraw::getWalletLogIn);
	connect(LastStraw,&JLastStraw::logined,this,logined);
	connect(LastStraw,&JLastStraw::gotWallet,this,showWallet);
	connect(ui->checkBoxZeroBalance,&QCheckBox::stateChanged,this,&MainWindow::chengedShowZero);
	connect(LastStraw,&JLastStraw::started,this,started);
	connect(LastStraw,&JLastStraw::chengeCountWorkMarket,this,chengeCountWorkMarket);
	connect(LastStraw,&JLastStraw::marketNotFound,this,&MainWindow::marketNotFound);

	connect(LastStraw,&JLastStraw::processBTC,this,processBTC);
	connect(LastStraw,&JLastStraw::processETH,this,processETH);
	connect(LastStraw,&JLastStraw::processUSDT,this,processUSDT);

	ui->gridLayoutWallet->addWidget(new QLabel("Currency",this,0),0,0,1,1,Qt::AlignHCenter);
	ui->gridLayoutWallet->addWidget(new QLabel("Balance",this,0),0,1,1,1,Qt::AlignHCenter);
	ui->gridLayoutWallet->addWidget(new QLabel("Available",this,0),0,2,1,1,Qt::AlignHCenter);

	for(int i = 0;i<400;i++)
	{
		labelCurrensy << new QLabel(QString::number(i+1),nullptr,0);
		labelBalance << new QLabel("0",nullptr,0);
		labelAvailable << new QLabel("0",nullptr,0);
		labelCurrensy.at(i)->setHidden(1);
		labelBalance.at(i)->setHidden(1);
		labelAvailable.at(i)->setHidden(1);
		labelCurrensy.at(i)->setParent(ui->gridLayoutWallet->widget());
		labelBalance.at(i)->setParent(ui->gridLayoutWallet->widget());
		labelAvailable.at(i)->setParent(ui->gridLayoutWallet->widget());

		ui->gridLayoutWallet->addWidget(labelCurrensy.at(i),i+1,0,1,1,Qt::AlignHCenter);
		ui->gridLayoutWallet->addWidget(labelBalance.at(i),i+1,1,1,1,Qt::AlignHCenter);
		ui->gridLayoutWallet->addWidget(labelAvailable.at(i),i+1,2,1,1,Qt::AlignHCenter);
	}
	for(int i = 0;i<3;i++)
	{
		progress<<new QProgressBar(ui->gridLayoutProcess->widget());
		progress.at(i)->setValue(0);
		progress.at(i)->setMaximum(5);
		progress.at(i)->setFormat("");
		ui->gridLayoutProcess->addWidget(progress.at(i),i+1,1,1,1,Qt::AlignHCenter);
	}
	ui->gridLayoutProcess->addWidget(new QLabel("Process",ui->gridLayoutProcess->widget(),0),0,1,1,2,Qt::AlignLeft);
	ui->gridLayoutProcess->addWidget(new QLabel("Base currency",ui->gridLayoutProcess->widget(),0),0,0,1,1,Qt::AlignHCenter);
	ui->gridLayoutProcess->addWidget(new QLabel("BTC",ui->gridLayoutProcess->widget(),0),1,0,1,1,Qt::AlignHCenter);
	ui->gridLayoutProcess->addWidget(new QLabel("ETH",ui->gridLayoutProcess->widget(),0),2,0,1,1,Qt::AlignHCenter);
	ui->gridLayoutProcess->addWidget(new QLabel("USDT",ui->gridLayoutProcess->widget(),0),3,0,1,1,Qt::AlignHCenter);
	ui->gridLayoutProcess->addWidget(new QLabel("waiting",ui->gridLayoutProcess->widget(),0),1,2,1,1,Qt::AlignHCenter);
	ui->gridLayoutProcess->addWidget(new QLabel("waiting",ui->gridLayoutProcess->widget(),0),2,2,1,1,Qt::AlignHCenter);
	ui->gridLayoutProcess->addWidget(new QLabel("waiting",ui->gridLayoutProcess->widget(),0),3,2,1,1,Qt::AlignHCenter);

	connect(LastStraw,&JLastStraw::selectMarketForBTC,this,&MainWindow::selectMarketForBTC);
	connect(LastStraw,&JLastStraw::selectMarketForETH,this,&MainWindow::selectMarketForETH);
	connect(LastStraw,&JLastStraw::selectMarketForUSDT,this,&MainWindow::selectMarketForUSDT);

}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::showPing(int _ping)
{
	ping->setText("ping: " + QString::number(_ping));
}

void MainWindow::logined()
{
	logIn->close();
	show();
	ui->console->append(QDateTime::currentDateTime().toString("d.MM.yy hh:mm:ss: ")+"Успешная авторизация!");
}

void MainWindow::showWallet(QList<JBalance *> _wallet)
{

	for(int i = 0;i<400;i++)
	{
		labelCurrensy.at(i)->setHidden(1);
		labelBalance.at(i)->setHidden(1);
		labelAvailable.at(i)->setHidden(1);
	}
	if(!ui->checkBoxZeroBalance->isChecked())
	{
		for(int i = 0; i<_wallet.count();i++)
		{

			labelCurrensy.at(i)->setText(_wallet.at(i)->getCurrency());
			labelBalance.at(i)->setText(QString::number(_wallet.at(i)->getBalance()));
			labelAvailable.at(i)->setText(QString::number(_wallet.at(i)->getAvailable()));
			labelCurrensy.at(i)->setHidden(0);
			labelBalance.at(i)->setHidden(0);
			labelAvailable.at(i)->setHidden(0);
		}
	}else{
		int n = 0;
		for(int i = 0; i<_wallet.count();i++)
		{
			if(_wallet.at(i)->getBalance() != 0.0)
			{

				labelCurrensy.at(n)->setText(_wallet.at(i)->getCurrency());
				labelBalance.at(n)->setText(QString::number(_wallet.at(i)->getBalance()));
				labelAvailable.at(n)->setText(QString::number(_wallet.at(i)->getAvailable()));
				labelCurrensy.at(n)->setHidden(0);
				labelBalance.at(n)->setHidden(0);
				labelAvailable.at(n)->setHidden(0);
				n++;
			}

		}
	}

}

void MainWindow::chengedShowZero(int)
{
	LastStraw->getWallet();
}

void MainWindow::started(int _countMarkets)
{
	ui->groupBoxStart->setHidden(1);
	ui->groupBoxStop->setHidden(0);
	ui->console->append(QDateTime::currentDateTime().toString("d.MM.yy hh:mm:ss: ")+"Начало работы. Для работы взято "+ QString::number(_countMarkets)+ " пар.");
}

void MainWindow::chengeCountWorkMarket(QString _market)
{
	ui->console->append(QDateTime::currentDateTime().toString("d.MM.yy hh:mm:ss: ")+"Маленький баланс для торговли к "+_market+".");
}

void MainWindow::marketNotFound()
{
	ui->console->append(QDateTime::currentDateTime().toString("d.MM.yy hh:mm:ss: ")+"Все выбранные валюты имеют низкий доступный баланс. Виберите другую валюту или пополните баланс.");
}


void MainWindow::on_pushButtonUpdateWallet_clicked()
{
	LastStraw->getWallet();
}



void MainWindow::on_pushButtonStart_clicked()
{
	 if(ui->checkBoxUseBTC->isChecked()||ui->checkBoxUseETH->isChecked()||ui->checkBoxUseUSDT->isChecked())
	 {
		 LastStraw->firstStart(ui->checkBoxUseBTC->isChecked(),ui->checkBoxUseETH->isChecked(),ui->checkBoxUseUSDT->isChecked(),ui->spinBoxTimeout->value());
	 }else{
		 selectCurrency->setGeometry(this->geometry().x()+this->geometry().width()/2,this->geometry().y()+this->geometry().height()/2,400,400);
		 selectCurrency->exec();
	 }
}

void MainWindow::selectMarketForBTC(QString _market)
{
	ui->console->append(QDateTime::currentDateTime().toString("d.MM.yy hh:mm:ss: ")+"Выбрано пару " +_market+ " для торговли к BTC.");
}

void MainWindow::selectMarketForETH(QString _market)
{
	ui->console->append(QDateTime::currentDateTime().toString("d.MM.yy hh:mm:ss: ")+"Выбрано пару " +_market+ " для торговли к ETH.");
}

void MainWindow::selectMarketForUSDT(QString _market)
{
	ui->console->append(QDateTime::currentDateTime().toString("d.MM.yy hh:mm:ss: ")+"Выбрано пару " +_market+ " для торговли к USDT.");
}

void MainWindow::processBTC(int _process)
{
	if(_process==0)
	{

	}
	if(_process==1)
	{

	}
	if(_process==2)
	{
		ui->console->append(QDateTime::currentDateTime().toString("d.MM.yy hh:mm:ss: ")+"Ордер на покупку выставлен (BTC).");
	}
	if(_process==3)
	{

		ui->console->append(QDateTime::currentDateTime().toString("d.MM.yy hh:mm:ss: ")+"Ордер на покупку исполнен (BTC).");
	}
	if(_process==4)
	{
		ui->console->append(QDateTime::currentDateTime().toString("d.MM.yy hh:mm:ss: ")+"Ордер на продажу выставлен (BTC).");
	}
	if(_process==5)
	{

		ui->console->append(QDateTime::currentDateTime().toString("d.MM.yy hh:mm:ss: ")+"Ордер на продажу исполнен (BTC).");
	}
}

void MainWindow::processETH(int _process)
{
	if(_process==0)
	{

	}
	if(_process==1)
	{

	}
	if(_process==2)
	{
		ui->console->append(QDateTime::currentDateTime().toString("d.MM.yy hh:mm:ss: ")+"Ордер на покупку выставлен (ETH).");
	}
	if(_process==3)
	{

		ui->console->append(QDateTime::currentDateTime().toString("d.MM.yy hh:mm:ss: ")+"Ордер на покупку исполнен (ETH).");
	}
	if(_process==4)
	{
		ui->console->append(QDateTime::currentDateTime().toString("d.MM.yy hh:mm:ss: ")+"Ордер на продажу выставлен (ETH).");
	}
	if(_process==5)
	{

		ui->console->append(QDateTime::currentDateTime().toString("d.MM.yy hh:mm:ss: ")+"Ордер на продажу исполнен (ETH).");
	}
}

void MainWindow::processUSDT(int _process)
{
	if(_process==0)
	{

	}
	if(_process==1)
	{

	}
	if(_process==2)
	{
		ui->console->append(QDateTime::currentDateTime().toString("d.MM.yy hh:mm:ss: ")+"Ордер на покупку выставлен (USDT).");
	}
	if(_process==3)
	{

		ui->console->append(QDateTime::currentDateTime().toString("d.MM.yy hh:mm:ss: ")+"Ордер на покупку исполнен (USDT).");
	}
	if(_process==4)
	{
		ui->console->append(QDateTime::currentDateTime().toString("d.MM.yy hh:mm:ss: ")+"Ордер на продажу выставлен (USDT).");
	}
	if(_process==5)
	{

		ui->console->append(QDateTime::currentDateTime().toString("d.MM.yy hh:mm:ss: ")+"Ордер на продажу исполнен (USDT).");
	}
}
