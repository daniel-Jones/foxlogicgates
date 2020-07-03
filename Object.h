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

#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <string>

class Object
{
	/*
	 * abstract class that all usable objects should subclass
	 */
	public:
		Object() { }; // default constructor for all objects
		Object(int id_) { id = id_; }; // overloaded constructor for passing id - used when loading

		/*
		 * this enum is an internal reference to every kind object, each object needs an entry here
		 */
		enum OBJECT_TYPE
		{
			NONE = 0,
			GATE,
			BINARYDISPLAY,
		};

		struct selected_input_object
		{
			Object *object;
			int input;
		};

		virtual ~Object() = default;
		int get_id() { return this->id; };
		int get_x() { return this->x; };
		int get_y() { return this->y; };
		int get_width() { return this->w; };
		int get_height() { return this->h; };
		void set_x(int x) { this->x = x; };
		void set_y(int y) { this->y = y; };
		
		void add_output_object_id(int id_) { this->output_object_ids.push_back(id_); };

		virtual void update_state() {}; // subclasses must implement

		static void set_object_id_counter(int id_) { object_id_counter = id_; };
		static int get_object_id_counter() { return object_id_counter; };

		OBJECT_TYPE get_object_type() { return object_type; };

		virtual std::string get_object_name() { return "NOT IMPLEMENTED"; };

		std::vector<int> *get_output_objects() { return &this->output_object_ids; };

		void remove_output_object_id(int id_)
		{
			int pos = 0;
			for(auto o = output_object_ids.begin(); o != output_object_ids.end(); ++o)
			{
				if (id_ == (*o))
				{
					output_object_ids.erase(output_object_ids.begin() + pos);
					break;
				}
				pos++;
			}
		}

		bool get_output_state() { return this->output_state; };
		void set_state(bool state) { this->output_state = state; };

		virtual void remove_input_object(int id_) { printf("remove_input_object not implemented\n"); };

	protected:
		int id;
		int x;
		int y;
		int w;
		int h;

		std::vector<int> output_object_ids;

		void set_object_type(OBJECT_TYPE type) { object_type = type; }; // every object must set this, none by default

		static int object_id_counter; // used as the id of a new objecct - this is NOT a count of the number of objects

		/* states */
		bool output_state = false;
	
	private:
		enum OBJECT_TYPE object_type = NONE;

};

#endif
