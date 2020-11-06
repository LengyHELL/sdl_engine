#include "Engine.hpp"
#include "Controls.hpp"

int main(int argc, char** argv) {
  Engine engine(640, 480, "Test", false);

  engine.load_font("lhll.ttf");
  engine.load_image("img/no_part.png");
  engine.load_image("img/basic_button.png");
  engine.load_image("img/basic_picker.png");

  Button b1(engine, "img/basic_button.png", 150, 20, 75, 25, "Button", 16, {0, 0, 0, 0}, true);
  Picker red(engine, "img/basic_picker.png", "img/basic_button.png", 150, 50, 75, 50, 1, {100, 0, 0, 0}, 16, 0, 255, 127, 1);
  Picker green(engine, "img/basic_picker.png", "img/basic_button.png", 150, 105, 75, 50, 1, {0, 100, 0, 0}, 16, 0, 255, 127, 1);
  Picker blue(engine, "img/basic_picker.png", "img/basic_button.png", 150, 160, 75, 50, 1, {0, 0, 100, 0}, 16, 0, 255, 127, 1);
  Button b2(engine, "img/basic_button.png", 230, 20, 75, 25, "Gomb A", 16, {0, 0, 0, 0}, true);

  unsigned sz = 50;

  while(!engine.get_exit()) {
    engine.update_inputs();

    b1.update(engine);
    b2.update(engine);
    red.update(engine);
    green.update(engine);
    blue.update(engine);

    b1.draw(engine);
    b2.draw(engine);
    red.draw(engine);
    green.draw(engine);
    blue.draw(engine);

    engine.draw_text(std::to_string(engine.get_fps()) + " fps", 5, 5, {100, 100, 100, 0}, 16);
    engine.draw_text("Frame time: " + std::to_string(engine.get_ft()) + " ms", 5, 20, {100, 100, 100, 0}, 16);
    engine.draw_text("Hello world!", engine.get_mouse_pos_x(), engine.get_mouse_pos_y(), {(Uint8)red.actual, (Uint8)green.actual, (Uint8)blue.actual, 0}, 32);

    if (b1.selected) {
      sz /= 2;
      if (sz < 1) { sz = 1; }
    }
    if (b2.selected) {
      sz *= 2;
      if (sz > 800) { sz = 800; }
    }
    engine.draw_image("img/no_part.png", 150, 220, sz, sz);

    engine.render();
  }
  return 0;
}
