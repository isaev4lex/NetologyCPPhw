#include "dbmanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDateTime>
#include <QDir>
#include <QCoreApplication>
#include <QDebug>

static const char* PG_HOST = "981757-ca08998.tmweb.ru";
static const int   PG_PORT = 5432;
static const char* PG_DB   = "demo";
static const char* PG_USER = "netology_usr_cpp";
static const char* PG_PASS = "CppNeto3";

DbManager::DbManager(QObject *parent) : QObject(parent)
{
    open();
}

DbManager::~DbManager()
{
    if (m_db.isOpen()) m_db.close();
}

bool DbManager::open()
{
    if (m_db.isOpen()) return true;

    if (!QSqlDatabase::contains("pg"))
        m_db = QSqlDatabase::addDatabase("QPSQL", "pg");
    else
        m_db = QSqlDatabase::database("pg");

    m_db.setHostName(PG_HOST);
    m_db.setPort(PG_PORT);
    m_db.setDatabaseName(PG_DB);
    m_db.setUserName(PG_USER);
    m_db.setPassword(PG_PASS);

    if (!m_db.open()) {
        qWarning() << "PG connect failed:" << m_db.lastError().text();
        return false;
    }

    QSqlQuery q(m_db);
    q.exec("SET TIME ZONE 'UTC'");

    q.exec(
        "CREATE TEMP TABLE IF NOT EXISTS ui_bookings ("
        "  id SERIAL PRIMARY KEY,"
        "  flight_id BIGINT NOT NULL,"
        "  passenger_name TEXT NOT NULL,"
        "  passport TEXT NOT NULL,"
        "  created_at TIMESTAMPTZ NOT NULL DEFAULT now()"
        ") ON COMMIT PRESERVE ROWS"
        );

    qInfo() << "Connected to PG" << PG_HOST << PG_DB;
    return true;
}

QList<QPair<QString, QString>> DbManager::airports() const
{
    QList<QPair<QString,QString>> out;
    QSqlQuery q(m_db);
    q.exec("SELECT ad.airport_code, ad.airport_name->>'ru' AS ru "
           "FROM bookings.airports_data ad "
           "WHERE ad.airport_name ? 'ru' "
           "ORDER BY ru ASC");
    while (q.next()) {
        out.push_back({ q.value(0).toString(), q.value(1).toString() });
    }
    return out;
}

bool DbManager::searchFlights(const QString &fromCode, const QString &toCode,
                              const QDate &date, QSqlQueryModel *intoModel)
{
    if (!intoModel) return false;
    if (!m_db.isOpen() && !const_cast<DbManager*>(this)->open()) return false;

    QSqlQuery q(m_db);
    q.prepare(
        "SELECT "
        "  f.flight_id::bigint AS id, "
        "  (ad1.airport_name->>'ru') || ' → ' || (ad2.airport_name->>'ru') AS route, "
        "  (f.scheduled_departure AT TIME ZONE 'UTC') AS departure, "
        "  (f.scheduled_arrival   AT TIME ZONE 'UTC') AS arrival, "
        "  ''::text AS airline, "
        "  ''::text AS price "
        "FROM bookings.flights f "
        "JOIN bookings.airports_data ad1 ON ad1.airport_code = f.departure_airport "
        "JOIN bookings.airports_data ad2 ON ad2.airport_code = f.arrival_airport "
        "WHERE f.departure_airport = :from "
        "  AND f.arrival_airport   = :to "
        "  AND (f.scheduled_departure::date = :d) "
        "ORDER BY f.scheduled_departure ASC"
        );
    q.bindValue(":from", fromCode);
    q.bindValue(":to",   toCode);
    q.bindValue(":d",    date);

    if (!q.exec()) {
        qWarning() << "searchFlights SQL error:" << q.lastError().text();
        return false;
    }

    intoModel->setQuery(std::move(q));
    while (intoModel->canFetchMore()) intoModel->fetchMore();
    intoModel->setHeaderData(1, Qt::Horizontal, "Маршрут");
    intoModel->setHeaderData(2, Qt::Horizontal, "Вылет");
    intoModel->setHeaderData(3, Qt::Horizontal, "Прилёт");
    intoModel->setHeaderData(4, Qt::Horizontal, "Авиакомпания");
    intoModel->setHeaderData(5, Qt::Horizontal, "Цена");
    return true;
}


