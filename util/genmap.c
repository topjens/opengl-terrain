#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

int main(int argc, char *argv[])
{
		register int i, j;
		
		const int width = 800;
		const int height = 600;

		int filesize = 54 + 3 * width * height;

		FILE *fp;

		unsigned char *img = NULL;

		img = calloc(width*height*3, 1);
	
		for(j = 0; j < height; j++)
				for(i = 0; i < width; i++) {
						int xx = i, yy = (height-1)-j;
						float x = (float)i/(float)width;
						float y = (float)j/(float)height;
						float h = (sin(4*M_PI*x)+sin(4*M_PI*y)+sin(16*M_PI*x)*sin(16*M_PI*y))*0.125+2.125;
						unsigned char pixel = (unsigned char)(h*255/5.25);

					    img[(xx + yy * width) * 3 + 2] = pixel;
						img[(xx + yy * width) * 3 + 1] = pixel;
						img[(xx + yy * width) * 3] = pixel;
				}
						

		unsigned char bmpfileheader[14] = {'B','M',0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0};
		unsigned char bmpinfoheader[40] = {40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0};
		unsigned char bmppad[3] = {0, 0, 0};

		bmpfileheader[2]  = (unsigned char)(filesize);
		bmpfileheader[3]  = (unsigned char)(filesize>>8);
		bmpfileheader[4]  = (unsigned char)(filesize>>16);
		bmpfileheader[5]  = (unsigned char)(filesize>>24);

		bmpinfoheader[4]  = (unsigned char)(width);
		bmpinfoheader[5]  = (unsigned char)(width>>8);
		bmpinfoheader[6]  = (unsigned char)(width>>16);
		bmpinfoheader[7]  = (unsigned char)(width>>24);
		bmpinfoheader[8]  = (unsigned char)(height);
		bmpinfoheader[9]  = (unsigned char)(height>>8);
		bmpinfoheader[10] = (unsigned char)(height>>16);
		bmpinfoheader[11] = (unsigned char)(height>>24);

		fp = fopen("map.bmp", "w");

		if(!fp) {
				free(img);
				fprintf(stderr, "Could not pen map.bmp\n");
				exit(EXIT_FAILURE);
		}
		
		fwrite(bmpfileheader, 1, 14, fp);
		fwrite(bmpinfoheader, 1, 40, fp);

		for(int i = 0; i < height; i++) {
				fwrite(img + (width * (height - i - 1) * 3), 3, width, fp);
				fwrite(bmppad, 1, (4 - (width * 3) % 4) % 4, fp);
		}

		free(img);

		fclose(fp);
}
