
#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    book = new Book();
    manager = new WritersManager(2, book, this);

    field = new QTextEdit("OLOLO", this);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(field);
    this->setLayout(layout);
}

MainWindow::~MainWindow()
{
}


