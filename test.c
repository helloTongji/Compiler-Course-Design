int a;
int b;
int e[5];
char hj;
int program(int a,int b,int c)
{
	int i;
	int j;
	int m;
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
		j=i;
	}
	return (i);
}

int demo(int a)
{
	int i;
	a=a+2;
	a=a*2;
	i=0;
	return (a);
}

void main()
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