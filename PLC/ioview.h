#ifndef IOVIEW_H
#define IOVIEW_H

#include "InputOutput/iobase.h"

// кликабельная область контакта контроллера

class IOView: public IOBase
{
    int x; // left top corner
    int y;
    int width;
    int height;
    bool text_before_flag;  // вывод текста с описанием функции контакта перед кликабельной областью (или после)
public:
    IOView() = default;
    int getX() const {return x;}
    int getY() const {return y;}
    int getWidth() const {return width;}
    int getHeight() const {return height;}
    void setX(int value) {x = value;}
    void setY(int value) {y = value;}
    void setWidth(int value) {width = value;}
    void setHeight(int value) {height = value;}
    bool isTextBefore() const {return text_before_flag;}
    void setTextBeforeFlag(bool value) {text_before_flag = value;}
};

#endif // IOVIEW_H
