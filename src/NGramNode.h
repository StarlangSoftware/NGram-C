//
// Created by Olcay Taner YILDIZ on 16.09.2023.
//

#ifndef NGRAM_NGRAMNODE_H
#define NGRAM_NGRAMNODE_H

#include <HashMap/HashMap.h>
#include <HashMap/HashSet.h>
#include <stdio.h>
#include <CounterHashMap.h>
#include "MultipleFile.h"

struct n_gram_node{
    Hash_map_ptr children;
    void* symbol;
    int count;
    double probability;
    double probability_of_unseen;
    struct n_gram_node* unknown;
};

typedef struct n_gram_node N_gram_node;

typedef N_gram_node *N_gram_node_ptr;

N_gram_node_ptr create_n_gram_node(void* symbol,
                                   unsigned int (*hash_function)(const void *, int),
                                   int (*key_compare)(const void *, const void *));

N_gram_node_ptr create_n_gram_node2(bool is_root_node,
                                    FILE* input_file,
                                    unsigned int (*hash_function)(const void *, int),
                                    int (*key_compare)(const void *, const void *));

N_gram_node_ptr create_n_gram_node3(bool is_root_node,
                                    Multiple_file_ptr multiple_file,
                                    unsigned int (*hash_function)(const void *, int),
                                    int (*key_compare)(const void *, const void *));

N_gram_node_ptr create_n_gram_node4(unsigned int (*hash_function)(const void *, int),
                                   int (*key_compare)(const void *, const void *));

void free_n_gram_node(N_gram_node_ptr n_gram_node);

int size_of_n_gram_node(const N_gram_node* n_gram_node);

int maximum_occurrence_node(const N_gram_node* n_gram_node, int height);

double child_sum(const N_gram_node* n_gram_node);

void update_counts_of_counts_node(const N_gram_node* n_gram_node, int* counts_of_counts, int height);

void set_node_probability_with_pseudo_count(N_gram_node_ptr n_gram_node,
                                            double pseudo_count,
                                            int height,
                                            double vocabulary_size);

void set_adjusted_probability_node(N_gram_node_ptr n_gram_node,
                                   double *N,
                                   int height,
                                   double vocabulary_size,
                                   double p_zero);

void add_n_gram_to_node(N_gram_node_ptr n_gram_node,
                        Array_list_ptr s,
                        int index,
                        int height,
                        int sentence_count);

double get_uni_gram_probability_node(const N_gram_node* n_gram_node, const void* w1);

double get_bi_gram_probability_node(const N_gram_node* n_gram_node, const void* w1, const void* w2);

double get_tri_gram_probability_node(const N_gram_node* n_gram_node, const void* w1, const void* w2, const void* w3);

void count_words(const N_gram_node* n_gram_node, Counter_hash_map_ptr word_counter, int height);

void replace_unknown_words_node(N_gram_node_ptr n_gram_node, Hash_set_ptr dictionary);

int get_count_node(const N_gram_node* n_gram_node, void** s, int length, int index);

void prune_node(N_gram_node_ptr n_gram_node, double threshold, int N);

void merge_node(N_gram_node_ptr n_gram_node, const N_gram_node* toBeMerged);

void* generate_next_string(const N_gram_node* n_gram_node, Array_list_ptr s, int index);

N_gram_node_ptr clone_n_gram_node(const N_gram_node* n_gram_node);

#endif //NGRAM_NGRAMNODE_H
