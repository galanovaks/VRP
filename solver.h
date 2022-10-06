double **create_list(int n,FILE *f)
{
    double **list;
    int i;
    list=malloc(n*sizeof(double*));
    for (i=0;i<n;i++)
    {
        list[i]=malloc(4*sizeof(double));
        fscanf(f,"%lf %lf %lf",&list[i][2],&list[i][0],&list[i][1]);
        list[i][3]=-1;
    }
    return list;
}

int **create_graph(int n)
{
    int **graph;
    int i,j;
    graph=malloc(n*sizeof(int*));
    for (i=0;i<n;i++)
    {
        graph[i]=malloc(n*sizeof(int));
        graph[i][i]=-1;
    }
    for (i=0;i<n-1;i++)
        for (j=i+1;j<n;j++)
    {
        graph[i][j]=0;
        graph[j][i]=graph[i][j];
    }
    return graph;
}

void solution(double **l,int n)
{
    int i,j,**g=create_graph(n);
    for (i=0;i<n;i++)
        for (j=0;j<n;j++)
            printf("%d",g[i][j]);
}

