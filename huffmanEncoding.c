//
// Created by perez on 3/1/2020.
//

#include <stdlib.h>
#include <stdio.h>
//#include "huffmanEncoding.h"

int numNodes = 1200;

void concatenate(char *currCode, char *prevCode, char direction){
    int i = 0;
    while(*(prevCode+i) != '\0') {
        *(currCode+i) = *(prevCode+i);
        i++;
    }
    *(currCode+i) = direction;
    *(currCode+i+1) = '\0';
}
void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    for (i=size-1;i>=0;i--)
    {
        for (j=7;j>=0;j--)
        {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}


int8_t* huffEncodingNEW(int *runLength) {
  int length = runLength[0];

  struct pixFreq {
      int value;
      int freq;
      struct pixFreq *left, *right;
      char *code;
  };

  struct huffcode {
      int value;
      int freq;
      int arrloc;
  };

  struct nodes {
      int value;
      int freq;
  };

  struct nodes nodesArr[numNodes];

  for (int i = 0; i < numNodes; i++){
      nodesArr[i].freq = 0;
      nodesArr[i].value = numNodes;
  }

  int tempVal = 0;
  for(int i = 1; i <= length; i++){
    //printf("%d\n",runLength[i]);
    tempVal = runLength[i];
    if(nodesArr[tempVal+numNodes/2].value == numNodes){
      nodesArr[tempVal+numNodes/2].freq = 1;
      nodesArr[tempVal+numNodes/2].value = tempVal;
    }else{
      nodesArr[tempVal+numNodes/2].freq = nodesArr[tempVal+numNodes/2].freq + 1;
    }

  }


  int nodes = 0;
  for(int i = 0; i < numNodes; i++){
      if(nodesArr[i].value != numNodes){
          nodes++;
      }
  }

  //int totalNodes = nodes;
  struct pixFreq freqArr[numNodes];
  struct huffcode huffArr[numNodes];

  //printf("nodes(%d)\n",nodes );

  int j = 0;

  for(int i = 0; i < numNodes; i++){
    freqArr[i].code = (char*)malloc(sizeof(char) * ROWS);
    freqArr[j].left = NULL;
    freqArr[j].right = NULL;


  }

  for (int i = 0; i < numNodes; i++){

      if(nodesArr[i].value != numNodes){

          //value and frequency
          huffArr[j].value = nodesArr[i].value;
          huffArr[j].freq = nodesArr[i].freq;
          freqArr[j].value = nodesArr[i].value;
          freqArr[j].freq = nodesArr[i].freq;


          freqArr[j].code[0] ='\0';

          //location of node in original array
          huffArr[j].arrloc = j;

          //declare child of leaf

          j++;
      }
  }


  //sort probabilities
  struct huffcode temp;
  for (int i=0; i < nodes; i++){
      for (int j=i+1; j < nodes; j++){
          if (huffArr[i].freq < huffArr[j].freq){
              temp = huffArr[i];
              huffArr[i] = huffArr[j];
              huffArr[j] = temp;
          }
      }
  }

  int order [nodes];
  for(int i=0; i < nodes; i++){
    order[i] = huffArr[i].value;
  }

  //build tree
  int sumFreq;
  int sumValue;
  int n=0, k=0;
  int newNode = 0;

  newNode = nodes;
  while(n < nodes-1){

      //printf("nodes(%d) %d\n",nodes, n);
      //add lowest two frequencies and values
      sumFreq = huffArr[nodes-n-1].freq + huffArr[nodes-n-2].freq;
      sumValue = huffArr[nodes-n-1].value + huffArr[nodes-n-2].value;
      //printf("here1 nodes(%d) k(%d)\n",nodes,k);
      //append to freqArr
      freqArr[newNode].value = sumValue;
      freqArr[newNode].freq = sumFreq;
      freqArr[newNode].left = &freqArr[huffArr[nodes-n-1].arrloc];
      freqArr[newNode].right = &freqArr[huffArr[nodes-n-2].arrloc];

      //printf("here nodes(%d) k(%d)\n",nodes,k);
      freqArr[newNode].code[0] = '\0';

      int i = 0;
      while (sumFreq <= huffArr[i].freq) {
          i++;
      }

      for(k = nodes-1; k >= 0; k--){
        //printf("nodes(%d) k(%d)\n",nodes,k);
          if (k == i) {
              huffArr[k].value = sumValue;
              huffArr[k].freq = sumFreq;
              huffArr[k].arrloc = newNode;
          } else if (k > i && k>0){
              huffArr[k] = huffArr[k-1];
          }
      }
      n += 1;
      newNode += 1;
  }
/*
  for(int i = 0; i < newNode; i++){
    printf("%d %d\n", huffArr[i].value, huffArr[i].freq);
  }

  printf("frequencies\n");
  for (int i=0; i < newNode; i++){
    printf("%d %d\n",freqArr[i].freq,freqArr[i].value);
  }
*/
  //assign codes
  char left = '0';
  char right = '1';
  int index;
  for (int i = newNode-1; i >= nodes; i--){
    //printf("i %d\n",i );
      if(freqArr[i].left != NULL){
          concatenate(freqArr[i].left->code, freqArr[i].code, left);
      }
      if(freqArr[i].right != NULL){
          concatenate(freqArr[i].right->code, freqArr[i].code, right);
      }
  }

  //printf("Huffman Codes for Coefficients\n");
  for(int i = 0; i < nodes; i++){
      //printf("%d \t-> %s\n", freqArr[i].value, freqArr[i].code);
  }




  u_int8_t output [length];
  u_int8_t tempbits;
  int count = 1;
  int byteCount = 0;
  int bitCount = 0;
  int replace;
  int charCount;

  for(int i = 0; i < length; i++){
    output[i] = 0;
  }
  byteCount+=2;
  output[byteCount] = nodes;
  byteCount++;

  for(int i = 0; i < nodes; i++){
    output[byteCount] = (int8_t)(freqArr[i].value);
    byteCount++;
    int codelen = (u_int8_t)strlen((freqArr[i].code));
    output[byteCount] = codelen;
    byteCount++;
    u_int16_t code = 0;
    for(int j = 0; j < codelen; j++){
      if(freqArr[i].code[codelen-1-j] == '1'){
        code |= 1 << j;
      }
    }
    output[byteCount] = code >> 8;
    byteCount++;
    output[byteCount] = code & 0x0FF;
    byteCount++;
  }
  while(count <= length){
    replace = 0;
    charCount = 0;
    int ValToReplace = runLength[count];
    char* valCode;
    for(int i = 0; i < numNodes; i++){
      if(ValToReplace == freqArr[i].value){
        valCode = freqArr[i].code;
        break;
      }
    }
    while(valCode[charCount] != '\0'){
      if(valCode[charCount] == '0'){
        replace = replace <<1;
      }else{
        replace = (replace << 1)+1;
      }
      charCount ++;
    }
    tempbits = 0;
    int numbits = 0;

    while(charCount > 0){
      //printf("Replace\t %x\n",replace);
      if(bitCount + charCount > 8){
        numbits = 8-bitCount;
        tempbits = (replace >> (charCount-numbits));
        output[byteCount] |= (tempbits);
        replace -= tempbits <<(charCount-numbits) ;
        charCount -= numbits;

        bitCount = 0;
        byteCount++;
      }else{
        tempbits = (replace << (8-bitCount-charCount));
        output[byteCount] |= tempbits;
        bitCount = bitCount + charCount;
        if(bitCount == 8){
          byteCount++;
          bitCount = 0;
        }
        charCount = 0;

      }
      //printBits(1,&tempbits);
      //printf("Tempbits %x\t %d\t %d\n", tempbits,charCount,byteCount);
    }


    count++;

  }
  output[0] = byteCount >> 8;
  output[1] = byteCount & 0x0FF;

  int8_t* ret = (int8_t*)malloc(byteCount);
  for(int i = 0; i< byteCount; i++){
    ret[i] = output[i];
  }
  //printf("%d\n",byteCount );

  return ret;
}








int8_t* huffmanDecode(int8_t* data){

  struct huffcode {
    int8_t value;
    u_int8_t codelen;
    u_int16_t code;
  };


  int count = 0;
  int len = 0;
  int numCodes = 0;

  len = (data[0] << 8) + ((u_int8_t) data[1]);

  count += 2;
  numCodes = data[count];
  count++;

  struct huffcode huffcodes[numCodes];

  for(int i = 0; i < numCodes; i++){
    huffcodes[i].value = (int8_t) data[count];
    count++;
    huffcodes[i].codelen = (u_int8_t) data[count];
    count++;
    huffcodes[i].code = ((u_int8_t)data[count]) << 8 | (u_int8_t)data[count+1];
    count+=2;
  }

  for(int i = 0; i < numCodes; i++){
    printf("%d %d %d\n",huffcodes[i].value,huffcodes[i].codelen,huffcodes[i].code);
  }

  u_int16_t decodelen = len - count;

  int8_t* dest = (int8_t*) malloc(64*64*2);

  int charCount = count;
  int bitCount = 7;
  u_int16_t tempCode = 0;
  int destcount = 2;
  int tempCodeLen = 0;
  u_int8_t bit = 0;
  int gotcode = 0;
  while(count < len){

    tempCode = 0;
    tempCodeLen = 0;
    while(1){
      if(count > len){
        break;
      }
      //printf("%x\n",data[charCount] );
      tempCode <<= 1;
      bit = ((data[charCount]>>bitCount) & 1);// >> bitCount;
      //printf("%x bit\n",bit);
      tempCode |= bit;
      //printf("%x\n",tempCode );
      tempCodeLen++;
      bitCount --;

      if(bitCount < 0){
        bitCount = 7;
        charCount++;
        count++;
      }

      for(int i = 0; i < numCodes; i++){
        //printf("val %x\n",huffcodes[i].code);
        if(tempCode == huffcodes[i].code && tempCodeLen == huffcodes[i].codelen){//} && tempCodeLen == huffcodes[i].codelen){
          dest[destcount] = huffcodes[i].value;
          destcount++;
          //printf("%i \t",huffcodes[i].value);
          gotcode = 1;
          break;
        }

      }
      if(gotcode){
        gotcode = 0;
        break;
      }
      if(count == len){
        break;
      }

    }


  }
  dest[0] = (u_int8_t)destcount>>8;
  dest[1] = (u_int8_t)destcount&0x0FF;

  return dest;


}
