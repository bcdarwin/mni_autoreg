/* ----------------------------- MNI Header -----------------------------------
@NAME       : matrix_basics.c
@DESCRIPTION: File containing routines for doing basic matrix calculations
@METHOD     : Contains routines :
                 printmatrix
                 calc_centroid
                 translate
                 transpose
                 matrix_multiply
                 trace
                 matrix_scalar_multiply
		 invertmatrix
@CALLS      : 
@CREATED    : January 31, 1992 (Peter Neelin)
@MODIFIED   : 
Tue Jun  1 12:46:44 EST 1993 LC
     added routines to make identity matrices, copy matrices, make rotation matrices
     each routine has two copies (one for float, one for double parameters)

Fri Jun  4 14:10:34 EST 1993 LC
     moved the *to_homogeneous() routines here, from procrustes.c
     changed all homogeneous transformation calls, so that the 
       homogeneous transformation has the translations< on the right,
       ie new_vec = matrix * old_vec!

---------------------------------------------------------------------------- */
#include <def_mni.h>
#include <recipes.h>

/* external calls: */
void   make_rots(float **xmat, float data_rot_x, float data_rot_y, float data_rot_z);

/* Routines defined in this file */

public void printmatrix(int rows, int cols, float **the_matrix);

public void calc_centroid(int npoints, int ndim, float **points, 
                          float *centroid);

public void translate(int npoints, int ndim, float **points, 
                      float *translation, float **newpoints);

public void transpose(int rows, int cols, float **mat, float **mat_transpose);

public void invertmatrix(int n, float **mat, float **mat_invert);

public void raw_matrix_multiply(int ldim, int mdim, int ndim, 
                                float **Amat, float **Bmat, float **Cmat);

public void matrix_multiply(int ldim, int mdim, int ndim, 
                            float **Amat, float **Bmat, float **Cmat);

public float trace(int size, float **the_matrix);

public void matrix_scalar_multiply(int rows, int cols, float scalar, 
                            float **the_matrix, float **product);

public void nr_identd(double **A, int m1, int m2, int n1, int n2 );
public void nr_identf(float **A, int m1, int m2, int n1, int n2 );

public void nr_copyd(double **A, int m1, int m2, int n1, int n2, double **B );
public void nr_copyf(float  **A, int m1, int m2, int n1, int n2, float **B );

public void nr_rotxd(double **M, double a);
public void nr_rotxf(float **M, float a);

public void nr_rotyd(double **M,double a);
public void nr_rotyf(float **M, float a);

public void nr_rotzd(double **M,double a);
public void nr_rotzf(float **M, float a);

public void nr_multd(double **A, int mA1, int mA2, int nA1, int nA2,
		     double **B, int mB1, int mB2, int nB1, int nB2, 
		     double **C);
public void nr_multf(float **A, int mA1, int mA2, int nA1, int nA2,
		     float **B, int mB1, int mB2, int nB1, int nB2, 
		     float **C);


public void transformations_to_homogeneous(int ndim, 
                  float *translation, float *centre_of_rotation,
                  float **rotation, float scale,
                  float **transformation);

public void translation_to_homogeneous(int ndim, float *translation,
                                       float **transformation);

public void rotation_to_homogeneous(int ndim, float **rotation,
                                       float **transformation);





/* ----------------------------- MNI Header -----------------------------------
@NAME       : printmatrix
@INPUT      : rows   - number of rows in matrix
              cols   - number of columns in matrix
              the_matrix - matrix to be printed (in numerical recipes form).
                 The dimensions of this matrix should be defined to be 
                 1 to rows and 1 to cols (when calling the numerical 
                 recipes routine matrix).
@OUTPUT     : (nothing)
@RETURNS    : (nothing)
@DESCRIPTION: Prints out a matrix on stdout with one row per line.
@METHOD     : 
@GLOBALS    : (none)
@CALLS      : (nothing special)
@CREATED    : Feb. 26, 1990 (Weiqian Dai)
@MODIFIED   : January 31, 1992 (Peter Neelin)
                 - change to roughly NIL-abiding code
---------------------------------------------------------------------------- */
public void printmatrix(int rows, int cols, float **the_matrix)
{
   int i,j;
   float f;

   /* Loop through rows and columns, printing one row per line */
   for (i=1; i <= rows; ++i) {
      for (j=1; j <= cols; ++j) {
         f=the_matrix[i][j];
         (void) printf(" %10.6f ",f);
      }
      (void) printf("\n");
   }
}


