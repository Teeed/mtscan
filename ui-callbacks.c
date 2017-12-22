#include "model.h"
#include "ui-callbacks.h"
#include "ui-toolbar.h"
#include "ui-view.h"
#include "gps.h"
#include "scanlist.h"
#include "mt-ssh.h"
#include "ui-dialogs.h"

static gboolean ui_callback_heartbeat_timeout(gpointer);
static gboolean ui_callback_timeout(gpointer);

void
ui_callback_status(const mt_ssh_t *context,
                   const gchar    *status,
                   const gchar    *extended_error)
{
    if(ui.conn != context)
        return;

    if(!ui.conn_dialog)
        return;

    ui_connection_set_status(ui.conn_dialog, status, extended_error);
}

void
ui_callback_verify(const mt_ssh_t *context,
                   const gchar    *data)
{
    gboolean verify;

    if(ui.conn != context)
        return;

    if(!ui.conn_dialog)
        return;

    verify = ui_connection_verify(ui.conn_dialog, data);

    if(ui.conn != context)
        return;

    if(verify)
        mt_ssh_cmd(ui.conn, MT_SSH_CMD_AUTH, NULL);
    else
        mt_ssh_cancel(ui.conn);
}

void
ui_callback_connected(const mt_ssh_t *context,
                      const gchar    *identity)
{
    if(ui.conn != context)
        return;

    ui_connected(mt_ssh_get_login(context),
                 mt_ssh_get_hostname(context),
                 mt_ssh_get_interface(context));

    if(ui.conn_dialog)
        ui_connection_connected(ui.conn_dialog);
}

void
ui_callback_disconnected(const mt_ssh_t *context)
{
    if(ui.conn != context)
        return;

    ui_disconnected();
    ui.conn = NULL;

    if(ui.conn_dialog)
        ui_connection_disconnected(ui.conn_dialog);
}

void
ui_callback_scanning_state(const mt_ssh_t *context,
                           gboolean        value)
{
    if(ui.conn != context)
        return;

    ui.scanning = value;
    ui_toolbar_scan_set_state(ui.scanning);

    if(!ui.scanning)
        mtscan_model_buffer_clear(ui.model);

    gtk_widget_set_sensitive(GTK_WIDGET(ui.b_scan), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(ui.b_restart), TRUE);
}

void
ui_callback_scanning_error(const mt_ssh_t *context,
                           const gchar    *error)
{
    if(context != ui.conn)
        return;

    ui_dialog(GTK_WINDOW(ui.window),
              GTK_MESSAGE_ERROR,
              APP_NAME,
              "<big><b>Scanning error:</b></big>\n%s",
              error);
}

void
ui_callback_network(const mt_ssh_t *context,
                    network_t      *net)
{
    const mtscan_gps_data_t *gps_data;

    if(ui.conn != context)
    {
        network_free(net);
        g_free(net);
        return;
    }

    if(gps_get_data(&gps_data) == GPS_OK)
    {
        net->latitude = gps_data->lat;
        net->longitude = gps_data->lon;
    }

    mtscan_model_buffer_add(ui.model, net);
}

void
ui_callback_heartbeat(const mt_ssh_t *context)
{
    static guint timeout_id = 0;

    if(ui.conn != context)
        return;

    gtk_widget_freeze_child_notify(ui.treeview);
    switch(mtscan_model_buffer_and_inactive_update(ui.model))
    {
        case MODEL_UPDATE_NEW:
            ui_view_check_position(ui.treeview);
            ui_play_sound(APP_SOUND_NETWORK);
        case MODEL_UPDATE:
            ui_changed();
        case MODEL_UPDATE_ONLY_INACTIVE:
            ui_status_update_networks();
            break;
    }
    gtk_widget_thaw_child_notify(ui.treeview);
    ui_callback_heartbeat_timeout(GINT_TO_POINTER(TRUE));

    if(timeout_id)
        g_source_remove(timeout_id);
    timeout_id = g_timeout_add(ui.model->active_timeout * 1000, ui_callback_timeout, &timeout_id);
}

void
ui_callback_scanlist(const mt_ssh_t *context,
                     const gchar    *data)
{
    scanlist_current(data);
}

static gboolean
ui_callback_heartbeat_timeout(gpointer data)
{
    static guint timeout_id = 0;
    ui.heartbeat_status = GPOINTER_TO_INT(data);
    gtk_widget_queue_draw(ui.heartbeat);

    if(ui.heartbeat_status)
    {
        if(timeout_id)
            g_source_remove(timeout_id);
        timeout_id = g_timeout_add(500, ui_callback_heartbeat_timeout, GINT_TO_POINTER(FALSE));
    }
    else
    {
        timeout_id = 0;
    }
    return FALSE;
}

static gboolean
ui_callback_timeout(gpointer data)
{
    guint *timeout_id = (guint*)data;
    mtscan_model_clear_active(ui.model);
    ui_status_update_networks();
    *timeout_id = 0;
    return FALSE;
}
