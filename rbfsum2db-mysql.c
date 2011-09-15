/*
 * Convert rbfsum data to Libra web backend database
 * Copyright (C) 2011 - Ardhan Madras <ajhwb@knac.com>
 */

#include <sys/stat.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <glib.h>
#include <mysql/mysql.h>
#include "queue.h"
#include "station.h"

#define PROGRAM      "rbfsum2db"
#define DESC         "Convert rbfsum data to Libra web backend database"
#define VERSION      "1.0"
#define AUTHOR       "Ardhan Madras <ajhwb@knac.com>"
#define CONFIG_FILE  "/etc/rbfsum2db.conf"

struct config {
    char *host;
    char *username;
    char *password;
    char *database;
    unsigned short port;
};

static struct config *config;

void die(int exit_code, const char *msg)
{
    fprintf(stderr, "%s", msg);
    exit(exit_code);
}

void die_with_error(int err)
{
    switch (err) {
        case 0:
            die(0, "success\n");
        case 1:
            die(1, "invalid command line option\n");
        case 2:
            die(2, "could not read/parse configuration file\n");
        case 3:
            die(3, "could not connect to database\n");
        case 4:
            die(4, "could not open file\n");
        case 5:
            die(5, "could not parse file\n");
        case 6:
            die(6, "could not execute sql query\n");
        case 255:
            die(255, "general error\n");
        default:
            die(254, "unknown error\n");
    }
}

void usage()
{
    fprintf(stderr, "%s v%s - %s\nCopyright (C) 2011 - %s\n\n"
            "Usage: %s [-c config-file] rbfsum-file\n"
            "       -c, configuration file to read, default is %s\n"
            "       -h, show this message\n\n", 
            PROGRAM, VERSION, DESC, AUTHOR, PROGRAM, CONFIG_FILE);
    fprintf(stderr, "Exit code:\n"
            "0    success\n"
            "1    invalid command line option\n"
            "2    could not read/parse configuration file\n"
            "3    could not connect to database\n"
            "4    could not open file\n"
            "5    could not parse file\n"
            "6    could not execute sql query\n"
            "255  general error\n\n");
    exit(1);
}

int parse_config(const char *config_file)
{
    GKeyFile *key = g_key_file_new();
    char *host, *username, *password, *database;
    unsigned short port;

    if (g_key_file_load_from_file(key, config_file, G_KEY_FILE_NONE, NULL) == 0)
        return 0;

    host = username = password = database = NULL;

    host = g_key_file_get_string(key, "config", "host", NULL);
    if (host == NULL) goto err;
    username = g_key_file_get_string(key, "config", "username", NULL);
    if (username == NULL) goto err;
    password = g_key_file_get_string(key, "config", "password", NULL);
    if (password == NULL) goto err;
    database = g_key_file_get_string(key, "config", "database", NULL);
    if (database == NULL) goto err;
    port = g_key_file_get_integer(key, "config", "port", NULL);
    g_key_file_free(key);

    config = g_malloc(sizeof(struct config));
    config->host = host;
    config->username = username;
    config->password = password;
    config->database = database;
    config->port = port;
    return 1;

err:
    if (host != NULL) g_free(host);
    if (username != NULL) g_free(host);
    if (password != NULL) g_free(password);
    return 0;
}

char *generate_timestamp(time_t t)
{
    struct tm *st = localtime(&t);
    return g_strdup_printf ("%i-%.2i-%.2i %.2i:%.2i:%.2i",
                            st->tm_year + 1900, st->tm_mon + 1, st->tm_mday,
                            st->tm_hour, st->tm_min, st->tm_sec);
}

