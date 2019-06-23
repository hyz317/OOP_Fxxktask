#pragma once
#include <iostream>
bool _Equal(string a,string b)
{
	int l1=a.length();
	int l2=b.length();
	if(l1!=l2)return false;
	for(int i=0;i<l1;i++)
	{
		if(a[i]>='a'&&a[i]<='z')a[i]-=32;
		if(b[i]>='a'&&b[i]<='z')b[i]-=32;
		if(a[i]!=b[i])return false;
	}
	return true;
}
