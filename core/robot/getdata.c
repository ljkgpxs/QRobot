#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	if(argc < 3){
		fprintf(stderr, "Give Me The Strings\n");
		return 1;
	}


	char *str = (char *)malloc(3*300);
	int i, k;
	if(argv[2][0] == '1'){	// Get the Question
		if(argv[1][0] == '?')
			i=1;
		else
			i=3;
		for(k=0; i < strlen(argv[1]); i++, k++){
				if(argv[1][i] == '-'){
					str[k] = '\0';
					printf("%s", str);
					return 0;
				} else {
//					printf("%s", str);	
					str[k] = argv[1][i];
				}
		}
	}
	else if(argv[2][0] == '2'){	//Get the Ans
		int s=0;
		for(i=1, k=0; i < strlen(argv[1]); i++, k++){
			if(s == 1){
				str[k] = argv[1][i];
			}
			if(argv[1][i] == '-') {
				s=1;
				k=-1;
			}
		}
		str[k] = '\0';
		printf("%s", str);
		return 0;
	}
	return 0;

}


