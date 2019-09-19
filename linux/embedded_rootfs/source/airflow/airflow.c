/*
 * airflow.c
 *
 * Fan speed controller, backward compatible with fancontrol(8)
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/reboot.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#define POWER_SHUTDOWN        "/sys/niagara/misc/pwr_down"
#define CONFIG_FILE           "/etc/fancontrol"
#define DFLT_SCAN_INTERVAL    10
/* Temperature thresholds (in 1/1000 C).  */
#define MAX_TEMP_THRESH       85000
#define CRIT_TEMP_THRESH      90000
#define MIN_TEMP_THRESH       40000

/* Allocate array in chunks of 16 elements */
#define CHUNK 16

/* Keywords for configuration file parsing */

enum
  {
    INTERVAL,
    DEVPATH,
    DEVNAME,
    FCTEMPS,
    FCFANS,
    MINTEMP,
    MAXTEMP,
    MINSTART,
    MINSTOP,
    MINPWM,
    MAXPWM,
    KEYWORDS_COUNT
  };

static char *keywords[]=
  {
    "INTERVAL",
    "DEVPATH",
    "DEVNAME",
    "FCTEMPS",
    "FCFANS",
    "MINTEMP",
    "MAXTEMP",
    "MINSTART",
    "MINSTOP",
    "MINPWM",
    "MAXPWM"
  };


/*
 * Find matching configuration keyword.
 *
 */
int find_keyword(char *s)
{
 int i;
 for(i = 0; i < KEYWORDS_COUNT; i++)
   if(!strcmp(s, keywords[i]))
     return i;
 return -1;
}

/*
 * Temperature sensor descriptor.
 */
struct temp_sensor
{
  char *address;
  int fd;
  int value;
  int mintemp_present;
  int maxtemp_present;
  int mintemp;
  int maxtemp;
  struct temp_sensor *next;
};

/*
 * Fan sensor descriptor.
 */
struct fan_sensor
{
  char *address;
  int fd;
  int value;
  struct fan_sensor *next;
};

/*
 * Fan control descriptor.
 */
struct fan_control
{
  char *address;
  int fd;
  int value;
  int minstop;
  int minstart;
  int minpwm;
  int maxpwm;
  int mintemp;
  int maxtemp;
  int alloc_temp_sensors;
  int n_temp_sensors;
  struct temp_sensor **temp_sensors;
  int alloc_fan_sensors;
  int n_fan_sensors;
  struct fan_sensor **fan_sensors;
  struct fan_control *next;
};

/*
 * Lists of all sensors and controls.
 */
static struct temp_sensor *temp_sensors_head = NULL;
static struct fan_sensor *fan_sensors_head = NULL;
static struct fan_control *fan_controls_head = NULL;

/*
 * Read a chunk of data, retry on trivial errors.
 */
ssize_t read_whole_persist(int fd, void *buf, size_t count)
{
  ssize_t l;
  int again = 0;
  do
    {
      l = read(fd, buf, count);
      if(((l < 0) && ((errno == EAGAIN) || (errno == EINTR))) || (l == 0))
	{
	  lseek(fd, 0, SEEK_SET);
	  again = 1;
	}
      else
	again = 0;
    }
  while(again);
  return l;
}

/*
 * Write a chunk of data, retry on trivial errors.
 */
ssize_t write_whole_persist(int fd, void *buf, size_t count)
{
  ssize_t l;
  int again = 0;
  do
    {
      l = write(fd, buf, count);
      if(((l < 0) && ((errno == EAGAIN) || (errno == EINTR))) || (l == 0))
	{
	  lseek(fd, 0, SEEK_SET);
	  again = 1;
	}
      else
	again = 0;
    }
  while(again);
  return l;
}

/*
 * Open file descriptor associated with a sensor or control.
 */
int openfd(char *s, int flags)
{
  int l, fd;
  char *fname;
  l = strlen(s);
  fname = (char*) malloc(l + sizeof("/sys/class/hwmon/"));
  if(fname == NULL)
    return -1;
  memcpy(fname,"/sys/class/hwmon/", sizeof("/sys/class/hwmon/") - 1);
  memcpy(fname + sizeof("/sys/class/hwmon/") - 1, s, l + 1);
  fd = open(fname, flags);
  free(fname);
  return fd;
}

/*
 * Enable/disable direct fan control.
 */
