#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define max_package_len 512
#define len_arr 9

unsigned char arr[len_arr];

void print_arr1D(unsigned char * arr){
    int i = 0;
    while(true){
        if (*(arr+i)==0) break;
        //printf("arr[%d]:%d\n",i,*(arr+i));
        printf("%d",*(arr+i));
        i++;
    }
    printf("\n");
}

//-----------------------------------------------------------------


int main(void){

    for(int i = 0; i<len_arr; i++){
        *(arr+i) = i+65;
    }

    printf("%s\n",arr);
    //printf("%d\n",(void*)strstr(arr,"FG")-(void*)arr);
    printf("%d\n",strstr(arr,"F8G"));

    //           " A  T  +  C  R  E  G  ? \r \r \n  +  C  R  E  G  : ( ) 0  ,  1 \r \n \r \n  O  K \r \n";
    char arr[] = {65,84,43,67,82,69,71,63,13,13,10,43,67,82,69,71,58,32,48,44,49,13,10,13,10,79,75,13,10};
    /*
    char arr2[29];
    strcpy(arr2,arr);
    */

}