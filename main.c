#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "solver.h"

int main()
{
    int N,V,c,j;
    double **list;
    char files[67][13]={"vrp_101_10_1","vrp_101_11_1",
    "vrp_101_11_2","vrp_101_11_3","vrp_101_11_4","vrp_101_14_1",  
    "vrp_101_14_2","vrp_101_8_1","vrp_121_7_1","vrp_135_7_1",  
    "vrp_151_12_1","vrp_151_12_2","vrp_151_14_1","vrp_151_14_2",
    "vrp_151_14_3","vrp_151_15_1","vrp_16_3_1","vrp_16_5_1",  
    "vrp_200_16_1","vrp_200_16_2","vrp_200_17_1","vrp_200_17_2",  
    "vrp_21_4_1","vrp_21_6_1","vrp_22_4_1","vrp_22_6_1",
    "vrp_23_3_1","vrp_23_5_1","vrp_241_22_1","vrp_253_27_1",  
    "vrp_256_14_1","vrp_26_8_1","vrp_30_3_1","vrp_30_4_1",  
    "vrp_301_28_1","vrp_31_9_1","vrp_321_30_1","vrp_324_16_1",   
    "vrp_33_3_1","vrp_33_4_1","vrp_33_5_1","vrp_36_11_1",   
    "vrp_361_33_1","vrp_386_47_1","vrp_397_34_1","vrp_5_4_1",  
    "vrp_51_5_1","vrp_72_4_1","vrp_76_10_1","vrp_76_10_2",  
    "vrp_76_14_1","vrp_76_14_2","vrp_76_15_1","vrp_76_7_1",  
    "vrp_76_7_2","vrp_76_8_1","vrp_76_8_2","vrp_76_9_1",
    "vrp_76_9_2","vrp_76_9_3","vrp_400_18_1","vrp_41_14_1",  
    "vrp_421_41_1","vrp_45_4_1","vrp_481_38_1","vrp_484_19_1",  
    "vrp_48_4_1"};
    FILE *f;
    for(j=0;j<67;j++)
    {
        f=fopen(files[j],"r");
        fscanf(f,"%d %d %d",&N,&V,&c);
        list=create_list(N,f);
//        printf("%s-",files[j]);
        fclose(f);
        solution(list,N,V,c);
    }
    return 0;
}
