#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include<pthread.h>
#include<unistd.h>
#include <sys/time.h>

#define MAX 500

double matrix1[MAX][MAX],matrix2[MAX][MAX],matrix3[MAX][MAX];
int raw = MAX,col = MAX;

void *multithread(void *jj)
{

    int start = *((int*)jj);
    start = start*5;
    float val;
    for (int i = start; i<start+5; i++){
        for (int j=0; j< col; j++){
            val=0;
            for (int k=0; k<raw; k++){
                val += (matrix1[i][k] * matrix2[k][j]);
            }
            matrix3[i][j] = val;
        }
    }
    free(jj);
    pthread_exit(NULL);
    return 0;
}


int main()
{
    int i,j,k,x;
    float val;
    char infilename[20],outfilename[20];
    FILE *infile, *outfile;
    struct timeval tp_s, tp_e;
    struct timezone tzp_s, tzp_e;
    gettimeofday(&tp_s,&tzp_s);
    for(x=0; x<1000; x++)
    {
        sprintf(infilename,"in%d.txt",x+1);
        sprintf(outfilename,"out%d.txt",x+1);
        if ((infile=fopen(infilename,"r"))==NULL)
            perror("in open error~\n");
        fscanf(infile,"%d",&raw);
        fscanf(infile,"%d",&col);
        //printf("wrong1\n");
        printf("%d\n",x);
        fflush(stdout);
        for(i=0; i<raw; i++)
        {
            for(j=0; j<col; j++)
            {
                fscanf(infile,"%f",&val);
                matrix1[i][j]=matrix2[i][j]=val;
            }
        }
        if ((outfile=fopen(outfilename,"w"))==NULL)
            perror("out open error~\n");
        fprintf(outfile,"%d\n%d\n",raw,col);
        fflush(stdout);
        int jj=0;
        pthread_t p[100];

        for (jj=0; jj<100; jj++){
            int *arg = malloc(sizeof(*arg));
            *arg = jj;
            pthread_create(&p[jj],NULL,multithread,arg);
            //pthread_join(p[jj],NULL);
        }
        for (jj=0; jj<100; jj++){
            //pthread_create(&p[jj],NULL,multithread,NULL);
            pthread_join(p[jj],NULL);
        }

        for(i=0; i<raw; i++)
        {
            for(j=0; j<col; j++,val++)
                fprintf(outfile,"%f\n",matrix3[i][j]);
                fflush(stdout);
        }
        fclose(infile);
        fclose(outfile);
    }

    gettimeofday(&tp_e,&tzp_e);
    if ((outfile=fopen("time.txt","w"))==NULL)
        perror("out open error~\n");
    fprintf(outfile,"Total execution time =%ld\n",tp_e.tv_sec-tp_s.tv_sec);
    fclose(outfile);
}
