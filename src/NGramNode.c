//
// Created by Olcay Taner YILDIZ on 16.09.2023.
//

#include <stdlib.h>
#include <FileUtils.h>
#include <string.h>
#include <StringUtils.h>
#include "NGramNode.h"
#include "Memory/Memory.h"

/**
 * Constructor of NGramNode
 *
 * @param symbol symbol to be kept in this node.
 */
N_gram_node_ptr create_n_gram_node(void *symbol,
                                   unsigned int (*hash_function)(const void *, int),
                                   int (*key_compare)(const void *, const void *)) {
    N_gram_node_ptr result = malloc_(sizeof(N_gram_node), "create_n_gram_node");
    if (symbol != NULL){
        result->symbol = str_copy(result->symbol, symbol);
    } else {
        result->symbol = NULL;
    }
    result->children = create_hash_map(hash_function, key_compare);
    result->count = 0;
    result->unknown = NULL;
    result->probability = 0.0;
    result->probability_of_unseen = 0.0;
    return result;
}

N_gram_node_ptr clone_n_gram_node(const N_gram_node *n_gram_node) {
    N_gram_node_ptr result = malloc_(sizeof(N_gram_node), "clone_n_gram_node");
    if (n_gram_node->symbol != NULL){
        result->symbol = str_copy(result->symbol, n_gram_node->symbol);
    } else {
        result->symbol = NULL;
    }
    result->children = create_hash_map((unsigned int (*)(const void *, int)) hash_function_string,
                                       (int (*)(const void *, const void *)) compare_string);
    Array_list_ptr list = key_list(n_gram_node->children);
    for (int i = 0; i < list->size; i++){
        void* symbol = array_list_get(list, i);
        N_gram_node_ptr child = hash_map_get(n_gram_node->children, symbol);
        hash_map_insert(result->children, symbol, clone_n_gram_node(child));
    }
    free_array_list(list, NULL);
    result->count = n_gram_node->count;
    result->unknown = NULL;
    result->probability = n_gram_node->probability;
    result->probability_of_unseen = n_gram_node->probability_of_unseen;
    return result;
}

void free_n_gram_node(N_gram_node_ptr n_gram_node) {
    free_hash_map(n_gram_node->children, (void (*)(void *)) free_n_gram_node);
    free_(n_gram_node->symbol);
    free_(n_gram_node);
}

/**
 * Constructor of NGramNode
 *
 * @param is_root_node True if this node is root node, false otherwise.
 * @param input_file         File to be read.
 */
N_gram_node_ptr create_n_gram_node2(bool is_root_node,
                                    FILE *input_file,
                                    unsigned int (*hash_function)(const void *, int),
                                    int (*key_compare)(const void *, const void *)) {
    char line[MAX_LINE_LENGTH];
    N_gram_node_ptr result = create_n_gram_node(NULL, hash_function, key_compare);
    if (!is_root_node){
        fgets(line, MAX_LINE_LENGTH, input_file);
        line[strcspn(line, "\n")] = 0;
        result->symbol = str_copy(result->symbol, left_trim(line));
    }
    fgets(line, MAX_LINE_LENGTH, input_file);
    line[strcspn(line, "\n")] = 0;
    Array_list_ptr items = str_split(left_trim(line), ' ');
    if (items->size != 4){
        printf("Error in line -> %s", line);
    }
    result->count = atoi(array_list_get(items, 0));
    result->probability = atof(array_list_get(items, 1));
    result->probability_of_unseen = atof(array_list_get(items, 2));
    int number_of_children = atoi(array_list_get(items, 3));
    if (number_of_children > 0){
        for (int i = 0; i < number_of_children; i++){
            N_gram_node_ptr child_node = create_n_gram_node2(false, input_file, hash_function, key_compare);
            hash_map_insert(result->children, child_node->symbol, child_node);
        }
    }
    free_array_list(items, free_);
    return result;
}

/**
 * Constructor of NGramNode
 *
 * @param is_root_node True if this node is root node, false otherwise.
 * @param multiple_file Multiple file structure to read the nGram.
 */
