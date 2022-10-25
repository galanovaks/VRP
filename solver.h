double **create_list(int n,FILE *f)
{
    double **list;
    int i;
    list=malloc(n*sizeof(double*));
    for (i=0;i<n;i++)
    {
        list[i]=malloc(8*sizeof(double));
        fscanf(f,"%lf %lf %lf",&list[i][2],&list[i][0],&list[i][1]);
        list[i][3]=-1;
        list[i][4]=0;
        list[i][5]=-1;
        list[i][6]=-1;
        list[i][7]=-1;
    }
    return list;
}

void delete_list(double **l,int n)
{
    double **aux=l;
    int i;
    for (i=0;i<n;i++)
        free(l[i]);
    free(aux);
}

void delete_n(int **note,int n)
{
    int **aux=note;
    int i;
    for (i=0;i<n;i++)
        free(note[i]);
    free(aux);
}

double distance(double **l,int i,int j)
{
    return sqrt(pow(l[j][0]-l[i][0],2)+pow(l[j][1]-l[i][1],2));
}

void max_count(double **g,int **note,double **l,int n,int c,int var)
{
    double dist=0,dm=-1,p7=-1;;
    int i;
    for (i=0;i<n;i++)
        if ((i!=c)&&(l[i][4]!=2)&&((l[i][6]==var)||(l[i][6]==-1))
        &&(note[c][0]!=i))
        {
            if (dist<g[i][c])
            {
                dm=g[i][c];
                p7=i;
            }
            dist+=g[i][c];
        }
    l[c][3]=dist;
    l[c][5]=dm;
    l[c][7]=p7;
}

int search_start(double **l,int n,int var,int var2)
{
    int i,res=-2;
    double dist=-1;
    for (i=0;i<n;i++)
    {   
        if ((l[i][4]==2)||((l[i][6]!=var)&&(l[i][6]!=-1))
        ||((var2==0)&&(l[i][4]==1)))
            continue;
        if ((dist<l[i][3])||((dist==l[i][3])
        &&(l[res][5]<l[i][5])))
        {  
            dist=l[i][3];
            res=i;
        }
    }
    return res;
}

int check_cycle(int **note,double **l,int n,int c1,int c)
{
    int k=c,p=c1;
    if ((l[c][4]!=1)||(l[c1][4]!=1))
        return 0;
    l[c1][4]++;
    l[c][4]++;
    note[c][1]=c1;
    note[c1][1]=c;
    while (k!=c1)
    {
        if (l[k][4]==1)
            break;
        if (note[k][0]==p)
        {
            p=k;
            k=note[k][1];
        }
        else
        {
            p=k;
            k=note[k][0];
        }
    }
    note[c][1]=-1;
    note[c1][1]=-1;
    l[c1][4]--;
    l[c][4]--;
    if (k==c1)
        return 1;
    else
        return 0;
}

int search_min(double **g,int **note,double **l,int n,int c,double *len,int var)
{
    int i,res=-2;
    double dist=-1;
    for (i=0;i<n;i++)
    {
        if ((i==c)||(l[i][4]==2)||(note[c][0]==i)
        ||((l[i][6]!=var)&&(l[i][6]!=-1)))
            continue;
        if (((dist==-1)||(dist>g[i][c])||((dist==g[i][c])&&(l[i][3]>l[res][3])))
        &&(check_cycle(note,l,n,i,c)==0))
        {
            dist=g[i][c];
            res=i;
        }
    }
    if (res!=-2)
    {
        *len+=dist;
        l[c][4]++;
        l[res][4]++;
        l[c][3]-=dist;
        l[res][3]-=dist;
        if (l[res][4]==1)
            note[res][0]=c;
        else
            note[res][1]=c;
        if (l[c][4]==1)
            note[c][0]=res;
        else
            note[c][1]=res;
        max_count(g,note,l,n,res,var);
        max_count(g,note,l,n,c,var);
        if ((var!=-1)&&((l[c][4]==2)||(l[res][4]==2)))
            for (i=0;i<n;i++)
            {
                if ((l[i][4]==2)||((l[i][6]!=var)&&(l[i][6]!=-1)))
                    continue;
                if  ((l[c][4]==2)&&(note[c][0]!=i)&&(note[c][1]!=i))
                    l[i][3]-=g[i][c];
                if  ((l[res][4]==2)&&(note[res][0]!=i)&&(note[res][1]!=i))
                    l[i][3]-=g[i][c];
            }
    }
    return res;
}

void swipe(int **note,int i,int c1,int c2)
{
    if (note[i][0]==c1)
        note[i][0]=c2;
    else
    note[i][1]=c2;
}

