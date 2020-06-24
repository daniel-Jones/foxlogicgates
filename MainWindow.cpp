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

#include "MainWindow.h"
#include "FXMessageBox.h"
#include "fxdefs.h"
#include <memory>
#include <stdio.h>

FXDEFMAP(MainWindow) MainWindow_Map[]=
{
	//________Message_Type____________ID____________Message_Handler_______
	FXMAPFUNC(SEL_PAINT,             MainWindow::ID_CANVAS, MainWindow::on_paint),
	FXMAPFUNC(SEL_LEFTBUTTONPRESS,   MainWindow::ID_CANVAS, MainWindow::on_left_mouse_down),
	FXMAPFUNC(SEL_LEFTBUTTONRELEASE,   MainWindow::ID_CANVAS, MainWindow::on_left_mouse_up),
	FXMAPFUNC(SEL_RIGHTBUTTONPRESS,   MainWindow::ID_CANVAS, MainWindow::on_right_mouse_down),
	FXMAPFUNC(SEL_MOTION,            MainWindow::ID_CANVAS, MainWindow::on_mouse_move),
	FXMAPFUNC(SEL_KEYPRESS, 0, MainWindow::on_key_press),
	FXMAPFUNC(SEL_KEYRELEASE, 0, MainWindow::on_key_release),
	/* toolbox */
	FXMAPFUNC(SEL_COMMAND, MainWindow::ID_BUTTON_INPUT, MainWindow::input_button_press),
	FXMAPFUNC(SEL_COMMAND, MainWindow::ID_BUTTON_OUTPUT, MainWindow::output_button_press),
	FXMAPFUNC(SEL_COMMAND, MainWindow::ID_BUTTON_AND, MainWindow::and_button_press),
	FXMAPFUNC(SEL_COMMAND, MainWindow::ID_BUTTON_NAND, MainWindow::nand_button_press),
	FXMAPFUNC(SEL_COMMAND, MainWindow::ID_BUTTON_OR, MainWindow::or_button_press),
	FXMAPFUNC(SEL_COMMAND, MainWindow::ID_BUTTON_NOR, MainWindow::nor_button_press),
	FXMAPFUNC(SEL_COMMAND, MainWindow::ID_BUTTON_XOR, MainWindow::xor_button_press),
	FXMAPFUNC(SEL_COMMAND, MainWindow::ID_BUTTON_XNOR, MainWindow::xnor_button_press),
	FXMAPFUNC(SEL_COMMAND, MainWindow::ID_BUTTON_NOT, MainWindow::not_button_press),
};
FXIMPLEMENT(MainWindow, FXMainWindow, MainWindow_Map, ARRAYNUMBER(MainWindow_Map))

MainWindow::MainWindow(FXApp *a)
	: FXMainWindow(a, "foxlogicgates", nullptr, nullptr, DECOR_ALL, 0, 0, 500, 500)
{
	app = a;
	create_ui();
}

MainWindow::~MainWindow()
{
}

void
MainWindow::create()
{
	FXMainWindow::create();
	INPUT_icon->create();
	OUTPUT_icon->create();
	AND_icon->create();
	NAND_icon->create();
	OR_icon->create();
	NOR_icon->create();
	XOR_icon->create();
	XNOR_icon->create();
	NOT_icon->create();
	canvas_image->create();
	show(PLACEMENT_SCREEN);
}

