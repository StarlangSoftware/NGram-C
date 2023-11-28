//
// Created by Olcay Taner YILDIZ on 24.09.2023.
//

#include <ArrayList.h>
#include <math.h>
#include <Memory/Memory.h>
#include "ReadCorpus.h"
#include "../src/GoodTuringSmoothing.h"

void test_perplexity_simple(){
    Array_list_ptr simple_corpus = create_simple_corpus();
    N_gram_ptr simple_uni_gram = create_string_n_gram(simple_corpus, 1);
    N_gram_ptr simple_bi_gram = create_string_n_gram(simple_corpus, 2);
    N_gram_ptr simple_tri_gram = create_string_n_gram(simple_corpus, 3);
    set_probabilities_simple(simple_uni_gram, NULL, set_probabilities_with_level_good_turing_smoothing);
    set_probabilities_simple(simple_bi_gram, NULL, set_probabilities_with_level_good_turing_smoothing);
    set_probabilities_simple(simple_tri_gram, NULL, set_probabilities_with_level_good_turing_smoothing);
    perplexity_check(simple_uni_gram, simple_corpus, 14.500734);
    perplexity_check(simple_bi_gram, simple_corpus, 2.762526);
    perplexity_check(simple_tri_gram, simple_corpus, 3.685001);
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
    set_probabilities_simple(simple_uni_gram, NULL, set_probabilities_with_level_good_turing_smoothing);
    set_probabilities_simple(simple_bi_gram, NULL, set_probabilities_with_level_good_turing_smoothing);
    set_probabilities_simple(simple_tri_gram, NULL, set_probabilities_with_level_good_turing_smoothing);
    if (fabs(get_probability(simple_uni_gram, 1, "<s>") - 0.116607) > 0.000001){
        printf("Error in probability 1\n");
    }
    if (fabs(get_probability(simple_uni_gram, 1, "kitabı") - 0.026599) > 0.000001){
        printf("Error in probability 2\n");
    }
    if (fabs(get_probability(simple_bi_gram, 2, "<s>", "ali") - 0.492147) > 0.000001){
        printf("Error in probability 3\n");
    }
    if (fabs(get_probability(simple_bi_gram, 2, "at", "mehmet") - 0.323281) > 0.000001){
        printf("Error in probability 4\n");
    }
    if (fabs(get_probability(simple_tri_gram, 3, "<s>", "ali", "top") - 0.049190) > 0.000001){
        printf("Error in probability 5\n");
    }
    if (fabs(get_probability(simple_tri_gram, 3, "ali", "topu", "at") - 0.261463) > 0.000001){
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