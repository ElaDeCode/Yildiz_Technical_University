/* written by ali_özyalçın */

#include <stdio.h>
#include <math.h>

/********* adjustment section **************/
/* this value defines how many functions can be added */
#define MAX_POLYNOMIAL 20
#define MAX_EXPONENTIAL 10
#define MAX_LOGARITHMIC 10
#define MAX_TRIGONOMETRIC 10
#define MAX_INVERSE_TRIGONOMETRIC 10
/* how many component can a matrix take */
#define MAX_HEIGHT 15
#define MAX_WIDTH 15
/* this is a precision value for calculation */
#define DELTA 1e-10
/************************************************/
/* data types and structs */

/* logatihmic and inverse trigonometric Function has same arguments as
exponential and trigonometric so i used a macro for readability */
#define Logarithmic Exponential
#define Inverse_trigonometric Trigonometric

typedef struct
{
    double
        x_coef,
        x_exp;
} Polynomial;

typedef struct
{
    double
        x_coef,
        x_exp,
        fn_coef,
        fn_exp,
        base;
} Exponential;

typedef struct
{
    unsigned int trig_fn;
    double
        x_coef,
        x_exp,
        fn_coef,
        fn_exp;
} Trigonometric;

/*with Function class it is easier to pass all Function at the same time*/
typedef struct
{
    unsigned int
        polynomial_count,
        exponential_count,
        logarithmic_count,
        trigonometric_count,
        inverse_trigonometric_count;

    Polynomial polynomial[MAX_POLYNOMIAL];
    Exponential exponential[MAX_EXPONENTIAL];
    Logarithmic logarithmic[MAX_LOGARITHMIC];
    Trigonometric trigonometric[MAX_TRIGONOMETRIC];
    Inverse_trigonometric inverse_trigonometric[MAX_INVERSE_TRIGONOMETRIC];
} Function;

/* main methods (branch of main)*/
void bisection();
void regula_falsi();
void newton_raphson();
void inverse_of_matrix();
void gauss_elimination();
void gauss_seidel();
void numerical_derivative();
void simpson();
void trapez();
void gregory_newton();

/* input and input check functions */
void print_array(double array[], int n);
void print_function(Function func);
void print_matrix(double matrix[][MAX_WIDTH], int n, int m);
/*void set_array(double arr[], int n, double value);*/
void take_array(double array[], int n);
void take_function(Function *func);
void take_interval(Function func, double *a, double *b);
void take_matrix(double matrix[][MAX_WIDTH], int height, int width);

/* swaps 2 double value */
void swap_double(double *a, double *b);

/* calculation functions */

double calculate_derivative(Function func, double x);
double calculate_function(Function func, double x);
int check_root(Function func, double start, double end);

/* matrix operation related functions */

int check_non_zero_diagonal(double matrix[][MAX_WIDTH], int n);
double determinant(double matrix[][MAX_WIDTH], int n);
void divide_row_by_a(double row[], int n, double a);
void eliminate_zeros_on_diagonal(double matrix[][MAX_WIDTH], double comatrix[][MAX_WIDTH], int n);
void invert_matrix_using_sample(double sample[][MAX_WIDTH], int n, double output[][MAX_WIDTH]);
void make_diagonal_product_max(double matrix[][MAX_WIDTH], double result[], int n);
void make_unit_matrix(double matrix[][MAX_WIDTH], int n);
void swap_row(double matrix[][MAX_WIDTH], int n, int row_a, int row_b);
void substract_x_times_row_b_from_row_a(double matrix[][MAX_WIDTH], int n, int row_a, int row_b, double x);

/* branches of calculate function */
double calculate_polynomial(Polynomial func, double x);
double calculate_exponential(Exponential func, double x);
double calculate_logarithmic(Logarithmic func, double x);
double calculate_trigonometric(Trigonometric func, double x);
double calculate_inverse_trigonometric(Inverse_trigonometric func, double x);

/* branches of take function */
/* these functions return how many functions have been entered */
int take_polynomial(Polynomial func[]);
int take_exponential(Exponential func[]);
int take_logarithmic(Logarithmic func[]);
int take_trigonometric(Trigonometric func[]);
int take_inverse_trigonometric(Inverse_trigonometric func[]);

