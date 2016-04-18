DEBUG = 
all:
	echo "**Building tools.viewqr"
	@gcc viewqr.c -o viewqr -lpng -O3 -Wall $(DEBUG)
