
#if HAVE_CONFIG_H
#  include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <inttypes.h>
#include <infiniband/verbs.h>

int printIBDevicesList(struct ibv_device **dev_list, int *num_devices);

int main(int argc, char *argv[]) {

	// Print list of devices
	int num_devices;
	struct ibv_device **dev_list = ibv_get_device_list(&num_devices);
	int ret = printIBDevicesList(dev_list, &num_devices);

	


	return ret;
}

int printIBDevicesList(struct ibv_device **dev_list, int *num_devices){


	if (!dev_list) {
		perror("Failed to get RDMA devices list");
		return 1;
	}
	else{
		printf("%d devices found\nThey are:\n=====\n", *num_devices);
		for (int i = 0; i < *num_devices; i++) {
			struct ibv_device * device = dev_list[i];
			printf("%s\n",ibv_get_device_name(device));
			printf(" with device descriptor at address: %p\n", device);
			printf(" and node type: %s\n", 
					ibv_node_type_str(device->node_type));
			printf(" and transport type: %d\n", 
					(device->transport_type));
			printf(" and guid: %"PRIu64"\n=====\n", 
					ibv_get_device_guid(device));
		}
	}
	return 0;	
}

