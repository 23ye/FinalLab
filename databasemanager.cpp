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
    // 如果已经打开，直接返回 true
    if (m_db.isOpen()) {
        return true;
    }

    // 添加 SQLite 驱动
    m_db = QSqlDatabase::addDatabase("QSQLITE");

    // 设置数据库路径
    // QCoreApplication::applicationDirPath() 指向编译出的 .exe 所在目录
    QString dbPath = QCoreApplication::applicationDirPath() + "/" + DB_NAME;

    m_db.setDatabaseName(dbPath);

    qDebug() << "Connecting to database at:" << dbPath;

    if (!m_db.open()) {
        qDebug() << "Error: connection with database failed";
        qDebug() << m_db.lastError().text();
        return false;
    }

    qDebug() << "Database: connection ok";
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
