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
	FXMAPFUNC(SEL_COMMAND, MainWindow::ID_BUTTON_PULSE, MainWindow::pulse_button_press),
	FXMAPFUNC(SEL_COMMAND, MainWindow::ID_BUTTON_OUTPUT, MainWindow::output_button_press),
	FXMAPFUNC(SEL_COMMAND, MainWindow::ID_BUTTON_AND, MainWindow::and_button_press),
	FXMAPFUNC(SEL_COMMAND, MainWindow::ID_BUTTON_NAND, MainWindow::nand_button_press),
	FXMAPFUNC(SEL_COMMAND, MainWindow::ID_BUTTON_3NAND, MainWindow::nand3_button_press),
	FXMAPFUNC(SEL_COMMAND, MainWindow::ID_BUTTON_OR, MainWindow::or_button_press),
	FXMAPFUNC(SEL_COMMAND, MainWindow::ID_BUTTON_NOR, MainWindow::nor_button_press),
	FXMAPFUNC(SEL_COMMAND, MainWindow::ID_BUTTON_XOR, MainWindow::xor_button_press),
	FXMAPFUNC(SEL_COMMAND, MainWindow::ID_BUTTON_XNOR, MainWindow::xnor_button_press),
	FXMAPFUNC(SEL_COMMAND, MainWindow::ID_BUTTON_NOT, MainWindow::not_button_press),
	FXMAPFUNC(SEL_COMMAND, MainWindow::ID_BUTTON_BINARYDISPLAY, MainWindow::binarydisplay_button_press),

	/* options */
	FXMAPFUNC(SEL_COMMAND, MainWindow::ID_BUTTON_SAVE, MainWindow::save_button_press),
	FXMAPFUNC(SEL_COMMAND, MainWindow::ID_BUTTON_LOAD, MainWindow::load_button_press),

	FXMAPFUNC(SEL_IO_READ, MainWindow::ID_UPDATE_OBJECTS, MainWindow::update_objects),

};
FXIMPLEMENT(MainWindow, FXMainWindow, MainWindow_Map, ARRAYNUMBER(MainWindow_Map))

MainWindow::MainWindow(FXApp *a)
	: FXMainWindow(a, "foxlogicgates", nullptr, nullptr, DECOR_ALL, 0, 0, 500, 500)
{
	app = a;
	create_ui();
	sig = new FXGUISignal(app, this, ID_UPDATE_OBJECTS);
	update_thread = new Thread(this, sig);
	update_thread->start();
}

MainWindow::~MainWindow()
{
	delete update_thread;
	delete sig;
}

void
MainWindow::create()
{
	FXMainWindow::create();
	INPUT_icon->create();
	PULSE_icon->create();
	OUTPUT_icon->create();
	AND_icon->create();
	NAND_icon->create();
	NAND3_icon->create();
	OR_icon->create();
	NOR_icon->create();
	XOR_icon->create();
	XNOR_icon->create();
	NOT_icon->create();
	BinaryDisplay_icon->create();
	canvas_image->create();
	show(PLACEMENT_SCREEN);
	ready_to_draw = true;
}