int insert_data(const struct config *config, queue_t *data)
{
    MYSQL *sql_object, *sql_conn;
    queue_t *ptr;
    station_data_t *station;
    rbfsum_t rs;
    int ret, i, station_id;
    const int timeout = 3;
    char *cmd, *timestamp, *breaks;

    sql_object = mysql_init(NULL);
    if (sql_object == NULL)
        return 255;

    ret = mysql_options(sql_object, MYSQL_OPT_CONNECT_TIMEOUT, &timeout);
    if (ret) {
        mysql_close(sql_object);
        return 3;
    }

    sql_conn = mysql_real_connect(sql_object, config->host, config->username,
                                  config->password, config->database, 
                                  config->port, 0, 0);
    if (sql_conn == NULL) {
        mysql_close(sql_object);
        return 3;
    }

    ptr = data;
    while (ptr) {
        station = ptr->data;
        station_id = get_station_id(station->name);
        if (station_id <= 0 
            || station_info[station_id].state == 0
            || station->ndata != 24) {
            ptr = ptr->next;
            continue;
        }
        for (i = 0; i < 24; i++) {
            rs = station->rs[i];
            timestamp = generate_timestamp(rs.start);
            breaks = rs.breaks ? g_strdup_printf("%i", rs.breaks) : g_strdup("-");
            cmd = g_strdup_printf("INSERT INTO tbldatarbf VALUES(NULL, %i, "
                                  "'%s', '%g', '%g', '%g', '%s')", station_id,
                                  timestamp, rs.data, rs.retx, rs.gap, breaks);
            ret = mysql_query(sql_object, cmd);
            g_free(breaks);
            g_free(cmd);
            g_free(timestamp);
            if (ret) {
                mysql_close(sql_object);
                return 6;
            }
        }

        switch (rbfsum_get_status(station->rs, station->ndata)) {
            case RBFSUM_STATUS_EQUAL:
                ret = 3;
                break;
            case RBFSUM_STATUS_GREATER:
                ret = 2;
                break;
            case RBFSUM_STATUS_LESS:
                ret = 4;
                break;
            case RBFSUM_STATUS_OFF:
                ret = 5;
                break;
            default:
                ret = 1;
        }
        rs = station->rs[0];
        timestamp = generate_timestamp(rs.start);
        cmd = g_strdup_printf("INSERT INTO tblstatus VALUES(NULL, %i, "
                              "'%s', %i)", station_id, timestamp, ret);
        ret = mysql_query(sql_object, cmd);
        g_free(cmd);
        g_free(timestamp);
        if (ret) {
            mysql_close(sql_object);
            return 6;
        }

        ptr = ptr->next;
    }

    mysql_close(sql_object);
    return 0;
}

int read_data(const char *file, queue_t **data)
{
    struct stat st;
    FILE *fp;
    queue_t *tmp;
    char *buffer;
    int ret, i;

    ret = stat(file, &st);
    if (ret == -1)
        return 4;

    fp = fopen(file, "r");
    if (fp == NULL)
        return 4;

    buffer = g_malloc(st.st_size + 1);
    i = 0;
    while ((ret = fgetc(fp)) != EOF) {
        buffer[i] = ret;
        i++;
    }
    buffer[i] = 0;

    fclose(fp);
    tmp = read_rbfsum2(buffer);
    g_free(buffer);

    if (tmp == NULL)
        return 5;
    *data = tmp;
    return 0;
}

int main(int argc, char **argv)
{
    int      ret, config_defined = 0;
    char     *config_file, *data_file;
    queue_t  *data;

    if (argc < 2)
        usage();

    while ((ret = getopt(argc, argv, "c:h")) != -1) {
        switch (ret) {
            case 'c':
                config_file = optarg;
                config_defined = 1;
                break;
            case 'h':
                usage();
                break;
            default:
                die(1, "use -h for help\n");
        }
    }

    if (config_defined) {
        if (argc == 2 || (argc == 3 && !strcmp(argv[1], "-c")))
            usage();
    } else
        config_file = CONFIG_FILE;
    data_file = argv[--argc];
    if (!parse_config(config_file))
        die(2, "could not parse config file\n");

    ret = read_data(data_file, &data);
    if (ret)
        die_with_error(ret);

    ret = insert_data(config, data);
    if (ret)
        die_with_error(ret);

    exit(0);
}
