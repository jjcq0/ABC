#include "TableResult.h"
#include <string.h>

static void signal_button_start(GtkWidget *w, gpointer ptr);
static void signal_toogle_button_state(GtkWidget *w, gpointer ptr);

void TR_configure(GtkBuilder *builder)
{
  GtkListStore *store = gtk_list_store_new(8, G_TYPE_STRING,
                                              G_TYPE_STRING,
                                              G_TYPE_STRING,
                                              G_TYPE_STRING,
                                              G_TYPE_STRING,
                                              G_TYPE_STRING,
                                              G_TYPE_STRING,
                                              GDK_TYPE_RGBA);
  GtkTreeModel *model = GTK_TREE_MODEL(store);
  GtkWidget *view = GTK_WIDGET(gtk_builder_get_object(builder, "TR_view"));
 
  GtkCellRenderer *renderer;
  renderer = gtk_cell_renderer_text_new();
  /*g_object_set(renderer, "cell-background-set", TRUE, 
                         "align-set", TRUE, 
                         "alignment", PANGO_ALIGN_RIGHT,
                         NULL);*/
  gtk_tree_view_insert_column_with_attributes(
                                       GTK_TREE_VIEW(view),
                                       -1,
                                       "ID",
                                       renderer,
                                       "text", 0,
                                       "cell-background-rgba", 7,
                                       NULL);
  gtk_tree_view_insert_column_with_attributes(
                                       GTK_TREE_VIEW(view),
                                       -1,
                                       "Nombre Producto",
                                       renderer,
                                       "text", 1,
                                       NULL);

  gtk_tree_view_insert_column_with_attributes(
                                       GTK_TREE_VIEW(view),
                                       -1,
                                       "Precio Unitario",
                                       renderer,
                                       "text", 2,
                                       NULL);

  gtk_tree_view_insert_column_with_attributes(
                                       GTK_TREE_VIEW(view),
                                       -1,
                                       "Unidades Vendidas",
                                       renderer,
                                       "text", 3,
                                       NULL);

  gtk_tree_view_insert_column_with_attributes(
                                       GTK_TREE_VIEW(view),
                                       -1,
                                       "Venta total",
                                       renderer,
                                       "text", 4,
                                       NULL);

  gtk_tree_view_insert_column_with_attributes(
                                       GTK_TREE_VIEW(view),
                                       -1,
                                       "Participacion",
                                       renderer,
                                       "text", 5,
                                       NULL);

  gtk_tree_view_insert_column_with_attributes(
                                       GTK_TREE_VIEW(view),
                                       -1,
                                       "Participacion Acumulada",
                                       renderer,
                                       "text", 6,
                                       NULL);

  gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);
  
  GtkWidget *button_start = GTK_WIDGET(gtk_builder_get_object(builder, "main_button_start"));
  GtkWidget *toogle_button = GTK_WIDGET(gtk_builder_get_object(builder, "main_toggle_button_color"));
  GtkWidget *button_apply = GTK_WIDGET(gtk_builder_get_object(builder, "settings_button_apply"));

  g_signal_connect(G_OBJECT(button_start), "clicked", 
                   G_CALLBACK(signal_button_start), builder);
  g_signal_connect(G_OBJECT(toogle_button), "toggled", 
                   G_CALLBACK(signal_toogle_button_state), builder);
  g_signal_connect(G_OBJECT(button_apply), "clicked",
                   G_CALLBACK(signal_toogle_button_state), builder);
}

