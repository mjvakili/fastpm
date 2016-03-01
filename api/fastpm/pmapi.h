FASTPM_BEGIN_DECLS

/* 
 * Allocate memory for FFT/painting in PM. 
 * */

typedef struct {
    /* in units of real numbers, not bytes. */
    ptrdiff_t start[3];
    ptrdiff_t size[3];
    ptrdiff_t strides[3]; 
    ptrdiff_t total;
} PMRegion;

typedef struct {
    void * (*malloc )(size_t);
    void   (*free   )(void *);
    void   (*get_position)(void * pdata, ptrdiff_t index, double pos[3]);
    size_t (*pack)  (void * pdata, ptrdiff_t index, void * packed, int attributes);
    void   (*unpack)(void * pdata, ptrdiff_t index, void * packed, int attributes);
    void   (*reduce)(void * pdata, ptrdiff_t index, void * packed, int attributes);
    double (*to_double)(void * pdata, ptrdiff_t index, int attribute);
    void   (*from_double)(void * pdata, ptrdiff_t index, int attribute, double value);
} PMIFace;


#define PACK_POS   (1 << 0)
#define PACK_VEL   (1 << 1)
#define PACK_DX1   (1 << 2)
#define PACK_DX2   (1 << 3)
#define PACK_ACC   (1 << 4)
#define PACK_ID    (1 << 5)
#define PACK_Q     (1 << 6)


#define PACK_ACC_X (1 << 10)
#define PACK_ACC_Y (1 << 11)
#define PACK_ACC_Z (1 << 12)
#define PACK_DX1_X   (1 << 13)
#define PACK_DX1_Y   (1 << 14)
#define PACK_DX1_Z   (1 << 15)
#define PACK_DX2_X   (1 << 16)
#define PACK_DX2_Y   (1 << 17)
#define PACK_DX2_Z   (1 << 18)

struct PMStore {
    PMIFace iface;

    int attributes; /* bit flags of allocated attributes */

    double (* x)[3];
    float (* q)[3];
    float (* v)[3];
    float (* acc)[3];
    float (* dx1)[3];
    float (* dx2)[3];
    uint64_t * id;
    size_t np;
    size_t np_upper;
    double a_x;
    double a_v;
};

FastPMFloat * pm_alloc(PM * pm);
void pm_free(PM * pm, FastPMFloat * buf);
void pm_assign(PM * pm, FastPMFloat * from, FastPMFloat * to);

/* property accessors of PM objects */
size_t pm_size(PM * pm);
double pm_norm(PM * pm);
ptrdiff_t * pm_nmesh(PM * pm);
double * pm_boxsize(PM * pm);
PMRegion * pm_i_region(PM * pm);
PMRegion * pm_o_region(PM * pm);

void pm_unravel_o_index(PM * pm, ptrdiff_t ind, ptrdiff_t i[3]);
void pm_unravel_i_index(PM * pm, ptrdiff_t ind, ptrdiff_t i[3]);

ptrdiff_t pm_ravel_o_index(PM * pm, ptrdiff_t i[3]);
ptrdiff_t pm_ravel_i_index(PM * pm, ptrdiff_t i[3]);

typedef struct {
    float k_finite; /* k, finite */
    float k; /* k */
    float kk_finite; /* k ** 2, on a mesh */
    float kk;  /* k ** 2 */
    float cic;  /* 1 - 2 / 3 sin^2 ( 0.5 k L / N)*/
    float extra;  /* any temporary variable that can be useful. */
} PMKFactors;

void pm_create_k_factors(PM * pm, PMKFactors * fac[3]);
void pm_destroy_k_factors(PM * pm, PMKFactors * fac[3]);


typedef struct {
    ptrdiff_t start;
    ptrdiff_t end;
    ptrdiff_t ind;
    ptrdiff_t i[3];
    ptrdiff_t iabs[3];
    PMKFactors * fac[3];
    PM * pm;
} PMKIter;


void 
pm_kiter_init(PM * pm, PMKIter * iter);

int pm_kiter_stop(PMKIter * iter);

void pm_kiter_next(PMKIter * iter);

/* 
 * r2c is out-of-place and c2r is in-place.
 * */
void 
pm_r2c(PM * pm, FastPMFloat * from, FastPMFloat * to);
void 
pm_c2r(PM * pm, FastPMFloat * inplace);


FASTPM_END_DECLS