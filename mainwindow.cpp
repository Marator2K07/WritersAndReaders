
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
    writersManager = new WritersManager(2, book, this); // первый передаваемый параметр - количество писателей
    readersManager = new ReadersManager(3, book, this); // первый передаваемый параметр - количество читателей    
    connect(writersManager, SIGNAL(anyoneStarted()), readersManager, SLOT(stopReading()));
    connect(writersManager, SIGNAL(allFinished()), readersManager, SLOT(startReading()));
    field = new AutoScrollableTextEdit(this);
    field->setReadOnly(true);
    field->setStyleSheet("QTextEdit {background-color : black;"
                         "color : white}");
    // каждый писатель теперь может показывать какое слово он дописал в книгу
    // через виджет текствого поля
    foreach (Writer *writer, writersManager->getWriters()) {
        connect(writer, SIGNAL(writeWord(QString)), field, SLOT(insertHtmlWithAutoScroll(QString)));
    }
    // заполняем поле текста книги
    QList<QString> bookLines = book->getText();
    foreach (QString line, bookLines) {
        field->append(line);
    }
    // layout settings
    baseLayout->addWidget(field);
    baseLayout->addWidget(writersManager);
    baseLayout->addWidget(readersManager);

    resize(760,650);
}

MainWindow::~MainWindow()
{
}


