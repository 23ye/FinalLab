#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSplitter>
#include "databasemanager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initSideBar();
    // 测试数据库连接
    if (DatabaseManager::instance().openDb()) {
        ui->statusbar->showMessage("数据库连接成功！");
    } else {
        ui->statusbar->showMessage("数据库连接失败，请检查日志。");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initSideBar()
{
    // 清空现有项
    ui->listWidgetCategories->clear();

    // 添加固定分类
    QStringList categories = {
        "所有账号",
        "社交娱乐",
        "工作办公",
        "金融理财",
        "其他"
    };

    ui->listWidgetCategories->addItems(categories);

    // 默认选中第一项
    ui->listWidgetCategories->setCurrentRow(0);
}

void MainWindow::initTableView()
{

}
