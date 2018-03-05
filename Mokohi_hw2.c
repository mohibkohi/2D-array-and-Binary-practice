/*
 * Mohib kohi
 * TCSS 333.
 */
#include <stdio.h>
//#include <stdio.h>
#include <string.h>
/* This program changes the coloring of the image.
   Each pixel is made from a combination of Red, Green, Blue.
   For each of these 3 colors, there is a number indicating the intensity or 
   brightness.
   The current functionalities are modifying image to sepia, blur, horizontal, 
   and 180 rotation.
 */

#define HEADER_SIZE 54
//#define HEIGHT 240
//#define WIDTH 320

void sepia();
void blur();
void horizontal();
void rotation();

/**
 * Main method to run the program.
 * @return 0.
 */
int main(void) {
    //height and the width of the 2D arrays
    int height, width;

    printf("Enter the filename: ");
    //char array for file name.
    char fname[99];
    scanf("%123s", fname);
    strcat(fname, ".bmp");

    FILE *infile;
    infile = fopen(fname, "rb");

    //while loop to find the file
    while (!infile) {
        printf("Enter the filename: ");
        scanf("%123s", fname);
        strcat(fname, ".bmp");
        //open the file
        infile = fopen(fname, "rb");
    }

    printf("Enter height and width (in pixels): ");
    //the typed number
    scanf("%d %d", &height, &width);

    //first output file.
    FILE *outfile = fopen("copy1.bmp", "wb");

    char header[HEADER_SIZE];
    //create two dimensional array of pixels.
    unsigned char pixels[height][width * 3];

    //read the file.
    fread(header, 1, HEADER_SIZE, infile);
    fread(pixels, 1, height * width * 3, infile);

    //write the header.
    fwrite(header, sizeof (char), HEADER_SIZE, outfile);
    //Another array to modify.
    unsigned char pixelsToWrite[height][width * 3];

    //Memory copy the original 2d array into the copy
    memcpy(pixelsToWrite, pixels, sizeof (pixels));
    //calling sepia method on copy array.
    sepia(height, width, pixelsToWrite);

    //write the formated 
    fwrite(pixelsToWrite, sizeof (char), height * width * 3, outfile);

    //Memory copy the original 2d array into the copy
    memcpy(pixelsToWrite, pixels, sizeof (pixels));

    //first output file.
    FILE *outfile1 = fopen("copy2.bmp", "wb");

    //write the header.
    fwrite(header, sizeof (char), HEADER_SIZE, outfile1);
    //calling blur method on copy array.
    blur(height, width, pixelsToWrite);

    //write the formated image
    fwrite(pixelsToWrite, sizeof (char), height * width * 3, outfile1);

    //Memory copy the original 2d array into the copy
    memcpy(pixelsToWrite, pixels, sizeof (pixels));

    //first output file.
    FILE *outfile2 = fopen("copy3.bmp", "wb");

    //write the formated image
    fwrite(header, sizeof (char), HEADER_SIZE, outfile2);

    //calling horizontal method on copy array.
    horizontal(height, width, pixelsToWrite);
    fwrite(pixelsToWrite, sizeof (char), height * width * 3, outfile2);

    //Memory copy the original 2d array into the copy
    memcpy(pixelsToWrite, pixels, sizeof (pixels));
    FILE *outfile3 = fopen("copy4.bmp", "wb");

    fwrite(header, sizeof (char), HEADER_SIZE, outfile3);
    //calling rotation method on copy array.
    rotation(height, width, pixelsToWrite);
    fwrite(pixelsToWrite, sizeof (char), height * width * 3, outfile3);

    //close the opened files
    fclose(infile);
    fclose(outfile);
    fclose(outfile1);
    fclose(outfile2);
    fclose(outfile3);
    
    printf("Done. Check the generated images.");
    return 0;
}

/**
 * Modify image to sepia.
 * @param height of the image.
 * @param width of the image.
 * @param pixelsToWrite copy 2d array.
 */
