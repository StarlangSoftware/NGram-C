//
// Created by Olcay Taner YILDIZ on 22.09.2023.
//

#ifndef NGRAM_ADDITIVESMOOTHING_H
#define NGRAM_ADDITIVESMOOTHING_H

#include <ArrayList.h>
#include "NGram.h"
#include <KFoldCrossValidation.h>

double* learn_parameters_additive_smoothing(Array_list_ptr corpus, int N);

void set_probabilities_with_level_additive_smoothing(N_gram_ptr n_gram,
                                                     int level,
                                                     double* parameters);

double learn_best_delta(Array_list_ptr nGrams,
                        K_fold_cross_validation_ptr kFoldCrossValidation,
                        double lower_bound);

#endif //NGRAM_ADDITIVESMOOTHING_H
