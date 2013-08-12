#include <string.h>
#include <stdio.h>
#include "acl_license.h"

int main(int argc, char** argv)
{
	int success;
	printf("argc: %d\n", argc);
	if(argc > 1)
	{
		success = acl_license(argv[1]);
	} else {
		success = acl_license(NULL);
	}
	printf("Did it work? %d\n", success);
	return success;
}
