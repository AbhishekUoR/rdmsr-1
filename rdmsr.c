#include "rdmsr.h"

void usage(void)
{
  printf("rdmsr: use Linux /proc/cpu/msr file to read or write Intel MSR values\n");
  printf("\t--cpu <number> cpu number: required (ONE-indexed, base 10)\n");
  printf("\t--msr <number> MSR number to read (default) or write: required (base 16)\n");
  printf("\t--value <number> MSR number to write: optional (base 16)\n\n");
  printf("\tIf value is specified, will attempt to write the MSR, otherwise will attempt to read\n");
  printf("\tRequires root privilege on most Linux systems.\n");

  exit(0);
}

int cpu = 0;
off_t msr = 0;
uint64_t value = 0;

static void get_options(int argc, char **argv)
{
   if (argc < 3)
      usage();

   while (1) {
      int c = -1;
      char *end = NULL;

      static struct option long_options[] = {
        {"cpu", required_argument, NULL, 0},
        {"msr", required_argument, NULL, 1},
        {"value", required_argument, NULL, 2},
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
      {
        /**  --cpu <number> in decimal **/
        if (!(cpu = strtol(optarg, &end, 10)) || errno == ERANGE)
        {
          printf("error storing cpu number (base 10)\n");
          usage();
        }
        cpu--; /** convert to zero-indexed **/
        break;
      }
      case 1:
      {
        /** --msr <number> in hex **/
        if (!(msr = strtol(optarg, &end, 16)) || errno == ERANGE)
        {
          printf("error storing msr number (base 16)\n");
          usage();
        }
        break;
      }
      case 2:
      {
        /** --value <number> in hex **/
        if (!(value = strtol(optarg, &end, 16)) || errno == ERANGE)
        {
          printf("error storing value (base 16)\n");
          usage();
        }
        break;
      }
      default:
        usage();
        break;
      }
   }
}

int main(int argc, char **argv)
{
  get_options(argc, argv);

  if (value)
  {
    if(write_msr(cpu, msr, value))
    {
      perror("error writing MSR");
      exit(0);
    }
    exit(1);
  }

  if (read_msr(cpu, msr, &value))
  {
    perror("error reading MSR");
    exit(0);
  }
  printf("value: %0lx\n", value);
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
