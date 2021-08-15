#include "complex.h"
#include "common.h"

/*All functions in the header*/

void read_comp(complex *cp, double a, double b)
{
    (*cp).a = a;
    (*cp).b = b;
}

/* Print complex Number function*/
void print_comp(complex *cp)
{
    printf("%s\n*SYSTEM: Print complex Number : ", LINE);
    printf("%.2f + %.2fi\n", (*cp).a, (*cp).b);
    printf("%s\n", LINE);
}

/*Calculate the sum between 2 complex*/
void add_comp(complex *cpA, complex *cpB)
{
    complex temp;
    temp.a = (*cpA).a + (*cpB).a;
    temp.b = (*cpA).b + (*cpB).b;
    printf("The result of the addition:\n");
    print_comp(&temp);
}

/*Calculate the subtract between 2 complex and print the result*/
void sub_comp(complex *cpA, complex *cpB)
{
    complex temp;
    temp.a = (*cpA).a - (*cpB).a;
    temp.b = (*cpA).b - (*cpB).b;

    printf("The result of the subtraction:\n");
    print_comp(&temp);
}

/*Multiple complex number with scalar and print the result*/
void mult_comp_real(complex *cp, double scalar)
{
    complex temp;
    temp.a = (*cp).a * scalar;
    temp.b = (*cp).b * scalar;
    printf("The result of the multiplication with scalar:\n");
    print_comp(&temp);
}

/*Multiple complex number with img number scalar and print the result*/
void mult_comp_img(complex *cp, double scalar)
{
    complex temp;
    temp.a = (*cp).a * scalar;
    temp.b = -(*cp).b * scalar;
    printf("The result of the multiplication with img scalar:\n");

    if (temp.a < 0)
        printf("%.2f + (%.2fi)\n", temp.b, temp.a);
    else
        printf("%.2f + %.2fi\n", temp.b, temp.a);
}

/*Multiple complex number with complex number and print the result*/
void mult_comp_comp(complex *cpA, complex *cpB)
{
    complex temp;
    temp.a = ((*cpA).a * (*cpB).a) - ((*cpA).b * (*cpB).b);
    temp.b = ((*cpA).a * (*cpB).b) + ((*cpA).b * (*cpB).a);

    printf("The result of the multiplication of two complex numbers:\n");
    print_comp(&temp);
}

/*Calculate the absolue value of complex number*/
void abs_comp(complex *cp)
{
    double absolue = sqrt((pow((*cp).a, 2)) + (pow((*cp).b, 2)));
    printf("The Absolute value of your variable is: %.2f\n", absolue);
}
