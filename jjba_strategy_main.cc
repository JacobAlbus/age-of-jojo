#include "gui/age_of_jojo.h"

//bool options::play_music = true;
//bool options::play_sounds = true;
//std::string global_variables::current_loaded_game = "In Cairo";
//bool global_variables::has_loaded_game_changed = false;

void prepareSettings(age_of_jojo::AgeOfJojo::Settings* settings) {
  settings->setResizable(false);
}

CINDER_APP(age_of_jojo::AgeOfJojo, ci::app::RendererGl, prepareSettings);
