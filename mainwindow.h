#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include "addwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initSideBar();//初始化选项函数

private slots:
    void on_actionAdd_triggered();

private:
    Ui::MainWindow *ui;
    QSqlTableModel *m_model;//声明数据模型指针
    void initTableView();//初始化表格函数
    void saveAccountToDb(AddWindow &dlg);

};
#endif // MAINWINDOW_H