int main()
{
    int input = -1;
    do
    {
        printf("\n"
               " 1 - Bisection\n"
               " 2 - Regula-Falsi\n"
               " 3 - Newton-Rapshon\n"
               " 4 - NxN'lik bir matrisin tersi\n"
               " 5 - Gauss Eleminasyon\n"
               " 6 - Gauss Seidal yontemleri\n"
               " 7 - Sayisal Turev (merkezi, ileri ve geri farklar opsiyonlu)\n"
               " 8 - Simpson\n"
               " 9 - Trapez\n"
               "10 - Degisken donusumsuz Gregory newton Enterpolasyonu\n\n"
               " 0 - Programi kapat\n"
               "yontem seciniz:  ");
        scanf("%d", &input);
        printf("\n");

        switch (input)
        {
        case 1:
            bisection();
            break;
        case 2:
            regula_falsi();
            break;
        case 3:
            newton_raphson();
            break;
        case 4:
            inverse_of_matrix();
            break;
        case 5:
            gauss_elimination();
            break;
        case 6:
            gauss_seidel();
            break;
        case 7:
            numerical_derivative();
            break;
        case 8:
            simpson();
            break;
        case 9:
            trapez();
            break;
        case 10:
            gregory_newton();
            break;
        case 0:
            break;
        default:
            printf("gecerli bir yontem giriniz\n");
            break;
        }
    } while (input != 0);
    return 0;
}

void bisection()
{
    Function func;
    unsigned int max_iterations;
    double epsilon, a, b, c, fc; /* abc points fc = function vaule of c */
    int i = 1;

    take_function(&func);

    take_interval(func, &a, &b);
    if (check_root(func, a, b) == 0)
        return;

    printf("hata miktari: ");
    scanf("%lf", &epsilon);
    if (epsilon < 0)
        epsilon = -epsilon;

    printf("maksimum deneme sayisi: ");
    scanf("%u", &max_iterations);

    /* bisection main */
    if (calculate_function(func, a) == 0)
        c = a;
    else if (calculate_function(func, b) == 0)
        c = b;
    else
        do
        {
            /* c = middle point of new a and b */
            c = (a + b) / 2;

            /* setting new interval */
            if (check_root(func, a, c))
                b = c;
            else
                a = c;

            fc = calculate_function(func, c);
            printf("%d. iterasyon => f(%lf) = %lf\n", i, c, fc);
        } while (i++ < max_iterations && fabs(fc) > epsilon && fc != 0);

    if (i >= max_iterations)
    {
        printf("maksimum iterasyon sayisi asildi\n"
               "son deger: ");
    }

    printf("%lf\n", c);
}

void regula_falsi()
{
    Function func;
    unsigned int max_iterations;
    double epsilon, a, b, c;
    int i = 0;
    double fa, fb, fc;

    take_function(&func);
    take_interval(func, &a, &b);
    if (check_root(func, a, b) == 0)
        return;

    printf("hata miktari: ");
    scanf("%lf", &epsilon);
    if (epsilon < 0)
        epsilon = -epsilon;

    printf("maksimum deneme sayisi: ");
    scanf("%u", &max_iterations);

    fa = calculate_function(func, a);
    fb = calculate_function(func, b);
    fc = fa; /* initial value */

    /* checking if a or b is root */
    if (fa == 0)
        c = a, fc = fa;
    else if (fb == 0)
        c = b, fc = fb;
    else
        while (i++ < max_iterations && fabs(fc) > epsilon && fc != 0)
        {
            /* regula falsi formula */
            c = (b * fa - a * fb) / (fa - fb);
            fc = calculate_function(func, c);
            printf("%d. iterasyon => f(%lf) = %lf\n", i, c, fc);

            /* setting new interval */
            if (fa * fc < 0)
            {
                b = c;
                fb = calculate_function(func, b);
            }
            else
            {
                a = c;
                fa = calculate_function(func, a);
            }
        }

    if (i >= max_iterations)
    {
        printf("maksimum iterasyon sayisi asildi\n"
               "son deger: ");
    }

    printf("%lf\n", c);
}

