int sum(int a,int b){
	int c = a+b;
	return c;
}
int arithmetic(int a,int b,int c,int d,int e){
	int f = (a+b)+(c*d)-e;
	return f;
}
int pointer(int *a,int *b,int *c){
	int d = *a + *b + *c;
	return d;
}
void reset_handler(void)
{
	int a = 100;
	int b = 200;
	int c = 300;
	sum(10,20);
	arithmetic(1,2,3,4,5);
	pointer(&a,&b,&c);
	while (1)
		;
}
