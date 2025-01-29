//
// Created by Olcay Taner YILDIZ on 20.09.2023.
//

#ifndef NGRAM_NGRAM_H
#define NGRAM_NGRAM_H

#include "NGramNode.h"

struct n_gram{
    N_gram_node_ptr root_node;
    int N;
    double lambda1;
    double lambda2;
    bool interpolated;
    Hash_set_ptr vocabulary;
    double* probability_of_unseen;
};

typedef struct n_gram N_gram;

typedef N_gram *N_gram_ptr;

N_gram_ptr create_n_gram(Array_list_ptr corpus,
                         int N,
                         unsigned int (*hash_function)(const void *, int),
                         int (*key_compare)(const void *, const void *));

N_gram_ptr create_string_n_gram(Array_list_ptr corpus, int N);

N_gram_ptr create_n_gram2(int N,
                          unsigned int (*hash_function)(const void *, int),
                          int (*key_compare)(const void *, const void *));

N_gram_ptr create_string_n_gram2(int N);

N_gram_ptr create_n_gram3(const char* file_name,
                          unsigned int (*hash_function)(const void *, int),
                          int (*key_compare)(const void *, const void *));

N_gram_ptr create_string_n_gram3(const char* file_name);

N_gram_ptr create_n_gram4(FILE* input_file,
                          unsigned int (*hash_function)(const void *, int),
                          int (*key_compare)(const void *, const void *));

N_gram_ptr create_string_n_gram4(FILE* input_file);

N_gram_ptr create_n_gram5(unsigned int (*hash_function)(const void *, int),
                          int (*key_compare)(const void *, const void *),
                          int num,
                          ...);

N_gram_ptr create_string_n_gram5(int num, ...);

void free_n_gram(N_gram_ptr n_gram);

void add_n_gram(N_gram_ptr n_gram,
                Array_list_ptr s,
                int size);

void add_n_gram_sentence(N_gram_ptr n_gram,
                         Array_list_ptr symbols,
                         int sentence_count);

void load_n_gram(N_gram_ptr n_gram,
                 FILE* input_file);

int vocabulary_size(const N_gram* n_gram);

void set_lambda1(N_gram_ptr n_gram, double lambda1);

void set_lambda2(N_gram_ptr n_gram, double lambda1, double lambda2);

void set_probability_with_pseudo_count(N_gram_ptr n_gram, double pseudo_count, int height);

void replace_unknown_words(N_gram_ptr n_gram, Hash_set_ptr dictionary);

Hash_set_ptr construct_dictionary_with_non_rare_words(N_gram_ptr n_gram,
                                                      int level,
                                                      double probability);

int* calculate_counts_of_counts(const N_gram* n_gram, int height, int* size);

int maximum_occurrence(const N_gram* n_gram, int height);

void update_counts_of_counts(const N_gram* n_gram, int* counts_of_counts, int height);

double get_perplexity(const N_gram* n_gram, const Array_list* corpus);

double get_uni_gram_perplexity(const N_gram* n_gram, const Array_list* corpus);

double get_bi_gram_perplexity(const N_gram* n_gram, const Array_list* corpus);

double get_tri_gram_perplexity(const N_gram* n_gram, const Array_list* corpus);

double get_probability(const N_gram* n_gram, int num, ...);

double get_uni_gram_probability(const N_gram* n_gram, const void* w1);

double get_bi_gram_probability(const N_gram* n_gram, const void* w1, const void* w2);

double get_tri_gram_probability(const N_gram* n_gram, const void* w1, const void* w2, const void* w3);

int get_count(const N_gram* n_gram, void** symbols, int length);

void set_adjusted_probability(N_gram_ptr n_gram, double* countsOfCounts, int height, double pZero);

void prune(N_gram_ptr n_gram, double threshold);

void merge(N_gram_ptr n_gram, const N_gram* toBeMerged);

void set_probabilities_simple(N_gram_ptr n_gram,
                              void* data,
                              void (*set_probabilities_with_level)(N_gram_ptr n_gram,
                                      int level, void* data));

void set_probabilities_trained(N_gram_ptr n_gram,
                               Array_list_ptr corpus,
                               double* (*learn_parameters)(Array_list_ptr corpus, int N),
                               void (*set_probabilities_with_level)(N_gram_ptr n_gram, int level, double* parameters));

#endif //NGRAM_NGRAM_H
