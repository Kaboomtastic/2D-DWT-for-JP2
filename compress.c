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

  /*quantizer(Y,ROWS/8,COLS/8,ROWS/8,COLS/8,3);
  quantizer(Y,ROWS/8,COLS/8,ROWS/8,COLS/8,3);
  quantizer(Y,0,COLS/8,ROWS/8,COLS/8,2);
  quantizer(Y,ROWS/8,0,ROWS/8,COLS/8,2); //top left corners
  quantizer(Y,0,0,ROWS/8,COLS/8,1); //top left */


  subquantizer(subU,ROWS/4,COLS/4,ROWS/4,COLS/4,8);
  subquantizer(subU,0,COLS/4,ROWS/4,COLS/4,4);
  subquantizer(subU,ROWS/4,0,ROWS/4,COLS/4,4);
  subquantizer(subU,0,0,ROWS/4,COLS/4,2);

  subquantizer(subV,ROWS/4,COLS/4,ROWS/4,COLS/4,8);
  subquantizer(subV,0,COLS/4,ROWS/4,COLS/4,4);
  subquantizer(subV,ROWS/4,0,ROWS/4,COLS/4,4);
  subquantizer(subV,0,0,ROWS/4,COLS/4,2);


}
/*
int i,j,k,l;
for(k = 0; k < 64; k*=2){
  for(l = 0; l < 64; l*=2){
    for(i = 0; i < ROWS; i++){
      for(j = 0; j < COLS; j++){
        if(i >= k && j >= l){
          divisorArray[i][j] += 1;
        }
      }
      if(l == 0) l =1;
    }
    if(k == 0) k =1;
  }

}
printf("\n");
for(i = 0; i < ROWS; i++){
  for(j = 0; j < COLS; j++){
    printf("%d ", divisorArray[i][j]);
  }
  printf("\n");
}
quantizer(Y,ROWS/2,COLS/2,ROWS/2,COLS/2,10); //lower Right
quantizer(Y,ROWS/8,COLS/8,ROWS/8,COLS/8,3);
quantizer(Y,ROWS/8,COLS/8,ROWS/8,COLS/8,3);
quantizer(Y,0,COLS/8,ROWS/8,COLS/8,2);
quantizer(Y,ROWS/8,0,ROWS/8,COLS/8,2); //top left corners
quantizer(Y,0,0,ROWS/8,COLS/8,1); //top left
subquantizer(subU,ROWS/4,COLS/4,ROWS/4,COLS/4,8);
subquantizer(subU,0,COLS/4,ROWS/4,COLS/4,4);
subquantizer(subU,ROWS/4,0,ROWS/4,COLS/4,4);
subquantizer(subU,0,0,ROWS/4,COLS/4,2);

subquantizer(subV,ROWS/4,COLS/4,ROWS/4,COLS/4,8);
subquantizer(subV,0,COLS/4,ROWS/4,COLS/4,4);
subquantizer(subV,ROWS/4,0,ROWS/4,COLS/4,4);
subquantizer(subV,0,0,ROWS/4,COLS/4,2);
*/
void dwtize(){
  int i,j;
  for(i = 0; i <ROWS; i++){
    rowdwt(Y,i);
  }
  for(i = 0; i <COLS; i++){
    coldwt(Y,i);
  }
  /*
  for(i = 0; i<ROWS/2; i++){
    partialrowdwt(Y,i, 0, COLS/2);
  }
  for(i = 0; i <COLS/2; i++){
    partialcoldwt(Y,i, 0, ROWS/2);
  }

  for(i = 0; i<ROWS/2; i++){
    partialrowdwt(Y,i, COLS/2, COLS/2);
  }
  for(i = 0; i <COLS/2; i++){
    partialcoldwt(Y,i+COLS/2, 0, ROWS/2);
  }

  for(i = 0; i<ROWS/2; i++){
    partialrowdwt(Y,i+ROWS/2, 0, COLS/2);
  }
  for(i = 0; i <COLS/2; i++){
    partialcoldwt(Y,0, ROWS/2, ROWS/2);
  }
  */





  for(i = 0; i <ROWS/2; i++){
    subPartialrowdwt(subU,i,0,COLS/2);
    subPartialrowdwt(subV,i,0,COLS/2);
  }
  for(i = 0; i <COLS/2; i++){
    subPartialcoldwt(subU,i,0,ROWS/2);
    subPartialcoldwt(subV,i,0,ROWS/2);
  }
  /*
  for(i = 0; i<ROWS/4; i++){
    subPartialrowdwt(subU,i+ROWS/4, COLS/4, COLS/4);
    subPartialrowdwt(subV,i+ROWS/4, COLS/4, COLS/4);
  }
  for(i = 0; i <COLS/4; i++){
    subPartialcoldwt(subU,i+COLS/4, ROWS/4, ROWS/4);
    subPartialcoldwt(subV,i+COLS/4, ROWS/4, ROWS/4);
  }
  */


}


