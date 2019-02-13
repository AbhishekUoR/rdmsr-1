#include "rdmsr.h"

void usage(void)
{
  printf("rdmsr ... exiting\n");
  exit(0);
}


static void get_options(int argc, char **argv)
{
   if (argc < 2)
      usage();

   while (1) {
      int c = -1;
      char *end = NULL;
      (void)end;

      static struct option long_options[] = {
        {"help", no_argument, NULL, 0},
        {0, 0, 0, 0}
      };

      int option_index = 0;
      c = getopt_long(argc, argv, "",
                      long_options, &option_index);
      if (c == '?' || c == ':')
      {
        printf("getopt_long returned an error or ambigious result\n");
        usage();
      }
      if (c == -1)
      {
        return;
      }
      switch(option_index)
      {
      case 0:
        break;

      default:
        usage();
        break;
      }
   }
}

int main(int argc, char **argv)
{
  get_options(argc, argv);

  return 1;
}



/*
 * read_msr
 *
 * Will return 0 on success and -1 on failure.
 * Possible errno values could be:
 * EFAULT -If the read/write did not fully complete
 * EIO    -If the CPU does not support MSRs
 * ENXIO  -If the CPU does not exist
 */

int read_msr(int cpu, off_t msr, uint64_t *val)
{
	int fd, _err = 0;
	char msr_file_name[64];

	sprintf(msr_file_name, "/dev/cpu/%d/msr", cpu);
	fd = open(msr_file_name, O_RDONLY);
	if (fd < 0)
		return -1;
	if (lseek(fd, msr, SEEK_CUR) == -1)
		goto err;
  _err = read(fd, val, sizeof *val);
	if ( _err != sizeof *val)
		goto err;
	close(fd);
	return 0;
 err:
	close(fd);
	return -1;
}

int write_msr(int cpu, off_t msr, uint64_t val)
{
	int fd;
	char msr_file_name[64];

	sprintf(msr_file_name, "/dev/cpu/%d/msr", cpu);
	fd = open(msr_file_name, O_WRONLY);
	if (fd < 0)
		return -1;
	if (lseek(fd, msr, SEEK_CUR) == -1)
		goto err;
	if (write(fd, &val, sizeof val) != sizeof val)
		goto err;
	close(fd);
	return 0;
 err:
	close(fd);
	return -1;
}
