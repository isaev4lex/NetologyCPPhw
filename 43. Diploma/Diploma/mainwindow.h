#pragma once
#include <QMainWindow>
#include <QStandardItemModel>
#include "config.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onSearch();

private:
    void initDb();
    void loadWordsTab();
    void runSearch(const QStringList& tokens);

private:
    Ui::MainWindow* ui = nullptr;
    Config cfg_;
    QStandardItemModel* resultsModel_ = nullptr;
    QStandardItemModel* wordsModel_   = nullptr;
};
