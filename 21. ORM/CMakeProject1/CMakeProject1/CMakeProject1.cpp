#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Postgres.h>
#include <Wt/WDateTime.h>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

namespace dbo = Wt::Dbo;

class Publisher;
class Book;
class Shop;
class Stock;
class Sale;

class Publisher {
public:
    long long id;
    std::string name;
    dbo::collection<dbo::ptr<Book>> books;

    template<class Action>
    void persist(Action& a) {
        dbo::id(a, id, "id");
        dbo::field(a, name, "name");
        dbo::hasMany(a, books, dbo::ManyToOne, "publisher");
    }
};

class Book {
public:
    long long id;
    std::string title;
    dbo::ptr<Publisher> publisher;
    dbo::collection<dbo::ptr<Stock>> stocks;

    template<class Action>
    void persist(Action& a) {
        dbo::id(a, id, "id");
        dbo::field(a, title, "title");
        dbo::belongsTo(a, publisher, "publisher");
        dbo::hasMany(a, stocks, dbo::ManyToOne, "book");
    }
};

class Shop {
public:
    long long id;
    std::string name;
    dbo::collection<dbo::ptr<Stock>> stocks;

    template<class Action>
    void persist(Action& a) {
        dbo::id(a, id, "id");
        dbo::field(a, name, "name");
        dbo::hasMany(a, stocks, dbo::ManyToOne, "shop");
    }
};

class Stock {
public:
    long long id;
    dbo::ptr<Book> book;
    dbo::ptr<Shop> shop;
    int count;
    dbo::collection<dbo::ptr<Sale>> sales;

    template<class Action>
    void persist(Action& a) {
        dbo::id(a, id, "id");
        dbo::belongsTo(a, book, "book");
        dbo::belongsTo(a, shop, "shop");
        dbo::field(a, count, "count");
        dbo::hasMany(a, sales, dbo::ManyToOne, "stock");
    }
};

class Sale {
public:
    long long id;
    double price;
    std::string date_sale;
    dbo::ptr<Stock> stock;
    int count;

    template<class Action>
    void persist(Action& a) {
        dbo::id(a, id, "id");
        dbo::field(a, price, "price");
        dbo::field(a, date_sale, "date_sale");
        dbo::belongsTo(a, stock, "stock");
        dbo::field(a, count, "count");
    }
};


int main() {
    try {
        std::unique_ptr<dbo::backend::Postgres> conn = std::make_unique<dbo::backend::Postgres>(
            "host=localhost dbname=wt_test user=postgres password=QWaszx123!"
        );

        dbo::Session session;
        session.setConnection(std::move(conn));
        session.mapClass<Publisher>("publisher");
        session.mapClass<Book>("book");
        session.mapClass<Shop>("shop");
        session.mapClass<Stock>("stock");
        session.mapClass<Sale>("sale");

        {
            dbo::Transaction t(session);
            session.createTables();
            t.commit();
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

            auto stock = session.add(std::make_unique<Stock>());
            stock.modify()->book = book;
            stock.modify()->shop = shop;
            stock.modify()->count = 5;

            t.commit();
        }

        std::string name;
        std::cout << "Enter publisher name: ";
        std::getline(std::cin, name);

        std::vector<dbo::ptr<Shop>> result;
        {
            dbo::Transaction t(session);

            dbo::collection<dbo::ptr<Shop>> query =
                session.query<dbo::ptr<Shop>>(
                    "SELECT shop.* FROM shop "
                    "JOIN stock ON stock.shop_id = shop.id "
                    "JOIN book ON stock.book_id = book.id "
                    "JOIN publisher ON book.publisher_id = publisher.id "
                    "WHERE publisher.name = ?"
                ).bind(name);

            for (auto shop : query) {
                result.push_back(shop);
            }
        }

        std::cout << "Shops selling books by \"" << name << "\":" << std::endl;
        for (auto& shop : result) {
            std::cout << "- " << shop->name << std::endl;
        }

    }
    catch (const std::exception& ex) {
        std::cerr << "ERROR: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
