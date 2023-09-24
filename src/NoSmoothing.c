//
// Created by Olcay Taner YILDIZ on 20.09.2023.
//

#include "NoSmoothing.h"

void set_probabilities_with_level_no_smoothing(N_gram_ptr n_gram, int level, void* data) {
    set_probability_with_pseudo_count(n_gram, 0.0, level);
}
