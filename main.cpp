#include <QApplication>
#include <QTextCodec>
#include <QtSql>
#include <QtDebug>
#include "dialog.h"
#include "commonstore.h"
#include "goodsnamestore.h"
#include "shopnamestore.h"
#include "invoicestore.h"
#include "store.h"

class DBDefer
{
public:
    DBDefer(const QString& dbpath)
    {
        qDebug() << QSqlDatabase::drivers();

        m_db = QSqlDatabase::addDatabase("QSQLITE");
        m_db.setDatabaseName(dbpath);
        if (false == m_db.open())
        {
            qFatal(m_db.lastError().text().toStdString().c_str());
        }
    }

    ~DBDefer()
    {
        m_db.close();
    }

public:
    QSqlDatabase& db(void)
    {
        return m_db;
    }

private:
    QSqlDatabase m_db;
};

//设置指定样式
static void setStyle(const QString &qssFile) {
    QFile file(qssFile);
    if (file.open(QFile::ReadOnly))
    {
        QString qss = QLatin1String(file.readAll());
        qApp->setStyleSheet(qss);
        QString PaletteColor = qss.mid(20, 7);
        qApp->setPalette(QPalette(QColor(PaletteColor)));
        file.close();
    }
}

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    QString curPath = QDir::currentPath();
    QString dbPath = curPath + "/data.db";
    qDebug() << dbPath;

    DBDefer dd(dbPath);
    QSqlDatabase& db = dd.db();
    if (false == CommonStore::instance()->init(&db))
    {
        return -1;
    }
    if (false == GoodsNameStore::instance()->init(&db))
    {
        return -1;
    }
    if (false == ShopNameStore::instance()->init(&db))
    {
        return -1;
    }
    if (false == InvoiceStore::instance()->init())
    {
        return -1;
    }
    if (false == Store::instance()->init(&db))
    {
        return -1;
    }

    setStyle(":/skin/MetroLite.qss");

    Dialog w;
    w.setWindowIcon(QIcon(":icon/main.ico"));
    w.setWindowFlags(Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
    w.show();

    return a.exec();
}
