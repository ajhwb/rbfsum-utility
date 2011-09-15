#include <glib.h>
#include "station.h"
#include "queue.h"

int main(int argc, char *argv[])
{
  GKeyFile *key = g_key_file_new ();
  int i = 0;
  char *tmp;

  while (station_info[i].id) {
    g_key_file_set_string (key, 
                           station_info[i].name,
                           "name",
                           station_info[i].desc);

    tmp = g_strdup_printf ("%g", station_info[i].lat);

    g_key_file_set_string (key,
                           station_info[i].name,
                           "lat",
                           tmp);

    g_free (tmp);

    tmp = g_strdup_printf ("%g", station_info[i].lng);

    g_key_file_set_string (key,
                           station_info[i].name,
                           "lng",
                           tmp);

    g_free (tmp);

    g_key_file_set_integer (key,
                           station_info[i].name,
                           "enable",
                           station_info[i].state);
    i++;
  }

  tmp = g_key_file_to_data (key, NULL, NULL);
  tmp = g_strstrip (tmp);
  g_key_file_free (key);

  g_print ("%s", tmp);

  g_free (tmp);

  return 0;
}
