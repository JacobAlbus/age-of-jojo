#include "gui/jjba_strategy_app.h"
#include <sstream>
#include <algorithm>
//#include "gui/board_game_renderer.h"
//#include "gui/pause_menu_renderer.h"
//#include "gui/main_menu_renderer.h"
//#include "gui/level_select_menu_renderer.h"

namespace jjba_strategy {

JJBAStrategyApp::JJBAStrategyApp() {
//  render_engines_.emplace(std::make_pair(GameState::kMainMenu, new MainMenuRenderer()));
//  render_engines_.emplace(std::make_pair(GameState::kPauseMenu, new PauseMenuRenderer()));
//  render_engines_.emplace(std::make_pair(GameState::kPlayingBoard, new BoardGameRenderer()));
//  render_engines_.emplace(std::make_pair(GameState::kLevelSelectMenu, new LevelSelectRenderer()));
//
//  kSongNames_.emplace_back("audio/johnathan_theme.wav");
//  kSongNames_.emplace_back("audio/kira_theme.mp3");
//  kSongNames_.emplace_back("audio/pillar_men_theme.mp3");
//
//  current_render_engine_ = render_engines_[current_game_state_];
//
//  PlayMusic("audio/blank_audio.mp3"); // needed for initialization

  ci::app::setWindowSize((int) kWindowSize_, (int) kWindowSize_);
  top_right_corner_ = glm::vec2(0, 0);
  moving_ball_coords_ = glm::vec2(100, 100);
  is_moving_right_ = true;
  mouse_coords_ = glm::vec2((int) kWindowSize_ / 2, (int) kWindowSize_ / 2);

  std::string file_path = "maps/background.png";
  ci::fs::path path = ci::fs::path(file_path);
  background_ = ci::gl::Texture::create(ci::loadImage(cinder::app::loadAsset(path)));
}

// TODO look into memory leak

void JJBAStrategyApp::update() {
//  if (!music_player_->isPlaying() && options::play_music) {
//    PlayRandomSong();
//  }
//
//  if (!options::play_music) {
//    music_player_->stop();
//  }
//
//  if (previous_loaded_game_ != global_variables::current_loaded_game) {
//    global_variables::has_loaded_game_changed = true;
//    previous_loaded_game_ = global_variables::current_loaded_game;
//  }

//  current_render_engine_->Update();
  if (moving_ball_coords_.x > kWindowSize_) {
    is_moving_right_ = false;
  } else if (moving_ball_coords_.x < 0) {
    is_moving_right_ = true;
  }

  if (is_moving_right_) {
    moving_ball_coords_.x += 10;
  } else {
    moving_ball_coords_.x -= 10;
  }

  float margin = kWindowSize_ / 5;
  float camera_speed = 60.0f;
  if (mouse_coords_.x > kWindowSize_ - margin && top_right_corner_.x < 2000 - kWindowSize_) {
    top_right_corner_.x += mouse_coords_.x / camera_speed;
  }
  if (mouse_coords_.x < margin && top_right_corner_.x > 0) {
    top_right_corner_.x -= (kWindowSize_ - mouse_coords_.x) / camera_speed;
  }
}

void JJBAStrategyApp::draw() {
  ci::gl::clear(ci::Color("black"));

  ci::gl::color(ci::Color("white"));
  ci::gl::draw(background_, ci::Rectf(-top_right_corner_.x, top_right_corner_.y, 2000 - top_right_corner_.x, 900 + top_right_corner_.y));

  ci::gl::color(ci::Color("purple"));
  ci::gl::drawSolidCircle(moving_ball_coords_, 30);
}


void JJBAStrategyApp::mouseMove(ci::app::MouseEvent event) {
  mouse_coords_ = event.getPos();

  float margin_of_error = 3; // to avoid divide by 0 errors

  if (mouse_coords_.x >= kWindowSize_ - margin_of_error) {
    mouse_coords_.x = kWindowSize_ - margin_of_error;
  } else if (mouse_coords_.x <= margin_of_error) {
    mouse_coords_.x = margin_of_error;
  }
}

void JJBAStrategyApp::PlaySound(const std::string& file_path) {
//  if (options::play_sounds) {
//    ci::audio::SourceFileRef source = ci::audio::load(ci::app::loadAsset(file_path));
//    sound_player_ = ci::audio::Voice::create(source);
//    sound_player_->start();
//  }
}

void JJBAStrategyApp::PlayMusic(const std::string& file_path) {
//  if (options::play_music) {
//    ci::audio::SourceFileRef source = ci::audio::load(ci::app::loadAsset(file_path));
//    music_player_ = ci::audio::Voice::create(source);
//    music_player_->start();
//  }
}

void JJBAStrategyApp::PlayRandomSong() {
//  std::random_device r;
//
//  std::default_random_engine e1(r());
//  std::uniform_int_distribution<int> uniform_dist(0, kSongNames_.size() - 1);
//  int random_index = uniform_dist(e1);
//
//  std::string file_path = kSongNames_[random_index];
//  PlayMusic(file_path);
}

}  // namespace jjba_strategy
