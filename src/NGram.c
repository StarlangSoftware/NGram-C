//
// Created by Olcay Taner YILDIZ on 20.09.2023.
//

#include <FileUtils.h>
#include <stdarg.h>
#include <math.h>
#include <StringUtils.h>
#include <Memory/Memory.h>
#include "NGram.h"

/**
 * Constructor of NGram class which takes a {@link vector<vector<Symbol>>} corpus and Integer size of ngram as input.
 * It adds all sentences of corpus as ngrams.
 *
 * @param corpus {@link vector<vector<Symbol>>} list of sentences whose ngrams are added.
 * @param N size of ngram.
 */
N_gram_ptr create_n_gram(Array_list_ptr corpus,
                         int N,
                         unsigned int (*hash_function)(const void *, int),
                         int (*key_compare)(const void *, const void *)) {
    N_gram_ptr result = create_n_gram2(N, hash_function, key_compare);
    for (int i = 0; i < corpus->size; i++){
        Array_list_ptr data = array_list_get(corpus, i);
        add_n_gram_sentence(result, data, 1);
    }
    return result;
}

void free_n_gram(N_gram_ptr n_gram) {
    free_n_gram_node(n_gram->root_node);
    free_(n_gram->probability_of_unseen);
    free_hash_set(n_gram->vocabulary, free_);
    free_(n_gram);
}

/**
 * Constructor of NGram class which takes Integer size of ngram.
 *
 * @param N size of ngram.
 */
N_gram_ptr create_n_gram2(int N, unsigned int (*hash_function)(const void *, int),
                          int (*key_compare)(const void *, const void *)) {
    N_gram_ptr result = malloc_(sizeof(N_gram), "create_n_gram2_1");
    result->N = N;
    result->probability_of_unseen = calloc_(N, sizeof(double), "create_n_gram2_2");
    result->vocabulary = create_hash_set(hash_function, key_compare);
    result->root_node = create_n_gram_node(NULL, hash_function, key_compare);
    return result;
}

/**
 * Adds {@link Symbol*} given array of symbols to {@link unordered_set} the vocabulary and to NGramNode the rootNode
 *
 * @param symbols {@link Symbol*} ngram added.
 * @param size size of symbols.
 */
void add_n_gram(N_gram_ptr n_gram, Array_list_ptr symbols, int size) {
    for (int i = 0; i < size; i++){
        hash_set_insert(n_gram->vocabulary, array_list_get(symbols, i));
    }
    add_n_gram_to_node(n_gram->root_node, symbols, 0, n_gram->N, 1);
}

/**
 * Adds given sentence count times to {@link unordered_set} the vocabulary and create and add ngrams of the sentence to NGramNode the rootNode
 *
 * @param symbols {@link Symbol*} sentence whose ngrams are added.
 * @param size size of symbols.
 * @param count Number of times this sentence is added.
 */
void add_n_gram_sentence(N_gram_ptr n_gram,
                         Array_list_ptr symbols,
                         int sentence_count) {
    for (int i = 0; i < symbols->size; i++){
        if (!hash_set_contains(n_gram->vocabulary, array_list_get(symbols, i))){
            char* word = str_copy(word, array_list_get(symbols, i));
            hash_set_insert(n_gram->vocabulary, word);
        }
    }
    for (int j = 0; j < symbols->size - n_gram->N + 1; j++){
        add_n_gram_to_node(n_gram->root_node, symbols, j, n_gram->N, sentence_count);
    }
}

void load_n_gram(N_gram_ptr n_gram, FILE *input_file) {
    int vocabulary_size;
    char line[MAX_LINE_LENGTH];
    fscanf(input_file, "%d%lf%lf", &(n_gram->N), &(n_gram->lambda1), &(n_gram->lambda2));
    n_gram->probability_of_unseen = malloc_(n_gram->N * sizeof(double), "load_n_gram");
    for (int i = 0; i < n_gram->N; i++){
        fscanf(input_file, "%lf", &(n_gram->probability_of_unseen[i]));
    }
    fscanf(input_file, "%d", &vocabulary_size);
    for (int i = 0; i < vocabulary_size; i++){
        fscanf(input_file, "%s\n", line);
        char* word = str_copy(word, line);
        hash_set_insert(n_gram->vocabulary, word);
    }
}

