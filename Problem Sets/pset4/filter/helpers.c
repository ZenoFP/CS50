#include "helpers.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int z = 0; z < height; z++)
    {
        for(int y = 0; y < width; y++)
        {
            //calculate integer with the grayscale value
            int m = round( ( round (image[z][y].rgbtBlue) + round (image[z][y].rgbtGreen) + round(image[z][y].rgbtRed) ) / 3 );

            //all the values are equals, so blue = green = red = greyscale formula
            image[z][y].rgbtBlue = image[z][y].rgbtGreen = image[z][y].rgbtRed = m;

        }

    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int x = 0; x < height; x++)
    {
        for (int w = 0; w < width; w++)
        {
            //calculate values based on sepia formula
            int red = round( (0.393 * image[x][w].rgbtRed) + (0.769 * image[x][w].rgbtGreen) + ( 0.189 * image[x][w].rgbtBlue) );
            int green = round( (0.349 * image[x][w].rgbtRed) + (0.686 * image[x][w].rgbtGreen) + ( 0.168 * image[x][w].rgbtBlue) );
            int blue = round( (0.272 * image[x][w].rgbtRed) + (0.534 * image[x][w].rgbtGreen) + ( 0.131 * image[x][w].rgbtBlue) );

            //control if values gone above 255, if so set to 255 instead
            if (red > 255)
            {
                red = 255;
            }

            if (blue > 255)
            {
                blue = 255;
            }

            if (green > 255)
            {
                green = 255;
            }

            //transfer sepia formula's calculated values to the pixels
            image[x][w].rgbtBlue = blue;
            image[x][w].rgbtGreen = green;
            image[x][w].rgbtRed = red;


        }

    }


    return;
}




// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int v = 0; v < height; v++)
    {

        // for loop in every element of the row with u < width/2 to prevent even/odd differentiation
               for (int u = 0; u < (width/2); u++)
                {
                    //initialize values for switching
                    int r =  image[v][u].rgbtRed;
                    int g =  image[v][u].rgbtGreen;
                    int b = image[v][u].rgbtBlue;

                    //set one on the left as the ones on the right
                    image[v][u].rgbtBlue = image[v][width-u-1].rgbtBlue;
                    image[v][u].rgbtGreen = image[v][width-u-1].rgbtGreen;
                    image[v][u].rgbtRed = image[v][width-u-1].rgbtRed;

                    //set the last ones as the ones just switched
                    image[v][width-u-1].rgbtBlue = b;
                    image[v][width-u-1].rgbtGreen = g;
                    image[v][width-u-1].rgbtRed = r;
                }


    }


    return;
}



//ADJUST


// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //declare new rgtriple as big as the original one
    RGBTRIPLE copy[height][width];

    //declare avg colors and counter counters
    double avg_red;
    double avg_green;
    double avg_blue;
    int tt;
    int ss;
    int counter;

    //iterate every row
    for (int t = 0; t < height; t++)
    {
        //iterate every element of the row
        for (int s = 0; s < width; s++)
        {
            //set average colors counters to 0
            avg_red = 0;
            avg_green = 0;
            avg_blue = 0;
            counter = 0;

            //iterate through all the pixels around, if they exist
            for (int a = (t - 1); a == (t + 1); a++)
            {
                for (int b = (s - 1); b == (s + 1); b++)
                {
                    if (a < 0)
                    {continue;}

                    else if(a >= height)
                    {continue;}

                    else if(b < 0)
                    {continue;}

                    else if(b >= width)
                    {continue;}

                    //if row and coloumn exist, update averages + counter to make division later
                    avg_red += image[a][b].rgbtRed;
                    avg_green +=  image[a][b].rgbtGreen;
                    avg_blue +=  image[a][b].rgbtBlue;
                    counter = counter + 1;
                }

            }

            //if statement to avoid seg fault
            if (counter != 0)
            {
                copy[t][s].rgbtBlue = round(avg_blue/(double)counter);
                copy[t][s].rgbtGreen = round(avg_green/(double)counter);
                copy[t][s].rgbtRed = round(avg_red/(double)counter);
            }

            else
            {return;}

        }
    }

    //copy rgbt copy values to the real image rounded
    for (int f = 0; f < height; f++)
    {
        for (int u = 0; u < width; u++)
        {
            image[f][u].rgbtBlue = round (copy[f][u].rgbtBlue);
            image[f][u].rgbtGreen = round (copy[f][u].rgbtGreen);
            image[f][u].rgbtRed = round (copy[f][u].rgbtRed);
        }

    }

    return;
}
