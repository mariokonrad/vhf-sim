#include "Widget.hpp"
#include <fstream>
#include <QApplication>
#include <QMessageBox>
#include <QDir>
#include <QDebug>
#include "util/Circle.hpp"
#include "util/Rectangle.hpp"
#include "System.hpp"
#include "version.hpp"

namespace simradrd68
{
namespace
{
static QDir device_path(const std::string & filename)
{
	auto app = QCoreApplication::instance();
	return QDir{app->applicationDirPath() + "/../share/" + simradrd68::project_name + "/"
		+ System::type().c_str() + "/" + filename.c_str()};
}
}

Widget::Widget(QWidget * parent)
	: QWidget(parent)
{
	try {
		// initialize engine
		engine.reset(new engine::Engine);
		engine->reg_view(this);
		engine->reg_model(this);
		engine->reg_controller(this);
		engine->reg_error(this);

		// load script
		const std::string path = device_path("vhf.lua").absolutePath().toStdString();
		std::ifstream ifs{path.c_str()};
		std::string script;
		if (!ifs) {
			throw engine::exception("Cannot read file: " + path);
		}
		script = std::string(
			(std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
		engine->init(script);
	} catch (engine::exception & e) {
		QMessageBox::critical(
			this, tr("Script Error"), tr("Lua error:\n%1").arg(e.what().c_str()));
		throw e;
	}
}

void Widget::engine_error(const std::string &) { qDebug() << __PRETTY_FUNCTION__; }

void Widget::insert_bind_button(
	std::shared_ptr<Button> button, int mouse_button, int event_press, int event_release)
{
	int mb = -1;
	switch (mouse_button) {
		case engine::MOUSE_LEFT:
			mb = 0;
			break;
		case engine::MOUSE_MIDDLE:
			mb = 1;
			break;
		case engine::MOUSE_RIGHT:
			mb = 2;
			break;
	}
	if (mb < 0)
		return;
	buttons.emplace(mouse_entry{button, mb}, event_entry{event_press, event_release});
}

void Widget::bind_button_circle(
	int mx, int my, int r, int mouse_button, int id_press, int id_release)
{
	insert_bind_button(std::make_shared<Circle>(mx, my, r), mouse_button, id_press, id_release);
}

void Widget::bind_button_rect(
	int x0, int y0, int x1, int y1, int mouse_button, int id_press, int id_release)
{
	insert_bind_button(
		std::make_shared<Rectangle>(x0, y0, x1, y1), mouse_button, id_press, id_release);
}

void Widget::bind_key(int code, int event_press, int event_release)
{
	keys.emplace(code, event_entry{event_press, event_release});
}

void Widget::timer_create(int) { qDebug() << __PRETTY_FUNCTION__; }

void Widget::timer_delete(int) { qDebug() << __PRETTY_FUNCTION__; }

void Widget::timer_start(int, int, bool) { qDebug() << __PRETTY_FUNCTION__; }

void Widget::timer_stop(int) { qDebug() << __PRETTY_FUNCTION__; }

int Widget::msg_recv(engine::msg_t &)
{
	qDebug() << __PRETTY_FUNCTION__;
	return -1;
}

int Widget::msg_send(const engine::msg_t &)
{
	qDebug() << __PRETTY_FUNCTION__;
	return -1;
}

void Widget::process(const engine::msg_t &) { qDebug() << __PRETTY_FUNCTION__; }

void Widget::bind_msg(int) { qDebug() << __PRETTY_FUNCTION__; }

void Widget::gps_process(const std::string &) { qDebug() << __PRETTY_FUNCTION__; }

void Widget::bind_gps(int) { qDebug() << __PRETTY_FUNCTION__; }

void Widget::set_exam_mode(bool) { qDebug() << __PRETTY_FUNCTION__; }

void Widget::update_view() { qDebug() << __PRETTY_FUNCTION__; }

void Widget::set_background(int, int, int) { qDebug() << __PRETTY_FUNCTION__; }

void Widget::set_brush(int, int, int) { qDebug() << __PRETTY_FUNCTION__; }

void Widget::set_pen(int, int, int) { qDebug() << __PRETTY_FUNCTION__; }

void Widget::set_text_background(int, int, int) { qDebug() << __PRETTY_FUNCTION__; }

void Widget::set_text_foreground(int, int, int) { qDebug() << __PRETTY_FUNCTION__; }

void Widget::clear() { qDebug() << __PRETTY_FUNCTION__; }

void Widget::draw_rectangle(int, int, int, int) { qDebug() << __PRETTY_FUNCTION__; }

void Widget::draw_circle(int, int, int) { qDebug() << __PRETTY_FUNCTION__; }

void Widget::draw_text(int, int, const std::string &, engine::View::TextAlign)
{
	qDebug() << __PRETTY_FUNCTION__;
}

void Widget::draw_ch(int, int, const std::string &) { qDebug() << __PRETTY_FUNCTION__; }

void Widget::set_clipping_region(int, int, int, int) { qDebug() << __PRETTY_FUNCTION__; }

void Widget::clear_clipping_region() { qDebug() << __PRETTY_FUNCTION__; }

void Widget::register_font(int, int) { qDebug() << __PRETTY_FUNCTION__; }

void Widget::set_font(int) { qDebug() << __PRETTY_FUNCTION__; }

void Widget::img_load(int id, const std::string & filename)
{
	images[id] = QImage(device_path(filename).absolutePath());
}

void Widget::img_size(int id, int & width, int & height)
{
	width = -1;
	height = -1;
	auto i = images.find(id);
	if (i != images.end()) {
		width = i->second.width();
		height = i->second.height();
	}
}

void Widget::draw_img(int, int, int) { qDebug() << __PRETTY_FUNCTION__; }

void Widget::bitmap_register(
	const std::string & id, int width, int height, char c, const char * data)
{
	auto & bm = bitmaps[id];
	bm.width = width;
	bm.height = height;
	bm.c = c;
	bm.data = data;
}

void Widget::bitmap_unregister(const std::string &) { qDebug() << __PRETTY_FUNCTION__; }

void Widget::draw_bitmap(const std::string &, int, int, int, int)
{
	qDebug() << __PRETTY_FUNCTION__;
}

int Widget::snd_init(int num)
{
	if (System::sound_disabled())
		return 0;

	if (num < 0)
		return -1;

	System::sound_enable(true);
	return 0;
}

void Widget::snd_destroy() { sounds.clear(); }

void Widget::snd_load_wav(int id, const std::string & filename)
{
	auto i = sounds.find(id);
	if (i != sounds.end()) {
		delete i->second.sound;
	}
	sounds[id] = {new QSound{device_path(filename).absolutePath(), this}, 1.0f, 1.0f};
}

void Widget::snd_play(int, bool) { qDebug() << __PRETTY_FUNCTION__; }

void Widget::snd_stop(int) { qDebug() << __PRETTY_FUNCTION__; }

void Widget::snd_gain(int id, float gain)
{
	auto i = sounds.find(id);
	if (i != sounds.end()) {
		i->second.gain = gain;
	}
}

void Widget::snd_pitch(int id, float pitch)
{
	auto i = sounds.find(id);
	if (i != sounds.end()) {
		i->second.pitch = pitch;
	}
}

bool Widget::vhf_lat_set() { qDebug() << __PRETTY_FUNCTION__; return false; }

bool Widget::vhf_lon_set() { qDebug() << __PRETTY_FUNCTION__; return false; }

bool Widget::vhf_time_set() { qDebug() << __PRETTY_FUNCTION__; return false; }

void Widget::vhf_clear_pos_time() { System::vhf_clear_pos_time(); }

engine::MMSI Widget::mmsi() { qDebug() << __PRETTY_FUNCTION__; return engine::MMSI{}; }

void Widget::mmsi(const engine::MMSI &) { qDebug() << __PRETTY_FUNCTION__; }

engine::MMSI Widget::group() { qDebug() << __PRETTY_FUNCTION__; return engine::MMSI{}; }

void Widget::group(const engine::MMSI &) { qDebug() << __PRETTY_FUNCTION__; }

void Widget::lat(const engine::Latitude &) { qDebug() << __PRETTY_FUNCTION__; }

engine::Latitude Widget::lat() { qDebug() << __PRETTY_FUNCTION__; return engine::Latitude{}; }

void Widget::lon(const engine::Longitude &) { qDebug() << __PRETTY_FUNCTION__; }

engine::Longitude Widget::lon() { qDebug() << __PRETTY_FUNCTION__; return engine::Longitude{}; }

void Widget::time(const engine::Date &) { qDebug() << __PRETTY_FUNCTION__; }

engine::Date Widget::time() { qDebug() << __PRETTY_FUNCTION__; return engine::Date{}; }

void Widget::dir_set(const engine::Directory &) { qDebug() << __PRETTY_FUNCTION__; }

engine::Directory Widget::dir_get() { qDebug() << __PRETTY_FUNCTION__; return engine::Directory{}; }
}
