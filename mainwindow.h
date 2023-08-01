
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <writer.h>
#include <book.h>

#include <QMainWindow>

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
