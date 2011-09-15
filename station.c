#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "station.h"
#include "queue.h"

const station_t station_info[] = {
  {1, "AAI", "Ambon", -3.68711, 128.194, 1},
  {2, "ABJI", "Asem Bagus", -7.79564, 114.234, 1},
  {3, "ALKI", "Alor", -8.12449, 125.412, 1},
  {4, "APSI", "Ampana", -0.910782, 121.649, 1},
  {5, "BASI", "Baing", -10.2189, 120.578, 1},
  {6, "BATI", "Kupang CTBTO", -10.2065, 123.663, 1},
  {7, "BBKI", "Banjar Baru", -3.4625, 114.841, 1},
  {8, "BBSI", "Bau Bau", -5.48845, 122.569, 1},
  {9, "BKSI", "Bulukumba", -5.32184, 120.122, 1},
  {10, "BLJI", "Banyu Glugur", -7.74537, 113.595, 1},
  {11, "BLSI", "Bandar Lampung", -5.36755, 105.245, 1},
  {12, "BNSI", "Bone", -4.40052, 120.107, 1},
  {13, "BSSI", "Benteng, P. Selayar", -6.1428, 120.49, 1},
  {14, "BWJI", "Bawean", -5.8511, 112.658, 1},
  {15, "BYJI", "Banyuwangi", -8.214, 114.356, 1},
  {16, "CBJI", "Citeko", -6.69805, 106.935, 1},
  {17, "CGJI", "Cigeulis Pandeglang Banten", -6.73437, 108.263, 1},
  {18, "CMJI", "Cimerak", -7.7837, 108.449, 1},
  {19, "CNJI", "Cianjur", -7.309, 107.13, 1},
  {20, "CTJI", "Tegal", -7.00755, 109.184, 1},
  {21, "DBJI", "Dermaga", -6.5538, 106.75, 1},
  {22, "DBNI", "Bima", -8.50189, 118.312, 1},
  {23, "DNP", "Denpasar", -8.6774, 115.21, 1},
  {24, "DSRI", "Dabo Singkep", -0.4793, 104.578, 1},
  {25, "EDFI", "Ende", -8.7497, 121.69, 1},
  {26, "EGSI", "Enggano", -5.3526, 102.277, 1},
  {27, "GLMI", "Galela", 1.8381, 127.788, 1},
  {28, "GMJI", "Gumukmas", -8.27321, 113.444, 1},
  {29, "GRJI", "Gresik", -6.91443, 112.479, 1},
  {30, "JAY", "Jayapura CTBTO", -2.51451, 140.704, 1},
  {31, "JCJI", "Jatiwangi", -6.7344, 108.263, 1},
  {32, "KASI", "Kota Agung Lampung", -5.52361, 104.496, 1},
  {33, "KBKI", "Kotabaru Kalimantan", -3.2995, 116.167, 1},
  {34, "KCSI", "Kutacane", 3.52219, 97.7716, 1},
  {35, "KKSI", "Kolaka", -4.17172, 121.651, 1},
  {36, "KLI", "Kotabumi", -4.83629, 104.87, 1},
  {37, "KLNI", "Mataram", -8.42189, 116.095, 1},
  {38, "KMMI", "Kalianget", -7.04116, 113.916, 1},
  {39, "KMPI", "Kaimana", -3.6616, 133.704, 1},
  {40, "KMSI", "Kotamubago", 0.574544, 123.891, 1},
  {41, "KPJI", "Karang Pucung", -7.33319, 108.931, 1},
  {42, "KRAI", "Kai Ratu", -3.31836, 128.395, 1},
  {43, "KRJI", "Kerinci", -2.0912, 101.462, 1},
  {44, "KRK", "Karangkates", -8.15215, 112.451, 1},
  {45, "LASI", "Langsa", 4.45725, 97.9704, 1},
  {46, "LBMI", "Labuha", -0.637887, 127.501, 1},
  {47, "LEM", "Lembang CTBTO", -6.826, 107.617, 1},
  {48, "LHSI", "Lahat", -3.82662, 103.523, 1},
  {49, "LWLI", "Liwa", -5.01747, 104.059, 1},
  {50, "MASI", "Muara Aman", -3.14152, 102.24, 1},
  {51, "MDSI", "Muara Dua", -4.48604, 104.178, 1},
  {52, "MKBI", "Muko-Muko", -2.44736, 101.24, 1},
  {53, "MLSI", "Melaboh", 4.26685, 96.404, 1},
  {54, "MMPI", "Merauke", -8.51812, 140.414, 1},
  {55, "MNSI", "Mandailing Natal", 0.795498, 99.5796, 1},
  {56, "MPSI", "Mapaga", 0.3374, 119.898, 1},
  {57, "MRSI", "Marissa Gorontalo", 0.477076, 121.941, 1},
  {58, "MSAI", "Masohi", -3.34612, 128.928, 1},
  {59, "MTKI", "Muara Teweh", -0.941833, 114.896, 1},
  {60, "NBPI", "Nabire", -1.31824, 135.483, 1},
  {61, "NGJI", "Ngawi", -7.36754, 111.461, 1},
  {62, "NLAI", "Namlea, Pulau Buru", -3.23897, 127.1, 1},
  {63, "PBKI", "Pangkalan Bun", -2.7047, 111.67, 1},
  {64, "PBSI", "Pulau Batu", -0.0547, 98.28, 1},
  {65, "PCJI", "Pacitan", -8.19466, 111.177, 1},
  {66, "PDSI", "Padang", -0.911762, 100.462, 1},
  {67, "PKKI", "Palangkaraya", -2.2262, 113.945, 1},
  {68, "PMSI", "Majene", -3.50085, 118.915, 1},
  {69, "PPBI", "Pangkal Pinang", -2.16154, 106.136, 1},
  {70, "PPSI", "Pulau Pagai", -2.766, 100.01, 1},
  {71, "PSI", "Prapat CTBTO", 2.69518, 98.9239, 1},
  {72, "PWJI", "Pagerwojo", -8.02191, 111.804, 1},
  {73, "RAPI", "Raja Ampat", -0.4102, 130.821, 1},
  {74, "RBSI", "Rajabasa Lampung", -5.84445, 105.742, 0},
  {75, "RGRI", "Rengat", -0.349101, 102.334, 1},
  {76, "RKPI", "Ransiki", -1.5107, 134.177, 1},
  {77, "RPSI", "Rantau Prapat", 2.15038, 99.7881, 1},
  {78, "RTBI", "Negara", -8.4597, 114.942, 1},
  {79, "SBJI", "Serang", -6.11116, 106.132, 1},
  {80, "SBSI", "Sibolga", 1.39876, 99.4309, 1},
  {81, "SCJI", "Cilacap", -7.681, 109.169, 1},
  {82, "SDSI", "Sungai Dareh", -0.932516, 101.428, 1},
  {83, "SGKI", "Sangata", 0.530205, 117.604, 1},
  {84, "SGSI", "Sangihe", 3.68598, 125.529, 1},
  {85, "SIJI", "Sorong CTBTO", -0.9, 131.3, 1},
  {86, "SISI", "Pulau Saibi", -1.3265, 99.0895, 1},
  {87, "SKJI", "Sukabumi", -7.00527, 106.563, 1},
  {88, "SKSI", "Soroako", -2.5283, 121.335, 1},
  {89, "SLSI", "Sarolangun", -2.3924, 102.593, 1},
  {90, "SMKI", "Samarinda", -0.446136, 117.209, 1},
  {91, "SMPI", "Sarmi", -1.981, 138.711, 1},
  {92, "SNSI", "Sinabang", 2.40893, 96.3267, 1},
  {93, "SPSI", "Sidrap Palu", -3.9646, 119.769, 1},
  {94, "SRBI", "Singaraja", -8.08475, 115.213, 1},
  {95, "SRPI", "Serui", -1.87552, 136.24, 1},
  {96, "STKI", "Sintang", 0.065624, 111.477, 1},
  {97, "SWI", "Sorong", -0.862801, 131.259, 1},
  {98, "SWJI", "Sawahan", 38.3417, -95.7129, 1},
  {99, "TBJI", "Tambak Boyo", -6.8179, 111.848, 1},
  {100, "TNG", "Tangerang", -6.17194, 106.647, 1},
  {101, "TPRI", "Tanjung Pinang", 0.91836, 104.526, 1},
  {102, "TRSI", "Tarutung", 2.0256, 98.9594, 1},
  {103, "TSI", "Tuntungan", 3.50118, 98.5646, 1},
  {104, "TTSI", "Tana Toraja", -3.04508, 119.819, 1},
  {105, "TWSI", "Taliwang", -8.73818, 116.882, 1},
  {106, "UBSI", "Bengkulu", -3.7611, 102.271, 1},
  {107, "UWJI", "Ujung Watu", -6.41913, 110.947, 1},
  {108, "WBSI", "Waikabubak", -9.64112, 119.391, 1},
  {109, "WOJI", "Wonogiri", -7.83715, 110.924, 1},
  {0, 0, 0, 0.0, 0.0, 0 }
};

