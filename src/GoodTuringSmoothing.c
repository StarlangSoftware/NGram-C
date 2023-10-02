//
// Created by Olcay Taner YILDIZ on 21.09.2023.
//

#include <stdlib.h>
#include <Matrix.h>
#include <math.h>
#include "GoodTuringSmoothing.h"

/**
 * Wrapper function to set the N-gram probabilities with Good-Turing smoothing. N[1] / \sum_{i=1}^infty N_i is
 * the out of vocabulary probability.
 * @param nGram N-Gram for which the probabilities will be set.
 * @param level Level for which N-Gram probabilities will be set. Probabilities for different levels of the
 *              N-gram can be set with this function. If level = 1, N-Gram is treated as UniGram, if level = 2,
 *              N-Gram is treated as Bigram, etc.
 */
void set_probabilities_with_level_good_turing_smoothing(N_gram_ptr n_gram,
                                                        int level,
                                                        void *data) {
    int size;
    int* countsOfCounts = calculate_counts_of_counts(n_gram, level, &size);
    double* N = linear_regression_on_counts_of_counts(countsOfCounts, size);
    double sum = 0;
    for (int r = 1; r < size; r++){
        sum += countsOfCounts[r] * r;
    }
    free(countsOfCounts);
    set_adjusted_probability(n_gram, N, level, N[1] / sum);
    free(N);
}

/**
 * Given counts of counts, this function will calculate the estimated counts of counts c$^*$ with
 * Good-Turing smoothing. First, the algorithm filters the non-zero counts from counts of counts array and constructs
 * c and r arrays. Then it constructs Z_n array with Z_n = (2C_n / (r_{n+1} - r_{n-1})). The algorithm then uses
 * simple linear regression on Z_n values to estimate w_1 and w_0, where log(N[i]) = w_1log(i) + w_0
 * @param countsOfCounts Counts of counts. countsOfCounts[1] is the number of words occurred once in the corpus.
 *                       countsOfCounts[i] is the number of words occurred i times in the corpus.
 * @return Estimated counts of counts array. N[1] is the estimated count for out of vocabulary words.
 */
double *linear_regression_on_counts_of_counts(const int *counts_of_counts, int size) {
    int r_i_plus_1, r_i_minus_1;
    double* N = malloc(sizeof(double) * size);
    Array_list_ptr r = create_array_list();
    Array_list_ptr c = create_array_list();
    double x_t, r_t;
    for (int i = 1; i < size; i++) {
        if (counts_of_counts[i] != 0) {
            array_list_add_int(r, i);
            array_list_add_int(c, counts_of_counts[i]);
        }
    }
    Matrix_ptr A = create_matrix(2, 2);
    Vector_ptr y = create_vector2(2, 0);
    for (int i = 0; i < r->size; i++){
        int r_i = *(int*)array_list_get(r, i);
        if (i != r->size - 1){
            r_i_plus_1 = *(int*)array_list_get(r, i + 1);
        }
        if (i != 0){
            r_i_minus_1 = *(int*)array_list_get(r, i - 1);
        }
        double c_i = *(int*)array_list_get(c, i);
        x_t = log(r_i);
        if (i == 0){
            r_t = log(c_i);
        } else {
            if (i == r->size - 1){
                r_t = log((1.0 * c_i) / (r_i - r_i_minus_1));
            } else {
                r_t = log((2.0 * c_i) / (r_i_plus_1 - r_i_minus_1));
            }
        }
        add_value_to_matrix(A, 0, 0, 1.0);
        add_value_to_matrix(A, 0, 1, x_t);
        add_value_to_matrix(A, 1, 0, x_t);
        add_value_to_matrix(A, 1, 1, x_t * x_t);
        add_value(y, 0, r_t);
        add_value(y, 1, r_t * x_t);
    }
    inverse(A);
    Vector_ptr w = multiply_with_vector_from_right(A, y);
    free_matrix(A);
    free_vector(y);
    double w0 = get_value(w, 0);
    double w1 = get_value(w, 1);
    free_vector(w);
    for (int i = 1; i < size; i++){
        N[i] = exp(log(i) * w1 + w0);
    }
    free_array_list(r, free);
    free_array_list(c, free);
    return N;
}
