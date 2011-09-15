#include <gtk/gtk.h>
#include <time.h>
#include <string.h>
#include "config.h"
#include "station.h"
#include "gui.h"

static gchar *rbfsum_buffer = NULL;

static queue_t *station_list = NULL;

void
set_status (void)
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  rbfsum_t rs[96];
  guint ndata;
  gchar *name;
  rbfsum_status_t status;

  model = gtk_tree_view_get_model (GTK_TREE_VIEW (window_main->treeview_station));

  if (!gtk_tree_model_get_iter_first (model, &iter))
    return;

  do
    {
      gtk_tree_model_get (model, &iter, 1, &name, -1);
      read_rbfsum (rbfsum_buffer, name, rs, &ndata);
      g_free (name);
      status = rbfsum_get_status (rs, ndata);

      if (ndata == 0)
        {
          gtk_list_store_set (GTK_LIST_STORE (model), &iter, 2, "-", -1);
        }
      else
        {
          if (status == RBFSUM_STATUS_GREATER)
            gtk_list_store_set (GTK_LIST_STORE (model), &iter, 2, "GR", -1);
          else if (status == RBFSUM_STATUS_LESS)
            gtk_list_store_set (GTK_LIST_STORE (model), &iter, 2, "LE", -1);
          else if (status == RBFSUM_STATUS_EQUAL)
            gtk_list_store_set (GTK_LIST_STORE (model), &iter, 2, "EQ", -1);
          else if (status == RBFSUM_STATUS_ON)
            gtk_list_store_set (GTK_LIST_STORE (model), &iter, 2, "ON", -1);
          else
            gtk_list_store_set (GTK_LIST_STORE (model), &iter, 2, "OF", -1);
        }
    } while (gtk_tree_model_iter_next (model, &iter));
}

void
menu_open_activate_cb (GtkWidget *widget, gpointer data)
{
  queue_t *list;
  GtkWidget *fs;
  GIOChannel *channel;
  GError *error = NULL;
  GIOStatus status;
  gchar *filename, *buffer, *tmp, *tmp2, *title;

  fs = gtk_file_chooser_dialog_new ("Open File", 
                                    GTK_WINDOW (window_main->window),
                                    GTK_FILE_CHOOSER_ACTION_OPEN,
                                    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                    GTK_STOCK_OPEN, GTK_RESPONSE_OK,
                                    NULL);

  if (gtk_dialog_run (GTK_DIALOG (fs)) != GTK_RESPONSE_OK)
    {
      gtk_widget_destroy (fs);
      return;
    }

  filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (fs));
  gtk_widget_destroy (fs);
  channel = g_io_channel_new_file (filename, "r", NULL);

  status = g_io_channel_read_to_end (channel, &buffer, NULL, &error);
  if (status != G_IO_STATUS_NORMAL)
    {
      tmp = g_path_get_basename (filename);
      fs = gtk_message_dialog_new (GTK_WINDOW (window_main->window),
                                   GTK_DIALOG_MODAL,
                                   GTK_MESSAGE_ERROR,
                                   GTK_BUTTONS_OK,
                                   "%s: %s", tmp, error->message);
      gtk_dialog_run (GTK_DIALOG (fs));
      gtk_widget_destroy (fs);
      g_free (tmp);
      g_error_free (error);
    }
  else
    {
      list = read_rbfsum2 (buffer);
      g_free (buffer);
      if (!list)
        {
          fs = gtk_message_dialog_new (GTK_WINDOW (window_main->window),
                                       GTK_DIALOG_MODAL,
                                       GTK_MESSAGE_ERROR,
                                       GTK_BUTTONS_OK,
                                       "Format file tidak valid");
          gtk_dialog_run (GTK_DIALOG (fs));
          gtk_widget_destroy (fs);
          return;
        }
           

      set_treeview_station (list);

      station_list = list;

      tmp = filename;
      if ((tmp2 = strrchr (filename, '/')))
        tmp = ++tmp2;

      title = g_strdup_printf ("%s - RBFSUM Utility", tmp);
      gtk_window_set_title (GTK_WINDOW (window_main->window), title);
      g_free (title);
    }

  g_free (filename);
  g_io_channel_shutdown (channel, FALSE, NULL);
  g_io_channel_unref (channel);
}

void
menu_map_view_activate_cb (GtkWidget *widget, gpointer data)
{
  
}

void
menu_about_activate_cb (GtkWidget *widget, gpointer data)
{
  GtkWidget *dialog;

  dialog = gtk_about_dialog_new ();
  gtk_about_dialog_set_program_name (GTK_ABOUT_DIALOG (dialog), PROGRAM_NAME);
  gtk_about_dialog_set_version (GTK_ABOUT_DIALOG (dialog), PROGRAM_VERS);
  gtk_about_dialog_set_comments (GTK_ABOUT_DIALOG (dialog), PROGRAM_DESC);
  gtk_about_dialog_set_copyright (GTK_ABOUT_DIALOG (dialog), 
                                  "Copyright \302\251 2011 Ardhan Madras "
                                  "<ardhan@mindotama.co.id>");
  gtk_window_set_skip_taskbar_hint (GTK_WINDOW (dialog), TRUE);

  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);
}

