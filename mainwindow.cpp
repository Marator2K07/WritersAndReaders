
#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    Book *book = new Book();
    Writer *writer = new Writer(book, this);
    writer->completingWork();
}

MainWindow::~MainWindow()
{
}