N_gram_ptr create_n_gram3(char *file_name, unsigned int (*hash_function)(const void *, int),
                          int (*key_compare)(const void *, const void *)) {
    FILE* input_file = fopen(file_name, "r");
    N_gram_ptr result = malloc_(sizeof(N_gram), "create_n_gram3");
    result->vocabulary = create_hash_set(hash_function, key_compare);
    load_n_gram(result, input_file);
    result->root_node = create_n_gram_node2(true, input_file, hash_function, key_compare);
    fclose(input_file);
    return result;
}

N_gram_ptr create_n_gram4(FILE *input_file, unsigned int (*hash_function)(const void *, int),
                          int (*key_compare)(const void *, const void *)) {
    N_gram_ptr result = malloc_(sizeof(N_gram), "create_n_gram4");
    result->vocabulary = create_hash_set(hash_function, key_compare);
    load_n_gram(result, input_file);
    result->root_node = create_n_gram_node2(true, input_file, hash_function, key_compare);
    return result;
}

N_gram_ptr create_n_gram5(unsigned int (*hash_function)(const void *, int),
                          int (*key_compare)(const void *, const void *),
                          int num,
                          ...) {
    N_gram_ptr result = malloc_(sizeof(N_gram), "create_n_gram5");
    result->vocabulary = create_hash_set(hash_function, key_compare);
    va_list valist;
    va_start(valist, num);
    Multiple_file_ptr multiple_file = create_multiple_file(num, valist);
    va_end(valist);
    load_n_gram(result, multiple_file->reader);
    result->root_node = create_n_gram_node3(true, multiple_file, hash_function, key_compare);
    multiple_file_close(multiple_file);
    free_multiple_file(multiple_file);
    return result;
}

/**
 * @return vocabulary size.
 */
int vocabulary_size(const N_gram *n_gram) {
    return n_gram->vocabulary->hash_map->count;
}

/**
 * Sets lambda, interpolation ratio, for bigram and unigram probabilities.
 *
 * ie. _lambda1 * bigramProbability + (1 - _lambda1) * unigramProbability
 *
 * @param _lambda1 interpolation ratio for bigram probabilities
 */
void set_lambda1(N_gram_ptr n_gram, double lambda1) {
    if (n_gram->N == 2){
        n_gram->interpolated = true;
        n_gram->lambda1 = lambda1;
    }
}

/**
 * Sets lambdas, interpolation ratios, for trigram, bigram and unigram probabilities.
 * ie. _lambda1 * trigramProbability + _lambda2 * bigramProbability  + (1 - _lambda1 - _lambda2) * unigramProbability
 *
 * @param _lambda1 interpolation ratio for trigram probabilities
 * @param _lambda2 interpolation ratio for bigram probabilities
 */
void set_lambda2(N_gram_ptr n_gram, double lambda1, double lambda2) {
    if (n_gram->N == 2){
        n_gram->interpolated = true;
        n_gram->lambda1 = lambda1;
        n_gram->lambda2 = lambda2;
    }
}

/**
 * Sets probabilities by adding pseudocounts given height and pseudocount.
 * @param pseudoCount pseudocount added to all N-Grams.
 * @param height  height for N-Gram. If height= 1, N-Gram is treated as UniGram, if height = 2,
 *                N-Gram is treated as Bigram, etc.
 */
void set_probability_with_pseudo_count(N_gram_ptr n_gram,
                                       double pseudo_count,
                                       int height) {
    double size_of_vocabulary;
    if (pseudo_count != 0){
        size_of_vocabulary = vocabulary_size(n_gram) + 1;
    } else {
        size_of_vocabulary = vocabulary_size(n_gram);
    }
    set_node_probability_with_pseudo_count(n_gram->root_node, pseudo_count, height, size_of_vocabulary);
    if (pseudo_count != 0){
        n_gram->probability_of_unseen[height - 1] = 1.0 / size_of_vocabulary;
    } else {
        n_gram->probability_of_unseen[height - 1] = 0.0;
    }
}

/**
 * Replaces words not in {@link unordered_set} given dictionary.
 *
 * @param dictionary dictionary of known words.
 */
void replace_unknown_words(N_gram_ptr n_gram, Hash_set_ptr dictionary) {
    replace_unknown_words_node(n_gram->root_node, dictionary);
}

