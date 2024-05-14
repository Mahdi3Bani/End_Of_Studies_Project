#include <stdio.h>
#include <math.h>
#include <assert.h>

#define NumberOfHiddenStates  2
#define NumberOfObs           13
#define NumberOfWords         3


void print_arr(const float *a, const int strictupperbound){
  printf("{ %.4f ",*a);
  for (int i=1; i<strictupperbound; i++){
    printf(", %.4f ",*(a+i));
  }
  printf("}\n");
}

void print_2d_arr(const float *a, const int n_rows, const int n_cols){
  printf("{ \n ");
  for (int i=0; i<n_rows; i++){
    printf("    ");
    print_arr((a+n_cols*i),n_cols);
  }
  printf("}\n");
}

void copy_first_in_second(const float *first, float *second, const int strictupperbound){
  for (int i=0; i<strictupperbound; i++){ *(second+i)=*(first+i);}
}

void copy_col_in_arr(float *arr, const int arr_height, const int arr_width, const float *col, const int nb_col ){
  for (int i=0; i<arr_height; i++){ *(arr+i*arr_width+nb_col) = *(col+i); }
}

float Sum(const float *a, const int strictupperbound ){
  float answer=0;
  for(int i=0; i<strictupperbound; i++){ answer += *(a+i); }
  return answer;
}

void Add(float *a ,const float d, const int strictupperbound){
  for(int i=0; i<strictupperbound; i++){ *(a+i) += d; }
}

void Mult(float *a ,const float d, const int strictupperbound){
for(int i=0; i<strictupperbound; i++){ *(a+i) *= d; }
}

int  matMult(float *a, int num_rows_a, int num_cols_a, float* b, int num_rows_b, int num_cols_b, float *c){
  assert(num_cols_a == num_rows_b);
  int num_cols_c = num_cols_b;
    for (int i = 0; i < num_rows_a; i++) {
        for (int j = 0; j < num_cols_b; j++) {
            for (int k = 0; k < num_cols_a; k++) {
	      *(c+i*num_cols_c+j) += *( a + i * num_cols_a + k) * *(b+ k* num_cols_b +j); 
            }
        }
    }
    return 0;
}



