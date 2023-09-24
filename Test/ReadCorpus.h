//
// Created by Olcay Taner YILDIZ on 23.09.2023.
//

#ifndef NGRAM_READCORPUS_H
#define NGRAM_READCORPUS_H

#include <ArrayList.h>
#include "../src/NGram.h"

Array_list_ptr read_corpus(char* file_name);

Array_list_ptr create_simple_corpus();

void perplexity_check(N_gram_ptr n_gram, Array_list_ptr corpus, double _perplexity);

#endif //NGRAM_READCORPUS_H
