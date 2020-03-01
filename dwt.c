/**
 *  dwt97.c - Fast discrete biorthogonal CDF 9/7 wavelet forward and inverse transform (lifting implementation)
 *
 *  This code is provided "as is" and is given for educational purposes.
 *  2006 - Gregoire Pau - gregoire.pau@ebi.ac.uk
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "constants.h"
//double *tempbank=0;


float tempbank[ROWS];

void readFile(float R[ROWS][COLS],float G[ROWS][COLS],float B[ROWS][COLS]){
  FILE * fp;
  fp = fopen("testImage.rgb","r");

  for(int i = 0; i <833; i++){
    for(int j = 0; j<833; j++){
      uint8_t tempR,tempG,tempB;
      fread(&tempR, 1, 1, fp);
      fread(&tempG, (size_t)1, (size_t) 1, fp);
      fread(&tempB, (size_t)1, (size_t) 1, fp);
      if(i<ROWS && j<COLS){
        R[i][j] = tempR;
        G[i][j] = tempG;
        B[i][j] = tempB;
      }
    }
  }
  fclose(fp);
}



void colorSpaceChange(float R[ROWS][COLS],float G[ROWS][COLS],float B[ROWS][COLS],float Y[ROWS][COLS],float U[ROWS][COLS],float V[ROWS][COLS]){
  for(int i = 0; i < ROWS; i++){
    for(int j = 0; j < COLS; j++){
      Y[i][j] = 0.299* R[i][j] + 0.587*G[i][j] + 0.114*B[i][j];
      U[i][j] = 0.492* (B[i][j]-Y[i][j]);
      V[i][j] = 0.877* (R[i]-Y[j]);
    }
  }
}

// 4:2:2 sampling
void chromaSubSampler(float U[ROWS][COLS],float V[ROWS][COLS],float subU[ROWS][COLS],float subV[ROWS][COLS]){
  for(int i=0; i<ROWS; i+=2){
    for(int j=0; j<COLS; j+=2){
      subU[i/2][j/2] = U[i][j];
      subV[i/2][j/2] = V[i][j];
    }
  }
}


void quantizer(float x[ROWS][COLS],int startRow,int startCol, int numRows, int numCols, int q){
    int i,j;
    for(i = 0; i < numRows; i++){
      for(j=0; j < numCols; j++){
        x[i+startRow][j+startCol] = x[i+startRow][j+startCol]/q;
      }
    }
}

void dequantizer(float x[ROWS][COLS],int startRow,int startCol, int numRows, int numCols, int q){
    int i,j;
    for(i = 0; i < numRows; i++){
      for(j=0; j < numCols; j++){
        x[i+startRow][j+startCol] = x[i+startRow][j+startCol]*q;
      }
    }
}


void chromaUpSampler(float U[ROWS][COLS],float V[ROWS][COLS],float subU[ROWS][COLS],float subV[ROWS][COLS]){
  for(int i=0; i<ROWS; i+=2){
    for(int j=0; j<COLS; j+=2){
      U[i][j] = subU[i][j];
      U[i+1][j] = subU[i][j];
      U[i][j+1] = subU[i][j];
      U[i+1][j+1] = subU[i][j];

      V[i][j] = subV[i][j];
      V[i+1][j] = subV[i][j];
      V[i][j+1] = subV[i][j];
      V[i+1][j+1] = subV[i][j];

    }
  }
}

void coldwt(float x[ROWS][COLS],int n){ //n is the current col number
  float a;
  int i;

  // Predict 1tempbank
  a=-1.586134342;
  for (i=1;i<ROWS-2;i+=2) {
    x[i][n]+=a*(x[i-1][n]+x[i+1][n]);
  }
  x[ROWS-1][n]+=2*a*x[ROWS-2][n];

  // Update 1
  a=-0.05298011854;
  for (i=2;i<ROWS;i+=2) {
    x[i][n]+=a*(x[i-1][n]+x[i+1][n]);
  }
  x[0][n]+=2*a*x[1][n];

  // Predict 2
  a=0.8829110762;
  for (i=1;i<ROWS-2;i+=2) {
    x[i][n]+=a*(x[i-1][n]+x[i+1][n]);
  }
  x[ROWS-1][n]+=2*a*x[ROWS-2][n];

  // Update 2
  a=0.4435068522;
  for (i=2;i<ROWS;i+=2) {
    x[i][n]+=a*(x[i-1][n]+x[i+1][n]);
  }
  x[0][n]+=2*a*x[1][n];

  // Scale
  a=1/1.149604398;
  for (i=0;i<ROWS;i++) {
    if (i%2) x[i][n]*=a;
    else x[i][n]/=a;
  }

  // Pack
  //if (tempbank==0) tempbank=(double *)malloc(n*sizeof(double));
  for (i=0;i<ROWS;i++) {
    if (i%2==0) tempbank[i/2]=x[i][n];
    else tempbank[ROWS/2+i/2]=x[i][n];
  }
  for (i=0;i<ROWS;i++) x[i][n]=tempbank[i];
}



void partialcoldwt(float x[ROWS][COLS],int n,int startRow, int numRows){ //n is the current col number
  float a;
  int i;

  // Predict 1tempbank
  a=-1.586134342;
  for (i=1;i<numRows-2;i+=2) {
    x[i+startRow][n]+=a*(x[i+startRow-1][n]+x[i+startRow+1][n]);
  }
  x[startRow + numRows-1][n]+=2*a*x[startRow+numRows-2][n];

  // Update 1
  a=-0.05298011854;
  for (i=2;i<numRows;i+=2) {
    x[i+startRow][n]+=a*(x[i+startRow-1][n]+x[i+startRow+1][n]);
  }
  x[0+startRow][n]+=2*a*x[1+startRow][n];

  // Predict 2
  a=0.8829110762;
  for (i=1;i<numRows-2;i+=2) {
    x[i+startRow][n]+=a*(x[i+startRow-1][n]+x[i+startRow+1][n]);
  }
  x[startRow+numRows-1][n]+=2*a*x[startRow+numRows-2][n];

  // Update 2
  a=0.4435068522;
  for (i=2;i<numRows;i+=2) {
    x[i+startRow][n]+=a*(x[i+startRow-1][n]+x[i+startRow+1][n]);
  }
  x[0+startRow][n]+=2*a*x[1+startRow][n];

  // Scale
  a=1/1.149604398;
  for (i=0;i<numRows;i++) {
    if (i%2) x[i+startRow][n]*=a;
    else x[i+startRow][n]/=a;
  }

  // Pack
  //if (tempbank==0) tempbank=(double *)malloc(n*sizeof(double));
  for (i=0;i<numRows;i++) {
    if (i%2==0) tempbank[i/2]=x[i+startRow][n];
    else tempbank[numRows/2+i/2]=x[i+startRow][n];
  }
  for (i=0;i<numRows;i++) x[i+startRow][n]=tempbank[i];
}





/**
 *  fwt97 - Forward biorthogonal 9/7 wavelet transform (lifting implementation)
 *
 *  x is an input signal, which will be replaced by its output transform.
 *  n is the length of the signal, and must be a power of 2.
 *
 *  The first half part of the output signal contains the approximation coefficients.
 *  The second half part contains the detail coefficients (aka. the wavelets coefficients).
 *
 *  See also iwt97.
 */
