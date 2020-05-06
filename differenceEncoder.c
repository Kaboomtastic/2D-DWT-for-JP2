#include "constants.h"

int* diffEncode(int* data,int D){
  int* dest = (int*)malloc(sizeof(int)*D*D);

  for(int i = 0; i < D*D; i++){
    if(i%(D/2) == 0){
      dest[i] = data[i];
    }else{
      dest[i] = data[i] - data[i-1];
    }
  }

  printf("\n");
  printf("\n");
  return dest;
}

int* diffDecode(int8_t* data, int D){
  int* dest = (int*)malloc(D*D);
  int8_t prev = 0;
  prev = data[0];

  for(int i = 0; i < D*D; i++){
    if(i%(D/2) == 0){
      dest[i] = data[i];
      prev = dest[i];
    }else{
      dest[i] = data[i] + prev;
      prev = dest[i];
    }
  }
  return dest;

}
