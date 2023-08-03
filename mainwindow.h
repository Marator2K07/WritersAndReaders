
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "writersmanager.h"

#include <QMainWindow>
#include <QTextEdit>
#include <QVBoxLayout>

class MainWindow : public QMainWindow

{
    Q_OBJECT
private:
    QWidget *centralWidget;
    QVBoxLayout *baseLayout;
    Book *book;
    WritersManager *manager;
    QTextEdit *field;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
