//
// Created by Olcay Taner YILDIZ on 23.09.2023.
//

#include <ArrayList.h>
#include <Memory/Memory.h>
#include "../src/NGram.h"
#include "ReadCorpus.h"

void check_n_gram(N_gram_ptr n_gram, char** s1, int length, int count, int index){
    if (get_count(n_gram, (void **) s1, length) != count){
        printf("Error in ngram %d: value %d\n", index, get_count(n_gram, (void **) s1, length));
    }
}

void test_counts(N_gram_ptr simple_uni_gram, N_gram_ptr simple_bi_gram, N_gram_ptr simple_tri_gram){
    check_n_gram(simple_uni_gram, (char*[]){"<s>"}, 1, 5, 1);
    check_n_gram(simple_uni_gram, (char*[]){"mahmut"}, 1, 0, 2);
    check_n_gram(simple_uni_gram, (char*[]){"kitabı"}, 1, 1, 3);
    check_n_gram(simple_bi_gram, (char*[]){"<s>", "ali"}, 2, 4, 4);
    check_n_gram(simple_bi_gram, (char*[]){"ayşe", "ali"}, 2, 0, 5);
    check_n_gram(simple_bi_gram, (char*[]){"mahmut", "ali"}, 2, 0, 6);
    check_n_gram(simple_bi_gram, (char*[]){"at", "mehmet"}, 2, 2, 7);
    check_n_gram(simple_tri_gram, (char*[]){"<s>", "ali", "top"}, 3, 1, 8);
    check_n_gram(simple_tri_gram, (char*[]){"ayşe", "kitabı", "at"}, 3, 0, 9);
    check_n_gram(simple_tri_gram, (char*[]){"mahmut", "evde", "kal"}, 3, 0, 10);
    check_n_gram(simple_tri_gram, (char*[]){"ali", "topu", "at"}, 3, 2, 11);
}

void test_get_count_simple(){
    Array_list_ptr simple_corpus = create_simple_corpus();
    N_gram_ptr simple_uni_gram = create_string_n_gram(simple_corpus, 1);
    N_gram_ptr simple_bi_gram = create_string_n_gram(simple_corpus, 2);
    N_gram_ptr simple_tri_gram = create_string_n_gram(simple_corpus, 3);
    test_counts(simple_uni_gram, simple_bi_gram, simple_tri_gram);
    free_2d_array_list(simple_corpus, NULL);
    free_n_gram(simple_uni_gram);
    free_n_gram(simple_bi_gram);
    free_n_gram(simple_tri_gram);
}

void test_vocabulary_size_simple(){
    Array_list_ptr simple_corpus = create_simple_corpus();
    N_gram_ptr simple_uni_gram = create_string_n_gram(simple_corpus, 1);
    if (vocabulary_size(simple_uni_gram) != 15){
        printf("Error in vocabulary size\n");
    }
    free_2d_array_list(simple_corpus, NULL);
    free_n_gram(simple_uni_gram);
}