int fan_enable(char *s, int oper)
{
  int l, fd, retval = 0, enabled = !oper;
 char *fname, buf[21];
  l = strlen(s);
  fname = (char*) malloc(l + sizeof("/sys/class/hwmon/")
			 + sizeof("_enable") - 1);
  if(fname == NULL)
    return -1;
  memcpy(fname,"/sys/class/hwmon/", sizeof("/sys/class/hwmon/") - 1);
  memcpy(fname + sizeof("/sys/class/hwmon/") - 1, s, l );
  memcpy(fname + sizeof("/sys/class/hwmon/") - 1 + l,
	 "_enable", sizeof("_enable"));
  fd = open(fname, O_RDWR);
  if(fd >=0)
    {
      /* write */
      lseek(fd, 0, SEEK_SET);
      if(write_whole_persist(fd, oper?"1\n":"0\n", 2)!=2)
	retval = -1;

      /* read */
      lseek(fd, 0, SEEK_SET);
      l = read_whole_persist(fd, &buf, sizeof(buf) - 1);
      if(l>=1)
	{
	  buf[l] = '\0';
	  if((sscanf(buf, "%i", &enabled) != 1)
	     || (enabled != oper))
	    {
	      /*
	      fprintf(stderr, "Enable/disable fan control "
		      "not confirmed by re-reading\n");
	      */
	      retval = -1;
	    }
	}
      else
	{
	  fprintf(stderr, "Can't enable/disable fan control.\n");
	  retval = -1;
	}
      close(fd);
    }
  else
    {
      perror("Can't enable/disable fan control");
      retval = -1;
    }
  free(fname);
  return retval;
}

/*
 * Find temperature sensor by address.
 */
struct temp_sensor *find_temp_sensor(char *s)
{
  struct temp_sensor *current = temp_sensors_head;
  while(current && strcmp(current->address, s))
    current = current->next;
  return current;
}

/*
 * Find fan sensor by address.
 */
struct fan_sensor *find_fan_sensor(char *s)
{
  struct fan_sensor *current = fan_sensors_head;
  while(current && strcmp(current->address, s))
    current = current->next;
  return current;
}

/*
 * Find fan control by address.
 */
struct fan_control *find_fan_control(char *s)
{
  struct fan_control *current = fan_controls_head;
  while(current && strcmp(current->address, s))
    current = current->next;
  return current;
}

/*
 * Create a temperature sensor descriptor.
 */
struct temp_sensor *create_temp_sensor(char *s)
{
  struct temp_sensor *newval =
    (struct temp_sensor*)malloc(sizeof(struct temp_sensor));
  if(newval == NULL)
    return NULL;
  memset(newval, 0, sizeof(struct temp_sensor));
  newval->address = s;
  newval->value = -1;
  newval->mintemp_present = 0;
  newval->mintemp = 0;
  newval->maxtemp_present = 0;
  newval->maxtemp = 60;
  newval->fd = openfd(s, O_RDONLY);
  newval->next = temp_sensors_head;
  temp_sensors_head = newval;
  return newval;
}

/*
 * Create a fan sensor descriptor.
 */
struct fan_sensor *create_fan_sensor(char *s)
{
  struct fan_sensor *newval =
    (struct fan_sensor*)malloc(sizeof(struct fan_sensor));
  if(newval == NULL)
    return NULL;
  memset(newval, 0, sizeof(struct fan_sensor));
  newval->address = s;
  newval->value = -1;
  newval->fd = openfd(s, O_RDONLY);
  newval->next = fan_sensors_head;
  fan_sensors_head = newval;
  return newval;
}

/*
 * Create a fan control descriptor.
 */
struct fan_control *create_fan_control(char *s)
{
  struct fan_control *newval =
    (struct fan_control*)malloc(sizeof(struct fan_control));
  if(newval == NULL)
    return NULL;
  memset(newval, 0, sizeof(struct fan_control));
  newval->address = s;
  newval->value = -1;
  newval->minstop = 0;
  newval->minstart = 128;
  newval->minpwm = 0;
  newval->maxpwm = 255;
  newval->mintemp = 0;
  newval->maxtemp = 60;
  newval->fd = openfd(s, O_RDWR);
  newval->next = fan_controls_head;
  fan_controls_head = newval;
  return newval;
}

/*
 * Add temperature sensor to fan control.
 */
int add_temp_sensor(struct fan_control *fc, struct temp_sensor *sensor)
{
  struct temp_sensor **tmpptr;
  if(fc->alloc_temp_sensors <= fc->n_temp_sensors)
    {
      if(fc->alloc_temp_sensors > 0)
	{
	  tmpptr = (struct temp_sensor **)
	    realloc(fc->temp_sensors,
		    sizeof(struct temp_sensor*)
		    * (fc->alloc_temp_sensors + CHUNK));
	  if(tmpptr == NULL)
	    return -1;

	  fc->temp_sensors = tmpptr;
	  fc->alloc_temp_sensors += CHUNK;
	}
      else
	{
	  fc->temp_sensors = (struct temp_sensor **)
	    malloc(sizeof(struct temp_sensor*) * CHUNK);
	  if(fc->temp_sensors == NULL)
	    return -1;
	  fc->alloc_temp_sensors = CHUNK;
	}
    }
  fc->temp_sensors[fc->n_temp_sensors++] = sensor;
  return 0;
}