FlightInfo DbManager::flightInfo(qint64 flightId)
{
    FlightInfo fi;
    if (!m_db.isOpen() && !open()) return fi;

    QSqlQuery q(m_db);
    q.prepare(
        "SELECT "
        "  f.flight_id::bigint, "
        "  f.departure_airport, "
        "  f.arrival_airport, "
        "  (f.scheduled_departure AT TIME ZONE 'UTC')::text, "
        "  (f.scheduled_arrival   AT TIME ZONE 'UTC')::text, "
        "  ''::text AS airline, "
        "  0.0::float8 AS price, "
        "  (ad1.airport_name->>'ru'), "
        "  (ad2.airport_name->>'ru') "
        "FROM bookings.flights f "
        "JOIN bookings.airports_data ad1 ON ad1.airport_code = f.departure_airport "
        "JOIN bookings.airports_data ad2 ON ad2.airport_code = f.arrival_airport "
        "WHERE f.flight_id = CAST(:id AS BIGINT)"
        );
    q.bindValue(":id", QVariant::fromValue<qint64>(flightId));

    if (!q.exec()) {
        qWarning() << "[flightInfo] SQL error:" << q.lastError().text()
        << " id=" << flightId;
        return fi;
    }
    if (!q.next()) {
        qWarning() << "[flightInfo] not found, id=" << flightId;
        return fi;
    }

    fi.ok       = true;
    fi.id       = q.value(0).toLongLong();
    fi.fromCode = q.value(1).toString();
    fi.toCode   = q.value(2).toString();
    fi.depTime  = q.value(3).toString();
    fi.arrTime  = q.value(4).toString();
    fi.airline  = q.value(5).toString();
    fi.price    = q.value(6).toDouble();
    fi.fromName = q.value(7).toString();
    fi.toName   = q.value(8).toString();
    return fi;
}

QList<QPair<QString,QString>> DbManager::airportsList() const {
    return airports();
}

FlightInfo DbManager::flightSummary(qint64 flightId) {
    return flightInfo(flightId);
}

bool DbManager::addBooking(qint64 flightId, const QString& passengerName, const QString& passport)
{
    if (!m_db.isOpen() && !open()) return false;

    QSqlQuery q(m_db);
    q.prepare("INSERT INTO ui_bookings(flight_id,passenger_name,passport,created_at) "
              "VALUES(:fid, :pn, :pp, now())");
    q.bindValue(":fid", QVariant::fromValue<qlonglong>(flightId));
    q.bindValue(":pn",  passengerName.trimmed());
    q.bindValue(":pp",  passport.trimmed());
    if (!q.exec()) {
        qWarning() << "addBooking error:" << q.lastError().text();
        return false;
    }
    return true;
}

bool DbManager::loadBookings(QSqlQueryModel *intoModel)
{
    if (!intoModel) return false;
    if (!m_db.isOpen() && !open()) return false;

    QSqlQuery q(m_db);
    q.exec(
        "SELECT b.id, "
        "       b.passenger_name AS \"Пассажир\", "
        "       b.passport       AS \"Паспорт\", "
        "       (ad1.airport_name->>'ru') || ' → ' || (ad2.airport_name->>'ru') AS \"Маршрут\", "
        "       (f.scheduled_departure AT TIME ZONE 'UTC') AS \"Вылет\", "
        "       ''::text AS \"Цена\", "
        "       b.created_at AS \"Создано\" "
        "  FROM ui_bookings b "
        "  JOIN bookings.flights f ON f.flight_id = b.flight_id "
        "  JOIN bookings.airports_data ad1 ON ad1.airport_code = f.departure_airport "
        "  JOIN bookings.airports_data ad2 ON ad2.airport_code = f.arrival_airport "
        " ORDER BY b.id DESC"
        );
    intoModel->setQuery(std::move(q));
    while (intoModel->canFetchMore()) intoModel->fetchMore();
    return true;
}

bool DbManager::deleteBooking(int bookingId)
{
    if (!m_db.isOpen() && !open()) return false;

    QSqlQuery q(m_db);
    q.prepare("DELETE FROM ui_bookings WHERE id=:id");
    q.bindValue(":id", bookingId);
    if (!q.exec()) {
        qWarning() << "deleteBooking error:" << q.lastError().text();
        return false;
    }
    return true;
}


