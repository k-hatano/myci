#include "myci_comp.h"

int MILLION = 1000000;
int REPEAT = 10000;

int myexp()
{
	int e = 0;
	int i = 1;
	int ii = 1;
	while (MILLION / ii > 0)
	{
		e = e + MILLION / ii;
		ii = ii * i;
		i = i + 1;
	}
	return e;
}

int main()
{
	int j;
	int e;
	j = 0;
	while (j < REPEAT)
	{
		e = myexp();
		j = j + 1;
	}
	print(e);
	return 0;
}