void
insert_rbfsum_result (rbfsum_t data[96], guint len, gint station_id)
{
  GtkTreeSelection *selection;
  GtkTreeModel *model;
  GtkTreeIter titer;
  GtkTextBuffer *buffer;
  GtkTextIter iter;
  gchar *tmp, *start, *end, *breaks;
  gint i;
  struct tm *st;

  buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (window_main->textview_rbfsum));
  gtk_text_buffer_set_text (buffer, "", 0);
  gtk_text_buffer_get_start_iter (buffer, &iter);

  gtk_text_buffer_insert (buffer, &iter, "Station : ", -1);
  if (station_id >= 0)
    {
      gtk_text_buffer_insert (buffer, &iter, station_info[station_id].name, -1);
    }
  else
    {
      selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (window_main->treeview_station));
      gtk_tree_selection_get_selected (selection, &model, &titer);
      gtk_tree_model_get (model, &titer, 1, &tmp, -1);
      gtk_text_buffer_insert (buffer, &iter, tmp, -1);
      g_free (tmp);
    } 

  if (station_id >= 0)
    {
      gtk_text_buffer_insert (buffer, &iter, ", ", -1);
      gtk_text_buffer_insert (buffer, &iter, station_info[station_id].desc, -1);
    }

  gtk_text_buffer_insert (buffer, &iter, "\n", -1);
  if (station_id >= 0)
    tmp = g_strdup_printf ("Location: %g, %g\n", 
                           station_info[station_id].lat,
                           station_info[station_id].lng);
  else
    tmp = g_strdup_printf ("Location: Unknown\n");
  gtk_text_buffer_insert (buffer, &iter, tmp, -1);
  g_free (tmp);

  if (len == 0)
    {
      gtk_text_buffer_insert (buffer, &iter, "\nNo rbfsum data\n", -1);
      return;
    }

  st = localtime (&data[0].start);
  start = g_strdup_printf ("%i-%.2i-%.2i_%.2i:00", st->tm_year + 1900,
                           st->tm_mon + 1, st->tm_mday, st->tm_hour);

  st = localtime (&data[len - 1].start);
  end = g_strdup_printf ("%i-%.2i-%.2i_%.2i:00", st->tm_year + 1900,
                           st->tm_mon + 1, st->tm_mday, st->tm_hour);

  gtk_text_buffer_insert (buffer, &iter, "Interval: ", -1);
  gtk_text_buffer_insert (buffer, &iter, start, -1);
  gtk_text_buffer_insert (buffer, &iter, " - ", -1);
  gtk_text_buffer_insert (buffer, &iter, end, -1);
  gtk_text_buffer_insert (buffer, &iter, "\n\n", -1);
  g_free (start);
  g_free (end);

  gtk_text_buffer_insert (buffer, &iter,
                          " ID  Hour starting       Data     ReTx     Gaps   Breaks\n",
                          -1);
  gtk_text_buffer_insert (buffer, &iter, " -------------------------------------------------------\n", -1);

  for (i = 0; i < len; i++) 
    {
      st = localtime (&data[i].start);
      start = g_strdup_printf ("%i-%.2i-%.2i_%.2i:00", st->tm_year + 1900,
                              st->tm_mon + 1, st->tm_mday, st->tm_hour);
      breaks = g_strdup_printf ("%i", data[i].breaks);
      tmp = g_strdup_printf ("%3i  %s%10.2f%7.1f%10.2f%5s\n",
                             i + 1, start, data[i].data, data[i].retx, 
                             data[i].gap, data[i].breaks ? breaks : "-");
      gtk_text_buffer_insert (buffer, &iter, tmp, -1);
      g_free (tmp);
      g_free (start);
      g_free (breaks);
    }
}

void
treeview_station_row_activated_cb (GtkWidget *widget, GtkTreePath *path, 
                GtkTreeViewColumn *column, gpointer data)
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  gchar *name;
  gint id;
  queue_t *ptr;
  station_data_t *station_data;

  if (!station_list)
    return;

  model = gtk_tree_view_get_model (GTK_TREE_VIEW (widget));
  gtk_tree_model_get_iter (model, &iter, path);
  gtk_tree_model_get (model, &iter, 1, &name, -1);

  id = lookup_station (name);
  ptr = station_list;
  while (ptr)
    {
      station_data = ptr->data;
      if (!strcmp (name, station_data->name))
        break;
      ptr = ptr->next;
    }

  g_free (name);
  g_return_if_fail (ptr != NULL);
  insert_rbfsum_result (station_data->rs, station_data->ndata, id);
}

