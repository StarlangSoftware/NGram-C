//
// Created by Olcay Taner YILDIZ on 22.09.2023.
//

#include <float.h>
#include <math.h>
#include <Memory/Memory.h>
#include "InterpolatedSmoothing.h"
#include "TrainedSmoothing.h"
#include "GoodTuringSmoothing.h"

/**
 * The algorithm tries to optimize the best lambda for a given corpus. The algorithm uses perplexity on the validation
 * set as the optimization criterion.
 *
 * @param nGrams 10 N-Grams learned for different folds of the corpus. nGrams[i] is the N-Gram trained with i'th train fold of the corpus.
 * @param kFoldCrossValidation Cross-validation data used in training and testing the N-grams.
 * @param lower_bound Initial lower bound for optimizing the best lambda.
 * @return  Best lambda optimized with k-fold crossvalidation.
 */
double *learn_best_lambda(Array_list_ptr nGrams,
                          K_fold_cross_validation_ptr kFoldCrossValidation,
                          double lower_bound) {
    double best_perplexity, best_previous = -1, upper_bound = 0.999, perplexity;
    double* best_lambda = malloc_(sizeof(double), "learn_best_lambda");
    *best_lambda = (lower_bound + upper_bound) / 2;
    int number_of_parts = 5;
    Array_list_ptr test_folds[10];
    for (int i = 0; i < 10; i++){
        test_folds[i] = get_test_fold_k_fold(kFoldCrossValidation, i);
    }
    while (true){
        best_perplexity = DBL_MAX;
        for (double value = lower_bound; value <= upper_bound; value += (upper_bound - lower_bound) / number_of_parts){
            perplexity = 0;
            for (int i = 0; i < 10; i++){
                N_gram_ptr n_gram = array_list_get(nGrams, i);
                set_lambda1(n_gram, value);
                perplexity += get_perplexity(n_gram, test_folds[i]);
            }
            if (perplexity < best_perplexity){
                best_perplexity = perplexity;
                *best_lambda = value;
            }
        }
        lower_bound = new_lower_bound(*best_lambda, lower_bound, upper_bound, number_of_parts);
        upper_bound = new_upper_bound(*best_lambda, lower_bound, upper_bound, number_of_parts);
        if (best_previous != -1){
            if (fabs(best_previous - best_perplexity) / best_perplexity < 0.001){
                break;
            }
        }
        best_previous = best_perplexity;
    }
    for (int i = 0; i < 10; i++){
        free_array_list(test_folds[i], NULL);
    }
    return best_lambda;
}

/**
 * The algorithm tries to optimize the best lambdas (lambda1, lambda2) for a given corpus. The algorithm uses perplexity on the validation
 * set as the optimization criterion.
 *
 * @param nGrams 10 N-Grams learned for different folds of the corpus. nGrams[i] is the N-Gram trained with i'th train fold of the corpus.
 * @param kFoldCrossValidation Cross-validation data used in training and testing the N-grams.
 * @param lower_bound1 Initial lower bound for optimizing the best lambda1.
 * @param lower_bound2 Initial lower bound for optimizing the best lambda2.
 * @return
 */
