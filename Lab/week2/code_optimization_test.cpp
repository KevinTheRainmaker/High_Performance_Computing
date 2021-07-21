#include<iostream>
#include<cstdlib>

#define LEFT_MARGINE_FOR_X -10000.0
#define RIGHT_MARGINE_FOR_X 10000.0
#define LEFT_MARGINE_FOR_Y -10000.0
#define RIGHT_MARGINE_FOR_Y 10000.0

using namespace std;

int main()
{
	//Get the constant value
	cout<<"Enter the constant value b>0"<<endl;
	cout<<"b->"; double dB; cin>>dB;

	if(dB<=0) return EXIT_FAILURE;
	if(dB>1000) return EXIT_FAILURE;

	//This is the potential maximum value of the function
	//and all other values could be bigger or smaller
	double dMaximumValue = (LEFT_MARGINE_FOR_X * LEFT_MARGINE_FOR_X + LEFT_MARGINE_FOR_Y * LEFT_MARGINE_FOR_Y) / (LEFT_MARGINE_FOR_Y * LEFT_MARGINE_FOR_Y + dB);

	double dMaximumX = LEFT_MARGINE_FOR_X;
	double dMaximumY = LEFT_MARGINE_FOR_Y;

	for(double dX = LEFT_MARGINE_FOR_X ; dX<=RIGHT_MARGINE_FOR_X ; dX+=1.0) {
		for(double dY = LEFT_MARGINE_FOR_Y ; dY<=RIGHT_MARGINE_FOR_Y ; dY+=1.0) {
			if(dMaximumValue < ((dX*dX + dY*dY)/(dY*dY + dB)))
			{
				dMaximumValue=((dX*dX + dY*dY)/(dY*dY + dB));
				dMaximumX=dX;
				dMaximumY=dY;
			}
		}
	}

	cout<<"Maximum value of the function is = "<<dMaximumValue<<endl;
	cout<<endl<<endl;
	cout<<"Value of X: "<<dMaximumX<<endl;
	cout<<"Value of Y: "<<dMaximumY<<endl;

	return EXIT_SUCCESS;
}

