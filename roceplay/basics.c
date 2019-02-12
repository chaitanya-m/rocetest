
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

int closeDevices(struct ibv_context ***contexts, int numDevices);

int getDeviceAttributes(struct ibv_device **deviceList, int numDevices, struct ibv_context **contexts);

int allocateProtectionDomains(struct ibv_context **contexts, struct ibv_pd ***pds, int numDevices);

int deallocateProtectionDomains(struct ibv_pd ***pds, int numDevices);

int registerMemoryRegions(struct ibv_mr ***mrs, struct ibv_pd **pds, int numDevices);

int deregisterMemoryRegions(struct ibv_mr ***mrs, int numDevices);

int main (int argc, char *argv[]) {

	int numDevices = 0, returnValue = 0;
	struct ibv_device **deviceList = NULL; 
	struct ibv_context **contexts = NULL;
	struct ibv_pd **pds = NULL;
	struct ibv_mr **mrs = NULL;

	// Ensure devices returned safely
	returnValue = findIBDevices(&deviceList, &numDevices);
	// Print list of devices
	returnValue = printIBDevicesList(deviceList, numDevices);
	// Open devices and get contexts
	returnValue = openDevices(deviceList, numDevices, &contexts);	
	// Get attributes of opened device contexts, print what is useful 
	returnValue = getDeviceAttributes(deviceList, numDevices, contexts);
	// Allocate Protection Domain - so resources don't overlap 
	returnValue = allocateProtectionDomains(contexts, &pds, numDevices);

	returnValue = registerMemoryRegions(&mrs, pds, numDevices);



	goto wind_down;	
	
wind_down:	
	returnValue = deregisterMemoryRegions(&mrs, numDevices);
	returnValue = deallocateProtectionDomains(&pds, numDevices);
	returnValue = closeDevices(&contexts, numDevices);
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

	*contexts = (struct ibv_context **) malloc(numDevices * sizeof(struct ibv_context *));

	for (int i = 0; i < numDevices; i++) {
		(*contexts)[i] = ibv_open_device(deviceList[i]);
			//(struct ibv_context *) malloc(sizeof(struct ibv_context));

		if (!(*contexts)[i]) {
			fprintf(stderr, "Context not found for device %s", 
					ibv_get_device_name(deviceList[i]));
			if(!DBG) {
				exit(EXIT_FAILURE); // Exit
			}
		}
		
	}
	return 0;
}

int closeDevices(struct ibv_context ***contexts, int numDevices) {

	for (int i = 0; i < numDevices; i++) {
		//assert(*contexts[i] != NULL);
		int returnValue = ibv_close_device((*contexts)[i]);
		if(returnValue){
			return returnValue;
		}
	}
	return 0;
}

int getDeviceAttributes(struct ibv_device **deviceList, int numDevices, 
		struct ibv_context **contexts) {

	for (int i = 0; i < numDevices; i++) {
		struct ibv_device_attr deviceAttr;
		ibv_query_device(contexts[i], &deviceAttr);
	}
	// It expects a created deviceAttr to modify rather than a 
	// reference to a pointer that it can stick a created object on
	return 0;
}

int allocateProtectionDomains(struct ibv_context **contexts, struct ibv_pd ***pds, int numDevices ) {

	*pds = malloc(numDevices * sizeof(struct ibv_pd *));
	for (int i = 0; i < numDevices; i++) {
		(*pds)[i] = ibv_alloc_pd(contexts[i]);
		if (!(*pds)[i]) {
			fprintf(stderr, "Error, ibv_alloc_pd() failed\n");
			return -1;
		}
	}
	return 0;
}

int deallocateProtectionDomains(struct ibv_pd ***pds, int numDevices) {

	for (int i = 0; i < numDevices; i++) {
		if (ibv_dealloc_pd((*pds)[i])) {
			fprintf(stderr, "Error, ibv_dealloc_pd() failed\n");
			return -1;
		}
	}
	return 0;
}

int registerMemoryRegions(struct ibv_mr ***mrs, struct ibv_pd **pds, int numDevices){

#define REGION_SIZE 0x1800

	*mrs = malloc(numDevices * sizeof(struct ibv_mr *));
	for (int i = 0; i < numDevices; i++) {
        	char* mr_buffer = malloc(REGION_SIZE);
		(*mrs)[i] = ibv_reg_mr(pds[i], mr_buffer, REGION_SIZE, 
				IBV_ACCESS_LOCAL_WRITE | IBV_ACCESS_REMOTE_WRITE 
				| IBV_ACCESS_REMOTE_READ);
		if (!(*mrs)[i]) {
			fprintf(stderr, "Error, ibv_reg_mr() failed\n");
			return -1;
		}
	}
	return 0;
}

int deregisterMemoryRegions(struct ibv_mr ***mrs, int numDevices){

#define REGION_SIZE 0x1800

	for (int i = 0; i < numDevices; i++) {
		if (ibv_dereg_mr((*mrs)[i])) {
			fprintf(stderr, "Error, ibv_dereg_mr() failed\n");
			return -1;
		}
	}
	return 0;
}

