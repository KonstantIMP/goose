#ifndef GAME_HPP
#define GAME_HPP

#pragma once

#include <SFML/Graphics.hpp>

#include "../include/config.hpp"

class game {
public:
	game();
	~game();

	void run();

private:
	sf::RenderWindow * main_win;

	sf::Event main_win_event;

	sf::Clock main_timer;

	double w_scale;
	double h_scale;

    config json_cfg;

    void start_screensaver();

    void start_main_menu();
};

#endif