double *learn_best_lambdas(Array_list_ptr nGrams,
                           K_fold_cross_validation_ptr kFoldCrossValidation,
                           double lower_bound1,
                           double lower_bound2) {
    double best_perplexity, upper_bound1 = 0.999, upper_bound2 = 0.999, best_previous = -1, perplexity, best_lambda1 = (lower_bound1 + upper_bound1) / 2, best_lambda2 = (lower_bound2 + upper_bound2) / 2;
    Array_list_ptr test_folds[10];
    int number_of_parts = 5;
    for (int i = 0; i < 10; i++){
        test_folds[i] = get_test_fold_k_fold(kFoldCrossValidation, i);
    }
    while (true){
        best_perplexity = DBL_MAX;
        for (double value1 = lower_bound1; value1 <= upper_bound1; value1 += (upper_bound1 - lower_bound1) / number_of_parts){
            for (double value2 = lower_bound2; value2 <= upper_bound2 && value1 + value2 < 1; value2 += (upper_bound2 - lower_bound2) / number_of_parts){
                perplexity = 0;
                for (int i = 0; i < 10; i++){
                    N_gram_ptr n_gram = array_list_get(nGrams, i);
                    set_lambda2(n_gram, value1, value2);
                    perplexity += get_perplexity(n_gram, test_folds[i]);
                }
                if (perplexity < best_perplexity){
                    best_perplexity = perplexity;
                    best_lambda1 = value1;
                    best_lambda2 = value2;
                }
            }
        }
        lower_bound1 = new_lower_bound(best_lambda1, lower_bound1, upper_bound1, number_of_parts);
        upper_bound1 = new_upper_bound(best_lambda1, lower_bound1, upper_bound1, number_of_parts);
        lower_bound2 = new_lower_bound(best_lambda2, lower_bound2, upper_bound2, number_of_parts);
        upper_bound2 = new_upper_bound(best_lambda2, lower_bound2, upper_bound2, number_of_parts);
        if (best_previous != -1){
            if (fabs(best_previous - best_perplexity) / best_perplexity < 0.001){
                break;
            }
        }
        best_previous = best_perplexity;
    }
    for (int i = 0; i < 10; i++){
        free_array_list(test_folds[i], NULL);
    }
    double* best_lambda = malloc_(2 * sizeof(double), "learn_best_lambdas");
    best_lambda[0] = best_lambda1;
    best_lambda[1] = best_lambda2;
    return best_lambda;
}

/**
 * Wrapper function to learn the parameters (lambda1 and lambda2) in interpolated smoothing. The function first creates K NGrams
 * with the train folds of the corpus. Then optimizes lambdas with respect to the test folds of the corpus depending on given N.
 * @param corpus Train corpus used to optimize lambda parameters
 * @param N N in N-Gram.
 */
double *learn_parameters_interpolated_smoothing(Array_list_ptr corpus, int N) {
    double* best_lambda = NULL;
    if (N <= 1){
        return best_lambda;
    }
    int K = 10;
    Array_list_ptr n_grams = create_array_list();
    K_fold_cross_validation_ptr k_fold_cross_validation = create_k_fold_cross_validation(corpus, K, 0);
    for (int i = 0; i < K; i++){
        N_gram_ptr nGram = create_n_gram(get_train_fold_k_fold(k_fold_cross_validation, i),
                                         N,
                                         (unsigned int (*)(const void *, int)) hash_function_string,
                                         (int (*)(const void *, const void *)) compare_string);
        array_list_add(n_grams, nGram);
        for (int j = 2; j<= N; j++){
            set_probabilities_with_level_good_turing_smoothing(array_list_get(n_grams, i), j, NULL);
        }
        set_probabilities_with_level_good_turing_smoothing(array_list_get(n_grams, i), 1, NULL);
    }
    if (N == 2){
        best_lambda = learn_best_lambda(n_grams, k_fold_cross_validation, 0.1);
    } else {
        if (N == 3){
            best_lambda = learn_best_lambdas(n_grams, k_fold_cross_validation, 0.1, 0.1);
        }
    }
    free_array_list(n_grams, (void (*)(void *)) free_n_gram);
    free_k_fold_cross_validation(k_fold_cross_validation);
    return best_lambda;
}

/**
 * Wrapper function to set the N-gram probabilities with interpolated smoothing.
 * @param n_gram N-Gram for which the probabilities will be set.
 * @param level Level for which N-Gram probabilities will be set. Probabilities for different levels of the
 *              N-gram can be set with this function. If level = 1, N-Gram is treated as UniGram, if level = 2,
 *              N-Gram is treated as Bigram, etc.
 * @param parameters Parameters of the ngram model.
 */
void set_probabilities_with_level_interpolated_smoothing(N_gram_ptr n_gram,
                                                         int level,
                                                         const double *parameters) {
    for (int j = 2; j<= n_gram->N; j++){
        set_probabilities_with_level_good_turing_smoothing(n_gram, j, NULL);
    }
    set_probabilities_with_level_good_turing_smoothing(n_gram, 1, NULL);
    switch (n_gram->N){
        case 2:
            set_lambda1(n_gram, parameters[0]);
            break;
        case 3:
            set_lambda2(n_gram, parameters[0], parameters[1]);
            break;
    }
}
