#ifndef EXCEPTIONSLIST_H
#define EXCEPTIONSLIST_H

#include <QMainWindow>
#include <QStringList>

namespace Ui {
class ExceptionsList;
}

class ExceptionsList : public QMainWindow
{
	Q_OBJECT

public:
	explicit ExceptionsList(QWidget *parent = 0);
	~ExceptionsList();
	QStringList listExceptions;
private slots:
	void on_pushButtonAdd_clicked();

	void on_pushButtonClear_clicked();

private:
	Ui::ExceptionsList *ui;
};

#endif // EXCEPTIONSLIST_H
