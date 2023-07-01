//inputs: a(n,n)
// outputs: pi(n), l(n,n), and u(n,n)
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
struct arg{
	double *a;
	double *u;
	double *l;
	int * pi;
	int n;
	int t;
	};
struct temp{
struct arg *o;
int id;};
struct arg t1;
pthread_mutex_t lock;
 struct arg g;
void initialise( int N, int T){ 
          g.n=N;
	  g.t=T;
	  g.a = malloc(sizeof(double)*N*N);
          g.pi = malloc(N*sizeof(int));
          g.u = malloc(sizeof(double)*N*N);
          g.l = malloc(sizeof(double)*N*N);
         }
//number of threads and size of matrix taken as inputs...

void* parallel(void *ptr){


    int id=((int*) ptr);
    int k2=0;
    int max=0;
    int k=0;
    int length=g.n;
   // printf("%d",id);
    //fflush(stdout);
   for(int k=1+id;k<=g.n;k+=(g.t))//k = 1 to n
      { max = 0;
     // printf("%d\n",k);
      for(int i=k;i<=length;i++){// i = k to n
        if(max < g.a[(i-1)*length+(k-1)]){
          max = g.a[(i-1)*length+(k-1)];
          k2 = i;}
          }

       if (max == 0&&k!=length){
        printf("error (singular matrix)");
        }
         pthread_mutex_lock(&lock);
      swap(&g.pi[k-1] ,&g.pi[k2-1],1,0,0,0,0,length);
      swap(g.a,g.a,2,k-1,k2-1,0,length,length);// swap(&a[k-1][:],&a[k2-1][:],1);//1 for row swap
      swap(g.l,g.l,2,k-1,k2-1,0,k-1,length);//swap(&l[k-1][1:k-2],&l[k2-1][1:k-2]);
      g.u[k-1+length*(k-1)] = g.a[k-1+length*(k-1)];
      for(int i = k+1;i<= length;i++){
        g.l[length*(i-1)+(k-1)] =g.a[(i-1)*length+(k-1)]/g.u[length*(k-1)+(k-1)];
        g.u[length*(k-1)+(i-1)] =g.a[length*(k-1)+(i-1)];}
      for(int i = k+1;i<=length;i++){
        for(int j = k+1;j<= length;j++)
          g.a[length*(i-1)+(j-1)] = g.a[length*(i-1)+(j-1)] - g.l[length*(i-1)+(k-1)]*g.u[length*(k-1)+(j-1)];
      }
      pthread_mutex_unlock(&lock);
    }
}


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



int main(){
    //int n, t;//scanf used instead of argument taking as main will not take two int values as arguments on all platforms
   // printf("Enter values of n and t with a space\n");
    //   scanf("%d %d",&n,&t);
    int n=7000;
    int t=16;
    initialise(n,t);
        
    pthread_mutex_init(&lock,NULL);
    srand(time(0));
    for(int x=0;x<n;x++){
        for(int y=0;y<n;y++){
        //printf("(%d,%d,%d,%d)\n ",n,t,x,y);
           g.a[x*n+y]=rand();
        }
    }
    
    time_t beg, end;
    time(&beg);
    for(int i=0;i<n;i++){
      for(int j=0;j<n;j++){
        if(i>j)
        g.u[i*n+(j)]=0;
        if(i==j)
        g.l[i*n+j]=1;
        if(i<j)
        g.l[i*n+j]=0;
      }
      }

   // initialize pi as a vector of length n DONE
    //initialize u as an n x n matrix with 0s below the diagonal DONE
    //initialize l as an n x n matrix with 1s on the diagonal and 0s above the diagonal
    for(int i=0;i<n;i++)// i = 1 to n
      g.pi[i] = i+1;

    pthread_t threads[t];
    int thr[t];


    for(int i=0;i<t;i++){
    thr[i]=i;
        pthread_create(&threads[i], NULL, &parallel,(void *)thr[i]);
    }
    for(int i=0;i<t;i++){
        pthread_join(threads[i], NULL);
    }


    time(&end);
    printf("Time in seconds %d\n",end-beg);
    
  //  printf("E");
        int (*P)= calloc(sizeof(int),n*n);//declared as memory assumed to not be an issue and to reuse matrix multiply fn
        double (*PA) = malloc(sizeof(double)*n*n);
        double (*LU) = malloc(sizeof(double)*n*n);
       int Z=0;
   // printf("E");
        for(int x=0;x<n;x++){
                Z=g.pi[x];
            //printf("%d ",x);
            P[x*n+Z]=1;
        }



       // printf("s1");
        multiply1(P,g.a,PA,n);
        multiply(g.l,g.u,LU,n);
     //           printf("s2");

        for(int y=0;y<n;y++){
            for(int x=0;x<n;x++){
                PA[x+y*n]-=LU[x+y*n];
            }

        }
        double res=norm(PA,n);


    printf("Time in seconds %d and Norm is %f\n",end-beg,norm);
     // free(o);
      return 0;

}
    /*Here, the vector pi is a compact representation of a permutation matrix p(n,n),
    which is very sparse. For the ith row of p, pi(i) stores the column index of
    the sole position that contains a 1.*/

