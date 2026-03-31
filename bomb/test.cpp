#include<iostream>
#include<cmath>

int res;

int fun7(int *x){
    if(x==nullptr)return 0xfffffff;
    int val=*x;
    if(val<res)return 2*fun7(x+16)+1;
    else if(val==res)return 0;
    else return 2*fun7(x+8);
}

int main(){
    
    return 0;
}