void
MainWindow::create_ui()
{
	contents=new FXHorizontalFrame(this, LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y);
	toolsFrame = new FXVerticalFrame(contents, FRAME_SUNKEN|LAYOUT_FILL_Y|LAYOUT_TOP, 0, 0, 0, 0, 10, 10, 10, 10);

	canvasFrame=new FXVerticalFrame(contents, FRAME_SUNKEN|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT, 0, 0, 0, 0, 10, 10, 10, 10);

	new FXLabel(canvasFrame,"foxlogicgates", NULL, JUSTIFY_CENTER_X|LAYOUT_FILL_X);
	new FXHorizontalSeparator(canvasFrame, SEPARATOR_GROOVE|LAYOUT_FILL_X);

	scroll_area = new FXScrollWindow(canvasFrame, FX::SCROLLERS_NORMAL|LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_SUNKEN);
	scroll_area->setBackColor(canvasFrame->getBackColor());
	canvas = new FXCanvas(scroll_area, this, ID_CANVAS, FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN);

	canvas_image = new FXBMPImage(app, NULL, 0, 5000, 5000);


	new FXLabel(toolsFrame, "Toolbox", NULL, JUSTIFY_CENTER_X|LAYOUT_FILL_X);
	new FXHorizontalSeparator(toolsFrame, SEPARATOR_RIDGE|LAYOUT_FILL_X);

	INPUT_icon = new FXGIFIcon(app, INPUT_icon_data, IMAGE_KEEP);
	OUTPUT_icon = new FXGIFIcon(app, OUTPUT_icon_data, IMAGE_KEEP);
	AND_icon = new FXGIFIcon(app, AND_icon_data, IMAGE_KEEP);
	NAND_icon = new FXGIFIcon(app, NAND_icon_data, IMAGE_KEEP);
	OR_icon = new FXGIFIcon(app, OR_icon_data, IMAGE_KEEP);
	NOR_icon = new FXGIFIcon(app, NOR_icon_data, IMAGE_KEEP);
	XOR_icon = new FXGIFIcon(app, XOR_icon_data, IMAGE_KEEP);
	XNOR_icon = new FXGIFIcon(app, XNOR_icon_data, IMAGE_KEEP);
	NOT_icon = new FXGIFIcon(app, NOT_icon_data, IMAGE_KEEP);

	/* tools */
	new FXButton(toolsFrame, "", INPUT_icon, this, MainWindow::ID_BUTTON_INPUT, BUTTON_NORMAL|LAYOUT_FILL_X);
	new FXButton(toolsFrame, "", OUTPUT_icon, this, MainWindow::ID_BUTTON_OUTPUT, BUTTON_NORMAL|LAYOUT_FILL_X);
	new FXButton(toolsFrame, "AND", AND_icon, this, MainWindow::ID_BUTTON_AND, BUTTON_NORMAL|LAYOUT_FILL_X);
	new FXButton(toolsFrame, "NAND", NAND_icon, this, MainWindow::ID_BUTTON_NAND, BUTTON_NORMAL|LAYOUT_FILL_X);
	new FXButton(toolsFrame, "OR", OR_icon, this, MainWindow::ID_BUTTON_OR, BUTTON_NORMAL|LAYOUT_FILL_X);
	new FXButton(toolsFrame, "NOR", NOR_icon, this, MainWindow::ID_BUTTON_NOR, BUTTON_NORMAL|LAYOUT_FILL_X);
	new FXButton(toolsFrame, "XOR", XOR_icon, this, MainWindow::ID_BUTTON_XOR, BUTTON_NORMAL|LAYOUT_FILL_X);
	new FXButton(toolsFrame, "XNOR", XNOR_icon, this, MainWindow::ID_BUTTON_XNOR, BUTTON_NORMAL|LAYOUT_FILL_X);
	new FXButton(toolsFrame, "NOT", NOT_icon, this, MainWindow::ID_BUTTON_NOT, BUTTON_NORMAL|LAYOUT_FILL_X);

	optionsFrame = new FXVerticalFrame(contents, FRAME_SUNKEN|LAYOUT_FILL_Y|LAYOUT_TOP, 0, 0, 0, 0, 10, 10, 10, 10);
	new FXLabel(optionsFrame, "Options", NULL, JUSTIFY_CENTER_X|LAYOUT_FILL_X);
	new FXHorizontalSeparator(optionsFrame, SEPARATOR_RIDGE|LAYOUT_FILL_X);

	input1_frame = new FXHorizontalFrame(optionsFrame, LAYOUT_SIDE_TOP);
	input2_frame = new FXHorizontalFrame(optionsFrame, LAYOUT_SIDE_TOP);
	output_state_frame = new FXHorizontalFrame(optionsFrame, LAYOUT_SIDE_TOP);


	new FXLabel(input1_frame, "Input 1: ", NULL, JUSTIFY_CENTER_X);
	input_1_details = new FXLabel(input1_frame, "", NULL, JUSTIFY_CENTER_X);
	input_1_details->setText("(None)");

	new FXLabel(input2_frame, "Input 2: ", NULL, JUSTIFY_CENTER_X);
	input_2_details = new FXLabel(input2_frame, "", NULL, JUSTIFY_CENTER_X);
	input_2_details->setText("(None)");

	new FXLabel(output_state_frame, "Output: ", NULL, JUSTIFY_CENTER_X);
	output_details = new FXLabel(output_state_frame, "", NULL, JUSTIFY_CENTER_X);
	output_details->setText("(None)");
}

