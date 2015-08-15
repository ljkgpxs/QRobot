#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define	LONG	3*300

int main(int argc, char **argv)
{
	if(argc < 3)
		return 0;
	int line = atoi(argv[1]);
	FILE *afile = fopen(argv[2], "r");
	if(afile == NULL)
		return 1;
	int cur;
	char *ans = (char *)malloc(LONG);
	for(cur = 1; cur <= line; cur++) {
		ans = fgets(ans, LONG, afile);
		if(ans == NULL && cur != line-1) 
			return 0;
	}
	printf("%s", ans);
	return line;
}

