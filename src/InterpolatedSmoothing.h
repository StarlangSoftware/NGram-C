//
// Created by Olcay Taner YILDIZ on 22.09.2023.
//

#ifndef NGRAM_INTERPOLATEDSMOOTHING_H
#define NGRAM_INTERPOLATEDSMOOTHING_H

#include <ArrayList.h>
#include <KFoldCrossValidation.h>
#include "NGram.h"

double *learn_parameters_interpolated_smoothing(Array_list_ptr corpus, int N);

void set_probabilities_with_level_interpolated_smoothing(N_gram_ptr n_gram,
                                                     int level,
                                                     const double *parameters);

double* learn_best_lambda(Array_list_ptr nGrams,
                         K_fold_cross_validation_ptr kFoldCrossValidation,
                         double lower_bound);

double* learn_best_lambdas(Array_list_ptr nGrams,
                         K_fold_cross_validation_ptr kFoldCrossValidation,
                         double lower_bound1,
                         double lower_bound2);

#endif //NGRAM_INTERPOLATEDSMOOTHING_H
