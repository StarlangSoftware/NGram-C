//
// Created by Olcay Taner YILDIZ on 21.09.2023.
//

#ifndef NGRAM_GOODTURINGSMOOTHING_H
#define NGRAM_GOODTURINGSMOOTHING_H

#include "NGram.h"

double* linear_regression_on_counts_of_counts(const int* counts_of_counts, int size);

void set_probabilities_with_level_good_turing_smoothing(N_gram_ptr n_gram,
                                                        int level,
                                                        void* data);

#endif //NGRAM_GOODTURINGSMOOTHING_H