/*
 * Add fan sensor to fan control.
 */
int add_fan_sensor(struct fan_control *fc, struct fan_sensor *sensor)
{
  struct fan_sensor **tmpptr;
  if(fc->alloc_fan_sensors <= fc->n_fan_sensors)
    {
      if(fc->alloc_fan_sensors > 0)
	{
	  tmpptr = (struct fan_sensor **)
	    realloc(fc->fan_sensors,
		    sizeof(struct fan_sensor*)
		    * (fc->alloc_fan_sensors + CHUNK));
	  if(tmpptr == NULL)
	    return -1;

	  fc->fan_sensors = tmpptr;
	  fc->alloc_fan_sensors += CHUNK;
	}
      else
	{
	  fc->fan_sensors = (struct fan_sensor **)
	    malloc(sizeof(struct fan_sensor*) * CHUNK);
	  if(fc->fan_sensors == NULL)
	    return -1;
	  fc->alloc_fan_sensors = CHUNK;
	}
    }
  fc->fan_sensors[fc->n_fan_sensors++] = sensor;
  return 0;
}

/*
 * Read temperature (in 1/1000 C).
 */
int read_temperature(struct temp_sensor *temp_sensor)
{
  int l, retval;
  char line[21];
  if(temp_sensor->fd >=0)
    {
      lseek(temp_sensor->fd, 0, SEEK_SET);
      l = read_whole_persist(temp_sensor->fd, &line, sizeof(line) - 1);
      if(l>=1)
	{
	  line[l] = '\0';
	  if(sscanf(line, "%i", &retval) == 1)
	    {
	      temp_sensor->value = retval;
	      return retval;
	    }
	}
    }
  return -1;
}

/*
 * Read fan speed (in RPM).
 */
int read_fan_speed(struct fan_sensor *fan_sensor)
{
  int l, retval;
  char line[21];
  if(fan_sensor->fd >=0)
    {
      lseek(fan_sensor->fd, 0, SEEK_SET);
      l = read_whole_persist(fan_sensor->fd, &line, sizeof(line) - 1);
      if(l>=1)
	{
	  line[l] = '\0';
	  if(sscanf(line, "%i", &retval) == 1)
	    {
	      fan_sensor->value = retval;
	      return retval;
	    }
	}
    }
  return -1;
}

/*
 * Read fan control (PWM value, usually in 0-255 range).
 */
int read_fan_pwm(struct fan_control *fan_control)
{
  int l, retval;
  char line[21];
  if(fan_control->fd >=0)
    {
      lseek(fan_control->fd, 0, SEEK_SET);
      l = read_whole_persist(fan_control->fd, &line, sizeof(line) - 1);
      if(l>=1)
	{
	  line[l] = '\0';
	  if(sscanf(line, "%i", &retval) == 1)
	    {
	      fan_control->value = retval;
	      return retval;
	    }
	}
    }
  return -1;
}

/*
 * Set fan control (PWM value, usually in 0-255 range).
 */
int set_fan_pwm(struct fan_control *fc, int value)
{
  int l;
  char line[21];
  if(fc->fd >=0)
    {
      snprintf(line, sizeof(line), "%d\n", value);
      l = strlen(line);
      lseek(fc->fd, 0, SEEK_SET);
      if(write_whole_persist(fc->fd, line, l) == l)
	{
	  fc->value = value;
	  return 0;
	}
    }
  return -1;
}

/*
 * Get maximum temperature among all sensors associated with the given control.
 */
int get_max_temp(struct fan_control *fc)
{
  int retval = -1, i;
  for(i = 0; i < fc->n_temp_sensors; i++)
    {
      if((i == 0) || (retval < fc->temp_sensors[i]->value))
	retval = fc->temp_sensors[i]->value;
    }
  return retval;
}

/*
 * Get minimum fan speed among all sensors associated with the given control.
 */
int get_min_fan_speed(struct fan_control *fc)
{
  int retval = -1, i;
  for(i = 0; i < fc->n_fan_sensors; i++)
    {
      if((i == 0) || (retval < fc->fan_sensors[i]->value))
	retval = fc->fan_sensors[i]->value;
    }
  return retval;
}

/*
 * Get PWM value based on the temperature.
 */