void newton_raphson()
{
    Function func;
    unsigned int max_iterations;
    double epsilon, c;
    int i = 1;
    double fc, fdc; /* fdc = f'(c) */

    take_function(&func);
    do
    {
        printf("baslangic noktasi: ");
        scanf("%lf", &c);

        printf("hata miktari: ");
        scanf("%lf", &epsilon);
        if (epsilon < 0)
            epsilon = -epsilon;

        printf("maksimum deneme sayisi: ");
        scanf("%u", &max_iterations);

        do
        {
            /* return if derivative = 0*/
            fdc = calculate_derivative(func, c);
            if (fdc == 0)
            {
                printf("iraksiyor (türev = 0)\n");
                return;
            }
            /* newton raphson new point formula */
            c = c - fc / fdc;

            fc = calculate_function(func, c);
            printf("%d. iterasyon => f(%lf) = %lf -------- f'(c) = %lf\n", i, c, fc, fdc);
        } while (i++ < max_iterations && fabs(fc) > epsilon && fc != 0);

        if (i >= max_iterations)
        {
            printf("maksimum iterasyon sayisi asildi\n"
                   "son deger: ");
        }

        printf("%lf\n", c);
        printf("ayni fonksiyonu kullanmak icin 0 giriniz: ");
        scanf("%d", &i);
    } while (i == 0);
}

void inverse_of_matrix()
{
    double matrix[MAX_HEIGHT][MAX_WIDTH] = {0};
    double inverse[MAX_HEIGHT][MAX_WIDTH] = {0};
    int n;

    printf("matrisin boyutu (NxN): ");
    scanf("%d", &n);

    take_matrix(matrix, n, n);

    printf("matris: \n");
    print_matrix(matrix, n, n);

    if (fabs(determinant(matrix, n)) < 0.00001)
    {
        printf("matrisin tersi yok\n");
        return;
    }

    if (check_non_zero_diagonal(matrix, n) != -1)
    {
        printf("köşegende 0 bulundu...\n");
        eliminate_zeros_on_diagonal(matrix, inverse, n);
        printf("köşegendeki 0lar elendi: \n");
        print_matrix(matrix, n, n);
    }

    make_unit_matrix(inverse, n);

    invert_matrix_using_sample(matrix, n, inverse);

    print_matrix(inverse, n, n);
}

void gauss_elimination()
{
    double matrix[MAX_HEIGHT][MAX_WIDTH] = {0};
    double result[MAX_HEIGHT] = {0};
    int n;
    int i, j, k;

    printf("matrisin boyutu (NxN): ");
    scanf("%d", &n);
    take_matrix(matrix, n, n);
    printf("sutun matrisini giriniz: \n");
    take_array(result, n);

    /* eliminate zeros */
    i = check_non_zero_diagonal(matrix, n);
    while (i != -1 && k++ < 10)
    {
        j = i + 1;
        while (matrix[j % n][i] == 0)
        {
            j++;
        }
        swap_row(matrix, n, i, j);
        swap_double(&result[i], &result[j]);
        i = check_non_zero_diagonal(matrix, n);
    }

    /* check if still has zero on diagonal */
    if (check_non_zero_diagonal(matrix, n) != -1)
    {
        printf("kosegendeki 0 lar elenemedi");
        return;
    }

    /* eliminate lower triangle */
    for (i = 0; i < n; i++)
    {
        result[i] /= matrix[i][i];
        divide_row_by_a(matrix[i], n, matrix[i][i]);
        for (j = i + 1; j < n; j++)
        {
            result[j] -= result[i] * matrix[j][i];
            substract_x_times_row_b_from_row_a(matrix, n, j, i, matrix[j][i]);
            print_matrix(matrix, n, n);
        }
    }

    /* eliminate upper triangle*/
    for (i = 1; i < n; i++)
        for (j = 0; j < i; j++)
        {
            result[j] -= result[i] * matrix[j][i];
            substract_x_times_row_b_from_row_a(matrix, n, j, i, matrix[j][i]);
            print_matrix(matrix, n, n);
        }
    printf("\n");
    print_array(result, n);
}

