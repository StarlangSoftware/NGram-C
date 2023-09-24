//
// Created by Olcay Taner YILDIZ on 22.09.2023.
//

#ifndef NGRAM_LAPLACESMOOTHING_H
#define NGRAM_LAPLACESMOOTHING_H

#include "NGram.h"

void set_probabilities_with_level_laplace_smoothing(N_gram_ptr n_gram,
                                                    int level,
                                                    void* data);

#endif //NGRAM_LAPLACESMOOTHING_H