void
MainWindow::create_ui()
{
	contents = new FXHorizontalFrame(this, LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y);

	/* icons */

	INPUT_icon = new FXGIFIcon(app, INPUT_icon_data, IMAGE_KEEP);
	PULSE_icon = new FXGIFIcon(app, PULSE_icon_data, IMAGE_KEEP);
	OUTPUT_icon = new FXGIFIcon(app, OUTPUT_icon_data, IMAGE_KEEP);
	AND_icon = new FXGIFIcon(app, AND_icon_data, IMAGE_KEEP);
	NAND_icon = new FXGIFIcon(app, NAND_icon_data, IMAGE_KEEP);
	NAND3_icon = new FXGIFIcon(app, NAND3_icon_data, IMAGE_KEEP);
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
	new FXButton(toolsFrame, "", PULSE_icon, this, MainWindow::ID_BUTTON_PULSE, BUTTON_NORMAL|LAYOUT_FILL_X);
	new FXButton(toolsFrame, "", OUTPUT_icon, this, MainWindow::ID_BUTTON_OUTPUT, BUTTON_NORMAL|LAYOUT_FILL_X);
	new FXButton(toolsFrame, "AND", AND_icon, this, MainWindow::ID_BUTTON_AND, BUTTON_NORMAL|LAYOUT_FILL_X);
	new FXButton(toolsFrame, "NAND", NAND_icon, this, MainWindow::ID_BUTTON_NAND, BUTTON_NORMAL|LAYOUT_FILL_X);
	new FXButton(toolsFrame, "3 NAND", NAND3_icon, this, MainWindow::ID_BUTTON_3NAND, BUTTON_NORMAL|LAYOUT_FILL_X);
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
	lock.lock();
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

					case Gate::PULSE:
					{
						if (gate1->get_output_state() == true)
						{
							/* input is switched on, indicate so */
							dc_image.setForeground(FXRGB(255, 255, 0));
							dc_image.fillRectangle(gate1->get_x(), gate1->get_y(), gate1->get_width(), gate1->get_height());
							dc_image.setForeground(FXRGB(0,0,0));
						}
						dc_image.drawIcon(PULSE_icon, gate1->get_x(), gate1->get_y());
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

					case Gate::NAND3:
					{
						dc_image.drawIcon(NAND3_icon, gate1->get_x(), gate1->get_y());
						dc_image.drawText(gate1->get_x(), gate1->get_y()+gate1->get_height()+20, "3 Input NAND");
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
				Object *in_gate1 = gate1->get_input_gate1();
				Object *in_gate2 = gate1->get_input_gate2();
				Object *in_gate3 = gate1->get_input_gate3();
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
					/* nand3 gate does not need a special case for the first input */
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
					else if (gate1->get_gate_type() == Gate::NAND3)
					{
						/* special case for 3 input gates */
						dc_image.drawLine(in_gate2->get_x()+in_gate2->get_width()-5, in_gate2->get_y()+(in_gate2->get_height()/2),
								gate1->get_x()+10, gate1->get_y()+25);
						dc_image.setForeground(FXRGB(0, 0, 0));
					}
					else
					{

						dc_image.drawLine(in_gate2->get_x()+in_gate2->get_width()-5, in_gate2->get_y()+(in_gate2->get_height()/2),
								gate1->get_x()+10, gate1->get_y()+43);
						dc_image.setForeground(FXRGB(0, 0, 0));
					}
				}
				if (in_gate3 != nullptr)
				{
					/* special case for drawing third input gate */
					dc_image.drawLine(in_gate3->get_x()+in_gate3->get_width()-5, in_gate3->get_y()+(in_gate3->get_height()/2),
							gate1->get_x()+10, gate1->get_y()+43);
						dc_image.setForeground(FXRGB(0, 0, 0));
				}
				dc_image.setForeground(FXRGB(0, 0, 0));
				break;
		}
			case Object::BINARYDISPLAY:
			{
				class BinaryDisplay *bdsp = (class BinaryDisplay*)(*g1).get();
				Object *input;
				if ((input = bdsp->get_input0()))
				{
					if (input == selected_input.object) { dc_image.setForeground(FXRGB(255, 0, 0)); }

					dc_image.drawLine(input->get_x()+input->get_width()-5, input->get_y()+(input->get_height()/2),
								bdsp->get_x()+10, bdsp->get_y()+90);
				}
				if ((input = bdsp->get_input1()))
				{
					if (input == selected_input.object) { dc_image.setForeground(FXRGB(255, 0, 0)); }
					dc_image.drawLine(input->get_x()+input->get_width()-5, input->get_y()+(input->get_height()/2),
								bdsp->get_x()+10, bdsp->get_y()+80);
				}
				if ((input = bdsp->get_input2()))
				{
					if (input == selected_input.object) { dc_image.setForeground(FXRGB(255, 0, 0)); }
					dc_image.drawLine(input->get_x()+input->get_width()-5, input->get_y()+(input->get_height()/2),
								bdsp->get_x()+10, bdsp->get_y()+66);
				}
				if ((input = bdsp->get_input3()))
				{
					if (input == selected_input.object) { dc_image.setForeground(FXRGB(255, 0, 0)); }
					dc_image.drawLine(input->get_x()+input->get_width()-5, input->get_y()+(input->get_height()/2),
								bdsp->get_x()+10, bdsp->get_y()+55);
				}
				if ((input = bdsp->get_input4()))
				{
					if (input == selected_input.object) { dc_image.setForeground(FXRGB(255, 0, 0)); }
					dc_image.drawLine(input->get_x()+input->get_width()-5, input->get_y()+(input->get_height()/2),
								bdsp->get_x()+10, bdsp->get_y()+44);
				}
				if ((input = bdsp->get_input5()))
				{
					if (input == selected_input.object) { dc_image.setForeground(FXRGB(255, 0, 0)); }
					dc_image.drawLine(input->get_x()+input->get_width()-5, input->get_y()+(input->get_height()/2),
								bdsp->get_x()+10, bdsp->get_y()+33);
				}
				if ((input = bdsp->get_input6()))
				{
					if (input == selected_input.object) { dc_image.setForeground(FXRGB(255, 0, 0)); }
					dc_image.drawLine(input->get_x()+input->get_width()-5, input->get_y()+(input->get_height()/2),
								bdsp->get_x()+10, bdsp->get_y()+22);
				}
				if ((input = bdsp->get_input7()))
				{
					if (input == selected_input.object) { dc_image.setForeground(FXRGB(255, 0, 0)); }
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
		dc_image.setForeground(FXRGB(0, 0, 0));
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
	lock.unlock();
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

long
MainWindow::update_objects(FXObject*,FXSelector,void* ptr)
{
	lock.lock();
	Object *object;
	for(auto o = objects.begin(); o != objects.end(); ++o)
	{
		object = (*o).get();
		object->update_state();
	}
	if (ready_to_draw)
	{
		lock.unlock();
		draw();
		lock.lock();
	}
	lock.unlock();
	return 1;
}

void
MainWindow::update_object_state(Object *object)
{
	return;
	object->update_state();
	/* update all objects that are using this object as an input */
	Object *object2;
	auto output_objects = object->get_output_objects();
	for(auto o = output_objects->begin(); o != output_objects->end(); ++o)
	{
		object2 = find_object_by_id((*o));
		if (object2)
		{
			update_object_state(object2);
		}
	}
}

void
MainWindow::find_selected_input(int x, int y)
{
	Object *input_object = nullptr;

	Object *object = selected_object;
	if (!selected_object)
	{
		selected_input.object = nullptr;
		selected_input.input = -1;
		return;
	}
	int input = -1;

	if (x >= selected_object->get_x() && x <= selected_object->get_x() + 20)
	{
		switch (selected_object->get_object_type())
		{
			case Object::GATE:
			{
				Gate *gate = (Gate*)object;
				switch (gate->get_gate_type())
				{
					case Gate::NAND3:
					{
						puts("nand");
						/* 3 input gates */
						int relypos = y-gate->get_y();
						if (relypos <= 16) 		   { input = 1; input_object = gate->get_input_gate1(); }
						if (relypos >16 && relypos <= 34)  { input = 2; input_object = gate->get_input_gate2(); }
						if (relypos >34)  		   { input = 3; input_object = gate->get_input_gate3(); }
						break;
					}
					default:
					{
						/* 2 or 1 input gates */
						if (y-selected_object->get_y() <= selected_object->get_height()/2)
						{ input = 1; } else { input = 2; }
						switch (input)
						{
							case 1:
							{
								if (gate->get_input_gate1() != nullptr)
								{
									input_object = gate->get_input_gate1();
								}
								break;
							}

							case 2:
							{
								if (gate->get_input_gate2() != nullptr)
								{
									input_object = gate->get_input_gate2();
								}
								else
								{
									/* special check for gates with only one input */
									if (gate->get_gate_type() == Gate::NOT || gate->get_gate_type() == Gate::OUTPUT)
									{
										input_object = gate->get_input_gate1();
										input = 1;
									}
								}
								break;
							}

							default:
								input = -1;
								break;
						}
					break;
					}
				}
				break;
			}
			case Object::BINARYDISPLAY:
			{
				puts("bdsp");
				BinaryDisplay *bdsp = (BinaryDisplay*)object;
				int relypos = y-bdsp->get_y();
				if (relypos <= 15) 		   { input = 7; input_object = bdsp->get_input7(); }
				if (relypos >15 && relypos <= 27)  { input = 6; input_object = bdsp->get_input6(); }
				if (relypos >27 && relypos <= 39)  { input = 5; input_object = bdsp->get_input5(); }
				if (relypos >39 && relypos <= 50)  { input = 4; input_object = bdsp->get_input4(); }
				if (relypos >50 && relypos <= 60)  { input = 3; input_object = bdsp->get_input3(); }
				if (relypos >60 && relypos <= 72)  { input = 2; input_object = bdsp->get_input2(); }
				if (relypos >72 && relypos <= 84)  { input = 1; input_object = bdsp->get_input1(); }
				if (relypos >84 && relypos <= 100) { input = 0; input_object = bdsp->get_input0(); }
				break;
			}
			case Object::NONE:
			default:
				printf("find_selected_input() object not implemented\n");
				break;
		}

		if (input_object != nullptr)
		{
			selected_input.object = input_object;
			selected_input.input = input;
			printf("selected input #%d of object id %d\n", input, selected_object->get_id());
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

	pugi::xml_document doc;
	auto declarationNode = doc.append_child(pugi::node_declaration);
	declarationNode.append_attribute("version")    = "1.0";
	declarationNode.append_attribute("encoding")   = "ISO-8859-1";
	declarationNode.append_attribute("standalone") = "yes";

	/* write meta data */
	auto meta = doc.append_child("Meta");
	pugi::xml_node info_xml = meta.append_child("Info");
	info_xml.append_attribute("next_id") = Object::get_object_id_counter();

	auto root = doc.append_child("Objects");

	/* iterate through all objects and add child nodes */
	Object *object;
	for (auto g = objects.begin(); g != objects.end(); ++g)
	{
		object = (*g).get();
		switch (object->get_object_type())
		{
			case Object::GATE:
			{
				pugi::xml_node object_xml = root.append_child("Gate");
				Gate *gate;
				gate = (Gate*)object;
				object_xml.append_attribute("id") = object->get_id();
				object_xml.append_attribute("object_type") = object->get_object_type();
				object_xml.append_attribute("x") = object->get_x();
				object_xml.append_attribute("y") = object->get_y();
				object_xml.append_attribute("w") = object->get_width();
				object_xml.append_attribute("h") = object->get_height();
				object_xml.append_attribute("gate_type") = gate->get_gate_type();
				if (gate->get_input_gate1())
					object_xml.append_attribute("input1_id") = gate->get_input_gate1()->get_id();
				if (gate->get_input_gate2())
					object_xml.append_attribute("input2_id") = gate->get_input_gate2()->get_id();
				if (gate->get_input_gate3())
					object_xml.append_attribute("input3_id") = gate->get_input_gate3()->get_id();
				object_xml.append_attribute("output_state") = gate->get_output_state();

				/* iterate through all output gates and write them */
				for(auto id = gate->get_output_objects()->begin(); id != gate->get_output_objects()->end(); ++id)
				{
					auto outid_node = object_xml.append_child("output_object_id");
					outid_node.append_attribute("id") = (*id);
				}
				break;
			}
			case Object::BINARYDISPLAY:
			{
				pugi::xml_node object_xml = root.append_child("BinaryDisplay");
				BinaryDisplay *bdsp = (BinaryDisplay*)object;
				object_xml.append_attribute("id") = object->get_id();
				object_xml.append_attribute("object_type") = object->get_object_type();
				object_xml.append_attribute("x") = object->get_x();
				object_xml.append_attribute("y") = object->get_y();
				object_xml.append_attribute("w") = object->get_width();
				object_xml.append_attribute("h") = object->get_height();
				if (bdsp->get_input0()) object_xml.append_attribute("input0_id") = bdsp->get_input0()->get_id();
				if (bdsp->get_input1()) object_xml.append_attribute("input1_id") = bdsp->get_input1()->get_id();
				if (bdsp->get_input2()) object_xml.append_attribute("input2_id") = bdsp->get_input2()->get_id();
				if (bdsp->get_input3()) object_xml.append_attribute("input3_id") = bdsp->get_input3()->get_id();
				if (bdsp->get_input4()) object_xml.append_attribute("input4_id") = bdsp->get_input4()->get_id();
				if (bdsp->get_input5()) object_xml.append_attribute("input5_id") = bdsp->get_input5()->get_id();
				if (bdsp->get_input6()) object_xml.append_attribute("input6_id") = bdsp->get_input6()->get_id();
				if (bdsp->get_input7()) object_xml.append_attribute("input7_id") = bdsp->get_input7()->get_id();
				object_xml.append_attribute("output_state") = bdsp->get_output_state();

				/* iterate through all output gates and write them */
				for(auto id = bdsp->get_output_objects()->begin(); id != bdsp->get_output_objects()->end(); ++id)
				{
					auto outid_node = object_xml.append_child("output_object_id");
					outid_node.append_attribute("id") = (*id);
				}
				break;
			}
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

	int next_object_id = 0;
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(file_name.c_str(),
			            pugi::parse_default|pugi::parse_declaration);
	if (!result)
	{
		/* failed reading file */
		return false;
	}

	remove_all_objects();

	/* read meta info */
	auto meta_objects = doc.child("Meta");
	next_object_id = meta_objects.child("Info").attribute("next_id").as_int();
	printf("next object id after loading is: %d\n", next_object_id);

	auto node_objects = doc.child("Objects");

	/* read gates */
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
		type = (Gate::GATE_TYPE)node.attribute("gate_type").as_int();
		output_state = node.attribute("output_state").as_bool();
		std::vector<int> output_gate_ids;
		printf("new gate with id %d\n", id);
		std::unique_ptr<Gate> gate(new Gate(type, x, y, w, h, id));
		gate->set_state(output_state);

		/* read output gate ids */
		for (auto output_node: node.children("output_object_id"))
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

	/* read binary displays */
	for (auto node: node_objects.children("BinaryDisplay"))
	{
		int id, x, y, w, h;
		bool output_state;

		id = node.attribute("id").as_int();
		x = node.attribute("x").as_int();
		y = node.attribute("y").as_int();
		w = node.attribute("w").as_int();
		h = node.attribute("h").as_int();
		output_state = node.attribute("output_state").as_bool();
		std::vector<int> output_object_ids;
		printf("new bdsp with id %d\n", id);
		std::unique_ptr<BinaryDisplay> bdsp(new BinaryDisplay(x, y, w, h, id));
		bdsp->set_state(output_state);

		/* read output bdsp ids */
		for (auto output_node: node.children("output_object_id"))
		{
			int out_id = output_node.attribute("id").as_int();
			output_object_ids.push_back(out_id);
		}

		/*set output ids */
		for (auto idout =  output_object_ids.begin(); idout != output_object_ids.end(); ++idout)
		{
			bdsp->add_output_object_id((*idout));
		}

		objects.push_back(std::move(bdsp));
	}


	/* iterate again through all gates in the xml file and set input gates if they exist */
	for (auto node: node_objects.children("Gate"))
	{
		int input1 = -1;
		int input2 = -1;
		int input3 = -1;
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
		if (strcmp(node.attribute("input3_id").as_string(), "") != 0)
		{
			input3 = node.attribute("input3_id").as_int();
			printf("input 3 exists: %d\n", input2);
		}


		gate = (Gate*)find_object_by_id(node.attribute("id").as_int());

		/* set inputs */
		if (input1 != -1)
		{
			gate->set_input_gate1(find_object_by_id(input1));
		}

		if (input2 != -1)
		{
			gate->set_input_gate2(find_object_by_id(input2));
		}

		if (input3 != -1)
		{
			gate->set_input_gate3(find_object_by_id(input3));
		}

	}

	/* iterate again through all bdsp's in the xml file and set output objects if they exist */
	for (auto node: node_objects.children("BinaryDisplay"))
	{
		int input0 = -1;
		int input1 = -1;
		int input2 = -1;
		int input3 = -1;
		int input4 = -1;
		int input5 = -1;
		int input6 = -1;
		int input7 = -1;
		BinaryDisplay *bdsp;

		if (strcmp(node.attribute("input0_id").as_string(), "") != 0) { input0 = node.attribute("input0_id").as_int();
			printf("input 0 exists: %d\n", input0); }
		if (strcmp(node.attribute("input1_id").as_string(), "") != 0) { input1 = node.attribute("input1_id").as_int();
			printf("input 1 exists: %d\n", input1); }
		if (strcmp(node.attribute("input2_id").as_string(), "") != 0) { input2 = node.attribute("input2_id").as_int();
			printf("input 2 exists: %d\n", input2); }
		if (strcmp(node.attribute("input3_id").as_string(), "") != 0) { input3 = node.attribute("input3_id").as_int();
			printf("input 3 exists: %d\n", input3); }
		if (strcmp(node.attribute("input4_id").as_string(), "") != 0) { input4 = node.attribute("input4_id").as_int();
			printf("input 4 exists: %d\n", input4); }
		if (strcmp(node.attribute("input5_id").as_string(), "") != 0) { input5 = node.attribute("input5_id").as_int();
			printf("input 5 exists: %d\n", input5); }
		if (strcmp(node.attribute("input6_id").as_string(), "") != 0) { input6 = node.attribute("input6_id").as_int();
			printf("input 6 exists: %d\n", input6); }
		if (strcmp(node.attribute("input7_id").as_string(), "") != 0) { input7 = node.attribute("input7_id").as_int();
			printf("input 7 exists: %d\n", input7); }
	

		bdsp = (BinaryDisplay*)find_object_by_id(node.attribute("id").as_int());

		/* set inputs */
		if (input0 != -1) { bdsp->set_input0(find_object_by_id(input0)); }
		if (input1 != -1) { bdsp->set_input1(find_object_by_id(input1)); }
		if (input2 != -1) { bdsp->set_input2(find_object_by_id(input2)); }
		if (input3 != -1) { bdsp->set_input3(find_object_by_id(input3)); }
		if (input4 != -1) { bdsp->set_input4(find_object_by_id(input4)); }
		if (input5 != -1) { bdsp->set_input5(find_object_by_id(input5)); }
		if (input6 != -1) { bdsp->set_input6(find_object_by_id(input6)); }
		if (input7 != -1) { bdsp->set_input7(find_object_by_id(input7)); }
		bdsp->update_state();
	}
	

	/* set object id counter */
	Object::set_object_id_counter(next_object_id);
	draw();
	return true;
}

void
MainWindow::remove_all_objects()
{
	objects.clear();
	selected_object = nullptr;
	selected_input.object = nullptr;
	selected_input.input = -1;
	Object::set_object_id_counter(0);
	draw();
}

void
MainWindow::find_objects_in_area(int x, int y, int width, int height)
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
			if (gate.get_input_gate3())
			{
				gate.get_input_gate3()->remove_output_object_id(gate.get_id());
				update_object_state(gate.get_input_gate3());
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
	/* delete outputs */
	for(auto g = object.get_output_objects()->begin(); g != object.get_output_objects()->end(); ++g)
	{
		out_object = find_object_by_id((*g));
		if (!out_object)
			continue;
		out_object->remove_input_object(object.get_id());
		update_object_state(out_object);
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
						switch (gate->get_gate_type())
						{
							case Gate::NAND3:
							{
								/* case for gates with 3 inputs */
								int relypos = ev->last_y-gate->get_y();
								if (relypos <= 16) 		   { input = 1; }
								if (relypos >16 && relypos <= 34)  { input = 2; }
								if (relypos >34)  		   { input = 3; }

								break;
							}
							default:
							{
								if (ev->last_y-gate->get_y() <= gate->get_height()/2)
									input = 1;
								else
									input = 2;
								break;
							}
						}

						if (gate->get_gate_type() != Gate::NOT && gate->get_gate_type() != Gate::OUTPUT)
						{
							// FIXME: these should be objects, cannot explicitly cast to gates
							printf("connecting gate %d with gate %d at input #%d\n", selected_object->get_id(), gate->get_id(), input);
							if (input == 1)
							{
								gate->set_input_gate1((Gate*)selected_object);
							}
							else if (input == 2)
							{
								gate->set_input_gate2((Gate*)selected_object);
							}
							else if (input == 3)
							{
								gate->set_input_gate3((Gate*)selected_object);
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
		find_objects_in_area(rubberband_startx, rubberband_starty, ev->last_x-rubberband_startx, ev->last_y-rubberband_starty);
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
				if (gate->get_gate_type() == Gate::INPUT || gate->get_gate_type() == Gate::PULSE)
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
				switch (selected_object->get_object_type())
				{
					case Object::GATE:
					{
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
							case 3:
								gate->get_input_gate3()->remove_output_object_id(selected_object->get_id());
								gate->set_input_gate3(nullptr);
								break;

							default: break;
						}
						break;
					}
					case Object::BINARYDISPLAY:
					{
						BinaryDisplay &bdsp = (BinaryDisplay&)*selected_object;
						switch (selected_input.input)
						{
							case 0: bdsp.get_input0()->remove_output_object_id(bdsp.get_id()); bdsp.set_input0(nullptr); break;
							case 1: bdsp.get_input1()->remove_output_object_id(bdsp.get_id()); bdsp.set_input1(nullptr); break;
							case 2: bdsp.get_input2()->remove_output_object_id(bdsp.get_id()); bdsp.set_input2(nullptr); break;
							case 3: bdsp.get_input3()->remove_output_object_id(bdsp.get_id()); bdsp.set_input3(nullptr); break;
							case 4: bdsp.get_input4()->remove_output_object_id(bdsp.get_id()); bdsp.set_input4(nullptr); break;
							case 5: bdsp.get_input5()->remove_output_object_id(bdsp.get_id()); bdsp.set_input5(nullptr); break;
							case 6: bdsp.get_input6()->remove_output_object_id(bdsp.get_id()); bdsp.set_input6(nullptr); break;
							case 7: bdsp.get_input7()->remove_output_object_id(bdsp.get_id()); bdsp.set_input7(nullptr); break;
							default: puts("input not handled in bdsp"); break;
						}
						break;
					}
					case Object::NONE:
					default:
						printf("remove selected input not implemented for object\n");
						break;
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
MainWindow::pulse_button_press(FXObject *sender, FXSelector sel, void *ptr)
{
	selected_object = nullptr;
	selected_object_type = Object::GATE;
	selected_gate_type = Gate::PULSE;
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
MainWindow::nand3_button_press(FXObject *sender, FXSelector sel, void *ptr)
{
	selected_object = nullptr;
	selected_object_type = Object::GATE;
	selected_gate_type = Gate::NAND3;
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
