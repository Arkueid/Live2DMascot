/**
 * @file PieMenu.cpp
 * @author Simon Buchholz
 * @date 9 Jan 2023
 * @brief A simple pie menu QWidget implementation
 *
 * The pie menu is fully customizable for arbitrary amounts of buttons,
 * disabled buttons, button icons and appearance parameters.
 * All button clicks are published as a Qt signal containing the
 * clicked button's index.
 *
 * When the pin/unpin button is activated, the pie menu can be pinned
 * to keep it open until it is manually or programmatically closed.
 */

#include "PieMenu.h"

#include <QtGui/QPainter>
#include <QtWidgets/QApplication>
#include <QtGui/QMouseEvent>
#include <QtGui/QIcon>
#include <QtCore/QtMath>

#include <stdexcept>

PieMenu::PieMenu(QWidget *parent):
    QWidget(parent),
    default_button_icons{button_count, QIcon()},
    disabled_button_icons{button_count, QIcon()},
    pie_button_paths{button_count, QPainterPath()},
    buttons_enabled(button_count, true),
    base_size(pie_radius * 2, pie_radius * 2),
    full_size(base_size + QSize(stroke_width * 2, stroke_width * 2)),
    angle_per_button(360.0f / button_count),
    close_button_index(button_count + 1),
    pin_button_index(button_count + 2)
{
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::ToolTip);
    setAttribute(Qt::WA_TranslucentBackground);
    setMinimumSize(full_size);
    setMaximumSize(full_size);
    setMouseTracking(true);
    setWindowOpacity(0.9);
    hide();

    initPainterPaths();
}

void PieMenu::pop(QWidget *p) {
    resize(floor(p->width() * 0.8), floor(p->height() * 0.8));
    move(p->x() + (p->width()-width()) / 2, p->y() + (p->height()-height() )/ 2);
    show();
}

void PieMenu::initPainterPaths()
{
    for (uint8_t i = 0; i < button_count; i++)
    {
        QPainterPath path;

        qreal angle = angle_per_button * (i - 1) + base_angle;

        path.moveTo(full_size.width() / 2.0f, full_size.height() / 2.0f);
        path.arcTo(QRectF(stroke_width, stroke_width, full_size.width() - stroke_width * 2,
                          full_size.height() - stroke_width * 2), -angle, angle_per_button);

        pie_button_paths[i] = path;
    }
}

void PieMenu::display()
{
    auto geometry_adjusted = geometry();
    auto mapped_position = mapToParent(mapFromGlobal(QCursor::pos()));

    geometry_adjusted.setTopLeft(mapped_position - QPoint(full_size.width() / 2, full_size.height() / 2));
    setGeometry(geometry_adjusted);

    show();
    setFocus();
}

void PieMenu::hideIfNotPinned()
{
    if (!isPinned)
    {
        hide();
    }
}

void PieMenu::setButtonCount(uint8_t count)
{
    button_count = count;

    default_button_icons.resize(button_count);
    disabled_button_icons.resize(button_count);
    pie_button_paths.resize(button_count);

    close_button_index = button_count + 1;
    pin_button_index = button_count + 2;

    angle_per_button = 360.0f / button_count;

    buttons_enabled.resize(button_count, true);

    initPainterPaths();
}

void PieMenu::setBaseAngle(int32_t angle)
{
    base_angle = angle;
    initPainterPaths();
}

void PieMenu::setAlternateColors(bool value)
{
    alternate_colors = value;
}

void PieMenu::setStrokeWidth(int32_t value)
{
    stroke_width = value;
    full_size = QSize(base_size + QSize(stroke_width * 2, stroke_width * 2));

    setMinimumSize(full_size);
    setMaximumSize(full_size);
    initPainterPaths();
}

void PieMenu::setCloseButtonRadius(uint32_t radius)
{
    close_button_radius = radius;
}

void PieMenu::setPinButtonRadius(uint32_t radius)
{
    pin_button_radius = radius;
}

void PieMenu::setPieButtonIconSize(uint8_t size)
{
    pie_icon_size = size;
}

void PieMenu::setCloseButtonIconSize(uint8_t size)
{
    close_icon_size = size;
}

void PieMenu::setPinButtonIconSize(uint8_t size)
{
    pin_icon_size = size;
}