float Baum_Welch_norm_step( float a[NumberOfHiddenStates][NumberOfHiddenStates],
			    float b[NumberOfWords][NumberOfHiddenStates],
			    float init[NumberOfHiddenStates],
			    const int   obs[NumberOfObs] )
{
  // N = NumberOfHiddenStates
  // M = NumberOfWords
  // T = NumberOfObs

  float alphas[NumberOfHiddenStates][NumberOfObs]={0};
  float betas[NumberOfHiddenStates][NumberOfObs]={0};
  float lambdas[NumberOfHiddenStates][NumberOfHiddenStates][NumberOfObs-1]={0};
  float gammas[NumberOfHiddenStates][NumberOfObs-1]={0};
  float cs[NumberOfObs];
  
  // Compute the alphas

  float alpha[NumberOfHiddenStates];
  for ( int i=0; i< NumberOfHiddenStates; i++ ){ alpha[i]=b[obs[0]][i]*init[i]; }
  float c   = Sum(&alpha[0], NumberOfHiddenStates);                                // TODO what if c==0 ?
  cs[0] = 1/c;
  Mult(&(alpha[0]),1/c,NumberOfHiddenStates);
  copy_col_in_arr( &(alphas[0][0]), NumberOfHiddenStates, NumberOfObs, &(alpha[0]), 0);

  for (int i=1; i< NumberOfObs; i++)
    {
      float contalpha[NumberOfHiddenStates]={0};
      matMult(&(a[0][0]),NumberOfHiddenStates,NumberOfHiddenStates, &(alpha[0]),NumberOfHiddenStates,1, &(contalpha[0]));
      for (int r=0; r<NumberOfHiddenStates; r++){ alpha[r]=contalpha[r]*b[obs[i]][r]; }
      c = Sum( &(alpha[0]) , NumberOfHiddenStates);                                // TODO why is c/=0
      cs[i]=1/c;
      Mult(&(alpha[0]), 1/c, NumberOfHiddenStates);
      copy_col_in_arr( &(alphas[0][0]), NumberOfHiddenStates, NumberOfObs, &(alpha[0]), i);
    }

  // Compute the betas
  
  float beta[NumberOfHiddenStates]={0};
  Add(&(beta[0]),1.0,NumberOfHiddenStates);
  Mult(beta,cs[NumberOfObs-1],NumberOfHiddenStates);
  copy_col_in_arr( &(betas[0][0]), NumberOfHiddenStates, NumberOfObs, &(beta[0]), NumberOfObs-1);
    
  for (int i=1; i< NumberOfObs-1; i++)
    {                                              
      for(int r=0; r< NumberOfHiddenStates;r++){ beta[r]*=b[obs[NumberOfObs-i]][r]; }
      float contbeta[NumberOfHiddenStates]={0};
      matMult( &(beta[0]), 1, NumberOfHiddenStates, &a[0][0], NumberOfHiddenStates,NumberOfHiddenStates, &(contbeta[0]));
      copy_first_in_second(&(contbeta[0]), &(beta[0]), NumberOfHiddenStates);
      Mult(beta,cs[NumberOfObs-i-1],NumberOfHiddenStates);
      copy_col_in_arr( &(betas[0][0]), NumberOfHiddenStates, NumberOfObs, &(beta[0]), NumberOfObs-i-1);
    }

  // Compute the lambdas
  
  for (int s=0; s< NumberOfObs-1; s++){
    for( int i=0; i< NumberOfHiddenStates; i++){
      for ( int j=0; j< NumberOfHiddenStates; j++){
	lambdas[i][j][s]= betas[i][s+1] * b[obs[s+1]][i] * a[i][j] * alphas[j][s];
      }
    }
  }

  // Compute the gammas
  
  for (int s=0; s< NumberOfHiddenStates; s++){
    for( int i=0; i< NumberOfHiddenStates; i++){
      for ( int j=0; j< NumberOfObs-1; j++){
	gammas[i][j]+= lambdas[s][i][j];
      }
    }
  }
  
  float sumgammas[NumberOfHiddenStates]={0};
  for( int i=0; i< NumberOfHiddenStates; i++){
    for ( int j=0; j< NumberOfObs-1; j++){
  	sumgammas[i]+= gammas[i][j];
    }
  }

  float newB[NumberOfWords][NumberOfHiddenStates]={0};
  for (int k=0; k<NumberOfObs-1; k++){
    for (int j=0; j<NumberOfHiddenStates;j++){
      newB[obs[k]][j]+=gammas[j][k]/sumgammas[j];      
    } 	   
  }

  float newA[NumberOfHiddenStates][NumberOfHiddenStates]={0};
  for (int s=0; s< NumberOfHiddenStates; s++){
    for( int i=0; i< NumberOfHiddenStates; i++){
      for ( int j=0; j< NumberOfObs-1; j++){
	newA[s][i]+= lambdas[s][i][j]/sumgammas[i];
      }
    }
  }
  
  float new_init[NumberOfHiddenStates]={0};
  for (int s=0; s< NumberOfHiddenStates; s++){
    new_init[s]=gammas[s][0];
  }

  float log_lik=0;
  for (int s=0; s<NumberOfObs; s++){
    log_lik += -1*log(cs[s]);
  }

  copy_first_in_second(&(newA[0][0]), &(a[0][0]), NumberOfHiddenStates*NumberOfHiddenStates);
  copy_first_in_second(&(newB[0][0]), &(b[0][0]), NumberOfHiddenStates*NumberOfWords);
  copy_first_in_second(&(new_init[0]), &(init[0]), NumberOfHiddenStates);
    
  return log_lik;
}


int main(){

  float A[2][2] = { {.7 , .3} ,
		    {.3 , .7} };

  float B[3][2] = { {.4 , .3} ,
		    {.3 , .4} ,
		    {.3 , .3} };
  
  float init[2] = { 1 , 0 };
  
  int obs[13] = {0,1,2,0,1,2,0,1,2,1,1,1,1};
  
  for (int i=0; i<10; i++){
    float ll = Baum_Welch_norm_step(A,B,init,obs);
    printf("%f\n",ll);
  }

  return 0;
}
  

