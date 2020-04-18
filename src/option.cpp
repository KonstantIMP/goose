#include "../include/config.hpp"

#include <gtk/gtk.h>
#include <cstring>
#include <locale>

#define CONFIG_FILE "config.json"

#if defined(linux) || defined(__linux)
    #define GLADE_API
    #define ERROR_MACROS(error_message) std::string("./error_msg " + std::string(error_message)).c_str()
#else
    #define GLADE_API __declspec(dllexport)
    #define ERROR_MACROS(error_message) std::string("error_msg.exe " + std::string(error_message)).c_str()
#endif

// ----------------------------------------------------

config cfg_json(CONFIG_FILE);

GtkWidget * main_win;

// ----------------------------------------------------

static GtkWidget * create_main_window();

extern "C" {
    GLADE_API void cancel() {
        if(!cfg_json.is_config()) {
            system(ERROR_MACROS("\"You cannot play the game without option file!\""));
        }

        gtk_main_quit();
    }

    GLADE_API void ok() {
        cfg_json.write();

        gtk_main_quit();
    }

    GLADE_API void change_check(GtkWidget * check, gpointer data) {
        if(std::string(gtk_label_get_text(GTK_LABEL(data))) == "VSync") {
            if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check))) cfg_json.add_param("vsync", "en");
            else cfg_json.add_param("vsync", "dis");
        }

        if(std::string(gtk_label_get_text(GTK_LABEL(data))) == "Music" || std::string(gtk_label_get_text(GTK_LABEL(data))) == "\xd0\x9c\xd1\x83\xd0\xb7\xd1\x8b\xd0\xba\xd0\xb0") {
            if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check))) cfg_json.add_param("music", "en");
            else cfg_json.add_param("music", "dis");
        }

        if(std::string(gtk_label_get_text(GTK_LABEL(data))) == "Sound effects" || std::string(gtk_label_get_text(GTK_LABEL(data))) == "\xd0\x97\xd0\xb2\xd1\x83\xd0\xba\xd0\xbe\xd0\xb2\xd1\x8b\xd0\xb5\x20\xd1\x8d\xd1\x84\xd1\x84\xd0\xb5\xd0\xba\xd1\x82\xd1\x8b") {
            if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check))) cfg_json.add_param("sound", "en");
            else cfg_json.add_param("sound", "dis");
        }
    }

    GLADE_API void lang_change(GtkWidget * combo) {
        if(gtk_combo_box_get_active(GTK_COMBO_BOX(combo)) == 0) cfg_json.add_param("language", "en");
        else cfg_json.add_param("language", "ru");

        cfg_json.write();

        gtk_main_quit();

        g_object_unref(main_win);

        gtk_widget_destroy(main_win);

        main_win = create_main_window();

        gtk_widget_show_all(main_win);

        gtk_main();
    }
}

int main(int argc, char* argv[]) {
    gtk_init(&argc, &argv);

    main_win = create_main_window();

    gtk_widget_show_all(main_win);

    gtk_main();

    return 0;
}

