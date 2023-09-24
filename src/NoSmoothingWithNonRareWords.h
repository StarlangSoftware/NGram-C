//
// Created by Olcay Taner YILDIZ on 22.09.2023.
//

#ifndef NGRAM_NOSMOOTHINGWITHNONRAREWORDS_H
#define NGRAM_NOSMOOTHINGWITHNONRAREWORDS_H

#include "NGram.h"

void set_probabilities_with_level_no_smoothing_with_non_rare_words(N_gram_ptr n_gram,
                                                                  int level,
                                                                  void* data);

#endif //NGRAM_NOSMOOTHINGWITHNONRAREWORDS_H
