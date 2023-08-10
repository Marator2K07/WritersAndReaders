
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
    QWaitCondition *waitCondition = new QWaitCondition;
    QMutex *waitConditionLocker = new QMutex;
    writersManager = new WritersManager(3, book, waitCondition, this); // первый передаваемый параметр - свое количество писателей
    readersManager = new ReadersManager(4, book, waitCondition, waitConditionLocker, this); // первый передаваемый параметр - свое количество читателей
    field = new AutoScrollableTextEdit(this);
    field->setReadOnly(true);
    field->setStyleSheet("QTextEdit {background-color : black;"
                         "color : white}");
    // очистка поля от лишнего текста ради оптимизации
    connect(book, SIGNAL(clearText()), field, SLOT(clear()));
    // подгрузка уже обновленного текста книги
    connect(book, SIGNAL(updateText(QList<QString>)), field, SLOT(updateText(QList<QString>)));
    // каждый писатель теперь может показывать какое слово он дописал в книгу через виджет текствого поля
    foreach (Writer *writer, writersManager->getWriters()) {
        connect(writer, SIGNAL(writeWord(QString)), field, SLOT(insertHtmlWithAutoScroll(QString)));
    }
    // каждый читатель в состоянии узнать, активен ли хоть один писатель в данный момент времени
    foreach (Reader *reader, readersManager->getReaders()) {
        connect(reader, SIGNAL(checkWritersActivity(bool*)), writersManager, SLOT(writersActivityAnalysis(bool*)));
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


