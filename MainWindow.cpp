/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the * GNU General Public License for more details.  *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "MainWindow.h"
#include "FXMessageBox.h"
#include "FXScrollArea.h"
#include "FXScrollBar.h"
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
	FXMAPFUNC(SEL_COMMAND, MainWindow::ID_BUTTON_BINARYDISPLAY, MainWindow::binarydisplay_button_press),

	/* options */
	FXMAPFUNC(SEL_COMMAND, MainWindow::ID_BUTTON_SAVE, MainWindow::save_button_press),
	FXMAPFUNC(SEL_COMMAND, MainWindow::ID_BUTTON_LOAD, MainWindow::load_button_press),
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
	BinaryDisplay_icon->create();
	canvas_image->create();
	show(PLACEMENT_SCREEN);
}

void
MainWindow::create_ui()
{
	contents = new FXHorizontalFrame(this, LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y);

	/* icons */

	INPUT_icon = new FXGIFIcon(app, INPUT_icon_data, IMAGE_KEEP);
	OUTPUT_icon = new FXGIFIcon(app, OUTPUT_icon_data, IMAGE_KEEP);
	AND_icon = new FXGIFIcon(app, AND_icon_data, IMAGE_KEEP);
	NAND_icon = new FXGIFIcon(app, NAND_icon_data, IMAGE_KEEP);
	OR_icon = new FXGIFIcon(app, OR_icon_data, IMAGE_KEEP);
	NOR_icon = new FXGIFIcon(app, NOR_icon_data, IMAGE_KEEP);
	XOR_icon = new FXGIFIcon(app, XOR_icon_data, IMAGE_KEEP);
	XNOR_icon = new FXGIFIcon(app, XNOR_icon_data, IMAGE_KEEP);
	NOT_icon = new FXGIFIcon(app, NOT_icon_data, IMAGE_KEEP);
	BinaryDisplay_icon = new FXGIFIcon(app, BinaryDisplay_icon_data, IMAGE_KEEP);

	/* tools */
	toolbox_scroll_area = new FXScrollWindow(contents, FX::SCROLLERS_NORMAL|LAYOUT_FILL_Y|LAYOUT_FIX_WIDTH, 0, 0, 200);
	//toolsFrame = new FXVerticalFrame(toolbox_scroll_area, FRAME_SUNKEN|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_FIX_WIDTH, 0, 0, 70);
	FXPacker *toolsFrame = new FXPacker(toolbox_scroll_area, LAYOUT_FILL_X|LAYOUT_FILL_Y, 0, 0);
	new FXLabel(toolsFrame, "Toolbox", NULL, JUSTIFY_CENTER_X|LAYOUT_FILL_X);
	new FXHorizontalSeparator(toolsFrame, SEPARATOR_RIDGE|LAYOUT_FILL_X);
	new FXButton(toolsFrame, "", INPUT_icon, this, MainWindow::ID_BUTTON_INPUT, BUTTON_NORMAL|LAYOUT_FILL_X);
	new FXButton(toolsFrame, "", OUTPUT_icon, this, MainWindow::ID_BUTTON_OUTPUT, BUTTON_NORMAL|LAYOUT_FILL_X);
	new FXButton(toolsFrame, "AND", AND_icon, this, MainWindow::ID_BUTTON_AND, BUTTON_NORMAL|LAYOUT_FILL_X);
	new FXButton(toolsFrame, "NAND", NAND_icon, this, MainWindow::ID_BUTTON_NAND, BUTTON_NORMAL|LAYOUT_FILL_X);
	new FXButton(toolsFrame, "OR", OR_icon, this, MainWindow::ID_BUTTON_OR, BUTTON_NORMAL|LAYOUT_FILL_X);
	new FXButton(toolsFrame, "NOR", NOR_icon, this, MainWindow::ID_BUTTON_NOR, BUTTON_NORMAL|LAYOUT_FILL_X);
	new FXButton(toolsFrame, "XOR", XOR_icon, this, MainWindow::ID_BUTTON_XOR, BUTTON_NORMAL|LAYOUT_FILL_X);
	new FXButton(toolsFrame, "XNOR", XNOR_icon, this, MainWindow::ID_BUTTON_XNOR, BUTTON_NORMAL|LAYOUT_FILL_X);
	new FXButton(toolsFrame, "NOT", NOT_icon, this, MainWindow::ID_BUTTON_NOT, BUTTON_NORMAL|LAYOUT_FILL_X);
	new FXButton(toolsFrame, "BinaryDisplay", NULL, this, MainWindow::ID_BUTTON_BINARYDISPLAY, BUTTON_NORMAL|LAYOUT_FILL_X);

	canvasFrame=new FXVerticalFrame(contents, FRAME_SUNKEN|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT, 0, 0, 0, 0, 10, 10, 10, 10);

	new FXLabel(canvasFrame,"foxlogicgates", NULL, JUSTIFY_CENTER_X|LAYOUT_FILL_X);
	new FXHorizontalSeparator(canvasFrame, SEPARATOR_GROOVE|LAYOUT_FILL_X);

	scroll_area = new FXScrollWindow(canvasFrame, FX::SCROLLERS_NORMAL|LAYOUT_FILL_X|LAYOUT_FILL_Y);
	scroll_area->setBackColor(canvasFrame->getBackColor());
	FXPacker *canvas_packer = new FXPacker(scroll_area, LAYOUT_FILL_X|LAYOUT_FILL_Y, 0, 0, 2048, 2048);
	canvas = new FXCanvas(canvas_packer, this, ID_CANVAS, LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT|LAYOUT_FILL_X|LAYOUT_FILL_Y, 0, 0, 2048, 2048);

	canvas_image = new FXBMPImage(app, NULL, 0, 2048, 2048);

	/* options */
	options_scroll_area = new FXScrollWindow(contents, FX::SCROLLERS_NORMAL|LAYOUT_FILL_Y|LAYOUT_FIX_WIDTH, 0, 0, 123);
	optionsFrame = new FXVerticalFrame(options_scroll_area, FRAME_SUNKEN|LAYOUT_FILL_Y|LAYOUT_TOP, 0, 0, 0, 0, 10, 10, 10, 10);
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

	new FXHorizontalSeparator(optionsFrame, SEPARATOR_RIDGE|LAYOUT_FILL_X);

	/* save/load */
	new FXLabel(optionsFrame, "Save as XML", NULL, JUSTIFY_CENTER_X|LAYOUT_FILL_X);
	new FXButton(optionsFrame, "Save", nullptr, this, MainWindow::ID_BUTTON_SAVE, BUTTON_NORMAL|LAYOUT_FILL_X);
	new FXLabel(optionsFrame, "Load from XML", NULL, JUSTIFY_CENTER_X|LAYOUT_FILL_X);
	new FXButton(optionsFrame, "Load", nullptr, this, MainWindow::ID_BUTTON_LOAD, BUTTON_NORMAL|LAYOUT_FILL_X);
}