void
MainWindow::draw()
{
	FXDCWindow dc_image(canvas_image);
	dc_image.setFont(getApp()->getNormalFont());
	dc_image.setForeground(FXRGB(255, 255, 255));
	dc_image.fillRectangle(canvas->getX(), canvas->getY(), canvas->getWidth(), canvas->getHeight());
	dc_image.setForeground(FXRGB(0,0,0));

	Gate *gate1;

	/* update every gate */
	/*
	for(auto g1 = gates.begin(); g1 != gates.end(); ++g1)
	{
		gate1 = (*g1).get();
		gate1->update_state();
	}
	*/

	/* draw gates */
	for(auto g1 = gates.begin(); g1 != gates.end(); ++g1)
	{
		gate1 = (*g1).get();
		switch(gate1->get_gate_type())
		{
			case Gate::INPUT:
			{
				if (gate1->get_output_state() == true)
				{
					/* input is switched on, indicate so */
					dc_image.setForeground(FXRGB(255, 255, 0));
					dc_image.fillRectangle(gate1->get_x(), gate1->get_y(), gate1->get_width(), gate1->get_height());
					dc_image.setForeground(FXRGB(0,0,0));
				}
				dc_image.drawIcon(INPUT_icon, gate1->get_x(), gate1->get_y());
				break;
			}

			case Gate::OUTPUT:
			{
				if (gate1->get_output_state() == true)
				{
					/* output is on, indicate so */
					dc_image.setForeground(FXRGB(255, 255, 0));
					dc_image.fillRectangle(gate1->get_x(), gate1->get_y(), gate1->get_width(), gate1->get_height());
					dc_image.setForeground(FXRGB(0,0,0));
				}

				dc_image.drawIcon(OUTPUT_icon, gate1->get_x(), gate1->get_y());
				break;
			}

			case Gate::AND:
			{
				dc_image.drawIcon(AND_icon, gate1->get_x(), gate1->get_y());
				dc_image.drawText(gate1->get_x(), gate1->get_y()+gate1->get_height()+20, "AND");
				break;
			}

			case Gate::NAND:
			{
				dc_image.drawIcon(NAND_icon, gate1->get_x(), gate1->get_y());
				dc_image.drawText(gate1->get_x(), gate1->get_y()+gate1->get_height()+20, "NAND");
				break;
			}

			case Gate::OR:
			{
				dc_image.drawIcon(OR_icon, gate1->get_x(), gate1->get_y());
				dc_image.drawText(gate1->get_x(), gate1->get_y()+gate1->get_height()+20, "OR");
				break;
			}

			case Gate::NOR:
			{
				dc_image.drawIcon(NOR_icon, gate1->get_x(), gate1->get_y());
				dc_image.drawText(gate1->get_x(), gate1->get_y()+gate1->get_height()+20, "NOR");
				break;
			}

			case Gate::XOR:
			{
				dc_image.drawIcon(XOR_icon, gate1->get_x(), gate1->get_y());
				dc_image.drawText(gate1->get_x(), gate1->get_y()+gate1->get_height()+20, "XOR");
				break;
			}

			case Gate::XNOR:
			{
				dc_image.drawIcon(XNOR_icon, gate1->get_x(), gate1->get_y());
				dc_image.drawText(gate1->get_x(), gate1->get_y()+gate1->get_height()+20, "XNOR");
				break;
			}


			case Gate::NOT:
			{
				dc_image.drawIcon(NOT_icon, gate1->get_x(), gate1->get_y());
				dc_image.drawText(gate1->get_x(), gate1->get_y()+gate1->get_height()+20, "NOT");
				break;
			}
			case Gate::NONE:
			default:
				break;

		}
	}

	/* draw selected gate border box if one is selected */
	if (selected_gate != nullptr)
	{
		dc_image.drawHashBox(selected_gate->get_x(), selected_gate->get_y(), selected_gate->get_width(), selected_gate->get_height());
	}

	/* draw dragging link */
	if (dragging_link && selected_gate)
	{
		FXint mousex, mousey;
		FXuint mbuttons;
		canvas->getCursorPosition(mousex, mousey, mbuttons);
		dc_image.drawLine(selected_gate->get_x()+selected_gate->get_width()-5, selected_gate->get_y()+selected_gate->get_height()/2-2, mousex, mousey);
	}

	/* draw links */
	for(auto g1 = gates.begin(); g1 != gates.end(); ++g1)
	{
		/* draw lines from input gate->output gate */
		gate1 = (*g1).get();
		Gate *in_gate1 = gate1->get_input_gate1();
		Gate *in_gate2 = gate1->get_input_gate2();
		if (!gate1)
			continue;
		if (gate1->get_input_gate1() != nullptr)
		{
			if (in_gate1 == selected_input.gate)
			{
				dc_image.setForeground(FXRGB(255, 0, 0));
			}

			if (gate1->get_gate_type() == Gate::NOT || gate1->get_gate_type() == Gate::OUTPUT)
			{
				/* NOT,OUTPUT need a special case */
				dc_image.drawLine(in_gate1->get_x()+in_gate1->get_width()-5, in_gate1->get_y()+(in_gate1->get_height()/2),
						gate1->get_x()+10, gate1->get_y()+(gate1->get_height()/2));
				dc_image.setForeground(FXRGB(0, 0, 0));

			}
			else
			{
				dc_image.drawLine(in_gate1->get_x()+in_gate1->get_width()-5, in_gate1->get_y()+(in_gate1->get_height()/2),
						gate1->get_x()+10, gate1->get_y()+7);
				dc_image.setForeground(FXRGB(0, 0, 0));
			}
		}
		if (gate1->get_input_gate2() != nullptr)
		{
			if (in_gate2 == selected_input.gate)
			{
				dc_image.setForeground(FXRGB(255, 0, 0));
			}

			if (gate1->get_gate_type() == Gate::NOT || gate1->get_gate_type() == Gate::OUTPUT)
			{
				/* NOT,OUTPUT need a special case */
				continue;

			}
			else
			{

				dc_image.drawLine(in_gate2->get_x()+in_gate2->get_width()-5, in_gate2->get_y()+(in_gate2->get_height()/2),
					gate1->get_x()+10, gate1->get_y()+43);
				dc_image.setForeground(FXRGB(0, 0, 0));
			}
		}
			dc_image.setForeground(FXRGB(0, 0, 0));

	}

	/* update options panel */
	if (selected_gate)
	{
		input_1_details->setText((selected_gate->get_input_gate1() ? selected_gate->get_input_gate1()->get_output_type_text().c_str() : "(None)"));
		input_2_details->setText((selected_gate->get_input_gate2() ? selected_gate->get_input_gate2()->get_output_type_text().c_str() : "(None)"));
		output_details->setText(selected_gate->get_output_state() ? "ON" : "OFF");
	}
	else
	{
		input_1_details->setText("(None)");
		input_2_details->setText("(None)");
		output_details->setText("(None)");
	}

	FXDCWindow dc_canvas(canvas);
	dc_canvas.drawImage(canvas_image, 0, 0);
}

