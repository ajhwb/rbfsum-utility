#ifndef __GUI_H__
#define __GUI_H__

#include <gtk/gtk.h>
#include "queue.h"

struct window_main {
  GtkWidget *window;
  GtkWidget *treeview_station;
  GtkWidget *textview_rbfsum;
};

struct window_map {
  GtkWidget *window;
  GtkWidget *drawing;
};

extern struct window_main *window_main;
extern struct window_map *window_map;

void create_window_main (void);
void set_treeview_station (queue_t*);

#endif
