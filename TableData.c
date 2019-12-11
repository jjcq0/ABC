#include "TableData.h"
#include <string.h>
#include "SQL.h"

static void signal_add_button_add(GtkWidget *w, gpointer ptr);
static void signal_add_button_connect(GtkWidget *w, gpointer ptr);
static void signal_add_button_export(GtkWidget *w, gpointer ptr);
static void signal_add_button_delete(GtkWidget *w, gpointer ptr);
static void signal_add_button_refresh(GtkWidget *w, gpointer ptr);
static void signal_add_button_show_hide_db(GtkWidget *w, gpointer ptr);
static void signal_add_button_show_hide_manual(GtkWidget *w, gpointer ptr);

void TD_configure(GtkBuilder *builder)
{
  GtkListStore *store = gtk_list_store_new(3, G_TYPE_STRING, 
                                              G_TYPE_STRING,
                                              G_TYPE_STRING
                                          );
  GtkTreeModel *model = GTK_TREE_MODEL(store);
  GtkWidget    *view  = GTK_WIDGET(gtk_builder_get_object(builder, "TD_view"));

  GtkCellRenderer *renderer;
  renderer = gtk_cell_renderer_text_new();
  
  g_object_set(renderer, "align-set", TRUE, "alignment", PANGO_ALIGN_RIGHT,
                         "editable-set", TRUE, "editable", TRUE, NULL); 

  gtk_tree_view_insert_column_with_attributes(
                                       GTK_TREE_VIEW(view),
                                       -1,
                                       "Nombre Producto",
                                       renderer,
                                       "text", 0,
                                       NULL);

  gtk_tree_view_insert_column_with_attributes(
                                       GTK_TREE_VIEW(view),
                                       -1,
                                       "Precio Unitario",
                                       renderer,
                                       "text", 1,
                                       NULL);

  gtk_tree_view_insert_column_with_attributes(
                                       GTK_TREE_VIEW(view),
                                       -1,
                                       "Unidades Vendidas",
                                       renderer,
                                       "text", 2,
                                       NULL);

  gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);

  GtkWidget *main_button_refresh = GTK_WIDGET(gtk_builder_get_object(builder, "main_button_refresh"));
  GtkWidget *main_button_delete = GTK_WIDGET(gtk_builder_get_object(builder, "main_button_delete"));
  GtkWidget *add_button_add = GTK_WIDGET(gtk_builder_get_object(builder, "add_button_add"));
  GtkWidget *add_button_db = GTK_WIDGET(gtk_builder_get_object(builder, "add_button_db"));
  GtkWidget *add_button_manual = GTK_WIDGET(gtk_builder_get_object(builder, "add_button_manual"));
  GtkWidget *add_button_connect = GTK_WIDGET(gtk_builder_get_object(builder, "add_button_connect"));
  GtkWidget *add_button_export = GTK_WIDGET(gtk_builder_get_object(builder, "add_button_export_data"));
  
  g_signal_connect(G_OBJECT(add_button_add), "clicked", 
                   G_CALLBACK(signal_add_button_add), builder);
  g_signal_connect(G_OBJECT(main_button_refresh), "clicked", 
                   G_CALLBACK(signal_add_button_refresh), builder);
  g_signal_connect(G_OBJECT(main_button_delete), "clicked", 
                   G_CALLBACK(signal_add_button_delete), builder);

  g_signal_connect(G_OBJECT(add_button_db), "clicked", 
                   G_CALLBACK(signal_add_button_show_hide_db), builder);
  g_signal_connect(G_OBJECT(add_button_manual), "clicked",
                   G_CALLBACK(signal_add_button_show_hide_manual), builder);
  g_signal_connect(G_OBJECT(add_button_connect), "clicked",
                   G_CALLBACK(signal_add_button_connect), builder);
  g_signal_connect(G_OBJECT(add_button_export), "clicked",
                   G_CALLBACK(signal_add_button_export), builder);
  
}

