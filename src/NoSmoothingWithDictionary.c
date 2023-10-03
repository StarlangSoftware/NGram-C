//
// Created by Olcay Taner YILDIZ on 22.09.2023.
//

#include "NoSmoothingWithDictionary.h"

/**
 * Wrapper function to set the N-gram probabilities with no smoothing and replacing unknown words not found in HashSet the dictionary.
 * @param n_gram N-Gram for which the probabilities will be set.
 * @param level Level for which N-Gram probabilities will be set. Probabilities for different levels of the
 *              N-gram can be set with this function. If level = 1, N-Gram is treated as UniGram, if level = 2,
 *              N-Gram is treated as Bigram, etc.
 *
 */
void set_probabilities_with_level_no_smoothing_with_dictionary(N_gram_ptr n_gram,
                                                               int level,
                                                               void *data) {
    Hash_set_ptr dictionary = (Hash_set_ptr) data;
    replace_unknown_words(n_gram, dictionary);
    set_probability_with_pseudo_count(n_gram, 0.0, level);
}
