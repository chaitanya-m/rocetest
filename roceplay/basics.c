
#if HAVE_CONFIG_H
#  include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <errno.h> // Provides a global errno; use errno -l to list possible values
#include <inttypes.h> // For uint64_t etc
#include <stdlib.h> // exit(), etc
#include <assert.h>

#include <infiniband/verbs.h>

#define DBG 1

int findIBDevices(struct ibv_device ***deviceList, int *numDevices);
// Only ** because we don't want the print function to modify this! (pass-by-value)
int printIBDevicesList(struct ibv_device **deviceList, int numDevices);

int openDevices(struct ibv_device **deviceList, int numDevices, struct ibv_context ***contexts);	

int getDeviceAttributes(struct ibv_device **deviceList, int numDevices, struct ibv_context **contexts);

int main (int argc, char *argv[]) {

	int numDevices = 0, returnValue = 0;
	struct ibv_device **deviceList = NULL; 
	struct ibv_context **contexts = NULL;
	
	// Ensure devices returned safely
	returnValue = findIBDevices(&deviceList, &numDevices);
	// Print list of devices
	returnValue = printIBDevicesList(deviceList, numDevices);
	// Open devices and get contexts
	returnValue = openDevices(deviceList, numDevices, &contexts);	
	// Print some attributes of interest of opened devices 


	goto free_device_list;	
	
free_device_list:	
	ibv_free_device_list(deviceList);

	return returnValue; // exit is a system call... just return cleanly...
}

int findIBDevices(struct ibv_device ***deviceList, int *numDevices) {
	*deviceList = ibv_get_device_list(numDevices);
	printf("%d devices found\nThey are:\n=====\n", *numDevices);

	if (! *deviceList) {
		perror("Failed to get RDMA devices list");
		printf("Errno value is: %d", errno);
		fflush(stdout); // Let's ensure the print buffer is flushed
	
		if(!DBG) {
			exit(EXIT_FAILURE); // Exit
		}
	}	
	
	else {
		assert (numDevices != 0);
		assert (deviceList != NULL);
		perror("Device List Obtained");
	}
	return 0;
}

int printIBDevicesList(struct ibv_device **deviceList, int numDevices) {

	printf("%d devices found\nThey are:\n=====\n", numDevices);
	for (int i = 0; i < numDevices; i++) {
		struct ibv_device * device = deviceList[i];
		printf("%s\n",ibv_get_device_name(device));
		printf(" with device descriptor at address: %p\n", device);
		printf(" and node type: %s\n", 
				ibv_node_type_str(device->node_type));
		printf(" and transport type: %d\n", 
				(device->transport_type));
		printf(" and guid: %"PRIu64"\n=====\n", // unit64_t 
				ibv_get_device_guid(device));
	}
	return 0;	
}

int openDevices(struct ibv_device **deviceList, int numDevices, struct ibv_context ***contexts) {

	*contexts = malloc(numDevices * sizeof(struct ibv_device *));

	for (int i = 0; i < numDevices; i++) {
		struct ibv_device * device = deviceList[i];
		struct ibv_context *context = ibv_open_device(device);

		if (!context) {
			fprintf(stderr, "Context not found for device %s", 
					ibv_get_device_name(device));
			if(!DBG) {
				exit(EXIT_FAILURE); // Exit
			}
		}
		(*contexts)[i] = context;
	}
	return 0;
}

int getDeviceAttributes(struct ibv_device **deviceList, int numDevices, 
		struct ibv_context **contexts){

	for (int i = 0; i < numDevices; i++) {
		struct ibv_device_attr deviceAttr;
		ibv_query_device(contexts[i], &deviceAttr);
	}
	// It expects a created deviceAttr to modify rather than a 
	// reference to a pointer that it can stick a created object on
	return 0;
}

