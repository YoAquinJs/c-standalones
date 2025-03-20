#include <argp.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINES 5000
#define MAXLEN 1000
char *lineptr[MAXLINES];
FILE *in_stream;

int strcmp_st(const char *, const char *);
void mergesort(void *lineptr[], size_t left, size_t right,
               int (*comp)(void *, void *));
int readlines(char *lineptr[], size_t nlines);
void writelines(char *lineptr[], size_t nlines);

// compare strategies
#define lexcmp strcmp
int numcmp(const char *, const char *);
int dircmp(const char *, const char *);
int (*cmp_st)(const char *, const char *);

// compare qualifers
#define REVERSE (1 << 0)
#define FOLD (1 << 1)
int cmp_st_qualifiers = 0;

// argument parsing

/* Program documentation */
static char doc[] =
    "sort stdin to stdout"
    "flags numeric -n, reverse -r, fold -f, and directory -d."
    "default comparison is lexicographic, -nf and -nd are invalid";

/* The options we understand */
static struct argp_option options[] = {
    {"numeric", 'n', NULL, 0, "sort by numeric parsing of lines"},
    {"directory", 'd', NULL, 0, "sort by letters numbers and blanks"},
    {"fold", 'f', NULL, 0, "sort case insensitive"},
    {"reverse", 'r', NULL, 0, "sort in reverse"},
    {0} // Terminate options list
};

struct arguments {
    int nflag, rflag, fflag, dflag;
};

/* Parse a single option */
static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;

    switch (key) {
    case 'n':
        if (arguments->fflag || arguments->dflag) {
            argp_usage(state);
        }
        arguments->nflag = 1;
        break;
    case 'f':
        if (arguments->nflag) {
            argp_usage(state);
        }
        arguments->fflag = 1;
        break;
    case 'r':
        arguments->rflag = 1;
        break;
    case 'd':
        arguments->dflag = 1;
        break;
    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

/* Our argp parser */
static struct argp argp = {options, parse_opt, NULL, doc};

int main(int argc, char *argv[]) {
    in_stream = stdin;

    /* Parse arguments */
    struct arguments arguments = {0};
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    if (arguments.nflag)
        cmp_st = numcmp;
    else if (arguments.dflag)
        cmp_st = dircmp;
    else
        cmp_st = lexcmp;

    if (arguments.rflag)
        cmp_st_qualifiers |= REVERSE;
    if (arguments.fflag)
        cmp_st_qualifiers |= FOLD;

    size_t nlines;
    if ((nlines = readlines(lineptr, MAXLINES)) == -1) {
        perror("input too big to sort\n");
        return 1;
    }

    mergesort((void **)lineptr, 0, nlines - 1,
              (int (*)(void *, void *))strcmp_st);
    writelines(lineptr, nlines);

    // free strings
    for (; nlines > -1; nlines--) {
        free(lineptr[nlines]);
    }
    return 0;
}

int strcmp_st(const char *str1, const char *str2) {
    char str1_cp[MAXLEN], str2_cp[MAXLEN];

    int reverse = 1, result;

    if (cmp_st_qualifiers & REVERSE)
        reverse = -1;
    if (cmp_st_qualifiers & FOLD) {
        size_t ptr;

        for (ptr = 1 + strcpy(str1_cp, str1) - str1_cp; ptr-- > 0;)
            str1_cp[ptr] = tolower(str1_cp[ptr]);
        for (ptr = 1 + strcpy(str2_cp, str2) - str2_cp; ptr-- > 0;)
            str2_cp[ptr] = tolower(str2_cp[ptr]);

        str1 = str1_cp;
        str2 = str2_cp;
    }

    return reverse * cmp_st(str1, str2);
}

/* numcmp: compare str1 and str2 numerically */
int numcmp(const char *str1, const char *str2) {
    double v1, v2;
    v1 = atof(str1);
    v2 = atof(str2);

    if (v1 == v2)
        return 0;
    return v1 < v2 ? -1 : 1;
}

/* dircmp: compare str1 and str2 only alphanumerically */
int dircmp(const char *str1, const char *str2) {
    char c1, c2;

    do {
        c1 = *str1;
        c2 = *str2;

        if (!isalnum(c1) && !isblank(c1) && c1 && (*++str1) == '\0')
            break;
        else if (!isalnum(c2) && !isblank(c2) && c2 && (*++str2) == '\0')
            break;
        else if (c1 != c2)
            break;
        else if (c1 == '\0')
            return 0;
        else {
            str1++;
            str2++;
        }
    } while (1);
    return c1 - c2;
}

/* mergesort: sort v[left]...v[right] into increasing order */
void mergesort(void *v[], size_t left, size_t right,
               int (*comp)(void *, void *)) {
    if (left >= right)
        return;

    size_t mid = left + (right - left) / 2;
    mergesort(v, left, mid, comp);
    mergesort(v, mid + 1, right, comp);

    // Temporary arrays for merging
    size_t left_size = mid - left + 1, right_size = right - mid;
    void **temp = malloc((right - left + 1) * sizeof(void *));
    if (!temp)
        return; // Handle allocation failure

    size_t i = left, j = mid + 1, k = 0;

    while (i <= mid && j <= right)
        temp[k++] = (comp(v[i], v[j]) <= 0) ? v[i++] : v[j++];

    while (i <= mid)
        temp[k++] = v[i++];
    while (j <= right)
        temp[k++] = v[j++];

    memcpy(&v[left], temp, k * sizeof(void *));
    free(temp);
}

void swap(void **v, size_t i, size_t j) {
    void *temp;
    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}

/* readlines: read input lines */
/* returns nlines strings heap allocated */
int readlines(char *lineptr[], size_t maxlines) {
    size_t len, nlines = 0;
    char *p, line[MAXLEN];

    while (fgets(line, MAXLEN, in_stream) && (len = strlen(line)) > 0) {
        if (nlines >= maxlines || !(p = malloc(sizeof(char) * len))) {
            for (; nlines > -1; nlines--) {
                free(lineptr[nlines]);
            }
            return -1;
        }

        line[len - 1] = '\0'; /* delete newline */
        strcpy(p, line);
        lineptr[nlines++] = p;
    }

    return nlines;
}
/* writelines: write output lines */
void writelines(char *lineptr[], size_t nlines) {
    for (size_t i = 0; i < nlines; i++) {
        printf("%s\n", lineptr[i]);
    }
}
