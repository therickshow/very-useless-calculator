#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/*
 * Same conversion logic as very-useless-calculator.c, but instead of
 * scanf/printf blocking in a console, everything happens inside a
 * "callback" function that GTK calls for us whenever the button is
 * clicked. That's the core shift from procedural console programs to
 * event-driven GUI programs: your code stops driving the flow and
 * instead reacts to things the user does.
 */

/* A little struct so the callback can reach the widgets it needs to
 * read from / write to. GTK callbacks only get one "user_data" pointer,
 * so bundling widgets into a struct like this is the normal pattern. */
typedef struct {
    GtkWidget *entry;
    GtkWidget *result_label;
} AppWidgets;

static void on_convert_clicked(GtkButton *button, gpointer user_data) {
    /* GTK chooses this signature, so we get a 'button' whether we want it
     * or not -- and we don't, since there is only one button. Casting it to
     * void tells the compiler the omission is deliberate, which stops the
     * -Wunused-parameter warning without weakening it everywhere else. */
    (void)button;

    AppWidgets *widgets = (AppWidgets *)user_data;

    const char *text = gtk_entry_get_text(GTK_ENTRY(widgets->entry));

    /* strtoll instead of scanf: it can tell us whether the text the
     * user typed was actually a valid whole number, which matters a
     * lot more here than it did with scanf in a console app, since a
     * GUI can't just let a bad input crash or hang the program. */
    errno = 0;
    char *end = NULL;
    long long microseconds = strtoll(text, &end, 10);

    if (end == text || *end != '\0' || errno == ERANGE) {
        gtk_label_set_text(GTK_LABEL(widgets->result_label),
                            "Please enter a whole number of microseconds.");
        return;
    }

    const long long microseconds_per_month  = 30LL * 24 * 60 * 60 * 1000000;
    const long long microseconds_per_week   = 7LL  * 24 * 60 * 60 * 1000000;
    const long long microseconds_per_day    = 24LL * 60 * 60 * 1000000;
    const long long microseconds_per_hour   = 60LL * 60 * 1000000;
    const long long microseconds_per_minute = 60LL * 1000000;
    const long long microseconds_per_second = 1000000;

    long long months = microseconds / microseconds_per_month;
    microseconds %= microseconds_per_month;

    long long weeks = microseconds / microseconds_per_week;
    microseconds %= microseconds_per_week;

    long long days = microseconds / microseconds_per_day;
    microseconds %= microseconds_per_day;

    long long hours = microseconds / microseconds_per_hour;
    microseconds %= microseconds_per_hour;

    long long minutes = microseconds / microseconds_per_minute;
    microseconds %= microseconds_per_minute;

    long long seconds = microseconds / microseconds_per_second;

    char result_text[256];
    snprintf(result_text, sizeof(result_text),
             "%lld months, %lld weeks, %lld days, %lld hours, %lld minutes, %lld seconds",
             months, weeks, days, hours, minutes, seconds);

    gtk_label_set_text(GTK_LABEL(widgets->result_label), result_text);
}

/* GTK calls this once the application is ready to show its first window. */
static void activate(GtkApplication *app, gpointer user_data) {
    /* Same story: 'activate' hands us a user_data pointer, and we passed
     * NULL when connecting the signal, so there is nothing to read. */
    (void)user_data;

    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Microsecond Calculator");
    gtk_window_set_default_size(GTK_WINDOW(window), 420, 180);
    gtk_container_set_border_width(GTK_CONTAINER(window), 12);

    /* A vertical box just stacks its children top to bottom. */
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_container_add(GTK_CONTAINER(window), box);

    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Enter microseconds...");
    gtk_box_pack_start(GTK_BOX(box), entry, FALSE, FALSE, 0);

    GtkWidget *button = gtk_button_new_with_label("Convert");
    gtk_box_pack_start(GTK_BOX(box), button, FALSE, FALSE, 0);

    GtkWidget *result_label = gtk_label_new("Result will appear here.");
    gtk_label_set_line_wrap(GTK_LABEL(result_label), TRUE);
    gtk_box_pack_start(GTK_BOX(box), result_label, FALSE, FALSE, 0);

    /* static: needs to outlive activate() since the callback still
     * needs it every time the button is clicked, long after this
     * function has returned. */
    static AppWidgets widgets;
    widgets.entry = entry;
    widgets.result_label = result_label;

    /* This is the heart of event-driven GUI code: "when the 'clicked'
     * signal fires on this button, call on_convert_clicked, and pass
     * it &widgets so it knows which entry/label to use." */
    g_signal_connect(button, "clicked", G_CALLBACK(on_convert_clicked), &widgets);

    /* Quality-of-life: pressing Enter in the text box also converts. */
    g_signal_connect_swapped(entry, "activate", G_CALLBACK(gtk_button_clicked), button);

    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("com.ricky.microsecondcalculator",
                                               G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
