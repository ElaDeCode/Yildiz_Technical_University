/* written by ali_özyalçın */

/****************************

            to do

diagonal max needed for jacobi
still need a lot of tests

*****************************/

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
#define DELTA 1e-15

/************************************************/
/* data types and structs */

/* bool definitions for readability */
#define bool char
#define true 1
#define false 0

/* logatihmic and inverse trigonometric Function has same arguments as
exponential and trigonometric so i used a macro for readability */
#define Logarithmic Exponential
#define Inverse_trigonometric Trigonometric

typedef struct
{
    double x, y;
} Vector2;

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
void gauss_seidal();
void numerical_derivative();
void simpson();
void trapez();
void gregory_newton();

void swap_double(double *a, double *b);

/* input and input check functions */
void print_array(double array[], int n);
void print_matrix(double matrix[][MAX_WIDTH], int n, int m);
void set_array(double arr[], int n, double value);
void take_array(double array[], int n);
void take_function(Function *func);
void take_interval(Function func, double *a, double *b);
void take_matrix(double matrix[][MAX_WIDTH], int height, int width);
void take_vector2_array(Vector2 vector2_arr[], int n);

/* calculation functions */
double calculate_derivative(Function func, double x);
double calculate_function(Function func, double x);
bool check_root(Function func, double start, double end);

/* matrix related functions */
int check_non_zero_diagonal(double matrix[][MAX_WIDTH], int n);
double determinant(double matrix[][MAX_WIDTH], int n);
void divide_row_by_a(double row[], int n, double a);
void eliminate_zeros_on_diagonal(double matrix[][MAX_WIDTH], int n);
void invert_matrix_using_sample(double sample[][MAX_WIDTH], int n, double output[][MAX_WIDTH]);
void make_diagonal_product_max(double matrix[][MAX_WIDTH], int n);
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
        printf(
            "1-\tBisection\n"
            "2-\tRegula-Falsi\n"
            "3-\tNewton-Rapshon\n"
            "4-\tNxN'lik bir matrisin tersi\n"
            "5-\tGauss Eleminasyon\n"
            "6-\tGauss Seidal yontemleri\n"
            "7-\tSayisal Turev (merkezi, ileri ve geri farklar opsiyonlu)\n"
            "8-\tSimpson\n"
            "9-\tTrapez\n"
            "10-\tDegisken donusumsuz Gregory newton Enterpolasyonu\n"
            "\n0-\tProgrami kapat\n"
            "yontem seciniz:  ");
        scanf("%d", &input);

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
            gauss_seidal();
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
    double epsilon, a, b, c, fc;
    int i = 1;

    take_function(&func);
    take_interval(func, &a, &b);

    printf("hata miktari: ");
    scanf("%lf", &epsilon);
    if (epsilon < 0)
        epsilon = -epsilon;

    printf("maksimum deneme sayisi: ");
    scanf("%u", &max_iterations);

    /* bisection main */
    do
    {
        /* c = middle point of new a and b */
        c = (a + b) / 2;

        /*  */
        if (check_root(func, a, c))
            b = c;
        else
            a = c;

        fc = calculate_function(func, c);
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
    int i = 1;
    double fa, fb, fc;

    take_function(&func);
    take_interval(func, &a, &b);

    printf("hata miktari: ");
    scanf("%lf", &epsilon);
    if (epsilon < 0)
        epsilon = -epsilon;

    printf("maksimum deneme sayisi: ");
    scanf("%u", &max_iterations);

    fc = calculate_function(func, a);

    do
    {
        fa = calculate_function(func, a);
        fb = calculate_function(func, b);
        c = (b * fa - a * fb) / (fa - fb);

        fc = calculate_function(func, c);
    } while (i++ < max_iterations && fabs(fc) > epsilon && fc != 0);

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
    double epsilon, a, b, c;
    int i = 1;
    double fc, fdc; /* fdc = f'(c) */

    take_function(&func);
    take_interval(func, &a, &b);

    do
    {
        printf("0- baslangic noktasi girmek istemiyorum\n"
               "1- baslangic noktasi girmek istiyorum\n");
        scanf(" %lf", &c);
    } while (c != 0 && c != 1);
    if (c == 1)
    {
        do
        {
            printf("başlangic noktasi: ");
            scanf("%lf", &c);
        } while (c < a || c > b);
    }
    else
        c = a;

    printf("hata miktari: ");
    scanf("%lf", &epsilon);
    if (epsilon < 0)
        epsilon = -epsilon;

    printf("maksimum deneme sayisi: ");
    scanf("%u", &max_iterations);

    do
    {
        fdc = calculate_derivative(func, c);
        if (fdc == 0)
        {
            printf("iraksiyor (türev = 0)\n");
            return;
        }
        c = c - fc / fdc;

        fc = calculate_function(func, c);
    } while (i++ < max_iterations && fabs(fc) > epsilon && fc != 0);

    if (i >= max_iterations)
    {
        printf("maksimum iterasyon sayisi asildi\n"
               "son deger: ");
    }

    printf("%lf\n", c);
}