Gate
*MainWindow::find_gate_at(int x, int y)
{
	/* iterate backwards through vector to get box on top */
	Gate *returngate = nullptr;
	Gate *gate = nullptr;
	int bx, by, bw, bh;
	for (auto g = gates.rbegin(); g != gates.rend(); ++g)
	{
		gate = (*g).get();
		bx = gate->get_x();
		by = gate->get_y();
		bw = gate->get_width();
		bh = gate->get_height();
		/* check if x,y pos is intersecting with the gate */
		if (x >= bx && x <= bx+bw &&
				y >= by && y <= by+bh)
		{
			returngate = gate;
			break;
		}
	}

	return returngate;
}

Gate
*MainWindow::find_gate_by_id(int id)
{
	/* iterate backwards through vector to get box on top */
	Gate *gate = nullptr;
	for (auto g = gates.rbegin(); g != gates.rend(); ++g)
	{
		gate = (*g).get();
		if (gate->get_id() == id)
			return gate;
	}
	return gate;
}

void
MainWindow::update_gate_state(Gate *gate)
{
	gate->update_state();
	/* update all gates that are using this gate as an input */
	Gate *gate2;
	for(auto g = gate->get_output_gates()->begin(); g != gate->get_output_gates()->end(); ++g)
	{
		 gate2 = find_gate_by_id((*g));
		 if (gate2)
		 {
			 update_gate_state(gate2);
		 }
	}

}

