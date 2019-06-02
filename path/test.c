int test(int number){
int a;
int b;
a=1;
b=2;
return a;
}
int main(void){
int a;
int b;
a=3;
b=4;
while(a-2*b>b){
a=a-1;
}
a=a+b;
if(a>b){
a=a+2;
}
else{
a=test(b);
}
return 0;
}