void inverse_of_matrix()
{
    double matrix[MAX_HEIGHT][MAX_WIDTH] = {0};
    double inverse[MAX_HEIGHT][MAX_WIDTH] = {0};
    int n;

    printf("matrisin boyutu (NxN): ");
    scanf("%d", &n);
    take_matrix(matrix, n, n);
    if (fabs(determinant(matrix, n)) < 0.00001)
    {
        printf("matrisin tersi yok\n");
        return;
    }
    eliminate_zeros_on_diagonal(matrix, n);

    make_unit_matrix(inverse, n);
    invert_matrix_using_sample(matrix, n, inverse);

    print_matrix(inverse, n, n);
}

void gauss_elimination()
{
    double matrix[MAX_HEIGHT][MAX_WIDTH] = {0};
    double column_matrix[MAX_HEIGHT] = {0};
    int n;

    printf("matrisin boyutu (NxN): ");
    scanf("%d", &n);
    take_matrix(matrix, n, n);
    printf("sutun matrisini giriniz");
    take_array(column_matrix, n);

    eliminate_zeros_on_diagonal(matrix, n);
    int i, j;
    /* eliminate lower triangle */
    for (i = 0; i < n; i++)
    {
        column_matrix[i] /= matrix[i][i];
        divide_row_by_a(matrix[i], n, matrix[i][i]);
        for (j = i + 1; j < n; j++)
        {
            column_matrix[j] -= column_matrix[i] * matrix[j][i];
            substract_x_times_row_b_from_row_a(matrix, n, j, i, matrix[j][i]);
        }
    }
    /* eliminate upper triangle*/
    for (i = 1; i < n; i++)
        for (j = 0; j < i; j++)
        {
            column_matrix[j] -= column_matrix[i] * matrix[j][i];
            substract_x_times_row_b_from_row_a(matrix, n, j, i, matrix[j][i]);
        }
    print_array(column_matrix, n);
}

void gauss_seidal()
{
    double matrix[MAX_HEIGHT][MAX_WIDTH];
    double column_matrix[MAX_HEIGHT];
    double variables[MAX_WIDTH];
    double epsilon, error, max_error = 0;
    double temp;
    unsigned int iterations = 0, max_iterations;
    int i, j;
    int n;

    printf("matrisin genisligi: ");
    scanf("%d", &n);
    take_matrix(matrix, n, n);
    printf("sutun matrisini giriniz\n");
    take_array(column_matrix, n);

    printf("başlangic noktasi: ");
    scanf("%d", &variables[0]);
    set_array(variables, n, variables[1]);

    printf("hata miktari: ");
    scanf("%lf", &epsilon);
    if (epsilon < 0)
        epsilon = -epsilon;

    printf("maksimum iterasyon sayisi: ");
    scanf("%u", &max_iterations);

    make_diagonal_product_max(matrix, n);

    do
    {
        max_error = 0;
        for (i = 0; i < n; i++)
        {
            temp = 0;
            for (j = 0; j < n; j++)
                temp += matrix[i][j] * variables[j];
            error = variables[i];
            temp -= matrix[i][i] * variables[i];
            variables[i] = (column_matrix[i] - temp) / matrix[i][i];
            error = fabs(error - variables[i]);
            if (error > max_error)
                max_error = error;
        }
    } while (iterations++ <= max_iterations && max_error > epsilon);

    if (i >= max_iterations)
    {
        printf("maksimum iterasyon sayisi asildi\n"
               "son deger: ");
    }

    print_array(variables, n);
}

