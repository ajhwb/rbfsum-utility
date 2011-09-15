#include <string.h>
#include <stdlib.h>
#include <stdlib.h>
#include <time.h>

#include "station.h"

const station_t station_info[STATION_NUM] = {
  { "AAI", "Ambon", -3.687109, 128.19429 },
  { "ABJI", "Asem Bagus", -7.795641, 114.234184 },
  { "ALKI", "Alor", -8.124491, 125.411682 },
  { "APSI", "Ampana", -0.910782, 121.648666 },
  { "BASI", "Baing", -10.2189, 120.5777 },
  { "BATI", "CTBTO", -10.2065, 123.6633 },
  { "BBKI", "Banjar Baru", -3.4625, 114.8411 },
  { "BBSI", "Bau Bau", -5.48845, 122.569458 },
  { "BKSI", "Bulukumba", -5.321843, 120.122436 },
  { "BLJI", "Banyuglugur", -7.745365, 113.595011 },
  { "BLSI", "Bandar Lampung", -5.367548, 105.245210 },
  { "BNSI", "Bone", -4.400521, 120.106521 },
  { "BSSI", "Benteng, P. Selayar", -6.1428, 120.4904 },
  { "BWJI", "Bawean", -5.8511, 112.6578 },
  { "BYJI", "Banyuwangi", -8.2140, 114.3557 },
  { "CBJI", "Citeko", -6.698052, 106.934951 },
  { "CGJI", "Cigeulis Pandeglang Banten", -6.73437, 108.263015 },
  { "CMJI", "Cimerak", -7.783696, 108.448516 },
  { "CNJI", "Cianjur", -7.309, 107.13 },
  { "CTJI", "Tegal", -7.007548, 109.183578 },
  { "DBJI", "Dermaga", -6.5538, 106.7497 },
  { "DBNI", "Bima", -8.501888, 118.312133 },
  { "DNP", "Denpasar", -8.677404, 115.209701 },
  { "DSRI", "Dabo Singkep", -0.4793, 104.5778 },
  { "EDFI", "Ende", -8.7497, 121.6903 },
  { "EGSI", "Enggano", -5.3526, 102.2767 },
  { "GLMI", "Galela", 1.8381, 127.7879 },
  { "GMJI", "Gumukmas", -8.273206, 113.444145 },
  { "GRJI", "Gresik", -6.914433, 112.479354 },
  { "JAY", "Jayapura CTBTO", -2.514513, 140.704299 },
  { "JCJI", "Jatiwangi", -6.7344, 108.2631 },
  { "KASI", "Kota Agung Lampung", -5.52361, 104.495964 },
  { "KBKI", "Kotabaru Kalimantan", -3.2995, 116.1668 },
  { "KCSI", "Kutacane", 3.522185, 97.771575 },
  { "KKSI", "Kolaka", -4.171717, 121.651252 },
  { "KLI", "Kotabumi", -4.836292, 104.870452 },
  { "KLNI", "Mataram", -8.421889, 116.094932 },
  { "KMMI", "Kalianget", -7.041162, 113.915725 },
  { "KMPI", "Kaimana", -3.6616, 133.7044 },
  { "KMSI", "Kotamubago", 0.574544, 123.890674 },
  { "KPJI", "Karang Pucung", -7.333193, 108.931228 },
  { "KRAI", "Kai Ratu", -3.318356, 128.394729 },
  { "KRJI", "Kerinci", -2.091199, 101.461929 },
  { "KRK", "Karangkates", -8.152149, 112.450637 },
  { "LASI", "Langsa", 4.457251, 97.970367 },
  { "LBMI", "Labuha", -0.637887, 127.500816 },
  { "LEM", "Lembang (CTBTO)", -6.826, 107.617 },
  { "LHSI", "Lahat", -3.826624, 103.52333 },
  { "LWLI", "Liwa", -5.017472, 104.058914 },
  { "MASI", "Muara Aman", -3.141521, 102.239608 },
  { "MDSI", "Muara Dua", -4.486037, 104.17823 },
  { "MKBI", "Muko-Muko", -2.447358, 101.239601 },
  { "MLSI", "Melaboh", 4.266851, 96.403984 },
  { "MMPI", "Merauke", -8.518124, 140.414138 },
  { "MNSI", "Mandailing Natal", 0.795498, 99.579627 },
  { "MPSI", "Mapaga", 0.3374, 119.898 },
  { "MRSI", "Marissa Gorontalo", 0.477076, 121.940589 },
  { "MSAI", "Masohi", -3.346117, 128.928465 },
  { "MTKI", "Muara Teweh", -0.941833, 114.895919 },
  { "NBPI", "Nabire", -1.318243, 135.483398 },
  { "NGJI", "Ngawi", -7.367541, 111.461257 },
  { "NLAI", "Namlea, Pulau Buru", -3.238971, 127.099815 },
  { "PBKI", "Pangkalan Bun", -2.7047, 111.6697 },
  { "PBSI", "Pulau Batu", -0.0547, 98.28 },
  { "PCJI", "Pacitan", -8.194656, 111.177085 },
  { "PDSI", "Padang", -0.911762, 100.461723 },
  { "PKKI", "Palangkaraya", -2.2262, 113.9453 },
  { "PMSI", "Majene", -3.500848, 118.914939 },
  { "PPBI", "Pangkal Pinang", -2.161539, 106.136367 },
  { "PPSI", "Pulau Pagai", -2.766, 100.01 },
  { "PSI", "Prapat (CTBTO)", 2.695185, 98.923934 },
  { "PWJI", "Pagerwojo", -8.02191, 111.804145 },
  { "RAPI", "Raja Ampat", -0.4102, 130.8211 },
  { "RBSI", "Rajabasa Lampung", -5.84445, 105.742 },
  { "RGRI", "Rengat", -0.349101, 102.333816 },
  { "RKPI", "Ransiki", -1.5107, 134.1773 },
  { "RPSI", "Rantau Prapat", 2.150381, 99.788053 },
  { "RTBI", "Negara", -8.4597, 114.942 },
  { "SBJI", "Serang", -6.111155, 106.132423 },
  { "SBSI", "Sibolga", 1.398759, 99.430938 },
  { "SCJI", "Cilacap", -7.681004, 109.168922 },
  { "SDSI", "Sungai Dareh", -0.932516, 101.428001 },
  { "SGKI", "Sangata", 0.530205, 117.604347 },
  { "SGSI", "Sangihe", 3.68598, 125.528587 },
  { "SIJI", "Sorong (CTBTO)", -7.734855, 111.766822 },
  { "SISI", "Pulau Saibi", -1.3265, 99.0895 },
  { "SKJI", "Sukabumi", -7.00527, 106.563003 },
  { "SKSI", "Soroako", -2.5283, 121.3345 },
  { "SLSI", "Sarolangun", -2.3924, 102.5927 },
  { "SMKI", "Samarinda", -0.446136, 117.208549 },
  { "SMPI", "Sarmi", -1.981002, 138.710525 },
  { "SNSI", "Sinabang", 2.408926, 96.326705 },
  { "SPSI", "Sidrap Palu", -3.964596, 119.769081 },
  { "SRBI", "Singaraja", -8.084745, 115.212608 },
  { "SRPI", "Serui", -1.875517, 136.239959 },
  { "STKI", "Sintang", 0.065624, 111.47714 },
  { "SWI", "Sorong", -0.862801, 131.259185 },
  { "SWJI", "Sawahan", 7.734855, 111.766822 },
  { "TBJI", "Tambak Boyo", -6.8179, 111.8481 },
  { "TNG", "Tangerang", -6.171935, 106.646919 },
  { "TPRI", "Tanjung Pinang", 0.91836, 104.52629 },
  { "TRSI", "Tarutung", 2.0256, 98.9594 },
  { "TSI", "Tuntungan", 3.501179, 98.564567 },
  { "TTSI", "Tana Toraja", -3.045083, 119.818969 },
  { "TWSI", "Taliwang", -8.738178, 116.882141 },
  { "UBSI", "Bengkulu", -3.7611, 102.2714 },
  { "UWJI", "Ujung Watu", -6.419125, 110.947341 },
  { "WBSI", "Waikabubak", -9.641119, 119.391166 },
  { "WOJI", "Wonogiri", -7.837153, 110.923606 },
};

