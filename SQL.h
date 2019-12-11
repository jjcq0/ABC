#ifndef _SQL_H_
#define _SQL_H_

#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
  char **data;
  int len;
  char *error;
} SQL_Result;

typedef struct
{
  char *hostname;
  char *namedb;
  char *user;
  char *password;
  char *table;
} SQL;

SQL_Result *SQL_Result_create();
void SQL_Result_destroy(SQL_Result *r);
SQL *SQL_create(const char *hostname, const char *namedb, 
                const char *user, const char *pass);
void SQL_destroy(SQL *sql);
int SQL_get_tables_from_db(SQL *sql, SQL_Result *r);

#include <gtk/gtk.h>

int ABC_get_data(SQL *sql, SQL_Result *r, GtkWidget *view);

#endif
