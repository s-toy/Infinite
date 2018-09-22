#include <iostream>
#include <windows.h>
#include <mmsystem.h>

using namespace std;

int main()
{
	BOOL flag = PlaySound("D:\\Media\\Audio\\31.wav", NULL, SND_ASYNC | SND_LOOP);

	getchar();

	return 0;
}