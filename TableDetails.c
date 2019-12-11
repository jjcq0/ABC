#include "TableDetails.h"
#include <stdlib.h>

#define GNUPLOT_PATH "gnuplot/data.dat"
#define GNUPLOT_IMG  "gnuplot/abc.png"

static void signal_view_details(GtkWidget *w, gpointer ptr);
static void signal_view_graphics(GtkWidget *w, gpointer ptr);

void TDE_configure(GtkBuilder *builder)
{
  GtkWidget *view  = GTK_WIDGET(gtk_builder_get_object(builder, "TDE_view"));
  GtkListStore *store = gtk_list_store_new(6, G_TYPE_STRING,
                                              G_TYPE_STRING,
                                              G_TYPE_STRING,
                                              G_TYPE_STRING,
                                              G_TYPE_STRING,
                                              G_TYPE_STRING);
  GtkTreeModel *model = GTK_TREE_MODEL(store);
  GtkCellRenderer *renderer;
  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_insert_column_with_attributes(
                                     GTK_TREE_VIEW(view),
                                     -1,
                                     "Participacion Estimada",
                                     renderer,
                                     "text", 0,
                                     NULL);  
  gtk_tree_view_insert_column_with_attributes(
                                     GTK_TREE_VIEW(view),
                                     -1,
                                     "Clasificacion de N",
                                     renderer,
                                     "text", 1,
                                     NULL);  
  gtk_tree_view_insert_column_with_attributes(
                                     GTK_TREE_VIEW(view),
                                     -1,
                                     "N",
                                     renderer,
                                     "text", 2,
                                     NULL); 
  gtk_tree_view_insert_column_with_attributes(
                                     GTK_TREE_VIEW(view),
                                     -1,
                                     "Participacion N",
                                     renderer,
                                     "text", 3,
                                     NULL);
  gtk_tree_view_insert_column_with_attributes(
                                     GTK_TREE_VIEW(view),
                                     -1,
                                     "Ventas",
                                     renderer,
                                     "text", 4,
                                     NULL); 
  gtk_tree_view_insert_column_with_attributes(
                                     GTK_TREE_VIEW(view),
                                     -1,
                                     "Participacion Ventas",
                                     renderer,
                                     "text", 5,
                                     NULL); 
  gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);

  GtkWidget *button_details  = GTK_WIDGET(gtk_builder_get_object(builder, "main_button_details"));
  g_signal_connect(G_OBJECT(button_details), "clicked", G_CALLBACK(signal_view_details), builder);

  GtkWidget *button_graphics = GTK_WIDGET(gtk_builder_get_object(builder, "main_button_graphics"));
  g_signal_connect(G_OBJECT(button_graphics), "clicked", G_CALLBACK(signal_view_graphics), builder);
}

