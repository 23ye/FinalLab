#include "mainwindow.h"
#include "loginwindow.h" //引入登录窗口头文件

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LoginWindow login;
    if (login.exec() == QDialog::Accepted) {
        // 只有当登录成功 (返回 Accepted) 时，才创建主窗口
        MainWindow w;
        w.show();
        // 进入主程序的事件循环
        return a.exec();
    }
    // 如果用户直接关掉登录窗或点了退出，程序直接结束，返回 0
    return 0;
}