void sepia(int height, int width, unsigned char pixelsToWrite[][width * 3]) {
    int r, c;

    //the SEPIA
    for (r = 0; r < height; r++) {
        for (c = 0; c < width * 3; c += 3) {

            unsigned char tempB = pixelsToWrite[r][c];
            unsigned char tempG = pixelsToWrite[r][c + 1];
            unsigned char tempR = pixelsToWrite[r][c + 2];

            //if more than 255 just set the pixels to 255
            if ((pixelsToWrite[r][c] * 0.131 + pixelsToWrite[r][c + 1] * 0.534 +
                    pixelsToWrite[r][c + 2] * 0.272) > 255) {
                tempB = 255;

            } else {
                tempB = pixelsToWrite[r][c] * 0.131 + pixelsToWrite[r][c + 1] * 0.534 +
                        pixelsToWrite[r][c + 2] * 0.272;
            }

            //if more than 255 just set the pixels to 255
            if ((pixelsToWrite[r][c] * 0.168 + pixelsToWrite[r][c + 1] * 0.686 +
                    pixelsToWrite[r][c + 2] * 0.349) > 255) {
                tempG = 255;
            } else {
                tempG = pixelsToWrite[r][c] * 0.168 + pixelsToWrite[r][c + 1] * 0.686 +
                        pixelsToWrite[r][c + 2] * 0.349;
            }


            //if more than 255 just set the pixels to 255
            if ((pixelsToWrite[r][c] * 0.189 + pixelsToWrite[r][c + 1] * 0.769 +
                    pixelsToWrite[r][c + 2] * 0.393) > 255) {
                tempR = 255;
            } else {
                tempR = pixelsToWrite[r][c] * 0.189 + pixelsToWrite[r][c + 1] * 0.769 +
                        pixelsToWrite[r][c + 2] * 0.393;
            }

            //printf("%d \n", temp);
            //pixels[r][c] = pixels[r][c];
            pixelsToWrite[r][c] = tempB;
            pixelsToWrite[r][c + 1] = tempG;
            pixelsToWrite[r][c + 2] = tempR;
        }
    }

}

/**
 * Modify image to blur.
 * @param height of the image.
 * @param width of the image.
 * @param pixelsToWrite copy 2d array.
 */
void blur(int height, int width, unsigned char pixelsToWrite[][width * 3]) {
    int r, c;

    for (r = 0; r < height; r++) {
        for (c = 0; c < width * 3; c++) {
            if (r > 0 && r != height - 1 && c > 0 && c != (width * 3) - 1) {
                //take all the simillar pixels
                unsigned char current = pixelsToWrite[r][c];
                unsigned char currentTop = pixelsToWrite[r - 1][c];
                unsigned char currentBottum = pixelsToWrite[r + 1][c];

                unsigned char topLeft = pixelsToWrite[r - 1][c - 3];
                unsigned char middleLeft = pixelsToWrite[r][c - 3];
                unsigned char bottumLeft = pixelsToWrite[r + 1][c - 3];

                unsigned char middleRight = pixelsToWrite[r][c + 3];
                unsigned char topRight = pixelsToWrite[r - 1][c + 3];
                unsigned char bottumRight = pixelsToWrite[r + 1][c + 3];

                //average of the pixels around
                pixelsToWrite[r][c] = (current + currentTop + currentBottum +
                        topLeft + middleLeft + bottumLeft +
                        middleRight + topRight + bottumRight) / 9;
            } else {
                unsigned char temp1 = pixelsToWrite[r][c];
                pixelsToWrite[r][c] = temp1;
            }
        }
    }
}

/**
 * Modify image to horizontal mirror image.
 * @param height of the image.
 * @param width of the image.
 * @param pixelsToWrite copy 2d array.
 */
