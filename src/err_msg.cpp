#include <gtk/gtk.h>

#include <iostream>

int main(int argc, char* argv[]) {
	if (argc == 1) {
		std::cout << "There are no arguments to show!\n";
		std::cout << "Please, use this syntax : wrng_msg [ Your message ]\n";

		return 1;
	}

	if (argc > 2) {
		std::cout << "There are too many arguments!\n";
		std::cout << "Please, use this syntax : wrng_msg \" Your message \"\n";

		return 2;
	}

    gtk_init(&argc, &argv);

    GtkWidget* main_win;

    GtkWidget* content_area;

    GtkWidget* err_msg;

    main_win = gtk_dialog_new_with_buttons("Error", GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL)), GTK_DIALOG_MODAL, ("_OK"), GTK_RESPONSE_ACCEPT, NULL);

    gtk_window_set_icon_from_file(GTK_WINDOW(main_win), "images/err.png", NULL);
    gtk_window_set_modal(GTK_WINDOW(main_win), true);
    gtk_window_set_focus(GTK_WINDOW(main_win), NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(main_win));

    err_msg = gtk_label_new(argv[1]);
    gtk_widget_set_size_request(GTK_WIDGET(err_msg), 300, 100);

    gtk_container_add(GTK_CONTAINER(content_area), err_msg);

    gtk_container_set_border_width(GTK_CONTAINER(main_win), 7);

    gtk_widget_show_all(main_win);

    gtk_dialog_run(GTK_DIALOG(main_win));

    gtk_widget_destroy(main_win);

	return 0;
}
