/*hw1.c*/
/**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//  the struct to form the matrix
typedef struct {
	int width;  // the width of matrix
	int height; // the height of matrix
}MartixProperty;

//to achieve the numbers in the string and put into the matrix
void GetNumberByString(char *buffer, int *martix, int needcount) {
	// place is the location in process
	int place = 0;
	// tmp save the number from the string
	int tmp = 0;
	//  jump over the space
	bool bSave = false;
	// go through the string
	for (int i = 0; i < strlen(buffer); ++i)
	{
		//c is the one in process
		char c = *(buffer + i);
		// if this char is a number
		if ((c >= 48) && (c <= 57))
		{
			tmp = tmp * 10 + c - 48;
			bSave = true;
		}
		// if it is a space
		if (c == 32)
		{
			// if before this one is not a space, put it into the matrix
			if (bSave)
			{
				*(martix + place) = tmp;
				place++;
				tmp = 0;
				bSave = false;
			}
			// if it goes beyond the domain, exit the loop
			if (place >= needcount || i + 1 == strlen(buffer))
			{
				return;
			}
		}
		// if the string is over,save the last char and exit
		if (c == '\n' || i + 1 == strlen(buffer))
		{
			*(martix + place) = tmp;
			place++;
			tmp = 0;
			bSave = false;
			if (c == '\n' || place >= needcount || c == '\0' || i + 1 == strlen(buffer))
			{
				return;
			}
		}
	}
}

// the calculation of matrix multi.
void MartixMulti(int *martix1, int *martix2, int *martixout, MartixProperty martixPro1, MartixProperty martixPro2) {
	int i, j, k, tmp;
	// to make sure the width of the first matrix is equal to the second matrix
	int width = martixPro1.width;
	if (width != martixPro2.height)
	{
		printf("input martix error ,martixPro1 col must equal martixPro2 row\n");
		return;
	}
	//go through matrix 1 's line
	for (i = 0; i < martixPro1.height; ++i)
	{
		//go through matrix 2's column
		for (j =0; j < martixPro2.width; ++j)
		{
			tmp = 0;
			//use matrix 1's line i ,column k times matrix 2's line k ,column i and get the sum
			for (k = 0; k < width; ++k)
			{
				tmp += *(martix1 + i * width + k) * *(martix2 + k * martixPro2.width + j);
			}
			//save the results in matrix
			*(martixout + i *martixPro2.width + j) = tmp;
		}
	}
}

int getspacecount(int maxnumber, int number) {
    char str[32] = {0};
    sprintf(str, "%d\n", maxnumber);
    int maxlength = strlen(str);
    sprintf(str, "%d\n", number);
    int numlength = strlen(str);
    return maxlength - numlength;
}

//print matrix
void PrintMartix(int *martix, MartixProperty martixPro) {

    int *maxnum = (int *)malloc(sizeof(int) *  martixPro.width);
    int i, j, k;
    for (i = 0; i < martixPro.width; ++i)
    {
        *(maxnum + i) = *(martix + i);
        for (j = 0; j < martixPro.height; ++j)
        {
            *(maxnum + i) = *(martix + j * martixPro.width + i) >  *(maxnum + i) ? *(martix + j * martixPro.width + i) : *(maxnum + i);
        }
    }

	//go through the lines
	for (i = 0; i < martixPro.height; ++i)
	{
		printf("[");
		//go through columns
		for (j = 0; j < martixPro.width; ++j)
		{
			//print the matrix's line i ,column j
			if(j != 0)
				printf(" ");
            int spacenumber = getspacecount(*(maxnum + j), *(martix + i * martixPro.width + j));
            for (k = 0; k <spacenumber; ++k)
            {
                printf(" ");
            }
			printf("%d", *(martix + i * martixPro.width + j));
		}
		printf("]\n");
	}
}

int main(int argc, char *argv[]) {
	if (argc != 5)
	{
		fprintf(stderr,"ERROR: Invalid inputs!\n");
		return EXIT_FAILURE;
	}
	MartixProperty martixPro1, martixPro2, martixProOut;
	//get the information we need for width and height
	martixPro1.height = atoi(argv[1]);
	martixPro1.width = atoi(argv[2]);
	martixPro2.height = atoi(argv[3]);
	martixPro2.width = atoi(argv[4]);
	// warn the wrong input
	if (martixPro1.width != martixPro2.height)
	{
		fprintf(stderr,"ERROR: Invalid inputs!\n");
		return EXIT_FAILURE;
	}
	martixProOut.height = martixPro1.height;
	martixProOut.width = martixPro2.width;

	// give martix space to save and an initial value 0
	int *martix1 = (int *)malloc(martixPro1.width * martixPro1.height * sizeof(int));
	int *martix2 = (int *)malloc(martixPro2.width * martixPro2.height * sizeof(int));
	memset(martix1, 0, martixPro1.width * martixPro1.height * sizeof(int));
	memset(martix2, 0, martixPro2.width * martixPro2.height * sizeof(int));

	int *martixout = (int *)malloc(martixPro1.height * martixPro2.width * sizeof(int));
	memset(martixout, 0, martixPro1.height * martixPro2.width * sizeof(int));

	//buffer to save every line user input
	char buffer[1024];

	printf("Please enter the values for the first matrix (%dx%d):\n", martixPro1.height, martixPro1.width);
	// input the first matrix 
	for (int i = 0; i < martixPro1.height; ++i)
	{
		fgets(buffer, 1024, stdin);
		// check whether they are available
		for (char *c = buffer; *c != '\n' && *c != '\0'; ++c)
		{
			if (*c != 32 && !(*c >= 48 && *c <= 57))
			{
				printf("input %c[%d] is not number\n", *c, *c);
				getchar();
				exit(0);
			}
		}
		// if they are all right ,save the number into the matrix
		GetNumberByString(buffer, martix1 + i * martixPro1.width, martixPro1.width);
	}

	printf("Please enter the values for the second matrix (%dx%d):\n", martixPro2.height, martixPro2.width);
	//same as above
	for (int i = 0; i < martixPro2.height; ++i)
	{
		fgets(buffer, 1024, stdin);
		for (char *c = buffer; *c != '\n' && *c != '\0'; ++c)
		{
			if (*c != 32 && !(*c >= 48 && *c <= 57))
			{
				printf("input %c[%d] is not number\n", *c, *c);
				getchar();
				exit(0);
			}
		}
		GetNumberByString(buffer, martix2 + i * martixPro2.width, martixPro2.width);
	}

	printf("\n");
	// print out
	PrintMartix(martix1, martixPro1);
	printf("multiplied by\n");
	PrintMartix(martix2, martixPro2);

	// do the calculation
	MartixMulti(martix1, martix2, martixout, martixPro1, martixPro2);
	printf("equals\n");
	//print out the result
	PrintMartix(martixout, martixProOut);
	free(martix1);
	free(martix2);
	free(martixout);
	return EXIT_SUCCESS;
}