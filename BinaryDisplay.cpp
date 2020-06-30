#include "BinaryDisplay.h"

BinaryDisplay::BinaryDisplay(int x_, int y_, int width, int height)
{
	x = x_;
	y = y_;
	w = width;
	h = height;
	set_object_type(Object::BINARYDISPLAY);
}

void
BinaryDisplay::update_state()
{
	puts("update binary display");
}
