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
#include <libpq-fe.h>
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
            "Usage: %s [-c config] file ...\n"
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

static int fetch_station_id(const PGresult *res, const char *code)
{
    int rows, cols, i;

    rows = PQntuples(res);
    cols = PQnfields(res);

    for (i = 0; i < rows; i++) {
        if (!strcmp(code, PQgetvalue(res, i, 0)))
            return atoi(PQgetvalue(res, i, 1));
    }
    return 0;
}

int insert_data(const struct config *config, queue_t *data)
{
    PGconn *conn;
    PGresult *res, *station_res;
    queue_t *ptr;
    station_data_t *station;
    rbfsum_t rs;
    int ret, i, station_id;
    char *cmd, *timestamp, *breaks;

    cmd = g_strdup_printf("hostaddr=%s port=%i dbname=%s user=%s password=%s "
                          "connect_timeout=2", config->host, config->port,
                          config->database, config->username, config->password);

    conn = PQconnectdb(cmd);
    g_free(cmd);
    ret = PQstatus(conn);
    if (ret != CONNECTION_OK) {
        PQfinish(conn);
        return 3;
    }

    station_res = PQexec(conn, "SELECT stasiun_kode,stasiun_id FROM tblstasiun "
                         "WHERE stasiun_hide='0' ORDER BY stasiun_kode");
    ret = PQresultStatus(station_res);
    if (ret != PGRES_TUPLES_OK) {
        PQfinish(conn);
        return 6;
    }

    ptr = data;
    while (ptr) {
        station = ptr->data;
        station_id = fetch_station_id(station_res, station->name);
        if (station_id <= 0 || station->ndata != 24) {
            ptr = ptr->next;
            continue;
        }
        for (i = 0; i < 24; i++) {
            rs = station->rs[i];
            timestamp = generate_timestamp(rs.start);
            breaks = rs.breaks ? g_strdup_printf("%i", rs.breaks) : g_strdup("-");

            cmd = g_strdup_printf("SELECT insert_data_rbf(%i, '%s', %g, %g, %g, '%s')",
                                 station_id, timestamp, rs.data, rs.retx, rs.gap, breaks);
            res = PQexec(conn, cmd);
            g_free(breaks);
            g_free(cmd);
            g_free(timestamp);
            if (!res) {
                PQclear(station_res);
                PQfinish(conn);
                return 6;
            }
            ret = PQresultStatus(res);
            PQclear(res);
            if (ret != PGRES_TUPLES_OK) {
                PQclear(station_res);
                PQfinish(conn);
                return 6;
            }
        }
        ptr = ptr->next;
    }

    timestamp = generate_timestamp(rs.start);
    *(timestamp + 10) = 0;
    cmd = g_strdup_printf("SELECT func_insert_percentage('%s')", timestamp);
    res = PQexec(conn, cmd);
    g_free(timestamp);
    g_free(cmd);

    ret = PQresultStatus(res);
    if (ret != PGRES_TUPLES_OK) {
        PQclear(station_res);
        PQfinish(conn);
        return 6;
    }

    PQclear(station_res);
    PQfinish(conn);
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

void free_func(void *data, void *user_data)
{
    free(data);
}

int main(int argc, char **argv)
{
    int      ret, config_defined = 0, i;
    char     *config_file;
    queue_t  *data;

    if (argc < 2)
        usage();

    while ((ret = getopt(argc, argv, "c:h")) != -1) {
        switch (ret) {
            case 'c':
                config_file = optarg;
                config_defined = 1;
                i = optind;
                break;
            case 'h':
                usage();
                break;
            default:
                die(1, "use -h for help\n");
        }
    }

    if (config_defined) {
        if (argc == 2  || (argc == 3 && !strcmp(argv[1], "-c")))
            usage();
    } else {
        config_file = CONFIG_FILE;
        i = 1;
    }

    if (!parse_config(config_file))
        die(2, "could not parse config file\n");

    for (; i < argc; i++) {
        puts(argv[i]);
        ret = read_data(argv[i], &data);
        if (ret)
            die_with_error(ret);

        ret = insert_data(config, data);
        if (ret)
            die_with_error(ret);
        queue_foreach(data, free_func, NULL);
        queue_free(data);
    }

    exit(0);
}
