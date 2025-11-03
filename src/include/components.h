#ifndef COMPONENETS_H
#define COMPONENETS_H

#include "../external/raygui.h"
#include "../external/raylib.h"
#include "../include/drawtools.h"
#include "../include/frames.h"
#include "../include/utils.h"
#include <stdbool.h>

// Sub-window Title Bar height
#define WINDOW_TOP_HEIGHT 30

// Input integer with slider on click
// `bounds` = Bounds of the input box only
// `value` = Value
// `min` = Minimum value
// `max` = Maximum value
// `unit` = Unit to show after the value
// `clicked` = The input box is clicked
// `hover` = Is the Input slider is being hovered
// Return => Not used
bool BpInputSliderInt(
    Rectangle bounds, int *value, int min, int max, const char *unit,
    bool *clicked, bool *hover
);

// Integer input slider
// `bounds` = Bounds of the slider
// `value` = Value
// `min` = Minimum Value
// `max` = Maximum Value
// `unit` = Unit to show after the value
// `sliderHover` = Slider is being hovered
// Return => Not used
bool BpSliderInt(
    Rectangle bounds, int *value, int min, int max, const char *unit,
    bool *sliderHover
);

// Simple scollpanel used for layerbar
// `bounds` = Bounds to where the scrollbars
// `scroll` = Scroll vector
// `view` = usedArea for scrollpanel
// `hDrag` = Is the horizontal scroll thumb is being dragged
// `vDrag` = Is the vertical scroll thumb is being dragged
// Return => Not used
bool BpScrollPanel(
    Rectangle bounds, Rectangle content, Vector2 *scroll, Rectangle view,
    bool *hDrag, bool *vDrag, const Rectangle *zone
);
// Simple Themed Text Input Box
// `bounds` = Bounds for the text input box
// `text` = Pointer to text which gets modified
// `textSize` = Maximum Text size
// `edit` = Is the text being edited
// Return => Not used
bool BpTextBox(Rectangle bounds, char *text, int textSize, bool *edit);

// Simple Themed Dropdown Menu
// `bounds` = Bounds for the dropdown menu
// `options` = ';' Semicolon separeated items list in a single string
// `selected` = Index of the selected items index from options being split by ;
// `clicked` = If the dropdown menu is clicked = opened
// Return => Not used
bool BpDropdownBox(
    Rectangle bounds, const char *options, int *selected, bool *clicked
);

// Draw Tool Button
// `bounds` = Bounds for the base button 
// `active` = Current active draw tool
// `showOther` = Are other/sub-tool buttons are visible
// `num` = Number of total buttons (including sub buttons)
// `tools` = List of draw tools this button contains. First item is base.
// `icons` = Icon texture (it contains all draw tool icons 16x16)
// Return => Not used
bool BpDToolButton(
    Rectangle bounds, DrawTool *active, bool *showOther, int num,
    const DrawTool *tools, Texture2D *icons
);

// Simple Themed Toggle Button
// `bounds` = Bounds for button
// `thick` = Border thickness
// `active` = Is the button active
// Returns => `true` on clicked otherwise `false`
bool BpToggleButton(Rectangle bounds, int thick, bool active);
// Simple Text Button With Borders
// `bounds` = Bounds for button
// `text` = Label for the button
// Returns => `true` on clicked otherwise `false`
bool BpTextButton(Rectangle bounds, const char *text);
// Simple Themed Label Button
// `bounds` = Bounds for button
// `text` = Label for the button
// Returns => `true` on clicked otherwise `false`
bool BpLabelButton(Rectangle bounds, const char *text);

// Special Buttons for usage in Menubar
// `bounds` = Bounds for button
// `thick` = Border thickness
// `active` = Is the menu button active
// Returns => `true` on clicked otherwise `false`
bool BpTopMenuBtn(Rectangle bounds, const char *text, bool active);

// Tab Item Button
// `bounds` = Bounds for whole tab panel
// `name` = Name of the tab
// `index` = Index of the tab, used to calculate the position of the button in 
// tab panel
// `index` = Width of the tab button, used to calculate size and position of the
// button in tab panel
// `active` = Is the tab active tab
// Returns => `0` if the tab is clicked (means open this tab), `1` is `x` button
// is clicked (means close the tab).
// TODO: Use Enums for the return
int BpTabItem(
    Rectangle bounds, const char *name, int index, float tabWidth, bool active
);

// Brush Shape Toggle Button
// `bounds` = Bounds of the button
// `shape` = Brush shape (modifies the value) (currently only square or circle)
// `icons` = Icon Texture (currently uses DrawToolBar icons)
// FUTURE: Custom Brush | Using DToolInfo as for prototype
void BpBrushShapeButton(Rectangle bounds, BrushShape *shape, Texture2D *icons);

// Simple Transparent Border Only Panel 
// `bounds` = Bounds for the panel
// `thick` = Border thickness
void BpPanelOnlyBorder(Rectangle bounds, int thick);
// Simple Transparent Border Only Panel with Custom Color
// `bounds` = Bounds of the panel
// `thick` = Border thickness
// `color` = Border color
void BpPanelOnlyClrBorder(Rectangle bounds, int thick, Color color);
// Simple Themed Bordered Panel
// `bounds` = Bounds for the panel
// `thick` = Border thickness
void BpPanelBorder(Rectangle bounds, int thick);

// Opaque Panel with No Borders
// `bounds` = Bounds for the panel
void BpPanelNoBorder(Rectangle bounds);

// Base Panel for all Panel Variants
// `bounds` = Bounds for the panel
// `thick` =  Border thickness 
// `border` = Which sides have borders
// `shadow` = Which sides have shadows
void BpSimplePanel(Rectangle bounds, int thick, Side border, Side shadow);
// Menu Panel for Context menus (not used)
// `position` = The Top Left corner position
// `width` = Width for the panel
// `items` = How many vertical items to make space for 
// (each item will get height of the font's base size)
// `roundness` = [Left for future use]
void BpMenuBarPanel(Vector2 position, float width, int items, float roundness);
// Draw Frame Item in Layer bar list
// `bounds` = Bounds for the box to draw
// `frame` = Frame object
// `preview` = If frame preview should be shown [Not yet implemented]
// Returns => `true` if clicked else `false`
bool BpFramePrevBox(Rectangle bounds, FrameObj *frame, bool preview);
// Draw Frame Item box for active frame in Layer bar list
// `bounds` = Bounds for the box to draw
// `frame` = Frame object
// `preview` = If frame preview should be shown [Not yet implemented]
// Returns => `true` if clicked else `false`
bool BpFramePrevActive(Rectangle bounds, FrameObj *frame, bool preview);

// Simple Sub Windows
// `bounds` = Bounds of the windows (will be modified if dragged)
// `title` = Title of the window
// `drag` = Is the window being dragged
// `resize` = Is the window resize handle being dragged
// Returns => `true` if `x` is clicked otherwise returns `false`
bool BpSimpleWindow(
    Rectangle *bounds, const char *title, bool *drag, bool *resize
);
#endif
