#ifndef MTSCAN_UI_H_
#define MTSCAN_UI_H_
#include <gtk/gtk.h>
#include "mtscan.h"
#include "model.h"
#include "ui-connection.h"
#include "mt-ssh.h"

#define UNIX_TIMESTAMP() (g_get_real_time() / 1000000)

typedef struct mtscan_gtk
{
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *box_toolbar;

    GtkWidget *toolbar;
    GtkToolItem *b_connect;
    GtkToolItem *b_scan;
    GtkToolItem *b_restart;
    GtkToolItem *b_scanlist_default;
    GtkToolItem *b_scanlist_preset;
    GtkToolItem *b_scanlist;

    GtkToolItem *b_new;
    GtkToolItem *b_open;
    GtkToolItem *b_merge;
    GtkToolItem *b_save;
    GtkToolItem *b_save_as;
    GtkToolItem *b_export;
    GtkToolItem *b_screenshot;

    GtkToolItem *b_preferences;
    GtkToolItem *b_sound;
    GtkToolItem *b_mode;
    GtkToolItem *b_gps;
    GtkToolItem *b_about;

    GtkWidget *scroll;
    GtkWidget *treeview;

    GtkWidget *statusbar_align;
    GtkWidget *statusbar;
    GtkWidget *heartbeat;
    GtkWidget *l_net_status;
    GtkWidget *l_conn_status;
    GtkWidget *group_gps, *l_gps_status;

    mtscan_model_t *model;
    gchar *filename;
    gchar *name;

    ui_connection_t *conn_dialog;
    mt_ssh_t *conn;

    gboolean connected;
    gboolean scanning;
    gboolean changed;
    gboolean heartbeat_status;
} mtscan_gtk_t;

mtscan_gtk_t ui;

void ui_init(void);

void ui_connected(const gchar*, const gchar*, const gchar*);
void ui_disconnected(void);
void ui_changed(void);
gboolean ui_can_discard_unsaved(void);
void ui_status_update_networks(void);

void ui_set_title(gchar*);
void ui_clear(void);
void ui_show_uri(const gchar*);
void ui_play_sound(gchar*);
void ui_screenshot(void);

#endif
