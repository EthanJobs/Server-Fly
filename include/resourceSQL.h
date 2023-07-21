#ifndef _RESOURCESQL_H
#define _RESOURCESQL_H

#include <mysql/mysql.h>

MYSQL *MYSQL_init(const char *HOST, const char *USERNAME, const char *PASSWORD, const char *DATABASE, unsigned int timeout);
void sendSqlRes(MYSQL *sqlConnection, const char *sql, int fd);
MYSQL_RES *getSqlRes(MYSQL *sqlConnection, const char *sql);
void MYSQL_free(MYSQL *sqlConnection);

#endif

/*
 *  MYSQL *MYSQL_init(const char *HOST, const char *USERNAME, const char *PASSWORD, const char *DATABASE, unsigned int timeout);
 *      创建一个相应的MYSQL
 *
 *  void sendSqlRes(MYSQL *sqlConnection, const char *sql, int fd);
 *      在sqlConnection所对应的数据库中执行sql语言并将其输出到fd中
 *
 *  MYSQL_RES *getSqlRes(MYSQL *sqlConnection, const char *sql);
 *      在sqlConnection所对应的数据库中执行sql语言并将其返回,失败返回NULL
 *
 *  void MYSQL_free(MYSQL *sqlConnection);
 *      释放sqlConnection
 */
