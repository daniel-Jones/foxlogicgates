#include "BinaryDisplay.h"

BinaryDisplay::BinaryDisplay(int x_, int y_, int width, int height, int loaded_id)
{
	/* special handing of id - if the object is loaded from file the loaded_id will be set and we use that */
	if (loaded_id != -1)
		id = loaded_id;
	else
		id = Object::object_id_counter++; // increment counter after assigning
	x = x_;
	y = y_;
	w = width;
	h = height;
	set_object_type(Object::BINARYDISPLAY);
}

void
BinaryDisplay::update_state()
{
	sum_value = 0;
	if (input0 && input0->get_object_type() == Object::GATE && input0->get_output_state())
		sum_value |= 1 << 0;
	if (input1 && input1->get_object_type() == Object::GATE && input1->get_output_state())
		sum_value |= 1 << 1;
	if (input2 && input2->get_object_type() == Object::GATE && input2->get_output_state())
		sum_value |= 1 << 2;
	if (input3 && input3->get_object_type() == Object::GATE && input3->get_output_state())
		sum_value |= 1 << 3;
	if (input4 && input4->get_object_type() == Object::GATE && input4->get_output_state())
		sum_value |= 1 << 4;
	if (input5 && input5->get_object_type() == Object::GATE && input5->get_output_state())
		sum_value |= 1 << 5;
	if (input6 && input6->get_object_type() == Object::GATE && input6->get_output_state())
		sum_value |= 1 << 6;
	if (input7 && input7->get_object_type() == Object::GATE && input7->get_output_state())
		sum_value |= 1 << 7;
	if (sum_value > 0)
		output_state = true;
	else
		output_state = false;
}

void
BinaryDisplay::remove_input_object(int id)
{
	if (input0 && input0->get_id() == id)
		input0 = nullptr;
	if (input1 && input1->get_id() == id)
		input1 = nullptr;
	if (input2 && input2->get_id() == id)
		input2 = nullptr;
	if (input3 && input3->get_id() == id)
		input3 = nullptr;
	if (input4 && input4->get_id() == id)
		input4 = nullptr;
	if (input5 && input5->get_id() == id)
		input5 = nullptr;
	if (input6 && input6->get_id() == id)
		input6 = nullptr;
	if (input7 && input7->get_id() == id)
		input7 = nullptr;
	update_state();
}
