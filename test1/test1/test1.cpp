// test1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdio.h"
#include "string.h"
#include "process.h"
#include "iostream"

using namespace std;

void thread1(void *parameter);
void predict(char buf[1024]);
void learn(char *buf, char *buf1);

char ibuf[1024];
int _tmain(int argc, _TCHAR* argv[])
{
	memset(ibuf, 0, 1024);
	char *p = ibuf;
	_beginthread(thread1, 0, NULL);
	while (1)
	{
		cin>>ibuf;
	}
	return 0;
}


void thread1(void *parameter)
{
	char buf[1024];
	unsigned int len;
	char *p1 = buf;
	char buf1[1024];
	while (1)
	{
		memcpy(buf, ibuf, 1024);
		len = strlen(buf);
	
		predict(buf1);
		learn(buf, buf1);
	}

}
