//
// Created by Olcay Taner YILDIZ on 22.09.2023.
//

#ifndef NGRAM_TRAINEDSMOOTHING_H
#define NGRAM_TRAINEDSMOOTHING_H

double new_lower_bound(double current,
                       double current_lower_bound,
                       double current_upper_bound,
                       int number_of_parts);

double new_upper_bound(double current,
                       double current_lower_bound,
                       double current_upper_bound,
                       int number_of_parts);

#endif //NGRAM_TRAINEDSMOOTHING_H