#define next_line0(tmp) \
({ \
  gchar *str = tmp; \
  while (*str != '\n') \
    if (*str != '\0') \
      str++; \
  if (*str != '\0') \
    str++; \
  str; \
})

static char
*xstrndup (const char *src, size_t len)
{
  char *dst = malloc (len + 1);
  if (dst != NULL)
    {
      memcpy (dst, src, len);
      dst[len] = '\0';
    }
  return dst;
}

int
lookup_station (const char *name)
{
  int i = 0;

  while (station_info[i].name)
    {
      if (!strcmp (name, station_info[i].name))
        return i;
      i++;
    }
  return -1;
}

int
get_station_id (const char *name)
{
  int i = 0;

  while (station_info[i].name)
    {
      if (!strcmp(station_info[i].name, name))
        return station_info[i].id;
      i++;
    }
  return -1;
}

inline char
*next_line (char *tmp)
{
  char *str = tmp;

  while (*str != '\n')
    if (*str != '\0')
      str++;
  if (*str != '\0')
    str++;
  return str;
}

inline char
*get_line (char *buffer)
{
  char *tmp = buffer;

  if (*tmp == '\r' || *tmp == '\n' || *tmp == '\0')
    return NULL;

  while (*tmp != '\r' && *tmp != '\n' && *tmp != '\0')
    tmp++;

  return xstrndup (buffer, tmp - buffer);
}

