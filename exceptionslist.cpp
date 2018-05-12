#include "exceptionslist.h"
#include "ui_exceptionslist.h"
#include <QDebug>

ExceptionsList::ExceptionsList(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::ExceptionsList)
{
	ui->setupUi(this);
}

ExceptionsList::~ExceptionsList()
{
	delete ui;
}

void ExceptionsList::on_pushButtonAdd_clicked()
{
	//ui->textEditExceptionList->;



	ui->textEditExceptionList->append(ui->lineEditExceptionsPair->text().toLatin1());
	//listExceptions = QString(ui->textEditExceptionList->toPlainText()).split('\n');
	//qDebug()<<listExceptions;
}

void ExceptionsList::on_pushButtonClear_clicked()
{
	ui->textEditExceptionList->clear();
}
