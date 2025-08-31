#include <fstream>
#include <string>
#include <iostream>
#include <utility>


struct ITextPrintable {
    virtual ~ITextPrintable() = default;
    virtual std::string asText() const = 0;
};

struct IHtmlPrintable {
    virtual ~IHtmlPrintable() = default;
    virtual std::string asHtml() const = 0;
};

struct IJsonPrintable {
    virtual ~IJsonPrintable() = default;
    virtual std::string asJson() const = 0;
};

class TextData : public ITextPrintable {
    std::string data_;
public:
    explicit TextData(std::string data) : data_(std::move(data)) {}
    std::string asText() const override {
        return data_; 
    }
};

class HtmlData : public IHtmlPrintable {
    std::string data_;
public:
    explicit HtmlData(std::string data) : data_(std::move(data)) {}
    std::string asHtml() const override { 
        return "<html>" + data_ + "</html>"; 
    }
};

class JsonData : public IJsonPrintable {
    std::string data_;
public:
    explicit JsonData(std::string data) : data_(std::move(data)) {}
    std::string asJson() const override { 
        return "{ \"data\": \"" + data_ + "\" }"; 
    }
};

inline void saveTo(std::ofstream& file, const std::string& payload) {
    file << payload;
}

inline void saveToAsText(std::ofstream& file, const ITextPrintable& p) {
    saveTo(file, p.asText());
}

inline void saveToAsHTML(std::ofstream& file, const IHtmlPrintable& p) {
    saveTo(file, p.asHtml());
}

inline void saveToAsJSON(std::ofstream& file, const IJsonPrintable& p) {
    saveTo(file, p.asJson());
}

int main() {
    TextData t("txtfile");
    HtmlData h("htmlfile");
    JsonData j("jsonfile");

    std::ofstream f1("out.txt");
    std::ofstream f2("out.html");
    std::ofstream f3("out.json");

    if (!f1 || !f2 || !f3) {
        std::cerr << "Can't open files" << std::endl;
        return 1;
    }

    saveToAsText(f1, t);
    saveToAsHTML(f2, h);
    saveToAsJSON(f3, j);

    std::cout << "Finished: out.txt, out.html, out.json" << std::endl;
    return 0;
}