int get_pwm_by_temperature(struct fan_control *fc, struct temp_sensor *sensor)
{
  int retval, mintemp, maxtemp;
  mintemp = fc->mintemp;
  maxtemp = fc->maxtemp;
  if(sensor->mintemp_present)
    mintemp = sensor->mintemp;
  if(sensor->maxtemp_present)
    maxtemp = sensor->maxtemp;
  if(sensor->value >= maxtemp * 1000)
    {
      retval = fc->maxpwm;
    }
  else
    if(sensor->value <= mintemp * 1000)
      {
	retval = fc->minpwm;
      }
    else
      retval = ((sensor->value - (mintemp * 1000))
		* (fc->maxpwm - fc->minstop)
		/ ((maxtemp - mintemp) * 1000))
	+ fc->minstop;
  if(retval > fc->maxpwm)
    retval = fc->maxpwm;
  else
    if(retval < fc->minpwm)
      retval = fc->minpwm;
  return retval;
}

/*
 * Get maximum PWM value based on all temperatures associated with the
 * given control.
 */
int get_max_pwm_by_temperature(struct fan_control *fc)
{
  int retval, i, value;

  if(fc->n_temp_sensors == 0)
    retval = fc->maxpwm;
  else
    retval = fc->minpwm;

  for(i = 0; i < fc->n_temp_sensors; i++)
    {
      value = get_pwm_by_temperature(fc, fc->temp_sensors[i]);
      if((i == 0) || (retval < value))
	retval = value;
    }
  return retval;
}

/*
 * Check if one device path matches the configured device.
 */
int check_one_devpath(char *hwmon_device, char *destination)
{
  int l, l_hwmon_device, l_dest, retval;
  char *tmpstr1, *endptr1, *startptr2;
  char tmpstr2[256];
  struct stat statbuf;

  l_hwmon_device = strlen(hwmon_device);
  l_dest = strlen(destination);
  tmpstr1 = (char*) malloc(l_hwmon_device
			   + sizeof("/sys/class/hwmon/")
			   + sizeof("/device") - 1);
  if(tmpstr1 == NULL)
    return -1;

  memcpy(tmpstr1, "/sys/class/hwmon/", sizeof("/sys/class/hwmon/") - 1);
  memcpy(tmpstr1 + sizeof("/sys/class/hwmon/") - 1,
	 hwmon_device, l_hwmon_device);
  memcpy(tmpstr1 + sizeof("/sys/class/hwmon/") - 1 + l_hwmon_device,
	 "/device", sizeof("/device"));
  /*
     If the file does not exist, or it is not a symlink,
     check against empty string.
  */
  if(lstat(tmpstr1, &statbuf) || !S_ISLNK(statbuf.st_mode))
    {
      free(tmpstr1);
      return (*destination != '\0');
    }

  /* Truncate */
  tmpstr1[sizeof("/sys/class/hwmon/") - 1 + l_hwmon_device] = '\0';

  /*  If the file is not a symlink, check against empty string. */
  l = readlink(tmpstr1, tmpstr2, sizeof(tmpstr2) - 1);
  if((l < 0) || (l > (sizeof(tmpstr2) - 1)))
    {
      free(tmpstr1);
      return (*destination != '\0');
    }

  /* Terminate the result of readlink() */
  tmpstr2[l] = '\0';

  /* End of the last directory name */
  endptr1 = strrchr(tmpstr1, '/');

  /* Start of the link name */
  startptr2 = tmpstr2;

  /*
    The following will only remove leading sequence of "../", but this is
    sufficient for symlinks used in /sys .
   */
  while(!strncmp(startptr2, "../", 3))
    {
      startptr2 += 3;
      if(endptr1 != tmpstr1)
	*endptr1 = '\0';
      endptr1 = strrchr(tmpstr1, '/');
    }

  /* Check if "/sys/" is still there */
  if(endptr1 < (tmpstr1 + 4))
    {
      free(tmpstr1);
      return -1;
    }

  /*
    Destination is compared in two chunks -- original directory after
    "/sys/" , truncated after the last slash, and symlink with all leading
    "../" removed. It must be a directory.
  */
  retval = strncmp(destination,
		   tmpstr1 + 5,
		   endptr1 + 1 - (tmpstr1 + 5))
    || strncmp(destination + (endptr1 + 1 - (tmpstr1 + 5)),
	       startptr2,
	       l_dest - (endptr1 + 1 - (tmpstr1 + 5)))
    || (startptr2[l_dest - (endptr1 + 1 - (tmpstr1 + 5))] != '/');
  free(tmpstr1);
  return retval;
}

/*
 * Check if all device paths match the configured devices.
 */
int check_devpaths(char *s)
{
  char *p1, *p2, *p3;
  int retval = 0;

  p1=s;

  do
    {
      p2=p1;

      while(*p2 && (*p2!=' ') && (*p2!='='))
	p2++;

      if(*p2 == '=')
	{
	  p3 = p2 + 1;
	  while(*p3 && (*p3!=' '))
	    p3++;
	}
      else
	p1 = NULL;

      if(p1)
	{
	  *p2 = '\0';
	  p2++;
	  if(*p3)
	    {
	      *p3 = '\0';
	      p3++;
	    }

	  retval |= check_one_devpath(p1, p2);

	  if(*p3)
	    p1 = p3;
	  else
	    p1 = NULL;
	}
    }
  while(p1);
  return retval;
}

