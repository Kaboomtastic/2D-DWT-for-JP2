

int* diffEncode(int* data,int D){
  int* dest = (int*)malloc(sizeof(int)*D*D);

  dest[0] = data[0];
  for(int i = 1; i < D*D; i++){
    dest[i] = data[i] - data[i-1];
  }
  for(int i = 0; i < D*D; i++){
    if(i%D == 0){
      printf("\n");
    }
    printf("%d ",dest[i]);

  }
  printf("\n");
  printf("\n");
  return dest;
}

int* diffDecode(int* data, int D){
  
}
