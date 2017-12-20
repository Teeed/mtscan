#ifndef MTSCAN_CONF_PROFILES_H_
#define MTSCAN_CONF_PROFILES_H_

typedef struct conf_profile conf_profile_t;

enum
{
    CONF_PROFILE_COL_NAME,
    CONF_PROFILE_COL_HOST,
    CONF_PROFILE_COL_PORT,
    CONF_PROFILE_COL_LOGIN,
    CONF_PROFILE_COL_PASSWORD,
    CONF_PROFILE_COL_INTERFACE,
    CONF_PROFILE_COL_DURATION_TIME,
    CONF_PROFILE_COL_DURATION,
    CONF_PROFILE_COL_REMOTE,
    CONF_PROFILE_COL_BACKGROUND,
    CONF_PROFILE_COLS
};

conf_profile_t* conf_profile_new(gchar*, gchar*, gint, gchar*, gchar*, gchar*, gint, gboolean, gboolean, gboolean);
void conf_profile_free(conf_profile_t*);

const gchar* conf_profile_get_name(const conf_profile_t*);
const gchar* conf_profile_get_host(const conf_profile_t*);
gint conf_profile_get_port(const conf_profile_t*);
const gchar* conf_profile_get_login(const conf_profile_t*);
const gchar* conf_profile_get_password(const conf_profile_t*);
const gchar* conf_profile_get_interface(const conf_profile_t*);
gint conf_profile_get_duration_time(const conf_profile_t*);
gint conf_profile_get_duration(const conf_profile_t*);
gboolean conf_profile_get_remote(const conf_profile_t*);
gboolean conf_profile_get_background(const conf_profile_t*);

GtkListStore* conf_profile_list_new(void);
GtkTreeIter conf_profile_list_add(GtkListStore*, const conf_profile_t*);
conf_profile_t* conf_profile_list_get(GtkListStore*, GtkTreeIter*);

#endif

