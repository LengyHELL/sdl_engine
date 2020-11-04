#include "Controls.hpp"

// Button

Button::Button(const Engine& engine, const std::string& texture, const float& position_x, const float& position_y,
  const float& width, const float& height, const std::string& text, const unsigned& size, const SDL_Color& text_color,
  const bool& on_mouse_down)
: texture(texture), position_x(position_x), position_y(position_y), width(width), height(height), on_mouse_down(on_mouse_down) {

  position.push_back(Coord(position_x, position_y));
  position.push_back(Coord(position_x + cut_size.x, position_y));
  position.push_back(Coord(position_x + (width - cut_size.x), position_y));
  cut.push_back(cut_size);
  cut.push_back(Coord(width - 2 * cut_size.x, cut_size.y));
  cut.push_back(cut_size);

  position.push_back(Coord(position_x, position_y + cut_size.y));
  position.push_back(Coord(position_x + (width - cut_size.x), position_y + cut_size.y));
  cut.push_back(Coord(cut_size.x, height - 2 * cut_size.y));
  cut.push_back(Coord(cut_size.x, height - 2 * cut_size.y));

  position.push_back(Coord(position_x, position_y + (height - cut_size.y)));
  position.push_back(Coord(position_x + cut_size.x, position_y + (height - cut_size.y)));
  position.push_back(Coord(position_x + (width - cut_size.x), position_y + (height - cut_size.y)));
  cut.push_back(cut_size);
  cut.push_back(Coord(width - 2 * cut_size.x, cut_size.y));
  cut.push_back(cut_size);

  allocated = true;
  content = engine.texture_of_text(text, text_color, size, &content_rect);

  content_rect.x = position_x + ((width / 2) - (content_rect.w / 2));
  content_rect.y = position_y + ((height / 2) - (content_rect.h / 2));
}

Button::Button(const Engine& engine, const std::string& texture, const float& position_x, const float& position_y,
  const float& width, const float& height, const std::string& image, const float& scale, SDL_Rect* image_cut,
  const bool& on_mouse_down)
: texture(texture), position_x(position_x), position_y(position_y), width(width), height(height), on_mouse_down(on_mouse_down) {
  if (image_cut != NULL) {
    content_cut = *image_cut;
    cut_source = true;
  }

  position.push_back(Coord(position_x, position_y));
  position.push_back(Coord(position_x + cut_size.x, position_y));
  position.push_back(Coord(position_x + (width - cut_size.x), position_y));
  cut.push_back(cut_size);
  cut.push_back(Coord(width - 2 * cut_size.x, cut_size.y));
  cut.push_back(cut_size);

  position.push_back(Coord(position_x, position_y + cut_size.y));
  position.push_back(Coord(position_x + (width - cut_size.x), position_y + cut_size.y));
  cut.push_back(Coord(cut_size.x, height - 2 * cut_size.y));
  cut.push_back(Coord(cut_size.x, height - 2 * cut_size.y));

  position.push_back(Coord(position_x, position_y + (height - cut_size.y)));
  position.push_back(Coord(position_x + cut_size.x, position_y + (height - cut_size.y)));
  position.push_back(Coord(position_x + (width - cut_size.x), position_y + (height - cut_size.y)));
  cut.push_back(cut_size);
  cut.push_back(Coord(width - 2 * cut_size.x, cut_size.y));
  cut.push_back(cut_size);

  content = engine.texture_of_image(image);
  if (cut_source) { content_rect = content_cut; }
  else { SDL_QueryTexture(content, 0, 0, &content_rect.w, &content_rect.h); }

  content_rect.w *= scale;
  content_rect.h *= scale;
  content_rect.x = position_x + ((width / 2) - (content_rect.w / 2));
  content_rect.y = position_y + ((height / 2) - (content_rect.h / 2));
}

void Button::draw(const Engine& engine) {
  SDL_Rect a_cut;
  a_cut.w = cut_size.x;
  a_cut.h = cut_size.y;

  a_cut.x = 8 * cut_size.x;
  a_cut.y = 0;
  engine.draw_image(texture, position_x, position_y, width, height, 0, NULL, &a_cut);

  for (unsigned i = 0; i < position.size(); i++) {
    a_cut.x = i * cut_size.x;
    a_cut.y = 0;
    engine.draw_image(texture, position[i].x, position[i].y, cut[i].x, cut[i].y, 0, NULL, &a_cut);
  }

  if (mouse_click) {
    SDL_Color color{255, 255, 255, 100};
    a_cut.x = 10 * cut_size.x;
    a_cut.y = 0;
    engine.draw_image(texture, position_x, position_y, width, height, 0, &color, &a_cut);
  }
  else if (mouse_hover) {
    SDL_Color color{255, 255, 255, 100};
    a_cut.x = 9 * cut_size.x;
    a_cut.y = 0;
    engine.draw_image(texture, position_x, position_y, width, height, 0, &color, &a_cut);
  }

  if (cut_source) { engine.draw_texture(content, content_rect.x, content_rect.y, content_rect.w, content_rect.h, 0, NULL, &content_cut); }
  else { engine.draw_texture(content, content_rect.x, content_rect.y, content_rect.w, content_rect.h); }
}