int triangle(double **g,int **note,double **l,int i,int var,double *len,int vert)
{
    int c1,c2,c3,var2,res=0;
    double r1,r2;
    if (((l[i][6]!=vert)&&(l[i][6]!=-1))||(l[i][4]!=2))
        return 0;
    if (var==0)
        var2=1;
    else
        var2=0;
    c1=note[i][var];
    c3=note[i][var2];
    if (l[c1][4]!=2)
        return 0;
    if (note[c1][0]==i)
        c2=note[c1][1];
    else
        c2=note[c1][0];
    if (((l[c1][6]!=vert)&&(l[c1][6]!=-1))||
        ((l[c2][6]!=vert)&&(l[c2][6]!=-1))||
        ((l[c3][6]!=vert)&&(l[c3][6]!=-1)))
        return 0;
    r1=g[i][c3]+g[c1][c2];
    r2=g[i][c2]+g[c1][c3];
    if (r2<r1)
    {
        *len+=r2;
        *len-=r1;
        swipe(note,i,c3,c2);
        swipe(note,c3,i,c1);
        swipe(note,c1,c2,c3);
        swipe(note,c2,c1,i);
        res++;
    }
    return res;
}

int optimization(double **g,int **note,double **l,int n,double *len,int i,int var)
{
    int j,r1,r2,r3,r4,res=0;
    double R1,R2;
    if ((l[i][4]!=2)||((l[i][6]!=var)&&(l[i][6]!=-1)))
            return 0;
    for (j=0;j<n;j++)
    {
        if ((note[j][1]==-1)||(i==j))
            continue;
        if ((note[i][0]==j)||(note[i][1]==j))
        {
            if (note[i][0]==j)
                r1=note[i][1];
            else
                r1=note[i][0];
            if (note[j][0]==i)
                r2=note[j][1];
            else
                r2=note[j][0];
            R1=g[i][r1]+g[j][r2];
            R2=g[i][r2]+g[j][r1];
            if (R2<R1)
            {
                *len+=R2;
                *len-=R1;
                swipe(note,i,r1,r2);
                swipe(note,j,r2,r1);
                swipe(note,r1,i,j);
                swipe(note,r2,j,i);
                res++;
            }
        }
        else
        {
            r1=note[i][0];
            r2=note[i][1];
            r3=note[j][0];
            r4=note[j][1];
            R1=g[i][r3]+g[i][r4]
            +g[j][r1]+g[j][r2];
            R2=g[i][r1]+g[i][r2]
            +g[j][r3]+g[j][r4];
            if (R2>R1)
            {
                *len+=R1;
                *len-=R2;
                swipe(note,i,r1,r3);
                swipe(note,i,r2,r4);
                swipe(note,j,r3,r1);
                swipe(note,j,r4,r2);
                if ((note[r1][0]!=j)&&(note[r1][1]!=j))
                    swipe(note,r1,i,j);
                if ((note[r2][0]!=j)&&(note[r2][1]!=j))
                    swipe(note,r2,i,j);
                if ((note[r3][0]!=i)&&(note[r3][1]!=i))
                    swipe(note,r3,j,i);
                if ((note[r4][0]!=i)&&(note[r4][1]!=i))
                    swipe(note,r4,j,i);
                res++;
            }
        }
    }
    return res;
}

void exchange(int **list,int l,int r,int var)
{
    int temp,i;
    temp=list[r][0];
    list[r][0]=list[l][0];
    list[l][0]=temp;
    temp=list[r][1];
    list[r][1]=list[l][1];
    list[l][1]=temp;
    if (var==0)
    {
        for (i=2;(list[r][i]!=-1)||(list[l][i]!=-1);i++)
        {
            temp=list[r][i];
            list[r][i]=list[l][i];
            list[l][i]=temp;
        }
    }
}

void sort_q(int left,int right,int **list,int var)
{
    int l=left,r=right;
    int ref=list[(r+l)/2][var];
    while (l<=r)
    {
        while (list[l][var]<ref)
            l++;
        while (list[r][var]>ref)
            r--;
        if (l<=r)
        {
            exchange(list,l,r,var);
            l++;
            r--;
        }
    }
    if (left<r)
        sort_q(left,r,list,var);
    if (right>l)
        sort_q(l,right,list,var);
}

double **create_graph(double **l,int n)
{
    double **graph;
    int i,j;
    graph=malloc(n*sizeof(double*));
    for (i=0;i<n;i++)
    {
        graph[i]=malloc(n*sizeof(double));
        graph[i][i]=-1;
    }
    for (i=0;i<n-1;i++)
        for (j=i+1;j<n;j++)
        {
            graph[i][j]=distance(l,i,j);
            graph[j][i]=graph[i][j];
        }
    return graph;
}

