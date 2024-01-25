#include <stdio.h>
#include <stdlib.h>

#define imHeight 240
#define imWidth 320
#define im_arr_len imHeight*imWidth

unsigned char *im_arr;

int main(void){
    printf("\n_PROG_START_\n\n");
    
    printf("im_arr_len: %.2f kb",(float)im_arr_len/1024);
    im_arr = (unsigned char *)malloc(im_arr_len * sizeof(unsigned char));
    // Initialize the array elements
    for (int i = 0; i < im_arr_len; i++) {
        im_arr[i] = 1; // You can use any initialization value here
    }

    
    // go through whole array
    for (int i = 0; i < im_arr_len; i++) {
        if (i%3 == 0) im_arr[i] = 255; // R channel
        else if (i%3 == 1) im_arr[i] =   0; // G channel
        else if (i%3 == 2) im_arr[i] = 150; // B channel
    }

    
    // go through whole array
    printf("\n");
    for (int i = 0; i < im_arr_len; i++) {
        if (i%3 == 0) printf("\n");
        printf("%d ",(unsigned char)im_arr[i]);
        
    }
    

    printf("\n\n_PROG_END_\n\n");
    return 1;
}