void
MainWindow::draw()
{
	FXDCWindow dc_image(canvas_image);
	dc_image.setFont(getApp()->getNormalFont());
	dc_image.setForeground(FXRGB(255, 255, 255));
	dc_image.fillRectangle(canvas->getX()-10, canvas->getY()-10, canvas->getWidth()+10, canvas->getHeight()+10); // -+10 to fix black border
	dc_image.setForeground(FXRGB(0,0,0));
	bool drawn_special_link = false;

	Gate *gate1;

	/* draw objects */
	for(auto g1 = objects.begin(); g1 != objects.end(); ++g1)
	{
		switch ((*g1)->get_object_type())
		{
			case Object::GATE:
			{
				gate1 = (Gate*)(*g1).get();
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
					break;
			}
			case Object::BINARYDISPLAY:
			{
				class BinaryDisplay *bdsp = (class BinaryDisplay*)(*g1).get();
				dc_image.drawIcon(BinaryDisplay_icon, bdsp->get_x(), bdsp->get_y());
				dc_image.drawText(bdsp->get_x()+bdsp->get_width()+10, bdsp->get_y()+(bdsp->get_height()/2), FXStringVal(bdsp->get_sum_value()));
				break;
			}
			case Object::NONE:
			default:
				printf("implement objects in draw() drawing objects\n");
				break;
		}
	}

	/* draw selected gate border box if one is selected */
	if (selected_object != nullptr)
	{
		dc_image.drawHashBox(selected_object->get_x(), selected_object->get_y(), selected_object->get_width(), selected_object->get_height());
	}
	else if (!selected_objects.empty())
	{
		/* draw border box if multuple gates selected */
		Object *selobject;
		for (auto g = selected_objects.begin(); g != selected_objects.end(); ++g)
		{
			selobject = (Object*)(*g);
			dc_image.drawHashBox(selobject->get_x(), selobject->get_y(), selobject->get_width(), selobject->get_height());
		}
	}

	/* draw dragging link */
	if (dragging_link && selected_object)
	{
		FXint mousex, mousey;
		FXuint mbuttons;
		canvas->getCursorPosition(mousex, mousey, mbuttons);
		dc_image.drawLine(selected_object->get_x()+selected_object->get_width()-5, selected_object->get_y()+selected_object->get_height()/2-2, mousex, mousey);
	}

	/* draw links */
	for(auto g1 = objects.begin(); g1 != objects.end(); ++g1)
	{
		/* draw lines from input gate->output gate */
		switch ((*g1)->get_object_type())
		{
			case Object::GATE:
			{
				gate1 = (Gate*)(*g1).get();
				if (!gate1)
					continue;
				Gate *in_gate1 = gate1->get_input_gate1();
				Gate *in_gate2 = gate1->get_input_gate2();
				if (in_gate1 != nullptr)
				{
					if (in_gate1 == selected_input.object)
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
				if (in_gate2 != nullptr)
				{
					if (in_gate2 == selected_input.object)
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
				break;
		}
			case Object::BINARYDISPLAY:
			{
				class BinaryDisplay *bdsp = (class BinaryDisplay*)(*g1).get();
				Object *input;
				int gap = 5;
				if ((input = bdsp->get_input0()))
				{
						dc_image.drawLine(input->get_x()+input->get_width()-5, input->get_y()+(input->get_height()/2),
								bdsp->get_x()+10, bdsp->get_y()+90);
				}
				if ((input = bdsp->get_input1()))
				{
						dc_image.drawLine(input->get_x()+input->get_width()-5, input->get_y()+(input->get_height()/2),
								bdsp->get_x()+10, bdsp->get_y()+80);
				}
				if ((input = bdsp->get_input2()))
				{
						dc_image.drawLine(input->get_x()+input->get_width()-5, input->get_y()+(input->get_height()/2),
								bdsp->get_x()+10, bdsp->get_y()+66);
				}
				if ((input = bdsp->get_input3()))
				{
						dc_image.drawLine(input->get_x()+input->get_width()-5, input->get_y()+(input->get_height()/2),
								bdsp->get_x()+10, bdsp->get_y()+55);
				}
				if ((input = bdsp->get_input4()))
				{
						dc_image.drawLine(input->get_x()+input->get_width()-5, input->get_y()+(input->get_height()/2),
								bdsp->get_x()+10, bdsp->get_y()+44);
				}
				if ((input = bdsp->get_input5()))
				{
						dc_image.drawLine(input->get_x()+input->get_width()-5, input->get_y()+(input->get_height()/2),
								bdsp->get_x()+10, bdsp->get_y()+33);
				}
				if ((input = bdsp->get_input6()))
				{
						dc_image.drawLine(input->get_x()+input->get_width()-5, input->get_y()+(input->get_height()/2),
								bdsp->get_x()+10, bdsp->get_y()+22);
				}
				if ((input = bdsp->get_input7()))
				{
						dc_image.drawLine(input->get_x()+input->get_width()-5, input->get_y()+(input->get_height()/2),
								bdsp->get_x()+10, bdsp->get_y()+11);
				}







				break;
			}
			case Object::NONE:
			default:
				printf("draw() implement other objects\n");
				break;
		}
	}

	/*draw rubber band */
	if (rubberbanding)
	{
		FXint mousex, mousey;
		FXuint mbuttons;
		canvas->getCursorPosition(mousex, mousey, mbuttons);
		dc_image.drawRectangle(rubberband_startx, rubberband_starty, mousex-rubberband_startx, mousey-rubberband_starty);
	}

	/* update options panel */
	if (selected_object)
	{
		switch (selected_object->get_object_type())
		{
			case Object::GATE:
			{
				Gate *gate = (Gate*)selected_object;
				input_1_details->setText((gate->get_input_gate1()) ? gate->get_input_gate1()->get_object_name().c_str() : "(None)");
				input_2_details->setText((gate->get_input_gate2()) ? gate->get_input_gate2()->get_object_name().c_str() : "(None)");
				output_details->setText(gate->get_output_state() ? "ON" : "OFF");
				break;
			}
			case Object::BINARYDISPLAY:
			{
				;break;
			}
			case Object::NONE:
			default:
				printf("otpion panel update object not implemetned\n");
				break;
		}
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

Object
*MainWindow::find_object_at(int x, int y)
{
	/* iterate backwards through vector to get box on top */
	Object *returnobject = nullptr;
	Object *object = nullptr;
	int bx, by, bw, bh;
	for (auto g = objects.rbegin(); g != objects.rend(); ++g)
	{
		object = (*g).get();
		bx = object->get_x();
		by = object->get_y();
		bw = object->get_width();
		bh = object->get_height();
		/* check if x,y pos is intersecting with the object */
		if (x >= bx && x <= bx+bw &&
				y >= by && y <= by+bh)
		{
			returnobject = object;
			break;
		}
	}

	return returnobject;
}

Object
*MainWindow::find_object_by_id(int id)
{
	/* iterate backwards through vector to get object on top */
	Object *object = nullptr;
	for (auto g = objects.rbegin(); g != objects.rend(); ++g)
	{
		object = (*g).get();
		if (object->get_id() == id)
			return object;
	}
	return object;
}

void
MainWindow::update_object_state(Object *object)
{
	object->update_state();
	/* update all gates that are using this gate as an input */
	Gate *object2;
	for(auto o = object->get_output_objects()->begin(); o != object->get_output_objects()->end(); ++o)
	{
		switch (object->get_object_type())
		{
			case Object::GATE:
				 object2 = (Gate*)find_object_by_id((*o));
				 if (object2)
				 {
					 update_object_state(object2);
				 }
				 break;
			case Object::NONE:
			default:
				    printf("implement update_object_state for other types");
				    break;
		}
	}

}

void
MainWindow::find_selected_input(int x, int y)
{
	Gate *input_gate = nullptr;

	//FIXME: make object
	Gate *gate = (Gate*)selected_object;
	if (!selected_object)
	{
		selected_input.object = nullptr;
		selected_input.input = -1;
		return;
	}
	int input = -1;

	if (x >= selected_object->get_x() && x <= selected_object->get_x() + 20)
	{
		if (y-selected_object->get_y() <= selected_object->get_height()/2)
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
				if (gate->get_input_gate1() != nullptr)
				{
					input_gate = gate->get_input_gate1();
				}
				break;
			}

			case 2:
			{
				if (gate->get_input_gate2() != nullptr)
				{
					input_gate = gate->get_input_gate2();
				}
				else
				{
					/* special check for gates with only one input */
					if (gate->get_gate_type() == Gate::NOT || gate->get_gate_type() == Gate::OUTPUT)
					{
						input_gate = gate->get_input_gate1();
						input = 1;
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
			selected_input.object = input_gate;
			selected_input.input = input;
			printf("selected input #%d of gate id %d\n", input, selected_object->get_id());
		}
	}
}

bool
MainWindow::save_file()
{
	FXString filename=FXFileDialog::getSaveFilename(this, "Save", "logicgates.xml", "XML files (*.xml)\nAll Files(*.*)"); 
	if(!filename.empty())
	{
		file_name = filename.text();
		printf("saving to %s\n", file_name.c_str());
	}
	else
	{
		/* cannot save */
		return false;
	}

	/*
	 * what we need to save:
	 * Meta:
	 * 	id for next created gate (we can just write the static variable gate_id_counter, it is incremented to the correct next gate id anyway)
	 *
	 * Gates:
	 * 	id
	 * 	x pos
	 * 	y pos
	 * 	width
	 * 	height
	 * 	gate_type (save as int?)
	 *	input gate (we need the pointer, but we will save the id and make the pointer later)
	 *	output gates (int vector of ids)
	 *	output_state
	 */
	pugi::xml_document doc;
	auto declarationNode = doc.append_child(pugi::node_declaration);
	declarationNode.append_attribute("version")    = "1.0";
	declarationNode.append_attribute("encoding")   = "ISO-8859-1";
	declarationNode.append_attribute("standalone") = "yes";

	/* write meta data */
	auto meta = doc.append_child("Meta");
	pugi::xml_node info_xml = meta.append_child("Info");
	info_xml.append_attribute("next_id") = Object::get_object_id_counter();

	auto root = doc.append_child("Gates");

	/* iterate through all objects and add child nodes */
	Object *object;
	for (auto g = objects.begin(); g != objects.end(); ++g)
	{
		pugi::xml_node gate_xml = root.append_child("Gate");
		object = (*g).get();
		switch (object->get_object_type())
		{
			case Object::GATE:
				Gate *gate;
				gate = (Gate*)object;
				gate_xml.append_attribute("id") = object->get_id();
				gate_xml.append_attribute("x") = object->get_x();
				gate_xml.append_attribute("y") = object->get_y();
				gate_xml.append_attribute("w") = object->get_width();
				gate_xml.append_attribute("h") = object->get_height();
				gate_xml.append_attribute("type") = gate->get_gate_type();
				if (gate->get_input_gate1())
					gate_xml.append_attribute("input1_id") = gate->get_input_gate1()->get_id();
				if (gate->get_input_gate2())
					gate_xml.append_attribute("input2_id") = gate->get_input_gate2()->get_id();
				gate_xml.append_attribute("output_state") = gate->get_output_state();

				/* iterate through all output gates and write them */
				for(auto id = gate->get_output_objects()->begin(); id != gate->get_output_objects()->end(); ++id)
				{
					auto outid_node = gate_xml.append_child("output_gate_id");
					outid_node.append_attribute("id") = (*id);
				}
				break;
		case Object::NONE:
		default:
			printf("save_file() implement other objects\n");
			break;
		}
	}

	bool saved = doc.save_file(file_name.c_str());

	if (saved)
		printf("saved to %s\n", file_name.c_str());
	else
		printf("could not save");

	return true;
}

bool
MainWindow::load_file()
{
	FXString filename=FXFileDialog::getOpenFilename(this, "Open", "", "XML files (*.xml)\nAll Files(*.*)");
	if(!filename.empty())
	{
		file_name = filename.text();
		printf("loading from %s\n", file_name.c_str());
	}
	else
	{
		/* cannot load */
		return false;
	}

	int next_gate_id = 0;
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(file_name.c_str(),
			            pugi::parse_default|pugi::parse_declaration);
	if (!result)
	{
		/* failed reading file */
		return false;
	}

	remove_all_gates();

	/* read meta info */
	auto meta_objects = doc.child("Meta");
	next_gate_id = meta_objects.child("Info").attribute("next_id").as_int();
	printf("next gate id after loading is: %d\n", next_gate_id);

	/* read gates */
	auto node_objects = doc.child("Gates");
	for (auto node: node_objects.children("Gate"))
	{
		int id, x, y, w, h;
		bool output_state;
		Gate::GATE_TYPE type;

		id = node.attribute("id").as_int();
		x = node.attribute("x").as_int();
		y = node.attribute("y").as_int();
		w = node.attribute("w").as_int();
		h = node.attribute("h").as_int();
				type = (Gate::GATE_TYPE)node.attribute("type").as_int();
		output_state = node.attribute("output_state").as_bool();
		std::vector<int> output_gate_ids;
		printf("new gate with id %d\n", id);
		std::unique_ptr<Gate> gate(new Gate(type, x, y, w, h, id));
		gate->set_state(output_state);

		/* read output gate ids */
		for (auto output_node: node.children("output_gate_id"))
		{
			int out_id = output_node.attribute("id").as_int();
			output_gate_ids.push_back(out_id);
		}

		/*set output ids */
		for (auto idout =  output_gate_ids.begin(); idout != output_gate_ids.end(); ++idout)
		{
			gate->add_output_object_id((*idout));
		}

		objects.push_back(std::move(gate));
	}

	/* iterate again through all gates in the xml file and set output gates if they exist */

	for (auto node: node_objects.children("Gate"))
	{
		int input1 = -1;
		int input2 = -1;
		Gate *gate;

		if (strcmp(node.attribute("input1_id").as_string(), "") != 0)
		{
			input1 = node.attribute("input1_id").as_int();
			printf("input 1 exists: %d\n", input1);
		}
		if (strcmp(node.attribute("input2_id").as_string(), "") != 0)
		{
			input2 = node.attribute("input2_id").as_int();
			printf("input 2 exists: %d\n", input2);
		}

		gate = (Gate*)find_object_by_id(node.attribute("id").as_int());

		/* set inputs */
		if (input1 != -1)
		{
			gate->set_input_gate1((Gate*)find_object_by_id(input1));
		}

		if (input2 != -1)
		{
			gate->set_input_gate2((Gate*)find_object_by_id(input2));
		}
	}

	/* set gate id counter */
	Object::set_object_id_counter(next_gate_id);
	return true;
}

void
MainWindow::remove_all_gates()
{
	objects.clear();
	selected_object = nullptr;
	selected_input.object = nullptr;
	selected_input.input = -1;
	Object::set_object_id_counter(0);
	draw();
}

void
MainWindow::find_gates_in_area(int x, int y, int width, int height)
{
	/*
	 * find all gates in a given rectangle
	 */
	Object *object;
	int gx, gy, gw, gh;
	for (auto g = objects.begin(); g != objects.end(); ++g)
	{
		object = (*g).get();
		gx = object->get_x();
		gy = object->get_y();
		gw = object->get_width();
		gh = object->get_height();

		/* check if rectangles intersect */
		if(gx < x+width && gx+gw > x
		   && gy < y+height && gy+gh > y)
		{
			printf("adding object %d to selected objects list\n", object->get_id());
			selected_objects.push_back(object);
		}
		if (!selected_objects.empty())
		{
			selected_object = nullptr;
			selected_input.object = nullptr;
			selected_input.input = -1;
		}
	}
}

void
MainWindow::remove_object(Object &object)
{
	Object *out_object;
	switch (object.get_object_type())
	{
		case Object::GATE:
		{
			Gate &gate = (Gate&)object;
			Gate *out_gate;
			/* delete inputs */
			if (gate.get_input_gate1())
			{
				gate.get_input_gate1()->remove_output_object_id(gate.get_id());
				update_object_state(gate.get_input_gate1());
			}
			if (gate.get_input_gate2())
			{
				gate.get_input_gate2()->remove_output_object_id(gate.get_id());
				update_object_state(gate.get_input_gate2());
			}

			/* delete outputs */
			for(auto g = gate.get_output_objects()->begin(); g != gate.get_output_objects()->end(); ++g)
			{
				out_gate = (Gate*)find_object_by_id((*g));
				if (!out_gate)
					continue;
				out_gate->remove_input_gate(gate.get_id());
				update_object_state(out_gate);
			}
			break;
		}
		case Object::BINARYDISPLAY:
		{
			BinaryDisplay &bdsp = (BinaryDisplay&)object;
			if (bdsp.get_input0()) { bdsp.get_input0()->remove_output_object_id(bdsp.get_id()); update_object_state(bdsp.get_input0());  }
			if (bdsp.get_input1()) { bdsp.get_input1()->remove_output_object_id(bdsp.get_id()); update_object_state(bdsp.get_input1());  }
			if (bdsp.get_input2()) { bdsp.get_input2()->remove_output_object_id(bdsp.get_id()); update_object_state(bdsp.get_input2());  }
			if (bdsp.get_input3()) { bdsp.get_input3()->remove_output_object_id(bdsp.get_id()); update_object_state(bdsp.get_input3());  }
			if (bdsp.get_input4()) { bdsp.get_input4()->remove_output_object_id(bdsp.get_id()); update_object_state(bdsp.get_input4());  }
			if (bdsp.get_input5()) { bdsp.get_input5()->remove_output_object_id(bdsp.get_id()); update_object_state(bdsp.get_input5());  }
			if (bdsp.get_input6()) { bdsp.get_input6()->remove_output_object_id(bdsp.get_id()); update_object_state(bdsp.get_input6());  }
			if (bdsp.get_input7()) { bdsp.get_input7()->remove_output_object_id(bdsp.get_id()); update_object_state(bdsp.get_input7());  }
			break;
		}
		case Object::NONE:
		default:
			    printf("remove_object implement other objects\n");
			    break;
	}
	int pos = 0;
	for (auto g = objects.begin(); g != objects.end(); ++g)
	{
		out_object = (*g).get();
		if (out_object->get_id() == object.get_id())
		{
			objects.erase(objects.begin() + pos);
			break;
		}
		pos++;
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
	Object *object = nullptr;
	selected_input.object = nullptr;
	selected_input.input = -1;
	if (selected_object_type != Object::NONE)
	{
		/* add new object */
		switch (selected_object_type)
		{
			case Object::GATE:
			{
				std::unique_ptr<Gate> gate(new Gate(selected_gate_type, ev->last_x-70/2, ev->last_y-50/2, 70, 50));
				selected_object = gate.get();
				objects.push_back(std::move(gate));
				selected_object_type = Object::NONE;
				break;
			}
			case Object::BINARYDISPLAY:
			{
				std::unique_ptr<class BinaryDisplay> binarydisplay(new class BinaryDisplay(ev->last_x-70/2, ev->last_y-50/2, 50, 100));
				selected_object = binarydisplay.get();
				objects.push_back(std::move(binarydisplay));
				selected_object_type = Object::NONE;
				break;
			}
			case Object::NONE:
			default:
				printf("Object not implemented. lmouse down\n");
		}
	}
	else
	{
		/* do other things */

		/* get object at cursor */
		object = find_object_at(ev->last_x, ev->last_y);
		if (object)
		{
			if (object && selected_objects.empty())
			{
				/* if we found an object, select it */
				selected_object = object;
				if (lshift_down)
				{
					dragging_link = true;
				}
				update_object_state(object);
			}
		}
		else
		{
			selected_object = nullptr;
			selected_input.object = nullptr;
			selected_input.input = -1;
		}

		if (selected_object)
		{
			/* check if the user clicked on an input and select it */
			find_selected_input(ev->last_x, ev->last_y);
			if (selected_input.object != nullptr)
			{
				/* an input is selected */
			}
		}
		else if (selected_objects.empty()) // TODO: maybe we want to allow rubberbanding when gates are already selected?
		{
			rubberbanding = true;
			rubberband_startx = ev->last_x;
			rubberband_starty = ev->last_y;
		}

		if (!selected_objects.empty())
		{
			if (!object)
			{
				selected_objects.clear();
			}
			else
			{
				Gate *selgate;
				bool found_gate = false;
				/* clear selection if we're not clicking on a selected gate */
				for (auto g = selected_objects.begin(); g != selected_objects.end(); ++g)
				{
					selgate = (Gate*)(*g);
					if (object->get_id() == selgate->get_id())
					{
						found_gate = true;
					}
				}
				
				if (!found_gate)
				{
					selected_objects.clear();
				}
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
	if (lshift_down && dragging_link && selected_object)
	{
		Object *object;
		object = find_object_at(ev->last_x, ev->last_y);
		if (object)
		{
			if (object == selected_object) /* objects cannot connect to themselves, probably */
				return 1;
			switch (object->get_object_type())
			{
				case Object::GATE:
				{
					Gate *gate = (Gate*)object;
					if (gate && gate->get_gate_type() != Gate::INPUT)
					{
						int input = -1;
						if (ev->last_y-gate->get_y() <= gate->get_height()/2)
							input = 1;
						else
							input = 2;
						if (gate->get_gate_type() != Gate::NOT && gate->get_gate_type() != Gate::OUTPUT)
						{
							printf("connecting gate %d with gate %d at input #%d\n", selected_object->get_id(), gate->get_id(), input);
							if (input == 1)
							{
								gate->set_input_gate1((Gate*)selected_object);
							}
							else if (input == 2)
							{
								gate->set_input_gate2((Gate*)selected_object);
							}
						}
						else
						{
							/* NOT,NOR,OUTPUT gates needs a special case */
							if (input == 1 || input == 2)
							{
								printf("connecting gate %d with gate %d at input #1\n", selected_object->get_id(), gate->get_id());
								gate->set_input_gate1((Gate*)selected_object);
							}
						}
						selected_object->add_output_object_id(gate->get_id());
						update_object_state(gate);
					}
					break;
				}
				case Object::BINARYDISPLAY:
				{
					/* connect object to bdsp input */
					class BinaryDisplay *bdsp = (class BinaryDisplay*)object;
					/* figure out which input we're connecting to */
					int input = -1;
					int gap = 13;
					if (ev->last_y-bdsp->get_y() <= gap)
					{
						input = 7;
						puts("input 7");
						bdsp->set_input7(selected_object);
						selected_object->add_output_object_id(bdsp->get_id());
						update_object_state(bdsp);
					}
					else if (ev->last_y-(bdsp->get_y()+(gap * 1)) <= gap)
					{
						input = 6;
						puts("input 6");
						bdsp->set_input6(selected_object);
						selected_object->add_output_object_id(bdsp->get_id());
						update_object_state(bdsp);
					}
					else if (ev->last_y-(bdsp->get_y()+(gap * 2)) <= gap)
					{
						input = 5;
						puts("input 5");
						bdsp->set_input5(selected_object);
						selected_object->add_output_object_id(bdsp->get_id());
						update_object_state(bdsp);
					}
					else if (ev->last_y-(bdsp->get_y()+(gap * 3)) <= gap)
					{
						input = 4;
						puts("input 4");
						bdsp->set_input4(selected_object);
						selected_object->add_output_object_id(bdsp->get_id());
						update_object_state(bdsp);
					}
					else if (ev->last_y-(bdsp->get_y()+(gap * 4)) <= gap)
					{
						input = 3;
						puts("input 3");
						bdsp->set_input3(selected_object);
						selected_object->add_output_object_id(bdsp->get_id());
						update_object_state(bdsp);
					}
					else if (ev->last_y-(bdsp->get_y()+(gap * 5)) <= gap)
					{
						input = 2;
						puts("input 2");
						bdsp->set_input2(selected_object);
						selected_object->add_output_object_id(bdsp->get_id());
						update_object_state(bdsp);
					}
					else if (ev->last_y-(bdsp->get_y()+(gap * 6)) <= gap)
					{
						input = 1;
						puts("input 1");
						bdsp->set_input1(selected_object);
						selected_object->add_output_object_id(bdsp->get_id());
						update_object_state(bdsp);
					}

					else if (ev->last_y-(bdsp->get_y()+(gap * 7)) <= gap)
					{
						input = 0;
						puts("input 0");
						bdsp->set_input0(selected_object);
						selected_object->add_output_object_id(bdsp->get_id());
						update_object_state(bdsp);
					}


					break;
				}
				case Object::NONE:
				default:
					printf("not implemented object left down link\n");
					break;
			}
		}
		dragging_link = false;
	}
	if (rubberbanding)
	{
		rubberbanding = false;
		multiple_move_startx = ev->last_x;
		multiple_move_starty = ev->last_y;
		find_gates_in_area(rubberband_startx, rubberband_starty, ev->last_x-rubberband_startx, ev->last_y-rubberband_starty);
		draw();
	}

	return 1;
}

long
MainWindow::on_right_mouse_down(FXObject*, FXSelector, void *ptr)
{
	FXEvent *ev = (FXEvent*)ptr;
	Object *object;
	object = find_object_at(ev->last_x, ev->last_y);
	if (object)
	{
		switch (object->get_object_type())
		{
			case Object::GATE:
			{
				Gate *gate = (Gate*)object;
				if (gate->get_gate_type() == Gate::INPUT)
				{
					/* toggle state */
					gate->set_state(!gate->get_output_state());
				}
				break;
			}
			case Object::NONE:
			default:
				printf("rmouse down objects not implemented\n");
						break;
		}
		update_object_state(object);
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
			if (selected_input.object != nullptr)
			{
				/* delete link */
				// FIXME make object
				Gate *gate = (Gate*)selected_object;
				switch (selected_input.input)
				{
					case 1:
						gate->get_input_gate1()->remove_output_object_id(selected_object->get_id());
						gate->set_input_gate1(nullptr);
						break;
					case 2:
						gate->get_input_gate2()->remove_output_object_id(selected_object->get_id());
						gate->set_input_gate2(nullptr);
						break;
					default: break;
				}
				update_object_state(selected_object);
				update_object_state(selected_input.object);
				selected_input.object = nullptr;
				selected_input.input = -1;
				draw();
			}
			else if (selected_object)
			{
				remove_object(*selected_object);
				selected_object = nullptr;
			}
			else if (!selected_objects.empty())
			{
				/* deleted multiple gates */
				for (auto g = selected_objects.begin(); g != selected_objects.end(); ++g)
				{
					remove_object(*(*g));
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
	FXEvent* event = (FXEvent*)ptr;
	if (lmouse_down && !dragging_link && selected_object)
	{
		Coord currentPos { event->last_x, event->last_y };
		auto diff = currentPos - lastPos;

		selected_object->set_x(selected_object->get_x() + diff.X);
		selected_object->set_y(selected_object->get_y() + diff.Y);
	}

	else if (lmouse_down && !dragging_link && !selected_objects.empty())
	{
		Coord currentPos { event->last_x, event->last_y };
		auto diff = currentPos - lastPos;

		/* moving multiple gates */
		for (auto* gate : selected_objects)
		{
			int gx = gate->get_x();
			int gy = gate->get_y();
			gate->set_x(diff.X + gx);
			gate->set_y(diff.Y + gy);
		}
	}

	if (lmouse_down)
		draw();

	lastPos = { event->last_x, event->last_y };

	return 1;
}

long
MainWindow::input_button_press(FXObject *sender, FXSelector sel, void *ptr)
{
	selected_object = nullptr;
	selected_object_type = Object::GATE;
	selected_gate_type = Gate::INPUT;
	return 1;
}

long
MainWindow::output_button_press(FXObject *sender, FXSelector sel, void *ptr)
{
	selected_object = nullptr;
	selected_object_type = Object::GATE;
	selected_gate_type = Gate::OUTPUT;
	return 1;
}

long
MainWindow::and_button_press(FXObject *sender, FXSelector sel, void *ptr)
{
	selected_object = nullptr;
	selected_object_type = Object::GATE;
	selected_gate_type = Gate::AND;
	return 1;
}

long
MainWindow::nand_button_press(FXObject *sender, FXSelector sel, void *ptr)
{
	selected_object = nullptr;
	selected_object_type = Object::GATE;
	selected_gate_type = Gate::NAND;
	return 1;
}

long
MainWindow::or_button_press(FXObject *sender, FXSelector sel, void *ptr)
{
	selected_object = nullptr;
	selected_object_type = Object::GATE;
	selected_gate_type = Gate::OR;
	return 1;
}

long
MainWindow::xor_button_press(FXObject *sender, FXSelector sel, void *ptr)
{
	selected_object = nullptr;
	selected_object_type = Object::GATE;
	selected_gate_type = Gate::XOR;
	return 1;
}

long
MainWindow::nor_button_press(FXObject *sender, FXSelector sel, void *ptr)
{
	selected_object = nullptr;
	selected_object_type = Object::GATE;
	selected_gate_type = Gate::NOR;
	return 1;
}

long
MainWindow::xnor_button_press(FXObject *sender, FXSelector sel, void *ptr)
{
	selected_object = nullptr;
	selected_object_type = Object::GATE;
	selected_gate_type = Gate::XNOR;
	return 1;
}

long
MainWindow::not_button_press(FXObject *sender, FXSelector sel, void *ptr)
{
	selected_object = nullptr;
	selected_object_type = Object::GATE;
	selected_gate_type = Gate::NOT;
	return 1;
}

long
MainWindow::binarydisplay_button_press(FXObject *sender, FXSelector sel, void *ptr)
{
	selected_object = nullptr;
	selected_object_type = Object::BINARYDISPLAY;
	return 1;
}


long
MainWindow::save_button_press(FXObject *sender, FXSelector sel, void *ptr)
{
	save_file();
	return 1;
}

long
MainWindow::load_button_press(FXObject *sender, FXSelector sel, void *ptr)
{
	load_file();
	return 1;
}