/*
 * Check if one device name matches the configured device.
 */
int check_one_devname(char *hwmon_device, char *name)
{
  int h, l, l_hwmon_device, l_name, retval;
  char *tmpstr1;

  l_hwmon_device = strlen(hwmon_device);
  l_name = strlen(name);
  tmpstr1 = (char*) malloc(l_hwmon_device
			   + sizeof("/sys/class/hwmon/")
			   + sizeof("/device/name") - 1);
  if(tmpstr1 == NULL)
    return -1;

  memcpy(tmpstr1, "/sys/class/hwmon/", sizeof("/sys/class/hwmon/") - 1);
  memcpy(tmpstr1 + sizeof("/sys/class/hwmon/") - 1,
	 hwmon_device, l_hwmon_device);
  memcpy(tmpstr1 + sizeof("/sys/class/hwmon/") - 1 + l_hwmon_device,
	 "/device/name", sizeof("/device/name"));
  /*
     If the file does not exist, or it is not a symlink,
     check against empty string.
  */
  h = open(tmpstr1, O_RDONLY);
  if(h < 0)
    {
      memcpy(tmpstr1 + sizeof("/sys/class/hwmon/") - 1 + l_hwmon_device,
	     "/name", sizeof("/name"));
      h = open(tmpstr1, O_RDONLY);
    }
  free(tmpstr1);

  /* If no file with device name, check for empty string */
  if(h < 0)
    return (*name != '\0');

  /* Allocate a buffer one byte longer */
  tmpstr1 = (char*) malloc(l_name + 1);
  if(tmpstr1 == NULL)
    {
      close(h);
      return -1;
    }

  /* Read name */
  l = read_whole_persist(h, tmpstr1, l_name + 1);

  /* If it's shorter, mismatch */
  if(l < l_name)
    retval = 1;
  else
    if((l == l_name) || ((unsigned char)(tmpstr1[l_name]) < ' '))
      {
	retval=memcmp(tmpstr1, name, l_name);
      }
    else
      retval = 1;

  /* Clean up */
  close(h);
  free(tmpstr1);

  return retval;
}

/*
 * Check if all device names match the configured devices.
 */
int check_devnames(char *s)
{
  char *p1, *p2, *p3;
  int retval = 0;

  p1=s;

  do
    {
      p2=p1;

      while(*p2 && (*p2!=' ') && (*p2!='='))
	p2++;

      if(*p2 == '=')
	{
	  p3 = p2 + 1;
	  while(*p3 && (*p3!=' '))
	    p3++;
	}
      else
	p1 = NULL;

      if(p1)
	{
	  *p2 = '\0';
	  p2++;
	  if(*p3)
	    {
	      *p3 = '\0';
	      p3++;
	    }

	  retval |= check_one_devname(p1, p2);

	  if(*p3)
	    p1 = p3;
	  else
	    p1 = NULL;
	}
    }
  while(p1);
  return retval;
}

/*
 * Process FCTEMPS configuration line.
 */
int configure_fctemps(char *s)
{
  char *p1, *p2, *p3;
  struct fan_control *fancontrol;
  struct temp_sensor *tempsensor;

  p1=s;

  do
    {
      p2=p1;

      while(*p2 && (*p2!=' ') && (*p2!='='))
	p2++;

      if(*p2 == '=')
	{
	  p3 = p2 + 1;
	  while(*p3 && (*p3!=' '))
	    p3++;
	}
      else
	p1 = NULL;

      if(p1)
	{
	  *p2 = '\0';
	  p2++;
	  if(*p3)
	    {
	      *p3 = '\0';
	      p3++;
	    }
	  fancontrol = find_fan_control(p1);
	  if(fancontrol == NULL)
	    fancontrol = create_fan_control(p1);

	  tempsensor = find_temp_sensor(p2);
	  if(tempsensor == NULL)
	    tempsensor = create_temp_sensor(p2);

	  if((fancontrol != NULL) && (tempsensor != NULL))
	    add_temp_sensor(fancontrol, tempsensor);

	  if(*p3)
	    p1 = p3;
	  else
	    p1 = NULL;
	}
    }
  while(p1);

  return 0;
}

/*
 * Process FCFANS configuration line.
 */
