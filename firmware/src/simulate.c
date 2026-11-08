
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "frame.h"



#define MAX_OUT 4
#define CAN_MAX_DATA 8

// ---------------------------------------------------------------- helpers

bool compare_original(uint8_t* data, uint8_t len)
{
    uint8_t data_original[CAN_MAX_DATA];
    CanFrame can_frame;

    printf("original data: ");
    for(int idx=0; idx<CAN_MAX_DATA; idx++){
        data_original[idx] = data[idx];
        can_frame.data[idx] = data[idx];
        printf("%d, ", data_original[idx]);
    }
    printf("\n");
    
    can_frame.id = 2;
    can_frame.len = len;

    uint8_t encoded_data[13];
    printf("encoding frame\n");
    if(encodeFrame(&can_frame, encoded_data)){
        printf("encoding frame succesfull");
    }else{
        printf("encoding frame failed.\n");
        return false;
    }

    CanFrame decoded_frame;
    printf("decode frame\n");
    if(decodeFrame(encoded_data, &decoded_frame)){
        printf("decoding successull!\n");
    }else{
        printf("decoding failed\n");
        return false;
    }
    bool result = true;

    printf("comparing frames:\n");
 
    result = (can_frame.id == decoded_frame.id) && result;
    printf("id_original: %d, id_decoded: %d\n", can_frame.id, decoded_frame.id);

    // len
    result = (can_frame.len == decoded_frame.len) && result;;
    printf("len_original: %d, len_decoded%d\n", can_frame.len, decoded_frame.len);

    for(int idx=0; idx<CAN_MAX_DATA; idx++){
        result = (can_frame.data[idx] == decoded_frame.data[idx]) && result;;
        printf("data[%d]_original: %d, data[%d]_decoded:%d\n",idx, can_frame.data[idx], idx,decoded_frame.data[idx]);
    }
    return result;

}


int main_sim(){
    uint8_t data_test[8];
    data_test[0] = 10;
    data_test[1] = 18;
    int len = 8;

    bool result;
    result = compare_original(&data_test, len);

    printf("result: %d", result);
}