int
parse_data (char *buffer, rbfsum_t *data)
{
  char *start, *end;
  char *tmp;
  char year[5], month[3], date[3], hour[3];
  int y, m, d, h;
  struct tm st;

  /* Hour starting */
  start = buffer;
  while (*start != ' ')
    {
      if (*start == '\0')
        return 0;
      start++;
    }
  tmp = xstrndup (buffer, start - buffer);
  if (strlen (tmp) != 16)
    {
      free (tmp);
      return 0;
    }

  snprintf (year, sizeof(year), "%s", tmp);
  snprintf (month, sizeof(month), "%s", tmp + 5);
  snprintf (date, sizeof(date), "%s", tmp + 8);
  snprintf (hour, sizeof(hour), "%s", tmp + 11);
  free (tmp);

  if ((y = strtol (year, NULL, 10)) <= 0)
    return 0;
  if ((m = strtol (month, NULL, 10)) <= 0)
    return 0;
  if ((d = strtol (date, NULL, 10)) <= 0)
    return 0;
  if ((h = strtol (hour, NULL, 10)) < 0)
    return 0;

  memset (&st, 0, sizeof(st));
  st.tm_year = y - 1900;
  st.tm_mon = m - 1;
  st.tm_mday = d;
  st.tm_hour = h;
  data->start = mktime (&st);

  /* Data (sec) */
  while (*start == ' ')
    {
      if (*start == '\0')
        return 0;
      start++;
    }

  end = start;
  while (*end != ' ')
    {
      if (*end == '\0')
        return 0;
      end++;
    }

  tmp = xstrndup (start, end - start);
  data->data = strtod (tmp, NULL);
  free (tmp);
  if (data->data < 0.0)
    return 0;

  /* ReTx */
  start = end;
  while (*start == ' ')
    {
      if (*start == '\0')
        return 0;
      start++;
    }

  end = start;
  while (*end != ' ')
    {
      if (*end == '\0')
        return 0;
      end++;
    }

  tmp = xstrndup (start, end - start);
  data->retx = strtod (tmp, NULL);
  free (tmp);
  if (data->retx < 0)
    return 0;

  /* Gaps */
  start = end;
  while (*start == ' ')
    {
      if (*start == '\0')
        return 0;
      start++;
    }

  end = start;
  while (*end != ' ')
    {
      if (*end == '\0')
        return 0;
      end++;
    }

  tmp = xstrndup (start, end - start);
  data->gap = strtod (tmp, NULL);
  free (tmp);
  if (data->gap < 0)
    return 0;

  /* Breaks */
  start = end;
  while (*start == ' ')
    {
      if (*start == '\0')
        return 0;
      start++;
    }

  end = start;
  while (*end != '\0')
    end++;

  tmp = xstrndup (start, end - start);
  if (strcmp (tmp, "-"))
    data->breaks = strtol (tmp, NULL, 10);
  else
    data->breaks = 0;
  free (tmp);

  return 1;
}

