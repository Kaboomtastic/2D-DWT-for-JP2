/**
 *  dwt97.c - Fast discrete biorthogonal CDF 9/7 wavelet forward and inverse transform (lifting implementation)
 *
 *  This code is provided "as is" and is given for educational purposes.
 *  2006 - Gregoire Pau - gregoire.pau@ebi.ac.uk
 */

#include <stdio.h>
#include <stdlib.h>

//double *tempbank=0;

#define ROWS 128
#define COLS 128

int64_t x [128][128];
int tempbank [128];


void coldwt(int n){ //n is the current col number
  int a;
  int i;

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
  a=111;
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
  for (i=0;i<ROWS;i++) x[i][n]=tempbank[i] /(128*9);
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
void rowdwt(int n) { //n is the current row number
  int a;
  int i;

  // Predict 1
  //a=-1.586134342;
  a=-203;
  for (i=1;i<COLS-2;i+=2) {
    x[n][i]+=a*(x[n][i-1]+x[n][i+1]);
  }
  x[n][COLS-1]+=2*a*x[n][COLS-2];

  // Update 1
  //a=-0.05298011854;
  a=-7;
  for (i=2;i<COLS;i+=2) {
    x[n][i]+=a*(x[n][i-1]+x[n][i+1]);
  }
  x[n][0]+=2*a*x[n][1];

  // Predict 2
  //a=0.8829110762;
  a=113;
  for (i=1;i<COLS-2;i+=2) {
    x[n][i]+=a*(x[n][i-1]+x[n][i+1]);
  }
  x[n][COLS-1]+=2*a*x[n][COLS-2];

  // Update 2
  //a=0.4435068522;
  a=57;
  for (i=2;i<COLS;i+=2) {
    x[n][i]+=a*(x[n][i-1]+x[n][i+1]);
  }
  x[n][0]+=2*a*x[n][1];

  // Scale
  a=111;
  //a=1/1.149604398;
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
  for (i=0;i<COLS;i++) x[n][i]=tempbank[i] /(128*9);
}

/**
 *  iwt97 - Inverse biorthogonal 9/7 wavelet transform
 *
 *  This is the inverse of fwt97 so that iwt97(fwt97(x,n),n)=x for every signal x of length n.
 *
 *  See also fwt97.
 */
void rowiwt(int n) { //n is the current row
  int a;
  int i;

  // Unpack
  //if (tempbank==0) tempbank=(double *)malloc(n*sizeof(double));
  for (i=0;i<COLS/2;i++) {
    tempbank[i*2]=x[n][i];
    tempbank[i*2+1]=x[n][i+COLS/2];
  }
  for (i=0;i<COLS;i++) x[n][i]=tempbank[i];

  // Undo scale
  //a=1.149604398;
  a=147;
  for (i=0;i<COLS;i++) {
    if (i%2) x[n][i]*=a;
    else x[n][i]/=a;
  }

  // Undo update 2
  //a=-0.4435068522;
  a=-57;
  for (i=2;i<COLS;i+=2) {
    x[n][i]+=a*(x[n][i-1]+x[n][i+1]);
  }
  x[n][0]+=2*a*x[n][1];

  // Undo predict 2
  //a=-0.8829110762;
  a=-113;
  for (i=1;i<COLS-2;i+=2) {
    x[n][i]+=a*(x[n][i-1]+x[n][i+1]);
  }
  x[n][COLS-1]+=2*a*x[n][COLS-2];

  // Undo update 1
  //a=0.05298011854;
  a=-7;
  for (i=2;i<COLS;i+=2) {
    x[n][i]+=a*(x[n][i-1]+x[n][i+1]);
  }
  x[n][0]+=2*a*x[n][1];

  // Undo predict 1
  //a=1.586134342;
  a=-203;
  for (i=1;i<COLS-2;i+=2) {
    x[n][i]+=a*(x[n][i-1]+x[n][i+1]);
  }
  x[n][COLS-1]+=2*a*x[n][COLS-2];


}

void coldiwt(int n){
    double a;
    int i;

    // Unpack
    //if (tempbank==0) tempbank=(double *)malloc(n*sizeof(double));
    for (i=0;i<ROWS/2;i++) {
      tempbank[i*2]=x[i][n];
      tempbank[i*2+1]=x[i+ROWS/2][n];
    }
    for (i=0;i<ROWS;i++) x[i][n]=tempbank[i];

    // Undo scale
    //a=1.149604398;
    a=147;
    for (i=0;i<ROWS;i++) {
      if (i%2) x[i][n]*=a;
      else x[i][n]/=a;
    }

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
    a=-7;
    for (i=2;i<ROWS;i+=2) {
      x[i][n]+=a*(x[i-1][n]+x[i+1][n]);
    }
    x[0][n]+=2*a*x[1][n];

    // Undo predict 1
    //a=1.586134342;
    a=-203;
    for (i=1;i<ROWS-2;i+=2) {
      x[i][n]+=a*(x[i-1][n]+x[i+1][n]);
    }
    x[ROWS-1][n]+=2*a*x[ROWS-2][n];
}


int main() {
  int i;

  // Makes a fancy cubic signal
  //for (i=0;i<32;i++) x[0][i]=5+i+0.4*i*i-0.02*i*i*i; //rows

  for (i=0;i<32;i++) x[i][0]= 10;//5+i+4*i*i;//-2*i*i*i; //cols

  // Prints original sigal x
  printf("Original signal:\n");
  //for (i=0;i<32;i++) printf("x[%d]=%f\n",i,x[0][i]); //rows
  for (i=0;i<32;i++) printf("x[%i]=%i\n",i,x[i][0]); //cols
  printf("\n");

  // Do the forward 9/7 transform
  //rowdwt(0);
  coldwt(0);

  // Prints the wavelet coefficients
  printf("Wavelets coefficients:\n");
  //for (i=0;i<32;i++) printf("wc[%d]=%f\n",i,x[0][i]); //rows
  for (i=0;i<32;i++) printf("wc[%i]=%i\n",i,x[i][0]); //cols
  printf("\n");

  // Do the inverse 9/7 transform
  //rowiwt(0);
  coldiwt(0);

  // Prints the reconstructed signal
  printf("Reconstructed signal:\n");
  //for (i=0;i<32;i++) printf("xx[%d]=%f\n",i,x[0][i]); //rows
  for (i=0;i<32;i++) printf("x[%i]=%i\n",i,x[i][0]); //cols
}
