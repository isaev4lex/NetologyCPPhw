#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

namespace first_task {
    class LogCommand {
        public:
            virtual ~LogCommand() = default;
            virtual void print(const std::string& message) = 0;
    };


    class ConsoleLogCommand : public LogCommand {
        public:
            void print(const std::string& message) override {
                std::cout << "[Console] " << message << std::endl;
            }
    };


    class FileLogCommand : public LogCommand {
        private:
            std::string path_;
        public:
            explicit FileLogCommand(std::string path) : path_(std::move(path)) {}
            void print(const std::string& message) override {
                std::ofstream out(path_, std::ios::app);
                if (!out) {
                    throw std::runtime_error("Cannot open file: " + path_);
                };
                out << message << '\n';
            }
    };


    void print(LogCommand& cmd, const std::string& message) {
        cmd.print(message);
    }
}

namespace second_task {
    class Observer {
        public:
            virtual ~Observer() = default;
            virtual void onWarning(const std::string&) {}
            virtual void onError(const std::string&) {}
            virtual void onFatalError(const std::string&) {}
    };


    class ObservableLogger {
    private:
        std::vector<Observer*> observers_;
    public:
        void addObserver(Observer* obs) {
            if (!obs) return;
            observers_.push_back(obs);
        }

        void removeObserver(Observer* obs) {
            if (!obs) return;
            for (std::size_t i = observers_.size(); i > 0; --i) {
                if (observers_[i - 1] == obs) {
                    observers_.erase(observers_.begin() + (i - 1));
                }
            }
        }

        void warning(const std::string& message) const {
            for (Observer* o : observers_) {
                if (o) {
                    o->onWarning(message);
                };
            }
        }

        void error(const std::string& message) const {
            for (Observer* o : observers_) {
                if (o) {
                    o->onError(message);
                };
            }
        }

        void fatalError(const std::string& message) const {
            for (Observer* o : observers_) {
                if (o) {
                    o->onFatalError(message);
                };
            }
        }
    };


    class WarningConsoleObserver : public Observer {
    public:
        void onWarning(const std::string& message) override {
            std::cout << "[Warning] " << message << std::endl;
        }
    };


    class ErrorFileObserver : public Observer {
        private:
            std::string path_;
        public:
            explicit ErrorFileObserver(const std::string& path) : path_(path) {}

            void onError(const std::string& message) override {
                std::ofstream out(path_.c_str(), std::ios::app);
                if (!out) {
                    std::cerr << "Cannot open file: " << path_ << std::endl;
                    return;
                }
                out << "[Error] " << message << '\n';
            }
    };


    class FatalConsoleAndFileObserver : public Observer {
        private:
            std::string path_;
        public:
            explicit FatalConsoleAndFileObserver(const std::string& path) : path_(path) {}

            void onFatalError(const std::string& message) override {
                std::cout << "[FATAL] " << message << std::endl;
                std::ofstream out(path_.c_str(), std::ios::app);
                if (!out) {
                    std::cerr << "Cannot open file: " << path_ << std::endl;
                    return;
                }
                out << "[FATAL] " << message << '\n';
            }
    };
}


namespace third_task {
    enum class Type { Warning, Error, Fatal, Unknown };


    class LogMessage {
        private:
            Type t_;
            std::string m_;
        public:
            LogMessage(Type t, std::string m) : t_(t), m_(std::move(m)) {}
            Type type() const { return t_; }
            const std::string& message() const { return m_; }
    };

    class Handler {
        private:
            std::unique_ptr<Handler> next_;
        protected:
            virtual bool canHandle(Type) const = 0;
            virtual void process(const LogMessage& msg) = 0;
        public:
            virtual ~Handler() = default;

            Handler* setNext(std::unique_ptr<Handler> next) {
                next_ = std::move(next);
                return next_.get();
            }

            void handle(const LogMessage& msg) {
                if (canHandle(msg.type())) {
                    process(msg);
                } else if (next_) {
                    next_->handle(msg);
                } else {
                    throw std::runtime_error("Unhandled message: " + msg.message());
                }
            }
    };


    class FatalHandler : public Handler {
        protected:
            bool canHandle(Type t) const override { return t == Type::Fatal; }
            void process(const LogMessage& msg) override {
                throw std::runtime_error(msg.message());
            }
    };

    class ErrorFileHandler : public Handler {
        private:
            std::string path_;
        protected:
            bool canHandle(Type t) const override { return t == Type::Error; }
            void process(const LogMessage& msg) override {
                std::ofstream out(path_, std::ios::app);
                if (!out) {
                    throw std::runtime_error("Cannot open file: " + path_);
                };
                out << msg.message() << '\n';
            }
        public:
            explicit ErrorFileHandler(std::string path) : path_(std::move(path)) {}
    };

    class WarningConsoleHandler : public Handler {
        protected:
            bool canHandle(Type t) const override { 
                return t == Type::Warning; 
            }
            void process(const LogMessage& msg) override {
                std::cout << "[WARN] " << msg.message() << std::endl;
            }
    };

    class UnknownHandler : public Handler {
    protected:
        bool canHandle(Type t) const override { return t == Type::Unknown; }
        void process(const LogMessage& msg) override {
            throw std::runtime_error("Unknown message: " + msg.message());
        }
    };
}
int main() {
    using namespace first_task;

    ConsoleLogCommand consoleCmd;
    FileLogCommand fileCmd("command.log");
    print(consoleCmd, "Test cmd");
    print(fileCmd, "test to file");



    using namespace second_task;

    ObservableLogger logger;
    WarningConsoleObserver warnObs;
    ErrorFileObserver errObs("observer.err");
    FatalConsoleAndFileObserver fatalObs("observer_fatal.log");

    logger.addObserver(&warnObs);
    logger.addObserver(&errObs);
    logger.addObserver(&fatalObs);

    logger.warning("WARN");
    logger.error("ERR");
    logger.fatalError("FATAL ERROR");

    logger.removeObserver(&warnObs);



    using namespace third_task;

    auto fatalH = std::make_unique<FatalHandler>();
    auto errorH = std::make_unique<ErrorFileHandler>("chain_error.log");
    auto warningH = std::make_unique<WarningConsoleHandler>();
    auto unknownH = std::make_unique<UnknownHandler>();

    Handler* head = fatalH.get();
    Handler* e = head->setNext(std::move(errorH));
    Handler* w = e->setNext(std::move(warningH));
    w->setNext(std::move(unknownH));

    try {
        head->handle(LogMessage{ Type::Warning, "CPU hot ( buy ryzen next time )" });
        head->handle(LogMessage{ Type::Error,   "Disk error ( Use hdd )" });
        head->handle(LogMessage{ Type::Unknown, "??? message" });
    } catch (const std::exception& e) {
        std::cout << "exceptin (Unknown): " << e.what() << '\n';
    }

    try {
        head->handle(LogMessage{ Type::Fatal, "Kernel panic" });
    } catch (const std::exception& e) {
        std::cout << "exception (Fatal): " << e.what() << '\n';
    }

    return 0;
}