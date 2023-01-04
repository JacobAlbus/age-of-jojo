#pragma once
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/audio/Voice.h"
//#include "gui/renderer.h"
#include <unordered_map>

namespace jjba_strategy {

/**
 * Allows a user to visualize particle collisions in an ideal system
 */
class JJBAStrategyApp : public ci::app::App {
 public:
  /**
   * Instantiates App
   */
  JJBAStrategyApp();

  /**
   * Draws the state of the board
   */
  void draw() override;

  /**
   * Updates the state of the board
   */
  void update() override;

  /**
   * Calls methods depending on which key is pressed
   * @param event object containing pressed key
   */
  void mouseMove(ci::app::MouseEvent event) override;

//  const float kWindowSize_ = style::kWindowSize;
  const float kWindowSize_ = 900;

//  const std::string kBoardsFolderPath_ = global_consts::kBoardsFolderPath;

 private:
  glm::vec2 top_right_corner_;
  glm::vec2 moving_ball_coords_;
  bool is_moving_right_;
  ci::gl::TextureRef background_;
  glm::vec2 mouse_coords_;

  /**
 * Plays specified song
 * @param file_path path to song file
 */
  void PlaySound(const std::string& file_path);

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
   * @kWindowSize_ : represents height and width of cinder app window
   * @render_engine_ : contains the Renderer objects with their corresponding GameState enum's as keys
   * @current_render_engine_ : renderer object currently being used
   * @current_game_state_ : GameState enum indicating which renderer object to use
   * @previous_loaded_game_ : used to track when loaded game name changes
   */
//  std::unordered_map<GameState, Renderer*> render_engines_;
//  Renderer* current_render_engine_;
//  GameState current_game_state_;
  std::string previous_loaded_game_;

  /**
   * @music_player_ : Cinder objects able to play audio files
   * @sound_player_ : Cinder objects able to play audio files
   * @kSongNames_ : contains all the song names to be played
   */
  ci::audio::VoiceRef music_player_;
  ci::audio::VoiceRef sound_player_;
  std::vector<std::string> kSongNames_;
};

}  // namespace jjba_strategy

