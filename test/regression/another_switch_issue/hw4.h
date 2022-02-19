#ifndef HW4_H
#define HW4_H

/* Constant definitions */

#define MAX_MANDELBROT (255)

/* Structure Declarations */

struct complex {
  double x;
  double y;
};

typedef struct complex complex_t;

/* Function Prototypes */

complex_t add_complex(complex_t, complex_t);
complex_t sub_complex(complex_t, complex_t);
complex_t mul_complex(complex_t, complex_t);
complex_t div_complex(complex_t, complex_t);
complex_t neg_complex(complex_t);
complex_t inv_complex(complex_t);
complex_t exp_complex(complex_t);
double dot_complex(complex_t, complex_t);
int mandelbrot(complex_t);

#endif // HW4_H
