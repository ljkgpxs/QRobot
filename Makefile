DEBUG = 
all:
	@echo "**Building tools.viewqr"
	@gcc viewqr.c -o viewqr -lpng -ljpeg -O3 -Wall $(DEBUG)