void Button::update(const Engine& engine) {
  bool x_in_range = (engine.mouse_position_x >= position_x) && (engine.mouse_position_x <= (position_x + width));
  bool y_in_range = (engine.mouse_position_y >= position_y) && (engine.mouse_position_y <= (position_y + height));
  mouse_hover = x_in_range && y_in_range;

  bool mouse_left = engine.mouse_state == SDL_BUTTON(SDL_BUTTON_LEFT);
  if (!mouse_left) {
    click_lock = false;
  }
  if (!mouse_hover && mouse_left) {
    click_lock = true;
  }
  mouse_click = mouse_hover && mouse_left && !click_lock;

  if (on_mouse_down) {
    selected = mouse_click && engine.mouse_button_down;
  }
  else {
    selected = mouse_click && engine.mouse_button_up;
  }
}



// Picker
Picker::Picker(const Engine& engine, const std::string& texture, const std::string& button_texture, const float& position_x,
  const float& position_y, const float& width, const float& height, const float& scale, const SDL_Color& text_color,
  const unsigned& text_size, const int& min, const int& max, const int& actual, const int& step)
: texture(texture), position_x(position_x), position_y(position_y), width(width), height(height), scale(scale),
  text_color(text_color), text_size(text_size), min(min), max(max), actual(actual), step(step) {
  float temp_width = width - (height / 2);

  position.push_back(Coord(position_x, position_y));
  position.push_back(Coord(position_x + cut_size.x, position_y));
  position.push_back(Coord(position_x + (temp_width - cut_size.x), position_y));
  cut.push_back(cut_size);
  cut.push_back(Coord(temp_width - 2 * cut_size.x, cut_size.y));
  cut.push_back(cut_size);

  position.push_back(Coord(position_x, position_y + cut_size.y));
  position.push_back(Coord(position_x + (temp_width - cut_size.x), position_y + cut_size.y));
  cut.push_back(Coord(cut_size.x, height - 2 * cut_size.y));
  cut.push_back(Coord(cut_size.x, height - 2 * cut_size.y));

  position.push_back(Coord(position_x, position_y + (height - cut_size.y)));
  position.push_back(Coord(position_x + cut_size.x, position_y + (height - cut_size.y)));
  position.push_back(Coord(position_x + (temp_width - cut_size.x), position_y + (height - cut_size.y)));
  cut.push_back(cut_size);
  cut.push_back(Coord(temp_width - 2 * cut_size.x, cut_size.y));
  cut.push_back(cut_size);

  SDL_Rect button_cut = {(int)0, (int)cut_size.x, (int)button_cut_size.x, (int)button_cut_size.y};
  up = Button(engine, button_texture, position_x + temp_width, position_y, height / 2, height / 2, texture, scale, &button_cut, true);

  button_cut.x = button_cut_size.x;
  down = Button(engine, button_texture, position_x + temp_width, position_y + (height / 2), height / 2, height / 2, texture, scale, &button_cut, true);
}

void Picker::draw(const Engine& engine) {
  SDL_Rect a_cut;
  a_cut.w = cut_size.x;
  a_cut.h = cut_size.y;

  a_cut.x = 8 * cut_size.x;
  a_cut.y = 0;
  engine.draw_image(texture, position_x, position_y, width, height, 0, NULL, &a_cut);

  up.draw(engine);
  down.draw(engine);

  for (unsigned i = 0; i < position.size(); i++) {
    a_cut.x = i * cut_size.x;
    a_cut.y = 0;
    engine.draw_image(texture, position[i].x, position[i].y, cut[i].x, cut[i].y, 0, NULL, &a_cut);
  }

  SDL_Rect content_rect;
  SDL_Texture* content = engine.texture_of_text(std::to_string(actual), text_color, text_size, &content_rect);

  content_rect.x = position_x + (((width - (height / 2)) / 2) - (content_rect.w / 2));
  content_rect.y = position_y + ((height / 2) - (content_rect.h / 2));

  engine.draw_texture(content, content_rect.x, content_rect.y, content_rect.w, content_rect.h);
  SDL_DestroyTexture(content);
}

void Picker::update(const Engine& engine) {
  Coord rel_mouse_pos(engine.mouse_position_x - position_x, engine.mouse_position_y - position_y);
  bool in_range = (rel_mouse_pos > Coord(0, 0)) && (rel_mouse_pos < Coord(width, height));

  bool prev = up.mouse_click || down.mouse_click;
  up.update(engine);
  down.update(engine);
  bool curr = up.mouse_click || down.mouse_click;

  if (in_range) { actual += engine.mouse_scroll * step; }

  if (up.mouse_click && (timer <= 0)) { actual += step; }
  if (down.mouse_click && (timer <= 0)) { actual -= step; }

  if (prev && curr) {
    if (timer > 0) { timer -= engine.get_ft(); }
    else { timer = repeat_rate; }
  }
  else if (prev && !curr) { timer = 0; }
  else if (!prev && curr) { timer = repeat_timer; }

  if (actual > max) { actual = max; }
  if (actual < min) { actual = min; }
}
