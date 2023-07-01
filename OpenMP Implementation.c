//inputs: a(n,n)
// outputs: pi(n), l(n,n), and u(n,n)
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
//number of threads and size of matrix taken as inputs...
void swap(double *ptr1, double *ptr2,int choice,int row1, int row2,int beg, int end ,int n){
   if (choice==1){
    double temp=*ptr1;
    *ptr1=*ptr2;
    *ptr2=temp;
   }
   if(choice==2){
    for(int j=beg;j<end;j++){
    double temp=ptr1[row1+j*n];
    ptr1[row1+j*n]=ptr2[row2+j*n];
    ptr2[row2+j*n]=temp;
       }
   }

}
void multiply(double *mat1,double *mat2,double *res,int N)
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            res[i*N+j] = 0;
            for (int k = 0; k < N; k++)
                res[i*N+j] += mat1[i*N+k] * mat2[k*N+j];
        }
    }
}
void multiply1(int *mat1,double *mat2,double *res,int N)
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            res[i*N+j] = 0;
            for (int k = 0; k < N; k++)
                res[i*N+j] += mat1[i*N+k] * mat2[k*N+j];
        }
    }
}
double norm(double *mat,int n){
        double *norm = calloc(sizeof(double),n*n);
        int max;
        for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                        norm[i] = norm[i] + mat[j*n+i];
                }
        }

        max = norm[0];
        for (int i = 0; i < n; i++) {
                if (max < norm[i]) {
                        max = norm[i];
                }
        }
        return max;}
int main(int n, int t){
   // int n, t;//scanf used instead of argument taking as main will not take two int values as arguments on all platforms 
   // printf("Enter values of n and t with a space\n");
     //  scanf("%d %d",&n,&t);
    
    double (*a) = malloc(sizeof(double)*n*n);
    //random value for n
    double (*pi) = calloc(n,sizeof(double));
    double (*u) = calloc(n*n,sizeof(double));
    double (*l) = calloc(n*n,sizeof(double));
    srand(time(0));
    for(int x=0;x<n;x++){
        for(int y=0;y<n;y++){
            a[x*n+y]= (rand()%(100000-1+1))+1;//function generates random values between 1 and 100000
         //   printf("at %d, %d, %f \n",x,y,a[x*n+y]);
        }
    }
    time_t beg;
    time(&beg);
    for(int i=0;i<n;i++){
      for(int j=0;j<n;j++){
        if(i>j)
        u[i*n+(j)]=0;
        if(i==j)
        l[i*n+j]=1;
        if(i<j)
        l[i*n+j]=0;
      }
    }

    int k2=0;
    int max=0;
    int k=0;
   // initialize pi as a vector of length n DONE
    //initialize u as an n x n matrix with 0s below the diagonal DONE
    //initialize l as an n x n matrix with 1s on the diagonal and 0s above the diagonal
    #pragma omp parallel for num_threads(t)
    for(int i=0;i<n;i++)// i = 1 to n
      pi[i] = i+1;
    omp_lock_t lock;
    omp_init_lock(&lock);

    #pragma omp parallel for num_threads(t)
    for(int k=1;k<=n;k++)//k = 1 to n
      { max = 0;
     // printf("%d\n",k);
      for(int i=k;i<=n;i++){// i = k to n
        if(max < a[(i-1)*n+(k-1)]){
          max = a[(i-1)*n+(k-1)];
          k2 = i;}
          }

       if (max==0&&k!=n){
        printf("error (singular matrix)\n");
        }
      omp_set_lock(&lock);  
      swap(&pi[k-1] ,&pi[k2-1],1,0,0,0,0,n);
      swap(a,a,2,k-1,k2-1,0,n,n);// swap(&a[k-1][:],&a[k2-1][:],1);//1 for row swap
      swap(l,l,2,k-1,k2-1,0,k-1,n);//swap(&l[k-1][1:k-2],&l[k2-1][1:k-2]);
      u[k-1+n*(k-1)] = a[k-1+n*(k-1)];
      for(int i = k+1;i<= n;i++){
        l[n*(i-1)+(k-1)] = a[(i-1)*n+(k-1)]/u[n*(k-1)+(k-1)];
        u[n*(k-1)+(i-1)] = a[n*(k-1)+(i-1)];}
      for(int i = k+1;i<=n;i++){
        for(int j = k+1;j<= n;j++)
          a[n*(i-1)+(j-1)] = a[n*(i-1)+(j-1)] - l[n*(i-1)+(k-1)]*u[n*(k-1)+(j-1)];
      }
      omp_unset_lock(&lock);
    }
    omp_destroy_lock(&lock);
    time_t end;
    time(&end);
     printf("Time in seconds %d\n",end-beg);
  //  printf("E");
        int (*P)= calloc(sizeof(int),n*n);//declared as memory assumed to not be an issue and to reuse matrix multiply fn
        double (*PA) = malloc(sizeof(double)*n*n);
        double (*LU) = malloc(sizeof(double)*n*n);
       int Z=0;
   // printf("E");
        for(int x=0;x<n;x++){
                Z=pi[x];
            //printf("%d ",x);
            P[x*n+Z]=1;
        }



       // printf("s1");
        multiply1(P,a,PA,n);
        multiply(l,u,LU,n);
     //           printf("s2");

        for(int y=0;y<n;y++){
            for(int x=0;x<n;x++){
                PA[x+y*n]-=LU[x+y*n];
            }

        }
        double res=norm(PA,n);


    printf("Time in seconds %d and Norm is %f\n",end-beg,norm);
      free(a);
      free(pi);
      free(l);
      free(u);
      return 0;

}
    /*Here, the vector pi is a compact representation of a permutation matrix p(n,n),
    which is very sparse. For the ith row of p, pi(i) stores the column index of
    the sole position that contains a 1.*/