void test_prune(){
    Array_list_ptr simple_corpus = create_simple_corpus();
    N_gram_ptr simple_bi_gram = create_string_n_gram(simple_corpus, 2);
    prune(simple_bi_gram, 0.0);
    check_n_gram(simple_bi_gram, (char*[]){"<s>", "ali"}, 2, 4, 1);
    check_n_gram(simple_bi_gram, (char*[]){"<s>", "ayşe"}, 2, 1, 2);
    check_n_gram(simple_bi_gram, (char*[]){"ali", "topu"}, 2, 3, 3);
    check_n_gram(simple_bi_gram, (char*[]){"ali", "top"}, 2, 1, 4);
    check_n_gram(simple_bi_gram, (char*[]){"topu", "at"}, 2, 2, 5);
    check_n_gram(simple_bi_gram, (char*[]){"topu", "mehmete"}, 2, 1, 6);
    prune(simple_bi_gram, 0.6);
    check_n_gram(simple_bi_gram, (char*[]){"<s>", "ali"}, 2, 4, 7);
    check_n_gram(simple_bi_gram, (char*[]){"<s>", "ayşe"}, 2, 0, 8);
    check_n_gram(simple_bi_gram, (char*[]){"ali", "topu"}, 2, 3, 9);
    check_n_gram(simple_bi_gram, (char*[]){"ali", "top"}, 2, 0, 10);
    check_n_gram(simple_bi_gram, (char*[]){"topu", "at"}, 2, 2, 11);
    check_n_gram(simple_bi_gram, (char*[]){"topu", "mehmete"}, 2, 0, 12);
    prune(simple_bi_gram, 0.7);
    check_n_gram(simple_bi_gram, (char*[]){"<s>", "ali"}, 2, 4, 13);
    check_n_gram(simple_bi_gram, (char*[]){"ali", "topu"}, 2, 3, 14);
    check_n_gram(simple_bi_gram, (char*[]){"topu", "at"}, 2, 2, 15);
    prune(simple_bi_gram, 0.8);
    check_n_gram(simple_bi_gram, (char*[]){"<s>", "ali"}, 2, 4, 16);
    check_n_gram(simple_bi_gram, (char*[]){"ali", "topu"}, 2, 3, 17);
    prune(simple_bi_gram, 0.9);
    check_n_gram(simple_bi_gram, (char*[]){"<s>", "ali"}, 2, 4, 18);
    free_2d_array_list(simple_corpus, NULL);
    free_n_gram(simple_bi_gram);
}

void test_load_n_gram(){
    N_gram_ptr n_gram_1 = create_string_n_gram3("simple1.txt");
    N_gram_ptr n_gram_2 = create_string_n_gram3("simple2.txt");
    N_gram_ptr n_gram_3 = create_string_n_gram3("simple3.txt");
    check_n_gram(n_gram_1, (char*[]){"<s>"}, 1, 5, 1);
    check_n_gram(n_gram_1, (char*[]){"at"}, 1, 4, 2);
    check_n_gram(n_gram_1, (char*[]){"mehmet"}, 1, 2, 3);
    check_n_gram(n_gram_2, (char*[]){"ver", "</s>"}, 2, 1, 4);
    check_n_gram(n_gram_2, (char*[]){"<s>", "ali"}, 2, 4, 5);
    check_n_gram(n_gram_2, (char*[]){"topu", "at"}, 2, 2, 6);
    check_n_gram(n_gram_3, (char*[]){"ayşe", "eve", "gitti"}, 3, 1, 7);
    check_n_gram(n_gram_3, (char*[]){"topu", "at", "mehmet"}, 3, 2, 8);
    check_n_gram(n_gram_3, (char*[]){"<s>", "ali", "topu"}, 3, 3, 9);
    free_n_gram(n_gram_1);
    free_n_gram(n_gram_2);
    free_n_gram(n_gram_3);
}

void test_merge(){
    N_gram_ptr n_gram_1 = create_string_n_gram3("simple1a.txt");
    N_gram_ptr n_gram_b = create_string_n_gram3("simple1b.txt");
    merge(n_gram_1, n_gram_b);
    if (vocabulary_size(n_gram_1) != 18){
        printf("Error in merge 1 %d\n", vocabulary_size(n_gram_1));
    }
    free_n_gram(n_gram_b);
    free_n_gram(n_gram_1);
    N_gram_ptr n_gram_2 = create_string_n_gram3("simple2a.txt");
    n_gram_b = create_string_n_gram3("simple2b.txt");
    N_gram_ptr n_gram_c = create_string_n_gram3("simple2c.txt");
    N_gram_ptr n_gram_d = create_string_n_gram3("simple2d.txt");
    merge(n_gram_2, n_gram_b);
    merge(n_gram_2, n_gram_c);
    merge(n_gram_2, n_gram_d);
    if (vocabulary_size(n_gram_2) != 21){
        printf("Error in merge 2 %d\n", vocabulary_size(n_gram_2));
    }
    free_n_gram(n_gram_b);
    free_n_gram(n_gram_c);
    free_n_gram(n_gram_d);
    free_n_gram(n_gram_2);
    N_gram_ptr n_gram_3 = create_string_n_gram3("simple3a.txt");
    n_gram_b = create_string_n_gram3("simple3b.txt");
    n_gram_c = create_string_n_gram3("simple3c.txt");
    merge(n_gram_3, n_gram_b);
    merge(n_gram_3, n_gram_c);
    if (vocabulary_size(n_gram_3) != 20){
        printf("Error in merge 3 %d\n", vocabulary_size(n_gram_3));
    }
    free_n_gram(n_gram_b);
    free_n_gram(n_gram_c);
    free_n_gram(n_gram_3);
}

