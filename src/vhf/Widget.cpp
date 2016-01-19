// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#include "Widget.hpp"
#include <fstream>
#include <cassert>
#include <QApplication>
#include <QMessageBox>
#include <QDir>
#include <QPainter>
#include <QKeyEvent>
#include <QDebug>
#include "util/Circle.hpp"
#include "util/Rectangle.hpp"
#include "nmea/nmea_sentence.hpp"
#include "System.hpp"
#include "version.hpp"

namespace simradrd68
{
namespace
{
struct keymap_entry {
	engine::KeyEvent key;
	int native_code;
};

static const std::vector<keymap_entry> keymap_entries = {
	{engine::EVT_KEY_0, Qt::Key_0}, {engine::EVT_KEY_1, Qt::Key_1},
	{engine::EVT_KEY_2, Qt::Key_2}, {engine::EVT_KEY_3, Qt::Key_3},
	{engine::EVT_KEY_4, Qt::Key_4}, {engine::EVT_KEY_5, Qt::Key_5},
	{engine::EVT_KEY_6, Qt::Key_6}, {engine::EVT_KEY_7, Qt::Key_7},
	{engine::EVT_KEY_8, Qt::Key_8}, {engine::EVT_KEY_9, Qt::Key_9},
	{engine::EVT_KEY_ENTER, Qt::Key_Return}, {engine::EVT_KEY_ENTER, Qt::Key_Enter},
	{engine::EVT_KEY_ESC, Qt::Key_Escape}, {engine::EVT_KEY_F1, Qt::Key_F1},
	{engine::EVT_KEY_F2, Qt::Key_F2}, {engine::EVT_KEY_F3, Qt::Key_F3},
	{engine::EVT_KEY_F4, Qt::Key_F4}, {engine::EVT_KEY_F5, Qt::Key_F5},
	{engine::EVT_KEY_F6, Qt::Key_F6}, {engine::EVT_KEY_F7, Qt::Key_F7},
	{engine::EVT_KEY_F8, Qt::Key_F8}, {engine::EVT_KEY_F9, Qt::Key_F9},
	{engine::EVT_KEY_F10, Qt::Key_F10}, {engine::EVT_KEY_F11, Qt::Key_F11},
	{engine::EVT_KEY_F12, Qt::Key_F12},
};

static bool valid_key(int key)
{
	const auto i = std::find_if(keymap_entries.begin(), keymap_entries.end(),
		[key](const keymap_entry & entry) { return entry.native_code == key; });
	return i != keymap_entries.end();
}

static QDir device_path(const std::string & filename)
{
	auto app = QCoreApplication::instance();
	return QDir{app->applicationDirPath() + "/../share/" + simradrd68::project_name + "/"
		+ System::type() + "/" + filename.c_str()};
}

static engine::MouseButton mouse_qt_to_engine(Qt::MouseButton button)
{
	switch (button) {
		case Qt::LeftButton:
			return engine::MOUSE_LEFT;
		case Qt::MiddleButton:
			return engine::MOUSE_MIDDLE;
		case Qt::RightButton:
			return engine::MOUSE_RIGHT;
		default:
			return engine::MOUSE_NONE;
	}
}
}

Widget::Widget(QWidget * parent)
	: QWidget(parent)
	, must_show_buttons(false)
	, old_key_code(-1)
	, event_gps(-1)
	, event_msg_recv(-1)
	, origin(0, 0)
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
	update_origin();
	update_view();

	setMinimumSize(engine->get_width(), engine->get_height());
	setFocus();
}

void Widget::update_origin()
{
	// this not computed on paintEvent, because the origin is used somewhere
	// else, and it should be computed only at one place, not scathered arround.
	origin.setX((width() - engine->get_width() + 1) / 2);
	origin.setY((height() - engine->get_height() + 1) / 2);
}

void Widget::resizeEvent(QResizeEvent *)
{
	update_origin();
	update_view();
}

void Widget::paintEvent(QPaintEvent *)
{
	QPainter p(this);

	// LCD to be drawn and manipulated by the LUA script
	painter = &p;
	painter->translate(origin);
	try {
		engine->draw();
	} catch (engine::exception & e) {
		QMessageBox::critical(
			this, tr("Script Error"), tr("Lua error:\n%1").arg(e.what().c_str()));
		throw e;
	}
	// draw binded buttons if desired
	if (must_show_buttons) {
		painter->setPen(QPen{Qt::red});
		painter->setBrush(Qt::NoBrush);
		for (auto const & button : buttons) {
			button.first.first->draw(*painter);
		}
	}
	painter = nullptr;
}

