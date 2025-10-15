#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

// Decoratr

struct ITextRenderable {
    virtual ~ITextRenderable() = default;
    virtual void render(const std::string& text) const = 0;
};

class Text : public ITextRenderable {
public:
    void render(const std::string& text) const override {
        std::cout << text << std::endl;
    }
};

class TextDecorator : public ITextRenderable {
protected:
    std::unique_ptr<ITextRenderable> wrappee_;
public:
    explicit TextDecorator(ITextRenderable* wrappee) : wrappee_(wrappee) {}
    void render(const std::string& text) const override {
        wrappee_->render(text);
    }
};

class Paragraph : public TextDecorator {
public:
    using TextDecorator::TextDecorator;
    void render(const std::string& text) const override {
        std::cout << "<p>";
        TextDecorator::render(text);
        std::cout << "</p>" << std::endl;
    }
};

class Reversed : public TextDecorator {
public:
    using TextDecorator::TextDecorator;
    void render(const std::string& text) const override {
        std::string tmp = text;
        std::reverse(tmp.begin(), tmp.end());
        TextDecorator::render(tmp);
    }
};

class Link {
    std::unique_ptr<ITextRenderable> wrappee_;
public:
    explicit Link(ITextRenderable* wrappee) : wrappee_(wrappee) {}

    void render(const std::string& href, const std::string& text) const {
        std::cout << "<a href=" << href << ">";
        std::cout << text;
        std::cout << "</a>" << std::endl;
    }
};







// Proxy

struct IDatabase {
    virtual ~IDatabase() = default;
    virtual std::string GetData(const std::string& key) = 0;
};

class VeryHeavyDatabase : public IDatabase {
    std::unordered_map<std::string, std::string> storage_;
public:
    VeryHeavyDatabase() {
        storage_.emplace("key", "value");
        storage_.emplace("user:1", "alex");
        storage_.emplace("user:2", "gaylord");
    }

    std::string GetData(const std::string& key) override {
        auto it = storage_.find(key);
        if (it == storage_.end()) return "not_found";
        return it->second;
    }
};

class OneShotDB : public IDatabase {
    IDatabase* real_ = nullptr;
    int shots_left_ = 1;
public:
    explicit OneShotDB(IDatabase* real_object, int shots = 1)
        : real_(real_object), shots_left_(shots) {
    }

    std::string GetData(const std::string& key) override {
        if (shots_left_ == 0) {
            return "error";
        }
        --shots_left_;
        return real_->GetData(key);
    }
};

int main() {
    std::cout << "DECORATOR\n";
    {
        auto text_block_p = std::make_unique<Paragraph>(new Text());
        text_block_p->render("Hello world");
        
        auto text_block_r = std::make_unique<Reversed>(new Text());
        text_block_r->render("Hello world");

        Link text_block_l(new Text());
        text_block_l.render("netology.ru", "Hello world");
    }

    std::cout << "\nPROXY\n";
    {
        VeryHeavyDatabase real_db;
        OneShotDB limit_db(&real_db, 2);

        std::cout << limit_db.GetData("key") << std::endl;
        std::cout << limit_db.GetData("key") << std::endl;
        std::cout << limit_db.GetData("key") << std::endl;
    }

    return 0;
}
