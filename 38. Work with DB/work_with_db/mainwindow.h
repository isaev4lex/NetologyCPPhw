#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQueryModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onReload();
    void onClear();

private:
    bool ensureConnection();
    void showAllWithTableModel();
    void showCategory(const QString& name);
    void applyTwoColumnsOnly();
    void setEmptyModel();

private:
    Ui::MainWindow *ui;

    QSqlDatabase   m_db;
    QSqlTableModel *m_tableModel = nullptr;
    QSqlQueryModel *m_queryModel = nullptr;
};

#endif // MAINWINDOW_H
