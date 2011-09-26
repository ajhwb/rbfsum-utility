/*
 * Convert RbfSum file to XML
 *
 * Copyright (C) 2011 - Ardhan Madras <ajhwb@knac.com> 
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <glib.h>

#include "station.h"
#include "queue.h"


int main(int argc, char **argv)
{
    char            *progname, *tmp, *buffer, *station_name;
    char            line[2048];
    int             ret, i, j, found, hour_id, type;
    double          total_data, total_gap, lat, lng;
    struct stat     stbuf;
    FILE            *fp;
    station_data_t  *station;
    queue_t         *list, *tmp_list;
    GKeyFile        *keyfile;

    progname = argv[0];
    if ((tmp = strrchr(progname, '/')))
        progname = ++tmp;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s config-file rbfsum-file\n", progname);
        exit(1);
    }

    keyfile = g_key_file_new();
    ret = g_key_file_load_from_file(keyfile, argv[1], G_KEY_FILE_NONE, NULL);
    if (ret == 0) {
        fprintf(stderr, "%s: could not read configuration file\n", progname);
        exit(1);
    }

    ret = stat(argv[2], &stbuf);
    if (ret == -1) {
        perror(argv[1]);
        exit(1);
    }

    if (stbuf.st_size <= 0)
        exit(1);

    fp = fopen(argv[2], "r");
    if (!fp) {
        perror(argv[1]);
        exit(1);
    }

    buffer = malloc(stbuf.st_size + 1);
    if (!buffer) {
        perror(progname);
        exit(0);
    }

    i = 0;
    while ((ret = fgetc(fp)) != EOF) {
        *(buffer + i) = ret;
        i++;
    }
    *(buffer + i) = '\0';

    list = read_rbfsum2(buffer);

    if (!list)
        fprintf(stderr, "%s: invalid file format\n", progname);
    else {
        puts("<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>");
        puts("<markers>");

        tmp_list = list;
        i = 0;

        while (tmp_list) {
            station = tmp_list->data;

            j = 0;
            found = 0;
            if (!g_key_file_has_group(keyfile, station->name)) {
                tmp_list = tmp_list->next;
                continue;
            }

            if (!g_key_file_get_integer(keyfile, station->name, "status", NULL)) {
                tmp_list = tmp_list->next;
                continue;
            }

            station_name = g_key_file_get_string(keyfile, station->name, "name", NULL);
            lat = g_key_file_get_double(keyfile, station->name, "lat", NULL);
            lng = g_key_file_get_double(keyfile, station->name, "lng", NULL);

            sprintf(line, "  <marker lat=\"%.10g\" lng=\"%.10g\" region=\"%s (%s)\" "
                    "latitude=\"Latitude: %.10g\" longitude=\"Longitude: %.10g\" ",
                    lat, lng, station_name, station->name, lat, lng);
            g_free(station_name);
            fwrite(line, strlen(line), 1, stdout);

            int x = 0;
            hour_id = 0;

            total_data = 0;
            total_gap = 0;

            if (station->ndata == 24)
                for (x = 0; x < 24; x++) {
                    total_data += station->rs[x].data;
                    total_gap += station->rs[x].gap;
                }

#if 0
            for (; x < 9; x++, hour_id++) {
                total_data += station->rs[hour_id].data;
                total_gap += station->rs[hour_id].gap;

                lt = localtime(&station->rs[hour_id].start);
                if (station->rs[hour_id].data == 3600.0 || station->rs[hour_id].data == 0.0)
                    sprintf(data, "%.1f", station->rs[hour_id].data);
                else
                    sprintf(data, "%.3f", station->rs[hour_id].data);
                sprintf(breaks, "%i", station->rs[hour_id].breaks);
                if (station->rs[hour_id].gap == 3600.0 || station->rs[hour_id].gap == 0.0)
                    sprintf(gap, "%.1f", station->rs[hour_id].gap);
                else
                    sprintf(gap, "%.3f", station->rs[hour_id].gap);

                sprintf(line, "ket%i=\"%.4i-%.2i-%.2i_%.2i:%.2i      %s         "
                        "%.1f          %s          %s\" ", 
                        x + 1, lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday,
                        lt->tm_hour, lt->tm_min, data,
                        station->rs[hour_id].retx, gap,
                        station->rs[hour_id].breaks ? breaks : "-");
                fwrite(line, strlen(line), 1, stdout);

                sprintf(line, "ket%i_data=\"%.4i-%.2i-%.2i_%.2i:%.2i,%s,%.1f,%s,%s\" ", 
                        x + 1, lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday,
                        lt->tm_hour, lt->tm_min, data,
                        station->rs[hour_id].retx, gap,
                        station->rs[hour_id].breaks ? breaks : "-");
                fwrite(line, strlen(line), 1, stdout);
            }

            for (x = 0; x < 9; x++, hour_id++) {
                total_data += station->rs[hour_id].data;
                total_gap += station->rs[hour_id].gap;

                lt = localtime(&station->rs[hour_id].start);
                if (station->rs[hour_id].data == 3600.0 || station->rs[hour_id].data == 0.0)
                    sprintf(data, "%.1f", station->rs[hour_id].data);
                else
                    sprintf(data, "%.3f", station->rs[hour_id].data);
                sprintf(breaks, "%i", station->rs[hour_id].breaks);
                if (station->rs[hour_id].gap == 3600.0 || station->rs[hour_id].gap == 0.0)
                    sprintf(gap, "%.1f", station->rs[hour_id].gap);
                else
                    sprintf(gap, "%.3f", station->rs[hour_id].gap);

                sprintf(line, "ket_%i=\"%.4i-%.2i-%.2i_%.2i:%.2i      %s         "
                        "%.1f          %s          %s\" ", 
                        x + 1, lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday,
                        lt->tm_hour, lt->tm_min, data,
                        station->rs[hour_id].retx, gap,
                        station->rs[hour_id].breaks ? breaks : "-");
                fwrite(line, strlen(line), 1, stdout);

                sprintf(line, "ket_%i_data=\"%.4i-%.2i-%.2i_%.2i:%.2i,%s,%.1f,%s,%s\" ", 
                        x + 1, lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday,
                        lt->tm_hour, lt->tm_min, data,
                        station->rs[hour_id].retx, gap,
                        station->rs[hour_id].breaks ? breaks : "-");
                fwrite(line, strlen(line), 1, stdout);
            }

            for (x = 0; x < 6; x++, hour_id++) {
                total_data += station->rs[hour_id].data;
                total_gap += station->rs[hour_id].gap;

                lt = localtime(&station->rs[hour_id].start);
                if (station->rs[hour_id].data == 3600.0 || station->rs[hour_id].data == 0.0)
                    sprintf(data, "%.1f", station->rs[hour_id].data);
                else
                    sprintf(data, "%.3f", station->rs[hour_id].data);
                sprintf(breaks, "%i", station->rs[hour_id].breaks);
                if (station->rs[hour_id].gap == 3600.0 || station->rs[hour_id].gap == 0.0)
                    sprintf(gap, "%.1f", station->rs[hour_id].gap);
                else
                    sprintf(gap, "%.3f", station->rs[hour_id].gap);

                sprintf(line, "ket__%i=\"%.4i-%.2i-%.2i_%.2i:%.2i      %s         "
                        "%.1f          %s          %s\" ", 
                        x + 1, lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday,
                        lt->tm_hour, lt->tm_min, data,
                        station->rs[hour_id].retx, gap,
                        station->rs[hour_id].breaks ? breaks : "-");
                fwrite(line, strlen(line), 1, stdout);

                sprintf(line, "ket__%i_data=\"%.4i-%.2i-%.2i_%.2i:%.2i,%s,%.1f,%s,%s\" ", 
                        x + 1, lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday,
                        lt->tm_hour, lt->tm_min, data,
                        station->rs[hour_id].retx, gap,
                        station->rs[hour_id].breaks ? breaks : "-");
                fwrite(line, strlen(line), 1, stdout);
            }

            sprintf(line, "ket__7=\"Hour starting:       Data (sec)    "
                    "ReTx(%%)    Gaps (sec)    Breaks\" ");
            fwrite(line, strlen(line), 1, stdout);
#endif

            ret = rbfsum_get_status(station->rs, 24);
            if (ret == RBFSUM_STATUS_ON)
                tmp = "1.png";
            else if (ret == RBFSUM_STATUS_GREATER)
                tmp = "2.png";
            else if (ret == RBFSUM_STATUS_EQUAL)
                tmp = "3.png";
            else if (ret == RBFSUM_STATUS_LESS)
                tmp = "4.png";
            else
                tmp = "5.png";

            sprintf(line, "icon=\"images/%s\" ", tmp);
            fwrite(line, strlen(line), 1, stdout);

            sprintf(line, "data=\"%g\" ", total_data);
            fwrite(line, strlen(line), 1, stdout);

            sprintf(line, "gap=\"%g\" ", total_gap);
            fwrite(line, strlen(line), 1, stdout);

            sprintf(line, "station=\"%s\" ", station->name);
            fwrite(line, strlen(line), 1, stdout);

            type = g_key_file_get_integer(keyfile, station->name, "type", NULL);
            sprintf(line, "type=\"%i\" />", type);
            puts(line);

            tmp_list = tmp_list->next;
        }
        fputs("</markers>", stdout);
    }

    free(buffer);
    fclose(fp);

    exit(0);
}

