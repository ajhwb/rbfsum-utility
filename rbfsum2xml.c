/*
 * Convert rbfsum file to xml
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "station.h"
#include "queue.h"

#define to_stderr(line) fwrite(line, strlen(line), 1, stderr)

int main(int argc, char **argv)
{
    char            *progname, *tmp, *buffer;
    char            line[2048];
    int             ret, i, j, found, hour_id;
    double          total_data, total_gap;
    struct stat     stbuf;
    struct tm       *lt;
    FILE            *fp;
    station_data_t  *station;
    queue_t         *list, *tmp_list;

    progname = argv[0];
    if ((tmp = strrchr(progname, '/')))
        progname = ++tmp;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s rbfsum-file\n", progname);
        exit(1);
    }

    ret = stat(argv[1], &stbuf);
    if (ret == -1) {
        perror(argv[1]);
        exit(1);
    }

    if (stbuf.st_size <= 0)
        exit(1);

    fp = fopen(argv[1], "r");
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

            if (station->ndata < 24) {
                tmp_list = tmp_list->next;
                continue;
            }

            j = 0;
            found = 0;
            while (station_info[j].name) {
                if (!strcmp(station_info[j].name, station->name) && station_info[j].state) {
                    found = 1;
                    break;
                }
                j++;
            }

            if (!found) {
                tmp_list = tmp_list->next;
                continue;
            }

            sprintf(line, "  <marker lat=\"%.10g\" lng=\"%.10g\" region=\"%s (%s)\" "
                    "latitude=\"Latitude: %.10g\" longitude=\"Longitude: %.10g\" ",
                    station_info[j].lat, station_info[j].lng, station_info[j].desc,
                    station_info[j].name, station_info[j].lat, station_info[j].lng);
            fwrite(line, strlen(line), 1, stdout);

            int x = 0;
            char data[9], breaks[6], gap[9];
            hour_id = 0;

            total_data = 0;
            total_gap = 0;

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

            sprintf(line, "station=\"%s\" />", station_info[j].name);
            puts(line);

            tmp_list = tmp_list->next;
        }
        fputs("</markers>", stdout);
    }

    free(buffer);
    fclose(fp);

    exit(0);
}

