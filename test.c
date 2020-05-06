#include "constants.h"
#include "dwt.c"
#include "huffmanEncoding.c"
#include "runLengthEncoding.c"
#include <math.h>
#include "differenceEncoder.c"

float R[ROWS][COLS],G[ROWS][COLS],B[ROWS][COLS];
float Y[ROWS][COLS],U[ROWS][COLS],V[ROWS][COLS];
float subU[ROWS/2][COLS/2],subV[ROWS/2][COLS/2];
int QuantizedTempY [ROWS][COLS];
int subQuantizedTempU [ROWS/2][COLS/2];
int subQuantizedTempV [ROWS/2][COLS/2];

int divisorArray[ROWS][COLS];
int subDivisorARRAY[ROWS/2][COLS/2];



void quantize(){


  quantizer(Y,ROWS/2,COLS/2,ROWS/2,COLS/2,8); //lower Right quadrant
  quantizer(Y,ROWS/2,0,ROWS/2,COLS/2,4); //lower left quadrant
  quantizer(Y,0,COLS/2,ROWS/2,COLS/2,4); //upper right quadrant
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

void dequantize(){

  dequantizer(Y,ROWS/2,COLS/2,ROWS/2,COLS/2,8); //lower Right quadrant
  dequantizer(Y,ROWS/2,0,ROWS/2,COLS/2,4); //lower left quadrant
  dequantizer(Y,0,COLS/2,ROWS/2,COLS/2,4); //upper right quadrant
  dequantizer(Y,0,0,ROWS/2,COLS/2,2);

  subdequantizer(subU,ROWS/4,COLS/4,ROWS/4,COLS/4,8);
  subdequantizer(subU,0,COLS/4,ROWS/4,COLS/4,4);
  subdequantizer(subU,ROWS/4,0,ROWS/4,COLS/4,4);
  subdequantizer(subU,0,0,ROWS/4,COLS/4,2);

  subdequantizer(subV,ROWS/4,COLS/4,ROWS/4,COLS/4,8);
  subdequantizer(subV,0,COLS/4,ROWS/4,COLS/4,4);
  subdequantizer(subV,ROWS/4,0,ROWS/4,COLS/4,4);
  subdequantizer(subV,0,0,ROWS/4,COLS/4,2);

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
    subrowdwt(subU,i);
    subrowdwt(subV,i);
  }
  for(i = 0; i <COLS/2; i++){
    subcoldwt(subU,i);
    subcoldwt(subV,i);
  }

}

void iwtize(){
  int i,j;
  for(i = 0; i <ROWS; i++){
    rowiwt(Y,i);
  }
  for(i = 0; i <COLS; i++){
    coliwt(Y,i);
  }
  for(i = 0; i <ROWS/2; i++){
    subrowiwt(subU,i);
    subrowiwt(subV,i);
  }
  for(i = 0; i <COLS/2; i++){
    subcoliwt(subU,i);
    subcoliwt(subV,i);
  }
}


void compress(int8_t** compressedY, int8_t** compressedU, int8_t** compressedV){
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
      QuantizedTempY[i][j] = (int) round(Y[i][j]); //cols
    }
  }
  for (i=0;i<ROWS/2;i++) {
    for (j=0; j< COLS/2; j++){
      subQuantizedTempU [i][j] = round(subU[i][j]);
      subQuantizedTempV [i][j] = round(subV[i][j]);
    }
  }

  printf("\n");
  for(int i = 0; i < ROWS/2; i++){
    printf("R[%d]: ",i);
    for(int j = 0; j < COLS/2; j++){
      printf("%d ",subQuantizedTempU[i][j]);
    }
    printf("\n");
  }
  printf("\n");

  printf("\n");
  for(int i = 0; i < ROWS/2; i++){
    printf("R[%d]: ",i);
    for(int j = 0; j < COLS/2; j++){
      printf("%d ",subQuantizedTempV[i][j]);
    }
    printf("\n");
  }
  printf("\n");

  int* diffY = diffEncode(QuantizedTempY, ROWS);
  int* runLengthY = encode(ROWS, COLS, diffY);
  int lenY = runLengthY[0];
  int8_t *huffmanY = huffEncodingNEW(runLengthY);
  lenY = (huffmanY[0] << 8) + ((u_int8_t)huffmanY[1]);

  int* diffU = diffEncode(subQuantizedTempU, ROWS/2);
  int* runLengthU = encode(ROWS/2, COLS/2, diffU);
  int lenU = runLengthU[0];
  int8_t *huffmanU = huffEncodingNEW(runLengthU);
  lenU = (huffmanU[0] << 8) + ((u_int8_t)huffmanU[1]);

  printf("\n");
  for(int i = 0; i < ROWS/2; i++){
    printf("R[%d]: ",i);
    for(int j = 0; j < COLS/2; j++){
      printf("%d ",diffU[i*ROWS/2+j]);
    }
    printf("\n");
  }
  printf("\n");

  printf("\n");
  for(int i = 0; i < 100; i++){

    printf("%d ",runLengthU[i]);

  }
  printf("\n");

  int* diffV = diffEncode(subQuantizedTempV, ROWS/2);
  int* runLengthV = encode(ROWS/2, COLS/2, diffV);
  int lenV = runLengthV[0];
  int8_t *huffmanV = huffEncodingNEW(runLengthV);
  lenV = (huffmanV[0] << 8) + ((u_int8_t)huffmanV[1]);

  printf("\n");
  for(int i = 0; i < 100; i++){

    printf("%d ",runLengthV[i]);

  }
  printf("\n");

  *compressedY = huffmanY;
  *compressedU = huffmanU;
  *compressedV = huffmanV;


  printf("\n%d\n", lenY+lenU+lenV);

}





