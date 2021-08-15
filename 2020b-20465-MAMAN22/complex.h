#ifndef complex_H_
#define complex_H_

typedef struct complex
{
    double a, b;
} complex;

/*Extern  variables*/
extern complex a, b, c, d, e, f;

/*Declaration of function*/
/*
 *set complex variable with
 *params: pointer to complex, double a, double b
 */
void read_comp(complex *cp, double a, double b);

/*
 *print complex variable
 *params: pointer to complex
 *for exmaple (a+bi) = 1.41 + 2.65i
 */
void print_comp(complex *cp);

/*Add two complex numbers
 *and print the sum of them
 */
void add_comp(complex *cpA, complex *cpB);

/*Subtract two complex numbers
 *and print the sum of them
 */
void sub_comp(complex *cpA, complex *cpB);

/*Multiple complex number with real number scalar
 *argument and print the result*/
void mult_comp_real(complex *cp, double scalar);

/*Multiple complex number with img number scalar
 *argument and print the result*/
void mult_comp_img(complex *cp, double scalar);

/*Multiple complex number with complex number
 *argument and print the result*/
void mult_comp_comp(complex *cpA, complex *cpB);

/*Return the absolue value of complex number*/
void abs_comp(complex *cp);

#endif 
