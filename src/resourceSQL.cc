#include <stdio.h>
#include <resourceSQL.h>
#include <mysql/mysql.h>
#include <errno.h>

MYSQL *MYSQL_init(const char *HOST, const char *USERNAME, const char *PASSWORD, const char *DATABASE, unsigned int timeout) {
    mysql_thread_init();

    MYSQL *sqlConnection = mysql_init(NULL);
    if (sqlConnection == NULL) {
        printf("[resourceSQL ERROR]:mysql_init\n");
        return NULL;
    }

    if (mysql_options(sqlConnection, MYSQL_OPT_CONNECT_TIMEOUT, (const char*)&timeout)) {
        printf("[resourceSQL ERROR]:mysql_options\n");
        return NULL;
    }
    
    mysql_real_connect(sqlConnection, HOST, USERNAME, PASSWORD, DATABASE, 3306, NULL, CLIENT_FOUND_ROWS);
    if (sqlConnection) {
        return sqlConnection;
    } else {
        printf("[resourceSQL ERROR]:mysql_connect\n");
        printf("%s\n", mysql_error(sqlConnection));
        return NULL;
    }
}

void sendSqlRes(MYSQL *sqlConnection, const char *sql, int fd) {
    if (sqlConnection == NULL || sql == NULL || fd <= 0) return ;

    int res;
    MYSQL_RES *res_ptr;
    MYSQL_ROW res_row;
    MYSQL_FIELD *fields;
    int row, col;
    
    mysql_query(sqlConnection, "set names utf8");
    res = mysql_query(sqlConnection, sql);
    if (res) {
        printf("[resourceSQL ERROR]:mysql_query %s\n", mysql_error(sqlConnection));
    } else {
        res_ptr = mysql_store_result(sqlConnection);
        if (res_ptr) {
            row = mysql_num_rows(res_ptr);
            col = mysql_num_fields(res_ptr);

            fields = mysql_fetch_fields(res_ptr);
            dprintf(fd, "{\"affectedRows\":\"%ld\", \"fields\":[", mysql_affected_rows(sqlConnection));
            for (int i = 0; i < col; i++) {
                if (i) dprintf(fd, ",");
                dprintf(fd, "\"%s\"", fields[i].name);
            }
            dprintf(fd, "],\"data\":[");
            for (int i = 0; i < row; i++) {
                if (i) dprintf(fd, ",");
                res_row = mysql_fetch_row(res_ptr);
                dprintf(fd, "[");
                for (int j = 0; j < col; j++) {
                    if (j) dprintf(fd, ",");
                    dprintf(fd, "\"%s\"", res_row[j]);
                }
                dprintf(fd, "]");    
            }
            dprintf(fd, "]}");
        }
        mysql_free_result(res_ptr);
    }
}

MYSQL_RES *getSqlRes(MYSQL *sqlConnection, const char *sql) {
    if (sqlConnection == NULL || sql == NULL) return NULL;

    int res;
    MYSQL_RES *res_ptr;
    
    mysql_query(sqlConnection, "set names utf8");
    res = mysql_query(sqlConnection, sql);
    if (res) {
        printf("[resourceSQL ERROR]:mysql_query %s\n", mysql_error(sqlConnection));
    } else {
        res_ptr = mysql_store_result(sqlConnection);
        return res_ptr;
    }
    return NULL;
}

void MYSQL_free(MYSQL *sqlConnection) {
    mysql_close(sqlConnection);
    mysql_thread_end();
}
