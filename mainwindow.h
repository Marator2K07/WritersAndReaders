
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "writersmanager.h"
#include "readersmanager.h"
#include "autoscrollabletextedit.h"

#include <QMainWindow>
#include <QVBoxLayout>

class MainWindow : public QMainWindow

{
    Q_OBJECT
private:
    QWidget *centralWidget;
    QVBoxLayout *baseLayout;
    Book *book;
    WritersManager *writersManager;
    ReadersManager *readersManager;
    AutoScrollableTextEdit *field;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
