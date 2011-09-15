#include <gtk/gtk.h>
#include "gui.h"

gint
main (gint argc, gchar **argv)
{
  gtk_init (&argc, &argv);

  create_window_main ();
  gtk_widget_show_all (window_main->window);

  gtk_main ();

  return 0;
}
