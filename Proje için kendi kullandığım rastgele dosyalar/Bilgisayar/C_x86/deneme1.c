#include <stdio.h>
#include <stdlib.h>

unsigned char rx_buf[3] = {0,0,0};

void shift_arr(unsigned char *pARR, char new_char){
    pARR[0] = pARR[1];
    pARR[1] = pARR[2];
    pARR[2] = new_char;
}

int main(void){
    printf("\n_PROG_START_\n\n");

    for (int i = 0; i < 3; i++) {
      printf("%d ",rx_buf[i]);
    }
    printf("\n");

    shift_arr(rx_buf,1);

    for (int i = 0; i < 3; i++) {
      printf("%d ",rx_buf[i]);
    }
    printf("\n");

    shift_arr(rx_buf,2);

    for (int i = 0; i < 3; i++) {
      printf("%d ",rx_buf[i]);
    }
    printf("\n");

    shift_arr(rx_buf,3);

    for (int i = 0; i < 3; i++) {
      printf("%d ",rx_buf[i]);
    }
    printf("\n");
    
    printf("\n\n_PROG_END_\n\n");
    return 1;
}