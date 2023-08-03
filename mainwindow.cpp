
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // инициализация центрального виджета и его установка в класс
    // также инициализация виджета компоновщика
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    baseLayout = new QVBoxLayout(centralWidget);
    // инициализация остальных атрибутов класса
    book = new Book();
    manager = new WritersManager(2, book, this);
    field = new QTextEdit("OLOLO", this);
    field->setReadOnly(true);
    field->setStyleSheet("QTextEdit {background-color : black;"
                         "color : mediumspringgreen}");
    // layout settings
    baseLayout->addWidget(field);
    baseLayout->addWidget(manager);

    resize(600,500);
}

MainWindow::~MainWindow()
{
}


