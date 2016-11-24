#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

typedef          long double   ld;
typedef unsigned long long int ull;

const ull ULL_MAX = (ull)-1;
const ld  MATH_E  = 2.71828182845904523536028747135266249775724709369995;


/* https://primes.utm.edu/howmany.html
 * Massias and Robin proved that if P(k) signifies the kth prime
 * P(k) <= k(ln k + ln ln k - 0.9427) for every k >= 15985
 *
 * According to the Nth prime page, the 15984th prime number is
 * 175,937
 */

inline ull max(ull a, ull b) { return (a > b ? a : b); }
inline ull min(ull a, ull b) { return (a < b ? a : b); }

#define DIV_BY_2(x) (x / 2)


ull ln_ceil(ull x) {
    // We find the minimum integral power to much e must be raised to *exceed* x:
    ull exp = 0;
    ld prod = MATH_E;

    while (prod < (ld)x) {
        ++exp;
        prod *= MATH_E;
    }

    return exp + 1;
}


/* Given some x, finds the minimum value s such that
 * s >= sqrt(x)
 */
ull sqrt_ceil(ull x) {
    for (ull s = 1; s < (x / 2) + 1; ++s)
        if (s * s >= x)
            return s;

    // Failure?
    return ULL_MAX;       // ?
}



/* Given some value k,
 * if P(k) denotes the kth prime number,
 * get_max_prime_bound(k) returns some value such that P(k) <= get_max_prime_bound(k)
 */
ull get_max_prime_bound(ull k) {
    if (k >= 15985)
        return k * (ln_ceil(k) + ln_ceil(ln_ceil(k)));
    else
        return 175937;
}


/* Allocates a sieve of Eratosthenes such that all numbers until max_val own a flag
 * in it.
 */
uint8_t *alloc_sieve(ull max_val) {
    return (uint8_t *)calloc(DIV_BY_2(max_val) / 8, 1);
}


uint8_t is_prime(uint8_t *sieve, ull num) {
    // Warning: does not handle cases where num is divisible by 2!
    ull index = DIV_BY_2(num);
    // We must access sieve[index / 8]'s (index % 8)th bit from the right
    return !(sieve[index / 8] & (1 << (index % 8)));
}


uint8_t mark_composite(uint8_t *sieve, ull num) {
    ull index = DIV_BY_2(num);
    sieve[index / 8] = sieve[index / 8] | (1 << (index % 8));
}


int main(int argc, char *argv[]) {
    printf("Nth prime finder!\n");
    printf("Please enter N: ");

    ull N;
    scanf("%llu", &N);

    printf("Working...\n");

    // Beginning timing:
    clock_t begin_clock = clock();

    // Getting the max bound, and allocating a sieve:
    ull max_bound = get_max_prime_bound(N);
    ull last_prime = 2;     // 2 is the only prime we take for granted.
    ull num_primes = 1;

    uint8_t *sieve = alloc_sieve(max_bound);

    for (ull i = 3; num_primes < N; i += 2) {
        if (is_prime(sieve, i)) {
            last_prime = i;
            ++num_primes;

            for (ull j = i; j <= max_bound / i; j += 2)
                mark_composite(sieve, i * j);
        }
    }

    clock_t end_clock = clock();

    printf("The %lluth prime is: %llu\n", N, last_prime);
    printf("That operation took %f CPU-seconds!\n", (double)(end_clock - begin_clock) / CLOCKS_PER_SEC);
}
