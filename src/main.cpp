#include "Engine.hpp"
#include "TextButton.hpp"
#include "ImageButton.hpp"
#include "Frame.hpp"
#include "Picker.hpp"
#include "TextBox.hpp"

int main(int argc, char** argv) {
  Engine engine(640, 480, "Test", false);

  engine.load_font("lhll.ttf");
  engine.load_image("img/no_part.png");
  engine.load_image("img/basic_style.png");

  std::string text = "Somebody once told me, the world is gonna roll me, i aint the sharpest tool in the shed.";

  Rect button_body(10, 10, 75, 25);
  TextButton tb1("img/basic_style.png", button_body.copy(20, 10), "Button", 16, {0, 0, 0, 0});
  ImageButton ib1("img/basic_style.png", button_body.copy(20, 40), "img/no_part.png");
  Frame f1("img/basic_style.png", Rect(10, 10, engine.get_width() - 20, engine.get_height() - 20));
  Picker p1("img/basic_style.png", button_body.copy(20, 70, 0, 25), 16, {0, 0, 0, 0}, 0.5, 23.1, 0.1, 10);
  TextBox tx1("img/basic_style.png", button_body.copy(20, 125, 75, 75), text, 16, {0, 0, 0, 0});

  while(!engine.get_exit()) {
    engine.update_inputs();

    f1.update(engine);
    tb1.update(engine);
    ib1.update(engine);
    p1.update(engine);
    tx1.update(engine);

    f1.draw(engine);
    tb1.draw(engine);
    ib1.draw(engine);
    p1.draw(engine);
    tx1.draw(engine);

    if (tb1.selected) {
      tb1.selected = false;
      SDL_StartTextInput();
    }

    if (ib1.selected) {
      ib1.selected = false;
      SDL_StopTextInput();
    }

    int min;
    int max;
    if (engine.cursor < engine.selection) {
      min = engine.cursor;
      max = engine.selection;
    }
    else {
      min = engine.selection;
      max = engine.cursor;
    }

    std::string start = engine.text.substr(0, min);
    std::string middle = engine.text.substr(min, max - min);
    std::string end = engine.text.substr(max);

    engine.draw_text(start, Coord(30, 400), {0, 0, 0, 0}, 16);

    int x_text = engine.size_text(start, 16).w;
    engine.draw_text(middle, Coord(30 + x_text, 400), {255, 0, 0, 0}, 16);
    if (min == engine.cursor) { engine.draw_image("img/no_part.png", Rect(30 + x_text, 400, 2, 16)); }

    x_text = engine.size_text(start + middle, 16).w;
    engine.draw_text(end, Coord(30 + x_text, 400), {0, 0, 0, 0}, 16);
    if (max == engine.cursor) { engine.draw_image("img/no_part.png", Rect(30 + x_text, 400, 2, 16)); }

    engine.draw_text(std::to_string(engine.cursor), Coord(30, 420), {0, 0, 0, 0}, 16);

    engine.draw_text(std::to_string(engine.get_fps()) + " fps", Coord(5, 5), {255, 0, 0, 0}, 16);
    engine.render();
  }
  return 0;
}
