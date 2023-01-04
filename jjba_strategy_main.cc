#include "gui/jjba_strategy_app.h"

using jjba_strategy::JJBAStrategyApp;

//bool options::play_music = true;
//bool options::play_sounds = true;
//std::string global_variables::current_loaded_game = "In Cairo";
//bool global_variables::has_loaded_game_changed = false;

void prepareSettings(JJBAStrategyApp::Settings* settings) {
  settings->setResizable(false);
}

CINDER_APP(JJBAStrategyApp, ci::app::RendererGl, prepareSettings);
