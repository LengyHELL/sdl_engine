#include "Engine.hpp"
#include "Rect.hpp"
#include "Coord.hpp"

#include "TextButton.hpp"
#include "ImageButton.hpp"
#include "Frame.hpp"
#include "Picker.hpp"
#include "TextBox.hpp"
#include "InputBox.hpp"

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
  InputBox in1("img/basic_style.png", button_body.copy(20, 230, 75, 0), 16, {0, 0, 0, 0});

  while(!engine.get_exit()) {
    engine.update_inputs();

    f1.update(engine);
    tb1.update(engine);
    ib1.update(engine);
    p1.update(engine);
    tx1.update(engine);
    in1.update(engine);

    f1.draw(engine);
    tb1.draw(engine);
    ib1.draw(engine);
    p1.draw(engine);
    tx1.draw(engine);
    in1.draw(engine);

    engine.draw_text(std::to_string(engine.get_fps()) + " fps", Coord(5, 5), {255, 0, 0, 0}, 16);
    engine.render();
  }
  return 0;
}