double sum_g(double **g,int **group,int vert,double **l,int n)
{
    double res=0,max1,max2;
    int i,j;
    for (j=1;group[vert][j]!=-1;j++)
    {
        max1=0;
        max2=0;
        for (i=0;i<n;i++)
        {
            if ((i==group[vert][j])||((l[i][6]!=vert)&&(l[i][6]!=-1)))
                continue;
            if ((max1==0)||(max1<=g[group[vert][j]][i]))
            {
                max2=max1;
                max1=g[group[vert][j]][i];
            }
        }
        res+=max1+max2;
    }
    return res;
}

void group_tsp(double **g,double **l,int **group,int **note,int n,double *len,int vert,int var)
{
    int k1,k2,c,fin,i,j;
    if (group[vert][0]==0)
        return;
    if (group[vert][0]==1)
    {
        k1=group[vert][1];
        *len+=2*g[0][k1];
        return;
    }
    if (group[vert][0]==2)
    {
        k1=group[vert][1];
        k2=group[vert][2];
        *len+=g[k1][k2]
        +g[0][k1]+g[0][k2];
        return;
    }
    for (i=0;i<n;i++)
    {
        note[i][0]=-1;
        note[i][1]=-1;
        l[i][3]=-1;
        l[i][4]=0;
        l[i][5]=-1;
    }
    for (i=0;i<n;i++)
        max_count(g,note,l,n,i,vert);
    i=0;
    c=search_start(l,n,vert,i);
    while ((c!=-2)&&((fin=search_min(g,note,l,n,c,len,vert))!=-2))
    {
        optimization(g,note,l,n,len,c,vert);
        optimization(g,note,l,n,len,fin,vert);
        triangle(g,note,l,c,0,len,vert);
        triangle(g,note,l,c,1,len,vert);
        triangle(g,note,l,fin,0,len,vert);
        triangle(g,note,l,fin,1,len,vert);
        c=search_start(l,n,vert,i);
        if (c==-2)
        {
            i++;
            c=search_start(l,n,vert,i);
        }
    }
    c=-1;
    j=-1;
    for (i=0;i<n;i++)
    {
        if ((l[i][6]!=vert)&&(l[i][6]!=-1))
            continue;
        if (note[i][1]==-1)
        {
            if (c==-1)
                c=i;
            else
            {
                j=i;
                break;
            }
        }
    }
    if (c!=-1)
    {
        *len+=g[c][j];
        l[c][4]++;
        l[j][4]++;
        note[c][1]=j;
        note[j][1]=c;
        optimization(g,note,l,n,len,c,vert);
        optimization(g,note,l,n,len,j,vert);
        triangle(g,note,l,c,0,len,vert);
        triangle(g,note,l,c,1,len,vert);
        triangle(g,note,l,j,0,len,vert);
        triangle(g,note,l,j,1,len,vert);
    }
    if (var==1)
    {
        for (c=0;c<2;c++)
            for (i=0;i<n;i++)
        {
            optimization(g,note,l,n,len,i,vert);
            triangle(g,note,l,i,0,len,vert);
            triangle(g,note,l,i,1,len,vert);
        }
    }
}

int opt2(double **g,int **note,int **group,double **g_opt,double **l,int cap,int n,int v,int var)
{
    int i,j,c,fin,k,k1,res=0;
    double *len,*len1;
    len=malloc(sizeof(double));
    len1=malloc(sizeof(double));
    for (i=0;i<v;i++)
    {
        if (g_opt[i][0]==0)
            continue;
        for (j=0;j<v;j++)
        {
            if (i==j)
                continue;
            if (group[i][0]==0)
                break;
            for (k=1;group[i][k]!=-1;k++)
            {   
                if (group[i][0]==0)
                    break;
                *len=0;
                *len1=0;
                if (g_opt[j][0]+l[group[i][k]][2]<cap)
                {
                    fin=group[i][k];
                    group[i][k]=-1;
                    for (c=k;group[i][c+1]!=-1;c++)
                        group[i][c]=group[i][c+1];
                    group[i][c]=-1;
                    for (c=1;group[j][c]!=-1;c++);
                    group[j][c]=fin;
                    l[fin][6]=j;
                    group[i][0]--;
                    group[j][0]++;
                    if (var==0)
                    {
                        *len=sum_g(g,group,i,l,n);
                        *len1=sum_g(g,group,j,l,n);
                    }
                    else
                    {
                        group_tsp(g,l,group,note,n,len,i,0);
                        group_tsp(g,l,group,note,n,len1,j,0);
                    }
                    if (*len+*len1<g_opt[i][1]+g_opt[j][1])
                    {
                        g_opt[i][1]=*len;
                        g_opt[j][1]=*len1;
                        g_opt[i][0]-=l[fin][2];
                        g_opt[j][0]+=l[fin][2];
                        k--;
                        res++;
                    }
                    else
                    {
                        for (c=1;group[j][c]!=-1;c++);
                        k1=group[j][c-1];
                        group[j][c-1]=-1;
                        for (c=1;group[i][c]!=-1;c++);
                        group[i][c]=k1;
                        l[k1][6]=i;
                        group[i][0]++;
                        group[j][0]--;
                    }
                }
            }
        }
    }
    return res;
}

