#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *PROCFS_FILE = "/proc/kmsg_log";

int main ( int argc, char* argv[] ) {
	FILE *klog;

	klog = fopen(PROCFS_FILE, "w");
	if ( !klog ) {
		fprintf(stderr, "ERROR: Unable to open %s for writing\n",
				PROCFS_FILE);
		return 1;
	}

	if ( argc > 1 ) {
		fprintf(klog, argv[1]);
	}

	fclose(klog);
	return 0;
}
