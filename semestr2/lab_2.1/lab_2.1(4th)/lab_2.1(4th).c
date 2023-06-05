#include <stdio.h>
#include <math.h>

//iterable

double iterable(double x, unsigned int n) {
    double sum = 1;
    double res = 1;
    if (n > 1) {
        for (unsigned int i = 2; i <= n; i++) {
            res *= - x * x /(4 * (i - 1) * (i - 1) - 2 * (i - 1));
            sum += res;
        }
    }
    return sum;
}

int main() {
    double x;
    unsigned int n;
    printf(" Enter x: ");
    scanf("%lf", &x);
    printf(" Enter depth: ");
    scanf("%d", &n);
    double sum = iterable(x, n);
    double func = cos(x);
    printf("\n x: %.2lf\n depth: %d\n sum: %f\n cos(%.2lf) = %lf", x, n, sum, x, func);
    return 0;
}

