#include "Controls.hpp"
#include <iostream>

TextBox::TextBox(const std::string& style, const Rect& body, const std::string& text,
  const unsigned& text_size, const SDL_Color& text_color, const Coord& cut_size)
  : text_box_frame(Frame(style, body, cut_size)), body(body), text(text), text_size(text_size),
  text_color(text_color) {}

void TextBox::draw(const Engine& engine) {
  const Coord cut_size = text_box_frame.get_cut_size();
  const Rect body = text_box_frame.get_body();

  text_box_frame.draw(engine);

  Rect a_text = engine.size_text(text, text_size);
  Coord place = Coord(body.x, body.y) + (Coord(body.w, body.h) / 2);
  place -= Coord(a_text.w, a_text.h) / 2;
  engine.draw_text(text, place, text_color, text_size);
}

void TextBox::update(const Engine& engine) {
  text_box_frame.update(engine);
  mouse_hover = text_box_frame.mouse_hover;
}
