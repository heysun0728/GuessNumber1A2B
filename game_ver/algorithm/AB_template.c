#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
//根據需求自由使用--->
int value[10000];
int groupnum[16];
//<---根據需求自由使用
//數字是否符合規則
int IsInvalid(int guessNum){
    int n=4;
    int num[10]={0};
    while(n--){
        if(num[guessNum%10]>0) return 1;
        num[guessNum%10]++;
        guessNum/=10;
    }
    return 0;
}
char* check(char* answer, char* ground_truth){
    static char result[5];
    //實作批改副程式--->
    char* truth=ground_truth;
    int a=0,b=0,i;
    //猜
    while((*answer)!='\0'){
        if((*answer)==(*ground_truth)){
            a++;
        }else{
            for(i=0;i<4;i++){
                if((*answer)==truth[i]) b++;
            }
        }
        answer++;
        ground_truth++;
    }
    result[0]=a+'0';
    result[1]='A';
    result[2]=b+'0';
    result[3]='B';
    result[4]='\0';
    return result;
    //<---實作批改副程式
    return result;//如果answer不符合正確格式->result="XXXX"
}
int group(char* r){
    if(strcmp(r,"0A0B")==0) return 1;
    else if(strcmp(r,"0A1B")==0) return 2;
    else if(strcmp(r,"0A2B")==0) return 3;
    else if(strcmp(r,"0A3B")==0) return 4;
    else if(strcmp(r,"0A4B")==0) return 5;
    else if(strcmp(r,"1A0B")==0) return 6;
    else if(strcmp(r,"1A1B")==0) return 7;
    else if(strcmp(r,"1A2B")==0) return 8;
    else if(strcmp(r,"1A3B")==0) return 9;
    else if(strcmp(r,"2A0B")==0) return 10;
    else if(strcmp(r,"2A1B")==0) return 11;
    else if(strcmp(r,"2A2B")==0) return 12;
    else if(strcmp(r,"3A0B")==0) return 13;
    else if(strcmp(r,"3A1B")==0) return 14;
    else if(strcmp(r,"4A0B")==0) return 15;
}
void itoa_2(int a,char* g){
    if(a>=1000){
        itoa(a,g,10);
    }
    else{
        g[0]='0';
        itoa(a,&g[1],10);
        //printf("g=%s\n",g);
    }
}

void MakeGroup(int *groupnum,char *answer){//將剩餘有可能勝出的 按照我們猜測的數字 分類
    char v[5];
    int i,g;
    for(i=0;i<=15;i++)
        groupnum[i]=0;
    for(i=123;i<10000;i++){
        if(value[i]<=0)continue;
        itoa_2(i,v);
        g=group(check(v,answer));
        value[i]=g;
        groupnum[g]++;
    }
}
char* guess(char* result){//對每個正解第一次猜測時->result="0000"
    static char answer[5];
    char r[5];
    strcpy(r,result);
    char* c;
    int i,ProbTotal=0;
    //實作猜測副程式--->
    //第一次都固定猜0123
    if(strcmp(result,"0000")==0){
        answer[0]='\0';
        strcat(answer,"0123");
        for(i=0;i<10000;i++){
            if(IsInvalid(i)) value[i]=0;
            else{
                value[i]=1;
            }
        }
        MakeGroup(groupnum,answer);
        /*for(i=0;i<=15;i++){
            printf("%d ",groupnum[i]);
        }*/
        value[123]=-1;
        //printf("guess=%s\n",answer);
        return answer;
    }
    //篩選可能答案集合
    int x=group(result);
    //printf("x=%d\n",x);
    int y=0;
    for(i=123;i<10000;i++){
        if(value[i]<=0) continue;
        if(value[i]!=x){
            value[i]=0;
            continue;
        }
        y++;
        //printf("%d ",i);
    }
    //printf("y=%d\n",y);
    //選擇要使用哪個數字
    int a,max,min;
    int minA,minA_Value;
    char g[5];
    minA_Value=10000;
    //printf("value[123]=%d\n",value[123]);
    for(a=123;a<10000;a++){
        if(IsInvalid(a)) continue;
        if(value[a]<0) continue;
        max=0;min=10000;
        itoa_2(a,g);
        MakeGroup(groupnum,g);
        for(i=1;i<15;i++){
            if(groupnum[i]>max) max=groupnum[i];
            if(groupnum[i]<min) min=groupnum[i];
        }
        if(max-min<minA_Value){
            //printf("max=%d,min=%d,a=%d\n",max,min,a);
            minA_Value=max-min;
            minA=a;
            /*for(i=0;i<=15;i++){
                printf("%d ",groupnum[i]);
            }
            printf("\n");*/
        }
    }
    //確定猜的數字是多少，進行資料重整
    value[minA]=-1;
    itoa_2(minA,g);
    MakeGroup(groupnum,g);
    //建立answer
    answer[4]='\0';
    for(i=3;i>=0;i--){
        answer[i]=minA%10+'0';
        minA/=10;
    }
    //printf("guess=%s\n",answer);
    //<---實作猜測副程式
    return answer;
}
//請勿更動--->
char* generate_ground_truth(){
    static char numbers[]="0123456789";
    static char ground_truth[5];
    int i,j;
    for(i=0;i<9;i++){
        j=i+rand()/(RAND_MAX/(9-i)+1);
        char c=numbers[j];
        numbers[j]=numbers[i];
        numbers[i]=c;
    }
    strncpy(ground_truth,numbers,4);
    ground_truth[4]='\0';
    return ground_truth;
}
int main(){
    int i,total_guess=0;
    char g[5];
    srand(time(0));
    for(i=0;i<50;i++){
        //生產正確答案
        //generate_ground_truth()
        char *ground_truth=generate_ground_truth(),*result="0000";
        printf("%s\n",ground_truth);
        while(1){
            result=check(guess(result),ground_truth);
            total_guess++;
            //printf("%s\n",result);
            if(strcmp(result,"4A0B")==0)break;
            //printf("\n");
        }
    }
    printf("%d\n",total_guess);
}
