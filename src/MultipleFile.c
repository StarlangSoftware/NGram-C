//
// Created by Olcay Taner YILDIZ on 16.09.2023.
//

#include <stdarg.h>
#include <FileUtils.h>
#include <string.h>
#include <StringUtils.h>
#include <Memory/Memory.h>
#include "MultipleFile.h"

/**
 * Constructor for MultipleFile class. Initializes the buffer reader with the first input file
 * from the fileNameList. MultipleFile supports simple multipart file system, where a text file is divided
 * into multiple files.
 * @param num Number of files.
 */
Multiple_file_ptr create_multiple_file(int num, ...) {
    va_list valist;
    Multiple_file_ptr result = malloc_(sizeof(Multiple_file), "create_multiple_file_1");
    result->index = 0;
    va_start(valist, num);
    result->count = num;
    result->file_name_list = malloc_(num * sizeof(char*), "create_multiple_file_2");
    for (int i = 0; i < num; i++){
        char* tmp = va_arg(valist, char*);
        result->file_name_list[i] = str_copy(result->file_name_list[i], tmp);
    }
    result->reader = fopen(result->file_name_list[0], "r");
    va_end(valist);
    return result;
}

/**
 * Closes the buffer reader.
 */
void multiple_file_close(Multiple_file_ptr multiple_file) {
    fclose(multiple_file->reader);
}

/**
 * Reads a single line from the current file. If the end of file is reached for the current file,
 * next file is opened and a single line from that file is read. If all files are read, the method
 * returns NULL.
 * @return Read line from the current file.
 */
char *read_line(Multiple_file_ptr multiple_file) {
    char line[MAX_LINE_LENGTH];
    char* result = NULL;
    char* input = fgets(line, MAX_LINE_LENGTH, multiple_file->reader);
    if (input != NULL){
        line[strcspn(line, "\n")] = 0;
        result = str_copy(result, line);
        return result;
    } else {
        fclose(multiple_file->reader);
        multiple_file->index++;
        multiple_file->reader = fopen(multiple_file->file_name_list[multiple_file->index], "r");
        fgets(line, MAX_LINE_LENGTH, multiple_file->reader);
        line[strcspn(line, "\n")] = 0;
        result = str_copy(result, line);
        return result;
    }
}

/**
 * Accessor for the buffered reader
 * @return Buffered reader
 */
FILE *get_file(const Multiple_file* multiple_file) {
    return multiple_file->reader;
}

/**
 * Deallocates memory for the MultipleFile class.
 * @param multiple_file MultipleFile object.
 */
void free_multiple_file(Multiple_file_ptr multiple_file) {
    for (int i = 0; i < multiple_file->count; i++){
        free_(multiple_file->file_name_list[i]);
    }
    free_(multiple_file->file_name_list);
    free_(multiple_file);
}

/**
 * Constructor for MultipleFile class. Initializes the buffer reader with the first input file
 * from the fileNameList. MultipleFile supports simple multipart file system, where a text file is divided
 * into multiple files.
 * @param num Number of files.
 * @param valist A list of files given as dynamic parameters.
 */
Multiple_file_ptr create_multiple_file2(int num, va_list valist) {
    Multiple_file_ptr result = malloc_(sizeof(Multiple_file), "create_multiple_file2_1");
    result->index = 0;
    result->count = num;
    result->file_name_list = malloc_(num * sizeof(char*), "create_multiple_file2_2");
    for (int i = 0; i < num; i++){
        char* tmp = va_arg(valist, char*);
        result->file_name_list[i] = str_copy(result->file_name_list[i], tmp);
    }
    result->reader = fopen(result->file_name_list[0], "r");
    return result;
}
