//
// Created by Olcay Taner YILDIZ on 24.09.2023.
//

#include <ArrayList.h>
#include <math.h>
#include "ReadCorpus.h"
#include "../src/NoSmoothing.h"

void test_perplexity_simple(){
    Array_list_ptr simple_corpus = create_simple_corpus();
    N_gram_ptr simple_uni_gram = create_string_n_gram(simple_corpus, 1);
    N_gram_ptr simple_bi_gram = create_string_n_gram(simple_corpus, 2);
    N_gram_ptr simple_tri_gram = create_string_n_gram(simple_corpus, 3);
    set_probabilities_simple(simple_uni_gram, NULL, set_probabilities_with_level_no_smoothing);
    set_probabilities_simple(simple_bi_gram, NULL, set_probabilities_with_level_no_smoothing);
    set_probabilities_simple(simple_tri_gram, NULL, set_probabilities_with_level_no_smoothing);
    perplexity_check(simple_uni_gram, simple_corpus, 12.318362);
    perplexity_check(simple_bi_gram, simple_corpus, 1.573148);
    perplexity_check(simple_tri_gram, simple_corpus, 1.248330);
    free_n_gram(simple_uni_gram);
    free_n_gram(simple_bi_gram);
    free_n_gram(simple_tri_gram);
    free_array_list(simple_corpus, NULL);
}

void test_perplexity_complex(){
    Array_list_ptr train_corpus = read_corpus("../train.txt");
    N_gram_ptr complex_uni_gram = create_string_n_gram(train_corpus, 1);
    N_gram_ptr complex_bi_gram = create_string_n_gram(train_corpus, 2);
    N_gram_ptr complex_tri_gram = create_string_n_gram(train_corpus, 3);
    set_probabilities_simple(complex_uni_gram, NULL, set_probabilities_with_level_no_smoothing);
    set_probabilities_simple(complex_bi_gram, NULL, set_probabilities_with_level_no_smoothing);
    set_probabilities_simple(complex_tri_gram, NULL, set_probabilities_with_level_no_smoothing);
    perplexity_check(complex_uni_gram, train_corpus, 3220.299369);
    perplexity_check(complex_bi_gram, train_corpus, 32.362912);
    perplexity_check(complex_tri_gram, train_corpus, 2.025259);
    free_n_gram(complex_uni_gram);
    free_n_gram(complex_bi_gram);
    free_n_gram(complex_tri_gram);
    free_array_list(train_corpus, NULL);
}

void test_calculate_probabilities_simple(){
    Array_list_ptr simple_corpus = create_simple_corpus();
    N_gram_ptr simple_uni_gram = create_string_n_gram(simple_corpus, 1);
    N_gram_ptr simple_bi_gram = create_string_n_gram(simple_corpus, 2);
    N_gram_ptr simple_tri_gram = create_string_n_gram(simple_corpus, 3);
    set_probabilities_simple(simple_uni_gram, NULL, set_probabilities_with_level_no_smoothing);
    set_probabilities_simple(simple_bi_gram, NULL, set_probabilities_with_level_no_smoothing);
    set_probabilities_simple(simple_tri_gram, NULL, set_probabilities_with_level_no_smoothing);
    if (get_probability(simple_uni_gram, 1, "<s>") != 5 / 35.0){
        printf("Error in probability 1\n");
    }
    if (get_probability(simple_uni_gram, 1, "kitabı") != 1.0 / 35.0){
        printf("Error in probability 2\n");
    }
    if (get_probability(simple_bi_gram, 2, "<s>", "ali") != 4.0 / 5.0){
        printf("Error in probability 3\n");
    }
    if (get_probability(simple_bi_gram, 2, "at", "mehmet") != 2.0 / 4.0){
        printf("Error in probability 4\n");
    }
    if (get_probability(simple_tri_gram, 3, "<s>", "ali", "top") != 1.0 / 4.0){
        printf("Error in probability 5\n");
    }
    if (get_probability(simple_tri_gram, 3, "ali", "topu", "at") != 2.0 / 3.0){
        printf("Error in probability 6\n");
    }
    free_n_gram(simple_uni_gram);
    free_n_gram(simple_bi_gram);
    free_n_gram(simple_tri_gram);
    free_array_list(simple_corpus, NULL);
}

void test_calculate_probabilities_complex() {
    Array_list_ptr train_corpus = read_corpus("../train.txt");
    N_gram_ptr complex_uni_gram = create_string_n_gram(train_corpus, 1);
    N_gram_ptr complex_bi_gram = create_string_n_gram(train_corpus, 2);
    N_gram_ptr complex_tri_gram = create_string_n_gram(train_corpus, 3);
    set_probabilities_simple(complex_uni_gram, NULL, set_probabilities_with_level_no_smoothing);
    set_probabilities_simple(complex_bi_gram, NULL, set_probabilities_with_level_no_smoothing);
    set_probabilities_simple(complex_tri_gram, NULL, set_probabilities_with_level_no_smoothing);
    if (get_probability(complex_uni_gram, 1, "<s>") != 20000 / 376019.0){
        printf("Error in probability 1\n");
    }
    if (get_probability(complex_uni_gram, 1, "atatürk") != 50 / 376019.0){
        printf("Error in probability 2\n");
    }
    if (get_probability(complex_bi_gram, 2, "<s>", "mustafa") != 11 / 20000.0){
        printf("Error in probability 3\n");
    }
    if (get_probability(complex_bi_gram, 2, "mustafa", "kemal") != 3 / 138.0){
        printf("Error in probability 4\n");
    }
    if (get_probability(complex_tri_gram, 3, "<s>", "mustafa", "kemal") != 1.0 / 11.0){
        printf("Error in probability 5\n");
    }
    if (get_probability(complex_tri_gram, 3, "mustafa", "kemal", "atatürk") != 1.0 / 3.0){
        printf("Error in probability 6\n");
    }
}

int main(){
    test_perplexity_simple();
    test_calculate_probabilities_simple();
    test_perplexity_complex();
    test_calculate_probabilities_complex();
}