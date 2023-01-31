#include "gui/age_of_jojo.h"
#include "engine/consts.h"
#include <sstream>
#include <algorithm>
#include <random>

namespace age_of_jojo {

AgeOfJojo::AgeOfJojo() : battle_engine_(BattleEngine()), game_mode_(GameMode::kPlaying_) {
  kSongNames_.emplace_back("audio/music/johnathan_theme.wav");
  kSongNames_.emplace_back("audio/music/kira_theme.mp3");
  kSongNames_.emplace_back("audio/music/pillar_men_theme.mp3");
  kSongNames_.emplace_back("audio/music/main_theme.mp3");
  PlayMusic("audio/blank_audio.mp3"); // needed for initialization

  ci::app::setWindowSize((int) styles::kWindowLength_, (int) styles::kWindowHeight_);
  top_right_corner_ = glm::vec2(0, 0);
  moving_ball_coords_ = glm::vec2(100, 100);
  is_moving_right_ = true;
  mouse_coords_ = glm::vec2((int) styles::kWindowLength_ / 2, (int) styles::kWindowHeight_ / 2);

  std::string file_path = "maps/background.png";
  ci::fs::path path = ci::fs::path(file_path);
  background_ = ci::gl::Texture::create(ci::loadImage(cinder::app::loadAsset(path)));
}

void AgeOfJojo::keyDown(ci::app::KeyEvent event) {
  switch (game_mode_) {
    case GameMode::kPlaying_:
      battle_engine_.HandleKeyDown(event, top_right_corner_);
      break;
    case GameMode::kDioWins_:
    case GameMode::kJojoWins_:
      RestartGame();
  }
}


void AgeOfJojo::update() {
  switch (game_mode_) {
    case GameMode::kPlaying_:
      UpdateGame();
      break;
    case GameMode::kDioWins_:
    case GameMode::kJojoWins_:
      int seconds = 2;
      PauseGame(seconds);
      RestartGame();
      return;
  }
}

void AgeOfJojo::UpdateGame() {
  if (!music_player_->isPlaying()) {
    PlayRandomSong();
  }

  UpdateFPSBall();

  float margin = styles::kWindowLength_ / 5;
  float camera_speed = 60.0f;
  if (mouse_coords_.x > styles::kWindowLength_ - margin &&
      top_right_corner_.x > -styles::kBackgroundLength_ + styles::kWindowLength_) {
    top_right_corner_.x -= mouse_coords_.x / camera_speed;
  }

  if (mouse_coords_.x < margin && top_right_corner_.x < 0 &&
      mouse_coords_.y > game_values::kUpgradeButtonPos_.getY2()) {
    top_right_corner_.x += (styles::kWindowLength_ - mouse_coords_.x) / camera_speed;
  }

  battle_engine_.UpdateGameState(top_right_corner_);
  game_mode_ = battle_engine_.GetGameMode();
}

void AgeOfJojo::draw() {
  switch (game_mode_) {
    case GameMode::kPlaying_:
      DrawGame();
      break;
    case GameMode::kDioWins_:
    case GameMode::kJojoWins_:
      DisplayWinScreen();
  }
}

void AgeOfJojo::DisplayWinScreen() {
  ci::gl::clear(ci::Color("black"));

  std::string winner;
  if (game_mode_ == GameMode::kDioWins_) {
    winner = "Dio Wins!";
  } else {
    winner = "Jojo Wins!";
  }

  ci::gl::drawStringCentered(winner,
                             glm::vec2(styles::kWindowLength_ / 2, styles::kWindowHeight_ / 2),
                             ci::Color("pink"),
                             ci::Font("Helvetica", 60));

  ci::gl::drawStringCentered("Press Any Key to Restart",
                             glm::vec2(styles::kWindowLength_ / 2, styles::kWindowHeight_ / 1.5),
                             ci::Color("pink"),
                             ci::Font("Helvetica", 30));
}

void AgeOfJojo::DrawGame() {
  ci::gl::clear(ci::Color("black"));

  ci::gl::color(ci::Color("white"));
  ci::gl::draw(background_, ci::Rectf(top_right_corner_.x,
                                      top_right_corner_.y,
                                      styles::kBackgroundLength_ + top_right_corner_.x,
                                      styles::kBackgroundHeight_ + top_right_corner_.y));

//  DebugScreen();
  battle_engine_.RenderAllUnits(top_right_corner_);
  battle_engine_.RenderBases(top_right_corner_);
  battle_engine_.RenderPlayer1HUD();
  battle_engine_.RenderPlayer1Queue(top_right_corner_);
  battle_engine_.RenderUnitCost(mouse_coords_);
}

void AgeOfJojo::mouseMove(ci::app::MouseEvent event) {
  mouse_coords_ = event.getPos();

  float margin_of_error = 3; // to avoid divide by 0 errors

  if (mouse_coords_.x >= styles::kWindowLength_ - margin_of_error) {
    mouse_coords_.x = styles::kWindowLength_ - margin_of_error;
  } else if (mouse_coords_.x <= margin_of_error) {
    mouse_coords_.x = margin_of_error;
  }
}

void AgeOfJojo::mouseDown(ci::app::MouseEvent event) {
  switch (game_mode_) {
    case GameMode::kPlaying_:
      battle_engine_.HandleMouseClick(event);
      break;
    case GameMode::kDioWins_:
    case GameMode::kJojoWins_:
      RestartGame();
  }
}

void AgeOfJojo::RestartGame() {
  game_mode_ = GameMode::kPlaying_;
  battle_engine_.RestartGame();
}

void AgeOfJojo::PlaySound(const std::string& file_path) {
//  if (options::play_sounds) {
//    ci::audio::SourceFileRef source = ci::audio::load(ci::app::loadAsset(file_path));
//    sound_player_ = ci::audio::Voice::create(source);
//    sound_player_->start();
//  }
}

void AgeOfJojo::PlayMusic(const std::string& file_path) {
  ci::audio::SourceFileRef source = ci::audio::load(ci::app::loadAsset(file_path));
  music_player_ = ci::audio::Voice::create(source);
  music_player_->setVolume(0.45f);
  music_player_->start();
}

void AgeOfJojo::PlayRandomSong() {
  std::random_device r;

  std::default_random_engine e1(r());
  std::uniform_int_distribution<int> uniform_dist(0, kSongNames_.size() - 1);
  int random_index = uniform_dist(e1);

  std::string file_path = kSongNames_[random_index];
  PlayMusic(file_path);
}

void AgeOfJojo::PauseGame(int seconds) const {
  ci::Timer timer;
  timer.start();
  while (timer.getSeconds() < seconds) {}
  timer.stop();
}

void AgeOfJojo::UpdateFPSBall() {
  if (moving_ball_coords_.x > styles::kBackgroundLength_) {
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
