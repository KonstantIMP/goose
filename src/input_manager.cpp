#include "../include/input_manager.hpp"

#include <thread>
#include <mutex>

input_manager::input_manager() {
    keyboard = new std::map<sf::Keyboard::Key, std::function<void()> *>;
    mouse = new std::map<std::string, mouse_event>;
}

input_manager::~input_manager() {
    clear_all();

    delete keyboard;
    delete mouse;
}

void input_manager::add_keyboard_event(const sf::Keyboard::Key & key, const std::function<void ()> & func) {
    if(keyboard->find(key) != keyboard->end()) {
        delete keyboard->find(key)->second;
        keyboard->erase(keyboard->find(key));
    }

    keyboard->insert(std::make_pair(key, new std::function<void ()>(func)));
}

void input_manager::add_mouse_event(const sf::IntRect & area, const sf::Mouse::Button & key, const std::function<void ()> & func) {
    std::string tmp = std::to_string(area.left) + ":" + std::to_string(area.top) + ":" + std::to_string(area.width) + ":" + std::to_string(area.height) + "." + std::to_string(key);

    if(mouse->find(tmp) != mouse->end()) {
        delete mouse->find(tmp)->second.event_function;
        mouse->erase(mouse->find(tmp));
    }

    mouse->insert(std::make_pair(tmp, mouse_event{area, key, new std::function<void ()>(func)}));
}

void input_manager::play_event(const sf::Event & event) {
    std::mutex func_mtx;

    std::thread mouse_thread([&](){
        sf::Mouse mouse_for_event;

        auto m_iter = mouse->begin();

        while (m_iter != mouse->end()) {
            if(m_iter->second.event_rect.contains(mouse_for_event.getPosition())) {
                if(mouse_for_event.isButtonPressed(m_iter->second.event_btn) || m_iter->second.event_btn == mouse_hover) {
                    func_mtx.lock();

                    (*m_iter->second.event_function)();

                    func_mtx.unlock();
                }
            }

            m_iter++;
        }
    });

    auto k_iter = keyboard->begin();

    while (k_iter != keyboard->end()) {
        if(event.key.code == k_iter->first) {
            func_mtx.lock();

            (*k_iter->second)();

            func_mtx.unlock();
        }

        k_iter++;
    }

    mouse_thread.join();
}

sf::IntRect input_manager::get_fullscreen_rect() const {
    sf::IntRect tmp;

    tmp.top = 0;
    tmp.left = 0;

    tmp.width = sf::VideoMode::getFullscreenModes()[0].width;
    tmp.height = sf::VideoMode::getFullscreenModes()[0].height;

    return tmp;
}

void input_manager::clear_all() {
    auto k_iter = keyboard->begin();

    while (k_iter != keyboard->end()) {
        delete k_iter->second;
        k_iter++;
    }

    keyboard->clear();

    auto m_iter = mouse->begin();

    while (m_iter != mouse->end()) {
        delete m_iter->second.event_function;
        m_iter++;
    }

    mouse->clear();
}
