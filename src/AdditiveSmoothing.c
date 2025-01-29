//
// Created by Olcay Taner YILDIZ on 22.09.2023.
//

#include <KFoldCrossValidation.h>
#include <float.h>
#include <math.h>
#include <Memory/Memory.h>
#include "AdditiveSmoothing.h"
#include "TrainedSmoothing.h"


/**
 * Wrapper function to learn the parameter (delta) in additive smoothing. The function first creates K NGrams
 * with the train folds of the corpus. Then optimizes delta with respect to the test folds of the corpus.
 * @param corpus Train corpus used to optimize delta parameter
 * @param N N in N-Gram.
 */
double* learn_parameters_additive_smoothing(Array_list_ptr corpus, int N) {
    int K = 10;
    Array_list_ptr n_grams = create_array_list();
    K_fold_cross_validation_ptr k_fold_cross_validation = create_k_fold_cross_validation(corpus, K, 0);
    for (int i = 0; i < K; i++){
        N_gram_ptr nGram = create_n_gram(get_train_fold_k_fold(k_fold_cross_validation, i),
                                         N,
                                         (unsigned int (*)(const void *, int)) hash_function_string,
                                         (int (*)(const void *, const void *)) compare_string);
        array_list_add(n_grams, nGram);
    }
    double* delta = malloc_(sizeof(double), "learn_parameters_additive_smoothing");
    *delta = learn_best_delta(n_grams, k_fold_cross_validation, 0.1);
    free_array_list(n_grams, (void (*)(void *)) free_n_gram);
    free_k_fold_cross_validation(k_fold_cross_validation);
    return delta;
}

/**
 * Wrapper function to set the N-gram probabilities with additive smoothing.
 * @param n_gram N-Gram for which the probabilities will be set.
 * @param level Level for which N-Gram probabilities will be set. Probabilities for different levels of the
 *              N-gram can be set with this function. If level = 1, N-Gram is treated as UniGram, if level = 2,
 *              N-Gram is treated as Bigram, etc.
 * @param parameters Parameters of the ngram model.
 */
void set_probabilities_with_level_additive_smoothing(N_gram_ptr n_gram, int level, const double* parameters) {
    set_probability_with_pseudo_count(n_gram, *parameters, level);
}

/**
 * The algorithm tries to optimize the best delta for a given corpus. The algorithm uses perplexity on the validation
 * set as the optimization criterion.
 * @param nGrams 10 N-Grams learned for different folds of the corpus. nGrams[i] is the N-Gram trained with i'th train
 *               fold of the corpus.
 * @param kFoldCrossValidation Cross-validation data used in training and testing the N-grams.
 * @param lower_bound Initial lower bound for optimizing the best delta.
 * @return Best delta optimized with k-fold crossvalidation.
 */
double learn_best_delta(Array_list_ptr nGrams, K_fold_cross_validation_ptr kFoldCrossValidation, double lower_bound) {
    double best_perplexity, best_previous = -1, upper_bound = 1, perplexity, best_delta = (lower_bound + upper_bound) / 2;
    int number_of_parts = 5;
    while (true){
        best_perplexity = DBL_MAX;
        for (double value = lower_bound; value <= upper_bound; value += (upper_bound - lower_bound) / number_of_parts){
            perplexity = 0;
            for (int i = 0; i < 10; i++){
                N_gram_ptr n_gram = array_list_get(nGrams, i);
                set_probability_with_pseudo_count(n_gram, value, n_gram->N);
                Array_list_ptr test_fold = get_test_fold_k_fold(kFoldCrossValidation, i);
                perplexity += get_perplexity(n_gram, test_fold);
                free_array_list(test_fold, NULL);
            }
            if (perplexity < best_perplexity){
                best_perplexity = perplexity;
                best_delta = value;
            }
        }
        lower_bound = new_lower_bound(best_delta, lower_bound, upper_bound, number_of_parts);
        upper_bound = new_upper_bound(best_delta, lower_bound, upper_bound, number_of_parts);
        if (best_previous != -1){
            if (fabs(best_previous - best_perplexity) / best_perplexity < 0.001){
                break;
            }
        }
        best_previous = best_perplexity;
    }
    return best_delta;
}