static void signal_view_details(GtkWidget *w, gpointer ptr)
{
  int i;
  w = NULL;
  GtkBuilder *builder = (GtkBuilder *) ptr;
  GtkWidget *TDE_view  = GTK_WIDGET(gtk_builder_get_object(builder, "TDE_view"));
  GtkTreeModel *TDE_model = gtk_tree_view_get_model(GTK_TREE_VIEW(TDE_view));
  GtkListStore *TDE_store = (GtkListStore *) TDE_model;

  GtkWidget *TR_view  = GTK_WIDGET(gtk_builder_get_object(builder, "TR_view"));
  GtkTreeModel *TR_model = gtk_tree_view_get_model(GTK_TREE_VIEW(TR_view));
//  GtkListStore *TR_store = (GtkListStore *) TR_model;

  int TR_len = gtk_tree_model_iter_n_children(TR_model, NULL);
  if(TR_len < 3) 
    return ;
  gtk_list_store_clear(GTK_LIST_STORE(TDE_store));

  GtkWidget *sbtna = GTK_WIDGET(gtk_builder_get_object(builder, "settings_spin_button_a"));
  GtkWidget *sbtnb = GTK_WIDGET(gtk_builder_get_object(builder, "settings_spin_button_b"));
  GtkWidget *sbtnc = GTK_WIDGET(gtk_builder_get_object(builder, "settings_spin_button_c"));

  double range_a = gtk_spin_button_get_value(GTK_SPIN_BUTTON(sbtna));
  double range_b = gtk_spin_button_get_value(GTK_SPIN_BUTTON(sbtnb));
  double range_c = gtk_spin_button_get_value(GTK_SPIN_BUTTON(sbtnc));

  int a = 0, b = 0, c = 0;  
  
  double acumulado[TR_len]; 
  double ventas[TR_len];
  char *names[TR_len];

  double sum_ventas = 0;
  int n_a = 0;
  int n_b = 0;
  int n_c = 0;

  double sum_ventas_a = 0;
  double sum_ventas_b = 0;
  double sum_ventas_c = 0;

  for(i = 0; i < TR_len; ++i)
  {
    GtkTreePath *path = gtk_tree_path_new_from_indices(i, -1);
    GtkTreeIter iter;
    gtk_tree_model_get_iter(TR_model, &iter, path);
    gchar *col_ac;
    gchar *col_ven;
    gchar *col_name;

    gtk_tree_model_get(TR_model, &iter, 6, &col_ac, 4, &col_ven, 1, &col_name, -1);
  
    acumulado[i] = atof(col_ac);
    ventas[i] = atof(col_ven);
    sum_ventas += ventas[i];
    names[i] = strdup(col_name);

    g_free(col_ac);
    g_free(col_ven);
    g_free(col_name);

    if(a == 0)
    {
      ++n_a;
      sum_ventas_a += ventas[i];
      if(acumulado[i] > range_a)
        a = 1;
    }
    else if(b == 0)
    {
      ++n_b;
      sum_ventas_b += ventas[i];
      if(acumulado[i] > range_b)
        b = 1;
    }
    else if(c == 0)
    {
      ++n_c;
      sum_ventas_c += ventas[i];
      if(acumulado[i] > range_c)
        c = 1;
    }
  }
  
  char str_row1[50];
  char str_row2[50];
  char str_row3[50];
  
  char str_row1_n[50];
  char str_row2_n[50];
  char str_row3_n[50];

  char str_row1_pn[50];
  char str_row2_pn[50];
  char str_row3_pn[50];

  char str_row1_v[50];
  char str_row2_v[50];
  char str_row3_v[50];

  char str_row1_pv[50];
  char str_row2_pv[50];
  char str_row3_pv[50];
    
  sprintf(str_row1, "%.02lf - %.02lf",0.0, range_a);
  sprintf(str_row2, "%.02lf - %.02lf", range_a + 1, range_b);
  sprintf(str_row3, "%.02lf - %.02lf", range_b + 1, range_c);

  sprintf(str_row1_n, "%i", n_a);
  sprintf(str_row2_n, "%i", n_b);
  sprintf(str_row3_n, "%i", n_c);

  sprintf(str_row1_pn, "%.02lf %%", (double)n_a/(double)TR_len*100);
  sprintf(str_row2_pn, "%.02lf %%", (double)n_b/(double)TR_len*100);
  sprintf(str_row3_pn, "%.02lf %%", (double)n_c/(double)TR_len*100);

  sprintf(str_row1_v, "%.02lf", sum_ventas_a);
  sprintf(str_row2_v, "%.02lf", sum_ventas_b);
  sprintf(str_row3_v, "%.02lf", sum_ventas_c);

  sprintf(str_row1_pv, "%.02lf %%", (sum_ventas_a)/sum_ventas * 100);
  sprintf(str_row2_pv, "%.02lf %%", (sum_ventas_b)/sum_ventas * 100);
  sprintf(str_row3_pv, "%.02lf %%", (sum_ventas_c)/sum_ventas * 100);
  

  GtkTreeIter iter;
  gtk_list_store_append(TDE_store, &iter);
  gtk_list_store_set(TDE_store, &iter, 
                     0, str_row1,
                     1, "A",
                     2, str_row1_n,
                     3, str_row1_pn,
                     4, str_row1_v,
                     5, str_row1_pv,
                     -1); gtk_list_store_append(TDE_store, &iter);
  gtk_list_store_set(TDE_store, &iter, 
                     0, str_row2,
                     1, "B", 
                     2, str_row2_n,
                     3, str_row2_pn,
                     4, str_row2_v,
                     5, str_row2_pv,
                     -1); gtk_list_store_append(TDE_store, &iter);
  gtk_list_store_set(TDE_store, &iter, 
                     0, str_row3,
                     1, "C", 
                     2, str_row3_n,
                     3, str_row3_pn,
                     4, str_row3_v,
                     5, str_row3_pv,
                     -1);

  FILE *f = fopen(GNUPLOT_PATH, "w+");
  if(!f)
  {
    printf("Error FILE OPEN\n");
    return ;
  }

  for(i = 0; i < TR_len; ++i)

    fprintf(f, "%i %s %lf\n", i+1, names[i], ventas[i]);
  fclose(f);
  for(i = 0; i < TR_len; ++i)
    free(names[i]);
}  

static void signal_view_graphics(GtkWidget *w, gpointer ptr)
{
  w = NULL;
  GtkBuilder *builder = (GtkBuilder *) ptr; 
  GtkWidget *view = GTK_WIDGET(gtk_builder_get_object(builder, "TDE_view"));
  GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(view));
  int len = gtk_tree_model_iter_n_children(model, NULL);
  if(len < 3)
    return ;

  system("./generate_image.sh");
  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  GtkWidget *img = gtk_image_new_from_file(GNUPLOT_IMG);
  gtk_container_add(GTK_CONTAINER(window), img);
  gtk_widget_show_all(window);
}

