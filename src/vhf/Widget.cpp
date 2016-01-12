#include "Widget.hpp"
#include <fstream>
#include <cassert>
#include <QApplication>
#include <QMessageBox>
#include <QDir>
#include <QPainter>
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
	, must_show_buttons(false)
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
	update_view();

	setMaximumSize(engine->get_width(), engine->get_height());
	setMinimumSize(engine->get_width(), engine->get_height());
	parentWidget()->setFixedSize(engine->get_width(), engine->get_height());

	setFocus();
}

void Widget::paintEvent(QPaintEvent *)
{
	QPainter p(this);

	// LCD to be drawn and manipulated by the LUA script
	painter = &p;
	try {
		engine->draw();
	} catch (engine::exception & e) {
		QMessageBox::critical(
			this, tr("Script Error"), tr("Lua error:\n%1").arg(e.what().c_str()));
		throw e;
	}
	// draw binded buttons if desired
	if (must_show_buttons) {
		painter->setPen(QPen{QColor{255, 0, 0}});
		painter->setBrush(Qt::NoBrush);
		for (auto const & button : buttons) {
			button.first.first->draw(*painter);
		}
	}
	painter = nullptr;
}

void Widget::keyPressEvent(QKeyEvent *)
{
	qDebug() << __PRETTY_FUNCTION__;
}

void Widget::keyReleaseEvent(QKeyEvent *)
{
	qDebug() << __PRETTY_FUNCTION__;
}

void Widget::mouseMoveEvent(QMouseEvent *)
{
	qDebug() << __PRETTY_FUNCTION__;
}

void Widget::mousePressEvent(QMouseEvent *)
{
	qDebug() << __PRETTY_FUNCTION__;
}

void Widget::mouseReleaseEvent(QMouseEvent *)
{
	qDebug() << __PRETTY_FUNCTION__;
}

void Widget::wheelEvent(QWheelEvent *)
{
	qDebug() << __PRETTY_FUNCTION__;
}

void Widget::show_buttons(bool flag)
{
	must_show_buttons = flag;
	update_view();
}

void Widget::engine_error(const std::string & s)
{
	qDebug() << __PRETTY_FUNCTION__ << s.c_str();
}

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

void Widget::on_timer(int id) { qDebug() << __PRETTY_FUNCTION__ << "id=" << id; }

void Widget::timer_create(int id)
{
	timer_delete(id);
	timers.emplace(id, new QTimer{this});
}

void Widget::timer_delete(int id)
{
	auto i = timers.find(id);
	if (i == timers.end())
		return;
	i->second->stop();
	i->second->disconnect();
	delete i->second;
	timers.erase(i);
}

void Widget::timer_start(int id, int msec, bool one_shot)
{
	auto i = timers.find(id);
	if (i == timers.end())
		return;
	i->second->setInterval(msec);
	i->second->setSingleShot(one_shot);
	connect(i->second, &QTimer::timeout, [this, id]() { this->on_timer(id); });
	i->second->start();
}

void Widget::timer_stop(int id)
{
	auto i = timers.find(id);
	if (i == timers.end())
		return;
	i->second->stop();
}

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

void Widget::update_view() { repaint(); }

void Widget::set_background(int r, int g, int b)
{
	background = QBrush{QColor{r, g, b}};
	painter->setBackground(background);
}

void Widget::set_brush(int r, int g, int b) { brush = QBrush(QColor(r, g, b)); }

void Widget::set_pen(int r, int g, int b) { pen = QPen(QColor(r, g, b), 1); }

void Widget::set_text_background(int, int, int) { qDebug() << __PRETTY_FUNCTION__; }

void Widget::set_text_foreground(int, int, int) { qDebug() << __PRETTY_FUNCTION__; }

void Widget::clear() { painter->setBackground(background); }

void Widget::draw_rectangle(int x, int y, int w, int h)
{
	painter->setPen(pen);
	painter->setBrush(brush);
	painter->setBackground(background);
	painter->drawRect(x, y, w, h);
}

void Widget::draw_circle(int x, int y, int r)
{
	painter->setPen(pen);
	painter->setBrush(brush);
	painter->setBackground(background);
	painter->drawEllipse(x - r, y - r, 2 * r, 2 * r);
}

void Widget::draw_text(int, int, const std::string &, engine::View::TextAlign)
{
	qDebug() << __PRETTY_FUNCTION__;
}

void Widget::draw_ch(int, int, const std::string &) { qDebug() << __PRETTY_FUNCTION__; }

void Widget::set_clipping_region(int x, int y, int w, int h)
{
	painter->setClipRect(x, y, w, h);
	painter->setClipping(true);
}

void Widget::clear_clipping_region()
{
	painter->setClipRect(0, 0, engine->get_width(), engine->get_height());
	painter->setClipping(true);
}

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

void Widget::draw_img(int x, int y, int id)
{
	assert(painter);
	auto i = images.find(id);
	if (i == images.end())
		return;
	painter->drawImage(x, y, i->second);
}

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

void Widget::draw_bitmap(
	const std::string & id, int sx, int sy, int pixel_width, int pixel_height)
{
	qDebug() << __PRETTY_FUNCTION__ << id.c_str() << sx << sy << pixel_width << pixel_height;
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

bool Widget::vhf_lat_set() { return System::vhf_lat_set(); }

bool Widget::vhf_lon_set() { return System::vhf_lon_set(); }

bool Widget::vhf_time_set() { return System::vhf_time_set(); }

void Widget::vhf_clear_pos_time() { System::vhf_clear_pos_time(); }

engine::MMSI Widget::mmsi() { return System::vhf_mmsi(); }

void Widget::mmsi(const engine::MMSI & mmsi) { System::vhf_mmsi(mmsi); }

engine::MMSI Widget::group() { return System::vhf_group(); }

void Widget::group(const engine::MMSI & mmsi) { System::vhf_group(mmsi); }

void Widget::lat(const engine::Latitude & l) { System::vhf_latitude(l); }

engine::Latitude Widget::lat() { return System::vhf_latitude(); }

void Widget::lon(const engine::Longitude & l) { System::vhf_longitude(l); }

engine::Longitude Widget::lon() { return System::vhf_longitude(); }

void Widget::time(const engine::Date & t) { System::vhf_time(t); }

engine::Date Widget::time() { return System::vhf_time(); }

void Widget::dir_set(const engine::Directory & dir) { System::dir_set(dir); }

engine::Directory Widget::dir_get() { return System::dir_get(); }
}
