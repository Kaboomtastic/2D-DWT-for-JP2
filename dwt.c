/**
 *  dwt97.c - Fast discrete biorthogonal CDF 9/7 wavelet forward and inverse transform (lifting implementation)
 *
 *  This code is provided "as is" and is given for educational purposes.
 *  2006 - Gregoire Pau - gregoire.pau@ebi.ac.uk
 */

#include <stdio.h>
#include <stdlib.h>

//double *tempbank=0;

#define ROWS 32
#define COLS 32

int x [128][128];
int tempbank [128];

void coldwt(int n){ //n is the current col number
  int a;
  int i;

  for(i = 0; i < ROWS; i++){
      x[i][n] = x[i][n] * 128;
  }

  // Predict 1
  //a=-1.586134342;
  a=-203;
  for (i=1;i<ROWS-2;i+=2) {
    x[i][n]+=a*(x[i-1][n]+x[i+1][n]);
  }
  x[ROWS-1][n]+=2*a*x[ROWS-2][n];

  // Update 1
  //a=-0.05298011854;
  a=-7;
  for (i=2;i<ROWS;i+=2) {
    x[i][n]+=a*(x[i-1][n]+x[i+1][n]);
  }
  x[0][n]+=2*a*x[1][n];

  // Predict 2
  //a=0.8829110762;
  a=113;
  for (i=1;i<ROWS-2;i+=2) {
    x[i][n]+=a*(x[i-1][n]+x[i+1][n]);
  }
  x[ROWS-1][n]+=2*a*x[ROWS-2][n];

  // Update 2
  //a=0.4435068522;
  a=57;
  for (i=2;i<ROWS;i+=2) {
    x[i][n]+=a*(x[i-1][n]+x[i+1][n]);
  }
  x[0][n]+=2*a*x[1][n];

  // Scale
  //a=1/1.149604398;
  //a=111;
  //for (i=0;i<ROWS;i++) {
  //  if (i%2) x[i][n]*=a;
  //  else x[i][n]/=a;
  //}

  // Pack
  //if (tempbank==0) tempbank=(double *)malloc(n*sizeof(double));
  for (i=0;i<ROWS;i++) {
    if (i%2==0) tempbank[i/2]=x[i][n];
    else tempbank[ROWS/2+i/2]=x[i][n];
  }

  for (i=0;i<ROWS;i++) x[i][n]=tempbank[i];

  //for (i=0;i<ROWS;i++) x[i][n] = x[i][n]/128;
}


void coliwt(int n){
    int a;
    int i;

    // Unpack
    //if (tempbank==0) tempbank=(double *)malloc(n*sizeof(double));
    for (i=0;i<ROWS/2;i++) {
      tempbank[i*2]=x[i][n];
      tempbank[i*2+1]=x[i+ROWS/2][n];
    }
    for (i=0;i<ROWS;i++) x[i][n]=tempbank[i];


    //for(i = 0; i < ROWS; i++) x[i][n] = x[i][n] * 128;

    // Undo scale
    //a=1.149604398;
    //a=147;
    //for (i=0;i<ROWS;i++) {
    //  if (i%2) x[i][n]*=a;
    //  else x[i][n]/=a;
    //}

    // Undo update 2
    //a=-0.4435068522;
    a=-57;
    for (i=2;i<ROWS;i+=2) {
      x[i][n]+=a*(x[i-1][n]+x[i+1][n]);
    }
    x[0][n]+=2*a*x[1][n];

    // Undo predict 2
    //a=-0.8829110762;
    a=-113;
    for (i=1;i<ROWS-2;i+=2) {
      x[i][n]+=a*(x[i-1][n]+x[i+1][n]);
    }
    x[ROWS-1][n]+=2*a*x[ROWS-2][n];

    // Undo update 1
    //a=0.05298011854;
    a=7;
    for (i=2;i<ROWS;i+=2) {
      x[i][n]+=a*(x[i-1][n]+x[i+1][n]);
    }
    x[0][n]+=2*a*x[1][n];

    // Undo predict 1
    //a=1.586134342;
    a=203;
    for (i=1;i<ROWS-2;i+=2) {
      x[i][n]+=a*(x[i-1][n]+x[i+1][n]);
    }
    x[ROWS-1][n]+=2*a*x[ROWS-2][n];


    for(i = 0;i<COLS;i++) x[i][n] = x[i][n]/128;
}


int main() {
  int i;

  // Makes a fancy cubic signal
  //for (i=0;i<32;i++) x[0][i]=50;//5+i+0.4*i*i-0.02*i*i*i; //rows

  for (i=0;i<32;i++) x[i][0]= 50;//5+i+4*i*i;//-2*i*i*i; //cols

  // Prints original sigal x
  printf("Original signal:\n");
  //for (i=0;i<32;i++) printf("x[%d]=%i\n",i,x[0][i]); //rows
  for (i=0;i<32;i++) printf("x[%i]=%i\n",i,x[i][0]); //cols
  printf("\n");

  // Do the forward 9/7 transform
  //rowdwt(0);
  coldwt(0);

  // Prints the wavelet coefficients
  printf("Wavelets coefficients:\n");
  //for (i=0;i<32;i++) printf("wc[%d]=%i\n",i,x[0][i]); //rows
  for (i=0;i<32;i++) printf("wc[%i]=%i\n",i,x[i][0]); //cols
  printf("\n");

  // Do the inverse 9/7 transform
  //rowiwt(0);
  coliwt(0);

  // Prints the reconstructed signal
  printf("Reconstructed signal:\n");
  //for (i=0;i<32;i++) printf("x[%d]=%i\n",i,x[0][i]); //rows
  for (i=0;i<32;i++) printf("x[%i]=%i\n",i,x[i][0]); //cols
}