int configure_fcfans(char *s)
{
  char *p1, *p2, *p3;
  struct fan_control *fancontrol;
  struct fan_sensor *fansensor;

  p1=s;

  do
    {
      p2=p1;

      while(*p2 && (*p2!=' ') && (*p2!='='))
	p2++;

      if(*p2 == '=')
	{
	  p3 = p2 + 1;
	  while(*p3 && (*p3!=' '))
	    p3++;
	}
      else
	p1 = NULL;

      if(p1)
	{
	  *p2 = '\0';
	  p2++;
	  if(*p3)
	    {
	      *p3 = '\0';
	      p3++;
	    }

	  fancontrol = find_fan_control(p1);
	  if(fancontrol == NULL)
	    fancontrol = create_fan_control(p1);

	  fansensor = find_fan_sensor(p2);
	  if(fansensor == NULL)
	    fansensor = create_fan_sensor(p2);

	  if((fancontrol != NULL) && (fansensor != NULL))
	    add_fan_sensor(fancontrol, fansensor);

	  if(*p3)
	    p1 = p3;
	  else
	    p1 = NULL;
	}
    }
  while(p1);

  return 0;
}

/*
 * Process parameter associated with a sensor (this feature is not
 * compatible with fancontrol(8)) and is currently unused.
 *
 * See processing of common parameters.
 */
int configure_temp_sensor_param(char *s, int target)
{
  char *p1, *p2, *p3;
  int value;
  struct temp_sensor *tempsensor;

  p1=s;

  do
    {
      p2=p1;

      while(*p2 && (*p2!=' ') && (*p2!='='))
	p2++;

      if(*p2 == '=')
	{
	  p3 = p2 + 1;
	  while(*p3 && (*p3!=' '))
	    p3++;
	}
      else
	p1 = NULL;

      if(p1)
	{
	  *p2 = '\0';
	  p2++;
	  if(*p3)
	    {
	      *p3 = '\0';
	      p3++;
	    }

	  tempsensor = find_temp_sensor(p1);
	  if(tempsensor == NULL)
	    tempsensor = create_temp_sensor(p1);
	  if(tempsensor != NULL)
	    {
	      if(sscanf(p2, "%i", &value) == 1)
		switch(target)
		  {
		  case MINTEMP:
		    tempsensor->mintemp = value;
		    tempsensor->mintemp_present = 1;
		    break;
		  case MAXTEMP:
		    tempsensor->maxtemp = value;
		    tempsensor->maxtemp_present = 1;
		    break;
		  }
	    }
	  if(*p3)
	    p1 = p3;
	  else
	    p1 = NULL;
	}
    }
  while(p1);

  return 0;
}

/*
 * Process parameter assicuated with a control.
 */
int configure_fan_control_param(char *s, int target)
{
  char *p1, *p2, *p3;
  int value;
  struct fan_control *fancontrol;

  p1=s;

  do
    {
      p2=p1;

      while(*p2 && (*p2!=' ') && (*p2!='='))
	p2++;

      if(*p2 == '=')
	{
	  p3 = p2 + 1;
	  while(*p3 && (*p3!=' '))
	    p3++;
	}
      else
	p1 = NULL;

      if(p1)
	{
	  *p2 = '\0';
	  p2++;
	  if(*p3)
	    {
	      *p3 = '\0';
	      p3++;
	    }

	  fancontrol = find_fan_control(p1);
	  if(fancontrol == NULL)
	    fancontrol = create_fan_control(p1);
	  if(fancontrol != NULL)
	    {
	      if(sscanf(p2, "%i", &value) == 1)
		switch(target)
		  {
		  case MINSTART:
		    fancontrol->minstart = value;
		    break;
		  case MINSTOP:
		    fancontrol->minstop = value;
		    break;
		  case MINPWM:
		    fancontrol->minpwm = value;
		    break;
		  case MAXPWM:
		    fancontrol->maxpwm = value;
		    break;
		  case MINTEMP:
		    fancontrol->mintemp = value;
		    break;
		  case MAXTEMP:
		    fancontrol->maxtemp = value;
		    break;
		  }
	    }

	  if(*p3)
	    p1 = p3;
	  else
	    p1 = NULL;
	}
    }
  while(p1);

  return 0;
}

/*
 * Process parameter associated with either a sensor or a control
 * (this feature is not compatible with fancontrol(8))
 */