/* ----------------------------- MNI Header -----------------------------------
@NAME       : calc_centroid
@INPUT      : npoints - number of points
              ndim    - number of dimensions
              points  - points matrix (in numerical recipes form).
                 The dimensions of this matrix should be defined to be 
                 1 to npoints and 1 to ndim (when calling the numerical 
                 recipes routine matrix).
@OUTPUT     : centroid - vector of centroid of points (in num. rec. form)
                 This vector should run from 1 to ndim.
@RETURNS    : (nothing)
@DESCRIPTION: Calculates the centroid of a number of points in ndim dimensions.
@METHOD     : 
@GLOBALS    : (none)
@CALLS      : (nothing special)
@CREATED    : Feb. 26, 1990 (Weiqian Dai)
@MODIFIED   : January 31, 1992 (Peter Neelin)
                 - change to roughly NIL-abiding code and modified calling
                 sequence.
---------------------------------------------------------------------------- */
public void calc_centroid(int npoints, int ndim, float **points, 
                          float *centroid)
{
   int i,j;

   /* Loop over each dimension */
   for (i=1; i <= ndim; ++i) {
      /* Add up points and divide by number of points */
      centroid[i] = 0;
      for (j=1; j <= npoints; ++j) {
         centroid[i] += points[j][i];
      }
      if (npoints >0) centroid[i] /= (float) npoints;
   }
}


/* ----------------------------- MNI Header -----------------------------------
@NAME       : translate
@INPUT      : npoints - number of points
              ndim    - number of dimensions
              points  - points matrix (in numerical recipes form).
                 The dimensions of this matrix should be defined to be 
                 1 to npoints and 1 to ndim (when calling the numerical 
                 recipes routine matrix).
              translation - translation vector (in num. rec. form, running
                 from 1 to ndim).
@OUTPUT     : newpoints - translated points matrix (see points). This matrix
                 can be the original points matrix.
@RETURNS    : (nothing)
@DESCRIPTION: Translates a set of points by a given translation.
@METHOD     : 
@GLOBALS    : (none)
@CALLS      : (nothing special)
@CREATED    : Feb. 26, 1990 (Weiqian Dai)
@MODIFIED   : January 31, 1992 (Peter Neelin)
                 - change to roughly NIL-abiding code and modified calling
                 sequence.
---------------------------------------------------------------------------- */
public void translate(int npoints, int ndim, float **points, 
                      float *translation, float **newpoints)
{
   int i,j;

   for (i=1; i <= npoints; ++i) {
      for (j=1; j <= ndim; ++j) {
         newpoints[i][j] = points[i][j] + translation[j];
      }
   }
}


/* ----------------------------- MNI Header -----------------------------------
@NAME       : transpose
@INPUT      : rows    - number of rows
              cols    - number of columns
              mat     - original matrix (in numerical recipes form).
                 The dimensions of this matrix should be defined to be 
                 1 to rows and 1 to cols (when calling the numerical 
                 recipes routine matrix).
@OUTPUT     : mat_transpose  - transposed matrix (in numerical recipes form,
                 with dimensions 1 to cols and 1 to rows). 
@RETURNS    : (nothing)
@DESCRIPTION: Transposes a matrix.
@METHOD     : 
@GLOBALS    : (none)
@CALLS      : (nothing special)
@CREATED    : Feb. 26, 1990 (Weiqian Dai)
@MODIFIED   : January 31, 1992 (Peter Neelin)
                 - change to roughly NIL-abiding code and modified calling
                 sequence.
Fri Jun  4 14:10:34 EST 1993 LC
    added the possibility to have input and out matrices the same!
---------------------------------------------------------------------------- */
public void transpose(int rows, int cols, float **mat, float **mat_transpose)
{
   int i,j;

   float **Ctemp;

   if (mat==mat_transpose) {	      /* if input and output the same, then alloc
				         temporary space, so as not to overwrite
					 the input before the compete transpose is 
					 done. */
     /* Allocate a temporary matrix */
     Ctemp = matrix(1,cols,1,rows);
     
     for (i=1; i <= rows; ++i) {
       for (j=1; j <= cols; ++j) {
         Ctemp[j][i]=mat[i][j];
       }
     }
     
     /* Copy the result */
     for (i=1; i <= cols; ++i)
       for (j=1; j <= rows; ++j)
         mat_transpose[i][j] = Ctemp[i][j];
     
     /* Free the matrix */
     free_matrix(Ctemp,1,cols,1,rows);
   }
   else {
     for (i=1; i <= rows; ++i) {
       for (j=1; j <= cols; ++j) {
         mat_transpose[j][i]=mat[i][j];
       }
     }
   }
}

