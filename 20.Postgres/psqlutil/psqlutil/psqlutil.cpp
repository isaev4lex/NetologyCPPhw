#include <iostream>
#include <pqxx/pqxx>
#include <string>
#include <vector>

class ClientDB {
private:
    pqxx::connection* conn;

public:
    ClientDB(const std::string& conn_info) {
        conn = new pqxx::connection(conn_info);
    }

    static void createDatabaseIfNotExists(const std::string& dbname, const std::string& admin_conn_info) {
        pqxx::connection admin_conn(admin_conn_info);

        // Сначала проверим — это можно делать в транзакции
        {
            pqxx::work check(admin_conn);
            pqxx::result r = check.exec_params("SELECT 1 FROM pg_database WHERE datname = $1", dbname);
            check.commit();

            if (!r.empty()) {
                std::cout << "Database " << dbname << " already exists." << std::endl;
                return;
            }
        }

        // А вот создание базы — ТОЛЬКО вне транзакции
        {
            pqxx::nontransaction direct(admin_conn);
            direct.exec("CREATE DATABASE \"" + dbname + "\"");
            std::cout << "Database " << dbname << " created successfully." << std::endl;
        }
    }

    void createTables() {
        pqxx::work txn(*conn);
        txn.exec(R"(
            CREATE TABLE IF NOT EXISTS clients (
                client_id SERIAL PRIMARY KEY,
                first_name VARCHAR(50),
                last_name VARCHAR(50),
                email VARCHAR(100) UNIQUE
            );

            CREATE TABLE IF NOT EXISTS phones (
                phone_id SERIAL PRIMARY KEY,
                client_id INTEGER REFERENCES clients(client_id) ON DELETE CASCADE,
                phone VARCHAR(20)
            );
        )");
        txn.commit();
        std::cout << "Tables created successfully." << std::endl;
    }

    void addClient(const std::string& first_name, const std::string& last_name, const std::string& email) {
        pqxx::work txn(*conn);
        txn.exec_params("INSERT INTO clients(first_name, last_name, email) VALUES ($1, $2, $3)",
            first_name, last_name, email);
        txn.commit();
        std::cout << "Client added successfully." << std::endl;
    }

    void addPhone(int client_id, const std::string& phone) {
        pqxx::work txn(*conn);
        txn.exec_params("INSERT INTO phones(client_id, phone) VALUES ($1, $2)",
            client_id, phone);
        txn.commit();
        std::cout << "Phone added successfully." << std::endl;
    }

    void updateClient(int client_id, const std::string& first_name, const std::string& last_name, const std::string& email) {
        pqxx::work txn(*conn);
        txn.exec_params("UPDATE clients SET first_name=$1, last_name=$2, email=$3 WHERE client_id=$4",
            first_name, last_name, email, client_id);
        txn.commit();
        std::cout << "Client updated successfully." << std::endl;
    }

    void deletePhone(int phone_id) {
        pqxx::work txn(*conn);
        txn.exec_params("DELETE FROM phones WHERE phone_id=$1", phone_id);
        txn.commit();
        std::cout << "Phone deleted successfully." << std::endl;
    }

    void deleteClient(int client_id) {
        pqxx::work txn(*conn);
        txn.exec_params("DELETE FROM clients WHERE client_id=$1", client_id);
        txn.commit();
        std::cout << "Client deleted successfully." << std::endl;
    }

    void findClient(const std::string& query) {
        pqxx::work txn(*conn);
        pqxx::result res = txn.exec_params(
            "SELECT c.client_id, first_name, last_name, email, phone "
            "FROM clients c LEFT JOIN phones p ON c.client_id = p.client_id "
            "WHERE first_name=$1 OR last_name=$1 OR email=$1 OR phone=$1",
            query
        );

        for (auto row : res) {
            std::cout << "Client ID: " << row["client_id"].as<int>()
                << ", Name: " << row["first_name"].c_str() << " " << row["last_name"].c_str()
                << ", Email: " << row["email"].c_str()
                << ", Phone: " << (row["phone"].is_null() ? "None" : row["phone"].c_str()) << std::endl;
        }
    }

    ~ClientDB() {
        delete conn;
    }
};

int main() {
    const std::string admin_conn_info = "host=localhost port=5432 dbname=postgres user=postgres password=QWaszx123!";
    const std::string target_db = "clientdb";

    ClientDB::createDatabaseIfNotExists(target_db, admin_conn_info);

    ClientDB db("host=localhost port=5432 dbname=" + target_db + " user=postgres password=QWaszx123!");

    db.createTables();

    db.addClient("John", "Doe", "john.doe@example.com");
    db.addClient("Jane", "Smith", "jane.smith@example.com");

    db.addPhone(1, "+1234567890");
    db.addPhone(1, "+0987654321");
    db.addPhone(2, "+111222333");

    db.updateClient(1, "Johnny", "Doe", "johnny.doe@example.com");

    db.deletePhone(2);

    db.findClient("Johnny");
    db.findClient("+111222333");

    db.deleteClient(2);

    return 0;
}