int configure_common_param(char *s, int target)
{
  char *p, *p1, *p2, *p3;
  int value;
  struct temp_sensor *tempsensor;
  struct fan_control *fancontrol;

  p1=s;

  do
    {
      p2=p1;

      while(*p2 && (*p2!=' ') && (*p2!='='))
	p2++;

      if(*p2 == '=')
	{
	  p3 = p2 + 1;
	  while(*p3 && (*p3!=' '))
	    p3++;
	}
      else
	p1 = NULL;

      if(p1)
	{
	  *p2 = '\0';
	  p2++;
	  if(*p3)
	    {
	      *p3 = '\0';
	      p3++;
	    }

	  tempsensor = find_temp_sensor(p1);
	  fancontrol = find_fan_control(p1);
	  if((tempsensor == NULL) && (fancontrol == NULL))
	    {
	      p = strrchr(p1,'/');
	      if(p)
		{
		  if(!strncmp(p, "/temp", 5))
		    tempsensor = create_temp_sensor(p1);
		  else
		    if(!strncmp(p, "/pwm", 4))
		      fancontrol = create_fan_control(p1);
		}
	    }

	  if(fancontrol != NULL)
	    {
	      if(sscanf(p2, "%i", &value) == 1)
		switch(target)
		  {
		  case MINTEMP:
		    fancontrol->mintemp = value;
		    break;
		  case MAXTEMP:
		    fancontrol->maxtemp = value;
		    break;
		  }
	    }
	  else
	    {
	      if(tempsensor != NULL)
		{
		  if(sscanf(p2, "%i", &value) == 1)
		    switch(target)
		      {
		      case MINTEMP:
			tempsensor->mintemp = value;
			tempsensor->mintemp_present = 1;
			break;
		      case MAXTEMP:
			tempsensor->maxtemp = value;
			tempsensor->maxtemp_present = 1;
			break;
		      }
		}
	    }

	  if(*p3)
	    p1 = p3;
	  else
	    p1 = NULL;
	}
    }
  while(p1);

  return 0;
}

/*
 * Restore un-controlled fans state on exit.
 */
void restore_fans(void)
{
  struct fan_control *current_control;
  current_control = fan_controls_head;
  while(current_control)
    {
      if(fan_enable(current_control->address, 0))
	{
	  fan_enable(current_control->address, 1);
	  set_fan_pwm(current_control, 255);
	}
      current_control = current_control->next;
    }
}

/*
 * Flag used to terminate the program.
 */
int running = 1;

/*
 * Signals handler.
 */
void sighandler(int n)
{
  running = 0;
}

static int max_temp = MAX_TEMP_THRESH;
static int crit_temp = CRIT_TEMP_THRESH;
static char configfilename[256];

static int parse_args(int argc, char *argv[])
{
  int errflag = 0, c;

  sprintf(configfilename, "%s", CONFIG_FILE);
  while (!errflag && (c=getopt(argc,argv,"c:m:f:")) != -1 ) {
    switch (c) {
      case 'm':
        max_temp = atoi(optarg);
        break;
      case 'c':
        crit_temp = atoi(optarg);
        break;
      case 'f':
        sprintf(configfilename, "%s", optarg);
        crit_temp = atoi(optarg);
        break;
      default:
        errflag = 1;
    }
  }
  if (errflag) {
    fprintf(stderr, "Usage: %s [-m max_tem] [-c crit_temp] [-f config_file]\n", argv[0]);
    return 1;
  }

  if (max_temp < MIN_TEMP_THRESH || crit_temp < MIN_TEMP_THRESH) {
    fprintf(stderr, "Maximum or critical temperature thresholds must be greater or equal %d!\n", MIN_TEMP_THRESH);
    return 1;
  }

  if (crit_temp < max_temp) crit_temp = max_temp;
  return 0;
}

static void shutdown_board(void)
{
  int fd = open(POWER_SHUTDOWN, O_RDWR);
  fprintf(stderr, "Board shutdown...\n");
  if(fd >=0) {
      lseek(fd, 0, SEEK_SET);
      if(write_whole_persist(fd, "1\n", 2)!=2)
        reboot(RB_AUTOBOOT);
  }
  reboot(RB_AUTOBOOT);
}

/*
 * main
 */
