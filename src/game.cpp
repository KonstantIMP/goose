#include "../include/input_manager.hpp"
#include "../include/game.hpp"

#include <SFML/Audio.hpp>

#if defined(linux) || defined(__linux)
    #define ERROR_MACROS(error_message) std::string("./error_msg " + std::string(error_message)).c_str()
    #define CONFIG_MACROS "./options"
#else
    #define ERROR_MACROS(error_message) std::string("error_msg.exe " + std::string(error_message)).c_str()
    #define CONFIG_MACROS "options.exe"
#endif

#define DEVELOP_SCREEN_W 1366
#define DEVELOP_SCREEN_H 768

game::game() {
    json_cfg.set_file_name("config.json");

    if(!json_cfg.is_config()) {
        system(CONFIG_MACROS);

        if(!json_cfg.is_config()) {
            ERROR_MACROS("You cannot play the game without config file");
            exit(7);
        }
    }

    main_win = new sf::RenderWindow(sf::VideoMode::getFullscreenModes()[0], "Gooose", sf::Style::Fullscreen);

    if(json_cfg.get_param("vsync") == "en") main_win->setVerticalSyncEnabled(true);

	sf::Image icon;
    if (!icon.loadFromFile("images/gui/icon.png")) {
        if (!icon.loadFromFile("images/gui/icon_64.png")){
            main_win->close();
            system(ERROR_MACROS("\"Unable to load icon file! Please, reinstall program, to avoid mistakes!\""));

            exit(2);
        }
    }

	main_win->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	w_scale = (double)sf::VideoMode::getFullscreenModes()[0].width / (double)DEVELOP_SCREEN_W;
	h_scale = (double)sf::VideoMode::getFullscreenModes()[0].height / (double)DEVELOP_SCREEN_H;
}

game::~game() {
	delete main_win;
}

void game::run() {
    sf::Music menu_music;

    if(json_cfg.get_param("music") == "en") {
        if (!menu_music.openFromFile("music/deliberate_thought.ogg")) {
            main_win->close();
            system(ERROR_MACROS("\"Unable to load menu music file! Please, reinstall program, to avoid mistakes!\""));

            exit(4);
        }

        menu_music.setLoop(true);
        menu_music.play();
    }

    start_screensaver();

    start_main_menu();
}

void game::start_screensaver() {
    main_timer.restart();

    sf::Texture start_image;
    if (!start_image.loadFromFile("images/gui/start.png")) {
        main_win->close();
        system(ERROR_MACROS("\"Unable to load logo image file! Please, reinstall program, to avoid mistakes!\""));

        exit(3);
    }

    sf::Sprite load_logo(start_image);
    load_logo.scale(sf::Vector2f(w_scale / 3, h_scale / 3));
    load_logo.setPosition(sf::Vector2f(w_scale * DEVELOP_SCREEN_W / 3, h_scale * DEVELOP_SCREEN_H / 3));

    sf::RectangleShape shadow;
    shadow.setSize(sf::Vector2f(DEVELOP_SCREEN_W, DEVELOP_SCREEN_H));
    shadow.setPosition(sf::Vector2f(0, 0));
    shadow.setFillColor(sf::Color(0, 0, 0, 0));

    while (main_timer.getElapsedTime().asSeconds() < 3 && main_win->isOpen()) {
        while(main_win->pollEvent(main_win_event)) {
            if (main_win_event.type == sf::Event::Closed) {
                main_win->close();
                exit(0);
            }
            if(main_win_event.type == sf::Event::KeyPressed) {
                if(main_win_event.key.code == sf::Keyboard::Escape) {
                    main_win->clear();

                    return;
                }
            }
        }

        if(main_timer.getElapsedTime().asMilliseconds() < 400) {
            if (main_timer.getElapsedTime().asMilliseconds() > 50) shadow.setFillColor(sf::Color(0, 0, 0, 225));
            if (main_timer.getElapsedTime().asMilliseconds() > 100) shadow.setFillColor(sf::Color(0, 0, 0, 205));
            if (main_timer.getElapsedTime().asMilliseconds() > 150) shadow.setFillColor(sf::Color(0, 0, 0, 155));
            if (main_timer.getElapsedTime().asMilliseconds() > 200) shadow.setFillColor(sf::Color(0, 0, 0, 105));
            if (main_timer.getElapsedTime().asMilliseconds() > 250) shadow.setFillColor(sf::Color(0, 0, 0, 50));
            if (main_timer.getElapsedTime().asMilliseconds() > 300) shadow.setFillColor(sf::Color(0, 0, 0, 0));
        }

        if(main_timer.getElapsedTime().asSeconds() > 2.3) {
            if(main_timer.getElapsedTime().asSeconds() > 2.4) shadow.setFillColor((sf::Color(0, 0, 0, 50)));
            if(main_timer.getElapsedTime().asSeconds() > 2.5) shadow.setFillColor((sf::Color(0, 0, 0, 105)));
            if(main_timer.getElapsedTime().asSeconds() > 2.63) shadow.setFillColor((sf::Color(0, 0, 0, 155)));
            if(main_timer.getElapsedTime().asSeconds() > 2.7) shadow.setFillColor((sf::Color(0, 0, 0, 205)));
            if(main_timer.getElapsedTime().asSeconds() > 2.8) shadow.setFillColor((sf::Color(0, 0, 0, 225)));
            if(main_timer.getElapsedTime().asSeconds() > 2.9) shadow.setFillColor((sf::Color(0, 0, 0, 255)));
        }

        main_win->clear();
        main_win->draw(load_logo);
        main_win->draw(shadow);
        main_win->display();
    }

    main_timer.restart();

    while (main_timer.getElapsedTime().asSeconds() < 1) {
        main_win->clear();
        main_win->display();
    }
}

