#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXBYTES 512

int main(int argc, char *argv[])
{
    //Control for proper imput
    if (argc != 2 )
    {
        printf ("./recover filename");
        return 1;

    }

    //OPEN MEMORY CARD
    FILE *raw = fopen(argv[1], "r");

    //CONTROL IF IT'S A LEGIT FILE
    if (raw == NULL)
    {
        return 2;
    }

    //JPG COUNTER FOR NAMING FILES && READ BYTES FROM RAW STORING THEM IN TO BYTES ARRAY
    int jpg_counter = 0;
    unsigned char bytes [MAXBYTES];
    bool jpg_found = false;
    //FOR STORING JPEG IMAGE NAME
    char jpg_name [8];
    FILE *JPG;

    while (fread(bytes, MAXBYTES, 1, raw))
    {
        //CONTROL IF YOU FOUND A JPEG
        if (bytes[0] == 0xff && bytes[1] == 0xd8 && bytes[2] == 0xff && ( bytes[3] & 0xf0) == 0xe0 )
        {
           if (jpg_found == true)
           {
               int fclose (FILE *JPG);
               jpg_found = false;

            }
            //GIVE TO JPG_NAME THE NAME OF THE JPG NUMBER FOUND EX. 000.JPG - 001.JPG
            sprintf (jpg_name, "%03i.jpg", jpg_counter);

            //OPEN AND WRITE A NEW FILE WITH THE NAME OF THE JPG JUST FOUND
            JPG = fopen(jpg_name, "w");

            //WRITE DATA TO THAT FILE
            fwrite (bytes, sizeof(bytes), 1 ,JPG );
            jpg_found = true;
            jpg_counter++;
        }
        else
        {
            if (jpg_found == true)
            {
                fwrite (bytes, sizeof(bytes), 1, JPG);
            }
        }
    }
    printf("I have found %i JPG files\n", jpg_counter);
    int fclose (FILE *raw);
    int fclose (FILE *JPG);

}
