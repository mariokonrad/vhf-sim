// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef VHFSIM_ENGINE_VIEW_HPP
#define VHFSIM_ENGINE_VIEW_HPP

#include <string>

namespace vhfsim
{
namespace engine
{
class View
{
public:
	enum TextAlign { TEXT_ALIGN_LEFT = 0, TEXT_ALIGN_RIGHT = 1, TEXT_ALIGN_MIDDLE = 2 };

public:
	virtual ~View() {}

	// graphics related
	virtual void update_view() = 0;
	virtual void set_background(int, int, int) = 0;
	virtual void set_brush(int, int, int) = 0;
	virtual void set_pen(int, int, int) = 0;
	virtual void clear() = 0;
	virtual void draw_rectangle(int, int, int, int) = 0;
	virtual void draw_circle(int, int, int) = 0;
	virtual void set_clipping_region(int, int, int, int) = 0;
	virtual void clear_clipping_region() = 0;
	virtual void img_load(int, const std::string &) = 0;
	virtual void img_size(int, int &, int &) = 0;
	virtual void draw_img(int, int, int) = 0;
	virtual void bitmap_register(const std::string &, int, int, char, const char *) = 0;
	virtual void bitmap_unregister(const std::string &) = 0;
	virtual void draw_bitmap(const std::string &, int, int, int, int) = 0;

	// graphics font related
	virtual void register_font(int, int) = 0;
	virtual void set_font(int) = 0;
	virtual void set_text_background(int, int, int) = 0;
	virtual void set_text_foreground(int, int, int) = 0;
	virtual void draw_text(int, int, const std::string &, TextAlign = TEXT_ALIGN_LEFT) = 0;
	virtual void draw_ch(int, int, const std::string &) = 0;

	// sound related
	virtual int snd_init(int) = 0;
	virtual void snd_destroy() = 0;
	virtual void snd_load_wav(int, const std::string &) = 0;
	virtual void snd_play(int, bool) = 0;
	virtual void snd_stop(int) = 0;
	virtual void snd_gain(int, float) = 0;
	virtual void snd_pitch(int, float) = 0;
};
}
}

#endif