void PieMenu::setButtonEnabled(uint8_t index, bool enable)
{
    if (index < buttons_enabled.size())
    {
        buttons_enabled[index] = enable;
    }
    else
    {
        throw std::invalid_argument("Could not set pie menu button enable state");
    }
}

void PieMenu::setButtonIcon(uint8_t index, const QString& path)
{
    default_button_icons[index] = QIcon(path);

    // Create a semi-transparent disabled version
    auto img = QImage(path);
    img.convertTo(QImage::Format_ARGB32);
    img.fill(Qt::transparent);

    QPainter painter(&img);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.setOpacity(0.2);
    painter.drawImage(0, 0, QImage(path));

    disabled_button_icons[index] = QIcon(QPixmap::fromImage(img));
}

void PieMenu::setCloseButtonIcon(const QIcon& icon)
{
    close_icon = icon;
}

void PieMenu::setPinButtonIcon(const QIcon& icon)
{
    pin_icon = icon;
}

void PieMenu::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    const auto button_under_mouse = getButtonUnderMouse();

    paintPieButtons(painter, button_under_mouse);

    paintCloseButton(painter, (button_under_mouse == close_button_index));

    if (show_pin_button)
    {
        paintPinButton(painter, (button_under_mouse == pin_button_index));
    }
}

void PieMenu::paintPieButtons(QPainter& painter, int8_t mouseover)
{
    for (int8_t i = 0; i < button_count; i++)
    {
        if (mouseover == i)
        {
            if (alternate_colors && (i % 2 == 0))
            {
                painter.fillPath(pie_button_paths[i], QBrush(QApplication::mouseButtons() == Qt::LeftButton ? QColor(200, 200, 200) : QColor(220, 220, 220)));
            }
            else
            {
                painter.fillPath(pie_button_paths[i], QBrush(QApplication::mouseButtons() == Qt::LeftButton ? QColor(220, 220, 220) : QColor(230, 230, 230)));
            }
        }
        else
        {
            if (alternate_colors && (i % 2 == 0))
            {
                painter.fillPath(pie_button_paths[i], QBrush(QColor(242, 242, 242)));
            }
            else
            {
                painter.fillPath(pie_button_paths[i], QBrush(QColor(250, 250, 250)));
            }
        }
    }

    for (int8_t i = 0; i < button_count; i++)
    {
        painter.strokePath(pie_button_paths[i], QPen(QColor(200, 200, 200), stroke_width));
    }

    for (int8_t i = 0; i < button_count; i++)
    {
        qreal angle = angle_per_button * i + base_angle - angle_per_button * 0.5f;

        QPoint reference_point = QPoint(pie_radius * qCos((angle * M_PI) / 180) + pie_radius,
                                        pie_radius * qSin((angle * M_PI) / 180) + pie_radius);

        int8_t index = i + 1;

        if (index >= button_count)
        {
            index = 0;
        }

        painter.drawPixmap(QRect((reference_point.x() * 2 + full_size.width() / 2) / 3 - pie_icon_size / 2 + stroke_width, (reference_point.y() * 2 + full_size.height() / 2) / 3 - pie_icon_size / 2 + stroke_width, pie_icon_size, pie_icon_size),
                           buttons_enabled[index] ? default_button_icons[index].pixmap(pie_icon_size, pie_icon_size) : disabled_button_icons[index].pixmap(pie_icon_size, pie_icon_size));
    }
}

void PieMenu::paintCloseButton(QPainter& painter, bool mouseover)
{
    painter.setPen(QPen(QColor(200, 200, 200), stroke_width));

    if (mouseover)
    {
        painter.setBrush(QBrush(QApplication::mouseButtons() == Qt::LeftButton ? QColor(220, 220, 220) : QColor(230, 230, 230)));
    }
    else
    {
        painter.setBrush(QBrush(QColor(242, 242, 242)));
    }

    painter.drawEllipse(QRectF(pie_radius - close_button_radius + stroke_width, pie_radius - close_button_radius + stroke_width,
                               close_button_radius * 2, close_button_radius * 2));

    painter.drawPixmap(QRect(full_size.width() / 2 - close_icon_size / 2, full_size.height() / 2 - close_icon_size / 2,
                             close_icon_size, close_icon_size), close_icon.pixmap(close_icon_size, close_icon_size));
}

