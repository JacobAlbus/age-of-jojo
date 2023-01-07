#include "gui/age_of_jojo.h"
#include "engine/styles.h"
#include <sstream>
#include <algorithm>
//#include "gui/board_game_renderer.h"
//#include "gui/pause_menu_renderer.h"
//#include "gui/main_menu_renderer.h"
//#include "gui/level_select_menu_renderer.h"

namespace age_of_jojo {

AgeOfJojo::AgeOfJojo() : battle_engine_(BattleEngine()) {
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

void AgeOfJojo::update() {
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
  UpdateFPSBall();

  float margin = kWindowSize_ / 5;
  float camera_speed = 60.0f;
  if (mouse_coords_.x > kWindowSize_ - margin && top_right_corner_.x > -kBackgroundHorizontal_ + kWindowSize_) {
    top_right_corner_.x -= mouse_coords_.x / camera_speed;
  }
  if (mouse_coords_.x < margin && top_right_corner_.x < 0) {
    top_right_corner_.x += (kWindowSize_ - mouse_coords_.x) / camera_speed;
  }

  battle_engine_.HandlePlayer2Input(top_right_corner_);
  battle_engine_.UpdateUnitPositions(top_right_corner_);
}

void AgeOfJojo::draw() {
  ci::gl::clear(ci::Color("black"));

  ci::gl::color(ci::Color("white"));
  ci::gl::draw(background_, ci::Rectf(top_right_corner_.x,
                                      top_right_corner_.y,
                                      styles::kBackgroundLength_ + top_right_corner_.x,
                                      styles::kBackgroundHeight_ + top_right_corner_.y));


//  DebugScreen();
  battle_engine_.RenderAllUnits(top_right_corner_);
  battle_engine_.RenderBases(top_right_corner_);
}

void AgeOfJojo::keyDown(ci::app::KeyEvent event) {
  battle_engine_.HandlePlayer1Input(event, top_right_corner_);
}

void AgeOfJojo::mouseMove(ci::app::MouseEvent event) {
  mouse_coords_ = event.getPos();

  float margin_of_error = 3; // to avoid divide by 0 errors

  if (mouse_coords_.x >= kWindowSize_ - margin_of_error) {
    mouse_coords_.x = kWindowSize_ - margin_of_error;
  } else if (mouse_coords_.x <= margin_of_error) {
    mouse_coords_.x = margin_of_error;
  }
}

void AgeOfJojo::PlaySound(const std::string& file_path) {
//  if (options::play_sounds) {
//    ci::audio::SourceFileRef source = ci::audio::load(ci::app::loadAsset(file_path));
//    sound_player_ = ci::audio::Voice::create(source);
//    sound_player_->start();
//  }
}

void AgeOfJojo::PlayMusic(const std::string& file_path) {
//  if (options::play_music) {
//    ci::audio::SourceFileRef source = ci::audio::load(ci::app::loadAsset(file_path));
//    music_player_ = ci::audio::Voice::create(source);
//    music_player_->start();
//  }
}

void AgeOfJojo::PlayRandomSong() {
//  std::random_device r;
//
//  std::default_random_engine e1(r());
//  std::uniform_int_distribution<int> uniform_dist(0, kSongNames_.size() - 1);
//  int random_index = uniform_dist(e1);
//
//  std::string file_path = kSongNames_[random_index];
//  PlayMusic(file_path);
}

void AgeOfJojo::UpdateFPSBall() {
  if (moving_ball_coords_.x > kBackgroundHorizontal_) {
    is_moving_right_ = false;
  } else if (moving_ball_coords_.x < 0) {
    is_moving_right_ = true;
  }

  if (is_moving_right_) {
    moving_ball_coords_.x += 10;
  } else {
    moving_ball_coords_.x -= 10;
  }
}

void AgeOfJojo::DebugScreen() const {
  std::ostringstream ss;
  ss << "(" << top_right_corner_.x << ", " << top_right_corner_.y << ")";
  std::string s(ss.str());

  ci::gl::drawStringCentered(s,
                             glm::vec2(300, 100),
                             ci::Color("pink"),
                             ci::Font("Helvetica", 60));

  std::ostringstream sss;
  sss << "(" << mouse_coords_.x << ", " << mouse_coords_.y << ")";
  std::string s2(sss.str());

  ci::gl::drawStringCentered(s2,
                             glm::vec2(300, 200),
                             ci::Color("pink"),
                             ci::Font("Helvetica", 60));

  std::ostringstream ssss;
  ssss << "(" << moving_ball_coords_.x << ", " << moving_ball_coords_.y << ")";
  std::string s3(ssss.str());

  ci::gl::drawStringCentered(s3,
                             glm::vec2(300, 300),
                             ci::Color("pink"),
                             ci::Font("Helvetica", 60));

  ci::gl::color(ci::Color("purple"));
  glm::vec2 update_coords(moving_ball_coords_.x + top_right_corner_.x, moving_ball_coords_.y);
  ci::gl::drawSolidCircle(update_coords, 30);
}

}  // namespace jjba_strategy