N_gram_node_ptr create_n_gram_node3(bool is_root_node, Multiple_file_ptr multiple_file,
                                    unsigned int (*hash_function)(const void *, int),
                                    int (*key_compare)(const void *, const void *)) {
    N_gram_node_ptr result = create_n_gram_node(NULL, hash_function, key_compare);
    if (!is_root_node){
        char* line = read_line(multiple_file);
        result->symbol = str_copy(result->symbol, left_trim(line));
        free_(line);
    }
    char* line = read_line(multiple_file);
    char* trimmed_line = left_trim(line);
    Array_list_ptr items = str_split(trimmed_line, ' ');
    if (items->size != 4){
        printf("Error in line -> %s", trimmed_line);
    }
    free_(line);
    result->count = atoi(array_list_get(items, 0));
    result->probability = atof(array_list_get(items, 1));
    result->probability_of_unseen = atof(array_list_get(items, 2));
    int number_of_children = atoi(array_list_get(items, 3));
    if (number_of_children > 0){
        for (int i = 0; i < number_of_children; i++){
            N_gram_node_ptr child_node = create_n_gram_node3(false, multiple_file, hash_function, key_compare);
            hash_map_insert(result->children, child_node->symbol, child_node);
        }
    }
    free_array_list(items, free_);
    return result;
}

N_gram_node_ptr
create_n_gram_node4(unsigned int (*hash_function)(const void *, int), int (*key_compare)(const void *, const void *)) {
    return create_n_gram_node(NULL, hash_function, key_compare);
}

/**
 * Gets the size of children of this node.
 * @return size of children of NGramNode this node.
 */
int size_of_n_gram_node(const N_gram_node* n_gram_node) {
    return n_gram_node->children->count;
}

/**
 * Finds maximum occurrence. If height is 0, returns the count of this node.
 * Otherwise, traverses this nodes' children recursively and returns maximum occurrence.
 * @param height height for NGram.
 * @return maximum occurrence.
 */
int maximum_occurrence_node(const N_gram_node* n_gram_node, int height) {
    int current, max = 0;
    if (height == 0){
        return n_gram_node->count;
    } else {
        Array_list_ptr children = value_list(n_gram_node->children);
        for (int i = 0; i < children->size; i++){
            N_gram_node_ptr child = array_list_get(children, i);
            current = maximum_occurrence_node(child, height - 1);
            if (current > max){
                max = current;
            }
        }
        free_array_list(children, NULL);
        return max;
    }
}

/**
 *
 * @return sum of counts of children nodes.
 */
double child_sum(const N_gram_node* n_gram_node) {
    double sum = 0;
    Array_list_ptr children = value_list(n_gram_node->children);
    for (int i = 0; i < children->size; i++){
        N_gram_node_ptr child = array_list_get(children, i);
        sum += child->count;
    }
    free_array_list(children, NULL);
    if (n_gram_node->unknown != NULL){
        sum += n_gram_node->unknown->count;
    }
    return sum;
}

/**
 * Traverses nodes and updates counts of counts for each node.
 * @param counts_of_counts counts of counts of NGrams.
 * @param height height for NGram. if height = 1, If level = 1, N-Gram is treated as UniGram, if level = 2,
 *               N-Gram is treated as Bigram, etc.
 */
void update_counts_of_counts_node(const N_gram_node* n_gram_node, int *counts_of_counts, int height) {
    if (height == 0){
        counts_of_counts[n_gram_node->count]++;
    } else {
        Array_list_ptr children = value_list(n_gram_node->children);
        for (int i = 0; i < children->size; i++){
            N_gram_node_ptr child = array_list_get(children, i);
            update_counts_of_counts_node(child, counts_of_counts, height - 1);
        }
        free_array_list(children, NULL);
    }
}

/**
 * Sets probabilities by traversing nodes and adding pseudocount for each NGram.
 * @param pseudo_count pseudocount added to each NGram.
 * @param height height for NGram. if height = 1, If level = 1, N-Gram is treated as UniGram, if level = 2,
 *               N-Gram is treated as Bigram, etc.
 * @param vocabulary_size size of vocabulary
 */
