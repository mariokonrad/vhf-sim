// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef VHFSIM_WIDGET_HPP
#define VHFSIM_WIDGET_HPP

#include <memory>
#include <map>
#include <queue>
#include <QWidget>
#include <QImage>
#include <QSoundEffect>
#include <QTimer>
#include <QPen>
#include "Button.hpp"
#include "engine/Engine.hpp"
#include "engine/View.hpp"
#include "engine/Model.hpp"
#include "engine/Controller.hpp"
#include "engine/ErrorWriter.hpp"
#include "MsgSender.hpp"

namespace vhfsim
{
class Widget : public QWidget,
			   virtual public engine::View,
			   virtual public engine::Model,
			   virtual public engine::Controller,
			   virtual public engine::ErrorWriter
{
	Q_OBJECT

public:
	Widget(QWidget * parent);

	void show_buttons(bool);
	void set_msg_sender(std::unique_ptr<MsgSender> sender);
	std::string get_vhf_version() const;

public: // error writer
	virtual void engine_error(const std::string &) override;

public: // controller
	virtual void bind_button_circle(int, int, int, int, int, int) override;
	virtual void bind_button_rect(int, int, int, int, int, int, int) override;
	virtual void bind_key(int, int, int) override;
	virtual void timer_create(int) override;
	virtual void timer_delete(int) override;
	virtual void timer_start(int, int, bool) override;
	virtual void timer_stop(int) override;
	virtual int msg_recv(engine::msg_t &) override;
	virtual int msg_send(const engine::msg_t &) override;
	virtual void process(const engine::msg_t &) override;
	virtual void bind_msg(int) override;
	virtual void gps_process(const std::string &) override;
	virtual void bind_gps(int) override;
	virtual void set_exam_mode(bool) override;

public: // view: graphics
	virtual void update_view() override;
	virtual void set_background(int, int, int) override;
	virtual void set_brush(int, int, int) override;
	virtual void set_pen(int, int, int) override;
	virtual void clear() override;
	virtual void draw_rectangle(int, int, int, int) override;
	virtual void draw_circle(int, int, int) override;
	virtual void set_clipping_region(int, int, int, int) override;
	virtual void clear_clipping_region() override;
	virtual void img_load(int, const std::string &) override;
	virtual void img_size(int, int &, int &) override;
	virtual void draw_img(int, int, int) override;
	virtual void bitmap_register(const std::string &, int, int, char, const char *) override;
	virtual void bitmap_unregister(const std::string &) override;
	virtual void draw_bitmap(const std::string &, int, int, int, int) override;

public: // view: graphics font : NOT SUPPORTED
	virtual void register_font(int, int) override {}
	virtual void set_font(int) override {}
	virtual void set_text_background(int, int, int) override {}
	virtual void set_text_foreground(int, int, int) override {}
	virtual void draw_text(int, int, const std::string &,
		engine::View::TextAlign = engine::View::TEXT_ALIGN_LEFT) override
	{
	}
	virtual void draw_ch(int, int, const std::string &) override {}

public: // view: sound
	virtual int snd_init(int) override;
	virtual void snd_destroy() override;
	virtual void snd_load_wav(int, const std::string &) override;
	virtual void snd_play(int, bool) override;
	virtual void snd_stop(int) override;
	virtual void snd_gain(int, float) override;
	virtual void snd_pitch(int, float) override;

public: // model
	virtual bool vhf_lat_set() override;
	virtual bool vhf_lon_set() override;
	virtual bool vhf_time_set() override;
	virtual void vhf_clear_pos_time() override;
	virtual engine::MMSI mmsi() override;
	virtual void mmsi(const engine::MMSI &) override;
	virtual engine::MMSI group() override;
	virtual void group(const engine::MMSI &) override;
	virtual void lat(const engine::Latitude &) override;
	virtual engine::Latitude lat() override;
	virtual void lon(const engine::Longitude &) override;
	virtual engine::Longitude lon() override;
	virtual void time(const engine::Date &) override;
	virtual engine::Date time() override;

public: // directory
	virtual void dir_set(const engine::Directory &) override;
	virtual engine::Directory dir_get() override;

protected:
	virtual void paintEvent(QPaintEvent * event) override;
	virtual void keyPressEvent(QKeyEvent * event) override;
	virtual void keyReleaseEvent(QKeyEvent * event) override;
	virtual void mousePressEvent(QMouseEvent * event) override;
	virtual void mouseReleaseEvent(QMouseEvent * event) override;
	virtual void resizeEvent(QResizeEvent * event) override;

private:
	struct event_entry {
		int pressed;
		int released;
	};
	using image_map = std::map<int, QImage>;
	using key_map = std::map<int, event_entry>;
	using mouse_entry = std::pair<std::shared_ptr<Button>, int>;
	using button_map = std::map<mouse_entry, event_entry>;

	struct sound_entry {
		QSoundEffect * sound;
		QString source;
		float gain;
		float pitch;
	};
	using sound_map = std::map<int, sound_entry>;

	struct Bitmap {
		int width;
		int height;
		char c;
		std::string data;
	};
	using bitmap_map = std::map<std::string, Bitmap>;

	using timer_map = std::map<int, QTimer *>;
	using message_queue = std::queue<engine::msg_t>;

	std::unique_ptr<engine::Engine> engine;
	QPainter * painter;
	image_map images;
	key_map keys;
	button_map buttons;
	sound_map sounds;
	bitmap_map bitmaps;
	timer_map timers;
	bool must_show_buttons;
	QBrush background;
	QPen pen;
	QBrush brush;
	int old_key_code;
	int event_gps;
	int event_msg_recv;
	QPoint origin;
	message_queue mque;
	std::unique_ptr<MsgSender> msg_sender;

	void insert_bind_button(std::shared_ptr<Button>, int, int, int);
	void on_timer(int id);
	void handle_key(int);
	void update_origin();
	int press(std::shared_ptr<Button>, engine::MouseButton);
	int release(std::shared_ptr<Button>, engine::MouseButton);
	int press(const QPoint &, engine::MouseButton);
	int release(const QPoint &, engine::MouseButton);
	std::shared_ptr<Button> button_contains(const QPoint &, engine::MouseButton);
};
}

#endif
