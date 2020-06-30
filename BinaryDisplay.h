#ifndef BINARYDISPLAY_H
#define BINARYDISPLAY_H

#include <stdio.h>
#include "Object.h"

class BinaryDisplay : public Object
{
	public:
		BinaryDisplay(int x, int y, int width, int height);
		~BinaryDisplay() override = default;
		void update_state() override;

		Object *get_input0() { return input0; };
		Object *get_input1() { return input1; };
		Object *get_input2() { return input2; };
		Object *get_input3() { return input3; };
		Object *get_input4() { return input4; };
		Object *get_input5() { return input5; };
		Object *get_input6() { return input6; };
		Object *get_input7() { return input7; };

		int get_sum_value() { return sum_value; };
	
	private:
		int sum_value = 0;
		Object *input0 = nullptr;
		Object *input1 = nullptr;
		Object *input2 = nullptr;
		Object *input3 = nullptr;
		Object *input4 = nullptr;
		Object *input5 = nullptr;
		Object *input6 = nullptr;
		Object *input7 = nullptr;
};

#endif