/* ----------------------------- MNI Header -----------------------------------
@NAME       : invertmatrix
@INPUT      : n       - number of rows/or columns (must be square)
              mat     - original matrix (in numerical recipes form).
                 The dimensions of this matrix should be defined to be 
                 1 to n rows and 1 to n cols (when calling the numerical 
                 recipes routine matrix).
@OUTPUT     : mat_invert  - the inverted  matrix (in numerical recipes form,
                 with dimensions 1 to n cols and 1 to n rows). 
@RETURNS    : (nothing)
@DESCRIPTION: Inverts a matrix.
@METHOD     : 
@GLOBALS    : (none)
@CALLS      : (nothing special)
@CREATED    : Fri Jun  4 14:10:34 EST 1993 Louis Collins
@MODIFIED   : 
---------------------------------------------------------------------------- */
public void raw_invertmatrix(int n, float **mat, float **mat_invert)
{

  void svdcmp(float **a, int m, int n, float w[], float **v);

  float 
    wmax,wmin,
    **ut,**u,*w,**v,**wd;
  int 
    i,j;

  u=matrix(1,n,1,n);
  ut=matrix(1,n,1,n);
  wd=matrix(1,n,1,n);
  w=vector(1,n);
  v=matrix(1,n,1,n);

  for (i=1; i<=n; ++i)		/* copy the input matrix */
    for (j=1; j<=n; ++j)
      u[i][j] = mat[i][j];


  svdcmp(u,n,n,w,v);

  wmax=0.0;
  for (j=1; j<=n; ++j) if (w[j]>wmax) wmax=w[j];

				/* this is where the threshold is set for editing
				   singular values.  The constant must be experimented
				   with. */
  wmin = wmax*1.0e-6;

  for (j=1; j<=n; ++j) 
    if (w[j]<wmin) 
      w[j]=0.0;
    else 
      w[j] = 1.0/w[j];  

  for (i=1; i<=n; ++i) {		/* multiply v by a matrix with diag=w */
    for (j=1; j<=n; ++j) 
      wd[i][j] = 0.0;
    wd[i][i] = w[i];
  }

  transpose(n,n,u,ut);
  raw_matrix_multiply(n,n,n,v,wd,u);
  raw_matrix_multiply(n,n,n,u,ut,mat_invert);

  free_matrix(u,1,n,1,n);
  free_matrix(ut,1,n,1,n);
  free_matrix(wd,1,n,1,n);
  free_matrix(v,1,n,1,n);
  free_vector(w,1,n);
  
}

public void invertmatrix(int ndim, float **mat, float **mat_invert)
{
  float **Ctemp;
  int i,j;

  if (mat==mat_invert) {	      /* if input and output the same, then alloc
				         temporary space, so as not to overwrite
					 the input as the inverse is being done. */
    /* Allocate a temporary matrix */
    Ctemp = matrix(1,ndim,1,ndim);
    
    /* invert the matrix */
    raw_invertmatrix(ndim, mat, Ctemp);
    
    /* Copy the result */
    for (i=1; i <= ndim; ++i)
      for (j=1; j <= ndim; ++j)
	mat_invert[i][j] = Ctemp[i][j];
    
    /* Free the matrix */
    free_matrix(Ctemp,1,ndim,1,ndim);
  }
  else {
    raw_invertmatrix(ndim, mat, mat_invert);
  }
}



/* ----------------------------- mni Header -----------------------------------
@NAME       : raw_matrix_multiply
@INPUT      : ldim, mdim, ndim - dimensions of matrices. Matrix Amat has
                 dimensions (ldim x mdim), matrix Bmat has dimension
                 (mdim x ndim) and resultant matrix has dimension
                 (ldim x ndim).
              Amat - First matrix of multiply (in numerical recipes form).
                 Dimensions are 1 to ldim and 1 to mdim.
              Bmat - Second matrix of multiply (in numerical recipes form).
                 Dimensions are 1 to mdim and 1 to ndim.
@OUTPUT     : Cmat - Resulting matrix (in numerical recipes form).
                 Dimensions are 1 to ldim and 1 to ndim. This matrix cannot
                 be either Amat or Bmat.
@RETURNS    : (nothing)
@DESCRIPTION: Multiplies two matrices.
@METHOD     : 
@GLOBALS    : (none)
@CALLS      : (nothing special)
@CREATED    : Feb. 26, 1990 (Weiqian Dai)
@MODIFIED   : January 31, 1992 (Peter Neelin)
                 - change to roughly NIL-abiding code and modified calling
                 sequence.
---------------------------------------------------------------------------- */
public void raw_matrix_multiply(int ldim, int mdim, int ndim, 
                                float **Amat, float **Bmat, float **Cmat)
{
   int i,j,k;

   /* Zero the output matrix */
   for (i=1; i <= ldim; ++i)
      for (j=1; j <= ndim; ++j)
         Cmat[i][j]=0.;

   /* Calculate the product */
   for (i=1; i <= ldim; ++i)
      for (j=1; j <= ndim; ++j)
         for (k=1; k <=mdim; ++k)
            Cmat[i][j] += (Amat[i][k] * Bmat[k][j]);
}