static void signal_add_button_add(GtkWidget *w, gpointer ptr)
{
  w = NULL;
  GtkBuilder *builder = (GtkBuilder *)ptr;
  GtkWidget *entry_name = GTK_WIDGET(gtk_builder_get_object(builder, "add_entry_name"));
  GtkWidget *entry_price = GTK_WIDGET(gtk_builder_get_object(builder, "add_entry_price"));
  GtkWidget *entry_ventas = GTK_WIDGET(gtk_builder_get_object(builder, "add_entry_ventas"));
  GtkWidget *view = GTK_WIDGET(gtk_builder_get_object(builder, "TD_view"));
  GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(view));
  GtkListStore *store = (GtkListStore *) model;

  const char *name = gtk_entry_get_text(GTK_ENTRY(entry_name));
  const char *_price = gtk_entry_get_text(GTK_ENTRY(entry_price));
  const char *_ventas = gtk_entry_get_text(GTK_ENTRY(entry_ventas));

  if(!strlen(name) || !strlen(_price) || !strlen(_ventas))
    return ; 

  char ventas[50];
  char price[50];

  sprintf(ventas, "%i", atoi(_ventas));
  sprintf(price, "%.02lf", atof(_price));

  GtkTreeIter iter;
  gtk_list_store_append(store, &iter); 
  gtk_list_store_set(store, &iter,
                     0, name,
                     1, price,
                     2, ventas,
                     -1); 
  
  gtk_entry_set_text(GTK_ENTRY(entry_name), "");
  gtk_entry_set_text(GTK_ENTRY(entry_price), "");
  gtk_entry_set_text(GTK_ENTRY(entry_ventas), "");
}

static void signal_add_button_delete(GtkWidget *w, gpointer ptr)
{
  w = NULL;
  GtkBuilder *builder = (GtkBuilder *) ptr;
  GtkWidget *view = GTK_WIDGET(gtk_builder_get_object(builder, "TD_view"));
  GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(view));
  GtkListStore *store = (GtkListStore *) model;

  GtkTreeSelection * selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
  GtkTreeIter iter;
  if(gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), NULL, &iter))
    gtk_list_store_remove(GTK_LIST_STORE(store), &iter);
}

static void signal_add_button_refresh(GtkWidget *w, gpointer ptr)
{
  w = NULL;
  GtkBuilder *builder = (GtkBuilder *) ptr;
  GtkWidget *view = GTK_WIDGET(gtk_builder_get_object(builder, "TD_view"));
  GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(view));
  GtkListStore *store = (GtkListStore *) model;
  gtk_list_store_clear(store);
}

