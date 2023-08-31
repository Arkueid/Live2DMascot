/**
 * @file PieMenu.h
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

#ifndef PIEMENU_H
#define PIEMENU_H
#include "../interface/IPieMenu.h"
#include <QtWidgets/QWidget>
#include <QtGui/QPainterPath>
#include <QtGui/QIcon>

/// \brief A simple pie menu widget for Qt
class PieMenu : public QWidget, public IPieMenu
{
    Q_OBJECT
public:
    QWidget* GetSelf() { return this; }
    /// \brief Constructor of the PieMenu widget
    /// \param parent: Pointer to the parent widget
    explicit PieMenu(QWidget *parent = nullptr);

    /// \brief Hides the pie menu if it is not pinned
    void hideIfNotPinned();

    /// \brief Displays the pie menu at the current mouse position
    /// Note: the position is mapped to the parent coordinate system
    void display();

    /// \brief Sets the icon image of the pie button with the given index
    /// \param index: The index of the button
    /// \param path: Reference to the path of the icon file
    void setButtonIcon(uint8_t index, const QString& path);

    /// \brief Sets the icon of the close button
    /// \param icon: Reference to the icon
    void setCloseButtonIcon(const QIcon& icon);

    /// \brief Sets the icon of the pin button
    /// \param icon: Reference to the icon
    void setPinButtonIcon(const QIcon& icon);

    /// \brief Sets the enabled state of the button with the given index
    /// \param index: The index of the button to be altered
    /// \param enable: Whether the button should be enabled or disabled
    void setButtonEnabled(uint8_t index, bool enable);

    /// \brief Sets the amount of pie buttons and updates dependent parameters
    /// \param count: The new amount of pie buttons
    void setButtonCount(uint8_t count);

    /// \brief Sets the base angle of the pie buttons and updates dependent parameters
    /// \param angle: The new base angle
    void setBaseAngle(int32_t angle);

    /// \brief Setter for the alternate button color property
    /// \param value: The new state of the alternate button color property
    void setAlternateColors(bool value);

    /// \brief Sets the stroke width of all buttons and updates dependent parameters
    /// \param value: The new stroke width
    void setStrokeWidth(int32_t value);

    /// \brief Sets the radius of the close button in the widget center
    /// \param radius: The new close button radius
    void setCloseButtonRadius(uint32_t radius);

    /// \brief Sets the radius of the pin/unpin button in the top right corner
    /// \param radius: The new pin/unpin button radius
    void setPinButtonRadius(uint32_t radius);

    /// \brief Sets the size of the pie button icons
    /// \param size: The new icon size in pixels
    void setPieButtonIconSize(uint8_t size);

    /// \brief Sets the size of the close button icon
    /// \param size: The new icon size in pixels
    void setCloseButtonIconSize(uint8_t size);

    /// \brief Sets the size of the pin/unpin button icon
    /// \param size: The new icon size in pixels
    void setPinButtonIconSize(uint8_t size);

    void pop(QWidget* p);

signals:
    /// \brief Emitted when one of the pie menu buttons is clicked
    /// \param index: The index of the clicked button
    void buttonClicked(uint8_t index);

protected:
    /// \brief Creates QPainterPath objects for the pie button shapes
    void initPainterPaths();

    /// \brief Calculates the index of the button that the mouse is over
    /// The buttons are numbered from 0 to n, index n+1 is the close button
    /// and index n+2 is the pin/unpin button
    /// \return The button index or -1, if not on a button
    int8_t getButtonUnderMouse(void) const;

    /// \brief Event handler to paint the widget
    /// \param event: Pointer to the paint event
    void paintEvent(QPaintEvent *event) override;

    /// \brief Paints the custom-shaped pie menu buttons
    /// \param painter: Reference to the QPainter
    /// \param mouseover: The index of the button that the mouse is over
    void paintPieButtons(QPainter& painter, int8_t mouseover);

    /// \brief Paints the close button in the center of the pie menu
    /// \param painter: Reference to the QPainter
    /// \param mouseover: Whether the mouse is over the button or not
    void paintCloseButton(QPainter& painter, bool mouseover);

    /// \brief Paints the pin/unpin button in the top right corner
    /// \param painter: Reference to the QPainter
    /// \param mouseover: Whether the mouse is over the button or not
    void paintPinButton(QPainter& painter, bool mouseover);

    /// \brief Event handler to process mouse clicks
    /// \param event: Pointer to the mouse event
    void mouseReleaseEvent(QMouseEvent *event) override;

    /// \brief Event handler to update on mouse press
    /// \param event: Pointer to the mouse event
    void mousePressEvent(QMouseEvent *event) override;

    /// \brief Event handler to update on mouse move
    /// \param event: Pointer to the mouse event
    void mouseMoveEvent(QMouseEvent *event) override;

    /// \brief Event handler to update when the mouse leaves the widget
    /// \param event: Pointer to the mouse event
    void leaveEvent(QEvent *event) override;

protected:
    /// \brief The amount of pie buttons the pie menu will have
    uint8_t button_count = 4;

    /// \brief Vector containing the default icons for the pie buttons
    std::vector<QIcon> default_button_icons;

    /// \brief Vector containing the icons for the disabled pie buttons
    std::vector<QIcon> disabled_button_icons;

    /// \brief Icon for the close icon in the center of the pie menu
    QIcon close_icon;

    /// \brief Icon for the pin/unpin icon
    QIcon pin_icon;

    /// \brief Vector containing the painter paths of the custom pie menu button shapes
    std::vector<QPainterPath> pie_button_paths;

    /// \brief Vector containing the button enable state of the pie menu buttons
    std::vector<bool> buttons_enabled;

    /// \brief Whether there should be a pin/unpin button
    bool show_pin_button = true;

    /// \brief Whether the pie menu is currently pinned or not
    bool isPinned = false;

    /// \brief The stroke width of all buttons
    uint8_t stroke_width = 1;

    /// \brief The radius of the pie menu
    uint32_t pie_radius = 100;

    /// \brief The size of the widget excluding border strokes
    QSize base_size;

    /// \brief The size of the widget including border strokes
    QSize full_size;

    /// \brief The angle of each pie button in degrees
    qreal angle_per_button;

    /// \brief The base angle (constant offset) of the pie buttons
    qreal base_angle = 45;

    /// \brief The radius of the close button in pixels
    uint8_t close_button_radius = 35;

    /// \brief The button index of the close button
    uint8_t close_button_index;

    /// \brief The radius of the pin/unpin button in pixels
    uint8_t pin_button_radius = 13;

    /// \brief The button index of the pin/unpin button
    uint8_t pin_button_index;

    /// \brief Whether the button colors should alternate or not
    bool alternate_colors = true;

    /// \brief The size of the pie menu button icons in pixels
    uint8_t pie_icon_size = 20;

    /// \brief The size of the close button icon in pixels
    uint8_t close_icon_size = 20;

    /// \brief The size of the pin/unpin button icon in pixels
    uint8_t pin_icon_size = 12;
};

#endif // PIEMENU_H
