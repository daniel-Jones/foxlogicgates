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

#include "Gate.h"

int Gate::gate_id_counter = 0;

Gate::Gate(GATE_TYPE type, int x, int y, int width, int height)
{
	this->gate_type = type;
	this->input_gate1 = nullptr;
	this->input_gate2 = nullptr;
	this->output_gate = nullptr;
	this->id = Gate::gate_id_counter++; // increment counter after assigning
	this->x = x;
	this->y = y;
	this->w = width;
	this->h = height;
}

Gate::~Gate() {}

void Gate::update_state()
{
	switch (this->gate_type)
	{
		case INPUT:
		{
			/* we're a simple input, do nothing */
			break;
		}

		case OUTPUT:
		{
			/* we're a simple output, do nothing */
			if (input_gate1 && input_gate1->get_output_state() == true)
			{
				this->output_state = true;
			}
			else if (input_gate1)
			{
				this->output_state = false;
			}
			break;
		}


		case AND:
		{
			if (!input_gate1 || !input_gate2) {this->output_state = false; return; }
			output_state = input_gate1->get_output_state() & input_gate2->get_output_state();
			break;
		}


		case OR:
		{
			if (!input_gate1 || !input_gate2) {this->output_state = false; return; }
			output_state = input_gate1->get_output_state() | input_gate2->get_output_state();
			break;
		}


		case NOT:
		{
			/* NOT only cares for input 1 */
			if (!input_gate1) {this->output_state = false; return; }
			output_state = !input_gate1->get_output_state();
			break;
		}


		case NAND:
		{
			if (!input_gate1 || !input_gate2) {this->output_state = false; return; }
			output_state = !(input_gate1->get_output_state() & input_gate2->get_output_state());
			break;
		}


		case NOR:
		{
			if (!input_gate1 || !input_gate2) {this->output_state = false; return; }
			output_state = !(input_gate1->get_output_state() | input_gate2->get_output_state());
			break;
		}


		case XOR:
		{
			if (!input_gate1 || !input_gate2) {this->output_state = false; return; }
			output_state = input_gate1->get_output_state() ^ input_gate2->get_output_state();
			break;
		}


		case XNOR:
		{
			if (!input_gate1 || !input_gate2) {this->output_state = false; return; }
			output_state = !(input_gate1->get_output_state() ^ input_gate2->get_output_state());
			break;
		}

		default:
			break;
	}
}
