double **create_list(int n,FILE *f)
{
    double **list;
    int i;
    list=malloc(n*sizeof(double*));
    for (i=0;i<n;i++)
    {
        list[i]=malloc(7*sizeof(double));
        fscanf(f,"%lf %lf %lf",&list[i][2],&list[i][0],&list[i][1]);
        list[i][3]=-1;
        list[i][4]=0;
        list[i][5]=-1;
        list[i][6]=-1;
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

void max_count(int **note,double **l,int n,int c,int var)
{
    double d,dist=-1,dm=-1;
    int i;
    for (i=0;i<n;i++)
        if ((i!=c)&&(l[i][4]!=2)&&((l[i][6]==var)||(l[i][6]==-1))
        &&(note[c][0]!=i))
        {
            d=distance(l,c,i);
            if (dist<d)
                dist=d;
            if ((dm==-1)||(d<dm))
                dm=d;
        }
    l[c][3]=dist;
    l[c][5]=dm;
}

int search_start(double **l,int n,int var)
{
    int i,res=-2;
    double dist=-1;
    for (i=0;i<n;i++)
    {   
        if ((l[i][4]==2)||((l[i][6]!=var)&&(l[i][6]!=-1)))
            continue;
        if ((dist<l[i][3])||((dist==l[i][3])
        &&(l[res][5]>l[i][5])))
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

int search_min(int **note,double **l,int n,int c,double *len,int var)
{
    int i,res=-2;
    double d,dist=-1;
    for (i=0;i<n;i++)
    {
        if ((i==c)||(l[i][4]==2)||(note[c][0]==i)
        ||((l[i][6]!=var)&&(l[i][6]!=-1)))
            continue;
        d=distance(l,c,i);
        if (((dist==-1)||(dist>d))
        &&(check_cycle(note,l,n,i,c)==0))
        {
            dist=d;
            res=i;
        }
    }
    if (res!=-2)
    {
//        dist=distance(l,c,res);
        if ((l[res][3]==dist)||(l[res][5]==dist))
            max_count(note,l,n,res,var);
        if ((l[c][3]==dist)||(l[c][5]==dist))
            max_count(note,l,n,c,var);
        *len+=dist;
        l[c][4]++;
        l[res][4]++;
        if (l[res][4]==1)
            note[res][0]=c;
        else
            note[res][1]=c;
        if (l[c][4]==1)
            note[c][0]=res;
        else
            note[c][1]=res;
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

int triangle(int **note,double **l,int i,int var,double *len,int vert)
{
    int c1,c2,c3,var2,res=0;
    double r1,r2;
    if ((l[i][6]!=vert)&&(l[i][6]!=-1))
        return 0;
    if (var==0)
        var2=1;
    else
        var2=0;
    c1=note[i][var];
    c3=note[i][var2];
    if (note[c1][0]==i)
        c2=note[c1][1];
    else
        c2=note[c1][0];
    if (((l[c1][6]!=vert)&&(l[c1][6]!=-1))||
        ((l[c2][6]!=vert)&&(l[c2][6]!=-1))||
        ((l[c3][6]!=vert)&&(l[c3][6]!=-1)))
        return 0;
    r1=distance(l,i,c3)+distance(l,c1,c2);
    r2=distance(l,i,c2)+distance(l,c1,c3);
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

int optimization(int **note,double **l,int n,double *len,int i,int var)
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
            R1=distance(l,i,r1)+distance(l,j,r2);
            R2=distance(l,i,r2)+distance(l,j,r1);
            if (R2<R1)
            {
                *len+=R2;
                *len-=R1;
                swipe(note,i,r1,r2);
                swipe(note,j,r2,r1);
                swipe(note,r1,i,j);
                swipe(note,r2,j,i);
                res++;
//                if (var2==0)
//                    return res;
            }
        }
        else
        {
            r1=note[i][0];
            r2=note[i][1];
            r3=note[j][0];
            r4=note[j][1];
            R1=distance(l,i,r3)+distance(l,i,r4)
            +distance(l,j,r1)+distance(l,j,r2);
            R2=distance(l,i,r1)+distance(l,i,r2)
            +distance(l,j,r3)+distance(l,j,r4);
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
//                if (var2==0)
//                    return res;
            }
        }
    }
    return res;
}

int check_sum(double **l,int **note,int *note0,int n,int i,int var,int v,int cap)
{
    int j,k,c,f3=0,f4=0,res=0,p,g=0,g1,pr1;
    double pr;
    c=note[i][var];
//    printf("\nPARA %d %d\n",i,c);
    if (note0[i]==1)
        f3++;
    note0[i]=1;
    if (note0[c]==1)
        f4++;
    note0[c]=1;
//    printf("%d",c);
//    if ((note[i][0]!=0)&&(note[i][1]!=0))
//    {
        swipe(note,i,c,0);
//    }
//    if ((note[c][0]!=0)&&(note[c][1]!=0))
//    {
        swipe(note,c,i,0);
//    }
    for (j=1;j<n;j++)
    {
//        printf("*%d*",j);
        if (note0[j]==1)
        {
            k=j;
            pr=0;
            pr+=l[k][2];
            p=0;
            g1=0;
//            printf("CYCLE%d!",k);
            while (k!=0)
            {
//                printf("%d%d ",p,k);
                if (note[k][0]==p)
                {
                    p=k;
                    k=note[k][1];
                    if ((int)pr+l[k][2]>cap)
                    {
                        g1++;
                        pr=0;
                    }
                    pr+=l[k][2];
                }
                else
                {
                    p=k;
                    k=note[k][0];
                    if ((int)pr+l[k][2]>cap)
                    {
                        g1++;
                        pr=0;
                    }
                    pr+=l[k][2];
                }
            }
            if ((pr==l[p][2])&&(g1==0))
                g+=2;
            else
            {
            pr1=(int)pr;
//            printf("\ng1=%d",g1);
            if (pr1!=0)
                g++;
            g+=g1;
            }
        }
    }
    if (g%2!=0)
        printf("HELP");
    g/=2;
//    printf("G=%d",g);
    if  (g>v)
        res++;
    swipe(note,i,0,c);
    swipe(note,c,0,i);
    if (f3==0)
        note0[i]=-1;
    if (f4==0)
        note0[c]=-1;
    return res; 
}

int search_edge(int **note,int *note0,double **l,int n,double *len,int v,int cap)
{
    int i=2,c1=-1,c2;
    double dist=-1,d;
    for (i=1;i<n;i++)
    {   
        if (note0[i]==1)
            continue;
        if (check_sum(l,note,note0,n,i,0,v,cap)!=1)
        {
        d=distance(l,i,note[i][0])-distance(l,0,i)
        -distance(l,0,note[i][0]);
//        printf("FFF%d\n",i);
        if ((dist==-1)||(d>dist))
        {
            dist=d;
            c1=i;
            c2=note[i][0];
        }
        }
        if (check_sum(l,note,note0,n,i,1,v,cap)!=1)
        {
//        printf("FFFF%d\n",i);
        d=distance(l,i,note[i][1])-distance(l,0,i)
        -distance(l,0,note[i][1]);
        if ((dist==-1)||(d>dist))
        {
            dist=d;
            c1=i;
            c2=note[i][1];
        }
        }
    }
    if(c1!=-1)
    {
    note0[c1]=1;
    note0[c2]=1;
//    printf("RAZB %d %d\n",c1,c2);
    swipe(note,c1,c2,0);
    swipe(note,c2,c1,0);
//    printf("%d-%d,%d\n",note0[c1],note[c1][0],note[c1][1]);
//    printf("%d-%d,%d\n",note0[c1],note[c2][0],note[c2][1]);
    *len-=dist;
    return 0;
    }
//    printf("STUP");
    return 1;
//    printf("alarm%d %d\n",c1,c2);
}

int check_res(double **l,int **note,int *note0,int n,int v,int cap)
{
    int j,k,p,g,count=0;
    double pr;
    for (j=1;j<n;j++)
        if (note0[j]==1)
        {
            k=j;
            pr=0;
            pr+=l[k][2];
            p=0;
            g=0;
//            printf("CYCLE%d!",k);
            while (k!=0)
            {
//                printf("%d ",p);
                if (note[k][0]==p)
                {   
                    p=k;
                    k=note[k][1];
                    if ((int)pr+l[k][2]>cap)
                    {
                        g++;
//                        printf("ST");
                        pr=0;
                    }
                    pr+=l[k][2];
                }
                else
                {
                    p=k;
                    k=note[k][0];
                    if ((int)pr+l[k][2]>cap)
                    {
                        g++;
//                        printf("ST");
                        pr=0;
                    }

                    pr+=l[k][2];
                }
            }
                if ((g!=0)||((g==1)&&(pr!=0)))
                {
//                    printf("VVVV %d\n",g);
                    return 1;
                }
                if ((pr==l[p][2])&&(g==0))
                    count+=2;
                else
                    count++;
//                printf("COU %d\n",count);
                if (count/2>v)
                {
//                    printf("ALARM");
                    return -1;
                }
        }
    return 0;
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

void group_tsp(double **l,int **group,int **note,int n,double *len,int vert,int var)
{
    int k1,k2,c,fin,ne,i,j;
    if (group[vert][0]==0)
        return;
    if (group[vert][0]==1)
    {
        k1=group[vert][1];
        *len+=2*distance(l,0,k1);
        return;
    }
    if (group[vert][0]==2)
    {
        k1=group[vert][1];
        k2=group[vert][2];
        *len+=distance(l,k1,k2)
        +distance(l,0,k1)+distance(l,0,k2);
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
        max_count(note,l,n,i,vert);
    c=search_start(l,n,vert);
    while ((c!=-2)&&((fin=search_min(note,l,n,c,len,vert))!=-2))
    {
        optimization(note,l,n,len,c,vert);
        optimization(note,l,n,len,fin,vert);
        c=search_start(l,n,vert);
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
        *len+=distance(l,c,j);
        l[c][4]++;
        l[j][4]++;
        note[c][1]=j;
        note[j][1]=c;
        optimization(note,l,n,len,c,vert);
        optimization(note,l,n,len,j,vert);
    }
    if (var==1)
    {
    for (c=0;c<2;c++)
    for (i=0;i<n;i++)
    {
        optimization(note,l,n,len,i,vert);
        triangle(note,l,i,0,len,vert);
        triangle(note,l,i,1,len,vert);
    }
    }
}
int opt2(int **note,int **group,double **g_opt,double **l,int cap,int n,int v)
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
//                printf("^");
                if (group[i][0]==0)
                    break;
                *len=0;
                *len1=0;
                if (g_opt[j][0]+l[group[i][k]][2]<cap)
                {
//                    printf("i%d j%d k%d\n",i,j,group[i][k]);
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
                    group_tsp(l,group,note,n,len,i,0);
                    group_tsp(l,group,note,n,len1,j,0);
/*                    for (k1=0;k1<v;k1++)
                    {
                        printf("%d-",k1);
                        for (c=0;group[k1][c]!=-1;c++)
                            printf("%d,",group[k1][c]);
                        printf("\n");
                    }*/
                    if (*len+*len1<g_opt[i][1]+g_opt[j][1])
                    {
                        g_opt[i][1]=*len;
                        g_opt[j][1]=*len1;
                        g_opt[i][0]-=l[fin][2];
                        g_opt[j][0]+=l[fin][2];
                        k--;
                        res++;
//                        printf("OK");
                       /* for (k1=0;k1<v;k1++)
                        {
                            printf("%d-",k1);
                            for (c=0;group[k1][c]!=-1;c++)
                                printf("%d,",group[k1][c]);
                            printf("\n");
                        }*/
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
                       /* for (k1=0;k1<v;k1++)
                        {
                            printf("%d-",k1);
                            for (c=0;group[k1][c]!=-1;c++)
                                printf("%d,",group[k1][c]);
                            printf("\n");
                        }*/
                    }
                }
            }
        }
    }
    return res;
}

int opt3(int **note,int **group,double **g_opt,double **l,int cap,int n,int v)
{
    int i,j,c,fin,k,k1,m,res=0;
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
                        group_tsp(l,group,note,n,len,i,0);
                        group_tsp(l,group,note,n,len1,j,0);
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
    int i,c,j,fin,ne,k;
    int **note,*note0,**m,**group;
    double *len=malloc(sizeof(double)),*len1,**g_opt;
    *len=0;
    len1=malloc(sizeof (double));
    note=malloc(n*sizeof(int*));
    note0=malloc(n*sizeof(int));
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
        g_opt[i][0]=0;
        for (j=1;j<n;j++)
        {
            group[i][j]=-1;
        }
    }
    for (i=0;i<n;i++)
        note0[i]=-1;
    for (i=0;i<n;i++)
        max_count(note,l,n,i,-1);
    c=search_start(l,n,-1);
    while ((c!=-2)&&((fin=search_min(note,l,n,c,len,-1))!=-2))
    {
        optimization(note,l,n,len,c,-1);
        optimization(note,l,n,len,fin,-1);
        c=search_start(l,n,-1);
    }
    c=-1;
    j=-1;
    for (i=0;i<n;i++)
    {
        if (l[i][4]==1)
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
    *len+=distance(l,c,j);
    l[c][4]++;
    l[j][4]++;
    note[c][1]=j;
    note[j][1]=c;
    ne=1;
//    optimization(note,l,n,len,c,-1);
//    optimization(note,l,n,len,j,-1);
    while(ne!=0)
    {
        ne=0;
        for (i=0;i<n;i++)
        {
            ne+=optimization(note,l,n,len,i,-1);
            ne+=triangle(note,l,i,0,len,-1);
            ne+=triangle(note,l,i,1,len,-1);
        }
    }
//    for (i=0;i<n;i++)
//        printf("%d-%d,%d\n",i,note[i][0],note[i][1]);
    c=0;
    fin=note[0][1];
//    printf("%d ",c);
    i=1;
    while (i!=n-1)
    {
        if (note[fin][1]==c)
        {
            c=fin;
            fin=note[fin][0];
        }
        else
        {
            c=fin;
            fin=note[fin][1];
        }
//        printf("%d ",c);
        i++;
    }
//    l[c][4]--;
//    l[j][4]--;
    note0[note[0][0]]=1;
    note0[note[0][1]]=1;
    i=check_res(l,note,note0,n,v,cap);
    c=search_edge(note,note0,l,n,len,v,cap);
    if (c==1)         
    {
        sort_q(0,n-1,m,1);
        for (i=n-1;i>0;i--)
        {
            sort_q(0,v-1,group,0);
            for(j=v-1;group[j][0]+m[i][1]>cap;j--);
            for(c=1;group[j][c]!=-1;c++);
            group[j][c]=m[i][0];
            group[j][0]+=m[i][1];
        }
       /* for (i=0;i<v;i++)
        {   
            printf("%d-",i);
            for (j=0;group[i][j]!=-1;j++)
                printf("%d,",group[i][j]);
            printf("\n");
        }*/
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
  /*      for (i=0;i<v;i++)
        {    
            printf("%d-",i);
            for (j=0;group[i][j]!=-1;j++)
                printf("%d,",group[i][j]);
            printf("\n");
        }*/
        *len1=0;
        for (i=0;i<v;i++)
        {   
            *len=0;
            group_tsp(l,group,note,n,len,i,0);
            g_opt[i][1]+=*len;
            *len1+=*len;
        }
        if (n/100==1)
            j=7;
        else
//            if (n/100==2)
//                c=3;
//            else
                j=2;
        fin=-1;
        for (k=0;k<j;k++)
        {
        c=opt2(note,group,g_opt,l,cap,n,v);
        if (j!=2)
            fin=opt3(note,group,g_opt,l,cap,n,v);
        else
            if(c!=0)
                c=opt2(note,group,g_opt,l,cap,n,v);
        if (((c==0)&&(fin==0))||((c==0)&&(j==2)))
            break;
        }
        *len1=0;
        for (k=0;k<v;k++)
        {
            *len=0;
            group_tsp(l,group,note,n,len,k,1);
            *len1+=*len;
        }
        printf("%.3lf\n",*len1);
    }
    else
    {
        while ((c==0)&&(i!=0))
        {
            c=search_edge(note,note0,l,n,len,v,cap);
            i=check_res(l,note,note0,n,v,cap);
        }
        ne=0;
        for (j=1;j<n;j++)
        {
            if (note0[j]==1)
            {   
                c=j;
                fin=0;
                k=1;
                while (c!=0)
                {   
                    group[ne][k]=c;
                    g_opt[ne][0]+=l[c][2];
                    l[c][6]=ne;
                    if (note[c][0]==fin)
                    {
                        fin=c;
                        c=note[c][1];
                    }
                    else
                    {
                        fin=c;
                        c=note[c][0];                 
                    }
                    k++;
                }
                ne++;
           }
           note0[fin]=-1;
        }
        for (k=0;k<v;k++)
        {
            for (j=1;group[k][j]!=-1;j++);
            j--;
            group[k][0]=j;
        }
/*        for (k=0;k<v;k++)
        {   
            printf("%d-",k);
            for (j=0;group[k][j]!=-1;j++)
                printf("%d,",group[k][j]);
            printf("\n");
        }*/
        *len1=0;
        for (k=0;k<v;k++)
        {
            *len=0;
            group_tsp(l,group,note,n,len,k,0);
            g_opt[k][1]=*len;
            *len1+=*len;
//            printf("%.0lf %.3lf\n",g_opt[k][0],g_opt[k][1]);
        }
        if (n/100==1)
            j=7;
        else 
//            if(n/100==2)
//                c=3;
//            else
                j=2;
        for (k=0;k<j;k++)
        {
        c=opt2(note,group,g_opt,l,cap,n,v);
        if (j!=2)
            fin=opt3(note,group,g_opt,l,cap,n,v);
        else
            if (c!=0)
                c=opt2(note,group,g_opt,l,cap,n,v);
        if ((((c==0)&&(fin==0)))&&((c==0)&&(j==2)))
            break;
        }
        *len=0;
        for (k=0;k<v;k++)
        {   
            *len1=0;
            group_tsp(l,group,note,n,len1,k,1);
            *len+=*len1;
//            printf("%.0lf %.3lf\n",g_opt[k][0],g_opt[k][1]);
        }
//        if (i!=0)
//            printf("oh no\n");
        printf("%.3lf\n",*len);
    }
/*    if (check_cycle(note,l,n,c,j)!=1)
        printf ("FATAL ERROR\n");
    else
        printf("OK\n");
    l[c][4]++;
    l[j][4]++;
    note[c][1]=j;
    note[j][1]=c;*/
/*    for (i=0;i<n;i++)
    {
        printf("%d-%d,%d,",i,note[i][0],note[i][1]);
        if (i==0)
            for (j=1;j<n;j++)
                if (note0[j]==1)
                    printf("%d,",j);
        printf("\n");                    
    }*/
    delete_list(l,n);
    delete_n(note,n);
    free(len);
}