static void signal_button_start(GtkWidget *w, gpointer ptr)
{
  w = NULL;
  GtkBuilder *builder = (GtkBuilder *)ptr;
  GtkWidget *view =  GTK_WIDGET(gtk_builder_get_object(builder, "TD_view"));
  GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(view));
  int TD_len = gtk_tree_model_iter_n_children(model, NULL);

  if(TD_len < 3)
    return ;
  
  int i = 0, j = 0;

  char *name[TD_len];
  double price[TD_len];
  double units[TD_len];
  double ventas[TD_len];
  double participacion[TD_len];
  double acumulado[TD_len];
  double sum_total = 0;

  for(i = 0; i < TD_len; ++i)
  {
    gchar *col1, *col2, *col3;
    GtkTreePath *path = gtk_tree_path_new_from_indices(i, -1);
    GtkTreeIter iter;
    gtk_tree_model_get_iter(model, &iter, path);

    gtk_tree_model_get(model, &iter,
                       0, &col1,
                       1, &col2,
                       2, &col3,
                       -1);
    name[i] = strdup(col1);
    price[i] = atof(col2);
    units[i] = atof(col3);
    ventas[i] = price[i] * units[i];

    sum_total += ventas[i];

    g_free(col1);
    g_free(col2);
    g_free(col3);
  }

  for (i = 1; i < TD_len; i++)
  {
    for (j = 0; j < TD_len - i; j++)
    {
      if (ventas[j] < ventas[j + 1])
      {
         double tmp = price[j];
         price[j] = price[j + 1];
         price[j + 1] = tmp;

         tmp = units[j];
         units[j] = units[j+1];
         units[j+1] = tmp;

         tmp = ventas[j];
         ventas[j] = ventas[j+1];
         ventas[j+1] = tmp;

         char *_name = name[j];
         name[j] = name[j+1];
         name[j+1] = _name;
      }
    }
  }

  participacion[0] = ventas[0];
  for(i = 1; i < TD_len; ++i)
    participacion[i] = (participacion[i - 1] + ventas[i]);

  for(i = 0; i < TD_len; ++i)
    acumulado[i] = ((participacion[i] * 100) / sum_total);

  GtkWidget *cbtna = GTK_WIDGET(gtk_builder_get_object(builder, "settings_color_button_a"));
  GtkWidget *cbtnb = GTK_WIDGET(gtk_builder_get_object(builder, "settings_color_button_b"));
  GtkWidget *cbtnc = GTK_WIDGET(gtk_builder_get_object(builder, "settings_color_button_c"));

  GtkWidget *sbtna = GTK_WIDGET(gtk_builder_get_object(builder, "settings_spin_button_a"));
  GtkWidget *sbtnb = GTK_WIDGET(gtk_builder_get_object(builder, "settings_spin_button_b"));
  GtkWidget *sbtnc = GTK_WIDGET(gtk_builder_get_object(builder, "settings_spin_button_c"));

  GdkRGBA colorA, colorB, colorC;
  gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(cbtna), &colorA);
  gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(cbtnb), &colorB);
  gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(cbtnc), &colorC);

  double range_a = gtk_spin_button_get_value(GTK_SPIN_BUTTON(sbtna));
  double range_b = gtk_spin_button_get_value(GTK_SPIN_BUTTON(sbtnb));
  double range_c = gtk_spin_button_get_value(GTK_SPIN_BUTTON(sbtnc));

  GtkWidget *view_r = GTK_WIDGET(gtk_builder_get_object(builder, "TR_view"));
  GtkTreeModel *model_r = gtk_tree_view_get_model(GTK_TREE_VIEW(view_r));
  GtkListStore *store_r = (GtkListStore *) model_r;

  gtk_list_store_clear(store_r);
  GtkTreeIter iter;

  int a = 0;
  int b = 0;
  int c = 0;

  /*
  printf("Loading\n");
  return ;*/ 

  for(i = 0; i < TD_len; ++i)
  {
    char _col1[30];
    char _col3[30];
    char _col4[30];
    char _col5[30];
    char _col6[30];
    char _col7[30];

    sprintf(_col1, "%i", (i + 1));
    sprintf(_col3, "%.2lf", price[i]);
    sprintf(_col4, "%.2lf", units[i]);
    sprintf(_col5, "%.2lf", ventas[i]);
    sprintf(_col6, "%.2lf", participacion[i]);
    sprintf(_col7, "%.2lf", acumulado[i]);

    gtk_list_store_append(store_r, &iter);
    gtk_list_store_set(store_r, &iter, 
                       0, _col1,
                       1, name[i],
                       2, _col3,
                       3, _col4,
                       4, _col5,
                       5, _col6,
                       6, _col7,
                       -1);
    if(a == 0)
    {
      gtk_list_store_set(store_r, &iter, 7, &colorA, -1);
      if(acumulado[i] > range_a)
        a = 1;
    }
    else if(b == 0)
    {
      gtk_list_store_set(store_r, &iter, 7, &colorB, -1);
      if(acumulado[i] > range_b)
        b = 1;
    }
    else if(c == 0)
    {
      gtk_list_store_set(store_r, &iter, 7, &colorC, -1);
      if(acumulado[i] > range_c)
        c = 1;
    }
  }

  for(i = 0; i < TD_len; ++i)
    free(name[i]);    
}

