#include "array.h"


void array_uint32_remove_element_by_index(unsigned int *array, unsigned int* size, unsigned int index){

    if (index >= *size) return;

    for (unsigned int i = index; i < *size-1; i++){
        array[i] = array[i+1];
    }
    (*size)--;
}

void array_uint32_remove_element_by_value(unsigned int *array, unsigned int* size, unsigned int value){

    for (unsigned int index = 0; index < *size; index++){
        if (array[index] == value){
            array_uint32_remove_element_by_index(array, size, index);
            break;
        }
    }
}

void array_uint32_remove_all_element_by_value(unsigned int *array, unsigned int* size, unsigned int value){
    unsigned int i = 0;
    while (i < *size) {
        if (array[i] == value) {
            array_uint32_remove_element_by_index(array, size, i);
        } else {
            i++;
        }
    }
}