static void signal_add_button_show_hide_db(GtkWidget *w, gpointer ptr)
{
  w = NULL;
  GtkBuilder *builder = (GtkBuilder *) ptr;
  GtkWidget *add_box_manual = GTK_WIDGET(gtk_builder_get_object(builder, "add_box_manual"));
  GtkWidget *add_box_db = GTK_WIDGET(gtk_builder_get_object(builder, "add_box_db"));
  GtkWidget *window_add = GTK_WIDGET(gtk_builder_get_object(builder, "window_add"));

  gtk_widget_hide(add_box_manual); 
  gtk_widget_show(add_box_db); 
  gtk_window_resize(GTK_WINDOW(window_add), 200, 200);
}
static void signal_add_button_show_hide_manual(GtkWidget *w, gpointer ptr)
{
  w = NULL;
  GtkBuilder *builder = (GtkBuilder *) ptr;
  GtkWidget *add_box_manual = GTK_WIDGET(gtk_builder_get_object(builder, "add_box_manual"));
  GtkWidget *add_box_db = GTK_WIDGET(gtk_builder_get_object(builder, "add_box_db"));
  GtkWidget *window_add = GTK_WIDGET(gtk_builder_get_object(builder, "window_add"));
  
  gtk_widget_show(add_box_manual);
  gtk_widget_hide(add_box_db);
  gtk_window_resize(GTK_WINDOW(window_add), 200, 200);
}
static void signal_add_button_connect(GtkWidget *w, gpointer ptr)
{
  w = NULL;
  GtkBuilder *builder = (GtkBuilder *) ptr;
  GtkWidget *entry_user = GTK_WIDGET(gtk_builder_get_object(builder, "add_entry_user"));
  GtkWidget *entry_password = GTK_WIDGET(gtk_builder_get_object(builder, "add_entry_password"));
  GtkWidget *spinner_connect = GTK_WIDGET(gtk_builder_get_object(builder, "add_spinner_connect"));
  GtkWidget *add_combo_box_tables = GTK_WIDGET(gtk_builder_get_object(builder, "add_combo_box_tables"));  
  
  const char *user = gtk_entry_get_text(GTK_ENTRY(entry_user));
  const char *pass = gtk_entry_get_text(GTK_ENTRY(entry_password));
  const char *namedb = "ABC";
  const char *hostname = "localhost";

  if(!strlen(user) || !strlen(pass))
    return ;

  gtk_spinner_start(GTK_SPINNER(spinner_connect));
  SQL *sql = SQL_create(hostname, namedb, user, pass);
  SQL_Result *result = SQL_Result_create();
  int ret = SQL_get_tables_from_db(sql, result);
  if(ret == -1)
  {
    printf("%s\n", result->error);
    gtk_spinner_stop(GTK_SPINNER(spinner_connect));
    SQL_destroy(sql);
    SQL_Result_destroy(result);
    return ;
  }

  gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(add_combo_box_tables));
  int i;
  for(i = 0; i < result->len; ++i)
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(add_combo_box_tables),
                              result->data[i],
                              result->data[i]);
  gtk_combo_box_set_active(GTK_COMBO_BOX(add_combo_box_tables), 0);

  gtk_spinner_stop(GTK_SPINNER(spinner_connect));
  SQL_destroy(sql);
  SQL_Result_destroy(result);
}

static void signal_add_button_export(GtkWidget *w, gpointer ptr)
{
  w = NULL;
  GtkBuilder *builder = (GtkBuilder *) ptr;
  GtkWidget *view = GTK_WIDGET(gtk_builder_get_object(builder, "TD_view"));
  GtkWidget *entry_user = GTK_WIDGET(gtk_builder_get_object(builder, "add_entry_user"));
  GtkWidget *entry_password = GTK_WIDGET(gtk_builder_get_object(builder, "add_entry_password"));
  GtkWidget *spinner_connect = GTK_WIDGET(gtk_builder_get_object(builder, "add_spinner_connect"));
  GtkWidget *add_combo_box_tables = GTK_WIDGET(gtk_builder_get_object(builder, "add_combo_box_tables"));

  const char *user = gtk_entry_get_text(GTK_ENTRY(entry_user));
  const char *pass = gtk_entry_get_text(GTK_ENTRY(entry_password));
  const char *namedb = "ABC";
  const char *hostname = "localhost";

  if(!strlen(user) || !strlen(pass))
    return ;
  
  SQL *sql = SQL_create(hostname, namedb, user, pass);
  SQL_Result *result = SQL_Result_create();

  const char *table = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(add_combo_box_tables));
  if(!table)
  {
    printf("No hay tabla\n");
    GtkWidget *window_add = GTK_WIDGET(gtk_builder_get_object(builder, "window_add"));
   	GtkWidget *MSG = gtk_message_dialog_new(GTK_WINDOW(window_add), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR,
                                            GTK_BUTTONS_OK, "Antes de continuar\nSeleccione una tabla");
    gtk_widget_show(MSG);
    return ;
  }

  sql->table = strdup(table);
  gtk_spinner_start(GTK_SPINNER(spinner_connect));
  
  int ret = ABC_get_data(sql, result, view); 
  if(ret == -1)
  {
    printf("%s\n", result->error);
    gtk_spinner_stop(GTK_SPINNER(spinner_connect));
    SQL_destroy(sql);
    SQL_Result_destroy(result);
    return ;
  }

  gtk_spinner_stop(GTK_SPINNER(spinner_connect));
  SQL_destroy(sql);
}