void horizontal(int height, int width, unsigned char pixelsToWrite[][width * 3]) {
    int r, c;

    //Horizontal mirror
    for (r = 0; r < height; r++) {
        for (c = 0; c < (width * 3) / 2; c += 3) {
            //take all the mirror pixels.
            unsigned char tempB = pixelsToWrite[r][c];
            unsigned char tempG = pixelsToWrite[r][c + 1];
            unsigned char tempR = pixelsToWrite[r][c + 2];

            //assign the mirror pixels
            pixelsToWrite[r][(width * 3) - 1 - c - 2] = tempB;
            pixelsToWrite[r][(width * 3) - 1 - c - 1] = tempG;
            pixelsToWrite[r][(width * 3) - 1 - c] = tempR;
        }
    }
}

/**
 * Modify image to rotation image.
 * @param height of the image.
 * @param width of the image.
 * @param pixelsToWrite copy 2d array.
 */
void rotation(int height, int width, unsigned char pixelsToWrite[][width * 3]) {
    int r, c;

    //180 rotate
    for (r = 0; r < height / 2; r++) {
        for (c = 0; c < (width * 3); c += 3) {
            //take the original pixels.
            unsigned char frontB = pixelsToWrite[r][c];
            unsigned char frontG = pixelsToWrite[r][c + 1];
            unsigned char frontR = pixelsToWrite[r][c + 2];

            //set the rotation pixels.
            pixelsToWrite[r][c] = pixelsToWrite[(height - 1) - r][(width * 3) - 1 - c - 2];
            pixelsToWrite[r][c + 1] = pixelsToWrite[(height - 1) - r][(width * 3) - 1 - c - 1];
            pixelsToWrite[r][c + 2] = pixelsToWrite[(height - 1) - r][(width * 3) - 1 - c];

            //update the new rotation pixels.
            pixelsToWrite[(height - 1) - r][(width * 3) - 1 - c - 2] = frontB;
            pixelsToWrite[(height - 1) - r][(width * 3) - 1 - c - 1] = frontG;
            pixelsToWrite[(height - 1) - r][(width * 3) - 1 - c] = frontR;
        }
    }
}
 


//    for (r = 0; r < height; r++) {
//        for (c = 0; c < (width * 3)/2; c += 3) {
//            unsigned char temp = pixels[r][c];
//            pixelsToWrite[r][width - c - 1] = temp;
//        }
//    }


//Extra credit IMAGES
//    for (r = 0; r < height; r++) {
//        for (c = 0; c < width * 3; c += 3) {
//            float temB;
//
//            if (r > 0 && r != height - 1 && c > 0 && c != (width * 3) - 1) {
//                int i, j;
//                for (i = r - 1; i < r + 2; i++) {
//                    for (j = c - 1; j < c + 2; j++) {
//                        temB += pixels[i][j];
//                    }
//                }
//
//                pixelsToWrite[r][c] = temB / 9;
//                //temB = NULL;
//                temB = 0;
//            } else {
//                unsigned char temp1 = pixels[r][c];
//                pixelsToWrite[r][c] = temp1;
//            }
//
//            float temG;
//            if (r > 0 && r + 1 != height - 1 && c > 0 && c + 1 != (width * 3) - 1) {
//                int i, j;
//                for (i = r - 1; i < r + 2; i++) {
//                    for (j = c; j < c + 3; j++) {
//                        temG += pixels[i][j];
//                    }
//                }
//
//                pixelsToWrite[r][c + 1] = temG / 9;
//                //temG = NULL;
//                temG = 0;
//            } else {
//                unsigned char temp1 = pixels[r][c + 1];
//                pixelsToWrite[r][c + 1] = temp1;
//            }
//
//            float temR;
//            if (r > 0 && r + 2 != height - 1 && c > 0 && c + 2 != (width * 3) - 1) {
//                int i, j;
//                for (i = r - 1; i < r + 2; i++) {
//                    for (j = c + 1; j < c + 4; j++) {
//                        temR += pixels[i][j];
//                    }
//                }
//
//                pixelsToWrite[r][c + 2] = temR / 9;
//                //temR = NULL;
//                temR = 0;
//            } else {
//                unsigned char temp1 = pixels[r][c + 2];
//                pixelsToWrite[r][c + 2] = temp1;
//            }
//
//        }
//    }


