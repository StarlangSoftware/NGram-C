//
// Created by Olcay Taner YILDIZ on 24.09.2023.
//

#include <ArrayList.h>
#include <Memory/Memory.h>
#include "ReadCorpus.h"
#include "../src/LaplaceSmoothing.h"

void test_perplexity_simple(){
    Array_list_ptr simple_corpus = create_simple_corpus();
    N_gram_ptr simple_uni_gram = create_string_n_gram(simple_corpus, 1);
    N_gram_ptr simple_bi_gram = create_string_n_gram(simple_corpus, 2);
    N_gram_ptr simple_tri_gram = create_string_n_gram(simple_corpus, 3);
    double delta[] = {1.0};
    set_probabilities_simple(simple_uni_gram, delta, set_probabilities_with_level_laplace_smoothing);
    set_probabilities_simple(simple_bi_gram, delta, set_probabilities_with_level_laplace_smoothing);
    set_probabilities_simple(simple_tri_gram, delta, set_probabilities_with_level_laplace_smoothing);
    perplexity_check(simple_uni_gram, simple_corpus, 12.809502);
    perplexity_check(simple_bi_gram, simple_corpus, 6.914532);
    perplexity_check(simple_tri_gram, simple_corpus, 7.694528);
    free_n_gram(simple_uni_gram);
    free_n_gram(simple_bi_gram);
    free_n_gram(simple_tri_gram);
    free_2d_array_list(simple_corpus, NULL);
}

void test_calculate_probabilities_simple(){
    Array_list_ptr simple_corpus = create_simple_corpus();
    N_gram_ptr simple_uni_gram = create_string_n_gram(simple_corpus, 1);
    N_gram_ptr simple_bi_gram = create_string_n_gram(simple_corpus, 2);
    N_gram_ptr simple_tri_gram = create_string_n_gram(simple_corpus, 3);
    double delta[] = {1.0};
    set_probabilities_simple(simple_uni_gram, delta, set_probabilities_with_level_laplace_smoothing);
    set_probabilities_simple(simple_bi_gram, delta, set_probabilities_with_level_laplace_smoothing);
    set_probabilities_simple(simple_tri_gram, delta, set_probabilities_with_level_laplace_smoothing);
    if (get_probability(simple_uni_gram, 1, "<s>") != (5 + 1.0) / (35 + vocabulary_size(simple_uni_gram) + 1)){
        printf("Error in probability 1\n");
    }
    if (get_probability(simple_uni_gram, 1, "kitabı") != (1 + 1.0) / (35 + vocabulary_size(simple_uni_gram) + 1)){
        printf("Error in probability 2\n");
    }
    if (get_probability(simple_bi_gram, 2, "<s>", "ali") != (4 + 1.0) / (5 + vocabulary_size(simple_bi_gram) + 1)){
        printf("Error in probability 3\n");
    }
    if (get_probability(simple_bi_gram, 2, "at", "mehmet") != (2 + 1.0) / (4 + vocabulary_size(simple_bi_gram) + 1)){
        printf("Error in probability 4\n");
    }
    if (get_probability(simple_tri_gram, 3, "<s>", "ali", "top") != (1 + 1) / (4.0 + vocabulary_size(simple_tri_gram) + 1) ){
        printf("Error in probability 5\n");
    }
    if (get_probability(simple_tri_gram, 3, "ali", "topu", "at") != (2 + 1) / (3.0 + vocabulary_size(simple_tri_gram) + 1)){
        printf("Error in probability 6\n");
    }
    free_n_gram(simple_uni_gram);
    free_n_gram(simple_bi_gram);
    free_n_gram(simple_tri_gram);
    free_2d_array_list(simple_corpus, NULL);
}

int main(){
    test_perplexity_simple();
    test_calculate_probabilities_simple();
}