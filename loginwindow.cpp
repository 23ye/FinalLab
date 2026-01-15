#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("用户登录");
    // 设置固定大小，不让用户拖动窗口
    this->setFixedSize(this->width(), this->height());

    ui->lineUser->setText("admin");
    ui->linePass->setText("123456");
    ui->btnLogin->setFocus();
    ui->btnLogin->setDefault(true); // 设置为默认按钮（按回车触发点击）
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_btnLogin_clicked()
{
    QString user = ui->lineUser->text().trimmed(); // 去除首尾空格
    QString pass = ui->linePass->text();

    // 设定默认账号和密码
    const QString ADMIN_USER = "admin";
    const QString ADMIN_PASS = "123456";

    if (user.isEmpty() || pass.isEmpty()) {
        QMessageBox::warning(this, "提示", "账号或密码不能为空！");
        return;
    }

    if (user == ADMIN_USER && pass == ADMIN_PASS) {
        // 验证通过，accept() 会关闭窗口，并返回 Accepted 状态给 main.cpp
        accept();
    } else {
        QMessageBox::critical(this, "错误", "账号或密码错误，请重试。");
        ui->linePass->clear();
        ui->linePass->setFocus(); // 光标回到密码框方便重输
    }
}


void LoginWindow::on_btnExit_clicked()
{
    reject();
}

