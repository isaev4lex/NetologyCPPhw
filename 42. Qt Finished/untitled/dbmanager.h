#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QPair>
#include <QDate>

struct FlightInfo {
    bool ok = false;
    qint64 id = -1;
    QString fromCode, toCode;
    QString fromName, toName;
    QString depTime, arrTime;
    QString airline;
    double  price = 0.0;
};

class DbManager : public QObject
{
    Q_OBJECT
public:
    explicit DbManager(QObject* parent = nullptr);
    ~DbManager();

    QList<QPair<QString, QString>> airports() const;

    bool searchFlights(const QString& fromCode, const QString& toCode, const QDate& date, QSqlQueryModel* intoModel);

    FlightInfo flightInfo(qint64 flightId);
    QList<QPair<QString,QString>> airportsList() const;
    FlightInfo flightSummary(qint64 flightId);

    bool addBooking(qint64 flightId, const QString& passengerName, const QString& passport);
    bool loadBookings(QSqlQueryModel* intoModel);
    bool deleteBooking(int bookingId);

private:
    bool open();

    QSqlDatabase m_db;
};

#endif // DBMANAGER_H
