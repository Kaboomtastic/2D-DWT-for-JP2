#include "constants.h"
#include "dwt.c"
#include "huffmanEncoding.c"
#include "runLengthEncoding.c"
#include <math.h>
#include "differenceEncoder.c"

float R[ROWS][COLS],G[ROWS][COLS],B[ROWS][COLS];
float Y[ROWS][COLS],U[ROWS][COLS],V[ROWS][COLS];
float subU[ROWS/2][COLS/2],subV[ROWS/2][COLS/2];
int QuantizedTemp [ROWS][COLS];
int subQuantizedTemp [ROWS/2][COLS/2];
int divisorArray[ROWS][COLS];
int subDivisorARRAY[ROWS/2][COLS/2];

void quantize(){

  quantizer(Y,ROWS/2,COLS/2,ROWS/2,COLS/2,10); //lower Right quadrant
  quantizer(Y,ROWS/2,0,ROWS/2,COLS/2,6); //lower left quadrant
  quantizer(Y,0,COLS/2,ROWS/2,COLS/2,6); //upper right quadrant
  quantizer(Y,0,0,ROWS/2,COLS/2,2);


  subquantizer(subU,ROWS/4,COLS/4,ROWS/4,COLS/4,8);
  subquantizer(subU,0,COLS/4,ROWS/4,COLS/4,4);
  subquantizer(subU,ROWS/4,0,ROWS/4,COLS/4,4);
  subquantizer(subU,0,0,ROWS/4,COLS/4,2);

  subquantizer(subV,ROWS/4,COLS/4,ROWS/4,COLS/4,8);
  subquantizer(subV,0,COLS/4,ROWS/4,COLS/4,4);
  subquantizer(subV,ROWS/4,0,ROWS/4,COLS/4,4);
  subquantizer(subV,0,0,ROWS/4,COLS/4,2);


}

void dwtize(){
  int i,j;
  for(i = 0; i <ROWS; i++){
    rowdwt(Y,i);
  }
  for(i = 0; i <COLS; i++){
    coldwt(Y,i);
  }

  for(i = 0; i <ROWS/2; i++){
    subPartialrowdwt(subU,i,0,COLS/2);
    subPartialrowdwt(subV,i,0,COLS/2);
  }
  for(i = 0; i <COLS/2; i++){
    subPartialcoldwt(subU,i,0,ROWS/2);
    subPartialcoldwt(subV,i,0,ROWS/2);
  }


}


int* compress(){
  int i,j;

  colorSpaceChange(R,G,B,Y,U,V);

  chromaSubSampler(U,V,subU,subV);

  dwtize();

  printf("DWTized:\n");

  for (i=0;i<ROWS;i++) {
    printf("Y[%d]=",i);
    for (j=0; j< COLS; j++){
      printf("%f ",Y[i][j]); //cols
    }
    printf("\n");
  }

  printf("\n");
  for (i=0;i<ROWS/2;i++) {
    printf("U[%d]=",i);
    for (j=0; j< COLS/2; j++){
      printf("%f ",subU[i][j]); //cols
    }
    printf("\n");
  }

  printf("\n");
  for (i=0;i<ROWS/2;i++) {
    printf("V[%d]=",i);
    for (j=0; j< COLS/2; j++){
      printf("%f ",subV[i][j]); //cols
    }
    printf("\n");
  }

  quantize();



  for (i=0;i<ROWS;i++) {
    for (j=0; j< COLS; j++){
      QuantizedTemp[i][j] = (int) round(Y[i][j]); //cols
    }
  }


  printf("Quanitized Coefficients:\n");
  for (i=0;i<ROWS;i++) {
    printf("Y[%d]=",i);
    for (j=0; j< COLS; j++){
      printf("%d ",QuantizedTemp[i][j]); //cols
    }
    printf("\n");
  }

  int* diff = diffEncode(QuantizedTemp, ROWS);
    // run Length Encoding
  int* runLength = encode(ROWS, COLS, diff);

  //int length = sizeof(runLength)/sizeof(runLength[0]);
  int len = runLength[0];
  for(int i = 1; i <= len; i++){
    printf("%d ",runLength[i] );
    //if(i%ROWS == 0) printf("\n");
  }printf("\n" );
  //printf("%d\n",len);
  //huffman Encoding

  int8_t *huffman = huffEncodingNEW(runLength);
  len = (huffman[0] << 8) + ((u_int8_t)huffman[1]);


  return huffman;
}





int main(){
  int i,j;



  readFile(R,G,B);



  compress();



    return 0;
}
