#include "databasemanager.h"
#include <QDir>

DatabaseManager::DatabaseManager()
{
}

DatabaseManager::~DatabaseManager()
{
    closeDb();
}

DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager instance;
    return instance;
}

bool DatabaseManager::openDb()
{
    if (m_db.isOpen()) return true;

    m_db = QSqlDatabase::addDatabase("QSQLITE");


    // QString dbPath = QCoreApplication::applicationDirPath() + "/" + DB_NAME;

    QString dbPath = "D:\\Qt_dev\\FinalProject\\db\\FinalLaba.db";

    m_db.setDatabaseName(dbPath);

    qDebug() << "正在连接数据库：" << dbPath;

    if (!m_db.open()) {
        qDebug() << "连接失败：" << m_db.lastError().text();
        return false;
    }

    qDebug() << "连接成功！";
    return true;
}

QSqlDatabase DatabaseManager::getDb() const
{
    return m_db;
}

void DatabaseManager::closeDb()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
}
