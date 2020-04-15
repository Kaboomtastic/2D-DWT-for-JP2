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

  /*printf("DWTized:\n");

  for (i=0;i<ROWS;i++) {
    printf("Y[%d]=",i);
    for (j=0; j< COLS; j++){
      printf("%f ",Y[i][j]); //cols
    }
    printf("\n");
  }*/


  quantize();



  for (i=0;i<ROWS;i++) {
    for (j=0; j< COLS; j++){
      QuantizedTemp[i][j] = (int) round(Y[i][j]); //cols
    }
  }



  int* diff = diffEncode(QuantizedTemp, ROWS);

  int* runLength = encode(ROWS, COLS, diff);

  int len = runLength[0];

  int8_t *huffman = huffEncodingNEW(runLength);
  len = (huffman[0] << 8) + ((u_int8_t)huffman[1]);


  return huffman;
}





void decompress(u_int8_t* compressed){//int* data){


  int8_t* huffdecoded = huffmanDecode(compressed);
  int8_t* deRunLengthed = decode(huffdecoded);
  int8_t* diffDecoded = diffDecode(deRunLengthed, ROWS);


  for(int i = 0; i < ROWS; i++){
    for(int j = 0; j < COLS; j++){
      R[i][j] = (float) diffDecoded[i*ROWS+j];
    }
  }


  dequantizer(R,ROWS/2,COLS/2,ROWS/2,COLS/2,10); //lower Right quadrant
  dequantizer(R,ROWS/2,0,ROWS/2,COLS/2,6); //lower left quadrant
  dequantizer(R,0,COLS/2,ROWS/2,COLS/2,6); //upper right quadrant
  dequantizer(R,0,0,ROWS/2,COLS/2,2);

  int i;
  /*for(i = 0; i <COLS/2; i++){
    partialcoliwt(R,i+COLS/2, ROWS/2, ROWS/2);
  }
  for(i = 0; i<ROWS/2; i++){
    partialrowiwt(R,i+ROWS/2, COLS/2, COLS/2);
  }*/
  for(i = 0; i <COLS; i++){
    coliwt(R,i);
  }
  for(i = 0; i <ROWS; i++){
    rowiwt(R,i);
  }

  for(int i = 0; i < ROWS; i++){
    printf("R[%d]: ",i);
    for(int j = 0; j < COLS; j++){
      printf("%f ",R[i][j]);
    }
    printf("\n");
  }

}



int main(){
  readFile(R,G,B);
  for(int i = 0; i < ROWS; i++){
    printf("R[%d]: ",i);
    for(int j = 0; j < COLS; j++){
      printf("%f ",R[i][j]);
    }
    printf("\n");
  }
  int* compressed = compress();
  decompress(compressed);

  return 0;
}
