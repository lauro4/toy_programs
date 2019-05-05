#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextBrowser>
#include "OmdbApi.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonDoSearch_released();

private:
    Ui::MainWindow *ui;
    OmdbApi *omdbApi;
    void displayResults();
};

#endif // MAINWINDOW_H
