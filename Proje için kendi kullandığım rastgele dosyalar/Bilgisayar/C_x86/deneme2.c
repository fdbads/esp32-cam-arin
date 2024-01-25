#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define debug false

#define msg_size_max 50
#define arr_size_x msg_size_max
#define arr_size_y 20

#define two_to_oneD(x,y) x+y*arr_size_x


//xy, 41 is x=4 y=1, each arrx is a y-level
////unsigned char arr2D[arr_size_x*arr_size_y];

unsigned char * incomming_msg_ptr;
//unsigned char incomming_msg[msg_size_max] = {'A','T','\r','\n','O','K','\r','\n'};


//unsigned char arr2D_2(int x, int y){return arr2D[x+y*arr_size_x];}
void print_arr1D(char * arr){
    int i = 0;
    while(true){
        if (*(arr+i)==0) break;
        printf("arr[%d]:%d\n",i,*(arr+i));
        i++;
    }
    printf("\n");
}

void print_arr1D_ascii(char * arr){
    int i = 0;
    while(true){
        if (*(arr+i)==0) break;
        
        if(*(arr+i)==13)printf("arr[%d]:\\r",i);
        else if(*(arr+i)==10)printf("arr[%d]:\\n",i);
        else printf("arr[%d]:%c",i,*(arr+i));
        
        printf("\n");
        i++;
    }
    printf("\n");
}

void clr_arr1D(unsigned char * arr,int size){
    for(int i=0;i<size;i++){
        arr[i] = 0;
    }
}

void print_arr2D(unsigned char * arr){
    for(int y=0;y<arr_size_y;y++){
        for(int x=0;x<arr_size_x;x++){
        
            if (arr[two_to_oneD(x,y)]!=0) printf("%03d ",arr[two_to_oneD(x,y)]);
            else printf("_ ");

        }
        printf("\n");
    }
}

void clr_arr2D(unsigned char * arr){
    for(int i=0;i<arr_size_y*arr_size_x;i++){
        arr[i] = 0;
    }
}

void msg_to_2D(unsigned char * msg_arr,unsigned char * twoD_arr){

    int current_line = 0;
    int last_chop = 0;
    int write_index = 0;

    for(int i=0;i<msg_size_max;i++){

        if(debug)printf("msg_arr[%d]:%d\n",i,msg_arr[i]);

        if (i>0 && msg_arr[i-1]=='\r' && msg_arr[i]=='\n'){
            // \r\n found, index of \n is i

            //write this chunk to line
            write_index = 0;
            for(int j=last_chop;j<i+1;j++){

                if(debug)printf("  -chop_seg[%d]:%d\n",j,msg_arr[j]);

                twoD_arr[two_to_oneD(write_index,current_line)] = msg_arr[j];
                write_index++;
            }
            current_line++;
            last_chop = i+1;



            //erase chunk

            
            //break;

        }

    }

}

void msg_to_2D_2(unsigned char * msg_arr,unsigned char * twoD_arr){

    int current_line = 0;
    int last_chop = 0;
    int write_index = 0;
    bool flag;

    for(int i=0;i<msg_size_max;i++){

        if(debug)printf("msg_arr[%d]:%d\n",i,msg_arr[i]);

        if (i>0 && msg_arr[i-1]=='\r' && msg_arr[i]=='\n'){
            // \r\n found, index of \n is i

            //write this chunk to line
            write_index = 0;
            flag = false;
            for(int j=last_chop;j<i-1;j++){

                if(debug)printf("  -chop_seg[%d]:%d\n",j,msg_arr[j]);

                twoD_arr[two_to_oneD(write_index,current_line)] = msg_arr[j];
                write_index++;
                flag = true;
            }
            if(flag)current_line++;
            last_chop = i+1;



            //erase chunk

            
            //break;

        }

    }

}

int arr_len(unsigned char * arr){
    //number of elements in an array until first \0 encountered
    for(int i = 0; i<msg_size_max; i++){
        if (*(arr+i)=='\0'){
            return i;
        }
    }
}

bool response_valid_signature_check(unsigned char * msg_arr,const char signature_str_c[],int signature_position){
    //signature_position >= 0 means count from start, 
    //signature_position <  0 means count from end
    
    unsigned char signature_str[msg_size_max];
    strcpy(signature_str,signature_str_c);

    int msg_len = arr_len(msg_arr);
    int sig_len = arr_len(signature_str);
    

    if (signature_position<0){
        
        //turn negative index into positive forward direction index
        signature_position += msg_len;

    }

    
    for(int i = 0; i<sig_len; i++){
        if(*(msg_arr+signature_position+i) != *(signature_str+i)){
            return false;
        }
    }
    return true;
    
}

int main(void){

    /*
    incomming_msg_ptr = malloc(msg_size_max);

    if(incomming_msg_ptr == 0) return -1;

    //unsigned char arr[msg_size_max] = {'\r','\n','A','T','+','C','O','P','S','?','\r','\r','\n','+','C','O','P','S',':',' ','0',',','0',',','"','T','u','r','k','c','e','l','l','"',',','7','\r','\n','\r','\n','O','K','\r','\n'};
    unsigned char arr[msg_size_max] = {'\r','\n','A','T','7','\r','\n','\r','\n','O','K','\r','\n'};
    
    strcpy(incomming_msg_ptr,arr);

    printf("%d\n",arr_len(incomming_msg_ptr));
    printf("%d\n",response_valid_signature_check(incomming_msg_ptr,"OK",-4));
    printf("%d\n",response_valid_signature_check(incomming_msg_ptr,"AT",-4));
    printf("%d\n",response_valid_signature_check(incomming_msg_ptr,"OK",2));
    printf("%d\n",response_valid_signature_check(incomming_msg_ptr,"AT",2));
    

    
    
    

    free(incomming_msg_ptr);
    */

    int fb_len = 275981;
    //int temp_int = (int)(275981)/10+1;
    //printf("%d\n",(int)fb_len/10+1);
    printf("%d\n",(int)log10((double)fb_len)+2);
   
    char temp10[] = "AT+CFTRANRX=\"F:/dummy_img_2.jpg\",                    \r\n"; //pos 32 is file size
    char temp11[60];
    
    strcpy(temp11,temp10);

    sprintf(temp11+33,"%d",fb_len);

    //print_arr1D(temp11);

    strcpy(temp11+(int)log10((double)fb_len)+1+33,"\r\n");

    print_arr1D_ascii(temp11);

    
    //printf("%s\n",temp11);

}
/*
int main(void){

    //int * a = NULL;

    printf("%d\n",*a); //C doesnt raise error at null pointer access

}
*/