void rowdwt(float x[ROWS][COLS],int n) { //n is the current row number
  float a;
  int i;

  // Predict 1
  a=-1.586134342;
  for (i=1;i<COLS-2;i+=2) {
    x[n][i]+=a*(x[n][i-1]+x[n][i+1]);
  }
  x[n][COLS-1]+=2*a*x[n][COLS-2];

  // Update 1
  a=-0.05298011854;
  for (i=2;i<COLS;i+=2) {
    x[n][i]+=a*(x[n][i-1]+x[n][i+1]);
  }
  x[n][0]+=2*a*x[n][1];

  // Predict 2
  a=0.8829110762;
  for (i=1;i<COLS-2;i+=2) {
    x[n][i]+=a*(x[n][i-1]+x[n][i+1]);
  }
  x[n][COLS-1]+=2*a*x[n][COLS-2];

  // Update 2
  a=0.4435068522;
  for (i=2;i<COLS;i+=2) {
    x[n][i]+=a*(x[n][i-1]+x[n][i+1]);
  }
  x[n][0]+=2*a*x[n][1];

  // Scale
  a=1/1.149604398;
  for (i=0;i<COLS;i++) {
    if (i%2) x[n][i]*=a;
    else x[n][i]/=a;
  }

  // Pack
  //if (tempbank==0) tempbank=(double *)malloc(n*sizeof(double));
  for (i=0;i<COLS;i++) {
    if (i%2==0) tempbank[i/2]=x[n][i];
    else tempbank[COLS/2+i/2]=x[n][i];
  }
  for (i=0;i<COLS;i++) x[n][i]=tempbank[i];
}



