#include "InputBox.hpp"

InputBox::InputBox(const std::string& style, const Rect& body,
  const unsigned& text_size, const SDL_Color& text_color, const Coord& cut_size)
  : button_frame(Frame(style, body, cut_size)), body(body), text_size(text_size),
  text_color(text_color) {}

void InputBox::draw(const Engine& engine) {
  const Coord cut_size = button_frame.get_cut_size();
  const Rect body = button_frame.get_body();
  Rect a_cut(0, 0, cut_size.x, cut_size.y);

  button_frame.draw(engine);

  int padding = (int)(cut_size.len() * 1.5);
  Coord place(body.x + padding, body.y + padding);

  SDL_Color inverse = {
    (Uint8)(255 - text_color.r),
    (Uint8)(255 - text_color.g),
    (Uint8)(255 - text_color.b),
    (Uint8)(255 - text_color.a)
  };

  std::string start = text.substr(0, selection_start);
  std::string middle = text.substr(selection_start, selection_end - selection_start);
  std::string end = text.substr(selection_end);

  engine.draw_text(start, place, text_color, text_size);

  int x_text = engine.size_text(start, text_size).w;
  engine.draw_text(middle, place + Coord(x_text, 0), inverse, text_size);
  if ((selection_start == engine.get_input_cursor()) && selected) {
    a_cut.x = 9 * cut_size.x;
    a_cut.y = 0;
    engine.draw_image(button_frame.get_style(), Rect(place.x + x_text - 1, place.y, 2, text_size), 0, {255, 255, 255, 255}, a_cut);
  }

  x_text += engine.size_text(middle, text_size).w;
  engine.draw_text(end, place + Coord(x_text, 0), text_color, text_size);
  if ((selection_end == engine.get_input_cursor()) && selected) {
    a_cut.x = 9 * cut_size.x;
    a_cut.y = 0;
    engine.draw_image(button_frame.get_style(), Rect(place.x + x_text - 1, place.y, 2, text_size), 0, {255, 255, 255, 255}, a_cut);
  }
}

void InputBox::update(Engine& engine) {
  button_frame.update(engine);
  mouse_hover = button_frame.mouse_hover;

  bool mouse_left = engine.mouse_state == SDL_BUTTON(SDL_BUTTON_LEFT);
  if (!mouse_left) {
    click_lock = false;
  }
  bool mouse_click = mouse_left && !click_lock && engine.get_mouse_button_down();
  if (mouse_click && mouse_hover) {
    selected = true;
    engine.set_text_input(true);
  }
  else if (mouse_click) {
    selected = false;
    engine.set_text_input(false);
  }

  if (selected) {
    text = engine.get_input_text();

    selection_start = engine.get_input_selection();
    selection_end = engine.get_input_cursor();
    if (selection_end < selection_start) {
      int temp = selection_start;
      selection_start = selection_end;
      selection_end = temp;
    }
  }
}
