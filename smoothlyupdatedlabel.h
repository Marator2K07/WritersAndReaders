
#ifndef SMOOTHLYUPDATEDLABEL_H
#define SMOOTHLYUPDATEDLABEL_H

#include <QLabel>
#include <QWidget>
#include <QTimer>

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
    const QString textColor; // случайный цвет в наследство от Писателя для
    // более наглядного восприятия работы виджета вместе с другими

public:
    explicit SmoothlyUpdatedLabel(Qt::Alignment align,
                                  short delay,
                                  QString textColor,
                                  QWidget *parent);

private slots:
    void updateSymbol();

public slots:
    void changeText(QString text);
};

#endif // SMOOTHLYUPDATEDLABEL_H
