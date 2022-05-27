#ifndef QMC_STDLIB_H
#define QMC_STDLIB_H

#ifdef __SIZE_TYPE__
typedef __SIZE_TYPE__ size_t;
#else
typedef long unsigned int size_t;
#endif
typedef   signed char  int8_t;
typedef unsigned char  uint8_t;
typedef   signed short int16_t;
typedef unsigned short uint16_t;
typedef   signed int   int32_t;
typedef unsigned int   uint32_t;

#define bool _Bool
#define true 1
#define false 0

// ================= Enviroment Functions ================= //

void abort(void);
void exit(int s);
int atexit(void (*func)(void));

char *getenv(const char *v);
int *putenv(const char *v);

int system(const char *string);

// ================= Algorithm Functions ================= //

void *bsearch(
	const void *key,
	const void *base,
	size_t num,
	size_t size,
	int (*compare)(const void *e1, const void *e2));

void qsort(
	void *base,
	size_t num,
	size_t width,
	int (*compare)(const void *e1, const void *e2));

// ================= Mathematical Functions ================= //

int abs(int n);
long labs(long n);

typedef struct { int quot; int rem; } div_t;
div_t div(int n, int d);

typedef struct { long quot; long rem; } ldiv_t;
ldiv_t ldiv(long n, long d);

int       atoi (const char *s);
long      atol (const char *s);
long long atoll(const char *s);
double    atof (const char *s);

int rand(void);
int rand_r(void);
void srand(unsigned int seed);

// ================= Memory Functions ================= //

void *calloc(size_t count, size_t size);
void *malloc(size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);

#endif