/* ----------------------------- MNI Header -----------------------------------
@NAME       : matrix_multiply
@INPUT      : ldim, mdim, ndim - dimensions of matrices. Matrix Amat has
                 dimensions (ldim x mdim), matrix Bmat has dimension
                 (mdim x ndim) and resultant matrix has dimension
                 (ldim x ndim).
              Amat - First matrix of multiply (in numerical recipes form).
                 Dimensions are 1 to ldim and 1 to mdim.
              Bmat - Second matrix of multiply (in numerical recipes form).
                 Dimensions are 1 to mdim and 1 to ndim.
@OUTPUT     : Cmat - Resulting matrix (in numerical recipes form).
                 Dimensions are 1 to ldim and 1 to ndim. This matrix can
                 be either matrix Amat or Bmat.
@RETURNS    : (nothing)
@DESCRIPTION: Multiplies two matrices.
@METHOD     : 
@GLOBALS    : (none)
@CALLS      : (nothing special)
@CREATED    : March 2, 1992 (Peter Neelin)
@MODIFIED   : March 2, 1992 (P.N.)
                 - Changed so that calling program can use an input matrix for
                 output.
---------------------------------------------------------------------------- */
public void matrix_multiply(int ldim, int mdim, int ndim, 
                            float **Amat, float **Bmat, float **Cmat)
{
   int i,j;
   float **Ctemp;

   /* Allocate a temporary matrix */
   Ctemp = matrix(1,ldim,1,ndim);

   /* Do the multiplication */
   raw_matrix_multiply(ldim,mdim,ndim,Amat,Bmat,Ctemp);

   /* Copy the result */
   for (i=1; i <= ldim; ++i)
      for (j=1; j <= ndim; ++j)
         Cmat[i][j] = Ctemp[i][j];

   /* Free the matrix */
   free_matrix(Ctemp,1,ldim,1,ndim);
}
                  

/* ----------------------------- MNI Header -----------------------------------
@NAME       : trace
@INPUT      : size   - size of the_matrix (the_matrix should be square)
              the_matrix - matrix for which trace should be calculated (in 
                 numerical recipes form). Dimensions are 1 to size and 
                 1 to size.
@OUTPUT     : (none)
@RETURNS    : trace of matrix
@DESCRIPTION: Calculates the trace of a matrix.
@METHOD     : 
@GLOBALS    : (none)
@CALLS      : (nothing special)
@CREATED    : Feb. 26, 1990 (Weiqian Dai)
@MODIFIED   : January 31, 1992 (Peter Neelin)
                 - change to roughly NIL-abiding code and modified calling
                 sequence.
---------------------------------------------------------------------------- */
public float trace(int size, float **the_matrix)
{
   float sum=0.;
   int i;

   for (i=1; i <= size; ++i) {
      sum += the_matrix[i][i];
   }

   return(sum);
}


/* ----------------------------- MNI Header -----------------------------------
@NAME       : matrix_scalar_multiply
@INPUT      : rows    - number of rows of the_matrix.
              cols    - number of columns of the_matrix
              scalar  - scalar by which the_matrix should be multiplied.
              the_matrix  - matrix to be multiplied (in numerical recipes 
                 form). Dimensions are 1 to rows and 1 to cols.
@OUTPUT     : product - result of multiply ( in numerical recipes form).
                 Dimensions are 1 to rows and 1 to cols. This matrix
                 can be the input matrix.
@RETURNS    : (nothing)
@DESCRIPTION: Multiplies a matrix by a scalar.
@METHOD     : 
@GLOBALS    : (none)
@CALLS      : (nothing special)
@CREATED    : Feb. 26, 1990 (Weiqian Dai)
@MODIFIED   : January 31, 1992 (Peter Neelin)
                 - change to roughly NIL-abiding code and modified calling
                 sequence.
---------------------------------------------------------------------------- */
public void matrix_scalar_multiply(int rows, int cols, float scalar, 
                            float **the_matrix, float **product)
{
   int i,j;

   for (i=1; i <= rows; ++i)
      for (j=1; j<=cols; ++j)
         product[i][j]=scalar*the_matrix[i][j];
}




