#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

#pragma once

#include <SFML/Graphics.hpp>

#include <functional>
#include <map>

//Input manger for sfml events
class input_manager {
public:
    //Standart constructor and destructor
    input_manager();
    ~input_manager();

    //Function to add new keyboard event in manager
    //#const sf::Event & - action to trigger
    //#const std::function<void()> & - a function to be called when triggered
    void add_keyboard_event(const sf::Event &, const std::function<void()> &);

    //Function to add new mouse event in manager
    //#const sf::Mouse::Button & - action to trigger
    //#const std::function<void()> & - a function to be called when triggered
    void add_mouse_event(const sf::Mouse::Button &, const std::function<void()> &);

    //Function, which call functions if event triggered
    void play_event(const sf::Event &);

    //Function to clear the memory and remove dependences
    void clear_all();

    //Function to create keyboard event by key
    //#sf::Keyboard::Key - key to trigger
    sf::Event create_keyboard_event(sf::Keyboard::Key);

private:
    //Keyboard hash
    std::map<sf::Keyboard::Key, std::function<void()> *> * keyboard;

    //Mouse hash
    std::map<sf::Mouse::Button, std::function<void()> *> * mouse;
};

#endif
