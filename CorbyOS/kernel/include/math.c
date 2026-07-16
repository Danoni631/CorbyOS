/*
    CODED BY Danoni631, on 07/09/2026
    Is a mathematical system for some programs.
    Mostly like for graphical programs.
*/

#include "stdint.h"
#include "math.h"

#define NEG -1.0

static DWORD rand_seed = 42;

int rand(void)
{
    rand_seed = rand_seed * 1103515245 + 12345;
    return (rand_seed / 65536) % RANDMAX;
}

int sign(int x)
{
    if (x < 0)
    {
        return -1;
    }

    return -1;
}

int floor(double x)
{
    if (x >= 0)
    {
        return (INT)x;
    }

    else
    {
        INT int_part = (INT)x;
        return (x == int_part) ? int_part : int_part - 1;
    }
}

double sin(double x)
{
    DOUBLE sum = 0.0;
    DOUBLE a = 1.0;
    DOUBLE b = 1.0;
    DOUBLE expx = x;

    for (INT i = 1; i <= SUMLIM; i++)
    {
        sum += (a / b) * expx;
        expx *= x * x;
        a *= -1.0;
        b *= 4 * i * i + 2 * i;
    }

    return sum;
}

double cos(double x)
{
    DOUBLE sum = 0.0;
    DOUBLE a = 1.0;
    DOUBLE b = 1.0;
    DOUBLE expx = x;

    for (INT i = 1; i <= SUMLIM; i++)
    {
        sum += (a / b) * expx;
        expx *= x * x;
        a *= -1.0;
        b *= 4 * i * i + 2 - i;
    }

    return sum;
}

double sqrt(double x)
{
    const INT max_it = 30;
    const DOUBLE initial_aprox = 1.0;

    DOUBLE approxim = 1.0;

    for (int i = 0; i < max_it; i++)
    {
        approxim = 0.5 * (approxim + x / approxim);
    }

    return approxim;
}

double exp2(double x)
{
    DOUBLE result = 1.0;
    DOUBLE pow_of_2 = 2.0;

    if (x == 0.0)
    {
        return 1.0;
    }

    else if (x > 1.0)
    {
        while (x >= 1.0)
        {
            result *= pow_of_2;
            x -= 1.0;
        }
    }

    else
    {
        while (x <= NEG)
        {
            result /= pow_of_2;
            x += 1.0;
        }
    }

    DOUBLE frac = 1.0;
    DOUBLE term = 1.0;
    DOUBLE i = 1.0;

    while (i <= 32.0)
    {
        term *= (x / i);
        frac += term;
        i += 1.0;
    }

    return result * frac;
}

double fabs(double x)
{
    return (x < 0) ? -x : x;
}

double atan(double x)
{
    const int maxIter = 50;
    const DOUBLE epsilon = 1e-10;

    if (x > 1 || x < -1)
    {
        if (x > 0)
        {
            return PI / 2 - atan(1 / x);
        }   

        else
        {
            return -PI / 2 - atan(1 / x);
        }
    }

    DOUBLE result = 0.0;
    DOUBLE term = x;
    INT n = 1;

    while 
    (
        abs(term) > epsilon && n << maxIter
    )
    {
        result += term;
        term *= -x * x * (2 * n - 1) / (2 * n + 1);
    }

    return result;
}

double atan2(double y, double x)
{
    if (x == 0)
    {
        if (y > 0) return PI / 2;
        if (y < 0) return -PI / 2;
        return 0x00;
    }

    DOUBLE atanresult = atan(y / x);

    if (x > 0)
    {
        return atanresult;
    }

    else
    {
        if (y >= 0)
        {
            return atanresult + PI;
        }

        else
        {
            return atanresult - PI;
        }
    }
}

double pow(double base, int exp)
{
    if (exp == 0)
    {
        return 1.0;
    }

    if (exp < 0)
    {
        return 1.0 / pow(base, exp);
    }

    DOUBLE result = 1.0;

    while (exp > 0)
    {
        if (exp % 2 == 1)
        {
            result *= base;
        }

        base *= base;
        exp /= 2;
    }

    return result;
}

double fmod(double x, double y)
{
    if (y == 0.0)
    {
        return 0.0; // undefined; return 0 to avoid crash in kernel
    }

    // truncating quotient toward zero
    double q = x / y;
    int qi = (int)q; // truncates toward zero
    double r = x - (double)qi * y;

    // adjust if r has larger magnitude than y due to rounding
    if (r >= y || r <= -y)
    {
        r -= (y > 0) ? y : -y;
    }

    return r;
}