#include <iostream>

using namespace std;

int main(void)
{
	int x=0, y=0, w=0, h=0;
	int min;
	cin >> x >> y >> w >> h;
	int xdiff = w - x;
	int ydiff = h - y;
	int arr[4] = {xdiff, ydiff, x, y};

	min = arr[0];

	for (int i = 0; i < 4; i++)
	{
		if (min > arr[i])
		{
			min = arr[i];
		}
	}
	cout << min;
}