void
MainWindow::find_selected_input(int x, int y)
{
	Gate *input_gate = nullptr;
	if (!selected_gate)
	{
		selected_input.gate = nullptr;
		selected_input.input = -1;
		return;
	}
	int input = -1;

	if (x >= selected_gate->get_x() && x <= selected_gate->get_x() + 20)
	{
		if (y-selected_gate->get_y() <= selected_gate->get_height()/2)
		{
			input = 1;
		}
		else
		{
			input = 2;
		}

		switch (input)
		{
			case 1:
			{
				if (selected_gate->get_input_gate1() != nullptr)
				{
					input_gate = selected_gate->get_input_gate1();
				}
				break;
			}

			case 2:
			{
				if (selected_gate->get_input_gate2() != nullptr)
				{
					input_gate = selected_gate->get_input_gate2();
				}
				else
				{
					/* special check for gates with only one input */
					if (selected_gate->get_gate_type() == Gate::NOT || selected_gate->get_gate_type() == Gate::OUTPUT)
					{
						input_gate = selected_gate->get_input_gate1();
					}
				}
				break;
			}

			default:
				input = -1;
				break;
		}
		if (input_gate != nullptr)
		{
			selected_input.gate = input_gate;
			selected_input.input = input;
			printf("selected input #%d of gate id %d\n", input, selected_gate->get_id());
		}
	}
}

long
MainWindow::on_paint(FXObject*, FXSelector, void *ptr)
{
	draw();
	return 1;
}

long
MainWindow::on_left_mouse_down(FXObject*, FXSelector, void *ptr)
{
	FXEvent *ev = (FXEvent*)ptr;
	lmouse_down = true;
	Gate *gate = nullptr;
	selected_input.gate = nullptr;
	selected_input.input = -1;
	if (selected_gate_type != Gate::NONE)
	{
		/* add new gate */
		std::unique_ptr<Gate> gate(new Gate(selected_gate_type, ev->last_x-70/2, ev->last_y-50/2, 70, 50));
		selected_gate = gate.get();
		gates.push_back(std::move(gate));
		selected_gate_type = Gate::NONE;
	}
	else
	{
		/* do other things */

		/* get gate at cursor */
		gate = find_gate_at(ev->last_x, ev->last_y);
		if (gate)
		{
			/* if we found a gate, select it */
			selected_gate = gate;
			if (lshift_down)
			{
				dragging_link = true;
			}
			update_gate_state(gate);
		}
		else
		{
			selected_gate = nullptr;
			selected_input.gate = nullptr;
			selected_input.input = -1;
		}

		if (selected_gate)
		{
			/* check if the user clicked on an input and select it */
			find_selected_input(ev->last_x, ev->last_y);
			if (selected_input.gate != nullptr)
			{
				/* an input is selected */
			}
		}
	}
	draw();
	return 1;

}

long
MainWindow::on_left_mouse_up(FXObject*, FXSelector, void *ptr)
{
	FXEvent *ev = (FXEvent*)ptr;
	lmouse_down = false;
	if (lshift_down && dragging_link && selected_gate)
	{
		Gate *gate;
		gate = find_gate_at(ev->last_x, ev->last_y);
		if (gate == selected_gate) /* gates cannot connect to themselves, probably */
			return 1;
		if (gate)
		{
			int input = -1;
			if (ev->last_y-gate->get_y() <= gate->get_height()/2)
				input = 1;
			else
				input = 2;
			if (gate->get_gate_type() != Gate::NOT && gate->get_gate_type() && gate->get_gate_type() != Gate::OUTPUT)
			{
				printf("connecting gate %d with gate %d at input #%d\n", selected_gate->get_id(), gate->get_id(), input);
				if (input == 1)
				{
					gate->set_input_gate1(selected_gate);
				}
				else if (input == 2)
				{
					gate->set_input_gate2(selected_gate);
				}
			}
			else
			{
				/* NOT,NOR,OUTPUT gates needs a special case */
				if (input == 1 || input == 2)
				{
					printf("connecting gate %d with gate %d at input #1\n", selected_gate->get_id(), gate->get_id());
					gate->set_input_gate1(selected_gate);
				}
			}
			selected_gate->add_output_gate_id(gate->get_id());
			update_gate_state(gate);
		}
		dragging_link = false;
	}
	return 1;
}

long
MainWindow::on_right_mouse_down(FXObject*, FXSelector, void *ptr)
{
	FXEvent *ev = (FXEvent*)ptr;
	Gate *gate;
	gate = find_gate_at(ev->last_x, ev->last_y);
	if (gate)
	{
		if (gate->get_gate_type() == Gate::INPUT)
		{
			/* toggle state */
			gate->set_state(!gate->get_output_state());
		}
		update_gate_state(gate);
	}
	draw();

	return 1;
}

long
MainWindow::on_key_press(FXObject *sender, FXSelector sel, void *ptr)
{
	FXEvent* event=(FXEvent*)ptr;
	switch(event->code)
	{
		case KEY_Shift_L:
			lshift_down = true;
			break;
		default:
			this->onKeyPress(sender, sel, ptr);
			break;
	}
	draw();
	return 1;
}