void gauss_seidel()
{
    double matrix[MAX_HEIGHT][MAX_WIDTH] = {0};
    double result[MAX_HEIGHT] = {0};
    double variables[MAX_WIDTH] = {0};
    double epsilon = 0, error = 0, max_error = 0;
    double temp = 0;
    unsigned int iterations = 0, max_iterations = 0;
    int i = 0, j = 0;
    int n = 0;

    printf("matrisin genisligi: ");
    scanf("%d", &n);
    take_matrix(matrix, n, n);
    print_matrix(matrix, n, n);

    printf("sutun matrisini giriniz\n");
    take_array(result, n);
    print_array(result, n);

    printf("\n");
    for (i = 0; i < n; i++)
    {
        printf("%d. degiskenin baslangic noktasi: ", i + 1);
        scanf("%d", &variables[i]);
    }

    printf("hata miktari: ");
    scanf("%lf", &epsilon);
    if (epsilon < 0)
        epsilon = -epsilon;

    printf("maksimum iterasyon sayisi: ");
    scanf("%u", &max_iterations);

    make_diagonal_product_max(matrix, result, n);

    do
    {
        max_error = 0;
        for (i = 0; i < n; i++)
        {
            temp = 0;
            /* sum of the values of variables multiplied coefficients */
            for (j = 0; j < n; j++)
                temp += matrix[i][j] * variables[j];
            /* substract diagonel because we dont need it */
            temp -= matrix[i][i] * variables[i];

            error = variables[i];                             /* error = old variable value - new value so we save the old value just before change it*/
            variables[i] = (result[i] - temp) / matrix[i][i]; /* update variable value */
            error = fabs(error - variables[i]);

            /* we'll continue until all of the errors become zero */
            if (error > max_error)
                max_error = error;
        }
        printf("%u. iterasyon =>\t", iterations);
        print_array(variables, n);
    } while (iterations++ <= max_iterations && max_error > epsilon);

    if (i >= max_iterations)
        printf("maksimum iterasyon sayisi asildi\n");
}

void numerical_derivative()
{
    Function func;
    double func_derivative;
    double x, h;
    int method = -1;

    take_function(&func);

    do
    {
        do
        {
            printf("metod seciniz\n"
                   "1- merkezi fark ile turev\n"
                   "2- ileri fark ile turev\n"
                   "3- geri fark ile turev\n"
                   "0- cikis\n"
                   "metod: ");
            scanf("%d", &method);
        } while (method < 0 || method > 3);

        if (method == 0)
            return;

        printf("turevi bulunacak deger: ");
        scanf("%lf", &x);

        printf("fark operatorunun degeri: ");
        scanf("%lf", &h);

        switch (method)
        {
        case 1:
            func_derivative = (calculate_function(func, x + h) - calculate_function(func, x - h)) / (2 * h);
            break;
        case 2:
            func_derivative = (calculate_function(func, x + h) - calculate_function(func, x)) / h;
            break;
        case 3:
            func_derivative = (calculate_function(func, x) - calculate_function(func, x - h)) / h;
            break;
        default:
            return;
        }
        printf("%lf\n", func_derivative);

        printf("ayni fonksiyonu kullanmak icin 0 giriniz: ");
        scanf("%d", &method);
    } while (method == 0);
}

void simpson() /* 1/3 only */
{
    Function func;
    unsigned int number_of_segments;
    double start, end;
    double segment_size, temp = 0, total_area = 0;
    int i, limit;
    take_function(&func);

    printf("baslangic: ");
    scanf("%lf", &start);
    printf("bitis: ");
    scanf("%lf", &end);

    do
    {
        printf("aralik sayisi (cift sayi olmalidir): ");
        scanf("%u", &number_of_segments);
    } while (number_of_segments % 2 != 0);

    segment_size = (end - start) / number_of_segments;

    /* first and last values are not going to multiplied by the same value as others */
    total_area = calculate_function(func, start);
    total_area += calculate_function(func, end);

    /* odd segments are multiplied by 4 so I first add them all then multiply */
    for (temp = 0, limit = number_of_segments - 1, i = 1; i <= limit; i += 2)
        temp += calculate_function(func, start + i * segment_size);
    total_area += 4 * temp;

    /* even segments are multiplied by 2 so I first add them all then multiply */
    for (temp = 0, limit = number_of_segments - 2, i = 2; i <= limit; i += 2)
        temp += calculate_function(func, start + i * segment_size);
    total_area += 2 * temp;

    /* its 1/3 rule so ill divide by 3 */
    total_area = total_area * segment_size / 3;

    printf("%lf\n", total_area);
}

void trapez()
{
    Function func;
    unsigned int number_of_segments;
    double start, end;
    double segment_size, total_area;
    int i;
    take_function(&func);

    printf("baslangic: ");
    scanf("%lf", &start);
    printf("bitis: ");
    scanf("%lf", &end);

    printf("aralik sayisi: ");
    scanf("%u", &number_of_segments);

    segment_size = (end - start) / number_of_segments;

    total_area = 0;
    /* sum of segment areas */
    /* i did not want to multiply them by 2 then divide by 2 so I only divide the first and last element because their coefficient is half of others */
    for (i = 1; i < number_of_segments - 1; i++)
        total_area += calculate_function(func, start + i * segment_size);
    total_area += (calculate_function(func, start) + calculate_function(func, end)) / 2;
    total_area = total_area * segment_size;

    printf("%lf\n", total_area);
}

