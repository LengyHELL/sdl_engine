#ifndef H_CONTROLS
#define H_CONTROLS

#include "Engine.hpp"
#include "Coord.hpp"
#include <vector>
#include <string>

class Button {
  std::string texture = "";
  float position_x = 0;
  float position_y = 0;
  float width = 0;
  float height = 0;

  SDL_Texture* content = NULL;
  SDL_Rect content_rect;
  SDL_Rect content_cut;
  bool cut_source = false;

  bool on_mouse_down = false;
  bool mouse_hover = false;
  bool click_lock = true;

  std::vector<Coord> position;
  std::vector<Coord> cut;
  Coord cut_size = Coord(3, 3);

  bool allocated = false; //set true for locally allocated textures

public:
  // variables
  bool selected = false;
  bool mouse_click = false;

  // constructor, destructor
  Button() {}

  Button(const Engine& engine, const std::string& texture, const float& position_x, const float& position_y,
    const float& width, const float& height, const std::string& text, const unsigned& size, const SDL_Color& text_color,
    const bool& on_mouse_down);

  Button(const Engine& engine, const std::string& texture, const float& position_x, const float& position_y,
    const float& width, const float& height, const std::string& image, const float& scale, SDL_Rect* image_cut,
    const bool& on_mouse_down);

  ~Button() {
    if (allocated) { SDL_DestroyTexture(content); }
  }

  //functions
  void draw(const Engine& engine);
  void update(const Engine& engine);
};

class Picker {
  std::string texture = "";
  float position_x = 0;
  float position_y = 0;
  float width = 0;
  float height = 0;

  float scale = 0;
  SDL_Color text_color;
  unsigned text_size;

  std::vector<Coord> position;
  std::vector<Coord> cut;
  Coord cut_size = Coord(3, 3);

  Button up;
  Button down;
  Coord button_cut_size = Coord(10, 10);

  float repeat_timer = 500;
  float repeat_rate = 100;

  float timer = 0;

public:
  // variables
  int min = 0;
  int max = 0;
  int actual = 0;
  int step = 1;

  // constructor, destructor
  Picker() {}

  Picker(const Engine& engine, const std::string& texture, const std::string& button_texture, const float& position_x,
    const float& position_y, const float& width, const float& height, const float& scale, const SDL_Color& text_color,
    const unsigned& text_size, const int& min, const int& max, const int& actual, const int& step);

  ~Picker() {}

  //functions
  void draw(const Engine& engine);
  void update(const Engine& engine);
};

#endif