#define next_line0(tmp) \
({ \
  char *str = tmp; \
  while (*str != '\n') \
    if (*str != '\0') \
      str++; \
  if (*str != '\0') \
    str++; \
  str; \
})

int
lookup_station (const char *name)
{
  int i = 0;
  int len = sizeof(station_info) / sizeof(station_info[0]);

  while (i < len)
    {
      if (!strcmp (name, station_info[i].name))
        return i;
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

  return strndup (buffer, tmp - buffer);
}

bool
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
        return false;
      start++;
    }
  tmp = g_strndup (buffer, start - buffer);
  if (strlen (tmp) != 16)
    {
      g_free (tmp);
      return false;
    }

  g_snprintf (year, sizeof(year), "%s", tmp);
  g_snprintf (month, sizeof(month), "%s", tmp + 5);
  g_snprintf (date, sizeof(date), "%s", tmp + 8);
  g_snprintf (hour, sizeof(hour), "%s", tmp + 11);
  g_free (tmp);

  if ((y = strtol (year, NULL, 10)) <= 0)
    return false;
  if ((m = strtol (month, NULL, 10)) <= 0)
    return false;
  if ((d = strtol (date, NULL, 10)) <= 0)
    return false;
  if ((h = strtol (hour, NULL, 10)) < 0)
    return false;

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
        return false;
      start++;
    }

  end = start;
  while (*end != ' ')
    {
      if (*end == '\0')
        return false;
      end++;
    }

  tmp = g_strndup (start, end - start);
  data->data = strtod (tmp, NULL);
  g_free (tmp);
  if (data->data < 0.0)
    return false;

  /* ReTx */
  start = end;
  while (*start == ' ')
    {
      if (*start == '\0')
        return false;
      start++;
    }

  end = start;
  while (*end != ' ')
    {
      if (*end == '\0')
        return false;
      end++;
    }

  tmp = g_strndup (start, end - start);
  data->retx = strtod (tmp, NULL);
  g_free (tmp);
  if (data->retx < 0)
    return false;

  /* Gaps */
  start = end;
  while (*start == ' ')
    {
      if (*start == '\0')
        return false;
      start++;
    }

  end = start;
  while (*end != ' ')
    {
      if (*end == '\0')
        return false;
      end++;
    }

  tmp = g_strndup (start, end - start);
  data->gap = strtod (tmp, NULL);
  g_free (tmp);
  if (data->gap < 0)
    return false;

  /* Breaks */
  start = end;
  while (*start == ' ')
    {
      if (*start == '\0')
        return false;
      start++;
    }

  end = start;
  while (*end != '\0')
    end++;

  tmp = g_strndup (start, end - start);
  if (strcmp (tmp, "-"))
    data->breaks = strtol (tmp, NULL, 10);
  else
    data->breaks = 0;
  g_free (tmp);

  return true;
}

