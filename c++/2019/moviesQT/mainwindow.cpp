#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream> ////
using namespace std; ////

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->ui->textBrowser->hide();
    this->omdbApi = new(OmdbApi);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete this->omdbApi;
}

void MainWindow::on_pushButtonDoSearch_released()
{
    this->ui->textBrowser->clear();
    this->ui->textBrowser->hide();
    //ideally should allow a search "bySearch (REST OP s), list many titles mathcing the search and allowing to click each result for full details
    //then run the searchTitle for the result clicked...
    if (! this->omdbApi->searchMediaByTitle(this->ui->lineEditSearchTitle->text().toStdString())) {
        this->ui->statusBar->showMessage("Something went wrong and we didn't receive a valid response from the server");
        //error getting a response
        return;
    }

    if (omdbApi->getLastResult("Response") == "False") {
        this->ui->statusBar->showMessage(QString::fromStdString(this->omdbApi->getLastResult("Error")));
        //error getting a search result
        return;
    }
    //a serch result to format into a table
    cout << this->omdbApi->dumpLastSearchResults() << endl;
    this->displayResults();
}

void MainWindow::displayResults() {
    this->ui->textBrowser->show();
    //a nice table on a pop up window would be nice, this is just a dump o the received json...
    this->ui->textBrowser->setText(QString::fromStdString(this->omdbApi->dumpLastSearchResults()));
}