void numerical_derivative()
{
    Function func;
    double func_derivative;
    double x, h;
    int method = -1;

    take_function(&func);

    printf("turevi bulunacak deger");
    scanf("%lf", &x);

    printf("fark operatorunun degeri");
    scanf("%lf", &h);

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
}

void simpson()
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

    total_area = calculate_function(func, start);
    total_area += calculate_function(func, end);

    for (temp = 0, limit = number_of_segments - 1, i = 1; i <= limit; i += 2)
        temp += calculate_function(func, start + i * segment_size);
    total_area += 4 * temp;

    for (temp = 0, limit = number_of_segments - 2, i = 2; i <= limit; i += 2)
        temp += calculate_function(func, start + i * segment_size);
    total_area += 2 * temp;

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
    for (i = 1; i < number_of_segments - 1; i++)
        total_area += calculate_function(func, start + i * segment_size);
    total_area += (calculate_function(func, start) + calculate_function(func, end)) / 2;
    total_area = total_area * segment_size;

    printf("%lf\n", total_area);
}

void gregory_newton()
{
    Vector2 points[MAX_WIDTH];
    unsigned int len;
    double x;
    double temp;
    double ans;
    int i, j;

    printf("girilecek nokta sayisi: ");
    scanf("%u", &len);

    printf("noktalari giriniz\n");
    take_vector2_array(points, len);

    printf("hangi noktanin degeri bulunmali: ");
    scanf("%lf", &x);

    ans = points[0].y;
    temp = 1, i = 0;
    while ((points[1].y - points[0].y != points[len].y - points[len - 1].y) || len <= 1)
    {
        len--;
        for (j = 0; j < len; j++)
            points[j].y = points[j + 1].y - points[j].y;
        temp = temp * (x - i) / (i + 1);
        ans += temp * points[0].y;
        i++;
    }

    printf("%lf", ans);
}

void set_array(double arr[], int n, double value)
{
    int i;
    for (i = 0; i < n; i++)
        arr[i] = value;
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
        printf("%d. eleman:", i + 1);
        scanf("%lf", &array[i]);
    }
}

void take_vector2_array(Vector2 vector2_arr[], int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        printf("%d. elemanin x degeri:", i + 1);
        scanf("%lf", &vector2_arr[i].x);
        printf("%d. elemanin y degeri:", i + 1);
        scanf("%lf", &vector2_arr[i].y);
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
        swap_double(&matrix[i][row_a], &matrix[i][row_b]);
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

void eliminate_zeros_on_diagonal(double matrix[][MAX_WIDTH], int n)
{
    int i = 0, j = 0;
    i = check_non_zero_diagonal(matrix, n);
    while (i != -1)
    {
        j = i + 1;
        while (matrix[j % n][i] == 0)
        {
            j++;
        }
        swap_row(matrix, n, i, j);
        i = check_non_zero_diagonal(matrix, n);
    }
}

void make_unit_matrix(double matrix[][MAX_WIDTH], int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        matrix[i][i] = 1;
    }
}

void invert_matrix_using_sample(double sample[][MAX_WIDTH], int n, double output[][MAX_WIDTH])
{
    int i, j;
    for (i = 0; i < n; i++)
    {
        divide_row_by_a(output[i], n, sample[i][i]);
        for (j = i + 1; j < n; j++)
            substract_x_times_row_b_from_row_a(output, n, j, i, sample[j][i]);
        divide_row_by_a(sample[i], n, sample[i][i]);
        for (j = i + 1; j < n; j++)
            substract_x_times_row_b_from_row_a(sample, n, j, i, sample[j][i]);

        for (i = 1; i < n; i++)
            for (j = 0; j < i; j++)
            {
                substract_x_times_row_b_from_row_a(output, n, j, i, sample[j][i]);
                substract_x_times_row_b_from_row_a(sample, n, j, i, sample[j][i]);
            }
    }
}