/* ----------------------------- MNI Header -----------------------------------
@NAME       : nr_identd, nr_identf - make identity matrix
@INPUT      : A - pointer to matrix
              m1,m2 - row limits
	      n1,n2 - col limits
              (matrix in numerical recipes form, allocated by calling routine)
@OUTPUT     : identiy matrix in A
@RETURNS    : (nothing)
@DESCRIPTION: 
@METHOD     : 
@GLOBALS    : (none)
@CALLS      : (nothing special)
@CREATED    : Tue Jun  1 12:49:21 EST 1993 (Louis Collins)
@MODIFIED   : 

---------------------------------------------------------------------------- */
public void nr_identd(double **A, int m1, int m2, int n1, int n2 )
{

   int i,j;

   for (i=m1; i<=m2; ++i)
      for (j=n1; j<=n2; ++j) {
	 if (i==j) 
	    A[i][j] = 1.0;
	 else
	    A[i][j] = 0.0;
      }
   
}

public void nr_identf(float **A, int m1, int m2, int n1, int n2 )
{

   int i,j;

   for (i=m1; i<=m2; ++i)
      for (j=n1; j<=n2; ++j) {
	 if (i==j) 
	    A[i][j] = 1.0;
	 else
	    A[i][j] = 0.0;
      }
   
}

/* ----------------------------- MNI Header -----------------------------------
@NAME       : nr_copyd, nr_copyf - copy matrix
@INPUT      : A - source matrix
              m1,m2 - row limits
	      n1,n2 - col limits
              (matrix in numerical recipes form, allocated by calling routine)
@OUTPUT     : B - copy of A
@RETURNS    : (nothing)
@DESCRIPTION: 
@METHOD     : 
@GLOBALS    : (none)
@CALLS      : (nothing special)
@CREATED    : Tue Jun  1 12:49:21 EST 1993 (Louis Collins)
@MODIFIED   : 

---------------------------------------------------------------------------- */
public void nr_copyd(double **A, int m1, int m2, int n1, int n2, double **B )
{
   int i,j;

   for (i=m1; i<=m2; ++i)
      for (j=n1; j<=n2; ++j)
	 B[i][j] = A[i][j];
}

public void nr_copyf(float  **A, int m1, int m2, int n1, int n2, float **B )
{
   int i,j;

   for (i=m1; i<=m2; ++i)
      for (j=n1; j<=n2; ++j)
	 B[i][j] = A[i][j];
}



/* ----------------------------- MNI Header -----------------------------------
@NAME       : nr_rotxd,nr_rotxf - make rot X matrix
@INPUT      : M - 4x4 matrix
              a - rotation angle in radians
              (matrix in numerical recipes form, allocated by calling routine)
@OUTPUT     : modified matrix M
@RETURNS    : (nothing)
@DESCRIPTION: 
@METHOD     : 
   rx =[1   0      0      0 
        0  cos(a)  -sin(a) 0
        0 sin(a)  cos(a) 0
        0   0      0      1];
@GLOBALS    : (none)
@CALLS      : (nothing special)
@CREATED    : Tue Jun  1 12:49:21 EST 1993 (Louis Collins)
@MODIFIED   : Tue Jun  8 08:44:59 EST 1993 (LC) changed to mat*vec format
---------------------------------------------------------------------------- */
public void nr_rotxd(double **M, double a)
{
   nr_identd(M,1,4,1,4);

   M[2][2] = cos(a);   M[2][3] = -sin(a);
   M[3][2] = sin(a);   M[3][3] = cos(a);
}


public void nr_rotxf(float **M, float a)
{
   nr_identf(M,1,4,1,4);

   M[2][2] = fcos(a);    M[2][3] = -fsin(a);
   M[3][2] = fsin(a);   M[3][3] = fcos(a);
}


