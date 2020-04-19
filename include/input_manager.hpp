#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

#pragma once

#include <SFML/Graphics.hpp>

#include <functional>
#include <string>
#include <map>

unsigned int mouse_hover = (sf::Mouse::Button)10;

//Input manger for sfml events
class input_manager {
public:
    //Standart constructor and destructor
    input_manager();
    ~input_manager();

    //Function to add new keyboard event in manager
    //#const sf::Event & - action to trigger
    //#const std::function<void()> & - a function to be called when triggered
    void add_keyboard_event(const sf::Keyboard::Key &, const std::function<void()> &);

    //Function to add new mouse event in manager
    //#const sf::Mouse::Button & - action to trigger
    //#const std::function<void()> & - a function to be called when triggered
    void add_mouse_event(const sf::IntRect &,const sf::Mouse::Button &, const std::function<void()> &);

    //Function, which call functions if event triggered
    void play_event(const sf::Event &);

    //Function, which return maximum rect area
    sf::IntRect get_fullscreen_rect() const;

    //Function to clear the memory and remove dependences
    void clear_all();

private:
    //Keyboard hash
    std::map<sf::Keyboard::Key, std::function<void()> *> * keyboard;

    //Mouse event structure
    struct mouse_event {
        sf::IntRect event_rect;
        sf::Mouse::Button event_btn;

        std::function<void ()> * event_function;
    };

    //Mouse hash
    std::map<std::string, mouse_event> * mouse;
};

#endif
