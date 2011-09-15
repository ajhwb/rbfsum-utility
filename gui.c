#include <gtk/gtk.h>
#include "gui.h"
#include "callback.h"
#include "station.h"
#include "queue.h"


struct window_main *window_main = NULL;
struct window_map *window_map = NULL;

void
create_window_main (void)
{
  GtkWidget *vbox;
  GtkWidget *hbox;
  GtkWidget *menubar;
  GtkWidget *menu;
  GtkWidget *menuitem;
  GtkWidget *submenu;
  GtkWidget *scrolled_window;
  GtkListStore *store;
  GtkCellRenderer *render_id;
  GtkCellRenderer *render_name;
  GtkCellRenderer *render_status;
  GtkTreeViewColumn *column_id;
  GtkTreeViewColumn *column_name;
  GtkTreeViewColumn *column_status;
  PangoFontDescription *font_desc;

  window_main = g_new (struct window_main, 1);

  window_main->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window_main->window), "RBFSUM Utility");
  gtk_window_set_default_size (GTK_WINDOW (window_main->window), 700, 600);
  g_signal_connect (window_main->window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

  vbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (window_main->window), vbox);

  menubar = gtk_menu_bar_new ();
  gtk_box_pack_start (GTK_BOX (vbox), menubar, FALSE, FALSE, 0);

  hbox = gtk_hbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (vbox), hbox);

  menu = gtk_menu_new ();

  menuitem = gtk_menu_item_new_with_mnemonic ("_File");
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem), menu);
  gtk_menu_shell_append (GTK_MENU_SHELL (menubar), menuitem);

  submenu = gtk_image_menu_item_new_from_stock (GTK_STOCK_OPEN, NULL);
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), submenu);
  g_signal_connect (submenu, "activate", G_CALLBACK (menu_open_activate_cb), NULL);

  submenu = gtk_image_menu_item_new_from_stock (GTK_STOCK_QUIT, NULL);
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), submenu);
  g_signal_connect (submenu, "activate", G_CALLBACK (gtk_main_quit), NULL);

  menu = gtk_menu_new ();

  menuitem = gtk_menu_item_new_with_mnemonic ("_Tool");
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem), menu);
  gtk_menu_shell_append (GTK_MENU_SHELL (menubar), menuitem);

  submenu = gtk_menu_item_new_with_mnemonic ("Map View");
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), submenu);
  g_signal_connect (submenu, "activate", G_CALLBACK (menu_map_view_activate_cb), NULL);

  menu = gtk_menu_new ();

  menuitem = gtk_menu_item_new_with_mnemonic ("_Help");
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem), menu);
  gtk_menu_shell_append (GTK_MENU_SHELL (menubar), menuitem);

  submenu = gtk_image_menu_item_new_from_stock (GTK_STOCK_ABOUT, NULL);
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), submenu);
  g_signal_connect (submenu, "activate", G_CALLBACK (menu_about_activate_cb), NULL);

  scrolled_window = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolled_window), GTK_SHADOW_IN);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
                                  GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
  gtk_box_pack_start (GTK_BOX (hbox), scrolled_window, FALSE, FALSE, 0);

  store = gtk_list_store_new (3, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
  window_main->treeview_station = gtk_tree_view_new_with_model (GTK_TREE_MODEL (store));
  gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (window_main->treeview_station), TRUE);
  g_signal_connect (window_main->treeview_station, "row-activated",
                    G_CALLBACK (treeview_station_row_activated_cb), NULL);

  render_id = gtk_cell_renderer_text_new ();
  column_id = gtk_tree_view_column_new_with_attributes ("ID", render_id,
                                                        "text", 0, NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (window_main->treeview_station), column_id);

  render_name = gtk_cell_renderer_text_new ();
  column_name = gtk_tree_view_column_new_with_attributes ("Station", render_name,
                                                          "text", 1, NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (window_main->treeview_station), column_name);

  render_status = gtk_cell_renderer_text_new ();
  g_object_set (render_status, "xalign", 0.5, NULL);
  column_status = gtk_tree_view_column_new_with_attributes ("Status", render_status,
                                                            "text", 2, NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (window_main->treeview_station), column_status);

  gtk_container_add (GTK_CONTAINER (scrolled_window), window_main->treeview_station);

  scrolled_window = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolled_window), GTK_SHADOW_IN);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
                                  GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
  gtk_container_add (GTK_CONTAINER (hbox), scrolled_window);

  font_desc = pango_font_description_new ();
  pango_font_description_set_family (font_desc, "monospace");

  window_main->textview_rbfsum = gtk_text_view_new ();
  gtk_widget_modify_font (window_main->textview_rbfsum, font_desc);
  gtk_text_view_set_editable (GTK_TEXT_VIEW (window_main->textview_rbfsum), FALSE);
  gtk_container_add (GTK_CONTAINER (scrolled_window), window_main->textview_rbfsum);
}

void
set_treeview_station (queue_t *list)
{
  queue_t *ptr;
  GtkTreeModel *model;
  GtkTreeIter iter;
  rbfsum_status_t status;
  station_data_t *data;

  gint i;
  gchar *tmp, *status_string;

  model = gtk_tree_view_get_model (GTK_TREE_VIEW (window_main->treeview_station));
  gtk_list_store_clear (GTK_LIST_STORE (model));

  for (ptr = list, i = 0; ptr; ptr = ptr->next, i++)
    {
      data = ptr->data;
      status = rbfsum_get_status (data->rs, data->ndata);

      if (data->ndata)
        {
          if (status == RBFSUM_STATUS_GREATER)
            status_string = g_strdup ("GR");
          else if (status == RBFSUM_STATUS_LESS)
            status_string = g_strdup ("LE");
          else if (status == RBFSUM_STATUS_EQUAL)
            status_string = g_strdup ("EQ");
          else if (status == RBFSUM_STATUS_ON)
            status_string = g_strdup ("ON");
          else
            status_string = g_strdup ("OF");
        }
      else
        {
          status_string = g_strdup ("-");
        }

      gtk_list_store_append (GTK_LIST_STORE (model), &iter);
      tmp = g_strdup_printf ("%i", i + 1);
      gtk_list_store_set (GTK_LIST_STORE (model), &iter, 
                          0, tmp, 
                          1, data->name, 
                          2, status_string, -1);
      g_free (tmp);
      g_free (status_string);
    }
}

void
create_window_map (void)
{
  GtkWidget *vbox;

  window_map = g_new (struct window_map, 1);

  window_map->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  g_signal_connect (window_map->window, "destroy", G_CALLBACK (gtk_widget_destroy), NULL);

  vbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (window_map->window), vbox);

  window_map->drawing = gtk_drawing_area_new ();
  gtk_container_add (GTK_CONTAINER (vbox), window_map->drawing);
}

