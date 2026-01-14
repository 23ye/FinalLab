#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSplitter>
#include "databasemanager.h"
#include "addwindow.h"
#include "cryptoutil.h"  // [新增] 加密头文件
#include <QDateTime>     // [新增] 用于记录时间
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

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

void MainWindow::on_actionAdd_triggered()
{
    AddWindow dlg(this);

    // 设置弹窗标题
    dlg.setWindowTitle("新增账号信息");

    // 调用 exec() 显示弹窗
    // 如果用户点了"保存" (我们在 DialogAdd 里写了 accept())，它返回 QDialog::Accepted
    // 如果用户点了"取消" 或右上角叉号，它返回 QDialog::Rejected
    int result = dlg.exec();

    // === 判断用户操作 ===
    if (result == QDialog::Accepted) {
        // 用户点击了保存，这里开始写数据库插入逻辑
        saveAccountToDb(dlg);
    } else {
        // 用户点击了取消，什么都不用做
        qDebug() << "用户取消了添加";
    }
}

void MainWindow::saveAccountToDb(AddWindow &dlg)
{
    // 1. 获取输入
    QString category = dlg.getCategory();
    QString site = dlg.getSite();
    QString user = dlg.getUser();
    QString rawPass = dlg.getPassword();
    QString note = dlg.getNote();

    // 2. 加密密码 (实现 CryptoUtil)
    // 如果还没实现加密，暂时用 rawPass 代替
    QString encPass = CryptoUtil::encrypt(rawPass);

    // 3. 执行 SQL
    QSqlQuery query;
    query.prepare("INSERT INTO accounts (category, site, username, enc_password, note, created_at) "
                  "VALUES (:cat, :site, :user, :pass, :note, :time)");

    query.bindValue(":cat", category);
    query.bindValue(":site", site);
    query.bindValue(":user", user);
    query.bindValue(":pass", encPass);
    query.bindValue(":note", note);
    query.bindValue(":time", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm"));

    if (query.exec()) {
        QMessageBox::information(this, "成功", "账号添加成功！");

        // 刷新表格显示
        // 重新查询数据库，让新数据立即出现在界面上
        if (m_model) {
            m_model->select();
        }
    } else {
        QMessageBox::critical(this, "错误", "数据库写入失败：" + query.lastError().text());
    }
}

