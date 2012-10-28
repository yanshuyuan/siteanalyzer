#include "bloomfilter.h"

#define SETBIT(a, n) (a[n/CHAR_BIT] |= (1<<(n%CHAR_BIT)))
#define GETBIT(a, n) (a[n/CHAR_BIT] & (1<<(n%CHAR_BIT)))

/* func: bloom_create
    arg: bsize -> bloomfilter bit size
    arg: fsize -> bloomfilter func size
    arg:   ... -> bloomfilter func pointer
    ret: struct for bloom filter
*/
int bloom_init(BF *bf, unsigned int bsize, unsigned int fsize, ...)
{
    if(bsize == 0) { 
        fprintf(stderr, "Bloom filter size can't be zero");
	return 0;
    }

    if(fsize == 0) {
        fprintf(stderr, "Hash function can't be none");
	return 0;
    }

    bf->bits = NULL;
    bf->func = NULL;

    /* allocate memory for bf->bits */
    bf->bits = (char *)malloc((((bsize - 1) / CHAR_BIT) + 1) * sizeof(char));
    if(bf->bits == NULL) {
	fprintf(stderr, "Allocate memory for bf->bits failed");
	return 0;
    }
    bzero(bf->bits, (((bsize - 1) / CHAR_BIT) + 1) * sizeof(char));
    /* allocate memory for bf->func */
    bf->func = (hashfunc_t *)malloc(fsize * sizeof(hashfunc_t));
    if(bf->func == NULL) {
	fprintf(stderr, "Allocate memory for bf->func failed");
	free(bf->bits);
	return 0;
    }

    /* push hash func pointer in to the hashfunc_t array */
    va_list func_list;

    va_start(func_list, fsize);
    int i;
    for(i = 0; i < fsize; i++) {
        bf->func[i] = va_arg(func_list, hashfunc_t);
    }

    va_end(func_list);

    bf->bsize = bsize;
    bf->fsize = fsize;
    
    return 1;
}

/* func: bloom_destroy
    arg:  bf -> bloomfilter struct
*/
int bloom_destroy(BF *bf)
{
    if(bf != NULL) {
        if(bf->bits != NULL) {
            free(bf->bits);
        }
        if(bf->func != NULL) {
            free(bf->func);
        }
    }
    return 0;
}

/* func: bloom_add
    arg:  bf -> bloomfilter struct
    arg: str -> add into bloomfilter string
*/
int bloom_add(BF *bf, char *str)
{
    unsigned int i;
    /* for every hash_func do hash and set bit to 1 */
    for(i = 0; i < bf->fsize; i++) {
	SETBIT(bf->bits, bf->func[i](str, strlen(str)) % bf->bsize);
    }
    return 0;
}

/* func: bloom_check
    arg:  bf -> bloomfilter struct
    arg: str -> check in bloomfilter string
*/
int bloom_check(BF *bf, char *str)
{
    unsigned int i;
    for(i = 0; i < bf->fsize; i++) {
	if(!GETBIT(bf->bits, bf->func[i](str, strlen(str)) % bf->bsize)) return 0;
    }
    return 1;
}