void partialrowdwt(float x[ROWS][COLS],int n,int startCol, int numCols){
  float a;
  int i;

  // Predict 1
  a=-1.586134342;
  for (i=1+startCol;i<numCols-2;i+=2) {
    x[n][i+startCol]+=a*(x[n][i+startCol-1]+x[n][i+startCol+1]);
  }
  x[n][startCol+numCols-1]+=2*a*x[n][startCol+numCols-2];

  // Update 1
  a=-0.05298011854;
  for (i=2;i<numCols;i+=2) {
    x[n][i+startCol]+=a*(x[n][i+startCol-1]+x[n][i+startCol+1]);
  }
  x[n][0+startCol]+=2*a*x[n][1+startCol];

  // Predict 2
  a=0.8829110762;
  for (i=1;i<numCols-2;i+=2) {
    x[n][i+startCol]+=a*(x[n][i+startCol-1]+x[n][i+startCol+1]);
  }
  x[n][startCol+numCols-1]+=2*a*x[n][startCol+numCols-2];

  // Update 2
  a=0.4435068522;
  for (i=2;i<numCols;i+=2) {
    x[n][i+startCol]+=a*(x[n][i+startCol-1]+x[n][i+startCol+1]);
  }
  x[n][0+startCol]+=2*a*x[n][1+startCol];

  // Scale
  a=1/1.149604398;
  for (i=0;i<numCols;i++) {
    if (i%2) x[n][i+startCol]*=a;
    else x[n][i+startCol]/=a;
  }

  // Pack
  //if (tempbank==0) tempbank=(double *)malloc(n*sizeof(double));
  for (i=0;i<numCols;i++) {
    if (i%2==0) tempbank[i/2]=x[n][i+startCol];
    else tempbank[numCols/2+i/2]=x[n][i+startCol];
  }
  for (i=0;i<numCols;i++) x[n][i+startCol]=tempbank[i];
}


/**
 *  iwt97 - Inverse biorthogonal 9/7 wavelet transform
 *
 *  This is the inverse of fwt97 so that iwt97(fwt97(x,n),n)=x for every signal x of length n.
 *
 *  See also fwt97.
 */
void rowiwt(float x[ROWS][COLS],int n) { //n is the current row
  float a;
  int i;

  // Unpack
  //if (tempbank==0) tempbank=(double *)malloc(n*sizeof(double));
  for (i=0;i<COLS/2;i++) {
    tempbank[i*2]=x[n][i];
    tempbank[i*2+1]=x[n][i+COLS/2];
  }
  for (i=0;i<COLS;i++) x[n][i]=tempbank[i];

  // Undo scale
  a=1.149604398;
  for (i=0;i<COLS;i++) {
    if (i%2) x[n][i]*=a;
    else x[n][i]/=a;
  }

  // Undo update 2
  a=-0.4435068522;
  for (i=2;i<COLS;i+=2) {
    x[n][i]+=a*(x[n][i-1]+x[n][i+1]);
  }
  x[n][0]+=2*a*x[n][1];

  // Undo predict 2
  a=-0.8829110762;
  for (i=1;i<COLS-2;i+=2) {
    x[n][i]+=a*(x[n][i-1]+x[n][i+1]);
  }
  x[n][COLS-1]+=2*a*x[n][COLS-2];

  // Undo update 1
  a=0.05298011854;
  for (i=2;i<COLS;i+=2) {
    x[n][i]+=a*(x[n][i-1]+x[n][i+1]);
  }
  x[n][0]+=2*a*x[n][1];

  // Undo predict 1
  a=1.586134342;
  for (i=1;i<COLS-2;i+=2) {
    x[n][i]+=a*(x[n][i-1]+x[n][i+1]);
  }
  x[n][COLS-1]+=2*a*x[n][COLS-2];
}

