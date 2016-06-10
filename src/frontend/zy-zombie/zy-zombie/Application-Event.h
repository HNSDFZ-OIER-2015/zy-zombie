#pragma once
using namespace std;
namespace App {
	namespace Event {
		//the event application can face and deal
		enum ApplicationEventType : int {
			APPLICATION_EVENT_MOUSE_MOVE,
			APPLICATION_EVENT_MOUSE_CLICK,
			APPLICATION_EVENT_MOUSE_WHEEL,
			APPLICATION_EVENT_BOARD_CLICK,
			APPLICATION_EVENT_WINDOW_SHOW,
			AOOLICATION_EVENT_WINDOW_HIDE,
			APPLICATION_EVENT_WINDOW_CLOSE
		};



		//the parent event struct
		struct ApplicationEventArg {

		};

		//the mouse move event
		struct ApplicationMouseMoveEvent :ApplicationEventArg {
			int x; //move to x
			int y; //move to y
			int last_x; //last x
			int last_y; //last y
		};

		enum ApplicationMouseButton {
			BUTTON_LEFT,
			BUTTON_MIDDLE,
			BUTTON_RIGHT
		};

		//the mouse click event
		struct ApplicationMouseClickEvent :ApplicationEventArg {
			int x; //x
			int y; //y 
			int state;
			ApplicationMouseButton button;
		};

		//the mouse wheel event
		struct ApplicationMouseWheelEvent :ApplicationEventArg {
			int offest; //the wheel offest
						//- is down ,+ is up
		};

		//the keyboard click event
		struct ApplicationKeyBoardEvent :ApplicationEventArg {
			Keycode key; //the keycode 
			bool state; //the keycode state
		};

		struct ApplicationEvent {
			ApplicationEventType type;
			ApplicationEventArg* eventarg;
		};

		//the call back function
		typedef function<void(void*, ApplicationEventArg *)> ApplicationCallBack;
		
	}
}

