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
        initTableView();
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
    //创建模型，使用全局数据库连接
    m_model = new QSqlTableModel(this, DatabaseManager::instance().getDb());

    //绑定表名
    m_model->setTable("accounts");

    //设置编辑策略：修改后立即更新到数据库
    m_model->setEditStrategy(QSqlTableModel::OnFieldChange);

    //执行查询，把数据拉取下来
    m_model->select();

    //设置表头别名 (让界面显示中文，而不是数据库字段名)
    m_model->setHeaderData(0, Qt::Horizontal, "ID");
    m_model->setHeaderData(1, Qt::Horizontal, "类别");
    m_model->setHeaderData(2, Qt::Horizontal, "网站/标题");
    m_model->setHeaderData(3, Qt::Horizontal, "用户名");
    m_model->setHeaderData(4, Qt::Horizontal, "密码(已加密)");
    m_model->setHeaderData(5, Qt::Horizontal, "备注");
    m_model->setHeaderData(6, Qt::Horizontal, "创建时间");

    //将模型绑定到 UI 上的视图
    ui->tableViewAccounts->setModel(m_model);

    // 细节优化：隐藏 ID 列
    // 必须在 setModel 之后调用 setColumnHidden
    ui->tableViewAccounts->setColumnHidden(0, true);

    //让列宽自动铺满
    ui->tableViewAccounts->horizontalHeader()->setStretchLastSection(true);
}
