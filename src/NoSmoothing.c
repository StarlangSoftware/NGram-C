//
// Created by Olcay Taner YILDIZ on 20.09.2023.
//

#include "NoSmoothing.h"

/**
 * Calculates the N-Gram probabilities with no smoothing
 * @param n_gram N-Gram for which no smoothing is done.
 * @param level Height of the NGram node.
 */
void set_probabilities_with_level_no_smoothing(N_gram_ptr n_gram, int level, void* data) {
    set_probability_with_pseudo_count(n_gram, 0.0, level);
}