int* compress(){
  int i,j;

  colorSpaceChange(R,G,B,Y,U,V);
  /*
  for (i=0;i<ROWS;i++) {
    printf("Y[%d]=",i);
    for (j=0; j< COLS; j++){
      printf("%f ",Y[i][j]); //cols
    }
    printf("\n");
  }
  printf("\n");
  for (i=0;i<ROWS;i++) {
    printf("U[%d]=",i);
    for (j=0; j< COLS; j++){
      printf("%f ",U[i][j]); //cols
    }
    printf("\n");
  }
  printf("\n");
  for (i=0;i<ROWS;i++) {
    printf("V[%d]=",i);
    for (j=0; j< COLS; j++){
      printf("%f ",V[i][j]); //cols
    }
    printf("\n");
  }
  printf("\n");
  */

  chromaSubSampler(U,V,subU,subV);
  /*
  printf("subSampled:\n");
  for (i=0;i<ROWS/2;i++) {
    printf("U[%d]=",i);
    for (j=0; j< COLS/2; j++){
      printf("%f ",subU[i][j]); //cols
    }
    printf("\n");
  }

  printf("subSampled:\n");
  for (i=0;i<ROWS/2;i++) {
    printf("V[%d]=",i);
    for (j=0; j< COLS/2; j++){
      printf("%f ",subV[i][j]); //cols
    }
    printf("\n");
  }
  */
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

  for(int i = 0; i<len; i++){
    if(i%50 == 0) printf("\n");
    printf("%i ", huffman[i]);
  }
  printf("\n" );

/////////////////////////////////////////////
  /*
  for (i=0;i<ROWS/2;i++) {
    for (j=0; j< COLS/2; j++){
      subQuantizedTemp[i][j] = (int) round(subU[i][j]); //cols
    }
  }

  for (i=0;i<ROWS/2;i++) {
    printf("U[%d]=",i);
    for (j=0; j< COLS/2; j++){
      printf("%d ",subQuantizedTemp[i][j]); //cols
    }
    printf("\n");
  }


  runLength = encode(ROWS/2, COLS/2, subQuantizedTemp);
  len = runLength[0];
  for(int i = 1; i <= len; i++){
    printf("%d\t",runLength[i] );
  }printf("\n" );

  huffEncodingNEW(runLength);

//////////////////////
  for (i=0;i<ROWS/2;i++) {
    for (j=0; j< COLS/2; j++){
      subQuantizedTemp[i][j] = (int) round(subV[i][j]); //cols
    }
  }

  for (i=0;i<ROWS/2;i++) {
    printf("V[%d]=",i);
    for (j=0; j< COLS/2; j++){
      printf("%d ",subQuantizedTemp[i][j]); //cols
    }
    printf("\n");
  }

  runLength = encode(ROWS/2, COLS/2, subQuantizedTemp);
  len = runLength[0];
  for(int i = 1; i <= len; i++){
    printf("%d\t",runLength[i] );
  }printf("\n" );
  huffEncodingNEW(runLength);
  */
  int8_t* huffdecoded = huffmanDecode(huffman);
  int8_t* deRunLengthed = decode(huffdecoded);
  int8_t* diffDecoded = diffDecode(deRunLengthed, ROWS);

  printf("\n");
  printf("\n");
  printf("\n");
  for(int i = 0; i < ROWS*COLS; i++){
    if(i % 64 == 0){
      printf("\n");
      printf("y %d\t",i/64);
    }
    printf("%d ", diffDecoded[i]);
  }

  return huffman;
}





int main(){
  int i,j;

  //readFile(R,G,B);
  /*
  for (i=0;i<ROWS;i++){
    for (j=0; j< COLS; j++){
      R[i][j]=round(5+i+0.4*i*i-0.02*i*i*j);
      R[i][j]=(int)R[i][j]%255;
      G[i][j]=(int)R[i][j]%255;
      B[i][j]=(int)R[i][j]%255;

    }
  }*/

  readFile(R,G,B);

  /*
  printf("Original signal:\n");
  //for (i=0;i<32;i++) printf("x[%d]=%f\n",i,x[0][i]); //rows
  for (i=0;i<ROWS;i++) {
    printf("R[%d]=",i);
    for (j=0; j< COLS; j++){
      printf("%f ",R[i][j]); //cols
    }
    printf("\n");
  }
  printf("\n");
  for (i=0;i<ROWS;i++) {
    printf("G[%d]=",i);
    for (j=0; j< COLS; j++){
      printf("%f ",G[i][j]); //cols
    }
    printf("\n");
  }
  printf("\n");
  for (i=0;i<ROWS;i++) {
    printf("B[%d]=",i);
    for (j=0; j< COLS; j++){
      printf("%f ",B[i][j]); //cols
    }
    printf("\n");
  }
  printf("\n");
  */


  compress();



    return 0;
}