int
read_rbfsum (char *buffer, const char *station, rbfsum_t data[96], unsigned int *ndata)
{
  char *tmp = buffer;
  char *filename;
  char *start;
  char *file_line;
  char *line;
  int test;
  int retval = 0;
  unsigned int id = 0;

  rbfsum_t rb;

  filename = malloc (strlen (station) + 6);
  if (filename == NULL)
    return 0;
  sprintf (filename, "R%s.BHZ", station);

  while (*tmp)
    {
      file_line = get_line (tmp);
      if (!file_line)
        {
          tmp = next_line (tmp);
          continue;
        }

      if (*file_line == '/')
        {
          start = strrchr (file_line, '/');
          start++;
          test = strcmp (start, filename) == 0;
          free (file_line);
          if (test)
            {
              tmp = next_line (tmp);
              if (*tmp == '\0') 
                break;

              tmp = next_line (tmp);
              if (*tmp == '\0')
                break;

              while (*tmp != '\n')
                {
                  line = get_line (tmp);
                  if (parse_data (line, &rb))
                    {
                      memcpy (&data[id], &rb, sizeof(rb));
                      id++;
                    }
                  free (line);
                  tmp = next_line (tmp);
                }
              *ndata = id;
              retval = 1;
            }
        }
      tmp = next_line (tmp);
    }

  *ndata = id;
  free (filename);

  return retval;
}

static int
comp_func (const void *a, const void *b)
{
  const station_data_t *data1 = a;
  const station_data_t *data2 = b;
  return strcmp (data1->name, data2->name);
}

queue_t
*read_rbfsum2 (char *buffer)
{
  queue_t *list = NULL;
  char *tmp = buffer;
  char *name;
  char *start, *end;
  char *file_line;
  char *line;
  rbfsum_t rb;
  unsigned int id;
  station_data_t *data;

  while (*tmp)
    {
      file_line = get_line (tmp);
      if (!file_line)
        {
          tmp = next_line (tmp);
          continue;
        }

      if (*file_line == '/')
        {
          start = strrchr (file_line, '/');
          start++;
          end = strrchr (start, '.');
          if (!end)
            {
              tmp = next_line (tmp);
              continue;
            }

          name = xstrndup (start + 1, end - start - 1);
          free (file_line);

          tmp = next_line (tmp);
          if (*tmp == '\0')
            {
              free (name);
              break;
            }

          tmp = next_line (tmp);
          if (*tmp == '\0')
            {
              free (name);
              break;
            }

          data = malloc (sizeof(station_data_t));
          data->name = name;

          id = 0;
          while ((line = get_line (tmp)))
            {
              if (parse_data (line, &rb))
                {
                  memcpy (&data->rs[id], &rb, sizeof(rb));
                  id++;
                }
              free (line);
              tmp = next_line (tmp);
            }
          data->ndata = id;
          list = queue_prepend (list, data);
        }
      tmp = next_line (tmp);
    }

  list = queue_sort (list, comp_func);
  return list;
}

rbfsum_status_t
rbfsum_get_status (rbfsum_t rs[], unsigned int ndata)
{
  double data = 0;
  double gaps = 0;
  unsigned int i = ndata;

  while (i--)
    {
      data += rs[i].data;
      gaps += rs[i].gap;
    }


  if (data >= (3600 * ndata) && gaps == 0)
    return RBFSUM_STATUS_ON;
  if (data == 0 && gaps >= (3600 * ndata))
    return RBFSUM_STATUS_OFF;
  if (data == gaps)
    return RBFSUM_STATUS_EQUAL;
  if (data > gaps)
    return RBFSUM_STATUS_GREATER;

  return RBFSUM_STATUS_LESS;
}

