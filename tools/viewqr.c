/* QRobot
 * Project By Ljk
 */

#include <stdio.h>
#include <stdlib.h>
#include <png.h>

#define DARK "\033[40;33m  \033[0m" 
#define WHILTE "\033[47;37m  \033[0m"
#define PNG_BYTES_TO_CHECK 4
#define ogr_x 7000
#define ogr_y 7000


unsigned char *tabr,*tabg,*tabb;
unsigned char *tab2r,*tab2g,*tab2b;
int image_x,image_y,depth,tempi, x, y;

int check_image(int ix,int iy,int id)
{
if ((ix>ogr_x) || (iy>ogr_y) || (id>32) || (ix<=0) || (iy<=0) || (id<=0)) {

    return 3;

}	ix++;
	iy++;
	if (ix*iy<256) ix=256;
	tabr=(unsigned char*)malloc(ix*iy);
	if (!tabr) fprintf(stderr,"Malloc error (tabr)\n");
	tabg=(unsigned char*)malloc(ix*iy);
	if (!tabg) fprintf(stderr,"Malloc error (tabg)\n");
	tabb=(unsigned char*)malloc(ix*iy);
	if (!tabb) fprintf(stderr,"Malloc error (tabb)\n");
	tab2r=(unsigned char*)malloc(ix*iy);
	if (!tab2r) fprintf(stderr,"Malloc error (tab2r)\n");
	tab2g=(unsigned char*)malloc(ix*iy);
	if (!tab2g) fprintf(stderr,"Malloc error (tab2g)\n");
	tab2b=(unsigned char*)malloc(ix*iy);
	if (!tab2b) fprintf(stderr,"Malloc error (tab2b)\n");

return 0;
}


int read_png(char *file_name)
{

	FILE *plik = fopen(file_name, "rb");
	if(plik == NULL)
		return 0;
	png_bytep 	row_pointers[4000];
	int row;
	unsigned char 		buf[PNG_BYTES_TO_CHECK];
	png_structp 	png_ptr;
	png_infop 	info_ptr;
	png_uint_32 	width, height;
	int 		bit_depth, color_type, interlace_type;
	
	if(!fread(buf, 1, PNG_BYTES_TO_CHECK,plik))
		return 0;
	tempi = png_sig_cmp(buf, (png_size_t)0, PNG_BYTES_TO_CHECK);
	if (tempi != 0)
		return 1;
	rewind(plik);
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(png_ptr == NULL)
		return 2;
	info_ptr = png_create_info_struct(png_ptr);
	if(info_ptr == NULL) {
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		return 2;
	}

	if(setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		return 2;
	}
	
	png_init_io(png_ptr, plik);    
	png_read_info(png_ptr, info_ptr);
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, NULL, NULL);
	image_x = width;
	image_y = height;
	depth = bit_depth;
	tempi = png_get_channels(png_ptr, info_ptr);
	check_image(image_x, image_y, depth);

	if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
	png_set_gray_to_rgb(png_ptr);
	png_set_expand(png_ptr);
	png_read_update_info(png_ptr, info_ptr);
	tempi = png_get_channels(png_ptr, info_ptr);

	for (row = 0; row < height ; row++){
		row_pointers[row] = png_malloc(png_ptr, png_get_rowbytes(png_ptr, info_ptr));
	}
	    
	png_read_image(png_ptr, row_pointers);
	png_read_end(png_ptr, info_ptr);

	for (y = 0; y < image_y; y++) {
		for (x = 0; x < image_x; x++)
		{
			tabr[x+y*image_x] = row_pointers[y][x*tempi];
			tabg[x+y*image_x] = row_pointers[y][x*tempi+1];
			tabb[x+y*image_x] = row_pointers[y][x*tempi+2];
		}
	}
	png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
	fclose(plik);
	return 1;
}

int main(int argc, char **argv)
{
	if(argc <= 1) {
		printf("Usage: viewqr [ PNG QR file path ]\n");
		return 0;
	}
	int res;
	if(!(res = read_png(argv[1]))) {
		fprintf(stderr, "Read png file failed(%d)\n", res);
		return 1;
	}
	unsigned char ixy[image_y][image_x];
	for(tempi = 0, x = 0, y = 0; tempi < image_x * image_y; tempi++) {
		ixy[y][x++] = tabr[tempi];
		if((tempi + 1) % image_x == 0) { 
			y++;
			x = 0;
		}
	}
	printf("\n\n    ");
	for(y = 0; y < image_y/5; y++) {
		for(x = 0; x < image_x/5; x++)
			if(ixy[y*5][x*5] == 255)
				printf(WHILTE);
			else
				printf(DARK);
		printf("\n    ");
	}
	printf("\n\n");
	return 0;
}