void PieMenu::paintPinButton(QPainter& painter, bool mouseover)
{
    painter.setPen(QPen(isPinned ? QColor(150, 150, 150) : QColor(200, 200, 200), stroke_width));

    if (mouseover)
    {
        if (QApplication::mouseButtons() == Qt::LeftButton)
        {
            painter.setBrush(isPinned ? QColor(190, 190, 190) : QBrush(QColor(200, 200, 200)));
        }
        else
        {
            painter.setBrush(isPinned ? QColor(190, 190, 190) : QBrush(QColor(220, 220, 220)));
        }
    }
    else
    {
        painter.setBrush(QBrush(isPinned ? QColor(190, 190, 190) : QColor(255, 255, 255)));
    }

    painter.drawEllipse(QRectF(base_size.width() - pin_button_radius * 2, stroke_width, pin_button_radius * 2, pin_button_radius * 2));

    painter.drawPixmap(QRect(base_size.width() - pin_button_radius - pin_icon_size / 2, stroke_width + pin_button_radius - pin_icon_size / 2,
                             pin_icon_size, pin_icon_size), pin_icon.pixmap(pin_icon_size, pin_icon_size));
}

int8_t PieMenu::getButtonUnderMouse() const
{
    const auto cursor = mapFromGlobal(QCursor::pos());

    if (cursor.x() <= 0 || cursor.x() >= full_size.width() || cursor.y() < 0 || cursor.y() >= full_size.height()) // out of widget
    {
        return -1;
    }

    int8_t nearest = -1;
    uint32_t distance = UINT_MAX;
    for (size_t i = 0; i < button_count; i++)
    {
        qreal angle = angle_per_button * i + base_angle - angle_per_button * 0.5f;

        QPoint reference_point = QPoint(pie_radius * qCos((angle * M_PI) / 180) + pie_radius,
                                        pie_radius * qSin((angle * M_PI) / 180) + pie_radius);

        uint32_t current = sqrt(std::pow(reference_point.x() - cursor.x(), 2) + std::pow(reference_point.y() - cursor.y(), 2));

        if (current < distance)
        {
            distance = current;

            int index = i + 1;

            if (index >= button_count)
            {
                index = 0;
            }

            nearest = index;
        }
    }

    qreal distance_to_pin_button = sqrt(std::pow(base_size.width() - pin_button_radius - cursor.x(), 2) + std::pow(pin_button_radius - cursor.y(), 2));

    if (distance_to_pin_button < pin_button_radius + stroke_width)
    {
        return pin_button_index;
    }

    qreal distance_to_close_button = sqrt(std::pow(base_size.width() / 2 - cursor.x(), 2) + std::pow(base_size.height() / 2 - cursor.y(), 2));

    if (distance_to_close_button < close_button_radius + stroke_width)
    {
        return close_button_index;
    }

    return nearest;
}

void PieMenu::mouseReleaseEvent(QMouseEvent *event)
{
    update();

    if (event->button() == Qt::LeftButton)
    {
        const auto button_under_mouse = getButtonUnderMouse();

        if (button_under_mouse == pin_button_index)
        {
            isPinned = !isPinned;
        }
        else if (button_under_mouse == close_button_index)
        {
            hide();
        }
        else if (button_under_mouse >= 0 && button_under_mouse < button_count && buttons_enabled[button_under_mouse])
        {
            emit buttonClicked(button_under_mouse);

            // //////////////////////////////////////////////////////////////////////
            // Edit this part to not close the menu when clicking on specific buttons
            // //////////////////////////////////////////////////////////////////////

            if (!isPinned)
            {
                hide();
            }
        }
        QWidget::mouseReleaseEvent(event);
    }
    else
    {
        event->accept();
    }
}

void PieMenu::mousePressEvent(QMouseEvent *event)
{
    update();
    QWidget::mousePressEvent(event);
}

void PieMenu::mouseMoveEvent(QMouseEvent *event)
{
    update();
    QWidget::mouseMoveEvent(event);
}

void PieMenu::leaveEvent(QEvent *event)
{
    update();
    QWidget::leaveEvent(event);
}
