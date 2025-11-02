#ifndef WIDGET_H
#define WIDGET_H

#include "../../external/raylib.h"
#include <stdbool.h>

// WidgetProp object
// Core/Parent of all Widgets
typedef struct {
	// is the widget active/visible
    bool active;
	// the actual bounds of the widget
    Rectangle bounds;
} WidgetProp;

// Create a new WidgetProp
// by default will be hidden, and bounds will be zeroed.
// return `WidgetProp`
WidgetProp NewWidgetProp();

// Create a new WidgetProp with given bounds;
// `rect` => The bounds for the widget
// return `WidgetProp`
WidgetProp NewWidgetPropRec(Rectangle rect);

// Toggle visibility of widget
// `prop` => The target `prop`
// `active` => Toggle the visibility of the widget
void ActivateWidget(WidgetProp *prop, bool active);

// Set Widget's bounds
// `prop` => The target `prop`
// `rect` => New bounds
void SetWidgetBounds(WidgetProp *prop, Rectangle rect);

#endif
