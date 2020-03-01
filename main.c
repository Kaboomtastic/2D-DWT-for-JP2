#include "constants.h"
#include "dwt.c"
#include "huffmanEncoding.c"
#include "runLengthEncoding.c"

float R[ROWS][COLS],G[ROWS][COLS],B[ROWS][COLS];
float Y[ROWS][COLS],U[ROWS][COLS],V[ROWS][COLS];
float subU[ROWS/2][COLS/2],subV[ROWS/2][COLS/2];

void compress(){
  for(i = 0; i <ROWS; i++){
    rowdwt(R,i);
  }
  for(i = 0; i <COLS; i++){
    coldwt(R,i);
  }
  for(i = 0; i<ROWS/2; i++){
    partialrowdwt(R,i+ROWS/2, COLS/2, COLS/2);
  }
  for(i = 0; i <COLS/2; i++){
    partialcoldwt(R,i+COLS/2, ROWS/2, ROWS/2);
  }

  quantizer(R,ROWS/2,COLS/2,ROWS/2,COLS/2,4);
  quantizer(R,0,COLS/2,ROWS/2,COLS/2,2);
  quantizer(R,ROWS/2,0,ROWS/2,COLS/2,2);

  // run Length Encoding
  int* runLength = encode(sizeof(R)/sizeof(R[0]), sizeof(R[0]),sizeof(R[0][0]), R);
  int length = sizeof(runLength)/sizeof(runLength[0]);

  //huffman Encoding
  huffEncoding(runLength, length);


}


void decompress(){

  // run Length Decoding
  // int* decode = decode();

  dequantizer(ROWS/2,COLS/2,ROWS/2,COLS/2,4);
  dequantizer(0,COLS/2,ROWS/2,COLS/2,2);
  dequantizer(ROWS/2,0,ROWS/2,COLS/2,2);

  for(i = 0; i <COLS/2; i++){
    partialcoliwt(i+COLS/2, ROWS/2, ROWS/2);
  }
  for(i = 0; i<ROWS/2; i++){
    partialrowiwt(i+ROWS/2, COLS/2, COLS/2);
  }
  for(i = 0; i <COLS; i++){
    coliwt(i);
  }
  for(i = 0; i <ROWS; i++){
    rowiwt(i);
  }

}



void main(){
  int i,j;

  readFile(R,G,B);


  for (i=0;i<ROWS;i++) {
    printf("x[%d]=",i);
    for (j=0; j< COLS; j++){
      printf("%f ",R[i][j]); //cols
    }
    printf("\n");
  }


  for (i=0;i<ROWS;i++) {
    printf("x[%d]=",i);
    for (j=0; j< COLS; j++){
      printf("%f ",x[i][j]); //cols
    }
    printf("\n");
  }



  printf("Original signal:\n");
  //for (i=0;i<32;i++) printf("x[%d]=%f\n",i,x[0][i]); //rows
  for (i=0;i<ROWS;i++) {
    printf("x[%d]=",i);
    for (j=0; j< COLS; j++){
      printf("%f ",x[i][j]); //cols
    }
    printf("\n");
  }




  //ROUND coefficients
  for (i=0;i<ROWS;i++) {
    for (j=0; j< COLS; j++){
      x[i][j] = round(x[i][j]); //cols
    }
  }




  printf("Wavelets coefficients:\n");
  for (i=0;i<ROWS;i++) {
    printf("x[%d]=",i);
    for (j=0; j< COLS; j++){
      printf("%f ",x[i][j]); //cols
      }
      printf("\n");
    }
    printf("\n");





    printf("Reconstructed signal:\n");
    for (i=0;i<ROWS;i++) {
      printf("x[%d]=",i);
      for (j=0; j< COLS; j++){
        printf("%f ",round(x[i][j])); //cols
      }
      printf("\n");
    }


}