long
MainWindow::on_key_release(FXObject *sender, FXSelector sel, void *ptr)
{
	FXEvent* event=(FXEvent*)ptr;
	switch(event->code)
	{
		case KEY_Shift_L:
			lshift_down = false;
			dragging_link = false;
			break;
		case KEY_Delete:
		{
			if (selected_input.gate != nullptr)
			{
				/* delete link */
				switch (selected_input.input)
				{
					case 1:
						selected_gate->get_input_gate1()->remove_output_gate_id(selected_gate->get_id());
						selected_gate->set_input_gate1(nullptr);
						break;
					case 2:
						selected_gate->get_input_gate2()->remove_output_gate_id(selected_gate->get_id());
						selected_gate->set_input_gate2(nullptr);
						break;
					default: break;
				}
				update_gate_state(selected_gate);
				update_gate_state(selected_input.gate);
				selected_input.gate = nullptr;
				selected_input.input = -1;
				draw();
			}
			else if (selected_gate)
			{
				Gate *gate;
				/* delete inputs */
				if (selected_gate->get_input_gate1())
				{
					selected_gate->get_input_gate1()->remove_output_gate_id(selected_gate->get_id());
					update_gate_state(selected_gate->get_input_gate1());
				}
				if (selected_gate->get_input_gate2())
				{
					selected_gate->get_input_gate2()->remove_output_gate_id(selected_gate->get_id());
					update_gate_state(selected_gate->get_input_gate2());
				}

				/* delete outputs */
				for(auto g = selected_gate->get_output_gates()->begin(); g != selected_gate->get_output_gates()->end(); ++g)
				{
					gate = find_gate_by_id((*g));
					if (!gate)
						continue;
					gate->remove_input_gate(selected_gate->get_id());
					update_gate_state(gate);
				}
				int pos = 0;
				for (auto g = gates.begin(); g != gates.end(); ++g)
				{
					gate = (*g).get();
					if (gate->get_id() == selected_gate->get_id())
					{
						gates.erase(gates.begin() + pos);
						break;
					}
					pos++;
				}
			}
			break;
		}

		default:
			this->onKeyPress(sender, sel, ptr);
			break;
	}
	draw();
	return 1;
}

long
MainWindow::on_mouse_move(FXObject *sender, FXSelector sel, void *ptr)
{
	if (lmouse_down && !dragging_link && selected_gate)
	{
		FXEvent* event = (FXEvent*)ptr;
		selected_gate->set_x(event->last_x-selected_gate->get_width()/2);
		selected_gate->set_y(event->last_y-selected_gate->get_height()/2);
	}
	if (lmouse_down)
		draw();
	return 1;
}

long
MainWindow::input_button_press(FXObject *sender, FXSelector sel, void *ptr)
{
	selected_gate = nullptr;
	selected_gate_type = Gate::INPUT;
	return 1;
}

long
MainWindow::output_button_press(FXObject *sender, FXSelector sel, void *ptr)
{
	selected_gate = nullptr;
	selected_gate_type = Gate::OUTPUT;
	return 1;
}

long
MainWindow::and_button_press(FXObject *sender, FXSelector sel, void *ptr)
{
	selected_gate = nullptr;
	selected_gate_type = Gate::AND;
	return 1;
}

long
MainWindow::nand_button_press(FXObject *sender, FXSelector sel, void *ptr)
{
	selected_gate = nullptr;
	selected_gate_type = Gate::NAND;
	return 1;
}

long
MainWindow::or_button_press(FXObject *sender, FXSelector sel, void *ptr)
{
	selected_gate = nullptr;
	selected_gate_type = Gate::OR;
	return 1;
}

long
MainWindow::xor_button_press(FXObject *sender, FXSelector sel, void *ptr)
{
	selected_gate = nullptr;
	selected_gate_type = Gate::XOR;
	return 1;
}

long
MainWindow::nor_button_press(FXObject *sender, FXSelector sel, void *ptr)
{
	selected_gate = nullptr;
	selected_gate_type = Gate::NOR;
	return 1;
}

long
MainWindow::xnor_button_press(FXObject *sender, FXSelector sel, void *ptr)
{
	puts("xnor");
	selected_gate = nullptr;
	selected_gate_type = Gate::XNOR;
	return 1;
}

long
MainWindow::not_button_press(FXObject *sender, FXSelector sel, void *ptr)
{
	selected_gate = nullptr;
	selected_gate_type = Gate::NOT;
	return 1;
}
