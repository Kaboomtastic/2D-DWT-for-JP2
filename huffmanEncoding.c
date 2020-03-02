//
// Created by perez on 3/1/2020.
//

#include <stdlib.h>
#include <stdio.h>
#include "huffmanEncoding.h"

void concatenate(char *currCode, char *prevCode, char direction){
    int i = 0;
    while(*(prevCode+i) != '\0') {
        *(currCode+i) = *(prevCode+i);
        i++;
    }
    *(currCode+i) = direction;
    *(currCode+i+1) = '\0';
}

void huffEncoding(int *arr) {
    int length = arr[0];

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

    struct nodes nodesArr[length/2];
    //struct nodes *nodesArr = malloc((sizeof *nodesArr) * (length/2));
    for (int i = 0; i < length/2; i++){
        nodesArr[i].freq = 255;
        nodesArr[i].value = 255;
    }

    for(int i = 1; i < length; i+=2){
        int count = arr[i+1];
        for(int j=i+2; j<length; j+=2){
            if(arr[i] == arr[j]){
                count+=arr[j+1];
                nodesArr[j/2].freq = 256;
                nodesArr[j/2].value = 256;
            }
        }
        if(nodesArr[i/2].value != 256){
            nodesArr[i/2].freq = count;
            nodesArr[i/2].value = arr[i];
        }
    }

    int nodes = 0;
    for(int i = 0; i < length/2; i++){
        if(nodesArr[i].freq != 256){
            nodes++;
        }
    }

    //allocate space for array of initial nodes and array of resultant parent nodes
    int totalNodes = (2 * nodes) - 1;
    struct pixFreq freqArr[totalNodes];
    struct huffcode huffArr[nodes];
    for(int i = 0; i < totalNodes; i++){
        freqArr[i].code = (char*)malloc(sizeof(char) * (totalNodes));
    }

    //populate arrays
    int j = 0;
    for (int i = 0; i < length/2; i++){
        if(nodesArr[i].freq != 256){
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
        freqArr[newNode].left = &freqArr[huffArr[nodes-n-1].arrloc];
        freqArr[newNode].right = &freqArr[huffArr[nodes-n-2].arrloc];
        freqArr[newNode].code[0] = '\0';

        int i = 0;
        while (sumFreq <= huffArr[i].freq) {
            i++;
        }

        for(k = nodes-1; k >= 0; k--){
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
    for (int i = totalNodes-1; i >= nodes; i--){
        if(freqArr[i].left != NULL){
            concatenate(freqArr[i].left->code, freqArr[i].code, left);
        }
        if(freqArr[i].right != NULL){
            concatenate(freqArr[i].right->code, freqArr[i].code, right);
        }
    }

    printf("Huffman Codes for Coefficients\n");
    for(int i = 0; i < nodes; i++){
        printf("%d           -> %s\n", freqArr[i].value, freqArr[i].code);
    }
}