void make_diagonal_product_max(double matrix[][MAX_WIDTH], int n)
{
    int i = 0, j = 0;
    int height_of_max = 0;
    for (i = 0; i < n; i++)
    {
        height_of_max = i;
        for (j = i + 1; j < n; j++)
            if (matrix[j][i] > matrix[height_of_max][i])
                height_of_max = j;
        if (height_of_max != i)
            swap_row(matrix, n, i, height_of_max);
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
    printf("\n\n");
    for (i = 0; i < n; i++)
    {
        printf("%lf - ", array[i]);
    }
    printf("\n\n");
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

        if (check_root(func, *a, *b) == false)
        {
            printf("aralikta kok yok\n"
                   "yeniden aralik girmek icin 'e' girin: ");
            scanf(" %c", &input);
        }
    } while (input == 'e' || input == 'E');
    return;
}

bool check_root(Function func, double start, double end)
{
    if (calculate_function(func, start) * calculate_function(func, end) < 0)
        return true;
    else
        return false;
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
    printf("katsayi*x^us\n"
           "polinom sayisi: ");
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
    printf("f katsayi * (taban ^ (x katsayi * x ^ x us)) ^ (f us) )\n"
           "ustel fonksyon sayisi: ");
    scanf("%d", &count);
    for (i = 0; i < count; i++)
    {
        printf("x katsayisi: ");
        scanf("%lf", &func[i].x_coef);
        printf("x ustu: ");
        scanf("%lf", &func[i].x_exp);
        printf("Fonksyonun katsayisi:  ");
        scanf("%lf", &func[i].fn_coef);
        printf("Fonksyonun ustu: ");
        scanf("%lf", &func[i].fn_exp);
        printf("Ustel ifadenin tabani: ");
        scanf("%lf", &func[i].base);
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
    printf("f katsayi * log(taban,x katsayi * x ^ x us) ^ f us\n"
           "logaritmik fonksyon sayisi: ");
    scanf("%d", &count);
    for (i = 0; i < count; i++)
    {
        printf("x katsayisi: ");
        scanf("%lf", &func[i].x_coef);
        printf("x ustu: ");
        scanf("%lf", &func[i].x_exp);
        printf("Fonksyonun katsayisi: ");
        scanf("%lf", &func[i].fn_coef);
        printf("Fonksyonun ustu: ");
        scanf("%lf", &func[i].fn_exp);
        printf("Logaritmik ifadenin tabani: ");
        scanf("%lf", &func[i].base);

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

    printf("f katsayi * trig f( x katsayi * x ^ x us ) ^ f us\n"
           "trigonometrik fonksyon sayisi: ");
    scanf("%d", &count);
    for (i = 0; i < count; i++)
    {
        do
        {
            printf(
                "lutfen fonksyon tipini seciniz\n"
                "sin: 0\n"
                "cos: 1\n"
                "tan: 2\n"
                "cot: 3\n");
            scanf("%d", &func[i].trig_fn);
        } while (func[i].trig_fn > 3);

        printf("x katsayisi: ");
        scanf("%lf", &func[i].x_coef);
        printf("x ustu: ");
        scanf("%lf", &func[i].x_exp);
        printf("Fonksyonun katsayisi: ");
        scanf("%lf", &func[i].fn_coef);
        printf("Fonksyonun ustu: ");
        scanf("%lf", &func[i].fn_exp);

        printf("eklendi: %lf * %s ^ (%lf * x ^ %lf) ^ %lf\n",
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

    printf("f katsayi * trig f( x katsayi * x ^ x us ) ^ f us\n"
           "ters trigonometrik fonksyon sayisi: ");
    scanf("%d", &count);
    for (i = 0; i < count; i++)
    {
        do
        {
            printf(
                "lutfen fonksyon tipini seciniz\n"
                "arcsin: 0\n"
                "arccos: 1\n"
                "acrtan: 2\n"
                "arccot: 3\n");
            scanf("%d", &func[i].trig_fn);
        } while (func[i].trig_fn > 3);

        printf("x katsayisi: ");
        scanf("%lf", &func[i].x_coef);
        printf("x ustu: ");
        scanf("%lf", &func[i].x_exp);
        printf("Fonksyonun katsayisi:  ");
        scanf("%lf", &func[i].fn_coef);
        printf("Fonksyonun ustu: ");
        scanf("%lf", &func[i].fn_exp);

        printf("eklendi: %lf * %s ^ (%lf * x ^ %lf) ^ %lf\n",
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