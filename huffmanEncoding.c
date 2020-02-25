#include <stdio.h>
#include <stdlib.h>

void huffEncoding(void) {
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
    }

    struct pixFreq* freqArr;
    struct huffcode* huffArr;
    struct nodes* nodesArr = (struct nodes*)malloc(sizeof(struct nodes) * (***length of encoded array/2));



    int nodes = 0;
    for(int i = 0; i < length; i+=2){
      int count = arr[i+1];
      for(int j=i+2; j<length; j+=2){
          if(arr[i] == arr[j]){
            count+=arr[j+1];
            nodesArr[j/2].freq = 0;
            nodesArr[j/2].value = 0;
          }
      }
      if(nodesArr[i/2].freq != 0){
        nodesArr[i/2].freq = count;
        nodesArr[i/2].value = arr[i];
        nodes++;
      }
    }

    //allocate space for array of initial nodes and array of resultant parent nodes
    int totalNodes = (2 * nodes) - 1;
    freqArr = (struct pixFreq*)malloc(sizeof(struct pixFreq) * totalNodes);
    huffArr = (struct huffcode*)malloc(sizeof(struct huffcode) * nodes);

    //populate arrays
    j = 0;
    for (int i = 0; i < nodes; i++){
      if(nodesArr[i].freq != 0){
        //value and frequency
        huffArr[j].value = nodesArr[i].value;
        huffArr[j].freq = nodesArr[i].freq;
        freqArr[j].value = nodesArr[i].value;
        freqArr[j].freq = nodesArr[i].freq;
        freqArr[j].code[0] ='\0';

        //location of node in original array
        huffArr[j].arrloc = j;

        //declare child of leaf
        freqArr[j].left = NULL;
        freqArr[j].right = NULL;


        j++;
      }
    }

    //sort probabilities
    struct huffcodes temp;
    for (int i=0; i < nodes; i++){
      for (int j=i+1; j < nodes; j++){
        if (huffcodes[i].freq < huffcodes[j].freq){
            temp = huffcodes[i];
            huffcodes[i] = huffcodes[j];
            huffcodes[j] = temp;
        }
      }
    }

    //build tree
    int sumFreq;
    int sumValue;
    int n=0, k=0;
    int newNode = nodes;

    while(n < nodes-1){
      //add lowest two frequencies and values
      sumFreq = huffArr[nodes-n-1].freq + huffArr[nodes-n-2].freq;
      sumValue = huffArr[nodes-n-1].value + huffArr[nodes-n-2].value;

      //append to freqArr
      freqArr[newNode].value = sumValue;
      freqArr[newNode].freq = sumFreq;
      freqArr[newNode].left = &freqArr[huffArr[nodes-n-2].arrloc];
      freqArr[newNode].right = &freqArr[huffArr[nodes-n-1].arrloc];
      freqArr[newNode].code[0] = '\0';

      i = 0;
      while (sumFreq <= huffArr[i].freq) {
        i++;
      }

      for(k = nodes; k >= 0; k--){
        if (k == i) {
          huffArr[k].value = sumValue;
          huffArr[k].freq = sumFreq;
          huffArr[k].arrloc = newNode;
        } else if (k > i){
          huffArr[k] = huffArr[k-1];
        }
      }
      n += 1;
      newNode += 1;
    }

    //assign codes
    char left = '0';
    char right = '1';
    int index;
    for (i = totalNodes-1; i >= nodes; i--){
      if(freqArr[i].left != NULL){
        concatenate(freqArr[i].left->code, freqArr[i].code, left);
      }
      if(freqArr[i].right != NULL){
        concatenate(freqArr[i].right->code, freqArr[i].code, right);
      }
    }

    void concatenate(char* currCode, char* prevCode, char direction){
      int i = 0;
      while(*(prevCode+i) != '\0') {
        *(currCode+i) = *(prevCode+i);
        i++;
      }
      *(currCode+i) = direction;
      *(currCode+i+1) = '\0';
    }

    printf("Huffman Codes for Coefficients\n");
    for(int i = 0; i < nodes; i++){
      printf("%d          -> %s", freqArr[i].value, freqArr[i].code);
    }
}