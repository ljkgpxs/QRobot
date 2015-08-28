/* QRobot Project By Ljkgpxs */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
	if(argc < 3) {
//		fprintf(stderr, "Give me a string\n");
		return 0;
	}
	FILE *qfile = fopen(argv[2], "r");
	if(qfile == NULL) {
//		fprintf(stderr, "Open Question File Failed\n");
		return 0;
	}

	char *ques = (char *)malloc(100);
	int line=0;
	int siz, i, k;
	char *voc = (char *)malloc(2*5);
	while(feof(qfile) == 0)
	{
		ques = fgets(ques, 100, qfile);
		if(ques == NULL)
			break;
		line++;
//		printf("Ques : %s\n", ques);
		siz = strlen(ques) - 1;
		ques[siz] = '\0';
//		printf("Find \"%s\" in \"%s\"\n", ques, argv[1]);
		if(strcmp(argv[1], ques) == 0)
			return line;
	}
	free(ques);
	free(voc);
	return 0;
}


