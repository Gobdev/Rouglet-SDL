#pragma once

void rng_init(unsigned short input);
/*
    An rng function based on the function used in Super Mario 64.
    Source for this code can be found here: 'https://www.youtube.com/watch?v=MiuLeTE2MeQ'.
    This function is a bijection from the set of all possible values of an unsigned short to the same set.
    In the cycle of this set used in the function, 65534 return values are possible.
*/
unsigned short rng_function();