static GtkWidget * create_main_window() {
    GtkWidget * window;
    GtkBuilder * window_builder;

    GError * err_msg = NULL;

    window_builder = gtk_builder_new();

    if(!gtk_builder_add_from_file(window_builder, "res/option.glade", &err_msg)) {
        system(ERROR_MACROS("\"Unable to load window form. Please, reinstall this programm to avoid mistakes!\""));

        g_error_free(err_msg);

        exit(1);
    }

    window = GTK_WIDGET(gtk_builder_get_object(window_builder, "win"));

    if(!window) {
        system(ERROR_MACROS("\"Unable to load option window. Please, reinstall this programm to avoid mistakes!\""));

        exit(2);
    }

    if(!gtk_window_set_icon_from_file(GTK_WINDOW(window), "images/option.png", &err_msg)) {
        system(ERROR_MACROS("\"Unable to load window icon. Please, reinstall this programm to avoid mistakes!\""));

        g_error_free(err_msg);

        exit(2);
    }

    gtk_window_set_title(GTK_WINDOW(window), "Options");

    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(gtk_builder_get_object(window_builder, "lang_combo")), "0", "English");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(gtk_builder_get_object(window_builder, "lang_combo")), "1", "\xd0\xa0\xd1\x83\xd1\x81\xd1\x81\xd0\xba\xd0\xb8\xd0\xb9");

    if(cfg_json.is_config()) {
        if(cfg_json.get_param("language") == "ru") {
            gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(window_builder, "lang_msg")), "\xd0\xaf\xd0\xb7\xd1\x8b\xd0\xba");
            gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(window_builder, "vsync_msg")), "VSync");
            gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(window_builder, "music_msg")), "\xd0\x9c\xd1\x83\xd0\xb7\xd1\x8b\xd0\xba\xd0\xb0");
            gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(window_builder, "sound_msg")), "\xd0\x97\xd0\xb2\xd1\x83\xd0\xba\xd0\xbe\xd0\xb2\xd1\x8b\xd0\xb5\x20\xd1\x8d\xd1\x84\xd1\x84\xd0\xb5\xd0\xba\xd1\x82\xd1\x8b");

            gtk_button_set_label(GTK_BUTTON(gtk_builder_get_object(window_builder, "cancel_btn")), "\xd0\x9e\xd1\x82\xd0\xbc\xd0\xb5\xd0\xbd\xd0\xb0");
            gtk_button_set_label(GTK_BUTTON(gtk_builder_get_object(window_builder, "ok_btn")), "OK");

            gtk_combo_box_set_active(GTK_COMBO_BOX(gtk_builder_get_object(window_builder, "lang_combo")), 1);
        }
        else {
            gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(window_builder, "lang_msg")), "Language");
            gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(window_builder, "vsync_msg")), "VSync");
            gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(window_builder, "music_msg")), "Music");
            gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(window_builder, "sound_msg")), "Sound effects");

            gtk_button_set_label(GTK_BUTTON(gtk_builder_get_object(window_builder, "cancel_btn")), "Cancel");
            gtk_button_set_label(GTK_BUTTON(gtk_builder_get_object(window_builder, "ok_btn")), "OK");

            gtk_combo_box_set_active(GTK_COMBO_BOX(gtk_builder_get_object(window_builder, "lang_combo")), 0);
        }

        if(cfg_json.get_param("vsync") == "en") gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(window_builder, "vsync_check")), true);

        if(cfg_json.get_param("music") == "en") gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(window_builder, "music_check")), true);

        if(cfg_json.get_param("sound") == "en") gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(window_builder, "sound_check")), true);
    }
    else {
        std::string lang = std::locale("").name();

        if(lang == "ru_RU.UTF-8") {
            gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(window_builder, "lang_msg")), "\xd0\xaf\xd0\xb7\xd1\x8b\xd0\xba");
            gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(window_builder, "vsync_msg")), "VSync");
            gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(window_builder, "music_msg")), "\xd0\x9c\xd1\x83\xd0\xb7\xd1\x8b\xd0\xba\xd0\xb0");
            gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(window_builder, "sound_msg")), "\xd0\x97\xd0\xb2\xd1\x83\xd0\xba\xd0\xbe\xd0\xb2\xd1\x8b\xd0\xb5\x20\xd1\x8d\xd1\x84\xd1\x84\xd0\xb5\xd0\xba\xd1\x82\xd1\x8b");

            gtk_button_set_label(GTK_BUTTON(gtk_builder_get_object(window_builder, "cancel_btn")), "\xd0\x9e\xd1\x82\xd0\xbc\xd0\xb5\xd0\xbd\xd0\xb0");
            gtk_button_set_label(GTK_BUTTON(gtk_builder_get_object(window_builder, "ok_btn")), "OK");

            gtk_combo_box_set_active(GTK_COMBO_BOX(gtk_builder_get_object(window_builder, "lang_combo")), 1);

            cfg_json.add_param("language", "ru");
        }
        else {
            gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(window_builder, "lang_msg")), "Language");
            gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(window_builder, "vsync_msg")), "VSync");
            gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(window_builder, "music_msg")), "Music");
            gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(window_builder, "sound_msg")), "Sound affects");

            gtk_button_set_label(GTK_BUTTON(gtk_builder_get_object(window_builder, "cancel_btn")), "Cancel");
            gtk_button_set_label(GTK_BUTTON(gtk_builder_get_object(window_builder, "ok_btn")), "OK");

            gtk_combo_box_set_active(GTK_COMBO_BOX(gtk_builder_get_object(window_builder, "lang_combo")), 0);

            cfg_json.add_param("language", "en");
        }

        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(window_builder, "vsync_check")), true);
        cfg_json.add_param("vsync", "en");

        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(window_builder, "music_check")), true);
        cfg_json.add_param("music", "en");

        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(window_builder, "sound_check")), true);
        cfg_json.add_param("sound", "en");
    }

    gtk_builder_connect_signals(window_builder, NULL);

    return window;
}
