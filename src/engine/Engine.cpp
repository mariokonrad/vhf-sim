// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#include <cassert>
#include <cstring>
#include "lua.hpp"
#include "Engine.hpp"
#include "View.hpp"
#include "Model.hpp"
#include "Controller.hpp"
#include "ErrorWriter.hpp"

namespace simradrd68
{
namespace engine
{

/* {{{DISABLED
static int stack_dump(lua_State * lua, FILE * file)
{
	int top = lua_gettop(lua);
	for (int i = top; i >= 1; --i) {
		fprintf(file, "%4d -- ", i);
		switch (lua_type(lua, i)) {
			case LUA_TSTRING:  fprintf(file, "string: [%s]", lua_tostring(lua, i)); break;
			case LUA_TBOOLEAN: fprintf(file, "bool  : %s", lua_toboolean(lua, i) ? "true" :
"false"); break;
			case LUA_TNUMBER:  fprintf(file, "number: %g", lua_tonumber(lua, i)); break;
			case LUA_TTABLE:   fprintf(file, "table"); break;
			default:           fprintf(file, "unkonwn"); break;
		}
		fprintf(file, "\n");
	}
	fprintf(file, "\n");
	return 0;
}
}}} */

// ACCESSORS {{{

static void put(lua_State * lua, void * data, const char * symbol)
{
	lua_pushlightuserdata(lua, data);
	lua_setglobal(lua, symbol);
}

template <class T> static T * get(lua_State * lua, const char * symbol)
{
	assert(lua);
	assert(symbol);
	lua_getglobal(lua, symbol);
	if (!lua_isuserdata(lua, -1))
		return nullptr;
	T * t = static_cast<T *>(lua_touserdata(lua, -1));
	lua_pop(lua, 1);
	return t;
}

static View * get_view(lua_State * lua) { return get<View>(lua, "_VIEW_"); }
static Model * get_model(lua_State * lua) { return get<Model>(lua, "_MODEL_"); }
static Controller * get_controller(lua_State * lua)
{
	return get<Controller>(lua, "_CONTROLLER_");
}

// }}}

// HELPER {{{

static const msg_t * helper_checkmsg(lua_State * lua)
{
	assert(lua);
	if (!lua_islightuserdata(lua, -1))
		lua_error(lua);
	return reinterpret_cast<const msg_t *>(lua_topointer(lua, -1));
}

static void helper_setfield(lua_State * lua, const char * name, const char * value)
{
	lua_pushstring(lua, value);
	lua_setfield(lua, -2, name);
}

static void helper_setfield(lua_State * lua, const char * name, int value)
{
	lua_pushinteger(lua, value);
	lua_setfield(lua, -2, name);
}

// }}}

// GENERAL FUNCTIONS {{{

// bool = gen_vhf_lat_set()
static int lua__gen_vhf_lat_set(lua_State * lua)
{
	assert(lua);
	Model * model = get_model(lua);
	assert(model);
	lua_pushboolean(lua, model->vhf_lat_set());
	return 1;
}

// bool = gen_vhf_lon_set()
static int lua__gen_vhf_lon_set(lua_State * lua)
{
	assert(lua);
	Model * model = get_model(lua);
	assert(model);
	lua_pushboolean(lua, model->vhf_lon_set());
	return 1;
}

// bool = gen_vhf_time_set()
static int lua__gen_vhf_time_set(lua_State * lua)
{
	assert(lua);
	Model * model = get_model(lua);
	assert(model);
	lua_pushboolean(lua, model->vhf_time_set());
	return 1;
}

// gen_vhf_clear_pos_time()
static int lua__gen_vhf_clear_pos_time(lua_State * lua)
{
	assert(lua);
	Model * model = get_model(lua);
	assert(model);
	model->vhf_clear_pos_time();
	return 0;
}

// }}}

// {{{CONTROLLER MANIPULATION

// DSC / MSG FUNCTIONS {{{

// res = msg_send(mmsi, work_channel, power, type, recv, designation)
static int lua__msg_send(lua_State * lua)
{
	assert(lua);
	Controller * controller = get_controller(lua);
	assert(controller);
	Model * model = get_model(lua);
	assert(model);

	// parameters
	MMSI mmsi(luaL_checkstring(lua, -6));
	int work_channel = luaL_checkinteger(lua, -5);
	int power = luaL_checkinteger(lua, -4);
	int type = luaL_checkinteger(lua, -3);
	int recv = luaL_checkinteger(lua, -2);
	int designation = luaL_checkinteger(lua, -1);

	// prepare message
	msg_t m;
	memset(&m, 0, sizeof(m));

	// fill senders information
	model->lat().str(m.lat);
	model->lon().str(m.lon);
	model->time().str(m.time);
	model->mmsi().str(m.mmsi);
	model->group().str(m.group);
	m.channel = 70;
	m.work_channel = work_channel;
	m.power = power;

	m.valid_lat = model->vhf_lat_set();
	m.valid_lon = model->vhf_lon_set();
	m.valid_time = model->vhf_time_set();

	// fill receivers information
	m.dsc.type = type;
	m.dsc.designation = designation;
	switch (recv) {
		case DSC_ALL:
			m.dsc.recv = recv;
			break;

		case DSC_GROUP:
		case DSC_INDIVIDUAL:
			m.dsc.recv = recv;
			mmsi.str(m.dsc.mmsi);
			break;

		default: // unknown
			m.dsc.recv = DSC_INVALID_RECV;
			break;
	}

	// send message
	int res = controller->msg_send(m);

	// return result
	lua_pushinteger(lua, res);
	return 1;
}

// result,msg = msg_recv()
static int lua__msg_recv(lua_State * lua)
{
	assert(lua);
	Controller * controller = get_controller(lua);
	assert(controller);
	msg_t * m = new msg_t;
	msg_init(*m);
	if (controller->msg_recv(*m) < 0) {
		delete m;
		lua_pushboolean(lua, false);
		lua_pushlightuserdata(lua, nullptr);
	} else {
		lua_pushboolean(lua, true);
		lua_pushlightuserdata(lua, m);
	}
	return 2;
}

// msg_destroy(msg)
static int lua__msg_destroy(lua_State * lua)
{
	assert(lua);
	const msg_t * m = helper_checkmsg(lua);
	if (m)
		delete m;
	return 0;
}

// msg = msg_get_tab(msg)
//
// table = {
//     valid_lat,
//     valid_lon,
//     valid_time,
//     lat,
//     lon,
//     time,
//     mmsi,
//     group,
//     channel,
//     work_channel,
//     power,
//     dsc = {
//         type,
//         recv,
//         designation,
//         mmsi,
//     },
// }
static int lua__msg_tab(lua_State * lua)
{
	assert(lua);
	const msg_t * m = helper_checkmsg(lua);
	if (!m) {
		lua_pushnil(lua);
		return 1;
	}

	lua_newtable(lua); // table

	helper_setfield(lua, "vaild_lat", m->valid_lat);
	helper_setfield(lua, "vaild_lon", m->valid_lon);
	helper_setfield(lua, "vaild_time", m->valid_time);

	helper_setfield(lua, "lat", m->lat);
	helper_setfield(lua, "lon", m->lon);
	helper_setfield(lua, "time", m->time);
	helper_setfield(lua, "mmsi", m->mmsi);
	helper_setfield(lua, "group", m->group);
	helper_setfield(lua, "channel", m->channel);
	helper_setfield(lua, "work_channel", m->work_channel);
	helper_setfield(lua, "power", m->power);

	lua_newtable(lua); // dsc

	helper_setfield(lua, "type", m->dsc.type);
	helper_setfield(lua, "recv", m->dsc.recv);
	helper_setfield(lua, "designation", m->dsc.designation);
	helper_setfield(lua, "mmsi", m->dsc.mmsi);

	lua_setfield(lua, -2, "dsc");

	return 1;
}

// bind_msg(event)
static int lua__bind_msg(lua_State * lua)
{
	assert(lua);
	Controller * controller = get_controller(lua);
	assert(controller);

	controller->bind_msg(luaL_checkinteger(lua, -1));
	return 0;
}

// }}}

// TIMER FUNCTIONS {{{

// timer_create(id)
static int lua__timer_create(lua_State * lua)
{
	assert(lua);
	Controller * controller = get_controller(lua);
	assert(controller);
	int id = luaL_checkinteger(lua, -1);
	controller->timer_create(id);
	return 0;
}

// timer_delete(id)
static int lua__timer_delete(lua_State * lua)
{
	assert(lua);
	Controller * controller = get_controller(lua);
	assert(controller);
	int id = luaL_checkinteger(lua, -1);
	controller->timer_delete(id);
	return 0;
}

// timer_start(id, msec)
static int lua__timer_start(lua_State * lua)
{
	assert(lua);
	Controller * controller = get_controller(lua);
	assert(controller);
	int id = luaL_checkinteger(lua, -2);
	int msec = luaL_checkinteger(lua, -1);
	if (msec > 0) {
		controller->timer_start(id, msec, true); // one shot
	}
	return 0;
}

// timer_stop(id)
static int lua__timer_stop(lua_State * lua)
{
	assert(lua);
	Controller * controller = get_controller(lua);
	assert(controller);
	int id = luaL_checkinteger(lua, -1);
	controller->timer_stop(id);
	return 0;
}

// }}}

// BUTTON FUNCTIONS {{{

// bind_btn_circle(mx, my, r, button, id_press, id_release)
static int lua__bind_btn_circle(lua_State * lua)
{
	assert(lua);
	Controller * controller = get_controller(lua);
	assert(controller);

	int mx = luaL_checkinteger(lua, -6);
	int my = luaL_checkinteger(lua, -5);
	int r = luaL_checkinteger(lua, -4);
	int b = luaL_checkinteger(lua, -3);
	int id_p = luaL_checkinteger(lua, -2);
	int id_r = luaL_checkinteger(lua, -1);

	controller->bind_button_circle(mx, my, r, b, id_p, id_r);

	return 0;
}

// bind_btn_rect(x0, y0, x1, y1, button, id_press, id_release)
static int lua__bind_btn_rect(lua_State * lua)
{
	assert(lua);
	Controller * controller = get_controller(lua);
	assert(controller);

	int x0 = luaL_checkinteger(lua, -7);
	int y0 = luaL_checkinteger(lua, -6);
	int x1 = luaL_checkinteger(lua, -5);
	int y1 = luaL_checkinteger(lua, -4);
	int b = luaL_checkinteger(lua, -3);
	int id_p = luaL_checkinteger(lua, -2);
	int id_r = luaL_checkinteger(lua, -1);

	controller->bind_button_rect(x0, y0, x1, y1, b, id_p, id_r);

	return 0;
}

// }}}

// KEY FUNCTIONS {{{

struct KeyEventMap {
	const char * key;
	KeyEvent event;
};

// bind_key('key', event_press, event_release)
static int lua__bind_key(lua_State * lua)
{
	static const KeyEventMap KEM[] = {
		{"0", EVT_KEY_0}, {"1", EVT_KEY_1}, {"2", EVT_KEY_2}, {"3", EVT_KEY_3},
		{"4", EVT_KEY_4}, {"5", EVT_KEY_5}, {"6", EVT_KEY_6}, {"7", EVT_KEY_7},
		{"8", EVT_KEY_8}, {"9", EVT_KEY_9}, {"ENTER", EVT_KEY_ENTER}, {"ESC", EVT_KEY_ESC},
		{"F1", EVT_KEY_F1}, {"F2", EVT_KEY_F2}, {"F3", EVT_KEY_F3}, {"F4", EVT_KEY_F4},
		{"F5", EVT_KEY_F5}, {"F6", EVT_KEY_F6}, {"F7", EVT_KEY_F7}, {"F8", EVT_KEY_F8},
		{"F9", EVT_KEY_F9}, {"F10", EVT_KEY_F10}, {"F11", EVT_KEY_F11}, {"F12", EVT_KEY_F12},
	};

	assert(lua);
	Controller * controller = get_controller(lua);
	assert(controller);

	const char * k = luaL_checkstring(lua, -3);
	int ep = luaL_checkinteger(lua, -2);
	int er = luaL_checkinteger(lua, -1);
	for (size_t i = 0; i < sizeof(KEM) / sizeof(KeyEventMap); ++i) {
		if (strcmp(k, KEM[i].key) == 0) {
			controller->bind_key(KEM[i].event, ep, er);
			break;
		}
	}
	return 0;
}

// }}}

// GPS FUNCTIONS {{{

// bind_gps(event)
static int lua__bind_gps(lua_State * lua)
{
	assert(lua);
	Controller * controller = get_controller(lua);
	assert(controller);

	controller->bind_gps(luaL_checkinteger(lua, -1));
	return 0;
}

// }}}

// CONTROLLER MANIPULATION}}}

// {{{VIEW MANIPULATION

// VIEW FUNCTIONS {{{

// view_update()
static int lua__view_update(lua_State * lua)
{
	assert(lua);
	View * view = get_view(lua);
	assert(view);
	view->update_view();
	return 0;
}

// }}}

// DRAW FUNCTIONS {{{

// draw_clear(x, y, w, h)
static int lua__draw_clear(lua_State * lua)
{
	assert(lua);
	View * view = get_view(lua);
	assert(view);
	int x = luaL_checkinteger(lua, -4);
	int y = luaL_checkinteger(lua, -3);
	int w = luaL_checkinteger(lua, -2);
	int h = luaL_checkinteger(lua, -1);

	view->set_clipping_region(x, y, w, h);
	view->clear();
	view->clear_clipping_region();

	return 0;
}

// draw_text(text, x, y, font-id, alignment)
//
// align: left=0, right=1
static int lua__draw_text(lua_State * lua)
{
	assert(lua);
	View * view = get_view(lua);
	assert(view);
	const char * text = luaL_checkstring(lua, -5);
	int x = luaL_checkinteger(lua, -4);
	int y = luaL_checkinteger(lua, -3);
	int font_id = luaL_checkinteger(lua, -2);
	int align = luaL_checkinteger(lua, -1);

	view->set_font(font_id);
	view->draw_text(x, y, text, static_cast<View::TextAlign>(align));
	return 0;
}

// draw_ch(channel, x, y, font-id)
static int lua__draw_ch(lua_State * lua)
{
	assert(lua);
	View * view = get_view(lua);
	assert(view);
	const char * ch = luaL_checkstring(lua, -4);
	int x = luaL_checkinteger(lua, -3);
	int y = luaL_checkinteger(lua, -2);
	int font_id = luaL_checkinteger(lua, -1);
	view->set_font(font_id);
	view->draw_ch(x, y, ch);
	return 0;
}

// draw_img(img-id, x, y)
static int lua__draw_img(lua_State * lua)
{
	assert(lua);
	View * view = get_view(lua);
	assert(view);
	int id = luaL_checkinteger(lua, -3);
	int x = luaL_checkinteger(lua, -2);
	int y = luaL_checkinteger(lua, -1);
	view->draw_img(x, y, id);
	return 0;
}

// draw_bitmap(id, x, y, pixel_width, pixel_height)
static int lua__draw_bitmap(lua_State * lua)
{
	assert(lua);
	View * view = get_view(lua);
	assert(view);
	const char * id = luaL_checkstring(lua, -5);
	int x = luaL_checkinteger(lua, -4);
	int y = luaL_checkinteger(lua, -3);
	int pw = luaL_checkinteger(lua, -2);
	int ph = luaL_checkinteger(lua, -1);
	view->draw_bitmap(id, x, y, pw, ph);
	return 0;
}

static int __getraw(lua_State * lua, int i)
{
	lua_rawgeti(lua, -1, i);
	int r = luaL_checkinteger(lua, -1);
	lua_pop(lua, 1);
	return r;
}

// draw_set_pen(r, g, b)
static int lua__draw_set_pen(lua_State * lua)
{
	assert(lua);
	View * view = get_view(lua);
	assert(view);
	int r, g, b;
	if (lua_istable(lua, -1)) {
		r = __getraw(lua, 1);
		g = __getraw(lua, 2);
		b = __getraw(lua, 3);
	} else {
		r = luaL_checkinteger(lua, -3);
		g = luaL_checkinteger(lua, -2);
		b = luaL_checkinteger(lua, -1);
	}
	view->set_pen(r, g, b);
	view->set_text_foreground(r, g, b);
	return 0;
}

// draw_set_brush(r, g, b)
static int lua__draw_set_brush(lua_State * lua)
{
	assert(lua);
	View * view = get_view(lua);
	assert(view);
	int r, g, b;
	if (lua_istable(lua, -1)) {
		r = __getraw(lua, 1);
		g = __getraw(lua, 2);
		b = __getraw(lua, 3);
	} else {
		r = luaL_checkinteger(lua, -3);
		g = luaL_checkinteger(lua, -2);
		b = luaL_checkinteger(lua, -1);
	}
	view->set_brush(r, g, b);
	view->set_text_background(r, g, b);
	return 0;
}

// draw_set_bg(r, g, b)
static int lua__draw_set_bg(lua_State * lua)
{
	assert(lua);
	View * view = get_view(lua);
	assert(view);
	int r, g, b;
	if (lua_istable(lua, -1)) {
		r = __getraw(lua, 1);
		g = __getraw(lua, 2);
		b = __getraw(lua, 3);
	} else {
		r = luaL_checkinteger(lua, -3);
		g = luaL_checkinteger(lua, -2);
		b = luaL_checkinteger(lua, -1);
	}
	view->set_background(r, g, b);
	return 0;
}

// draw_reg_font(id, size)
static int lua__draw_reg_font(lua_State * lua)
{
	assert(lua);
	View * view = get_view(lua);
	assert(view);
	int id = luaL_checkinteger(lua, -2);
	int size = luaL_checkinteger(lua, -1);

	view->register_font(id, size);
	return 0;
}

// draw_rect(x0, y0, x1, y1)
static int lua__draw_rect(lua_State * lua)
{
	assert(lua);
	View * view = get_view(lua);
	assert(view);
	int x0 = luaL_checkinteger(lua, -4);
	int y0 = luaL_checkinteger(lua, -3);
	int x1 = luaL_checkinteger(lua, -2);
	int y1 = luaL_checkinteger(lua, -1);

	int x = (x0 < x1) ? x0 : x1;
	int y = (y0 < y1) ? y0 : y1;
	int w = (x0 < x1) ? x1 - x0 : x0 - x1;
	int h = (y0 < y1) ? y1 - y0 : y0 - y1;

	view->draw_rectangle(x, y, w, h);
	return 0;
}

// draw_circ(x, y, r)
static int lua__draw_circ(lua_State * lua)
{
	assert(lua);
	View * view = get_view(lua);
	assert(view);
	int x = luaL_checkinteger(lua, -3);
	int y = luaL_checkinteger(lua, -2);
	int r = luaL_checkinteger(lua, -1);

	if (r < 0)
		r = -r;

	view->draw_circle(x, y, r);
	return 0;
}

// }}}

// IMAGE FUNCTIONS {{{

// img_load(id, filename)
static int lua__img_load(lua_State * lua)
{
	assert(lua);
	View * view = get_view(lua);
	assert(view);
	int id = luaL_checkinteger(lua, -2);
	const char * filename = luaL_checkstring(lua, -1);
	view->img_load(id, filename);
	return 0;
}

// width,height = img_get_size(id)
static int lua__img_get_size(lua_State * lua)
{
	assert(lua);
	View * view = get_view(lua);
	assert(view);
	int id = luaL_checkinteger(lua, -1);
	int w = -1;
	int h = -1;
	view->img_size(id, w, h);
	lua_pushinteger(lua, w);
	lua_pushinteger(lua, h);
	return 2;
}

// bitmap_register(id, w, h, char, data)
static int lua__bitmap_register(lua_State * lua)
{
	assert(lua);
	View * view = get_view(lua);
	assert(view);
	const char * id = luaL_checkstring(lua, -5);
	int w = luaL_checkinteger(lua, -4);
	int h = luaL_checkinteger(lua, -3);
	const char * ch = luaL_checkstring(lua, -2);
	const char * data = luaL_checkstring(lua, -1);
	view->bitmap_register(id, w, h, ch[0], data);
	return 0;
}

// bitmap_unregister(id)
static int lua__bitmap_unregister(lua_State * lua)
{
	assert(lua);
	View * view = get_view(lua);
	assert(view);
	const char * id = luaL_checkstring(lua, -1);
	view->bitmap_unregister(id);
	return 0;
}

// }}}

// VIEW MANIPULATION}}}

// {{{MODEL MANIPULATION

// MMSI FUNCTIONS {{{

// mmsi_set_ship(mmsi)
static int lua__mmsi_set_ship(lua_State * lua)
{
	assert(lua);
	Model * model = get_model(lua);
	assert(model);
	model->mmsi(MMSI(luaL_checkstring(lua, -1)));
	return 0;
}

// mmsi = mmsi_get_ship()
static int lua__mmsi_get_ship(lua_State * lua)
{
	assert(lua);
	Model * model = get_model(lua);
	assert(model);
	lua_pushstring(lua, model->mmsi().str().c_str());
	return 1;
}

// mmsi_set_group(mmsi)
static int lua__mmsi_set_group(lua_State * lua)
{
	assert(lua);
	Model * model = get_model(lua);
	assert(model);
	model->group(MMSI(luaL_checkstring(lua, -1)));
	return 0;
}

// mmsi = mmsi_get_group()
static int lua__mmsi_get_group(lua_State * lua)
{
	assert(lua);
	Model * model = get_model(lua);
	assert(model);
	lua_pushstring(lua, model->group().str().c_str());
	return 1;
}

// }}}

// GPS FUNCTIONS {{{

// pos_set_lat(hem, deg, min)
// hem: +1=N, -1=S
static int lua__pos_set_lat(lua_State * lua)
{
	assert(lua);
	Model * model = get_model(lua);
	assert(model);
	int hem = luaL_checkinteger(lua, -3);
	int deg = luaL_checkinteger(lua, -2);
	int min = luaL_checkinteger(lua, -1);
	model->lat(Latitude((hem > 0) ? 'N' : 'S', deg, min)); // ignoring seconds
	return 0;
}

// pos_set_lon(hem, deg, min)
// hem: +1=W, -1=E
static int lua__pos_set_lon(lua_State * lua)
{
	assert(lua);
	Model * model = get_model(lua);
	assert(model);
	int hem = luaL_checkinteger(lua, -3);
	int deg = luaL_checkinteger(lua, -2);
	int min = luaL_checkinteger(lua, -1);
	model->lon(Longitude((hem > 0) ? 'W' : 'E', deg, min)); // ignoring seconds
	return 0;
}

// hem,deg,min = pos_get_lat()
// hem: +1=N, -1=S
static int lua__pos_get_lat(lua_State * lua)
{
	assert(lua);
	Model * model = get_model(lua);
	assert(model);
	Latitude l = model->lat();
	lua_pushinteger(lua, (l.hem() == 'N') ? +1 : -1);
	lua_pushinteger(lua, l.deg());
	lua_pushinteger(lua, l.min());
	return 3;
}

// deg = pos_get_lon()
// hem: +1=W, -1=E
static int lua__pos_get_lon(lua_State * lua)
{
	assert(lua);
	Model * model = get_model(lua);
	assert(model);
	Longitude l = model->lon();
	lua_pushinteger(lua, (l.hem() == 'W') ? +1 : -1);
	lua_pushinteger(lua, l.deg());
	lua_pushinteger(lua, l.min());
	return 3;
}

// }}}

// TIME FUNCTIONS {{{

// time_set(time)
static int lua__time_set(lua_State * lua)
{
	assert(lua);
	Model * model = get_model(lua);
	assert(model);
	float val = luaL_checknumber(lua, -1);
	unsigned short h = static_cast<unsigned short>(floor(val));
	unsigned short m = static_cast<unsigned short>(floor((val - floor(val)) * 60.0 + 0.5));
	model->time(Date(2000, 1, 1, h, m, 0));
	return 0;
}

// time_sethm(hour, minute)
static int lua__time_sethm(lua_State * lua)
{
	assert(lua);
	Model * model = get_model(lua);
	assert(model);
	int h = luaL_checkinteger(lua, -1);
	int m = luaL_checkinteger(lua, -2);
	model->time(Date(2000, 1, 1, h, m, 0));
	return 0;
}

// time = time_get()
static int lua__time_get(lua_State * lua)
{
	assert(lua);
	Model * model = get_model(lua);
	assert(model);
	Date t = model->time();
	float val = static_cast<float>(t.hour()) + static_cast<float>(t.min()) / 60.0f;
	lua_pushnumber(lua, val);
	return 1;
}

// }}}

// DIRECTORY FUNCTIONS {{{

// directory = dir_get()
static int lua__dir_get(lua_State * lua)
{
	assert(lua);
	Model * model = get_model(lua);
	assert(model);
	Directory dir = model->dir_get();
	lua_newtable(lua);
	for (Directory::size_type i = 0; i < dir.size(); ++i) {
		const DirEntry & de = dir[i];
		lua_newtable(lua);
		lua_pushstring(lua, de.name.c_str());
		lua_setfield(lua, -2, "name");
		lua_pushstring(lua, de.mmsi.c_str());
		lua_setfield(lua, -2, "mmsi");
		lua_rawseti(lua, -2, i + 1);
	}
	return 1;
}

// dir_set(directory)
//
// directory = {
//   [1] = { name='...', mmsi='...' },
//   [2] = { name='...', mmsi='...' },
//   [3] = { name='...', mmsi='...' },
//   ...
// }
static int lua__dir_set(lua_State * lua)
{
	assert(lua);
	Model * model = get_model(lua);
	assert(model);
	Directory dir;
	if (!lua_istable(lua, -1))
		return 0;
	for (int i = 1;; ++i) {
		lua_rawgeti(lua, -1, i);
		if (lua_isnil(lua, -1))
			break;
		if (!lua_istable(lua, -1)) {
			lua_pop(lua, 1);
			continue;
		}
		DirEntry de;
		lua_getfield(lua, -1, "name");
		de.name = luaL_checkstring(lua, -1);
		lua_getfield(lua, -2, "mmsi");
		de.mmsi = luaL_checkstring(lua, -1);
		dir.push_back(de);
		lua_pop(lua, 3);
	}
	model->dir_set(dir);
	return 0;
}

// }}}

// MODEL MANIPULATION}}}

// SOUND FUNCTIONS {{{

// snd_destroy()
static int lua__snd_destroy(lua_State * lua)
{
	assert(lua);
	View * view = get_view(lua);
	assert(view);
	view->snd_destroy();
	return 0;
}

// bool success = snd_init(num)
static int lua__snd_init(lua_State * lua)
{
	assert(lua);
	int snd_num = luaL_checkinteger(lua, -1);
	if (snd_num <= 0) {
		lua_pushboolean(lua, true);
		return 1;
	}

	View * view = get_view(lua);
	assert(view);
	if (view->snd_init(snd_num)) {
		view->snd_destroy();
		lua_pushboolean(lua, false);
	} else {
		lua_pushboolean(lua, true);
	}
	return 1;
}

// snd_load_wav(id, filename)
static int lua__snd_load_wav(lua_State * lua)
{
	assert(lua);
	int id = luaL_checkinteger(lua, -2);
	const char * filename = luaL_checkstring(lua, -1);
	View * view = get_view(lua);
	assert(view);
	view->snd_load_wav(id, filename);
	return 0;
}

// snd_play(id, loop)
static int lua__snd_play(lua_State * lua)
{
	assert(lua);
	int id = luaL_checkinteger(lua, -2);
	int loop = luaL_checkinteger(lua, -1);
	View * view = get_view(lua);
	assert(view);
	view->snd_play(id, loop);
	return 0;
}

// snd_stop(id)
static int lua__snd_stop(lua_State * lua)
{
	assert(lua);
	int id = luaL_checkinteger(lua, -1);
	View * view = get_view(lua);
	assert(view);
	view->snd_stop(id);
	return 0;
}

// snd_gain(id, gain)
// gain = 0.0 .. 1.0
static int lua__snd_gain(lua_State * lua)
{
	assert(lua);
	int id = luaL_checkinteger(lua, -2);
	float gain = luaL_checknumber(lua, -1);
	View * view = get_view(lua);
	assert(view);
	view->snd_gain(id, gain);
	return 0;
}

// snd_pitch(id, pitch)
// pitch = 0.0 .. 1.0
static int lua__snd_pitch(lua_State * lua)
{
	assert(lua);
	int id = luaL_checkinteger(lua, -2);
	float pitch = luaL_checknumber(lua, -1);
	View * view = get_view(lua);
	assert(view);
	view->snd_pitch(id, pitch);
	return 0;
}

// }}}

// {{{MISC

// BIT ARITHMETHIC FUNCTIONS {{{

// value = bit_and(value1, value2)  : means in C: value = value1 & value2
static int lua__bit_and(lua_State * lua)
{
	assert(lua);
	int v1 = luaL_checkinteger(lua, -2);
	int v2 = luaL_checkinteger(lua, -1);
	lua_pushinteger(lua, v1 & v2);
	return 1;
}

// value = bit_or(value1, value2)  : means in C: value = value1 | value2
static int lua__bit_or(lua_State * lua)
{
	assert(lua);
	int v1 = luaL_checkinteger(lua, -2);
	int v2 = luaL_checkinteger(lua, -1);
	lua_pushinteger(lua, v1 | v2);
	return 1;
}

// }}}

// MISC}}}

// registers all function defined in this section
static int lua_register_functions(lua_State * lua)
{
	// controller manipulation: timers
	lua_register(lua, "timer_create", lua__timer_create);
	lua_register(lua, "timer_delete", lua__timer_delete);
	lua_register(lua, "timer_start", lua__timer_start);
	lua_register(lua, "timer_stop", lua__timer_stop);

	// controller manipulation: button register
	lua_register(lua, "bind_btn_circle", lua__bind_btn_circle);
	lua_register(lua, "bind_btn_rect", lua__bind_btn_rect);

	// controller manipulation: key bindings
	lua_register(lua, "bind_key", lua__bind_key);

	// controller manipulation: gps bindings
	lua_register(lua, "bind_gps", lua__bind_gps);

	// controller manipulation: message sending and receiving
	lua_register(lua, "msg_send", lua__msg_send);
	lua_register(lua, "msg_recv", lua__msg_recv);
	lua_register(lua, "msg_destroy", lua__msg_destroy);
	lua_register(lua, "msg_tab", lua__msg_tab);
	lua_register(lua, "bind_msg", lua__bind_msg);

	// view manipulation: misc
	lua_register(lua, "view_update", lua__view_update);

	// view manipulation: drawing
	lua_register(lua, "draw_ch", lua__draw_ch);
	lua_register(lua, "draw_text", lua__draw_text);
	lua_register(lua, "draw_clear", lua__draw_clear);
	lua_register(lua, "draw_img", lua__draw_img);
	lua_register(lua, "draw_set_pen", lua__draw_set_pen);
	lua_register(lua, "draw_set_brush", lua__draw_set_brush);
	lua_register(lua, "draw_set_bg", lua__draw_set_bg);
	lua_register(lua, "draw_reg_font", lua__draw_reg_font);
	lua_register(lua, "draw_rect", lua__draw_rect);
	lua_register(lua, "draw_circ", lua__draw_circ);
	lua_register(lua, "draw_bitmap", lua__draw_bitmap);
	lua_register(lua, "img_load", lua__img_load);
	lua_register(lua, "img_get_size", lua__img_get_size);
	lua_register(lua, "bitmap_register", lua__bitmap_register);
	lua_register(lua, "bitmap_unregister", lua__bitmap_unregister);

	// model manipulation: read access
	lua_register(lua, "gen_vhf_lat_set", lua__gen_vhf_lat_set);
	lua_register(lua, "gen_vhf_lon_set", lua__gen_vhf_lon_set);
	lua_register(lua, "gen_vhf_time_set", lua__gen_vhf_time_set);
	lua_register(lua, "gen_vhf_clear_pos_time", lua__gen_vhf_clear_pos_time);

	// model manipulation: identification
	lua_register(lua, "mmsi_set_ship", lua__mmsi_set_ship);
	lua_register(lua, "mmsi_get_ship", lua__mmsi_get_ship);
	lua_register(lua, "mmsi_set_group", lua__mmsi_set_group);
	lua_register(lua, "mmsi_get_group", lua__mmsi_get_group);

	// model manipulation: position
	lua_register(lua, "pos_set_lat", lua__pos_set_lat);
	lua_register(lua, "pos_set_lon", lua__pos_set_lon);
	lua_register(lua, "pos_get_lat", lua__pos_get_lat);
	lua_register(lua, "pos_get_lon", lua__pos_get_lon);

	// model manipulation: time
	lua_register(lua, "time_get",   lua__time_get);
	lua_register(lua, "time_set",   lua__time_set);
	lua_register(lua, "time_sethm", lua__time_sethm);

	// model manipulation: directory
	lua_register(lua, "dir_get", lua__dir_get);
	lua_register(lua, "dir_set", lua__dir_set);

	// sound
	lua_register(lua, "snd_init", lua__snd_init);
	lua_register(lua, "snd_destroy", lua__snd_destroy);
	lua_register(lua, "snd_load_wav", lua__snd_load_wav);
	lua_register(lua, "snd_play", lua__snd_play);
	lua_register(lua, "snd_stop", lua__snd_stop);
	lua_register(lua, "snd_gain", lua__snd_gain);
	lua_register(lua, "snd_pitch", lua__snd_pitch);

	// misc
	lua_register(lua, "bit_and", lua__bit_and);
	lua_register(lua, "bit_or", lua__bit_or);
	return 0;
}

// --------------------------------------------------------

const Engine::KeyEventTableItem Engine::KEY_EVENT_TABLE[24] = {
	{ EVT_KEY_0       , "0"     },
	{ EVT_KEY_1       , "1"     },
	{ EVT_KEY_2       , "2"     },
	{ EVT_KEY_3       , "3"     },
	{ EVT_KEY_4       , "4"     },
	{ EVT_KEY_5       , "5"     },
	{ EVT_KEY_6       , "6"     },
	{ EVT_KEY_7       , "7"     },
	{ EVT_KEY_8       , "8"     },
	{ EVT_KEY_9       , "9"     },
	{ EVT_KEY_ENTER   , "ENTER" },
	{ EVT_KEY_ESC     , "ESC"   },
	{ EVT_KEY_F1      , "F1"    },
	{ EVT_KEY_F2      , "F2"    },
	{ EVT_KEY_F3      , "F3"    },
	{ EVT_KEY_F4      , "F4"    },
	{ EVT_KEY_F5      , "F5"    },
	{ EVT_KEY_F6      , "F6"    },
	{ EVT_KEY_F7      , "F7"    },
	{ EVT_KEY_F8      , "F8"    },
	{ EVT_KEY_F9      , "F9"    },
	{ EVT_KEY_F10     , "F10"   },
	{ EVT_KEY_F11     , "F11"   },
	{ EVT_KEY_F12     , "F12"   },
};

Engine::Engine()
	: view(nullptr)
	, model(nullptr)
	, controller(nullptr)
	, error(nullptr)
	, lua(nullptr)
{
}

Engine::~Engine() { destroy(); }

void Engine::reg_view(View * v) { view = v; }

void Engine::reg_model(Model * m) { model = m; }

void Engine::reg_controller(Controller * c) { controller = c; }

void Engine::reg_error(ErrorWriter * e) { error = e; }

void Engine::write_error(const std::string & s)
{
	if (error)
		error->engine_error(s);
}

void Engine::get_main_size(int & w, int & h)
{
	assert(lua);
	lua_getglobal(lua, "main_size");
	if (lua_pcall(lua, 0, 2, 0)) {
		std::string s = lua_tostring(lua, -1);
		lua_pop(lua, 1);
		write_error(s);
		throw exception(s);
	}
	w = luaL_checkinteger(lua, -2);
	h = luaL_checkinteger(lua, -1);
}

int Engine::get_width()
{
	int w = 0, h = 0;
	get_main_size(w, h);
	return w;
}

int Engine::get_height()
{
	int w = 0, h = 0;
	get_main_size(w, h);
	return h;
}

std::string Engine::get_vhf_version()
{
	assert(lua);
	lua_getglobal(lua, "VERSION");
	std::string v = luaL_checkstring(lua, -1);
	lua_pop(lua, 1);
	return v;
}

std::string Engine::get_lua_version() { return LUA_RELEASE; }

void Engine::init(const std::string & script)
{
	destroy();
	lua = lua_open();
	assert(lua);

	luaopen_base(lua);
	// luaopen_package(lua);
	luaopen_string(lua);
	luaopen_table(lua);
	luaopen_math(lua);
	// luaopen_io(lua);
	// luaopen_os(lua);
	luaopen_debug(lua);

	// register functions provided by the engine
	lua_register_functions(lua);

	// register global objects
	put(lua, view, "_VIEW_");
	put(lua, model, "_MODEL_");
	put(lua, controller, "_CONTROLLER_");

	if (luaL_dostring(lua, script.c_str())) {
		std::string s = lua_tostring(lua, -1);
		lua_pop(lua, 1);
		write_error(s);
		throw simradrd68::engine::exception(s);
	}
}

void Engine::destroy()
{
	if (lua) {
		lua_close(lua);
		lua = nullptr;
	}
}

int Engine::event(int event)
{
	assert(lua);
	int result = 0;
	lua_getglobal(lua, "event");
	lua_pushinteger(lua, event);
	if (lua_pcall(lua, 1, 1, 0)) {
		std::string s = lua_tostring(lua, -1);
		lua_pop(lua, 1);
		write_error(s);
		throw simradrd68::engine::exception(s);
	}
	result = luaL_checkinteger(lua, -1);
	lua_pop(lua, 1);
	return result;
}

void Engine::draw()
{
	assert(lua);
	lua_getglobal(lua, "draw");
	if (lua_pcall(lua, 0, 0, 0)) {
		std::string s = lua_tostring(lua, -1);
		lua_pop(lua, 1);
		write_error(s);
		throw exception(s);
	}
}

void Engine::set_exam_mode(bool flag)
{
	assert(lua);
	lua_getglobal(lua, "exam_mode");
	lua_pushboolean(lua, flag);
	if (lua_pcall(lua, 1, 0, 0)) {
		std::string s = lua_tostring(lua, -1);
		lua_pop(lua, 1);
		write_error(s);
		throw exception(s);
	}
}
}
}