/**
 * Constructs a dictionary of nonrare words with given N-Gram level and probability threshold.
 *
 * @param level Level for counting words. Counts for different levels of the N-Gram can be set. If level = 1, N-Gram is
 * treated as UniGram, if level = 2, N-Gram is treated as Bigram, etc.
 * @param probability probability threshold for nonrare words.
 * @return {@link unordered_set} nonrare words.
 */
Hash_set_ptr construct_dictionary_with_non_rare_words(N_gram_ptr n_gram, int level, double probability) {
    Hash_set_ptr result = create_hash_set(n_gram->vocabulary->hash_map->hash_function, n_gram->vocabulary->hash_map->key_compare);
    Counter_hash_map_ptr word_counter = create_counter_hash_map(n_gram->vocabulary->hash_map->hash_function, n_gram->vocabulary->hash_map->key_compare);
    count_words(n_gram->root_node, word_counter, level);
    int sum = sum_of_counts_counter_hash_map(word_counter);
    Array_list_ptr list = key_value_list(word_counter->map);
    for (int i = 0; i < list->size; i++){
        Hash_node_ptr node = array_list_get(list, i);
        if (*((int*)node->value) / (sum + 0.0) > probability){
            hash_set_insert(result, node->value);
        }
    }
    return result;
}

/**
 * Calculates counts of counts of NGrams.
 * @param height  height for NGram. If height = 1, N-Gram is treated as UniGram, if height = 2,
 *                N-Gram is treated as Bigram, etc.
 * @return counts of counts of NGrams.
 */
int *calculate_counts_of_counts(const N_gram *n_gram, int height, int* size) {
    *size = maximum_occurrence(n_gram, height) + 2;
    int* counts_of_counts = calloc_(*size, sizeof(int), "calculate_counts_of_counts");
    update_counts_of_counts(n_gram, counts_of_counts, height);
    return counts_of_counts;
}

/**
 * Find maximum occurrence in given height.
 * @param height height for occurrences. If height = 1, N-Gram is treated as UniGram, if height = 2,
 *               N-Gram is treated as Bigram, etc.
 * @return maximum occurrence in given height.
 */
int maximum_occurrence(const N_gram* n_gram, int height) {
    return maximum_occurrence_node(n_gram->root_node, height);
}

/**
 * Update counts of counts of N-Grams with given counts of counts and given height.
 * @param countsOfCounts updated counts of counts.
 * @param height  height for NGram. If height = 1, N-Gram is treated as UniGram, if height = 2,
 *                N-Gram is treated as Bigram, etc.
 */
void update_counts_of_counts(const N_gram* n_gram, int *counts_of_counts, int height) {
    update_counts_of_counts_node(n_gram->root_node, counts_of_counts, height);
}

/**
 * Calculates the perplexity of given corpus depending on N-Gram model (unigram, bigram, trigram, etc.)
 *
 * @param corpus corpus whose perplexity is calculated.
 * @return perplexity of given corpus
 */
double get_perplexity(const N_gram* n_gram, const Array_list* corpus) {
    switch (n_gram->N){
        case 1:
            return get_uni_gram_perplexity(n_gram, corpus);
        case 2:
            return get_bi_gram_perplexity(n_gram, corpus);
        case 3:
            return get_tri_gram_perplexity(n_gram, corpus);
        default:
            return 0;
    }
}

/**
 * Calculates unigram perplexity of given corpus. First sums negative log likelihoods of all unigrams in corpus.
 * Then returns exp of average negative log likelihood.
 *
 * @param corpus corpus whose unigram perplexity is calculated.
 *
 * @return unigram perplexity of corpus.
 */
double get_uni_gram_perplexity(const N_gram * n_gram, const Array_list* corpus) {
    double sum = 0;
    int count = 0;
    for (int i = 0; i < corpus->size; i++){
        Array_list_ptr sentence = array_list_get(corpus, i);
        for (int j = 0; j < sentence->size; j++){
            double p = get_probability(n_gram, 1, array_list_get(sentence, j));
            sum -= log(p);
            count++;
        }
    }
    return exp(sum / count);
}

/**
 * Calculates bigram perplexity of given corpus. First sums negative log likelihoods of all bigrams in corpus.
 * Then returns exp of average negative log likelihood.
 *
 * @param corpus corpus whose bigram perplexity is calculated.
 *
 * @return bigram perplexity of given corpus.
 */
