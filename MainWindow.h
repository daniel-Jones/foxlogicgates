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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <fx.h>
#include <FXScrollArea.h>
#include <FXMessageBox.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include <memory>
#include <string>
#include <fxkeys.h>
#include "Gate.h"
#include "icons.h"

class MainWindow : public FXMainWindow
{
	FXDECLARE(MainWindow)

	public:
		explicit MainWindow(FXApp* a);
		~MainWindow();
		virtual void create();

		/* Event system */
		// Messages for our class
		enum {
			ID_CANVAS       = FXMainWindow::ID_LAST,
			ID_BUTTON_INPUT,
			ID_BUTTON_OUTPUT,
			ID_BUTTON_AND,
			ID_BUTTON_NAND,
			ID_BUTTON_OR,
			ID_BUTTON_NOR,
			ID_BUTTON_XOR,
			ID_BUTTON_XNOR,
			ID_BUTTON_NOT,
		};

		/* Event handlers */
		long on_paint(FXObject*,FXSelector,void* ptr);
		long on_left_mouse_down(FXObject*,FXSelector,void* ptr);
		long on_left_mouse_up(FXObject*,FXSelector,void* ptr);
		long on_right_mouse_down(FXObject*,FXSelector,void* ptr);
		long on_key_press(FXObject*,FXSelector,void* ptr);
		long on_key_release(FXObject*,FXSelector,void* ptr);
		long on_mouse_move(FXObject*,FXSelector,void* ptr);
		/* toolbox */
		long input_button_press(FXObject*,FXSelector,void* ptr);
		long output_button_press(FXObject*,FXSelector,void* ptr);
		long and_button_press(FXObject*,FXSelector,void* ptr);
		long nand_button_press(FXObject*,FXSelector,void* ptr);
		long or_button_press(FXObject*,FXSelector,void* ptr);
		long nor_button_press(FXObject*,FXSelector,void* ptr);
		long xor_button_press(FXObject*,FXSelector,void* ptr);
		long xnor_button_press(FXObject*,FXSelector,void* ptr);
		long not_button_press(FXObject*,FXSelector,void* ptr);
		
		/* selected gate */
		Gate *selected_gate = nullptr;

		FXApp *get_app(){ return app; };


	protected:
		MainWindow(){}

	private:
		void create_ui();
		void draw();
		Gate *find_gate_at(int x, int y);
		Gate *find_gate_by_id(int id);

		FXHorizontalFrame *contents;
		FXVerticalFrame   *canvasFrame;
		FXVerticalFrame   *toolsFrame;
		FXScrollWindow 	*scroll_area;
		FXScrollWindow 	*toolbox_scroll_area;
		FXCanvas *canvas;
		FXBMPImage *canvas_image;
		FXApp *app;

		/* icons */
		FXGIFIcon *INPUT_icon;
		FXGIFIcon *OUTPUT_icon;
		FXGIFIcon *AND_icon;
		FXGIFIcon *NAND_icon;
		FXGIFIcon *OR_icon;
		FXGIFIcon *NOR_icon;
		FXGIFIcon *XOR_icon;
		FXGIFIcon *XNOR_icon;
		FXGIFIcon *NOT_icon;

		/* buttons */
		FXButton *INPUT_button;
		FXButton *OUTPUT_button;
		FXButton *AND_button;
		FXButton *NAND_button;
		FXButton *OR_button;
		FXButton *NOR_button;
		FXButton *XOR_button;
		FXButton *XNOR_button;
		FXButton *NOT_button;

		Gate::GATE_TYPE selected_gate_type = Gate::NONE; // the type of gate we will place

		/* mouse */
		bool lmouse_down = false;
		bool rmouse_down = false;
		bool dragging_link = false;

		/* keyboard */
		bool lshift_down = false;

		/* general */
		std::vector<std::unique_ptr<Gate>> gates;
};

#endif // MAINWINDOW_H
