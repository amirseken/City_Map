/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   math_helpers.h
 * Author: tokhoi
 *
 * Created on February 22, 2016, 4:49 PM
 */

#ifndef MATH_HELPERS_H
#define MATH_HELPERS_H

//cosine approximation
//Reference: W. J. Cody, W. Waite, Software Manual for the Elementary Functions. Prentice-Hall, 1980
double cos_core (double x);

//new absolute value function to support find closest function
double new_abs (double x);

#endif /* MATH_HELPERS_H */

