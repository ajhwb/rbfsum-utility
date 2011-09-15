#ifndef __STATION_H__
#define __STATION_H__

#define STATION_NUM 110

struct _rbfsum_t {
  long start;
  double data;
  double retx;
  double gap;
  int breaks;
};

typedef struct _rbfsum_t rbfsum_t;


struct _station_t {
  const char *name;
  const char *desc;
  double lat;
  double lng;
};

typedef struct _station_t station_t;

struct _station_data_t {
  char *name;
  unsigned int ndata;
  rbfsum_t rs[128];
};

typedef struct _station_data_t station_data_t;

enum _rbfsum_status_t {
  RBFSUM_STATUS_EQUAL,
  RBFSUM_STATUS_GREATER,
  RBFSUM_STATUS_LESS,
  RBFSUM_STATUS_OFF,
  RBFSUM_STATUS_ON
};

typedef enum _rbfsum_status_t rbfsum_status_t;

extern const station_t station_info[STATION_NUM];

int lookup_station (const char*);
bool read_rbfsum (char*, const char*, rbfsum_t[], unsigned int*);
rbfsum_status_t rbfsum_get_status (rbfsum_t[], unsigned int);

#endif