void gregory_newton()
{
    double points[MAX_WIDTH];
    double first_point;
    double space;
    unsigned int len;
    double x;
    double temp;
    double ans;
    int i, j;

    printf("girilecek nokta sayisi: ");
    scanf("%u", &len);

    printf("\nilk noktanin x degeri: ");
    scanf("%lf", &first_point);

    printf("\nx degerleri arasindaki bosluklar ne kadar: ");
    scanf("%lf", &space);

    printf("\nsirasiyle y degerlerini giriniz\n");
    take_array(points, len);

    printf("\n\n");
    for (i = 0; i < len; i++)
        printf("%lf\t|\t", first_point + i * space);
    printf("\n");
    print_array(points, len);

    printf("\nhangi noktanin degeri bulunmali: ");
    scanf("%lf", &x);
    printf("\n");

    ans = points[0];
    temp = 1, i = 0;
    while (is_all_same(points, len) == 0 && len > 1)
    {
        len--;
        for (j = 0; j < len; j++)
        {
            points[j] = points[j + 1] - points[j];
        }
        print_array(points, len);
        /* temp is used to calculate the regularly varying coefficient */
        temp = temp * (x - (first_point + (i * space))) / ((i + 1) * space);
        /* temp * delta added to answer variable */
        ans += temp * points[0];
        i++;
    }

    printf("\n%lf\n", ans);
}

int is_all_same(double *array, int len)
{
    len--;
    while (len > 0)
    {
        if (array[len] != array[len - 1])
            return 0;
        len--;
    }
    return 1;
}

double determinant(double matrix[][MAX_WIDTH], int n)
{
    int i, j;
    double det_component_a = 1, det_component_b = 1, det = 0;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
            det_component_a = det_component_a * matrix[(i + j) % n][j];

        for (j = 0; j < n; j++)
            det_component_b = det_component_b * matrix[(i + j) % n][n - 1 - j];

        det += det_component_a - det_component_b;
        det_component_a = det_component_b = 1;
    }
    return det;
}

void take_matrix(double matrix[][MAX_WIDTH], int height, int width)
{
    int i, j;
    for (i = 0; i < height; i++)
        for (j = 0; j < width; j++)
        {
            printf("%d. satir %d. sutun elemani: ", i + 1, j + 1);
            scanf("%lf", &matrix[i][j]);
        }
}

void take_array(double array[], int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        printf("%d: ", i + 1);
        scanf("%lf", &array[i]);
    }
}

int check_non_zero_diagonal(double matrix[][MAX_WIDTH], int n)
{
    int i;
    for (i = 0; i < n; i++)
        if (matrix[i][i] == 0)
            return i;
    return -1;
}

void swap_double(double *a, double *b)
{
    double temp = *a;
    *a = *b;
    *b = temp;
}

void swap_row(double matrix[][MAX_WIDTH], int n, int row_a, int row_b)
{
    int i;
    for (i = 0; i < n; i++)
    {
        swap_double(&matrix[row_a][i], &matrix[row_b][i]);
    }
}

void divide_row_by_a(double row[], int n, double a)
{
    int i;
    for (i = 0; i < n; i++)
    {
        row[i] /= a;
    }
}
void substract_x_times_row_b_from_row_a(double matrix[][MAX_WIDTH], int n, int row_a, int row_b, double x)
{
    int i;
    for (i = 0; i < n; i++)
    {
        matrix[row_a][i] -= matrix[row_b][i] * x;
    }
}

void eliminate_zeros_on_diagonal(double matrix[][MAX_WIDTH], double comatrix[][MAX_WIDTH], int n)
{
    int i = 0, j = 0, k = 0;
    i = check_non_zero_diagonal(matrix, n);
    while (i != -1 && k++ < 10)
    {
        j = i + 1;
        while (matrix[j % n][i] == 0)
        {
            j++;
        }
        swap_row(matrix, n, i, j);
        swap_row(comatrix, n, i, j);
        i = check_non_zero_diagonal(matrix, n);
    }
}

