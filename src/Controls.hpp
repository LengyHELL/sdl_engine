#ifndef H_CONTROLS
#define H_CONTROLS

#include "Engine.hpp"
#include "Coord.hpp"
#include "Rect.hpp"
#include <list>
#include <string>

class Frame {
  std::string style = "";
  std::list<std::pair<Coord, Coord>> style_cut;

  Rect body = Rect(0, 0, 0, 0);

  Coord cut_size = Coord(0, 0);

public:
  // variables
  bool mouse_hover = false;

  // constructor, destructor
  Frame() {}
  Frame(const std::string& style, const Rect& body, const Coord& cut_size = Coord(3, 3));
  ~Frame() {}

  //functions
  void draw(const Engine& engine);
  void update(const Engine& engine);

  const std::string& get_style() const { return style; }
  const Rect& get_body() const { return body; }
  const Coord& get_cut_size() const { return cut_size; }
};

class TextBox {
  Frame text_box_frame;

  Rect body = Rect(0, 0, 0, 0);

  std::string text = "";
  unsigned text_size = 0;
  SDL_Color text_color = {0, 0, 0, 0};

  std::list<std::string> lines;

public:
  // variables
  bool mouse_hover = false;

  // constructor, destructor
  TextBox() {}

  TextBox(const std::string& style, const Rect& body, const std::string& text, const unsigned& text_size,
    const SDL_Color& text_color, const Coord& cut_size = Coord(3, 3));

  ~TextBox() {}

  //functions
  void draw(const Engine& engine);
  void update(const Engine& engine);
};

class TextButton {
  Frame button_frame;

  Rect body = Rect(0, 0, 0, 0);

  std::string text = "";
  unsigned text_size = 0;
  SDL_Color text_color = {0, 0, 0, 0};

  bool on_mouse_down = false;
  bool click_lock = true;

public:
  // variables
  bool mouse_hover = false;
  bool selected = false;
  bool mouse_click = false;

  // constructor, destructor
  TextButton() {}

  TextButton(const std::string& style, const Rect& body, const std::string& text, const unsigned& text_size,
    const SDL_Color& text_color, const bool& on_mouse_down = false, const Coord& cut_size = Coord(3, 3));

  ~TextButton() {}

  //functions
  void draw(const Engine& engine);
  void update(const Engine& engine);
};

class ImageButton {
  Frame button_frame;

  Rect body = Rect(0, 0, 0, 0);

  std::string image = "";

  bool on_mouse_down = false;
  bool click_lock = true;

  Rect source_rect = Rect(0, 0, 0, 0);

public:
  // variables
  bool mouse_hover = false;
  bool selected = false;
  bool mouse_click = false;

  // constructor, destructor
  ImageButton() {}

  ImageButton(const std::string& style, const Rect& body, const std::string& image,
    const bool& on_mouse_down = false, const Rect& source_rect = Rect(0, 0, 0, 0), const Coord& cut_size = Coord(3, 3));

  ~ImageButton() {}

  //functions
  void draw(const Engine& engine);
  void update(const Engine& engine);
};

class Picker {
  Frame picker_frame;
  ImageButton up;
  ImageButton down;

  Rect body = Rect(0, 0, 0, 0);

  unsigned text_size = 0;
  SDL_Color text_color = {0, 0, 0, 0};

  float repeat_timer = 500;
  float repeat_rate = 100;

  float timer = 0;

public:
  // variables
  float min = 0;
  float max = 0;
  float step = 0;
  float actual = 0;

  // constructor, destructor
  Picker() {}

  Picker(const std::string& style, const Rect& body, const unsigned& text_size,
    const SDL_Color& text_color, const float& min, const float& max, const float& step,
    const float& actual, const Coord& button_cut_size = Coord(10, 10), const Coord& cut_size = Coord(3, 3));

  ~Picker() {}

  //functions
  void draw(const Engine& engine);
  void update(const Engine& engine);
};

#endif