void Widget::keyPressEvent(QKeyEvent * event)
{
	auto const key = event->key();
	if (!valid_key(key))
		return;
	if (old_key_code != -1)
		return;
	old_key_code = key;

	auto i = keys.find(key);
	if (i != keys.end())
		handle_key(i->second.pressed);
}

void Widget::keyReleaseEvent(QKeyEvent * event)
{
	auto const key = event->key();
	if (!valid_key(key))
		return;
	if (key != old_key_code)
		return;
	old_key_code = -1;

	auto i = keys.find(key);
	if (i != keys.end())
		handle_key(i->second.released);
}

int Widget::press(std::shared_ptr<Button> btn, engine::MouseButton mb)
{
	if (!btn)
		return -1;

	try {
		auto i = buttons.find(mouse_entry{btn, mb});
		if (i != buttons.end())
			return engine->event(i->second.pressed);
	} catch (const engine::exception & e) {
		QMessageBox::critical(
			this, tr("Script Error"), tr("Lua error:\n%1").arg(e.what().c_str()));
		throw e;
	}
	return -1;
}

int Widget::release(std::shared_ptr<Button> btn, engine::MouseButton mb)
{
	if (!btn)
		return -1;

	try {
		auto i = buttons.find(mouse_entry{btn, mb});
		if (i != buttons.end())
			return engine->event(i->second.released);
	} catch (const engine::exception & e) {
		QMessageBox::critical(
			this, tr("Script Error"), tr("Lua error:\n%1").arg(e.what().c_str()));
		throw e;
	}
	return -1;
}

int Widget::press(const QPoint & p, engine::MouseButton mb)
{
	return press(button_contains(p, mb), mb);
}

int Widget::release(const QPoint & p, engine::MouseButton mb)
{
	return release(button_contains(p, mb), mb);
}

std::shared_ptr<Button> Widget::button_contains(const QPoint & p, engine::MouseButton mb)
{
	auto i = std::find_if(
		buttons.begin(), buttons.end(), [p, mb](const button_map::value_type & v) {
			const auto & m = v.first;
			return ((mb == m.second) && (m.first->within(p.x(), p.y())));
		});
	if (i == buttons.end())
		return nullptr;
	return i->first.first;
}

void Widget::mousePressEvent(QMouseEvent * event)
{
	if ((event->button() == Qt::LeftButton) || (event->button() == Qt::RightButton)) {
		if (press(event->pos() - origin, mouse_qt_to_engine(event->button())) == 0)
			update_view();
	}
}

void Widget::mouseReleaseEvent(QMouseEvent * event)
{
	if ((event->button() == Qt::LeftButton) || (event->button() == Qt::RightButton)) {
		if (release(event->pos() - origin, mouse_qt_to_engine(event->button())) == 0)
			update_view();
	}
}

void Widget::show_buttons(bool flag)
{
	must_show_buttons = flag;
	update_view();
}

void Widget::engine_error(const std::string & s)
{
#if !defined(NDEBUG)
	qDebug() << "ENGINE ERROR:" << s.c_str();
#else
	QMessageBox::critical(this, tr("Script Error"), tr("Lua error:\n%1").arg(s.c_str()));
#endif
}

