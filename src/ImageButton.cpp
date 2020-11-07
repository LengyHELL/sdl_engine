#include "Controls.hpp"
#include <iostream>

ImageButton::ImageButton(const std::string& style, const Rect& body,
  const std::string& image, const bool& on_mouse_down, const Rect& source_rect, const Coord& cut_size)
  : button_frame(Frame(style, body, cut_size)), body(body),
  image(image), on_mouse_down(on_mouse_down), source_rect(source_rect) {}

void ImageButton::draw(const Engine& engine) {
  const Coord cut_size = button_frame.get_cut_size();
  const Rect body = button_frame.get_body();
  SDL_Rect a_cut;
  a_cut.w = cut_size.x;
  a_cut.h = cut_size.y;

  button_frame.draw(engine);

  if (mouse_click) {
    SDL_Color color{255, 255, 255, 100};
    a_cut.x = 10 * cut_size.x;
    a_cut.y = 0;
    engine.draw_image(button_frame.get_style(), body.x, body.y, body.w, body.h, 0, &color, &a_cut);
  }
  else if (mouse_hover) {
    SDL_Color color{255, 255, 255, 100};
    a_cut.x = 9 * cut_size.x;
    a_cut.y = 0;
    engine.draw_image(button_frame.get_style(), body.x, body.y, body.w, body.h, 0, &color, &a_cut);
  }

  Rect a_image = engine.size_image(image);
  if ((source_rect.w + source_rect.h) > 0) {
    a_image = source_rect;
  }
  float ratio = (body.h * 0.5) / a_image.h;
  a_image.h *= ratio;
  a_image.w *= ratio;
  Coord place = Coord(body.x, body.y) + (Coord(body.w, body.h) / 2);
  place -= Coord(a_image.w, a_image.h) / 2;
  if ((source_rect.w + source_rect.h) > 0) {
    SDL_Rect src = {(int)source_rect.x, (int)source_rect.y, (int)source_rect.w, (int)source_rect.h};
    engine.draw_image(image, place.x, place.y, a_image.w, a_image.h, 0, NULL, &src);
  }
  else {
    engine.draw_image(image, place.x, place.y, a_image.w, a_image.h);
  }
}

void ImageButton::update(const Engine& engine) {
  button_frame.update(engine);
  mouse_hover = button_frame.mouse_hover;

  bool mouse_left = engine.mouse_state == SDL_BUTTON(SDL_BUTTON_LEFT);
  if (!mouse_left) {
    click_lock = false;
  }
  if (!mouse_hover && mouse_left) {
    click_lock = true;
  }
  mouse_click = mouse_hover && mouse_left && !click_lock;

  if (on_mouse_down) {
    selected = mouse_click && engine.get_mouse_button_down();
  }
  else {
    selected = mouse_click && engine.get_mouse_button_up();
  }
}
