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

class Gate
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
		};

		Gate(GATE_TYPE type = INPUT, int x = 0, int y = 0, int width = 70, int height = 50);
		~Gate();

		int get_id() { return this->id; }
		int get_x() { return this->x; };
		int get_y() { return this->y; };
		int get_width() { return this->w; };
		int get_height() { return this->h; };
		bool get_output_state() { return this->output_state; };
		GATE_TYPE get_gate_type() { return this->gate_type; };
		Gate *get_input_gate1() { return this->input_gate1; };
		Gate *get_input_gate2() { return this->input_gate2; };
		Gate *get_output_gate() { return this->output_gate; };
		void set_x(int x) { this->x = x; };
		void set_y(int y) { this->y = y; };
		void set_state(bool state) { this->output_state = state; };
		void set_input_gate1(Gate *gate) { this->input_gate1 = gate; };
		void set_input_gate2(Gate *gate) { this->input_gate2 = gate; };
		void set_output_gate(Gate *gate) { this->output_gate = gate; };
		void update_state();

	private:
		GATE_TYPE gate_type;
		int id;
		int x;
		int y;
		int w;
		int h;
		static int gate_id_counter; // used as the id of a new gate - this is NOT a count of the number of gates

		/* inputs/outputs */
		Gate *input_gate1;
		Gate *input_gate2;
		Gate *output_gate;

		/* states */
		bool output_state = false;
};

#endif