int opt3(double **g,int **note,int **group,double **g_opt,double **l,int cap,int n,int v,int var)
{
    int i,j,fin,k,k1,m,res=0;
    double *len,*len1;
    len=malloc(sizeof(double));
    len1=malloc(sizeof(double));
    for (i=0;i<v;i++)
    {
        if (g_opt[i][0]==0)
            continue;
        for (j=0;j<v;j++)
        {
            if ((i==j)||(g_opt[j][0]==0))
                continue;
            for (k=1;group[i][k]!=-1;k++)
            {
                for (m=1;group[j][m]!=-1;m++)
                {
                    *len=0;
                    *len1=0;
                    if ((g_opt[j][0]-l[group[j][m]][2]
                    +l[group[i][k]][2]<cap)&&(g_opt[i][0]-
                    l[group[i][k]][2]+l[group[j][m]][2]<cap))
                    {
                        fin=group[i][k];
                        k1=group[j][m];
                        group[i][k]=k1;
                        group[j][m]=fin;
                        if (var==0)
                        {
                            *len=sum_g(g,group,i,l,n);
                            *len1=sum_g(g,group,j,l,n);
                        }
                        else
                        {
                            group_tsp(g,l,group,note,n,len,i,1);
                            group_tsp(g,l,group,note,n,len1,j,1);
                        }
                        if (*len+*len1<g_opt[i][1]+g_opt[j][1])
                        {
                            g_opt[i][1]=*len;
                            g_opt[j][1]=*len1;
                            g_opt[i][0]-=l[fin][2]-l[k1][2];
                            g_opt[j][0]-=l[k1][2]-l[fin][2];
                            res++;
                        }
                        else
                        {
                            group[i][k]=fin;
                            group[j][m]=k1;
                        }
                    }
                }
            }
        }
    }
    return res;
}

void solution(double **l,int n,int v,int cap)
{
    int i,c,j,k;
    int **note,**m,**group;
    double *len=malloc(sizeof(double)),*len1,**g_opt;
    double **g=create_graph(l,n);
    *len=0;
    len1=malloc(sizeof (double));
    note=malloc(n*sizeof(int*));
    m=malloc(n*sizeof(int*));
    group=malloc(v*sizeof(int*));
    g_opt=malloc(v*sizeof(double *));
    for (i=0;i<n;i++)
    {
        note[i]=malloc(2*sizeof(int));
        note[i][0]=-1;
        note[i][1]=-1;
        m[i]=malloc(2*sizeof(int));
        m[i][0]=i;
        m[i][1]=(int)l[i][2];
    }
    for (i=0;i<v;i++)
    {
        group[i]=malloc(n*sizeof(int));
        g_opt[i]=malloc(2*sizeof(double));
        group[i][0]=0;
        g_opt[i][0]=0;
        for (j=1;j<n;j++)
        {
            group[i][j]=-1;
        }
    }
    sort_q(0,n-1,m,1);
    for (i=n-1;i>0;i--)
    {
        sort_q(0,v-1,group,0);
        for(j=v-1;group[j][0]+m[i][1]>cap;j--);
        for(c=1;group[j][c]!=-1;c++);
        group[j][c]=m[i][0];
        group[j][0]+=m[i][1];
    }
    for (i=0;i<v;i++)
    {
        for (j=1;group[i][j]!=-1;j++)
        {
            l[group[i][j]][6]=i;
            g_opt[i][0]+=l[group[i][j]][6];
        }
        j--;
        group[i][0]=j;
    }
    for (i=0;i<v;i++)
         g_opt[i][1]=sum_g(g,group,i,l,n);
    if (n<300)
        j=8;
    else
        j=5;
    for (k=0;k<j;k++)
        opt2(g,note,group,g_opt,l,cap,n,v,0);
    for (k=0;k<v;k++)
    {
        *len=0;
        group_tsp(g,l,group,note,n,len,k,0);
        g_opt[k][1]=*len;
    }
    opt2(g,note,group,g_opt,l,cap,n,v,1);
    *len1=0;
    for (k=0;k<v;k++)
    {
        *len=0;
        group_tsp(g,l,group,note,n,len,k,1);
        *len1+=*len;
    }
    printf("%.3lf\n",*len1);
    delete_list(l,n);
    delete_n(note,n);
    free(len);
}

