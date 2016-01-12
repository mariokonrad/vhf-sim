#include "Widget.hpp"
#include <QDebug>

namespace simradrd68
{
Widget::Widget(QWidget * parent)
	: QWidget(parent)
{
}

void Widget::engine_error(const std::string &) { qDebug() << __PRETTY_FUNCTION__; }

void Widget::bind_button_circle(int, int, int, int, int, int)
{
	qDebug() << __PRETTY_FUNCTION__;
}

void Widget::bind_button_rect(int, int, int, int, int, int, int)
{
	qDebug() << __PRETTY_FUNCTION__;
}

void Widget::bind_key(int, int, int) { qDebug() << __PRETTY_FUNCTION__; }

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

void Widget::img_load(int, const std::string &) { qDebug() << __PRETTY_FUNCTION__; }

void Widget::img_size(int, int &, int &) { qDebug() << __PRETTY_FUNCTION__; }

void Widget::draw_img(int, int, int) { qDebug() << __PRETTY_FUNCTION__; }

void Widget::bitmap_register(const std::string &, int, int, char, const char *)
{
	qDebug() << __PRETTY_FUNCTION__;
}

void Widget::bitmap_unregister(const std::string &) { qDebug() << __PRETTY_FUNCTION__; }

void Widget::draw_bitmap(const std::string &, int, int, int, int)
{
	qDebug() << __PRETTY_FUNCTION__;
}

int Widget::snd_init(int)
{
	qDebug() << __PRETTY_FUNCTION__;
	return -1;
}

void Widget::snd_destroy() { qDebug() << __PRETTY_FUNCTION__; }

void Widget::snd_load_wav(int, const std::string &) { qDebug() << __PRETTY_FUNCTION__; }

void Widget::snd_play(int, bool) { qDebug() << __PRETTY_FUNCTION__; }

void Widget::snd_stop(int) { qDebug() << __PRETTY_FUNCTION__; }

void Widget::snd_gain(int, float) { qDebug() << __PRETTY_FUNCTION__; }

void Widget::snd_pitch(int, float) { qDebug() << __PRETTY_FUNCTION__; }

bool Widget::vhf_lat_set() { qDebug() << __PRETTY_FUNCTION__; return false; }

bool Widget::vhf_lon_set() { qDebug() << __PRETTY_FUNCTION__; return false; }

bool Widget::vhf_time_set() { qDebug() << __PRETTY_FUNCTION__; return false; }

void Widget::vhf_clear_pos_time() { qDebug() << __PRETTY_FUNCTION__; }

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
