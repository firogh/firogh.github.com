int utilfunc(long x, long y, long z)
{
	int  a;
	a += x + y + z;
	
	return a;
}

int zfuc()
{
	int y = 1;
	y += utilfunc(10, 100, 1000);
	return y;
}
long myfunc(long a, long b, long c, long d,
            long e, long f, long g, long h)
{
    long xx = a * b * c * d * e * f * g * h;
    long yy = a + b + c + d + e + f + g + h;
    long zz = xx + yy + zfuc();
    return zz + 20;
}
int main(void)
{
	int x = 1;
	
	return x + myfunc(1,2,3,4,5,6,7,8);
}
