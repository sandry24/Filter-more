#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            RGBTRIPLE pixel = image[i][j];
            int sum = pixel.rgbtBlue + pixel.rgbtGreen + pixel.rgbtRed;
            int average = round(sum / 3.0);
            RGBTRIPLE pixel_gray = {average, average, average};
            image[i][j] = pixel_gray;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++){
        RGBTRIPLE row[width];
        for (int j = 0; j < width; j++){
            row[j] = image[i][width - j - 1];
        }
        for (int j = 0; j < width; j++){
            image[i][j] = row[j];
        }
    }
}

int max(int a, int b){
    if (a > b)
        return a;
    return b;
}

int min(int a, int b){
    if (a < b)
        return a;
    return b;
}

RGBTRIPLE blur_3x3(int height, int width, RGBTRIPLE image[height][width], int row, int col){
    float sum_red = 0, sum_green = 0, sum_blue = 0, count = 0;
    for (int i = max(row - 1, 0); i <= min(row + 1, height - 1); i++){
        for (int j = max(col - 1, 0); j <= min(col + 1, width - 1); j++){
            RGBTRIPLE pixel = image[i][j];
            sum_red += pixel.rgbtRed;
            sum_green += pixel.rgbtGreen;
            sum_blue += pixel.rgbtBlue;
            count++;
        }
    }
    RGBTRIPLE pixel_blurred = {round(sum_blue / count), round(sum_green / count), round(sum_red / count)};
    return pixel_blurred;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];

    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            temp[i][j] = blur_3x3(height, width, image, i, j);
         }
    }

    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            image[i][j] = temp[i][j];
        }
    }
}

RGBTRIPLE compute_edge(int height, int width, RGBTRIPLE image[height][width], int row, int col){
    int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    int gx_red = 0, gx_green = 0, gx_blue = 0;
    int gy_red = 0, gy_green = 0, gy_blue = 0;
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            int cur_i = row + (i - 1), cur_j = col + (j - 1);
            if(cur_i < 0 || cur_i >= height || cur_j < 0 || cur_j >= width)
                continue;

            RGBTRIPLE pixel = image[cur_i][cur_j];
            gx_red += pixel.rgbtRed * gx[i][j];
            gx_green += pixel.rgbtGreen * gx[i][j];
            gx_blue += pixel.rgbtBlue * gx[i][j];

            gy_red += pixel.rgbtRed * gy[i][j];
            gy_green += pixel.rgbtGreen * gy[i][j];
            gy_blue += pixel.rgbtBlue * gy[i][j];
        }
    }
    int red = min(round(sqrt(gx_red * gx_red + gy_red * gy_red)), 255);
    int green = min(round(sqrt(gx_green * gx_green + gy_green * gy_green)), 255);
    int blue = min(round(sqrt(gx_blue * gx_blue + gy_blue * gy_blue)), 255);
    RGBTRIPLE pixel_edge = {blue, green, red};
    return pixel_edge;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];

    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            temp[i][j] = compute_edge(height, width, image, i, j);
        }
    }

    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            image[i][j] = temp[i][j];
        }
    }
}