void set_node_probability_with_pseudo_count(N_gram_node_ptr n_gram_node,
                                            double pseudo_count,
                                            int height,
                                            double vocabulary_size) {
    if (height == 1){
        double sum = child_sum(n_gram_node) + pseudo_count * vocabulary_size;
        Array_list_ptr children = value_list(n_gram_node->children);
        for (int i = 0; i < children->size; i++){
            N_gram_node_ptr child = array_list_get(children, i);
            child->probability = (child->count + pseudo_count) / sum;
        }
        free_array_list(children, NULL);
        if (n_gram_node->unknown != NULL){
            n_gram_node->unknown->probability = (n_gram_node->unknown->count + pseudo_count) / sum;
        }
        n_gram_node->probability_of_unseen = pseudo_count / sum;
    } else {
        Array_list_ptr children = value_list(n_gram_node->children);
        for (int i = 0; i < children->size; i++){
            N_gram_node_ptr child = array_list_get(children, i);
            set_node_probability_with_pseudo_count(child, pseudo_count, height - 1, vocabulary_size);
        }
        free_array_list(children, NULL);
    }
}

/**
 * Sets adjusted probabilities with counts of counts of NGrams.
 * For count < 5, count is considered as ((r + 1) * N[r + 1]) / N[r]), otherwise, count is considered as it is.
 * Sum of children counts are computed. Then, probability of a child node is (1 - p_zero) * (r / sum) if r > 5
 * otherwise, r is replaced with ((r + 1) * N[r + 1]) / N[r]) and calculated the same.
 * @param N counts of counts of NGrams.
 * @param height height for NGram. if height = 1, If level = 1, N-Gram is treated as UniGram, if level = 2,
 *               N-Gram is treated as Bigram, etc.
 * @param vocabulary_size size of vocabulary.
 * @param p_zero probability of zero.
 */
void set_adjusted_probability_node(N_gram_node_ptr n_gram_node,
                                   double *N,
                                   int height,
                                   double vocabulary_size,
                                   double p_zero) {
    if (height == 1){
        double sum = 0;
        Array_list_ptr children = value_list(n_gram_node->children);
        for (int i = 0; i < children->size; i++){
            N_gram_node_ptr child = array_list_get(children, i);
            int r = child->count;
            if (r <= 5){
                double newR = ((r + 1) * N[r + 1]) / N[r];
                sum += newR;
            } else {
                sum += r;
            }
        }
        free_array_list(children, NULL);
        children = value_list(n_gram_node->children);
        for (int i = 0; i < children->size; i++){
            N_gram_node_ptr child = array_list_get(children, i);
            int r = child->count;
            if (r <= 5){
                double newR = ((r + 1) * N[r + 1]) / N[r];
                child->probability = (1 - p_zero) * (newR / sum);
            } else {
                child->probability = (1 - p_zero) * (r / sum);
            }
        }
        n_gram_node->probability_of_unseen = p_zero / (vocabulary_size - children->size);
        free_array_list(children, NULL);
    } else {
        Array_list_ptr children = value_list(n_gram_node->children);
        for (int i = 0; i < children->size; i++){
            N_gram_node_ptr child = array_list_get(children, i);
            set_adjusted_probability_node(child, N, height - 1, vocabulary_size, p_zero);
        }
        free_array_list(children, NULL);
    }
}

/**
 * Adds NGram given as array of symbols to the node as a child.
 * @param s array of symbols
 * @param index  start index of NGram
 * @param height height for NGram. if height = 1, If level = 1, N-Gram is treated as UniGram, if level = 2,
 *               N-Gram is treated as Bigram, etc.
 */
void add_n_gram_to_node(N_gram_node_ptr n_gram_node,
                        Array_list_ptr s,
                        int index,
                        int height,
                        int sentence_count) {
    N_gram_node_ptr child;
    if (height == 0){
        return;
    }
    void* _symbol = array_list_get(s, index);
    if (size_of_n_gram_node(n_gram_node) != 0 && hash_map_contains(n_gram_node->children, _symbol)){
        child = hash_map_get(n_gram_node->children, _symbol);
    } else {
        child = create_n_gram_node(_symbol, n_gram_node->children->hash_function, n_gram_node->children->key_compare);
        hash_map_insert(n_gram_node->children, _symbol, child);
    }
    child->count += sentence_count;
    add_n_gram_to_node(child, s, index + 1, height - 1, sentence_count);
}

