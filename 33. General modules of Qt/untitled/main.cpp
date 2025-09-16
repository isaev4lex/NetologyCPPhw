#include <QCoreApplication>
#include <QSqlDatabase>
#include <QNetworkAccessManager>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QNetworkAccessManager networkMgr;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");


    return a.exec();
}
