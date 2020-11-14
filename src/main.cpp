#include "Engine.hpp"
#include "Rect.hpp"
#include "Coord.hpp"

#include "TextButton.hpp"
#include "ImageButton.hpp"
#include "Frame.hpp"
#include "Picker.hpp"
#include "TextBox.hpp"
#include "InputBox.hpp"

#include <time.h>
#include <list>

class Bird {
  Coord pos = Coord(100, 240);
  Coord force = Coord(10, 0);
  Rect body = Rect(0, 0, 30, 30);
  bool jump_lock = false;

public:
  void draw(const Engine& engine) {
    body.x = pos.x - body.w / 2;
    body.y = pos.y - body.h / 2;
    float deg = force.get_degree(Coord(1, 0));
    if (force.y < 0) { deg *= -1; }
    engine.draw_image("img/bird.png", body, deg);
  }

  void update(const Engine& engine) {
    if (engine.keyboard_state[SDL_SCANCODE_SPACE] && !jump_lock) {
      jump_lock = true;
      force.y = -50;
    }
    else { force.y += 1; }
    if (!engine.keyboard_state[SDL_SCANCODE_SPACE]) { jump_lock = false; }

    pos.y += force.y * (engine.get_ft() / 100);
  }

  Coord get_pos() const { return pos; }
};

class Wall {
  Rect gap = Rect(0, 0, 0, 0);

public:
  Wall(const int& x) : gap(x, rand() % 405, 50, 75) {}
  float speed = -10;

  bool out() { return (gap.x + gap.w) < 0; }

  bool hit(const Bird& bird) const {
    Coord pos = bird.get_pos();
    pos -= Coord(gap.x, gap.y);
    if ((pos.x >= 0) && (pos.x <= gap.w)) {
      return (pos.y < 0) || (pos.y > gap.h);
    }
    return false;
  }

  void draw(const Engine& engine) {
    Rect top_body(gap.x, gap.y - engine.get_height(), gap.w, engine.get_height());
    Rect bottom_body(gap.x, gap.y + gap.h, gap.w, engine.get_height());

    engine.draw_image("img/top_tube.png", top_body);
    engine.draw_image("img/bottom_tube.png", bottom_body);
  }

  void update(const Engine& engine) {
    gap.x += speed * (engine.get_ft() / 100);
  }

  float get_side() const { return gap.x + gap.w; }
};

int main(int argc, char** argv) {
  // Initializing engine
  srand(time(NULL));

  Engine engine(640, 480, "csuriz.exxe", false);
  engine.set_fps_cap(150);
  engine.load_font("lhll.ttf");
  engine.load_image("img/bottom_tube.png");
  engine.load_image("img/top_tube.png");
  engine.load_image("img/background.png");
  engine.load_image("img/bird.png");

  engine.load_image("img/basic_style.png");

  // Creating menu elements
  TextButton tb1("img/basic_style.png", Rect(295, 225, 50, 30), "Start", 16, {0, 0, 0, 0});

  // Creating game objects
  bool menu = true;
  Bird bird;
  std::list<Wall> walls;
  bool space_lock = true;
  int wall_pos = 0;
  bool score_lock = false;
  unsigned score = 0;

  while(!engine.get_exit()) {
    engine.update_inputs();

    if(menu) {
      tb1.update(engine);
      if (tb1.selected) {
        tb1.selected = false;
        menu = false;
      }
      if (engine.keyboard_state[SDL_SCANCODE_SPACE] && !space_lock) {
        space_lock = true;
        menu = false;
      }
      if (!engine.keyboard_state[SDL_SCANCODE_SPACE]) { space_lock = false; }

      if (!menu) {
        score = 0;
        walls.clear();
        bird = Bird();
        wall_pos = 600;
        for (int i = 0; i < 5; ++i) {
          walls.push_back(Wall(wall_pos));
          wall_pos += 300;
        }

        wall_pos -= 650;
      }
    }
    if(!menu) {
      if (walls.front().out()) {
        score_lock = false;
        walls.pop_front();
        walls.push_back(Wall(wall_pos));
      }
      if (walls.front().hit(bird)) { menu = true; }
      Coord pos = bird.get_pos();
      if ((pos.y < 0) || (pos.y > (float)engine.get_height())) { menu = true; }
      if (pos.x > walls.front().get_side() && !score_lock) {
        score_lock = true;
        score += 1;
      }

      bird.update(engine);
      for (auto& w : walls) { w.update(engine); }
    }

    engine.draw_image("img/background.png", Rect(0, 0, engine.get_width(), engine.get_height()));

    for (auto& w : walls) { w.draw(engine); }
    bird.draw(engine);
    if (menu) { tb1.draw(engine); }

    engine.draw_text("Score: " + std::to_string(score), Coord(280, 50), {0, 0, 0, 0}, 32);

    engine.draw_text(std::to_string(engine.get_fps()) + " fps", Coord(5, 5), {255, 0, 0, 0}, 16);
    engine.render();
  }
  return 0;
}
