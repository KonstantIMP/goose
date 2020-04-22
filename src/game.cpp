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

    while(1) {
        if(start_main_menu()) {
            start_option();

            if(json_cfg.get_param("vsync") == "en") main_win->setVerticalSyncEnabled(true);
            else main_win->setVerticalSyncEnabled(false);

            if(json_cfg.get_param("music") == "en") {
                if (!menu_music.openFromFile("music/deliberate_thought.ogg")) {
                    main_win->close();
                    system(ERROR_MACROS("\"Unable to load menu music file! Please, reinstall program, to avoid mistakes!\""));

                    exit(4);
                }

                menu_music.setLoop(true);
                menu_music.play();
            }
            else {
                if(menu_music.getStatus() == sf::Music::Playing) menu_music.stop();
            }
        }
        else break;
    }
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

bool game::start_main_menu() {
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
    btn_sound.setVolume(50);

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
           is_option = true;
           is_continue = is_new = is_exit = false;
           is_menu = false;
       }
    });

    menu_mgr.add_mouse_event(sf::IntRect(513 * w_scale, 250 * h_scale, 340 * w_scale, 100 * h_scale), (sf::Mouse::Button)mouse_hover, [&](){
       if(is_continue) return;

       if(json_cfg.get_param("sound") == "en") btn_sound.play();

       is_new = is_option = is_exit = false;
       is_continue = true;
    });

    menu_mgr.add_mouse_event(sf::IntRect(513 * w_scale, (250 + 125) * h_scale, 340 * w_scale, 100 * h_scale), (sf::Mouse::Button)mouse_hover, [&](){
       if(is_new) return;

       if(json_cfg.get_param("sound") == "en") btn_sound.play();

       is_continue = is_option = is_exit = false;
       is_new = true;
    });

    menu_mgr.add_mouse_event(sf::IntRect(513 * w_scale, (250 + 250) * h_scale, 340 * w_scale, 100 * h_scale), (sf::Mouse::Button)mouse_hover, [&](){
       if(is_option) return;

       if(json_cfg.get_param("sound") == "en") btn_sound.play();

       is_continue = is_new = is_exit = false;
       is_option = true;
    });

    menu_mgr.add_mouse_event(sf::IntRect(513 * w_scale, (250 + 375) * h_scale, 340 * w_scale, 100 * h_scale), (sf::Mouse::Button)mouse_hover, [&](){
       if(is_exit) return;

       if(json_cfg.get_param("sound") == "en") btn_sound.play();

       is_continue = is_new = is_option = false;
       is_exit = true;
    });

    menu_mgr.add_mouse_event(sf::IntRect(513 * w_scale, (250 + 250) * h_scale, 340 * w_scale, 100 * h_scale), sf::Mouse::Button::Left, [&](){
        is_option = true;
        is_continue = is_new = is_exit = false;
        is_menu = false;
    });

    menu_mgr.add_mouse_event(sf::IntRect(513 * w_scale, (250 + 375) * h_scale, 340 * w_scale, 100 * h_scale), sf::Mouse::Button::Left, [&](){
        main_win->close();
        exit(0);
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

    return is_option;
}

void game::start_option() {
    bool is_option = true;

    bool is_en;
    if(json_cfg.get_param("language") == "ru") is_en = false;
    else is_en = true;

    char menu_num = 0;

    bool is_sound = false;
    if(json_cfg.get_param("sound") == "en") is_sound = true;

    bool is_music = false;
    if(json_cfg.get_param("music") == "en") is_music = true;

    bool is_vsyck = false;
    if(json_cfg.get_param("vsync") == "en") is_vsyck = true;

    sf::Texture background;
    if(!background.loadFromFile("images/gui/option_background.png")) {
        main_win->close();

        system(ERROR_MACROS("\"Unable to load option-menu background. Please, reinstall program, to avoid mistakes!\""));
    }

    sf::Sprite background_option(background);
    background_option.setPosition(sf::Vector2f(0, 0));
    background_option.setScale(w_scale, h_scale);

    sf::Texture buttons;
    if(!buttons.loadFromFile("images/gui/option_buttons.png")) {
        main_win->close();

        system(ERROR_MACROS("\"Unable to load option-menu buttons. Please, reinstall program, to avoid mistakes!\""));
    }

    sf::Sprite btn(buttons);
    btn.setScale(w_scale, h_scale);

    sf::Font sans_serif;
    if(!sans_serif.loadFromFile("res/sans_serif.ttf")) {
        main_win->close();

        system(ERROR_MACROS("\"Unable to load menu font. Please, reinstall program, to avoid mistakes!\""));
    }

    sf::Text menu_entries;
    menu_entries.setFont(sans_serif);
    menu_entries.setScale(w_scale, h_scale);
    menu_entries.setFillColor(sf::Color(200, 94, 108));

    sf::SoundBuffer btn_sound_bfr;
    if(json_cfg.get_param("sound") == "en") {
        if(!btn_sound_bfr.loadFromFile("music/menu_btn.ogg")) {
            main_win->close();

            system(ERROR_MACROS("\"Unable to load menu buttons sound. Please, reinstall program, to avoid mistakes!\""));
        }
    }

    sf::Sound btn_sound(btn_sound_bfr);
    btn_sound.setVolume(50);

    input_manager option_mgr;

    option_mgr.add_keyboard_event(sf::Keyboard::Down, [&](){
        if(menu_num < 5) {
            if(json_cfg.get_param("sound") == "en") btn_sound.play();
            menu_num++;
        }
    });

    option_mgr.add_keyboard_event(sf::Keyboard::Up, [&](){
        if(menu_num > 0) {
            if(json_cfg.get_param("sound") == "en") btn_sound.play();
            menu_num--;
        }
    });

    option_mgr.add_keyboard_event(sf::Keyboard::Escape, [&](){
        if(json_cfg.get_param("sound") == "en") btn_sound.play();
        is_option = false;
    });

    option_mgr.add_keyboard_event(sf::Keyboard::Enter, [&](){
        if(json_cfg.get_param("sound") == "en") btn_sound.play();
        if(menu_num == 0) is_en = !is_en;
        if(menu_num == 1) is_sound = !is_sound;
        if(menu_num == 2) is_music = !is_music;
        if(menu_num == 3) is_vsyck = !is_vsyck;
        if(menu_num == 4) is_option = false;
        if(menu_num == 5) {
            is_option = false;

            json_cfg.clear_all();
            json_cfg.add_param("encoding", "UTF-8");

            if(is_en) json_cfg.add_param("language", "en");
            else json_cfg.add_param("language", "ru");

            if(is_sound) json_cfg.add_param("sound", "en");
            else json_cfg.add_param("sound", "dis");

            if(is_music) json_cfg.add_param("music", "en");
            else json_cfg.add_param("music", "dis");

            if(is_vsyck) json_cfg.add_param("vsync", "en");
            else json_cfg.add_param("vsync", "dis");

            json_cfg.write();
        }
    });

    option_mgr.add_mouse_event(sf::IntRect(865 * w_scale, 485 * h_scale, 99 * w_scale, 99 * h_scale), (sf::Mouse::Button)mouse_hover, [&](){
        if(menu_num == 4) return;
        if(json_cfg.get_param("sound") == "en") btn_sound.play();
        menu_num = 4;
    });

    option_mgr.add_mouse_event(sf::IntRect(865 * w_scale, 610 * h_scale, 99 * w_scale, 99 * h_scale), (sf::Mouse::Button)mouse_hover, [&](){
        if(menu_num == 5) return;
        if(json_cfg.get_param("sound") == "en") btn_sound.play();
        menu_num = 5;
    });

    option_mgr.add_mouse_event(sf::IntRect(865 * w_scale, 485 * h_scale, 99 * w_scale, 99 * h_scale), sf::Mouse::Button::Left, [&](){
        if(json_cfg.get_param("sound") == "en") btn_sound.play();
        is_option = false;
    });

    option_mgr.add_mouse_event(sf::IntRect(865 * w_scale, 610 * h_scale, 99 * w_scale, 99 * h_scale), sf::Mouse::Button::Left, [&](){
        if(json_cfg.get_param("sound") == "en") btn_sound.play();
        is_option = false;

        json_cfg.clear_all();
        json_cfg.add_param("encoding", "UTF-8");

        if(is_en) json_cfg.add_param("language", "en");
        else json_cfg.add_param("language", "ru");

        if(is_sound) json_cfg.add_param("sound", "en");
        else json_cfg.add_param("sound", "dis");

        if(is_music) json_cfg.add_param("music", "en");
        else json_cfg.add_param("music", "dis");

        if(is_vsyck) json_cfg.add_param("vsync", "en");
        else json_cfg.add_param("vsync", "dis");

        json_cfg.write();
    });

    option_mgr.add_mouse_event(sf::IntRect(862 * w_scale, 235 * h_scale, 99 * w_scale, 99 * h_scale), sf::Mouse::Button::Left, [&](){
        if(json_cfg.get_param("sound") == "en") btn_sound.play();
        is_en = !is_en;
        menu_num = 0;
    });

    option_mgr.add_mouse_event(sf::IntRect(865 * w_scale, 360 * h_scale, 99 * w_scale, 99 * h_scale), sf::Mouse::Button::Left, [&](){
        if(json_cfg.get_param("sound") == "en") btn_sound.play();
        is_sound = !is_sound;
        menu_num = 1;
    });

    option_mgr.add_mouse_event(sf::IntRect(742 * w_scale, 485 * h_scale, 99 * w_scale, 99 * h_scale), sf::Mouse::Button::Left, [&](){
        if(json_cfg.get_param("sound") == "en") btn_sound.play();
        is_music = !is_music;
        menu_num = 2;
    });

    option_mgr.add_mouse_event(sf::IntRect(742 * w_scale, 610 * h_scale, 99 * w_scale, 99 * h_scale), sf::Mouse::Button::Left, [&](){
        if(json_cfg.get_param("sound") == "en") btn_sound.play();
        is_vsyck = !is_vsyck;
        menu_num = 3;
    });

    main_timer.restart();

    while (is_option == true && main_win->isOpen()) {
        while (main_win->pollEvent(main_win_event)) {
            if(main_win_event.type == sf::Event::Closed) main_win->close();

            if(main_timer.getElapsedTime().asSeconds() > 0.1) {
                option_mgr.play_event(main_win_event);
                main_timer.restart();
            }
        }

        main_win->clear();

        main_win->draw(background_option);

        if(!is_en) menu_entries.setString(L"Настройки");
        else menu_entries.setString("Options");
        menu_entries.setCharacterSize(72);
        menu_entries.setPosition(sf::Vector2f((550 - 50 * (!is_en)) * w_scale, 70 * h_scale));
        main_win->draw(menu_entries);

        if(!is_en) menu_entries.setString(L"Язык");
        else menu_entries.setString("Language");
        if(menu_num == 0) menu_entries.setString("@  " + menu_entries.getString());
        menu_entries.setCharacterSize(42);
        menu_entries.setPosition(sf::Vector2f(450 * w_scale, 255 * h_scale));
        main_win->draw(menu_entries);

        btn.setTextureRect(sf::IntRect(99 * is_en, 0, 99, 99));
        btn.setPosition(sf::Vector2f(862 * w_scale, 235 * h_scale));
        main_win->draw(btn);

        if(!is_en) menu_entries.setString(L"Звуковые эф.");
        else menu_entries.setString("Sound effects");
        if(menu_num == 1) menu_entries.setString("@  " + menu_entries.getString());
        menu_entries.setCharacterSize(42);
        menu_entries.setPosition(sf::Vector2f(450 * w_scale, 380 * h_scale));
        main_win->draw(menu_entries);

        btn.setTextureRect(sf::IntRect(99 * is_sound, 99, 99, 99));
        btn.setPosition(sf::Vector2f(865 * w_scale, 360 * h_scale));
        main_win->draw(btn);

        if(!is_en) menu_entries.setString(L"Музыка");
        else menu_entries.setString("Music");
        if(menu_num == 2) menu_entries.setString("@  " + menu_entries.getString());
        menu_entries.setCharacterSize(42);
        menu_entries.setPosition(sf::Vector2f(450 * w_scale, 505 * h_scale));
        main_win->draw(menu_entries);

        btn.setTextureRect(sf::IntRect(99 * is_music, 99, 99, 99));
        btn.setPosition(sf::Vector2f(742 * w_scale, 485 * h_scale));
        main_win->draw(btn);

        menu_entries.setString("VSync");
        if(menu_num == 3) menu_entries.setString("@  " + menu_entries.getString());
        menu_entries.setCharacterSize(42);
        menu_entries.setPosition(sf::Vector2f(450 * w_scale, 630 * h_scale));
        main_win->draw(menu_entries);

        btn.setTextureRect(sf::IntRect(99 * is_vsyck, 99, 99, 99));
        btn.setPosition(sf::Vector2f(742 * w_scale, 610 * h_scale));
        main_win->draw(btn);

        if(menu_num == 4) btn.setTextureRect(sf::IntRect(0, 198, 99, 99));
        else btn.setTextureRect(sf::IntRect(0, 99, 99, 99));
        btn.setPosition(sf::Vector2f(865 * w_scale, 485 * h_scale));
        main_win->draw(btn);

        if(menu_num == 5) btn.setTextureRect(sf::IntRect(99, 198, 99, 99));
        else btn.setTextureRect(sf::IntRect(0, 99, 99, 99));
        btn.setPosition(sf::Vector2f(865 * w_scale, 610 * h_scale));
        main_win->draw(btn);

        main_win->display();
    }
}
