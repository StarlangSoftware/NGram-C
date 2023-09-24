//
// Created by Olcay Taner YILDIZ on 22.09.2023.
//

#include "TrainedSmoothing.h"

/**
 * Calculates new lower bound.
 * @param current current value.
 * @param current_lower_bound current lower bound
 * @param current_upper_bound current upper bound
 * @param number_of_parts number of parts between lower and upper bound.
 * @return new lower bound
 */
double new_lower_bound(double current,
                       double current_lower_bound,
                       double current_upper_bound,
                       int number_of_parts) {
    if (current != current_lower_bound){
        return current - (current_upper_bound - current_lower_bound) / number_of_parts;
    } else {
        return current / number_of_parts;
    }
}

/**
 * Calculates new upper bound.
 * @param current current value.
 * @param current_lower_bound current lower bound
 * @param current_upper_bound current upper bound
 * @param number_of_parts number of parts between lower and upper bound.
 * @return new upper bound
 */
double new_upper_bound(double current,
                       double current_lower_bound,
                       double current_upper_bound,
                       int number_of_parts) {
    if (current != current_upper_bound){
        return current + (current_upper_bound - current_lower_bound) / number_of_parts;
    } else {
        return current * number_of_parts;
    }
}
