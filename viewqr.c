/* Project By ljk
 * http://ljkgpxs.xyz
 */

#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include <jpeglib.h>
#include <setjmp.h>
#include <string.h>

#define DARK "\033[40;33m  \033[0m" 
#define WHILTE "\033[47;37m  \033[0m"
#define PNG_BYTES_TO_CHECK 4
#define ogr_x 7000
#define ogr_y 7000

unsigned char *tabr,*tabg,*tabb;
unsigned char *tab2r,*tab2g,*tab2b;
int image_x,image_y,depth,tempi, x, y;

struct my_error_mgr {
    struct jpeg_error_mgr pub;
	jmp_buf setjmp_buffer;
};
typedef struct my_error_mgr * my_error_ptr;

int streq(const char *s1, const char *s2) { return strcmp(s1, s2) == 0; }


int check_image(int ix,int iy,int id)
{
	if ((ix>ogr_x) || (iy>ogr_y) || (id>32) || (ix<=0) || (iy<=0) || (id<=0)) {
	    return 3;
	}	
	ix++;
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

METHODDEF(void) my_error_exit(j_common_ptr cinfo)
{
	my_error_ptr myerr = (my_error_ptr) cinfo->err;
	(*cinfo->err->output_message)(cinfo);
	longjmp(myerr->setjmp_buffer,1);
}

int read_jpg(char *file_name)
{
	FILE *plik = fopen(file_name, "rb");
	if(plik == NULL)
		return 0;
	short int JPsyg;
	struct jpeg_decompress_struct cinfo;
	struct my_error_mgr jerr;
	JSAMPARRAY buffer;
	int row_stride,jaka;

	if(!fread(&JPsyg, sizeof (short int), 1, plik))
		return 0;
	if (JPsyg != -9985) {
		return 1;
	}
	rewind(plik);
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;

	if(setjmp(jerr.setjmp_buffer)) {
		jpeg_destroy_decompress(&cinfo);
		return 2;
	}
	
    
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo,plik);    
	jpeg_read_header(&cinfo,TRUE);
	jpeg_start_decompress(&cinfo);    

	image_x = cinfo.output_width;
	image_y = cinfo.output_height;
	jaka = cinfo.num_components;
	check_image(image_x, image_y, 8);

	row_stride = cinfo.output_width*cinfo.output_components;
	buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo,JPOOL_IMAGE, row_stride, 1);
	y = 0;
	while (cinfo.output_scanline < cinfo.output_height) {    
		jpeg_read_scanlines(&cinfo, buffer, 1);
		if (jaka == 3)  
			for(x=0; x < image_x; x++)
			{
				tabr[x+y*image_x] = buffer[0][x*3];
				tabg[x+y*image_x] = buffer[0][x*3+1];
				tabb[x+y*image_x] = buffer[0][x*3+2];

			}
		else
			for(x = 0; x < image_x; x++)
			{
				tabr[x+y*image_x] = buffer[0][x];
				tabg[x+y*image_x] = tabr[x+y*image_x];
				tabb[x+y*image_x] = tabr[x+y*image_x];

			}
		y++;	    
	}
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	rewind(plik);
	fseek(plik, 20, SEEK_CUR);
	tempi=fgetc(plik);
	if ((tempi == 255) && (fgetc(plik) == 254)) {
		fprintf(stderr,"Comment         : ");
		tempi = fgetc(plik);
		tempi = fgetc(plik);
    		for (x=0; x < tempi-2; x++) {	
			printf("%c", fgetc(plik));
		}
		printf("\n");
	}
	return 1;
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
	int res, unit_len = 0;
	unsigned char **ixy;
	if(argc <= 2) {
		printf("Usage: viewqr [-jpg|-png] [ File Path ]\n");
		return 0;
	}

	// 读取图片数据
	argv++;
	if(streq(*argv, "-png")) {
		if(!(res = read_png(*(++argv)))) {
			fprintf(stderr, "Read png file failed(%d)\n", res);
			return 1;
		}
		unit_len = 5;
	} else if(streq(*argv, "-jpg")) {
		if(!(res = read_jpg(*(++argv)))) {
			fprintf(stderr, "Read jpg file failed(%d)\n", res);
			return 1;
		}
		unit_len = 7;
	} else {
		printf("Usage: viewqr [-jpg|-png] [ File Path ]\n");
		return 0;
	}

	// 申请二维指针内存
	ixy = (unsigned char **)malloc(sizeof(unsigned char**)*image_y);
	for(tempi = 0; tempi < image_y; tempi++) {
		ixy[tempi] = (unsigned char *)malloc(sizeof(unsigned char *)*image_x);
	}

	// 转换R的原数据为坐标形式，并进行简单降噪
	for(tempi = 0, x = 0, y = 0; tempi < image_x * image_y; tempi++) {
		ixy[y][x++] = tabr[tempi]>100 ? 255:0;
		if((tempi + 1) % image_x == 0) { 
			y++;
			x = 0;
		}
	}

	// 打印二维码
	if(unit_len == 5)
		printf("\n\n    ");
	for(y = 0; y < image_y/unit_len; y++) {
		for(x = 0; x < image_x/unit_len; x++)
			if(ixy[y*unit_len][x*unit_len] == 255)
				printf(WHILTE);
			else
				printf(DARK);
		if(unit_len == 5)
			printf("\n    ");
		else
			printf("\n");
	}
	if(unit_len == 5)
		printf("\n\n");

	// 释放内存
	for(tempi = 0; tempi < image_y; tempi++) {
		free(ixy[tempi]);
	}
	free(ixy);
	return 0;
}