/**
 * Gets unigram probability of given symbol.
 * @param w1 unigram.
 * @return unigram probability of given symbol.
 */
double get_uni_gram_probability_node(const N_gram_node* n_gram_node, const void *w1) {
    if (hash_map_contains(n_gram_node->children, w1)){
        N_gram_node_ptr child = hash_map_get(n_gram_node->children, w1);
        return child->probability;
    } else {
        if (n_gram_node->unknown != NULL){
            return n_gram_node->unknown->probability;
        }
        return n_gram_node->probability_of_unseen;
    }
}

/**
 * Gets bigram probability of given symbols w1 and w2
 * @param w1 first gram of bigram.
 * @param w2 second gram of bigram.
 * @return probability of given bigram or -1 if unseen case
 */
double get_bi_gram_probability_node(const N_gram_node* n_gram_node, const void *w1, const void *w2) {
    if (hash_map_contains(n_gram_node->children, w1)){
        N_gram_node_ptr child = hash_map_get(n_gram_node->children, w1);
        return get_uni_gram_probability_node(child, w2);
    } else {
        if (n_gram_node->unknown != NULL){
            return get_uni_gram_probability_node(n_gram_node->unknown, w2);
        }
        return -1;
    }
}

double get_tri_gram_probability_node(const N_gram_node* n_gram_node, const void *w1, const void *w2, const void *w3) {
    if (hash_map_contains(n_gram_node->children, w1)){
        N_gram_node_ptr child = hash_map_get(n_gram_node->children, w1);
        return get_bi_gram_probability_node(child, w2, w3);
    } else {
        if (n_gram_node->unknown != NULL){
            return get_bi_gram_probability_node(n_gram_node->unknown, w2, w3);
        }
        return -1;
    }
}

/**
 * Counts words recursively given height and wordCounter.
 * @param wordCounter word counter keeping symbols and their counts.
 * @param height height for NGram. if height = 1, If level = 1, N-Gram is treated as UniGram, if level = 2,
 *               N-Gram is treated as Bigram, etc.
 */
void count_words(const N_gram_node* n_gram_node, Counter_hash_map_ptr word_counter, int height) {
    if (height == 0){
        put_counter_hash_map_n_times(word_counter, n_gram_node->symbol, n_gram_node->count);
    } else {
        Array_list_ptr children = value_list(n_gram_node->children);
        for (int i = 0; i < children->size; i++){
            N_gram_node_ptr child = array_list_get(children, i);
            count_words(child, word_counter, height - 1);
        }
        free_array_list(children, NULL);
    }
}

/**
 * Replace words not in given dictionary.
 * Deletes unknown words from children nodes and adds them to {@link NGramNode#unknown} unknown node as children recursively.
 *
 * @param dictionary dictionary of known words.
 */
void replace_unknown_words_node(N_gram_node_ptr n_gram_node, Hash_set_ptr dictionary) {
    if (size_of_n_gram_node(n_gram_node) != 0){
        Array_list_ptr childList = create_array_list();
        Array_list_ptr symbol_list = key_list(n_gram_node->children);
        for (int i = 0; i < symbol_list->size; i++){
            void* symbol = array_list_get(symbol_list, i);
            if (!hash_set_contains(dictionary, symbol)){
                N_gram_node_ptr child = hash_map_get(n_gram_node->children, symbol);
                array_list_add(childList, child);
            }
        }
        if (childList->size > 0){
            n_gram_node->unknown = create_n_gram_node4(n_gram_node->children->hash_function, n_gram_node->children->key_compare);
            int sum = 0;
            for (int i = 0; i < childList->size; i++){
                N_gram_node_ptr child = array_list_get(childList, i);
                if (size_of_n_gram_node(child) != 0){
                    hash_map_merge(n_gram_node->unknown->children,
                                   child->children,
                                   (void *(*)(void *)) clone_string,
                                   (void *(*)(void *)) clone_n_gram_node);
                }
                sum += child->count;
                hash_map_remove(n_gram_node->children, child->symbol, NULL);
            }
            n_gram_node->unknown->count = sum;
            replace_unknown_words_node(n_gram_node->unknown, dictionary);
        }
        Array_list_ptr children = value_list(n_gram_node->children);
        for (int i = 0; i < children->size; i++){
            N_gram_node_ptr child = array_list_get(children, i);
            replace_unknown_words_node(child, dictionary);
        }
        free_array_list(childList, NULL);
        free_array_list(children, NULL);
    }
}