double get_bi_gram_perplexity(const N_gram * n_gram, const Array_list* corpus) {
    double sum = 0;
    int count = 0;
    for (int i = 0; i < corpus->size; i++){
        Array_list_ptr sentence = array_list_get(corpus, i);
        for (int j = 0; j < sentence->size - 1; j++){
            double p = get_probability(n_gram, 2, array_list_get(sentence, j), array_list_get(sentence, j + 1));
            sum -= log(p);
            count++;
        }
    }
    return exp(sum / count);
}

/**
 * Calculates trigram perplexity of given corpus. First sums negative log likelihoods of all trigrams in corpus.
 * Then returns exp of average negative log likelihood.
 *
 * @param corpus corpus whose trigram perplexity is calculated.
 * @return trigram perplexity of given corpus.
 */
double get_tri_gram_perplexity(const N_gram* n_gram, const Array_list* corpus) {
    double sum = 0;
    int count = 0;
    for (int i = 0; i < corpus->size; i++){
        Array_list_ptr sentence = array_list_get(corpus, i);
        for (int j = 0; j < sentence->size - 2; j++){
            double p = get_probability(n_gram, 3, array_list_get(sentence, j), array_list_get(sentence, j + 1), array_list_get(sentence, j + 2));
            sum -= log(p);
            count++;
        }
    }
    return exp(sum / count);
}

/**
 * Gets probability of sequence of symbols depending on N in N-Gram. If N is 1, returns unigram probability.
 * If N is 2, if interpolated is true, then returns interpolated bigram and unigram probability, otherwise returns only bigram probability.
 * If N is 3, if interpolated is true, then returns interpolated trigram, bigram and unigram probability, otherwise returns only trigram probability.
 * @param symbols sequence of symbol.
 * @return probability of given sequence.
 */
double get_probability(const N_gram* n_gram, int num, ...) {
    va_list valist;
    char *w1, *w2, *w3;
    va_start(valist, num);
    switch (n_gram->N){
        case 1:
            w1 = va_arg(valist, char*);
            return get_uni_gram_probability(n_gram, w1);
        case 2:
            w1 = va_arg(valist, char*);
            w2 = va_arg(valist, char*);
            if (num == 1){
                return get_uni_gram_probability(n_gram, w1);
            }
            if (n_gram->interpolated){
                return n_gram->lambda1 * get_bi_gram_probability(n_gram, w1, w2) + (1 - n_gram->lambda1) * get_uni_gram_probability(n_gram, w2);
            } else {
                return get_bi_gram_probability(n_gram, w1, w2);
            }
        case 3:
            w1 = va_arg(valist, char*);
            w2 = va_arg(valist, char*);
            w3 = va_arg(valist, char*);
            if (num == 1){
                return get_uni_gram_probability(n_gram, w1);
            } else {
                if (num == 2){
                    return get_bi_gram_probability(n_gram, w1, w2);
                }
            }
            if (n_gram->interpolated){
                return n_gram->lambda1 * get_tri_gram_probability(n_gram, w1, w2, w3) + n_gram->lambda2 * get_bi_gram_probability(n_gram, w2, w3) + (1 - n_gram->lambda1 - n_gram->lambda2) * get_uni_gram_probability(n_gram, w3);
            } else {
                return get_tri_gram_probability(n_gram, w1, w2, w3);
            }
    }
    va_end(valist);
    return 0.0;
}

/**
 * Gets unigram probability of given symbol.
 * @param w1 a unigram symbol.
 * @return probability of given unigram.
 */
double get_uni_gram_probability(const N_gram* n_gram, const void *w1) {
    return get_uni_gram_probability_node(n_gram->root_node, w1);
}

/**
 * Gets bigram probability of given symbols.
 * @param w1 first gram of bigram
 * @param w2 second gram of bigram
 * @return probability of bigram formed by w1 and w2.
 */
double get_bi_gram_probability(const N_gram* n_gram, const void *w1, const void *w2) {
    double result = get_bi_gram_probability_node(n_gram->root_node, w1, w2);
    if (result != -1){
        return result;
    }
    return n_gram->probability_of_unseen[1];
}

