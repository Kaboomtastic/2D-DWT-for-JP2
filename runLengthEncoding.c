#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"

/* Returns the Run Length Encoded [] for the 2D Input Array*/
int* encode(int m, int n, int src[m][n])
{
    int rLen;
    int count[m*n];
    int numRows = m;
    int numCols = n;

    /* If all integers in the source [] are different,
    then size of destination [] would be twice of input [].*/
    int* dest = (int*)malloc(sizeof(int) * (numRows*numCols * 2 + 1));

    int i, l = 0;
    int count = 0;

    // traverse the input string one by one
    for (i = 0; i < numRows; i++) {
        for(int j = 0; j < numCols; j++){

            /* Copy the first occurrence of the new integer */
            dest[l] = src[i][j];

            // TESTING
            // printf("Number: %d", src[i][j]);

            /* Count the number of occurrences of the new integer */
            rLen = 1;
            while (j + 1 < numCols && src[i][j] == src[i][j+1]) {
                rLen++;
                j++;
            }

            // TESTING
            // printf("    Freq: %d \n", rLen);

            dest[l+1] = rLen;
            l += 2;
            count += 2;
        }


    }

    // terminate string and return
    // dest[l] = '\0';
    // dest[l] = NULL;
    // TESTING
    // for(int i=0;dest[i] != NULL;i++){
    //     printf("%d ", dest[i]);
    // }
    dest[0] = count;
    return dest;
}

int* decode(int* src)
{
    int count;

    /* If all integers in the source string are different,
    then size of destination [] would be twice of input [].*/
    int* dest = (int*)malloc(sizeof(int) * ROWS*COLS);

    int i, j = 0, k;

    // traverse the input []
    // i< ROWS*COLS
    for (i = 0; src[i] != NULL; i+=2) {

        // grab the character
        dest[j] = src[i];
        // grab the number of occurences of the character
        count = src[i+1];
        for(k=0;k<count;k++){
            dest[j++] = src[i];
        }

    }

    /*terminate the destination [] */
    // dest[j] = NULL;
    return dest;
}

/*driver program to test above function */
// int main()
// {
//     // char str[] = "geeksforgeeks";
//     // int str[16] = {1,1,1,1,4,4,4,4,5,5,5,5,6,6,6,6};
//     // printf("\nEnter a value: ");
//     //scanf("%d", str);
//
//     int str2[4][4] = {{1,1,2,2},{3,3,3,3},{5,5,4,5},{6,7,7,7}};
//     printf("\nInput []: \n");
//     for(int i=0;i<sizeof(str2)/sizeof(str2[0]);i++){
//         for(int j=0;j<sizeof(str2[0])/sizeof(str2[0][0]);j++){
//             printf("%d", str2[i][j]);
//         }
//         printf("\n");
//     }
//
//     // for(int i=0;i<sizeof(str);i++){
//     //     str2[i/(sizeof(str2)/sizeof(str2[0]))][i%(sizeof(str2[0])/sizeof(str2[0][0]))] = str[i];
//     // }
//
//     int* enc = (int*)malloc(sizeof(int)*16);
//     enc = encode(4, 4, str2);
//
//     // printf("\nNum of Elements: %d", sizeof(enc)/sizeof(enc[0]));
//
//     printf("\nEncoded Stream: ");
//     for(int i=0;enc[i]!=NULL;i++){
//         printf("%d", enc[i]);
//     }
//     printf("\n");
//
//     int* dec = decode(enc);
//     // printf("\nDecoded string: %s \n\n", dec);
//     printf("\nDecoded Stream: ");
//     for(int i=0;dec[i]!=NULL;i++){
//         printf("%d", dec[i]);
//     }
//     printf("\n");
//
// }
