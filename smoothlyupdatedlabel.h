
#ifndef SMOOTHLYUPDATEDLABEL_H
#define SMOOTHLYUPDATEDLABEL_H

#include <QLabel>
#include <QWidget>
#include <QTimer>
#include <QColor>

class SmoothlyUpdatedLabel : public QLabel
{
    Q_OBJECT
private:
    short currentIndex; // индекс текущего символа
    short delay; // посимвольная задержка вывода
    bool cleaning; // нужно ли сначала чистить текст, перед вводом нового
    QTimer timer;
    QString currentText; // текущий текст
    QString finalText; // текст, который в итоге должен быть отображен

public:
    SmoothlyUpdatedLabel(QWidget* parent = nullptr);

private slots:
    void updateSymbol();

public slots:
    void changeText(QString text);
};

#endif // SMOOTHLYUPDATEDLABEL_H