/* ----------------------------- MNI Header -----------------------------------
@NAME       : nr_rotyd,nr_rotyf - make rot Y matrix
@INPUT      : M - 4x4 matrix
              a - rotation angle in radians
              (matrix in numerical recipes form, allocated by calling routine)
@RETURNS    : (nothing)
@DESCRIPTION: 
@METHOD     : 
ry = [  cos(a)   0 sin(a)  0 
        0       1   0       0
        -sin(a)  0  cos(a)   0
        0   0      0      1];
@GLOBALS    : (none)
@CALLS      : (nothing special)
@CREATED    : Tue Jun  1 12:49:21 EST 1993 (Louis Collins)
@MODIFIED   : Tue Jun  8 08:44:59 EST 1993 (LC) changed to mat*vec format
---------------------------------------------------------------------------- */
public void nr_rotyd(double **M,double a)
{

   nr_identd(M,1,4,1,4);

   M[1][1] = cos(a);   M[1][3] = sin(a);
   M[3][1] = -sin(a);   M[3][3] = cos(a);
}

public void nr_rotyf(float **M, float a)
{

   nr_identf(M,1,4,1,4);

   M[1][1] = fcos(a);   M[1][3] = fsin(a);
   M[3][1] = -fsin(a);   M[3][3] = fcos(a);
}


/* ----------------------------- MNI Header -----------------------------------
@NAME       : nr_rotzd, nr_rotzf - make rot Z matrix
@INPUT      : M - 4x4 matrix
              a - rotation angle in radians
              (matrix in numerical recipes form, allocated by calling routine)
@RETURNS    : (nothing)
@DESCRIPTION: 
@METHOD     : 
rz = [cos(a)  -sin(a) 0  0
      sin(a) cos(a) 0  0
	0     0      1  0
        0     0      0  1];
@GLOBALS    : (none)
@CALLS      : (nothing special)
@CREATED    : Tue Jun  1 12:49:21 EST 1993 (Louis Collins)
@MODIFIED   : Tue Jun  8 08:44:59 EST 1993 (LC) changed to mat*vec format
---------------------------------------------------------------------------- */
public void nr_rotzd(double **M,double a)
{

   nr_identd(M,1,4,1,4);

   M[1][1] = cos(a);   M[1][2] = -sin(a);
   M[2][1] = sin(a);  M[2][2] = cos(a);
}

public void nr_rotzf(float **M, float a)
{

   nr_identf(M,1,4,1,4);

   M[1][1] = fcos(a);   M[1][2] = -fsin(a);
   M[2][1] = fsin(a);  M[2][2] = fcos(a);
}

/* ----------------------------- MNI Header -----------------------------------
@NAME       : nr_multd, nr_multf - mult matrix
@INPUT      : A - source matrix
              mA1,mA2 - row limits of A
	      nA1,nA2 - col limits of A
	      B - source matrix
              mB1,mB2 - row limits of B
	      nB1,nB2 - col limits of B
              (matrix in numerical recipes form, allocated by calling routine)
@OUTPUT     : C = A * B
@RETURNS    : (nothing)
@DESCRIPTION: 
   Routine multiplies matrices A*B to give C. A is a mA x nA matrix and
   B is a mB x nB matrix. The result is returned in C which is mA x nB.
@METHOD     : 
@GLOBALS    : (none)
@CALLS      : (nothing special)
@CREATED    : Tue Jun  1 12:49:21 EST 1993 (Louis Collins)
@MODIFIED   : 

---------------------------------------------------------------------------- */

public void nr_multd(double **A, int mA1, int mA2, int nA1, int nA2, 
	 double **B, int mB1, int mB2, int nB1, int nB2, 
	 double **C )
{
   int i, j, k;

   for ( k = mA1; k <= mA2; k++ ) {
      for ( i = nB1; i <= nB2; i++ ) {
         C[k][i] = 0.0;
         for ( j = mB1; j <= mB2; j++ ) {
            C[k][i] += A[k][j] * B[j][i];
         }
      }
   }

   return;
}


public void nr_multf(float **A, int mA1, int mA2, int nA1, int nA2, 
	 float **B, int mB1, int mB2, int nB1, int nB2, 
	 float **C)
{
   int i, j, k;

   for ( k = mA1; k <= mA2; k++ ) {
      for ( i = nB1; i <= nB2; i++ ) {
         C[k][i] = 0.0;
         for ( j = mB1; j <= mB2; j++ ) {
            C[k][i] += A[k][j] * B[j][i];
         }
      }
   }

   return;
}





