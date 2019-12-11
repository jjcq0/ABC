#include "SQL.h"
#include <string.h>

int ABC_get_data(SQL *sql, SQL_Result *r, GtkWidget *view)
{
  GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(view));
  GtkListStore *store = (GtkListStore *) model;
  
  MYSQL *conn;
  MYSQL_RES *res;
  MYSQL_ROW row;

  conn = mysql_init(NULL);
  if (!mysql_real_connect(conn, sql->hostname, 
                                sql->user, 
                                sql->password, 
                                sql->namedb, 0, NULL, 0))
	{
		r->error = strdup(mysql_error(conn));
    mysql_close(conn); 
	  return -1;	
  }
  char cmd[50];
  sprintf(cmd, "SELECT * FROM %s", sql->table);
	if (mysql_query(conn, cmd))
	{
		r->error = strdup(mysql_error(conn));
    mysql_close(conn);
		return -1;
	}
  
  res = mysql_use_result(conn);
  while((row = mysql_fetch_row(res)) != NULL)
  {
    GtkTreeIter iter;
    gtk_list_store_append(store, &iter); 
    gtk_list_store_set(store, &iter,
                      0, row[0],
                      1, row[1],
                      2, row[2],
                      -1);
  }
	mysql_free_result(res);
  mysql_close(conn);
  return 0;
}

int SQL_get_tables_from_db(SQL *sql, SQL_Result *r)
{
  MYSQL *conn;
  MYSQL_RES *res;
  MYSQL_ROW row;

  conn = mysql_init(NULL);
  if (!mysql_real_connect(conn, sql->hostname, 
                                sql->user, 
                                sql->password, 
                                sql->namedb, 0, NULL, 0))
	{
		r->error = strdup(mysql_error(conn));
    mysql_close(conn); 
	  return -1;	
  }

	if (mysql_query(conn, "show tables"))
	{
		r->error = strdup(mysql_error(conn));
    mysql_close(conn);
		return -1;
	}
  
  res = mysql_use_result(conn);
  while((row = mysql_fetch_row(res)) != NULL)
  {
    r->data[r->len++] = strdup(row[0]);
    r->data = (char **) realloc(r->data, sizeof(char *) * r->len + 1);
  }

	mysql_free_result(res);
  mysql_close(conn);
  return 0;
}

SQL_Result *SQL_Result_create()
{
  SQL_Result *r = (SQL_Result *) malloc(sizeof(SQL_Result));
  r->data = (char **) malloc(sizeof(char *));
  r->len = 0;
  r->error = NULL;
  return r;
}

void SQL_Result_destroy(SQL_Result *r)
{
  int i;
  for(i = 0; i < r->len; ++i)
    free(r->data[i]);
  if(r->error)
    free(r->error);
  free(r);
}

SQL *SQL_create(const char *hostname, const char *namedb, 
                const char *user, const char *pass)
{
  SQL *sql = (SQL*) malloc(sizeof(SQL));
  sql->hostname = strdup(hostname);
  sql->namedb = strdup(namedb);
  sql->password = strdup(pass);
  sql->user = strdup(user);
  sql->table = NULL;
  return sql;
}
void SQL_destroy(SQL *sql)
{
  free(sql->namedb);
  free(sql->user);
  free(sql->password);
  free(sql->hostname);
  if(sql->table)
    free(sql->table);
  free(sql);
}

