#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Postgres.h>
#include <iostream>
#include <string>
#include <vector>

namespace dbo = Wt::Dbo;

class Publisher;
class Book;
class Shop;
class Stock;
class Sale;

/* ─── dbo_traits<T> (чтобы ключ задавался вручную) ─── */
namespace Wt::Dbo {
    template<> struct dbo_traits<Publisher> : dbo_default_traits { static const char* surrogateIdField() { return nullptr; } };
    template<> struct dbo_traits<Book> : dbo_default_traits { static const char* surrogateIdField() { return nullptr; } };
    template<> struct dbo_traits<Shop> : dbo_default_traits { static const char* surrogateIdField() { return nullptr; } };
    template<> struct dbo_traits<Stock> : dbo_default_traits { static const char* surrogateIdField() { return nullptr; } };
    template<> struct dbo_traits<Sale> : dbo_default_traits { static const char* surrogateIdField() { return nullptr; } };
}

class Publisher {
public:
    long long id; std::string name;
    dbo::collection<dbo::ptr<Book>> books;
    template<class A> void persist(A& a) {
        dbo::id(a, id, "id"); dbo::field(a, name, "name");
        dbo::hasMany(a, books, dbo::ManyToOne, "publisher");
    }
};

class Book {
public:
    long long id; std::string title; dbo::ptr<Publisher> publisher;
    dbo::collection<dbo::ptr<Stock>> stocks;
    template<class A> void persist(A& a) {
        dbo::id(a, id, "id"); dbo::field(a, title, "title");
        dbo::belongsTo(a, publisher, "publisher");
        dbo::hasMany(a, stocks, dbo::ManyToOne, "book");
    }
};

class Shop {
public:
    long long id; std::string name;
    dbo::collection<dbo::ptr<Stock>> stocks;
    template<class A> void persist(A& a) {
        dbo::id(a, id, "id"); dbo::field(a, name, "name");
        dbo::hasMany(a, stocks, dbo::ManyToOne, "shop");
    }
};

class Stock {
public:
    long long id; dbo::ptr<Book> book; dbo::ptr<Shop> shop; int count;
    dbo::collection<dbo::ptr<Sale>> sales;
    template<class A> void persist(A& a) {
        dbo::id(a, id, "id");
        dbo::belongsTo(a, book, "book"); dbo::belongsTo(a, shop, "shop");
        dbo::field(a, count, "count");
        dbo::hasMany(a, sales, dbo::ManyToOne, "stock");
    }
};

class Sale {
public:
    long long id; double price; std::string date_sale;
    dbo::ptr<Stock> stock; int count;
    template<class A> void persist(A& a) {
        dbo::id(a, id, "id"); dbo::field(a, price, "price");
        dbo::field(a, date_sale, "date_sale");
        dbo::belongsTo(a, stock, "stock");
        dbo::field(a, count, "count");
    }
};

int main() {
    try {
        auto conn = std::make_unique<dbo::backend::Postgres>(
            "host=localhost dbname=wt_test user=postgres password=QWaszx123!");
        dbo::Session session;
        session.setConnection(std::move(conn));

        session.mapClass<Publisher>("publisher");
        session.mapClass<Book>("book");
        session.mapClass<Shop>("shop");
        session.mapClass<Stock>("stock");
        session.mapClass<Sale>("sale");

        {
            dbo::Transaction t(session);
            try {
                session.dropTables();
                session.createTables();
            }
            catch (const std::exception& e) {
                // «relation ... already exists» → код 42P07
                if (std::string(e.what()).find("42P07") == std::string::npos)
                    throw;
                std::cerr << "[info] tables already exist, skip create\n";
            }
        }

        {
            dbo::Transaction t(session);
            auto pub = session.add(std::make_unique<Publisher>());
            pub.modify()->name = "TargetPublisher";

            auto book = session.add(std::make_unique<Book>());
            book.modify()->title = "Target Book";
            book.modify()->publisher = pub;

            auto shop = session.add(std::make_unique<Shop>());
            shop.modify()->name = "Shop #1";

            auto st = session.add(std::make_unique<Stock>());
            st.modify()->book = book;
            st.modify()->shop = shop;
            st.modify()->count = 5;
        }

        std::cout << "Enter publisher name: ";
        std::string name; std::getline(std::cin, name);

        std::vector<dbo::ptr<Shop>> shops;
        {
            dbo::Transaction t(session);
            dbo::collection<dbo::ptr<Shop>> q =
                session.query<dbo::ptr<Shop>>(
                    "SELECT shop "
                    "FROM shop "
                    "JOIN stock     ON stock.shop_id  = shop.id "
                    "JOIN book      ON stock.book_id  = book.id "
                    "JOIN publisher ON book.publisher_id = publisher.id "
                    "WHERE publisher.name = ?"
                ).bind(name);

            for (auto& s : q) if (s) shops.push_back(s);
        }

        std::cout << "Shops selling books by \"" << name << "\":\n";
        for (auto& s : shops) std::cout << " - " << s->name << '\n';
    }
    catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << '\n';
        return 1;
    }
}
