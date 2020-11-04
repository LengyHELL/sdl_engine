#include "Engine.hpp"
#include "Controls.hpp"

int main(int argc, char** argv) {
  Engine engine(640, 480, "Test");

  engine.load_font("lhll.ttf");
  engine.load_image("img/no_part.png");
  engine.load_image("img/basic_button.png");
  engine.load_image("img/basic_picker.png");

  Button b1(engine, "img/basic_button.png", 150, 20, 75, 25, "Button", 16, {0, 0, 0, 0}, true);
  Picker red(engine, "img/basic_picker.png", "img/basic_button.png", 150, 50, 75, 50, 1, {100, 0, 0, 0}, 16, 0, 255, 127, 1);
  Picker green(engine, "img/basic_picker.png", "img/basic_button.png", 150, 105, 75, 50, 1, {0, 100, 0, 0}, 16, 0, 255, 127, 1);
  Picker blue(engine, "img/basic_picker.png", "img/basic_button.png", 150, 160, 75, 50, 1, {0, 0, 100, 0}, 16, 0, 255, 127, 1);

  unsigned sz = 50;

  while(!engine.get_exit()) {
    engine.update_inputs();

    b1.update(engine);
    red.update(engine);
    green.update(engine);
    blue.update(engine);

    b1.draw(engine);
    red.draw(engine);
    green.draw(engine);
    blue.draw(engine);

    engine.draw_text(std::to_string(engine.get_fps()) + " fps", 5, 5, {100, 100, 100, 0}, 16);
    engine.draw_text("Frame time: " + std::to_string((int)engine.get_ft()) + " ms", 5, 20, {100, 100, 100, 0}, 16);
    engine.draw_text("Hello world!", engine.mouse_position_x, engine.mouse_position_y, {(Uint8)red.actual, (Uint8)green.actual, (Uint8)blue.actual, 0}, 32);

    if (b1.selected) {
      b1.selected = false;
      sz /= 2;
      if (sz < 1) { sz = 1; }
    }
    engine.draw_image("img/no_part.png", 150, 220, sz, sz);

    engine.render();
  }
  return 0;
}
