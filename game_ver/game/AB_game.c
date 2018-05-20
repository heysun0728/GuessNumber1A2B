#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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
void check(char* answer, char* ground_truth,char *result){
    //static char result[5];
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
    //<---實作批改副程式
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
    char v1[5];
    int i,g;
    for(i=0;i<=15;i++)
        groupnum[i]=0;
    for(i=123;i<10000;i++){
        if(value[i]<=0)continue;
        itoa_2(i,v);
        check(v,answer,v1);
        g=group(v1);
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
    int x=group(r);
    //printf("result=%s ",r);
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
void mode2(){
    int total_guess=0;
    char g[5];
    char result[5]="0000";
    srand(time(0));
    char *ground_truth=generate_ground_truth();
    printf("--------------遊戲開始--------------\n");
    while(1){
        //選手猜的部分
        total_guess++;
        printf("[第%d回合]\n",total_guess);
        printf("您的猜測：");
        while(1){
            scanf("%s",g);
            if(strlen(g)!=4||IsInvalid(atoi(g))){
                printf("您輸入的數值不合規則，請再次輸入：");
            }else{
                break;
            }
        }
        check(g,ground_truth,result);
        printf("結果是：%s\n\n",result);
        if(strcmp(result,"4A0B")==0){
            printf("恭喜你贏了。你只猜了%d次\n",total_guess);
            break;
        }
    }
}
void mode1(){
    printf("\n---------------出題目---------------\n");
    char g[5];
    char *gc;
    char result[5]="0000";
    char resultC[5]="0000";
    int total_guess=0,total_guessC=0;

    //產生答案
    //選手要猜的
    srand(time(0));
    char *ground_truth=generate_ground_truth();
    //電腦要猜的
    char ground_truth_forC[5];
    while(1){
        printf("給電腦一個四位數值來猜測(數字不能重複)：");
        scanf("%s",ground_truth_forC);
        if(strlen(ground_truth_forC)!=4||IsInvalid(atoi(ground_truth_forC))){
            printf("您輸入的數字不合規則，再");
        }else{
            break;
        }
        //t=1208;
    }
    //printf("%s\n",ground_truth_forC);
    //ground_truth_forC=generate_ground_truth();
    //開始猜測
    printf("電腦也給您出好了一組數字，開始遊戲吧\n\n");
    printf("--------------遊戲開始--------------\n");
    while(1){
        //選手猜的部分
        total_guess++;
        printf("[第%d回合]\n",total_guess);
        printf("您的猜測：");
        while(1){
            scanf("%s",g);
            if(strlen(g)!=4||IsInvalid(atoi(g))){
                printf("您輸入的數值不合規則，請再次輸入：");
            }else{
                break;
            }
        }
        check(g,ground_truth,result);
        printf("結果是：%s\n\n",result);
        if(strcmp(result,"4A0B")==0){
            printf("恭喜你贏了。你只猜了%d次\n",total_guess);
            break;
        }
        //電腦猜
        total_guessC++;
        gc=guess(resultC);
        printf("電腦猜測：%s\n",gc);
        check(gc,ground_truth_forC,resultC);
        printf("結果是：%s\n\n",resultC);
        if(strcmp(resultC,"4A0B")==0){
            printf("你輸給了電腦。電腦猜了%d次就答對了。\n",total_guessC);
            break;
        }
        printf("------------------------------------\n");
    }
}
int main(){
    int gameMode;
    printf("----------請選擇遊戲模式------------\n1:與電腦競賽模式\n2:單人遊戲模式\n請輸入選擇：");
    while(1){
        scanf("%d",&gameMode);
        if(gameMode==1){
            mode1();
            break;
        }
        if(gameMode==2){
            mode2();
            break;
        }else{
            printf("您輸入了錯誤的值，請再次輸入：");
        }
    }
}

