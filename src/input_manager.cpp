#include "../include/input_manager.hpp"

//Standart constructor
input_manager::input_manager() {
    //Memory allocation for keyboard hash
    keyboard = new std::map<sf::Keyboard::Key, std::function<void()> *>;

    //Memory allocation for mouse hash
    mouse = new std::map<sf::Mouse::Button, std::function<void()> *>;
}

//Standart destructor
input_manager::~input_manager() {
    //Clearing memory
    clear_all();

    delete keyboard;
    delete mouse;
}

void input_manager::add_keyboard_event(const sf::Event & event, const std::function<void ()> & func) {
    //Checking for pressing a button on the keyboard
    if(event.type == sf::Event::KeyPressed) {
        //Checking for duplicates
        if(keyboard->find(event.key.code) == keyboard->end()){
            //Adding new dependence
            keyboard->insert(std::make_pair(event.key.code, new std::function<void()>(func)));
        }
        else {
            //Clearing memory
            delete keyboard->find(event.key.code)->second;
            keyboard->erase(keyboard->find(event.key.code));

            //Adding new dependence
            keyboard->insert(std::make_pair(event.key.code, new std::function<void()>(func)));
        }
    }
}

void input_manager::add_mouse_event(const sf::Mouse::Button & mouse_event, const std::function<void ()> & func) {
    if(mouse->find(mouse_event) == mouse->end()) {
        mouse->insert(std::make_pair(mouse_event, new std::function<void ()>(func)));
    }
    else {
        delete mouse->find(mouse_event)->second;
        mouse->erase(mouse->find(mouse_event));
    }
    mouse->insert(std::make_pair(mouse_event, new std::function<void ()>(func)));
}

void input_manager::play_event(const sf::Event & event) {
    if(event.type == sf::Event::KeyPressed) {
        if(keyboard->find(event.key.code) == keyboard->end()) return;
        auto iter = keyboard->find(event.key.code);
        (*iter->second)();
    }
    else {
        sf::Mouse cursor;

        auto iterator = mouse->begin();

        while(iterator != mouse->end()) {
            if (cursor.isButtonPressed(iterator->first)) {
                (*iterator->second)();
                return;
            }
            iterator++;
        }
    }
}

void input_manager::clear_all() {
    auto m_iter = mouse->begin();

    while (m_iter != mouse->end()) {
        delete m_iter->second;
        mouse->erase(m_iter);
        m_iter++;
    }

    auto k_iter = keyboard->begin();

    while (k_iter != keyboard->end()) {
        delete k_iter->second;
        keyboard->erase(k_iter);
        k_iter++;
    }
}

sf::Event input_manager::create_keyboard_event(sf::Keyboard::Key button) {
    sf::Event btn_pressed;
    btn_pressed.type = sf::Event::KeyPressed;
    btn_pressed.key.code = button;

    return btn_pressed;
}
