
#if HAVE_CONFIG_H
#  include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <infiniband/verbs.h>

int printIBDevicesList(struct ibv_device **dev_list);

int main(int argc, char *argv[]) {


	struct ibv_device **dev_list = NULL;
	int ret = printIBDevicesList(dev_list);
	return ret;
}

int printIBDevicesList(struct ibv_device **dev_list){

	int num_devices;

	dev_list = ibv_get_device_list(&num_devices);
	if (!dev_list) {
		perror("Failed to get RDMA devices list");
		return 1;
	}
	else{
		printf("%d devices found\nThey are:\n", num_devices);
		for (int i = 0; i < num_devices; i++) {
			struct ibv_device * device = dev_list[i];
			printf("%s",ibv_get_device_name(device));
			printf(" with device descriptor at address: %p\n", device);
		}
	}
	return 0;	
}