void game::start_main_menu() {
    bool is_continue = true;
    bool is_new = false;
    bool is_option = false;
    bool is_exit = false;

    bool is_menu = true;

    sf::Texture background;
    if(!background.loadFromFile("images/gui/menu_background.png")) {
        main_win->close();

        system(ERROR_MACROS("\"Unable to load menu background. Please, reinstall program, to avoid mistakes!\""));
    }

    sf::Texture buttons;
    if(json_cfg.get_param("language") == "ru") {
        if(!buttons.loadFromFile("images/gui/menu_buttons_ru.png")) {
            main_win->close();

            system(ERROR_MACROS("\"Unable to load menu buttons. Please, reinstall program, to avoid mistakes!\""));
        }
    }
    else {
        if(!buttons.loadFromFile("images/gui/menu_buttons_en.png")) {
            main_win->close();

            system(ERROR_MACROS("\"Unable to load menu buttons. Please, reinstall program, to avoid mistakes!\""));
        }
    }

    sf::Sprite back(background);
    back.setScale(sf::Vector2f(w_scale, h_scale));
    back.setPosition(sf::Vector2f(0, 0));

    sf::Sprite button(buttons);
    button.setScale(sf::Vector2f(w_scale, h_scale));

    sf::SoundBuffer btn_sound_bfr;
    if(json_cfg.get_param("sound") == "en") {
        if(!btn_sound_bfr.loadFromFile("music/menu_btn.ogg")) {
            main_win->close();

            system(ERROR_MACROS("\"Unable to load menu buttons sound. Please, reinstall program, to avoid mistakes!\""));
        }
    }

    sf::Sound btn_sound(btn_sound_bfr);

    input_manager menu_mgr;

    menu_mgr.add_keyboard_event(sf::Keyboard::Down, [&](){
        if(is_exit) return;

        if(json_cfg.get_param("sound") == "en") btn_sound.play();

        if(is_continue) {
            is_continue = false;
            is_new = true;
        }
        else if(is_new) {
            is_new = false;
            is_option = true;
        }
        else {
            is_option = false;
            is_exit = true;
        }
    });

    menu_mgr.add_keyboard_event(sf::Keyboard::Up, [&](){
       if(is_continue) return;

       if(json_cfg.get_param("sound") == "en") btn_sound.play();

       if(is_exit) {
           is_exit = false;
           is_option = true;
       }
       else if(is_option) {
           is_option = false;
           is_new = true;
       }
       else {
           is_new = false;
           is_continue = true;
       }
    });

    menu_mgr.add_keyboard_event(sf::Keyboard::Enter, [&](){
       if(is_exit) exit(0);
       if(is_option) {
           main_win->close();

           system(CONFIG_MACROS);

           main_win->create(sf::VideoMode::getFullscreenModes()[0], "Gooose", sf::Style::Fullscreen);
       }
    });

    menu_mgr.add_mouse_event(sf::IntRect(513 * w_scale, 250 * h_scale, 340, 100), (sf::Mouse::Button)mouse_hover, [&](){
       if(is_continue) return;

       if(json_cfg.get_param("sound") == "en") btn_sound.play();

       is_new = is_option = is_exit = false;
       is_continue = true;
    });

    menu_mgr.add_mouse_event(sf::IntRect(513 * w_scale, (250 + 125) * h_scale, 340, 100), (sf::Mouse::Button)mouse_hover, [&](){
       if(is_new) return;

       if(json_cfg.get_param("sound") == "en") btn_sound.play();

       is_continue = is_option = is_exit = false;
       is_new = true;
    });

    menu_mgr.add_mouse_event(sf::IntRect(513 * w_scale, (250 + 250) * h_scale, 340, 100), (sf::Mouse::Button)mouse_hover, [&](){
       if(is_option) return;

       if(json_cfg.get_param("sound") == "en") btn_sound.play();

       is_continue = is_new = is_exit = false;
       is_option = true;
    });

    menu_mgr.add_mouse_event(sf::IntRect(513 * w_scale, (250 + 375) * h_scale, 340, 100), (sf::Mouse::Button)mouse_hover, [&](){
       if(is_exit) return;

       if(json_cfg.get_param("sound") == "en") btn_sound.play();

       is_continue = is_new = is_option = false;
       is_exit = true;
    });

    main_timer.restart();

    while (is_menu == true && main_win->isOpen()) {
        while (main_win->pollEvent(main_win_event)) {
            if(main_win_event.type == sf::Event::Closed) main_win->close();
            if(main_timer.getElapsedTime().asSeconds() > 0.1) {
                menu_mgr.play_event(main_win_event);
                main_timer.restart();
            }
        }

        main_win->clear();
        main_win->draw(back);

        button.setTextureRect(sf::IntRect(340 * is_continue, 0 , 340, 100));
        button.setPosition(513 * w_scale, 250 * h_scale);
        main_win->draw(button);

        button.setTextureRect(sf::IntRect(340 * is_new, 100 , 340, 100));
        button.setPosition(513 * w_scale, (250 + 125) * h_scale);
        main_win->draw(button);

        button.setTextureRect(sf::IntRect(340 * is_option, 200 , 340, 100));
        button.setPosition(513 * w_scale, (250 + 250) * h_scale);
        main_win->draw(button);

        button.setTextureRect(sf::IntRect(340 * is_exit, 300 , 340, 100));
        button.setPosition(513 * w_scale, (250 + 375) * h_scale);
        main_win->draw(button);

        main_win->display();
    }

    menu_mgr.clear_all();
}