/* ----------------------------- MNI Header -----------------------------------
@NAME       : transformations_to_homogeneous
@INPUT      : ndim    - number of dimensions
              translation - Numerical recipes vector (1 to ndim) that 
                 specifies the translation to be applied first.
              centre_of_rotation - Numerical recipes vector (1 to ndim) that
                 specifies the centre of rotation and scaling.
              rotation - Numerical recipes matrix (1 to ndim by 1 to ndim) 
                 for rotation about centre_of_rotation (applied after 
                 translation). Note that this matrix need not only specify
                 rotation/reflexion - any ndim x ndim matrix will work.
              scale - Scalar value giving global scaling to be applied after
                 translation and rotation.
@OUTPUT     : transformation - Numerical recipes matrix (1 to ndim+1 by
                 1 to ndim+1) specifying the transformation for homogeneous 
                 coordinates. To apply this transformation, a point
                 vector should be pre-multiplied by this matrix, with the
                 last coordinate of the ndim+1 point vector having value
                 one. The calling routine must allocate space for this
                 matrix.
@RETURNS    : (nothing)
@DESCRIPTION: Computes a transformation matrix in homogeneous coordinates
              given a translation, a rotation matrix (or other 
              non-homogeneous matrix) and a global scaling factor.
              Transformations are applied in that order.
@METHOD     : Apply the following operations (multiply from left to right):
                 1) Translate by translation
                 2) Translate by -centre_of_rotation
                 3) Rotate
                 4) Scale
                 5) Translate by centre_of_rotation
@GLOBALS    : (none)
@CALLS      : numerical recipes stuff
              translation_to_homogeneous
              matrix_multiply
              matrix_scalar_multiply
@CREATED    : February 7, 1992 (Peter Neelin)
@MODIFIED   : 
Fri Jun  4 14:10:34 EST 1993  LC
   changed matrices, so that they must be applied by pre-multiplication:
      ie newvec = matrix * oldvec
---------------------------------------------------------------------------- */
public void transformations_to_homogeneous(int ndim, 
                  float *translation, float *centre_of_rotation,
                  float **rotation, float scale,
                  float **transformation)
{
   int i;
   int size;
   float *centre_translate;
   float **trans1, **trans2;
   float **trans_temp, **rotation_and_scale;

   size=ndim+1;

   /* Allocate matrices and vectors */
   centre_translate = vector(1,ndim);
   trans1 = matrix(1,size,1,size);
   trans2 = matrix(1,size,1,size);
   trans_temp = matrix(1,size,1,size);
   rotation_and_scale = matrix(1,ndim,1,ndim);


   /* Construct translation matrix */
   translation_to_homogeneous(ndim, translation, trans1);


   /* Construct translation matrix for centre of rotation and
      apply it */
   for (i=1; i<=ndim; i++) centre_translate[i] = -centre_of_rotation[i];
   translation_to_homogeneous(ndim, centre_translate, trans_temp);
   matrix_multiply(size, size, size, trans1, trans_temp, trans2);


   /* Scale rotation matrix, then convert it to homogeneous coordinates and
      apply it */
   matrix_scalar_multiply(ndim, ndim, scale, rotation, rotation_and_scale);
   rotation_to_homogeneous(ndim, rotation_and_scale, trans_temp);
   matrix_multiply(size, size, size, trans2, trans_temp, trans1);


   /* Return to centre of rotation */
   translation_to_homogeneous(ndim, centre_of_rotation, trans_temp);
   matrix_multiply(size, size, size, trans1, trans_temp, transformation);


   /* Free matrices */
   free_vector(centre_translate,1,ndim);
   free_matrix(trans1,1,size,1,size);
   free_matrix(trans2,1,size,1,size);
   free_matrix(trans_temp,1,size,1,size);
   free_matrix(rotation_and_scale,1,ndim,1,ndim);

}


/* ----------------------------- MNI Header -----------------------------------
@NAME       : translation_to_homogeneous
@INPUT      : ndim    - number of dimensions
              translation - Numerical recipes vector (1 to ndim) that 
                 specifies the translation.
@OUTPUT     : transformation - Numerical recipes matrix (1 to ndim+1 by
                 1 to ndim+1) specifying the transformation for homogeneous 
                 coordinates. To apply this transformation, a point
                 vector should be pre-multiplied by this matrix, with the
                 last coordinate of the ndim+1 point vector having value
                 one. The calling routine must allocate space for this
                 matrix.
@RETURNS    : (nothing)
@DESCRIPTION: Computes a transformation matrix in homogeneous coordinates
              given a translation.
@METHOD     : 
@GLOBALS    : (none)
@CALLS      : 
@CREATED    : February 7, 1992 (Peter Neelin)
@MODIFIED   : 
Fri Jun  4 14:10:34 EST 1993  LC
   changed matrices, so that they must be applied by pre-multiplication:
      ie newvec = matrix * oldvec
---------------------------------------------------------------------------- */
public void translation_to_homogeneous(int ndim, float *translation,
                                       float **transformation)
{
   int i,j;
   int size;

   size=ndim+1;

   /* Construct translation matrix */
   for (i=1; i<=ndim; i++) {
      for (j=1; j<=size; j++) {
         if (i == j) {
            transformation[i][j] = 1.0;
         }
         else {
            transformation[i][j] = 0.0;
         }
      }
   }
   for (j=1; j<=ndim; j++) {
      transformation[j][size] = translation[j];
   }

   transformation[size][size] = 1.0;

}


