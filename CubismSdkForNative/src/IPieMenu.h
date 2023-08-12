#pragma once
#include <QtWidgets/qwidget.h>

class IPieMenu 
{
public:
    virtual QWidget* GetSelf() = 0;
    
    virtual void hideIfNotPinned() = 0;

    virtual void display() = 0;

    virtual void setButtonIcon(uint8_t index, const QString& path) = 0;

    virtual void setCloseButtonIcon(const QIcon& icon) = 0;

    virtual void setPinButtonIcon(const QIcon& icon) = 0;

    virtual void setButtonEnabled(uint8_t index, bool enable) = 0;

    virtual void setButtonCount(uint8_t count) = 0;

    virtual void setBaseAngle(int32_t angle) = 0;

    virtual void setAlternateColors(bool value) = 0;

    virtual void setStrokeWidth(int32_t value) = 0;

    virtual void setCloseButtonRadius(uint32_t radius) = 0;

    virtual void setPinButtonRadius(uint32_t radius) = 0;

    virtual void setPieButtonIconSize(uint8_t size) = 0;

    virtual void setCloseButtonIconSize(uint8_t size) = 0;

    virtual void setPinButtonIconSize(uint8_t size) = 0;

    virtual void pop(QWidget* p) = 0;
};