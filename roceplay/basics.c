
#if HAVE_CONFIG_H
#  include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <infiniband/verbs.h>


int main(int argc, char *argv[]) {

	int num_devices;

	struct ibv_device **dev_list = ibv_get_device_list(&num_devices);
	if (!dev_list) {
		perror("Failed to get RDMA devices list");
		return 1;
	}
	else{
		printf("%d devices found\n", num_devices);
	}
	return 0;
}