void partialrowiwt(float x[ROWS][COLS],int n,int startCol, int numCols){
  float a;
  int i;

  // Unpack
  //if (tempbank==0) tempbank=(double *)malloc(n*sizeof(double));
  for (i=0;i<numCols/2;i++) {
    tempbank[i*2]=x[n][i+startCol];
    tempbank[i*2+1]=x[n][i+startCol+numCols/2];
  }
  for (i=0;i<numCols;i++) x[n][i+startCol]=tempbank[i];

  // Undo scale
  a=1.149604398;
  for (i=0;i<numCols;i++) {
    if (i%2) x[n][i+startCol]*=a;
    else x[n][i+startCol]/=a;
  }

  // Undo update 2
  a=-0.4435068522;
  for (i=2;i<numCols;i+=2) {
    x[n][i+startCol]+=a*(x[n][i+startCol-1]+x[n][i+startCol+1]);
  }
  x[n][0+startCol]+=2*a*x[n][1+startCol];

  // Undo predict 2
  a=-0.8829110762;
  for (i=1;i<numCols-2;i+=2) {
    x[n][i+startCol]+=a*(x[n][i+startCol-1]+x[n][i+startCol+1]);
  }
  x[n][numCols+startCol-1]+=2*a*x[n][numCols+startCol-2];

  // Undo update 1
  a=0.05298011854;
  for (i=2;i<numCols;i+=2) {
    x[n][i+startCol]+=a*(x[n][i+startCol-1]+x[n][i+startCol+1]);
  }
  x[n][0+startCol]+=2*a*x[n][1+startCol];

  // Undo predict 1
  a=1.586134342;
  for (i=1;i<numCols-2;i+=2) {
    x[n][i+startCol]+=a*(x[n][i+startCol-1]+x[n][i+startCol+1]);
  }
  x[n][numCols+startCol-1]+=2*a*x[n][numCols+startCol-2];
}


void coliwt(float x[ROWS][COLS],int n){
    float a;
    int i;

    // Unpack
    //if (tempbank==0) tempbank=(double *)malloc(n*sizeof(double));
    for (i=0;i<ROWS/2;i++) {
      tempbank[i*2]=x[i][n];
      tempbank[i*2+1]=x[i+ROWS/2][n];
    }
    for (i=0;i<ROWS;i++) x[i][n]=tempbank[i];

    // Undo scale
    a=1.149604398;
    for (i=0;i<ROWS;i++) {
      if (i%2) x[i][n]*=a;
      else x[i][n]/=a;
    }

    // Undo update 2
    a=-0.4435068522;
    for (i=2;i<ROWS;i+=2) {
      x[i][n]+=a*(x[i-1][n]+x[i+1][n]);
    }
    x[0][n]+=2*a*x[1][n];

    // Undo predict 2
    a=-0.8829110762;
    for (i=1;i<ROWS-2;i+=2) {
      x[i][n]+=a*(x[i-1][n]+x[i+1][n]);
    }
    x[ROWS-1][n]+=2*a*x[ROWS-2][n];

    // Undo update 1
    a=0.05298011854;
    for (i=2;i<ROWS;i+=2) {
      x[i][n]+=a*(x[i-1][n]+x[i+1][n]);
    }
    x[0][n]+=2*a*x[1][n];

    // Undo predict 1
    a=1.586134342;
    for (i=1;i<ROWS-2;i+=2) {
      x[i][n]+=a*(x[i-1][n]+x[i+1][n]);
    }
    x[ROWS-1][n]+=2*a*x[ROWS-2][n];
}


