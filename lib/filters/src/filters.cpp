#include "filters.h"

/*!
 * \brief This is a an implementation of the complementary filter
 * \param prev the previously computed value
 * \param a First source
 * \param b Second source
 * \param dt Time step in seconds (tunable, should match your sampling rate)
 * \param alpha Complementary filter coefficient (tunable)
 */
float complementaryFilter(float prev, float a, float b, float alpha)
{
    return alpha * (prev + a) + (1 - alpha) * b;
}
