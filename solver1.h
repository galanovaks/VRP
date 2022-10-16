double **create_list(int n,FILE *f)
{
    double **list,x,y;
    int i;
    list=malloc(n*sizeof(double*));
    for (i=0;i<n;i++)
    {
        list[i]=malloc(6*sizeof(double));
        fscanf(f,"%lf %lf %lf",&list[i][2],&list[i][0],&list[i][1]);
        if (i==0)
        {
            x=list[0][0];
            y=list[0][1];
        }
        list[i][0]-=x;
        list[i][1]-=y;
        list[i][3]=-1;
        list[i][4]=0;
        list[i][5]=-1;
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

/*int **create_graph(int n)
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
*/
double distance(double **l,int i,int j)
{
    return sqrt(pow(l[j][0]-l[i][0],2)+pow(l[j][1]-l[i][1],2));
}

void max_count(int **note,double **l,int n,int c)
{
    double d,dist=-1,dm=-1;
    int i;
    for (i=0;i<n;i++)
        if ((i!=c)&&(l[i][4]!=2)&&(note[c][0]!=i))
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

int search_start(double **l,int n)
{
    int i,res=-2;
    double dist=-1;
    for (i=0;i<n;i++)
    {   
        if (l[i][4]==2)
            continue;
        if ((dist<l[i][3])||((dist==l[i][3])
        &&(l[res][2]<l[i][2])))
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

int search_min(int **note,double **l,int n,int c,double *len)
{
    int i,res=-2;
    double d,dist=-1;
    for (i=0;i<n;i++)
    {
        if ((i==c)||(l[i][4]==2)||(note[c][0]==i))
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
            max_count(note,l,n,res);
        if ((l[c][3]==dist)||(l[c][5]==dist))
            max_count(note,l,n,c);
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

int triangle(int **note,double **l,int i,int var,double *len)
{
    int c1,c2,c3,var2,res=0;
    double r1,r2;
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

int optimization(int **note,double **l,int n,double *len,int i)
{
    int j,r1,r2,r3,r4,res=0;
    double R1,R2;
    if (l[i][4]!=2)
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

void solution(double **l,int n,int v,int cap)
{
    int i,c,j,fin,ne;
    int **note,*note0;
    double res=0,*len=malloc(sizeof(double));
    *len=0;
    for (i=1;i<n;i++)
        res+=l[i][2];
    ne=(int)res;
    ne/=cap;
    if (ne%cap!=0)
        ne++;
    note=malloc(n*sizeof(int*));
    note0=malloc(n*sizeof(int));
    for (i=0;i<n;i++)
    {
        note[i]=malloc(2*sizeof(int));
        note[i][0]=-1;
        note[i][1]=-1;
    }
    for (i=0;i<n;i++)
        note0[i]=-1;
    for (i=0;i<n;i++)
        max_count(note,l,n,i);
    c=search_start(l,n);
    while ((c!=-2)&&((fin=search_min(note,l,n,c,len))!=-2))
    {
//        optimization(note,l,n,len,c);
//        optimization(note,l,n,len,fin);
        c=search_start(l,n);
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
    while(ne!=0)
    {
        ne=0;
        for (i=0;i<n;i++)
        {
            ne+=optimization(note,l,n,len,i);
            ne+=triangle(note,l,i,0,len);
            ne+=triangle(note,l,i,1,len);
        }
    }
//    for (i=0;i<n;i++)
//        printf("%d-%d,%d\n",i,note[i][0],note[i][1]);
    c=0;
    fin=note[0][1];
//    printf("%d ",c);
    i=1;
    while (i!=n)
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
//        printf("%d *%.0lf* ",c,l[c][2]);
        i++;
    }
//    l[c][4]--;
//    l[j][4]--;
    note0[note[0][0]]=1;
    note0[note[0][1]]=1;
    i=check_res(l,note,note0,n,v,cap);
    c=search_edge(note,note0,l,n,len,v,cap);;
    if (c==1)         
        printf("VERY SAD\n");
    while ((c==0)&&(i!=0))
    {
        c=search_edge(note,note0,l,n,len,v,cap);
        i=check_res(l,note,note0,n,v,cap);
    }
    if (i==-1)
        printf("oh no\n");
    else
        printf("yes\n");
    printf("Result:%.3lf %d\n",*len,i);
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

