#include "helpers.h"
#include <math.h>

// Cap a colour value at 255 (a byte cannot hold more)
static int cap(int value)
{
    if (value > 255)
    {
        return 255;
    }
    return value;
}

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Dividing by 3.0 (not 3) makes it a decimal division, so round() has something to
            // round
            int average =
                round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);

            // All three channels equal means grey
            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Save the originals: each new value must use the OLD red, green and blue
            int red = image[i][j].rgbtRed;
            int green = image[i][j].rgbtGreen;
            int blue = image[i][j].rgbtBlue;

            int sepiaRed = cap(round(.393 * red + .769 * green + .189 * blue));
            int sepiaGreen = cap(round(.349 * red + .686 * green + .168 * blue));
            int sepiaBlue = cap(round(.272 * red + .534 * green + .131 * blue));

            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        // Only go halfway: going all the way would swap every pair twice and undo the reflection
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Copy of the original: we read from this and write to image
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int redSum = 0;
            int greenSum = 0;
            int blueSum = 0;
            int count = 0;

            // Visit the 3x3 box around pixel (i, j)
            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    int row = i + di;
                    int col = j + dj;

                    // Skip neighbours that fall off the image
                    if (row < 0 || row >= height || col < 0 || col >= width)
                    {
                        continue;
                    }

                    redSum += copy[row][col].rgbtRed;
                    greenSum += copy[row][col].rgbtGreen;
                    blueSum += copy[row][col].rgbtBlue;
                    count++;
                }
            }

            // (float) makes the division decimal so round() works
            image[i][j].rgbtRed = round(redSum / (float) count);
            image[i][j].rgbtGreen = round(greenSum / (float) count);
            image[i][j].rgbtBlue = round(blueSum / (float) count);
        }
    }
}