static void signal_toogle_button_state(GtkWidget *w, gpointer ptr)
{
  w = NULL;
  GtkBuilder *builder = (GtkBuilder *) ptr;
  GtkWidget *toggle = GTK_WIDGET(gtk_builder_get_object(builder, "main_toggle_button_color"));
  gboolean state = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(toggle));
  GtkWidget *view = GTK_WIDGET(gtk_builder_get_object(builder, "TR_view"));
  GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(view));
  GtkListStore *store = (GtkListStore *) model;

  int TR_len = gtk_tree_model_iter_n_children(model, NULL);
  if(TR_len < 3)
    return ;

  GtkWidget *cbtna = GTK_WIDGET(gtk_builder_get_object(builder, "settings_color_button_a"));
  GtkWidget *cbtnb = GTK_WIDGET(gtk_builder_get_object(builder, "settings_color_button_b"));
  GtkWidget *cbtnc = GTK_WIDGET(gtk_builder_get_object(builder, "settings_color_button_c"));

  GtkWidget *sbtna = GTK_WIDGET(gtk_builder_get_object(builder, "settings_spin_button_a"));
  GtkWidget *sbtnb = GTK_WIDGET(gtk_builder_get_object(builder, "settings_spin_button_b"));
  GtkWidget *sbtnc = GTK_WIDGET(gtk_builder_get_object(builder, "settings_spin_button_c"));

  GdkRGBA colorA, colorB, colorC;
  gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(cbtna), &colorA);
  gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(cbtnb), &colorB);
  gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(cbtnc), &colorC);

  double range_a = gtk_spin_button_get_value(GTK_SPIN_BUTTON(sbtna));
  double range_b = gtk_spin_button_get_value(GTK_SPIN_BUTTON(sbtnb));
  double range_c = gtk_spin_button_get_value(GTK_SPIN_BUTTON(sbtnc));

  int a = 0, b = 0, c = 0;

 
  int i;
  double acumulado[TR_len];

  for(i = 0; i < TR_len; ++i)
  {
    GtkTreePath *path = gtk_tree_path_new_from_indices(i, -1);
    GtkTreeIter iter;
    gtk_tree_model_get_iter(model, &iter, path);
    gchar *col_ac;
    gtk_tree_model_get(model, &iter, 6, &col_ac, -1);
    acumulado[i] = atof(col_ac);
    g_free(col_ac);
  }
  for(i = 0; i < TR_len; ++i)
  {
    GtkTreePath *path = gtk_tree_path_new_from_indices(i, -1);
    GtkTreeIter iter;
    gtk_tree_model_get_iter(model, &iter, path);
    
    if(a == 0)
    {
      gtk_list_store_set(store, &iter, 7, 
                         (state == TRUE)? &colorA : 0 , -1);
      if(acumulado[i] > range_a)
        a = 1;
    }
    else if(b == 0)
    {
      gtk_list_store_set(store, &iter, 7, 
                         (state == TRUE)? &colorB : 0, -1);
      if(acumulado[i] > range_b)
        b = 1;
    }
    else if(c == 0)
    {
      gtk_list_store_set(store, &iter, 7, 
                         (state == TRUE)? &colorC : 0, -1);
      if(acumulado[i] > range_c)
        c = 1;
    }
    else
    {
      gtk_list_store_set(store, &iter, 7, 0, -1);
    }
  }
}