void decompress(int8_t* compressedY, int8_t* compressedU, int8_t* compressedV){//int* data){


  int8_t* huffdecodedY = huffmanDecode(compressedY);
  int8_t* deRunLengthedY = decode(huffdecodedY,ROWS);
  int* diffDecodedY = diffDecode(deRunLengthedY, ROWS);



  int8_t* huffdecodedU = huffmanDecode(compressedU);
  int8_t* deRunLengthedU = decode(huffdecodedU, ROWS/2);
  int* diffDecodedU = diffDecode(deRunLengthedU, ROWS/2);
  printf("\n");
  for(int i = 0; i < 80; i++){
    printf("%d ",compressedU[i]);
  }

  printf("\n");
  for(int j = 0; j < 78; j++){
    printf("%d ",huffdecodedU[j]);
  }

  printf("\n");
  for(int i = 0; i < ROWS/2; i++){
    printf("R[%d]: ",i);
    for(int j = 0; j < COLS/2; j++){
      printf("%d ",deRunLengthedU[i*ROWS/2+j]);
    }
    printf("\n");
  }

  printf("\n");
  printf("\n");
  for(int i = 0; i < ROWS/2; i++){
    printf("R[%d]: ",i);
    for(int j = 0; j < COLS/2; j++){
      printf("%d ",diffDecodedU[i*ROWS/2+j]);
    }
    printf("\n");
  }
  printf("\n");

  int8_t* huffdecodedV = huffmanDecode(compressedV);
  int8_t* deRunLengthedV = decode(huffdecodedV,ROWS/2);
  int* diffDecodedV = diffDecode(deRunLengthedV, ROWS/2);

  printf("\n");
  for(int i = 0; i < ROWS/8; i++){
    printf("R[%d]: ",i);
    for(int j = 0; j < COLS/2; j++){
      printf("%d ",huffdecodedV[i*ROWS/2+j]);
    }
    printf("\n");
  }

  printf("\n");
  for(int i = 0; i < ROWS/2; i++){
    printf("R[%d]: ",i);
    for(int j = 0; j < COLS/2; j++){
      printf("%d ",deRunLengthedV[i*ROWS/2+j]);
    }
    printf("\n");
  }
  printf("\n");

  printf("\n");
  for(int i = 0; i < ROWS/2; i++){
    printf("R[%d]: ",i);
    for(int j = 0; j < COLS/2; j++){
      printf("%d ",diffDecodedV[i*ROWS/2+j]);
    }
    printf("\n");
  }
  printf("\n");


  for(int i = 0; i < ROWS; i++){
    for(int j = 0; j < COLS; j++){
      Y[i][j] = (float) diffDecodedY[i*ROWS+j];
    }
  }

  for(int i = 0; i < ROWS/2; i++){
    for(int j = 0; j < COLS/2; j++){
      subU[i][j] = (float) diffDecodedU[i*ROWS/2+j];
      subV[i][j] = (float) diffDecodedV[i*ROWS/2+j];
    }
  }



  dequantize();

  iwtize();

  chromaUpSampler(U,V,subU,subV);


  colorSpaceReverse(Y,U,V,R,G,B);



  for(int i = 0; i < ROWS; i++){
    printf("R[%d]: ",i);
    for(int j = 0; j < COLS; j++){
      printf("%f ",R[i][j]);
    }
    printf("\n");
  }

}



int main(){
  int8_t* ycompressed;
  int8_t* ucompressed;
  int8_t* vcompressed;

  readFile(R,G,B);
  for(int i = 0; i < ROWS; i++){
    printf("R[%d]: ",i);
    for(int j = 0; j < COLS; j++){
      printf("%f ",R[i][j]);
    }
    printf("\n");
  }
  compress(&ycompressed,&ucompressed,&vcompressed);
  decompress(ycompressed,ucompressed,vcompressed);

  return 0;
}