/* ----------------------------- MNI Header -----------------------------------
@NAME       : rotation_to_homogeneous
@INPUT      : ndim    - number of dimensions
              rotation - Numerical recipes matrix (1 to ndim by 1 to ndim) 
                 for rotation about origin. Note that this matrix need not 
                 only specify rotation/reflexion - any ndim x ndim matrix 
                 will work.
@OUTPUT     : transformation - Numerical recipes matrix (1 to ndim+1 by
                 1 to ndim+1) specifying the transformation for homogeneous 
                 coordinates. To apply this transformation, a point
                 vector should be pre-multiplied by this matrix, with the
                 last coordinate of the ndim+1 point vector having value
                 one. The calling routine must allocate space for this
                 matrix.
@RETURNS    : (nothing)
@DESCRIPTION: Computes a transformation matrix in homogeneous coordinates
              given a rotation matrix.
@METHOD     : 
@GLOBALS    : (none)
@CALLS      : 
@CREATED    : February 7, 1992 (Peter Neelin)
@MODIFIED   : 
Fri Jun  4 14:10:34 EST 1993  LC
   changed matrices, so that they must be applied by pre-multiplication:
      ie newvec = matrix * oldvec
---------------------------------------------------------------------------- */
public void rotation_to_homogeneous(int ndim, float **rotation,
                                       float **transformation)
{
   int i,j;
   int size;

   size=ndim+1;

   /* Construct  matrix */
   for (i=1; i<=size; i++) {
      for (j=1; j<=size; j++) {
         if ((i==size) || (j==size)) {
            transformation[i][j] = 0.0;
         }
         else {
            transformation[i][j] = rotation[i][j];
         }
      }
   }

   transformation[size][size] = 1.0;

}



/* ----------------------------- MNI Header -----------------------------------
@NAME       : angles_to_homogeneous
@INPUT      : ndim    - number of dimensions
              angles - Numerical recipes array (1 to ndim)
                 for rotation angles (in radians) about origin. 
@OUTPUT     : transformation - Numerical recipes matrix (1 to ndim+1 by
                 1 to ndim+1) specifying the transformation for homogeneous 
                 coordinates. To apply this transformation, a point
                 vector should be pre-multiplied by this matrix, with the
                 last coordinate of the ndim+1 point vector having value
                 one. The calling routine must allocate space for this
                 matrix.
@RETURNS    : (nothing)
@DESCRIPTION: Computes a transformation matrix in homogeneous coordinates
              given a rotation matrix.
@METHOD     : 
@GLOBALS    : (none)
@CALLS      : 
@CREATED    : Fri Jun  4 14:10:34 EST 1993  LC
@MODIFIED   : 
---------------------------------------------------------------------------- */
public void angles_to_homogeneous(int ndim, float *angles,
				  float **transformation)
{
   int i,j;
   int size;
   float **rot_matrix;


   size=ndim+1;

   rot_matrix = matrix(1,4,1,4);


   if (ndim==2 || ndim==3) {

     if (ndim==2)
       nr_rotzf(rot_matrix,*angles );
     else
       make_rots(rot_matrix, 
		 (float)(angles[0]),
		 (float)(angles[1]),
		 (float)(angles[2]));

     /* Construct  matrix */
     for (i=1; i<=size; i++) {
       for (j=1; j<=size; j++) {
         if ((i==size) || (j==size)) {
	   transformation[i][j] = 0.0;
         }
         else {
	   transformation[i][j] = rot_matrix[i][j];
         }
       }
     }
     transformation[size][size] = 1.0;

   }
   else {
     fprintf (stderr,"Can't handle %d dimensions in angles_to_homogeneous()\n",ndim);
     fprintf (stderr,"Error in %s, line %d\n",__FILE__,__LINE__);
     exit(-1);
   }



   free_matrix(rot_matrix,1,4,1,4);


}