int main(int argc, char **argv)
{
  int cfg_interval = DFLT_SCAN_INTERVAL;
  char *configbuffer, *p, **lines;
  int nlines, i, keyword;
  int dev_mismatch = 0;
  struct stat statbuf;
  int configfile;
  int interval;

  if(parse_args(argc, argv))
      return 1;

  /* Read configuration file. */
  configfile = open(configfilename, O_RDONLY);
  if(configfile < 0)
    {
      perror(configfilename);
      return 1;
    }

  if(fstat(configfile, &statbuf) < 0)
    {
      perror(configfilename);
      close(configfile);
      return 1;
    }
  if(statbuf.st_size <= 0)
    {
      fprintf(stderr, "Configuration file %s is empty.\n", configfilename);
      close(configfile);
      return 1;
    }
  configbuffer = (char*) malloc(statbuf.st_size + 1);
  if(configbuffer == NULL)
    {
      fprintf(stderr, "Insufficient memory.\n");
      close(configfile);
      return 0;
    }
  if(read_whole_persist(configfile, configbuffer, statbuf.st_size) != statbuf.st_size)
    {
      fprintf(stderr,  "Can't read the configuration file %s.\n",
	      configfilename);
      close(configfile);
      return 1;
    }
  configbuffer[statbuf.st_size] = '\0';
  close(configfile);

  /* Parse configuration. */
  p = configbuffer;
  nlines = 1;
  while(*p)
    if(*p++ == '\n')
      nlines++;

  lines = (char**) malloc(nlines * sizeof(char*));
  if(lines == NULL)
    {
      fprintf(stderr, "Insufficient memory.\n");
      free(configbuffer);
      return 1;
    }

  for(p = configbuffer, i = 0; i < nlines ; i++)
    {
      lines[i] = p;
      p = strchr(p, '\n');
      if(p != NULL)
	*p++ = '\0';
    }

  /* Parse lines, create descriptors. */
  for(i = 0; i < nlines; i++)
    {
      p = strchr(lines[i], '#');
      if(p != NULL)
	*p = '\0';
      p = strchr(lines[i], '=');
      if(p != NULL)
	{
	  *p = '\0';
	  p++;
	  keyword = find_keyword(lines[i]);
	  switch(keyword)
	    {
	    case INTERVAL:
	      if((sscanf(p, "%i", &cfg_interval) != 1)
		 || (cfg_interval < 1)
		 || (cfg_interval > 100))
		cfg_interval = DFLT_SCAN_INTERVAL;
	      break;
	    case DEVPATH:
	      dev_mismatch |= check_devpaths(p);
	      break;
	    case DEVNAME:
	      dev_mismatch |= check_devnames(p);
	      break;
	    case FCTEMPS:
	      configure_fctemps(p);
	      break;
	    case FCFANS:
	      configure_fcfans(p);
	      break;
	    case MINTEMP:
	    case MAXTEMP:
		configure_common_param(p, keyword);
	      break;
	    case MINSTART:
	    case MINSTOP:
	    case MINPWM:
	    case MAXPWM:
	      configure_fan_control_param(p, keyword);
	      break;
	    }
	}
    }

  if(dev_mismatch)
    {
      fprintf(stderr, "Devices do not match saved paths and names, exiting.\n");
      return 1;
    }

  /*
    Do not try to run if configuration does not contain
    at least one sensor and at least one control.
  */
  if((temp_sensors_head == NULL) || (fan_controls_head == NULL))
    {
      fprintf(stderr, "No devices defined, exiting.\n");
      return 1;
    }

  /* Set handler for restoring the fans uncontrolled state */
  struct sigaction act;

  memset(&act, 0, sizeof(act));
  act.sa_handler = sighandler;

  sigaction(SIGTERM, &act, NULL);
  sigaction(SIGINT, &act, NULL);
  sigaction(SIGQUIT, &act, NULL);

  /* Enable fan control */
  struct fan_control *current_control;
  current_control = fan_controls_head;
  while(current_control)
    {
      fan_enable(current_control->address, 1);
      current_control = current_control->next;
    }

  /* Main loop. */
  while(running)
    {
      /* Read all temperatures. */
      struct temp_sensor *current_temp_sensor;
      int max_cur_temp, temp_value;
      current_temp_sensor = temp_sensors_head;
      max_cur_temp = 0;
      while(current_temp_sensor)
	{
	  temp_value = read_temperature(current_temp_sensor);
          if (temp_value > max_cur_temp)
              max_cur_temp = temp_value;
          current_temp_sensor = current_temp_sensor->next;
	}

      /* Process temperature thresholds */
      if (max_cur_temp >= max_temp)
          interval = cfg_interval / 2;
      else
          interval = cfg_interval;
      if (max_cur_temp >= crit_temp)
          shutdown_board();

      /* Read all fan speeds */
      struct fan_sensor *current_fan_sensor;
      current_fan_sensor = fan_sensors_head;
      while(current_fan_sensor)
	{
	  read_fan_speed(current_fan_sensor);
	  current_fan_sensor = current_fan_sensor->next;
	}

      /*
	 Read current PWM value for controls,
	 determine and set new PWM values.
      */
      current_control = fan_controls_head;
      while(current_control)
	{
	  int min_fan_speed, new_pwm;
	  read_fan_pwm(current_control);
	  min_fan_speed = get_min_fan_speed(current_control);
	  new_pwm = get_max_pwm_by_temperature(current_control);

	  /* Check if we are starting a stopped fan */
	  if((new_pwm > current_control->minpwm)
	    && ((min_fan_speed == 0)
		|| (current_control->value <= current_control->minpwm)))
	    new_pwm = current_control->minstart;

	  /* Set new PWM if it changed */
	  if(new_pwm != current_control->value)
	    set_fan_pwm(current_control, new_pwm);
	  current_control = current_control->next;
	}
      sleep(interval);
    }

  /* Restore fans */
  restore_fans();
  return 0;
}
