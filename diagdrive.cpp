float x=pilot.GetTwist();
float y=pilot.GetY();

float rtot=1.41421356/2;//root(2)/2

float forward=x*rtot+y*rtot;
float MChammer=x*rtot-y*rtot//hammerslide- side to side

//setmotors
