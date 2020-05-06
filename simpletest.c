
#include "dwt.c"


void main(){
  float x [64][64];

  for(int i = 0; i < 64; i++){
    x[0][i] = i;
  }

  rowdwt(x,0);
  for(int i = 0; i < 64; i++){
    printf("%f ",x[0][i] );
  }
  printf("\n");
  rowiwt(x,0);
  for(int i = 0; i < 64; i++){
    printf("%f ",x[0][i] );
  }

}