void make_unit_matrix(double matrix[][MAX_WIDTH], int n)
{
    int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            matrix[i][j] = i == j ? 1 : 0;
}

void invert_matrix_using_sample(double sample[][MAX_WIDTH], int n, double output[][MAX_WIDTH])
{
    int i, j;
    /* eliminate below of diagonal while applying transfors to output */
    for (i = 0; i < n; i++)
    {
        divide_row_by_a(output[i], n, sample[i][i]);
        for (j = i + 1; j < n; j++)
            substract_x_times_row_b_from_row_a(output, n, j, i, sample[j][i]);
        divide_row_by_a(sample[i], n, sample[i][i]);
        for (j = i + 1; j < n; j++)
            substract_x_times_row_b_from_row_a(sample, n, j, i, sample[j][i]);
    }

    /* eliminate above of the diagonel while applying the transforms to the */
    for (i = 1; i < n; i++)
    {
        for (j = 0; j < i; j++)
        {
            substract_x_times_row_b_from_row_a(output, n, j, i, sample[j][i]);
            substract_x_times_row_b_from_row_a(sample, n, j, i, sample[j][i]);
        }
    }
}

void make_diagonal_product_max(double matrix[][MAX_WIDTH], double result[], int n)
{
    int i = 0, j = 0;
    int height_of_max = 0;
    for (i = 0; i < n; i++)
    {
        /*finds max*/
        height_of_max = i;
        for (j = i + 1; j < n; j++)
            if (matrix[j][i] > matrix[height_of_max][i])
                height_of_max = j;
        /* move max to diagonal */
        if (height_of_max != i)
        {
            swap_row(matrix, n, i, height_of_max);
            swap_double(&result[i], &result[height_of_max]);
            print_matrix(matrix, n, n);
            print_array(result, n);
            printf("\n");
        }
    }
}

void print_matrix(double matrix[][MAX_WIDTH], int n, int m)
{
    int i, j;
    printf("\n\n");
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            printf("%lf ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
}

void print_array(double array[], int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        printf("%lf\t|\t", array[i]);
    }
    printf("\n");
}

void take_interval(Function func, double *a, double *b)
{
    char input;
    do
    {
        input = 'a';
        printf("baslangic noktasi: ");
        scanf("%lf", a);
        printf("bitis noktasi: ");
        scanf("%lf", b);

        if (check_root(func, *a, *b) == 0)
        {
            printf("aralikta kok yok\n"
                   "yeniden aralik girmek icin 'e' girin: ");
            scanf(" %c", &input);
        }
    } while (input == 'e' || input == 'E');
    return;
}

int check_root(Function func, double start, double end)
{
    if (calculate_function(func, start) * calculate_function(func, end) <= 0)
        return 1;
    else
        return 0;
}

double calculate_derivative(Function func, double x)
{
    return (calculate_function(func, x + DELTA) - calculate_function(func, x - DELTA)) / (2 * DELTA);
}

void take_function(Function *func)
{
    func->polynomial_count = take_polynomial(func->polynomial);
    func->exponential_count = take_exponential(func->exponential);
    func->logarithmic_count = take_logarithmic(func->logarithmic);
    func->trigonometric_count = take_trigonometric(func->trigonometric);
    func->inverse_trigonometric_count = take_inverse_trigonometric(func->inverse_trigonometric);
    print_function(*func);
}

