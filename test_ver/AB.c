#include <stdio.h>
#include <string.h>
char str[200][5];
int total_guess;
int IsInvalid(char* ground_truth){
    int num[10]={0};
    while((*ground_truth)!='\0'){
        //printf("num[%d]=%d\n",(*ground_truth)-'0',num[(*ground_truth)-'0']);
        if(num[(*ground_truth)-'0']>0) return 1;
        num[(*ground_truth)-'0']++;
        ground_truth++;
    }
    return 0;
}
int IsGuessed(char* ground_truth){
    int i;
    for(i=0;i<total_guess;i++){
        if(strcmp(str[i],ground_truth)==0) return 1;
    }
    for(i=0;i<4;i++){
        str[total_guess][i]=ground_truth[i];
    }
    str[total_guess][4]='\0';
    return 0;
}

char* check(char* answer, char* ground_truth){
    static char result[50];
    char* ans=answer;
    int a=0,b=0,i;
    result[0]='\0';//將字串歸零
    //若有數字重複
    if(IsInvalid(ground_truth)){
        strcat(result,ground_truth);
        strcat(result," is an invalid guess.");
        return result;
    }
    //若已經猜過
    if(IsGuessed(ground_truth)){
        strcat(result,ground_truth);
        strcat(result," is already guessed.");
        return result;
    }
    //猜
    while((*ground_truth)!='\0'){
        if((*answer)==(*ground_truth)){
            a++;
        }else{
            for(i=0;i<4;i++){
                if((*ground_truth)==ans[i]) b++;
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
}
int main(){
    //freopen("AB.in","r",stdin);
    //freopen("AB_test.out","w",stdout);
    int nCase;
    char ans[5];
    char guess[5];
    char *result;
    scanf("%d",&nCase);
    while(nCase--){
        total_guess=0;
        scanf("%s",ans);
        while(1){
            scanf("%s",guess);
            result=check(ans,guess);
            total_guess++;
            printf("%s\n",result);
            if(strcmp(result,"4A0B")==0)break;
        }
        if(nCase) printf("\n");
    }

}

