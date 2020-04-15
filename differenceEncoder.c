

int* diffEncode(int* data,int D){
  int* dest = (int*)malloc(sizeof(int)*D*D);

  dest[0] = data[0];
  for(int i = 1; i < D*D; i++){
    dest[i] = data[i] - data[i-1];
  }
  
  printf("\n");
  printf("\n");
  return dest;
}

int* diffDecode(int8_t* data, int D){
  int8_t* dest = (int8_t*)malloc(D*D);
  int8_t prev = 0;
  prev = data[0];
  dest[0] = prev;
  for(int i = 1; i < D*D; i++){
    dest[i] = data[i] + prev;
    prev = dest[i];
  }
  return dest;

}
