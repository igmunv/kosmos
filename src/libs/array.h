#ifndef INCL_LIB_ARRAY
#define INCL_LIB_ARRAY

// удаление эллемента по индексу
void array_uint32_remove_element_by_index(unsigned int *array, unsigned int* size, unsigned int index);

// удаление одного эллемента по значению
void array_uint32_remove_element_by_value(unsigned int *array, unsigned int* size, unsigned int value);

// удаление всех эллементов по значению
void array_uint32_remove_all_element_by_value(unsigned int *array, unsigned int* size, unsigned int value);

#endif