void print_function(Function func)
{
    int i;
    char *trig_choices[] = {"sin",
                            "cos",
                            "tan",
                            "cot"};

    char *in_trig_choices[] = {"arcsin",
                               "arccos",
                               "acrtan",
                               "arccot"};
    printf("\n\n");
    for (i = 0; i < func.polynomial_count; i++)
    {
        printf("%lf * x ^ %lf + ",
               func.polynomial[i].x_coef,
               func.polynomial[i].x_exp);
    }
    for (i = 0; i < func.exponential_count; i++)
    {
        printf("%lf * %lf ^ (%lf * x ^ %lf) ^ %lf + ",
               func.exponential[i].fn_coef,
               func.exponential[i].base,
               func.exponential[i].x_coef,
               func.exponential[i].x_exp,
               func.exponential[i].fn_exp);
    }
    for (i = 0; i < func.logarithmic_count; i++)
    {
        printf("%lf * log(%lf,(%lf * x ^ %lf)) ^ %lf + ",
               func.logarithmic[i].fn_coef,
               func.logarithmic[i].base,
               func.logarithmic[i].x_coef,
               func.logarithmic[i].x_exp,
               func.logarithmic[i].fn_exp);
    }
    for (i = 0; i < func.trigonometric_count; i++)
    {
        printf("%lf * %s(%lf * x ^ %lf) ^ %lf + ",
               func.trigonometric[i].fn_coef,
               trig_choices[func.trigonometric[i].trig_fn],
               func.trigonometric[i].x_coef,
               func.trigonometric[i].x_exp,
               func.trigonometric[i].fn_exp);
    }
    for (i = 0; i < func.inverse_trigonometric_count; i++)
    {
        printf("%lf * %s(%lf * x ^ %lf) ^ %lf + ",
               func.inverse_trigonometric[i].fn_coef,
               in_trig_choices[func.inverse_trigonometric[i].trig_fn],
               func.inverse_trigonometric[i].x_coef,
               func.inverse_trigonometric[i].x_exp,
               func.inverse_trigonometric[i].fn_exp);
    }
    printf("\n");
}

double calculate_function(Function func, double x)
{
    int i;
    double ans = 0;
    for (i = 0; i < func.polynomial_count; i++)
        ans += calculate_polynomial(func.polynomial[i], x);
    for (i = 0; i < func.exponential_count; i++)
        ans += calculate_exponential(func.exponential[i], x);
    for (i = 0; i < func.logarithmic_count; i++)
        ans += calculate_logarithmic(func.logarithmic[i], x);
    for (i = 0; i < func.trigonometric_count; i++)
        ans += calculate_trigonometric(func.trigonometric[i], x);
    for (i = 0; i < func.inverse_trigonometric_count; i++)
        ans += calculate_inverse_trigonometric(func.inverse_trigonometric[i], x);
    return ans;
}

int take_polynomial(Polynomial func[])
{
    int i;
    int count;
    printf("\nkatsayi * x ^ us\n\n"
           "polinom eleman sayisi: ");
    scanf("%d", &count);
    for (i = 0; i < count; i++)
    {
        printf("x katsayisi: ");
        scanf("%lf", &func[i].x_coef);
        printf("x ustu: ");
        scanf("%lf", &func[i].x_exp);
        printf("eklendi: %lf * x ^ %lf\n",
               func[i].x_coef,
               func[i].x_exp);
    }
    return count;
}

int take_exponential(Exponential func[])
{
    int i;
    int count;
    printf("\nf katsayi * (taban ^ (x katsayi * x ^ x us)) ^ f us\n\n"
           "ustel eleman sayisi: ");
    scanf("%d", &count);
    for (i = 0; i < count; i++)
    {
        printf("Fonksyonun katsayisi:  ");
        scanf("%lf", &func[i].fn_coef);

        printf("Ustel ifadenin tabani: ");
        scanf("%lf", &func[i].base);

        printf("x katsayisi: ");
        scanf("%lf", &func[i].x_coef);

        printf("x ustu: ");
        scanf("%lf", &func[i].x_exp);

        printf("Fonksyonun ustu: ");
        scanf("%lf", &func[i].fn_exp);

        printf("eklendi: %lf * %lf ^ (%lf * x ^ %lf) ^ %lf\n",
               func[i].fn_coef,
               func[i].base,
               func[i].x_coef,
               func[i].x_exp,
               func[i].fn_exp);
    }
    return count;
}

int take_logarithmic(Logarithmic func[])
{
    int i;
    int count;
    printf("\nf katsayi * log(taban,x katsayi * x ^ x us) ^ f us\n\n"
           "logaritmik eleman sayisi: ");
    scanf("%d", &count);
    for (i = 0; i < count; i++)
    {
        printf("Fonksyonun katsayisi: ");
        scanf("%lf", &func[i].fn_coef);

        printf("Logaritmik ifadenin tabani: ");
        scanf("%lf", &func[i].base);

        printf("x katsayisi: ");
        scanf("%lf", &func[i].x_coef);

        printf("x ustu: ");
        scanf("%lf", &func[i].x_exp);

        printf("Fonksyonun ustu: ");
        scanf("%lf", &func[i].fn_exp);

        printf("eklendi: %lf * log(%lf,(%lf * x ^ %lf)) ^ %lf\n",
               func[i].fn_coef,
               func[i].base,
               func[i].x_coef,
               func[i].x_exp,
               func[i].fn_exp);
    }

    return count;
}

