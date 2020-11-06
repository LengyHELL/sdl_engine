#include "Engine.hpp"
#include <iostream>
#include <math.h>

Engine::Engine(const int& width, const int& height, const std::string& title, const bool& vsync) : width(width), height(height), exit(false), vsync(vsync) {
  if(SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL initialization failed!, SDL_Error: " << SDL_GetError() << std::endl;
  }
  else {
    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (window == NULL) {
      std::cerr << "Window initialization failed! SDL_Error: " << SDL_GetError() << std::endl;
    }
    else {
      if (vsync) { renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); }
      else { renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); }
      if (renderer == NULL) {
        std::cerr << "Renderer initialization failed! SDL_Error: " << SDL_GetError() << std::endl;
      }
    }
  }
  if (TTF_Init() < 0) {
    std::cerr << "TTF initialization failed!, SDL_Error: " << SDL_GetError() << std::endl;
  }

  update_inputs();
}

Engine::~Engine() {
  for (auto& i : images) {
    SDL_DestroyTexture(i.second);
  }
  for (auto& f : font) {
    TTF_CloseFont(f.second);
  }
  TTF_Quit();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void Engine::render() {

  //calculating fps
  fps_timer += get_ft();
  frame_counter++;

  if (fps_timer >= 1000) {
    fps_timer -= 1000;
    fps = frame_counter;
    frame_counter = 0;
  }

  SDL_RenderPresent(renderer);
  SDL_RenderClear(renderer);
}

void Engine::load_font(const std::string& font_path, const unsigned& min_size, const unsigned& max_size, const unsigned& step) {
  for (auto& f : font) {
    TTF_CloseFont(f.second);
  }
  if ((((int)max_size - (int)min_size) / step) > 128) {
    std::cerr << "Engine::load_font: Warning, may experience performance issues due to overloaded font sizes!" << std::endl;
  }
  for (unsigned i = min_size; i <= max_size; i += step) {
    TTF_Font* temp_font = TTF_OpenFont(font_path.c_str(), i);
    if (temp_font == NULL) {
        std::cerr << "Font initialization failed at size: " << i << "!, SDL_Error: " << SDL_GetError() << std::endl;
    }
    else {
      font[i] = temp_font;
    }
  }
}

void Engine::load_image(const std::string& image_path) {
  SDL_Texture* texture = load_texture(image_path);
  if (texture == NULL) {
    std::cerr << "Image load failed!, Image: " << image_path << std::endl;
  }
  else {
    images.insert(std::make_pair(image_path, texture));
  }
}

void Engine::unload_image(const std::string& image_path) {
  SDL_DestroyTexture(images[image_path]);
  images.erase(image_path);
}

SDL_Texture* Engine::load_texture(const std::string& filename, SDL_Rect* area) const {
  SDL_Texture* texture = NULL;
  SDL_Surface* surface = IMG_Load(filename.c_str());
  if (surface == NULL) {
    std::cerr << "Engine::load_texture: load failed!, File: " + filename << IMG_GetError() << std::endl;
  }
  else {
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (area != NULL) {
      area->x = 0;
      area->y = 0;
      area->w = surface->w;
      area->h = surface->h;
    }
    SDL_FreeSurface(surface);
  }
  return texture;
}

SDL_Texture* Engine::texture_of_text(const std::string& text, const SDL_Color& color, const unsigned& size, SDL_Rect* area) const {
  SDL_Texture* texture = NULL;
  if (!font.count(size)) {
    std::cerr << "Engine::texture_of_text: invalid font size of " << size << std::endl;
    return texture;
  }
  SDL_Surface* surface = TTF_RenderText_Solid(font.at(size), text.c_str(), color);
  if (surface == NULL) {
    std::cerr << "Engine::texture_of_text: cannot create text" << std::endl;
  }
  else {
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (area != NULL) {
      area->x = 0;
      area->y = 0;
      area->w = surface->w;
      area->h = surface->h;
    }
    SDL_FreeSurface(surface);
  }
  return texture;
}

SDL_Texture* const Engine::texture_of_image(const std::string& filename) const {
  SDL_Texture* texture = NULL;
  try { texture = images.at(filename); }
  catch (const std::out_of_range& e) {
    std::cerr << "Engine::texture_of_image: image not loaded! " + filename << std::endl;
  }
  return texture;
}

void Engine::draw_texture(SDL_Texture* texture, const float& position_x, const float& position_y, const float& width, const float& height, const float& angle, const SDL_Color* color, const SDL_Rect* const cut, SDL_Rect* area) const {
  bool def = false;
  if ((width == 0) && (height == 0)) {
    def = true;
  }
  else if ((width < 0) || (height < 0)) {
    std::cerr << "Engine::draw_texture: invalid size" << std::endl;
  }

  SDL_Rect rect;
  rect.x = round(position_x);
  rect.y = round(position_y);

  if (!def) {
    rect.w = round(width);
    rect.h = round(height);
  }
  else {
    SDL_QueryTexture(texture, 0, 0, &rect.w, &rect.h);
  }

  if (color != NULL) {
    SDL_SetTextureColorMod(texture, color->r, color->g, color->b);
    SDL_SetTextureAlphaMod(texture, color->a);
  }
  if (SDL_RenderCopyEx(renderer, texture, cut, &rect, angle, NULL, SDL_FLIP_NONE) != 0) {
    std::cerr << "Texture copying failed!, SDL_Error: " << SDL_GetError() << std::endl;
  }
  SDL_SetTextureColorMod(texture, 255, 255, 255);
  SDL_SetTextureAlphaMod(texture, 255);

  if (area != NULL) {
    area->x = rect.x;
    area->y = rect.y;
    area->w = rect.w;
    area->h = rect.h;
  }
}

void Engine::draw_text(const std::string& text, const int& position_x, const int& position_y, const SDL_Color& color, const unsigned& size) const {
  SDL_Texture* texture = texture_of_text(text, color, size, NULL);
  draw_texture(texture, position_x, position_y);
  SDL_DestroyTexture(texture);
}

void Engine::draw_image(const std::string& filename, const float& position_x, const float& position_y, const float& width, const float& height, const float& angle, const SDL_Color* color, SDL_Rect* cut, SDL_Rect* area) const {
  SDL_Texture* texture = texture_of_image(filename);
  draw_texture(texture, position_x, position_y, width, height, angle, color, cut, area);
}

void Engine::update_inputs() {
  if (fps_cap > 0) {
    Uint32 t_frame_time = SDL_GetTicks() - ticks;
    if (t_frame_time < (1000 / fps_cap)) {
      SDL_Delay((1000 / fps_cap) - t_frame_time);
    }
  }
  frame_time = SDL_GetTicks() - ticks;
  ticks = SDL_GetTicks();

  mouse_button_up = false;
  mouse_button_down = false;
  mouse_scroll = 0;
  while(SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      exit = true;
    }
    if (event.type == SDL_MOUSEWHEEL) {
      if (event.wheel.y > 0) { mouse_scroll = 1; }
      if (event.wheel.y < 0) { mouse_scroll = -1; }
    }
    mouse_button_up = (event.type == SDL_MOUSEBUTTONUP);
    mouse_button_down = (event.type == SDL_MOUSEBUTTONDOWN);
  }
  keyboard_state = SDL_GetKeyboardState(NULL);
  if (mouse_button_up) {
    SDL_GetMouseState(&mouse_position_x, &mouse_position_y);
  }
  else {
    mouse_state = SDL_GetMouseState(&mouse_position_x, &mouse_position_y);
  }
}

void Engine::resize_window(const int& new_width, const int& new_height) {
  width = new_width;
  height = new_height;
  SDL_SetWindowSize(window, width, height);
}
