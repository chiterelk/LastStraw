#include "login.h"
#include "ui_login.h"
#include <QApplication>

LogIn::LogIn(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::LogIn)
{
	ui->setupUi(this);
}

LogIn::~LogIn()
{
	delete ui;
}

void LogIn::on_pushButton_clicked()
{
	 QApplication::closeAllWindows();
}

void LogIn::on_pushButtonLogIn_clicked()
{
	 emit tookKeys(ui->lineEditApiKey->text().toLatin1(),ui->lineEditSecretKey->text().toLatin1());
}