int take_trigonometric(Trigonometric func[])
{
    int i;
    int count;
    char *choices[] = {"sin",
                       "cos",
                       "tan",
                       "cot"};

    printf("\nf katsayi * trig f( x katsayi * x ^ x us ) ^ f us\n\n"
           "trigonometrik eleman sayisi: ");
    scanf("%d", &count);
    for (i = 0; i < count; i++)
    {

        printf("Fonksyonun katsayisi: ");
        scanf("%lf", &func[i].fn_coef);

        do
        {
            printf(
                "lutfen fonksyon tipini seciniz\n"
                "0- sin\n"
                "1- cos\n"
                "2- tan\n"
                "3- cot\n"
                "tip: ");
            scanf("%d", &func[i].trig_fn);
        } while (func[i].trig_fn > 3);

        printf("x katsayisi: ");
        scanf("%lf", &func[i].x_coef);

        printf("x ustu: ");
        scanf("%lf", &func[i].x_exp);

        printf("Fonksyonun ustu: ");
        scanf("%lf", &func[i].fn_exp);

        printf("eklendi: %lf * %s(%lf * x ^ %lf) ^ %lf\n",
               func[i].fn_coef,
               choices[func[i].trig_fn],
               func[i].x_coef,
               func[i].x_exp,
               func[i].fn_exp);
    }

    return count;
}

int take_inverse_trigonometric(Inverse_trigonometric func[])
{
    int count;
    int i;
    char *choices[] = {"arcsin",
                       "arccos",
                       "acrtan",
                       "arccot"};

    printf("\nf katsayi * trig f( x katsayi * x ^ x us ) ^ f us\n\n"
           "ters trigonometrik eleman sayisi: ");
    scanf("%d", &count);
    for (i = 0; i < count; i++)
    {
        printf("Fonksyonun katsayisi: ");
        scanf("%lf", &func[i].fn_coef);

        do
        {
            printf(
                "lutfen fonksyon tipini seciniz\n"
                "0- arcsin\n"
                "1- arccos\n"
                "2- acrtan\n"
                "3- arccot\n"
                "tip: ");
            scanf("%d", &func[i].trig_fn);
        } while (func[i].trig_fn > 3);

        printf("x katsayisi: ");
        scanf("%lf", &func[i].x_coef);

        printf("x ustu: ");
        scanf("%lf", &func[i].x_exp);

        printf("Fonksyonun ustu: ");
        scanf("%lf", &func[i].fn_exp);

        printf("eklendi: %lf * %s(%lf * x ^ %lf) ^ %lf\n",
               func[i].fn_coef,
               choices[func[i].trig_fn],
               func[i].x_coef,
               func[i].x_exp,
               func[i].fn_exp);
    }
    return count;
}

double calculate_polynomial(Polynomial func, double x)
{
    return func.x_coef * pow(x, func.x_exp);
}

double calculate_exponential(Exponential func, double x)
{
    return func.fn_coef * pow(pow(func.base, func.x_coef * pow(x, func.x_exp)), func.fn_exp);
}

double calculate_logarithmic(Logarithmic func, double x)
{
    return func.fn_coef * pow(
                              log(func.x_coef * pow(x, func.x_exp)) / log(func.base) /* ln(x) / ln(base) = log(base,x)*/
                              ,
                              func.fn_exp);
}

double calculate_trigonometric(Trigonometric func, double x)
{
    double ans = func.x_coef * pow(x, func.x_exp);
    switch (func.trig_fn)
    {
    case 0:
        ans = sin(ans);
        break;
    case 1:
        ans = cos(ans);
        break;
    case 2:
        ans = tan(ans);
        break;
    case 3:
        ans = 1 / tan(ans);
        break;
    }
    return func.fn_coef * pow(ans, func.fn_exp);
}

double calculate_inverse_trigonometric(Inverse_trigonometric func, double x)
{
    double ans = func.x_coef * pow(x, func.x_exp);
    switch (func.trig_fn)
    {
    case 0:
        ans = asin(ans);
        break;
    case 1:
        ans = acos(ans);
        break;
    case 2:
        ans = atan(ans);
        break;
    case 3:
        ans = atan(1 / ans);
        break;
    }
    return func.fn_coef * pow(ans, func.fn_exp);
}
