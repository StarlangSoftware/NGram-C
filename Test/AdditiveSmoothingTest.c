//
// Created by Olcay Taner YILDIZ on 24.09.2023.
//

#include <ArrayList.h>
#include "ReadCorpus.h"
#include "../src/AdditiveSmoothing.h"

void test_perplexity_complex(){
    Array_list_ptr train_corpus = read_corpus("../train.txt");
    Array_list_ptr test_corpus = read_corpus("../test.txt");
    Array_list_ptr validation_corpus = read_corpus("../validation.txt");
    N_gram_ptr complex_uni_gram = create_string_n_gram(train_corpus, 1);
    N_gram_ptr complex_bi_gram = create_string_n_gram(train_corpus, 2);
    N_gram_ptr complex_tri_gram = create_string_n_gram(train_corpus, 3);
    set_probabilities_trained(complex_uni_gram, validation_corpus,  learn_parameters_additive_smoothing, set_probabilities_with_level_additive_smoothing);
    set_probabilities_trained(complex_bi_gram, validation_corpus,  learn_parameters_additive_smoothing, set_probabilities_with_level_additive_smoothing);
    set_probabilities_trained(complex_tri_gram, validation_corpus,  learn_parameters_additive_smoothing, set_probabilities_with_level_additive_smoothing);
    perplexity_check(complex_uni_gram, test_corpus, 4043.947022);
    perplexity_check(complex_bi_gram, test_corpus, 9220.218871);
    perplexity_check(complex_tri_gram, test_corpus, 30263.189040);
    free_n_gram(complex_uni_gram);
    free_n_gram(complex_bi_gram);
    free_n_gram(complex_tri_gram);
    free_array_list(train_corpus, NULL);
    free_array_list(test_corpus, NULL);
    free_array_list(validation_corpus, NULL);
}

int main(){
    test_perplexity_complex();
}