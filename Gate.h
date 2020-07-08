/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GATE_H
#define GATE_H

#include <string>
#include <vector>
#include "Object.h"

class Gate : public Object
{
	public:
		enum GATE_TYPE
		{
			NONE = 0,
			INPUT,
			OUTPUT,
			AND,
			OR,
			NOT,
			NAND,
			NOR,
			XOR,
			XNOR,
			NAND3,
		};

		Gate(GATE_TYPE type = INPUT, int x = 0, int y = 0, int width = 70, int height = 50, int id = -1);
		~Gate();

		std::vector<int> *get_output_objects() { return &this->output_object_ids; };
		GATE_TYPE get_gate_type() { return this->gate_type; };
		Object *get_input_gate1() { return this->input_gate1; };
		Object *get_input_gate2() { return this->input_gate2; };
		Object *get_input_gate3() { return this->input_gate3; };
		void set_input_gate1(Object *gate) { this->input_gate1 = gate; };
		void set_input_gate2(Object *gate) { this->input_gate2 = gate; };
		void set_input_gate3(Object *gate) { this->input_gate3 = gate; };
		void remove_input_object(int id) override;
		void update_state() override;

		std::string get_object_name() override { return get_output_type_text(); };

	private:
		GATE_TYPE gate_type;

		/* inputs/outputs */
		Object *input_gate1;
		Object *input_gate2;
		Object *input_gate3;

		std::string get_output_type_text();

};

#endif
