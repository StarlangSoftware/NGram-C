//
// Created by Olcay Taner YILDIZ on 16.09.2023.
//

#ifndef NGRAM_MULTIPLEFILE_H
#define NGRAM_MULTIPLEFILE_H

#include <stdio.h>

struct multiple_file{
    FILE* reader;
    int index;
    char** file_name_list;
    int count;
};

typedef struct multiple_file Multiple_file;

typedef Multiple_file *Multiple_file_ptr;

Multiple_file_ptr create_multiple_file(int num, ...);

void free_multiple_file(Multiple_file_ptr multiple_file);

void multiple_file_close(Multiple_file_ptr multiple_file);

char* read_line(Multiple_file_ptr multiple_file);

FILE* get_file(const Multiple_file* multiple_file);

#endif //NGRAM_MULTIPLEFILE_H
