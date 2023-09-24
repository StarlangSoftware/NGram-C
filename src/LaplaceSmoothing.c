//
// Created by Olcay Taner YILDIZ on 22.09.2023.
//

#include "LaplaceSmoothing.h"

/**
 * Wrapper function to set the N-gram probabilities with laplace smoothing.
 *
 * @param nGram N-Gram for which the probabilities will be set.
 * @param level Level for which N-Gram probabilities will be set. Probabilities for different levels of the
 *              N-gram can be set with this function. If level = 1, N-Gram is treated as UniGram, if level = 2,
 *              N-Gram is treated as Bigram, etc.
 */
void set_probabilities_with_level_laplace_smoothing(N_gram_ptr n_gram,
                                                    int level,
                                                    void *data) {
    double delta = *(double*) data;
    set_probability_with_pseudo_count(n_gram, delta, level);
}
