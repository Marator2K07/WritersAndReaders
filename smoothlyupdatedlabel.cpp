
#include "smoothlyupdatedlabel.h"

SmoothlyUpdatedLabel::SmoothlyUpdatedLabel(QWidget *parent)
    : QLabel(parent)
    , currentIndex{0}
    , currentText{""}
    , delay{18}
{
    // стилистика самого виджета надписи
    setAlignment(Qt::AlignCenter);
    setStyleSheet(("SmoothlyUpdatedLabel {background-color : black;"
                   "color : mediumspringgreen}"));
    setFixedHeight(33);
    // инициализация атрибута таймера таймера
    timer.setInterval(delay);
    connect(&timer, SIGNAL(timeout()), this, SLOT(updateSymbol()));
}

void SmoothlyUpdatedLabel::updateSymbol()
{
    // либо стираем старый, либо пишем новый текст
    if (cleaning) {
        currentText.truncate(currentIndex--);
    }
    else {
        currentText = finalText.left(currentIndex++);
    }
    setText(currentText);
    // если была очистка и стало чисто, то теперь вводим новое слово
    // иначе останавливаем таймер после ввода нового слова
    if (currentIndex > finalText.size() && !cleaning) {
        timer.stop();
    }
    else if (currentIndex <= 0 && cleaning) {
        cleaning = false;
    }
}

void SmoothlyUpdatedLabel::changeText(QString text)
{
    if (timer.isActive()) {
        timer.stop();
    }
    // начальные приготовления при запуске таймера
    finalText = text;
    currentIndex = currentText.size();
    // если в виджете уже чтото написано, то сначала стираем текст,
    // иначе сразу пишем новый
    if (currentIndex > 0) {
        cleaning = true;
    }
    else {
        cleaning = false;
    }

    timer.start();

}

