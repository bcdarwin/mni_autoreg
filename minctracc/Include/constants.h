
/* ------------------------ Constants used in program  ------------------------ */

#define VOL_NDIMS       3
#define WORLD_NDIMS     3   /* Number of world spatial dimensions */
#define SLICE_NDIMS     2   /* Number of slice dimensions */
#define MAT_NDIMS       WORLD_NDIMS+1   /* Number of dims for homogenous matrix */
#define NO_AXIS         -1
#define DEFAULT_MAX     1.0
#define DEFAULT_MIN     0.0
#define FILL_DEFAULT    DBL_MAX   /* Fillvalue indicating -nofill */
#define SMALL_VALUE     (100.0*FLT_MIN)   /* A small floating-point value */
#define TRANSFORM_BUFFER_INCREMENT 256

#ifndef TRUE
#  define TRUE          1
#  define FALSE         0
#endif

#define NORMAL_STATUS   0
#define ERROR_STATUS    1


#define TRANS_PROCRUSTES  0
#define TRANS_LSQ         1
#define TRANS_LSQ6        2
#define TRANS_LSQ7        3
#define TRANS_LSQ9        4
#define TRANS_LSQ12       5
#define TRANS_PAT         6

#define OPT_SIMPLEX       0

#define SLICE_IND 0
#define ROW_IND   1
#define COL_IND   2