void Widget::handle_key(int code)
{
	try {
		if (engine->event(code) == 0)
			update_view();
	} catch (engine::exception & e) {
		QMessageBox::critical(
			this, tr("Script Error"), tr("Lua error:\n%1").arg(e.what().c_str()));
		throw e;
	}
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

void Widget::bind_key(int key, int event_press, int event_release)
{
	// for loop needed (without early exit), because the list above
	// may contain duplicate entries for engine keys

	for (auto const & i : keymap_entries) {
		if (i.key == key) {
			keys.emplace(i.native_code, event_entry{event_press, event_release});
		}
	}
}

void Widget::on_timer(int id)
{
	try {
		engine->event(id);
	} catch (engine::exception & e) {
		QMessageBox::critical(
			this, tr("Script Error"), tr("Lua error:\n%1").arg(e.what().c_str()));
		throw e;
	}
}

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

int Widget::msg_recv(engine::msg_t & m)
{
	if (mque.empty())
		return -1;
	m = mque.front();
	mque.pop();
	return 0;
}

void Widget::set_msg_sender(std::unique_ptr<MsgSender> sender)
{
	msg_sender = std::move(sender);
	if (msg_sender) {
		using namespace engine;

		// send pulse with information to login
		msg_t msg;
		msg.type = MSG_PULSE;
		mmsi().str(msg.mmsi);
		group().str(msg.group);
		msg_send(msg);
	}
}

int Widget::msg_send(const engine::msg_t & m)
{
	assert(msg_sender);
	return msg_sender->send(m);
}

void Widget::process(const engine::msg_t & m)
{
	try {
		mque.push(m);
		engine->event(event_msg_recv);
	} catch (engine::exception & e) {
		QMessageBox::critical(
			this, tr("Script Error"), tr("Lua error:\n%1").arg(e.what().c_str()));
		throw e;
	}
}

void Widget::bind_msg(int event) { event_msg_recv = event; }

void Widget::gps_process(const std::string & s)
{
	if (!nmea::check(s))
		return; // ignore faulty GPS data

	// parse fields from string
	nmea::Fields f;
	nmea::parse(f, s);

	// ignore all other sentences than RMC
	if (nmea::type(f) != nmea::TYPE_RMC)
		return;

	nmea::RMC rmc;
	if (!rmc.set(f)) {
		// it seems to be a malformed sencence, just ignore it
		return;
	}

	engine::Latitude la;
	engine::Longitude lo;
	engine::Date t;

	if (rmc.get(la) && rmc.get(lo) && rmc.get(t)) {
		// set pos / date within model
		lat(la);
		lon(lo);
		time(t);

		// tell engine about GPS activities
		try {
			engine->event(event_gps);
		} catch (engine::exception & e) {
			QMessageBox::critical(
				this, tr("Script Error"), tr("Lua error:\n%1").arg(e.what().c_str()));
			throw e;
		}
	}
}

void Widget::bind_gps(int event) { event_gps = event; }

void Widget::set_exam_mode(bool flag)
{
	try {
		engine->set_exam_mode(flag);
	} catch (engine::exception & e) {
		QMessageBox::critical(
			this, tr("Script Error"), tr("Lua error:\n%1").arg(e.what().c_str()));
		throw e;
	}
}

void Widget::update_view() { update(); }

void Widget::set_background(int r, int g, int b)
{
	background = QBrush{QColor{r, g, b}};
	painter->setBackground(background);
}

void Widget::set_brush(int r, int g, int b) { brush = QBrush(QColor(r, g, b)); }

void Widget::set_pen(int r, int g, int b) { pen = QPen(QColor(r, g, b), 1); }

void Widget::clear()
{
	painter->setBackgroundMode(Qt::OpaqueMode);
	painter->setBackground(background);
	painter->eraseRect(0, 0, engine->get_width(), engine->get_height());
}

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

void Widget::set_clipping_region(int x, int y, int w, int h)
{
	painter->setClipRect(x, y, w, h);
}

void Widget::clear_clipping_region()
{
	painter->setClipRect(0, 0, engine->get_width(), engine->get_height());
}

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

void Widget::bitmap_unregister(const std::string & id) { bitmaps.erase(bitmaps.find(id)); }

void Widget::draw_bitmap(
	const std::string & id, int sx, int sy, int pixel_width, int pixel_height)
{
	const auto i = bitmaps.find(id);
	if (i == bitmaps.end())
		return;
	const Bitmap & b = i->second;

	painter->setPen(pen);
	painter->setBrush(brush);
	painter->setBackground(background);

	for (int y = 0; y < b.height; ++y) {
		const int index = b.width * y;
		for (int x = 0; x < b.width; ++x) {
			if (b.data[index + x] == b.c) {
				painter->fillRect(sx + (x + 1) * pixel_width, sy + (y + 1) * pixel_height,
					pixel_width, pixel_height, pen.brush());
			}
		}
	}
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

void Widget::snd_destroy()
{
	for (auto & entry : sounds)
		delete entry.second.sound;
	sounds.clear();
}

void Widget::snd_load_wav(int id, const std::string & filename)
{
	auto i = sounds.find(id);
	if (i != sounds.end()) {
		delete i->second.sound;
	}
	sounds[id] = {new QSoundEffect{this}, device_path(filename).absolutePath(), 1.0f, 1.0f};
}

void Widget::snd_play(int id, bool loop)
{
	if (System::sound_disabled())
		return;
	auto i = sounds.find(id);
	if (i == sounds.end())
		return;
	auto sound = i->second.sound;
	sound->setSource(QUrl::fromLocalFile(i->second.source));
	sound->setLoopCount(loop ? QSoundEffect::Infinite : 1);
	sound->setVolume(i->second.gain);
	sound->play();
}

void Widget::snd_stop(int id)
{
	auto i = sounds.find(id);
	if (i == sounds.end())
		return;
	i->second.sound->stop();
}

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
