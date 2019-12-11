#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include "TableData.h"
#include "TableResult.h"
#include "TableDetails.h"

static void signal_window_destroy();
static void signal_active_window(GtkWidget *w, gpointer ptr);
static void signal_disable_window(GtkWidget *w, gpointer ptr);

GtkBuilder *builder;

int main(int argc, char **argv)
{
  GtkWidget *window_main;
  GtkWidget *window_add;
  GtkWidget *window_settings;
  GtkWidget *button_active_add;
  GtkWidget *button_active_settings;

  GtkWidget *button_disable_add;
  GtkWidget *button_disable_main;
  GtkWidget *button_disable_settings;

  gtk_init(&argc, &argv);

  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "paint.glade", NULL);

  window_main = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
  window_add = GTK_WIDGET(gtk_builder_get_object(builder, "window_add"));
  window_settings = GTK_WIDGET(gtk_builder_get_object(builder, "window_settings"));
 
  button_active_add = GTK_WIDGET(gtk_builder_get_object(builder, "main_button_add"));
  button_active_settings = GTK_WIDGET(gtk_builder_get_object(builder, "main_button_settings"));
 
  button_disable_add = GTK_WIDGET(gtk_builder_get_object(builder, "add_button_close"));
  button_disable_settings = GTK_WIDGET(gtk_builder_get_object(builder, "settings_button_close"));
  button_disable_main = GTK_WIDGET(gtk_builder_get_object(builder, "main_button_close"));

  /* Signals */
  g_signal_connect(G_OBJECT(window_main), "destroy", 
                   G_CALLBACK(signal_window_destroy), NULL);
  g_signal_connect(G_OBJECT(window_add), "destroy", 
                   G_CALLBACK(signal_window_destroy), NULL);
  g_signal_connect(G_OBJECT(window_settings), "destroy", 
                   G_CALLBACK(signal_window_destroy), NULL);
  g_signal_connect(G_OBJECT(button_disable_main), "clicked", 
                   G_CALLBACK(signal_window_destroy), NULL);
 
  g_signal_connect(G_OBJECT(button_active_add), "clicked",
                   G_CALLBACK(signal_active_window), window_add);
  g_signal_connect(G_OBJECT(button_active_settings), "clicked",
                   G_CALLBACK(signal_active_window), window_settings);
  g_signal_connect(G_OBJECT(button_disable_add), "clicked",
                   G_CALLBACK(signal_disable_window), window_add);
  g_signal_connect(G_OBJECT(button_disable_settings), "clicked",
                   G_CALLBACK(signal_disable_window), window_settings);

 	gtk_window_set_transient_for(GTK_WINDOW(window_add), GTK_WINDOW(window_main));
 	gtk_window_set_transient_for(GTK_WINDOW(window_settings), GTK_WINDOW(window_main));

  /* TD_configure */
  TD_configure(builder);
  TR_configure(builder);
  TDE_configure(builder);

//  g_object_unref(builder);
  gtk_widget_show(window_main);
  gtk_main();

  return 0;
}

static void signal_window_destroy()
{
  gtk_main_quit();
  exit(0);
}
static void signal_active_window(GtkWidget *w, gpointer ptr)
{ 
  w = NULL;
  gtk_widget_show_all((GtkWidget *) ptr);  
  GtkWidget *add_box_db = GTK_WIDGET(gtk_builder_get_object(builder, "add_box_db"));
  GtkWidget *add_box_manual = GTK_WIDGET(gtk_builder_get_object(builder, "add_box_manual"));
  gtk_widget_hide(add_box_db);
  gtk_widget_show(add_box_manual);
  gtk_window_resize(GTK_WINDOW((GtkWidget *) ptr), 200, 200);
}
static void signal_disable_window(GtkWidget *w, gpointer ptr)
{
  w = NULL;
  gtk_widget_hide((GtkWidget *) ptr);
}

