#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"

/* Returns the Run Length Encoded [] for the 2D Input Array*/
int* encode(int m, int n, int src[m][n])
{
    int rLen;
    //int count[m*n];
    int numRows = m;
    int numCols = n;

    /* If all integers in the source [] are different,
    then size of destination [] would be twice of input [].*/
    int* dest = (int*)malloc(sizeof(int) * (numRows*numCols * 2 + 1));

    for(int i =0; i < numRows*numCols*2; i++){
      dest[i] = 0;
    }

    int i, l = 1;
    int countIdx = 0;

    // traverse the input string one by one
    for (i = 0; i < numRows*numCols; i++) {
        //for(int j = 0; j < numCols; j++){
            //if(1==1){//src[i][j] == 0){
              /* Copy the first occurrence of the new integer */
              dest[l] = src[i/numCols][i%numCols];

              // TESTING
              // printf("Number: %d", src[i][j]);

              /* Count the number of occurrences of the new integer */
              rLen = 1;
              while (i < numCols*numRows && src[i/numCols][i%numCols] == src[i/numCols][i%numCols+1] && rLen < 127) {
                  rLen++;
                  i++;
              }

              // TESTING
              // printf("    Freq: %d \n", rLen);

              dest[l+1] = rLen;
              l += 2;
              countIdx += 2;
            /*}else{
              dest[l] = src[i][j];
              l+=1;
              countIdx += 1;
            }*/

        //}


    }

    // terminate string and return
    // dest[l] = '\0';
    // dest[l] = NULL;
    // TESTING
    // for(int i=0;dest[i] != NULL;i++){
    //     printf("%d ", dest[i]);
    // }
    dest[0] = countIdx;
    return dest;
}

int8_t* decode(int8_t* src)
{
    int count;

    // need length of input string for for loop ending
    // i < src[0];

    /* If all integers in the source string are different,
    then size of destination [] would be twice of input [].*/
    int8_t* dest = (int8_t*)malloc(sizeof(int8_t) * ROWS*COLS);

    int i, j = 0, k;

    int srcLen = ((u_int8_t)src[0] << 8) + ((u_int8_t)src[1]);
    // traverse the input []
    // i< ROWS*COLS
    for (i = 2; i < srcLen; i+=2) {

        // grab the character
        dest[j] = (int8_t)src[i];
        // grab the number of occurences of the character
        if(i+1>srcLen){
          count = ROWS*COLS - 1- j;
        }else{
          count = src[i+1];
        }
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