void test_get_count_complex() {
    Array_list_ptr train_corpus = read_corpus("train.txt");
    N_gram_ptr complex_uni_gram = create_string_n_gram(train_corpus, 1);
    N_gram_ptr complex_bi_gram = create_string_n_gram(train_corpus, 2);
    N_gram_ptr complex_tri_gram = create_string_n_gram(train_corpus, 3);
    check_n_gram(complex_uni_gram, (char*[]){"<s>"}, 1, 20000, 1);
    check_n_gram(complex_uni_gram, (char*[]){"atatürk"}, 1, 50, 2);
    check_n_gram(complex_bi_gram, (char*[]){"<s>", "mustafa"}, 2, 11, 3);
    check_n_gram(complex_bi_gram, (char*[]){"mustafa", "kemal"}, 2, 3, 4);
    check_n_gram(complex_tri_gram, (char*[]){"<s>", "mustafa", "kemal"}, 3, 1, 5);
    check_n_gram(complex_tri_gram, (char*[]){"mustafa", "kemal", "atatürk"}, 3, 1, 6);
    free_2d_array_list(train_corpus, free_);
    free_n_gram(complex_uni_gram);
    free_n_gram(complex_bi_gram);
    free_n_gram(complex_tri_gram);
}

void test_vocabulary_size_complex() {
    Array_list_ptr train_corpus = read_corpus("train.txt");
    N_gram_ptr complex_uni_gram1 = create_string_n_gram(train_corpus, 1);
    if (vocabulary_size(complex_uni_gram1) != 57625){
        printf("Error in complex vocabulary size 1\n");
    }
    free_2d_array_list(train_corpus, free_);
    free_n_gram(complex_uni_gram1);
    Array_list_ptr test_corpus = read_corpus("test.txt");
    N_gram_ptr complex_uni_gram2 = create_string_n_gram(test_corpus, 1);
    if (vocabulary_size(complex_uni_gram2) != 55485){
        printf("Error in complex vocabulary size 2\n");
    }
    free_2d_array_list(test_corpus, free_);
    free_n_gram(complex_uni_gram2);
    Array_list_ptr validation_corpus = read_corpus("validation.txt");
    N_gram_ptr complex_uni_gram3 = create_string_n_gram(validation_corpus, 1);
    if (vocabulary_size(complex_uni_gram3) != 35663){
        printf("Error in complex vocabulary size 3\n");
    }
    free_2d_array_list(validation_corpus, free_);
    free_n_gram(complex_uni_gram3);
}

void test_load_multi_part(){
    N_gram_ptr simple_uni_gram = create_string_n_gram5(2, "simple1part1.txt", "simple1part2.txt");
    N_gram_ptr simple_bi_gram = create_string_n_gram5(3, "simple2part1.txt", "simple2part2.txt", "simple2part3.txt");
    N_gram_ptr simple_tri_gram = create_string_n_gram5(4, "simple3part1.txt", "simple3part2.txt", "simple3part3.txt", "simple3part4.txt");
    if (vocabulary_size(simple_uni_gram) != 15){
        printf("Error in vocabulary size\n");
    }
    test_counts(simple_uni_gram, simple_bi_gram, simple_tri_gram);
    free_n_gram(simple_uni_gram);
    free_n_gram(simple_bi_gram);
    free_n_gram(simple_tri_gram);
}

int main(){
    test_load_multi_part();
    test_get_count_simple();
    test_vocabulary_size_simple();
    test_prune();
    test_load_n_gram();
    test_merge();
    test_get_count_complex();
    test_vocabulary_size_complex();
}