/**
 * Gets count of symbol given array of symbols and index of symbol in this array.
 * @param s array of symbols
 * @param index index of symbol whose count is returned
 * @return count of the symbol.
 */
int get_count_node(const N_gram_node* n_gram_node, void **s, int length, int index) {
    if (index < length){
        if (hash_map_contains(n_gram_node->children, s[index])){
            N_gram_node_ptr child = hash_map_get(n_gram_node->children, s[index]);
            return get_count_node(child, s, length, index + 1);
        } else {
            return 0;
        }
    } else {
        return n_gram_node->count;
    }
}

void prune_node(N_gram_node_ptr n_gram_node, double threshold, int N) {
    if (N == 0){
        void* maxElement = NULL;
        Array_list_ptr toBeDeleted = create_array_list();
        Array_list_ptr children = key_list(n_gram_node->children);
        for (int i = 0; i < children->size; i++){
            void* symbol = array_list_get(children, i);
            N_gram_node_ptr node = hash_map_get(n_gram_node->children, symbol);
            if (node->count / (n_gram_node->count + 0.0) < threshold){
                array_list_add(toBeDeleted, symbol);
            }
            if (maxElement == NULL || node->count > ((N_gram_node_ptr) hash_map_get(n_gram_node->children, maxElement))->count){
                maxElement = symbol;
            }
        }
        for (int i = 0; i < toBeDeleted->size; i++){
            void* symbol1 = array_list_get(toBeDeleted, i);
            if (symbol1 != maxElement){
                N_gram_node_ptr deleted_node = hash_map_get(n_gram_node->children, symbol1);
                hash_map_remove(n_gram_node->children, symbol1, free_);
                free_n_gram_node(deleted_node);
            }
        }
        free_array_list(children, NULL);
        free_array_list(toBeDeleted, NULL);
    } else {
        Array_list_ptr children = value_list(n_gram_node->children);
        for (int i = 0; i < children->size; i++){
            N_gram_node_ptr node = array_list_get(children, i);
            prune_node(node, threshold, N - 1);
        }
        free_array_list(children, NULL);
    }
}

void merge_node(N_gram_node_ptr n_gram_node, const N_gram_node* toBeMerged) {
    Array_list_ptr children = key_list(n_gram_node->children);
    for (int i = 0; i < children->size; i++){
        void* symbol = array_list_get(children, i);
        if (hash_map_contains(toBeMerged->children, symbol)){
            N_gram_node_ptr child = hash_map_get(n_gram_node->children, symbol);
            merge_node(child, (N_gram_node_ptr) hash_map_get(toBeMerged->children, symbol));
        }
    }
    free_array_list(children, NULL);
    children = key_list(toBeMerged->children);
    for (int i = 0; i < children->size; i++){
        void* symbol = array_list_get(children, i);
        if (!hash_map_contains(n_gram_node->children, symbol)){
            hash_map_insert(n_gram_node->children, symbol, clone_n_gram_node(hash_map_get(toBeMerged->children, symbol)));
        }
    }
    free_array_list(children, NULL);
    n_gram_node->count += toBeMerged->count;
}

/**
 * Generates next string for given list of symbol and index
 * @param s list of symbol
 * @param index index index of generated string
 * @return generated string.
 */
void *generate_next_string(const N_gram_node* n_gram_node, Array_list_ptr s, int index) {
    double sum = 0.0, prob;
    if (index == s->size){
        prob = random() / (RAND_MAX + 1.0);
        Array_list_ptr children = key_list(n_gram_node->children);
        for (int i = 0; i < children->size; i++){
            void* symbol = array_list_get(children, i);
            N_gram_node_ptr node = hash_map_get(n_gram_node->children, symbol);
            if (prob < node->probability + sum){
                return node->symbol;
            } else {
                sum += node->probability;
            }
        }
    } else {
        void* symbol = array_list_get(s, index);
        N_gram_node_ptr node = hash_map_get(n_gram_node->children, symbol);
        return generate_next_string(node, s, index + 1);
    }
    return NULL;
}
