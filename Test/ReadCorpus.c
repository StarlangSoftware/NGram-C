//
// Created by Olcay Taner YILDIZ on 23.09.2023.
//

#include <FileUtils.h>
#include <stdio.h>
#include <string.h>
#include <StringUtils.h>
#include <math.h>
#include "ReadCorpus.h"

void perplexity_check(N_gram_ptr n_gram, Array_list_ptr corpus, double _perplexity){
    double perplexity = get_perplexity(n_gram, corpus);
    if (fabs(perplexity - _perplexity) > 0.000001){
        printf("Perplexity error %lf\n", perplexity);
    }
}

Array_list_ptr create_simple_corpus(){
    char* strings1[] = {"<s>", "ali", "topu", "at", "mehmet", "ayşeye", "gitti", "</s>"};
    Array_list_ptr text1 = create_array_list_of_string(strings1, 8);
    char* strings2[] = { "<s>", "ali", "top", "at", "ayşe", "eve", "gitti", "</s>"};
    Array_list_ptr text2 = create_array_list_of_string(strings2, 8);
    char* strings3[] = {"<s>", "ayşe", "kitabı", "ver", "</s>"};
    Array_list_ptr text3 = create_array_list_of_string(strings3, 5);
    char* strings4[] = {"<s>", "ali", "topu", "mehmete", "at", "</s>"};
    Array_list_ptr text4 = create_array_list_of_string(strings4, 6);
    char* strings5[] = {"<s>", "ali", "topu", "at", "mehmet", "ayşeyle", "gitti", "</s>"};
    Array_list_ptr text5 = create_array_list_of_string(strings5, 8);
    Array_list_ptr simple_corpus = create_array_list();
    array_list_add(simple_corpus, text1);
    array_list_add(simple_corpus, text2);
    array_list_add(simple_corpus, text3);
    array_list_add(simple_corpus, text4);
    array_list_add(simple_corpus, text5);
    return simple_corpus;
}

Array_list_ptr read_corpus(char *file_name) {
    char line[MAX_LINE_LENGTH];
    Array_list_ptr result = create_array_list();
    FILE* input_file = fopen(file_name, "r");
    char* input = fgets(line, MAX_LINE_LENGTH, input_file);
    while (input != NULL){
        line[strcspn(line, "\n")] = 0;
        Array_list_ptr items = str_split(line, ' ');
        array_list_add(result, items);
        input = fgets(line, MAX_LINE_LENGTH, input_file);
    }
    return result;
}