bool
read_rbfsum (char *buffer, const char *station, rbfsum_t data[96], guint *ndata)
{
  char *tmp = buffer;
  char *filename;
  char *start;
  char *file_line;
  char *line;
  bool test;
  bool retval = false;
  guint id = 0;

  rbfsum_t rb;

  filename = g_strdup_printf ("R%s.BHZ", station);

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
          g_free (file_line);
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
                  g_free (line);
                  tmp = next_line (tmp);
                }
              *ndata = id;
              retval = true;
            }
        }
      tmp = next_line (tmp);
    }

  *ndata = id;
  g_free (filename);

  return retval;
}

#if 0
static int
comp_func (const station_data_t *data1, const station_data_t *data2)
{
  return strcmp (data1->name, data2->name);
}

GList
*read_rbfsum2 (char *buffer)
{
  GList *list = NULL;
  char *tmp = buffer;
  char *name;
  char *start, *end;
  char *file_line;
  char *line;
  rbfsum_t rb;
  guint id;
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

          name = g_strndup (start + 1, end - start - 1);
          g_free (file_line);

          tmp = next_line (tmp);
          if (*tmp == '\0')
            {
              g_free (name);
              break;
            }

          tmp = next_line (tmp);
          if (*tmp == '\0')
            {
              g_free (name);
              break;
            }

          data = g_new (station_data_t, 1);
          data->name = name;

          id = 0;
          while ((line = get_line (tmp)))
            {
              if (parse_data (line, &rb))
                {
                  memcpy (&data->rs[id], &rb, sizeof(rb));
                  id++;
                }
              g_free (line);
              tmp = next_line (tmp);
            }
          data->ndata = id;
          list = g_list_prepend (list, data);
        }
      tmp = next_line (tmp);
    }

  list = g_list_sort (list, (GCompareFunc) comp_func);
  return list;
}
#endif

rbfsum_status_t
rbfsum_get_status (rbfsum_t rs[], guint ndata)
{
  gdouble data = 0;
  gdouble gaps = 0;
  guint i = ndata;

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

