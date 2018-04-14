#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#define MAGIC 	(('P' << 24) + ('S' << 16) + ('B' << 8) + 'I')
#define HEADERLEN 4 * 40
#define ENTITIES 0	

#define true 	1
#define false	0

uint32_t pos;

static _Bool usage(int32_t argc, char** argv) {
	if (argc < 2) {
		printf("Usage: %s <bspfile> [<bspfile>...]\n", argv[0]);
		return false;
	}

	return true;
}

uint32_t ReadInt(char *data) {
	uint32_t i;
	i = ((data[pos+3] & 0xff) << 24) + 
		((data[pos+2] & 0xff) << 16) + 
		((data[pos+1] & 0xff) << 8) + 
		(data[pos] & 0xff);
	pos += 4;
	
	return i;
}	

void printEntities(char *bspfile) {
	char header[160];
	
	uint32_t i;
	pos = 0;
	uint32_t offsets[19];
	uint32_t lengths[19];
	FILE *fp;
	uint32_t check, version;
	char texture_name[32];

	memset(&offsets, 0, sizeof(offsets));
	memset(&lengths, 0, sizeof(lengths));
	
	fp = fopen(bspfile, "r");
	
	fread(header, sizeof(header[0]), HEADERLEN, fp);
	
	check = ReadInt(header);
	version = ReadInt(header);
	
	if (check != MAGIC) {
		printf("Invalid BPS file: %s\n", bspfile);
		return;
	}
	
	for (i=0; i<19; i++) {
		offsets[i] = ReadInt(header) - HEADERLEN;
		lengths[i] = ReadInt(header);
	}	
	
	char entity_lump[(lengths[ENTITIES])];
	
	fseek(fp, offsets[ENTITIES] + HEADERLEN, SEEK_SET);
	fread(entity_lump, sizeof(char), lengths[ENTITIES], fp);
	printf("%s\n", entity_lump);
	
	fclose(fp);
}

int32_t main(int32_t argc, char** argv) {
	
	int32_t i;
	
	if (!usage(argc, argv)){
		return EXIT_FAILURE;
	}

	for (i=1; i<argc; i++) {
		printEntities(argv[i]);
	}
	
	return EXIT_SUCCESS;
}
