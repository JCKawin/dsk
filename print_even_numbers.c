/*
 * Helper program to display even numbers between 1 and 50.
 * Can be called using execl("./print_even_numbers", "print_even_numbers", NULL).
 *
 * Compile: gcc print_even_numbers.c -o print_even_numbers
 * Run:     ./print_even_numbers
 */

#include <stdio.h>

int main(void) {
    printf("Even numbers between 1 and 50:\n");
    for (int i = 1; i <= 50; i++) {
        if (i % 2 == 0) {
            printf("%d ", i);
        }
    }
    printf("\n");
    return 0;
}
