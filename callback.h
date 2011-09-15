#ifndef __CALLBACK_H__
#define __CALLBACK_H__

void menu_open_activate_cb (GtkWidget*, gpointer);
void menu_map_view_activate_cb (GtkWidget*, gpointer);
void menu_about_activate_cb (GtkWidget*, gpointer);
void treeview_station_row_activated_cb (GtkWidget*, GtkTreePath*, GtkTreeViewColumn*, gpointer);

#endif
