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


void decompress(int* compressed){//int* data){

  int* decode;
  decode = huffmanDecode(compressed);//data);
  // run Length Decoding

  int* runLengthDecode = decode(decode);
  int* diffDecode = diffDecode(runLengthDecode);

  

  dequantizer(R,ROWS/2,COLS/2,ROWS/2,COLS/2,4);
  dequantizer(R,0,COLS/2,ROWS/2,COLS/2,2);
  dequantizer(R,ROWS/2,0,ROWS/2,COLS/2,2);

  for(i = 0; i <COLS/2; i++){
    partialcoliwt(R,i+COLS/2, ROWS/2, ROWS/2);
  }
  for(i = 0; i<ROWS/2; i++){
    partialrowiwt(R,i+ROWS/2, COLS/2, COLS/2);
  }
  for(i = 0; i <COLS; i++){
    coliwt(R,i);
  }
  for(i = 0; i <ROWS; i++){
    rowiwt(R,i);
  }



int main(){

  decompress();

  return 0;
}