void partialcoliwt(float x[ROWS][COLS],int n,int startRow, int numRows){
  float a;
  int i;

  // Unpack
  //if (tempbank==0) tempbank=(double *)malloc(n*sizeof(double));
  for (i=0;i<numRows/2;i++) {
    tempbank[i*2]=x[i+startRow][n];
    tempbank[i*2+1]=x[i+startRow+numRows/2][n];
  }
  for (i=0;i<numRows;i++) x[i+startRow][n]=tempbank[i];

  // Undo scale
  a=1.149604398;
  for (i=0;i<numRows;i++) {
    if (i%2) x[i+startRow][n]*=a;
    else x[i+startRow][n]/=a;
  }

  // Undo update 2
  a=-0.4435068522;
  for (i=2;i<numRows;i+=2) {
    x[i+startRow][n]+=a*(x[i+startRow-1][n]+x[i+startRow+1][n]);
  }
  x[0+startRow][n]+=2*a*x[1+startRow][n];

  // Undo predict 2
  a=-0.8829110762;
  for (i=1;i<numRows-2;i+=2) {
    x[i+startRow][n]+=a*(x[i+startRow-1][n]+x[i+startRow+1][n]);
  }
  x[startRow+numRows-1][n]+=2*a*x[startRow+numRows-2][n];

  // Undo update 1
  a=0.05298011854;
  for (i=2;i<numRows;i+=2) {
    x[i+startRow][n]+=a*(x[i+startRow-1][n]+x[i+startRow+1][n]);
  }
  x[0+startRow][n]+=2*a*x[1+startRow][n];

  // Undo predict 1
  a=1.586134342;
  for (i=1;i<numRows-2;i+=2) {
    x[i+startRow][n]+=a*(x[i+startRow-1][n]+x[i+startRow+1][n]);
  }
  x[startRow+numRows-1][n]+=2*a*x[startRow+numRows-2][n];

}








/*

int main() {
  int i;
  int j;
  // Makes a fancy cubic signal
  //for (i=0;i<32;i++) x[0][i]=5+i+0.4*i*i-0.02*i*i*i; //rows

  for (i=0;i<ROWS;i++){
    for (j=0; j< COLS; j++){
      x[i][j]=5+i+0.4*i*i-0.02*i*i*j;
    }
  }

  // Prints original sigal x
  printf("Original signal:\n");
  //for (i=0;i<32;i++) printf("x[%d]=%f\n",i,x[0][i]); //rows
  for (i=0;i<10;i++) {
    printf("x[%d]=",i);
    for (j=0; j< 10; j++){
      printf("%f ",x[i][j]); //cols
    }
    printf("\n");
  }

  // Do the forward 9/7 transform
  for(i = 0; i<10; i++){
    partialrowdwt(i,0,10);
  }for(i = 0; i< 10; i++){
    partialcoldwt(i,0,10);
  }



  // Prints the wavelet coefficients
  printf("Wavelets coefficients:\n");
  //for (i=0;i<32;i++) printf("wc[%d]=%f\n",i,x[0][i]); //rows
  for (i=0;i<10;i++) {
    printf("x[%d]=",i);
    for (j=0; j< 10; j++){
      printf("%f ",x[i][j]); //cols
    }
    printf("\n");
  }

  printf("\n");

  // Do the inverse 9/7 transform
  for(i = 0; i<10; i++){
    partialrowiwt(i,0,10);
  }for(i = 0; i< 10; i++){
    partialcoliwt(i,0,10);
  }

  // Prints the reconstructed signal
  printf("Reconstructed signal:\n");
  //for (i=0;i<32;i++) printf("xx[%d]=%f\n",i,x[0][i]); //rows
  for (i=0;i<10;i++) {
    printf("x[%d]=",i);
    for (j=0; j< 10; j++){
      printf("%f ",x[i][j]); //cols
    }
    printf("\n");
  }
}
*/
