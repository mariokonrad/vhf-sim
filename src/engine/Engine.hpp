// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef VHFSIM_ENGINE_ENGINE_HPP
#define VHFSIM_ENGINE_ENGINE_HPP

#include <string>

struct lua_State;

namespace vhfsim {
namespace engine {

class View;
class Model;
class Controller;
class ErrorWriter;

enum KeyEvent {
	 EVT_KEY_0       = 0x00000000
	,EVT_KEY_1       = 0x00000001
	,EVT_KEY_2       = 0x00000002
	,EVT_KEY_3       = 0x00000003
	,EVT_KEY_4       = 0x00000004
	,EVT_KEY_5       = 0x00000005
	,EVT_KEY_6       = 0x00000006
	,EVT_KEY_7       = 0x00000007
	,EVT_KEY_8       = 0x00000008
	,EVT_KEY_9       = 0x00000009
	,EVT_KEY_ENTER   = 0x0000000c
	,EVT_KEY_ESC     = 0x0000001b
	,EVT_KEY_F1      = 0x00000100
	,EVT_KEY_F2      = 0x00000200
	,EVT_KEY_F3      = 0x00000300
	,EVT_KEY_F4      = 0x00000400
	,EVT_KEY_F5      = 0x00000500
	,EVT_KEY_F6      = 0x00000600
	,EVT_KEY_F7      = 0x00000700
	,EVT_KEY_F8      = 0x00000800
	,EVT_KEY_F9      = 0x00000900
	,EVT_KEY_F10     = 0x00000a00
	,EVT_KEY_F11     = 0x00000b00
	,EVT_KEY_F12     = 0x00000c00
};

enum MouseButton {
	 MOUSE_NONE   = -1
	,MOUSE_LEFT   = 0
	,MOUSE_MIDDLE = 1
	,MOUSE_RIGHT  = 2
};

class exception
{
private:
	std::string m;

public:
	exception(const std::string & m)
		: m(m)
	{
	}
	inline const std::string & what() const { return m; }
};

class Engine
{
public:
	struct KeyEventTableItem {
		KeyEvent event;
		const char * symbol;
	};
	static const KeyEventTableItem KEY_EVENT_TABLE[24];

private:
	View * view;
	Model * model;
	Controller * controller;
	ErrorWriter * error;
	lua_State * lua;

private:
	void get_main_size(int &, int &);
	void write_error(const std::string &);

public:
	Engine();
	~Engine();
	void reg_view(View *);
	void reg_model(Model *);
	void reg_controller(Controller *);
	void reg_error(ErrorWriter *);
	int get_width();
	int get_height();
	std::string get_vhf_version();
	void init(const std::string &);
	void destroy();
	int event(int);
	void draw();
	void set_exam_mode(bool);

	static std::string get_lua_version();
};
}
}

#endif
