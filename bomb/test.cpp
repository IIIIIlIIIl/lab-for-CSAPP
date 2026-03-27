#include<iostream>
#include<cmath>

int res;

int fun4(int x,int y){
    int val=floor((x-y)/2.0)+y;
    if(val>res)return 2*fun4(x-1,y);
    else if(val==res)return 0;
    else return 2*fun4(x,val+1)+1;
}

int main(){
    
    return 0;
}