/**
 * Gets trigram probability of given symbols.
 * @param w1 first gram of trigram
 * @param w2 second gram of trigram
 * @param w3 third gram of trigram
 * @return probability of bigram formed by w1, w2, w3.
 */
double get_tri_gram_probability(const N_gram* n_gram, const void *w1, const void *w2, const void *w3) {
    double result = get_tri_gram_probability_node(n_gram->root_node, w1, w2, w3);
    if (result != -1){
        return result;
    }
    return n_gram->probability_of_unseen[2];
}

/**
 * Gets count of given sequence of symbol.
 * @param symbols sequence of symbol.
 * @return count of symbols.
 */
int get_count(const N_gram * n_gram, void **symbols, int length) {
    return get_count_node(n_gram->root_node, symbols, length, 0);
}

/**
 * Sets probability with given counts of counts and pZero.
 * @param countsOfCounts counts of counts of NGrams.
 * @param height  height for NGram. If height = 1, N-Gram is treated as UniGram, if height = 2,
 *                N-Gram is treated as Bigram, etc.
 * @param pZero probability of zero.
 */
void set_adjusted_probability(N_gram_ptr n_gram, double *countsOfCounts, int height, double pZero) {
    set_adjusted_probability_node(n_gram->root_node, countsOfCounts, height, vocabulary_size(n_gram) + 1, pZero);
    n_gram->probability_of_unseen[height - 1] = 1.0 / (vocabulary_size(n_gram) + 1);
}

void prune(N_gram_ptr n_gram, double threshold) {
    if (threshold > 0.0 && threshold <= 1.0){
        prune_node(n_gram->root_node, threshold, n_gram->N - 1);
    }
}

void merge(N_gram_ptr n_gram, const N_gram* toBeMerged) {
    if (n_gram->N != toBeMerged->N){
        return;
    }
    hash_set_merge(n_gram->vocabulary, toBeMerged->vocabulary, NULL);
    merge_node(n_gram->root_node, toBeMerged->root_node);
}

void set_probabilities_simple(N_gram_ptr n_gram, void* data, void (*set_probabilities_with_level)(N_gram_ptr, int, void*)) {
    set_probabilities_with_level(n_gram, n_gram->N, data);
}

void set_probabilities_trained(N_gram_ptr n_gram,
                               Array_list_ptr corpus,
                               double* (*learn_parameters)(Array_list_ptr, int),
                               void (*set_probabilities_with_level)(N_gram_ptr, int, double*)) {
    double* parameters = learn_parameters(corpus, n_gram->N);
    set_probabilities_with_level(n_gram, n_gram->N, parameters);
    free_(parameters);
}

N_gram_ptr create_string_n_gram(Array_list_ptr corpus, int N) {
    return create_n_gram(corpus, N,
                         (unsigned int (*)(const void *, int)) hash_function_string,
                         (int (*)(const void *, const void *)) compare_string);
}

N_gram_ptr create_string_n_gram2(int N) {
    return create_n_gram2(N, (unsigned int (*)(const void *, int)) hash_function_string,
                          (int (*)(const void *, const void *)) compare_string);
}

N_gram_ptr create_string_n_gram3(char *file_name) {
    return create_n_gram3(file_name, (unsigned int (*)(const void *, int)) hash_function_string,
                          (int (*)(const void *, const void *)) compare_string);
}

N_gram_ptr create_string_n_gram4(FILE *input_file) {
    return create_n_gram4(input_file, (unsigned int (*)(const void *, int)) hash_function_string,
                          (int (*)(const void *, const void *)) compare_string);
}

N_gram_ptr create_string_n_gram5(int num, ...) {
    N_gram_ptr result = malloc_(sizeof(N_gram), "create_string_n_gram5");
    result->vocabulary = create_hash_set((unsigned int (*)(const void *, int)) hash_function_string, (int (*)(const void *, const void *)) compare_string);
    va_list valist;
    va_start(valist, num);
    Multiple_file_ptr multiple_file = create_multiple_file2(num, valist);
    va_end(valist);
    load_n_gram(result, multiple_file->reader);
    result->root_node = create_n_gram_node3(true, multiple_file, (unsigned int (*)(const void *, int)) hash_function_string, (int (*)(const void *, const void *)) compare_string);
    multiple_file_close(multiple_file);
    free_multiple_file(multiple_file);
    return result;
}
