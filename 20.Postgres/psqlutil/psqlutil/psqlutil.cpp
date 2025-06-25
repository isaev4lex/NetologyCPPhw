#include <iostream>
#include <pqxx/pqxx>
#include <string>
#include <vector>

struct ClientInfo {
    int client_id;
    std::string first_name;
    std::string last_name;
    std::string email;
    std::string phone;
};

class ClientDB {
private:
    pqxx::connection* conn;

public:
    ClientDB(const std::string& conn_info) {
        conn = new pqxx::connection(conn_info);
    }

    static void createDatabaseIfNotExists(const std::string& dbname, const std::string& admin_conn_info) {
        pqxx::connection admin_conn(admin_conn_info);

        {
            pqxx::work check(admin_conn);
            pqxx::result r = check.exec_params("SELECT 1 FROM pg_database WHERE datname = $1", dbname);
            check.commit();

            if (!r.empty()) {
                std::cout << "Database " << dbname << " already exists." << std::endl;
                return;
            }
        }

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

    int ensureClientExists(const std::string& first_name, const std::string& last_name, const std::string& email) {
        pqxx::work txn(*conn);
        pqxx::result r = txn.exec_params("SELECT client_id FROM clients WHERE email = $1", email);
        if (!r.empty()) {
            int existing_id = r[0]["client_id"].as<int>();
            txn.commit();
            return existing_id;
        }
        pqxx::result insert = txn.exec_params(
            "INSERT INTO clients(first_name, last_name, email) VALUES ($1, $2, $3) RETURNING client_id",
            first_name, last_name, email);
        int new_id = insert[0]["client_id"].as<int>();
        txn.commit();
        std::cout << "Client inserted automatically: " << new_id << std::endl;
        return new_id;
    }

    void addClient(const std::string& first_name, const std::string& last_name, const std::string& email) {
        ensureClientExists(first_name, last_name, email);
        std::cout << "Client added successfully." << std::endl;
    }

    void addPhone(int client_id, const std::string& phone) {
        pqxx::work txn(*conn);
        pqxx::result r = txn.exec_params("SELECT 1 FROM clients WHERE client_id = $1", client_id);
        if (r.empty()) {
            txn.commit();
            throw std::runtime_error("Client with ID " + std::to_string(client_id) + " does not exist.");
        }
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

    std::vector<ClientInfo> findClient(const std::string& query) {
        pqxx::work txn(*conn);
        pqxx::result res = txn.exec_params(
            "SELECT c.client_id, first_name, last_name, email, phone "
            "FROM clients c LEFT JOIN phones p ON c.client_id = p.client_id "
            "WHERE first_name=$1 OR last_name=$1 OR email=$1 OR phone=$1",
            query
        );

        std::vector<ClientInfo> results;
        for (auto row : res) {
            results.push_back({
                row["client_id"].as<int>(),
                row["first_name"].c_str(),
                row["last_name"].c_str(),
                row["email"].c_str(),
                row["phone"].is_null() ? "" : row["phone"].c_str()
                });
        }
        return results;
    }

    ~ClientDB() {
        delete conn;
    }
};

int main() {
    try {
        const std::string admin_conn_info = "host=localhost port=5432 dbname=postgres user=postgres password=QWaszx123!";
        const std::string target_db = "clientdb";

        ClientDB::createDatabaseIfNotExists(target_db, admin_conn_info);

        ClientDB db("host=localhost port=5432 dbname=" + target_db + " user=postgres password=QWaszx123!");

        db.createTables();

        int id1 = db.ensureClientExists("Dolorez", "Haze", "dolorez.haze@gmail.com");
        int id2 = db.ensureClientExists("Elliot", "Alderson", "elliot.alderson@protonmail.com");

        db.addPhone(id1, "+1265496558");
        db.addPhone(id1, "+0987654321");
        db.addPhone(id2, "+111222333");

        db.updateClient(id1, "Dolorez", "Haze", "dolorez.haze@gmail.com");

        db.deletePhone(2);

        auto found1 = db.findClient("Dolorez");
        for (const auto& client : found1) {
            std::cout << "Client ID: " << client.client_id
                << ", Name: " << client.first_name << " " << client.last_name
                << ", Email: " << client.email
                << ", Phone: " << (client.phone.empty() ? "None" : client.phone) << std::endl;
        }

        auto found2 = db.findClient("+111222333");
        for (const auto& client : found2) {
            std::cout << "Client ID: " << client.client_id
                << ", Name: " << client.first_name << " " << client.last_name
                << ", Email: " << client.email
                << ", Phone: " << (client.phone.empty() ? "None" : client.phone) << std::endl;
        }

        db.deleteClient(id2);
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
