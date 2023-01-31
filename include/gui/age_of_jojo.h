#pragma once
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/audio/Voice.h"
#include "engine/battle_engine.h"
#include <nlohmann/json.hpp>
#include <unordered_map>

namespace age_of_jojo {

class AgeOfJojo : public ci::app::App {
 public:
  /**
   * Instantiates App
   */
  AgeOfJojo();

  /**
   * Draws the state of the board
   */
  void draw() override;

  /**
   * Updates the state of the board
   */
  void update() override;

  /**
   * Calls methods depending on where mouse is
   * @param event data on mouse placement
   */
  void mouseMove(ci::app::MouseEvent event) override;

  /**
   * Handles when mouse is clicked
   * @param event data on mouse placement
   */
  void mouseDown(ci::app::MouseEvent event) override;

  void keyDown(ci::app::KeyEvent event) override;


 private:
  glm::vec2 top_right_corner_;
  glm::vec2 moving_ball_coords_;
  bool is_moving_right_;
  ci::gl::TextureRef background_;
  glm::vec2 mouse_coords_;
  BattleEngine battle_engine_;
  GameMode game_mode_;

  /**
 * Plays specified song
 * @param file_path path to song file
 */
  void PlaySound(const std::string& file_path);

  void PauseGame(int seconds) const;

  void RestartGame();

  void DisplayWinScreen();

  void UpdateGame();

  void DrawGame();

  void DebugScreen() const;

  void UpdateFPSBall();

  /**
   * Plays specified song
   * @param file_path path to song file
   */
  void PlayMusic(const std::string& file_path);

  /**
   * Plays random song
   */
  void PlayRandomSong();


  /**
   * @music_player_ : Cinder objects able to play audio files
   * @sound_player_ : Cinder objects able to play audio files
   * @kSongNames_ : contains all the song names to be played
   */
  ci::audio::VoiceRef music_player_;
  ci::audio::VoiceRef sound_player_;
  std::vector<std::string> kSongNames_;
};

}

