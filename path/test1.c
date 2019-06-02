int f;
int g;
int program(int a,int b,int c)
{
int i;
int j;
i=0; 	
if(a>(b+c))
{
j=a+(b*c+1);
}
else
{
j=a;
}
while(i<=100)
{
i=j*2;
}
return i;
}
int demo(int e)
{
f=f+2;
return f*2;
}

void main(void)
{
int a;
int b;
int c;
a=3;
b=4;
c=2;
a=program(a,b,demo(c));
return;
}



,demo(c))
	return;
}

