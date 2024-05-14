#define NumberOfHiddenStates  2
#define NumberOfObs           13
#define NumberOfWords         3


void print_arr(const float *a, const int strictupperbound);
void print_2d_arr(const float *a, const int n_rows, const int n_cols);
void copy_first_in_second(const float *first, float *second, const int strictupperbound);
void copy_col_in_arr(float *arr, const int arr_height, const int arr_width, const float *col, const int nb_col );
float Sum(const float *a, const int strictupperbound );
void Add(float *a ,const float d, const int strictupperbound);
void Mult(float *a ,const float d, const int strictupperbound);
int  matMult(float *a, int num_rows_a, int num_cols_a, float* b, int num_rows_b, int num_cols_b, float *c);
float Baum_Welch_norm_step( float a[NumberOfHiddenStates][NumberOfHiddenStates],
			    float b[NumberOfWords][NumberOfHiddenStates],
			    float init[NumberOfHiddenStates],
			    const int   obs[NumberOfObs] );