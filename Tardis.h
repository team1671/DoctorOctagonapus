
void DoctaEight::drive(void)
{				
	if (pilot.GetY() > 0 && pilot.GetX() >= 0)//forward right
	{
		lefty.Set(pilot.GetY() * negate * decrement);//left motors full
		leftyB.Set(pilot.GetY() * negate * decrement);//left motors full
		righty.Set(( pilot.GetY() - pilot.GetX() * 2 ) * negate * decrement);//right motors full dec by twiceX abs X
		rightyB.Set(( pilot.GetY() - pilot.GetX() * 2 ) * negate * decrement);
	}//(so up to x = 0 right rev and when y negative, backward curve)
	else if (pilot.GetY() < 0 && pilot.GetX() > 0)//backward left
	{
		righty.Set(pilot.GetY() * negate * -1 * decrement);
		rightyB.Set(pilot.GetY() * negate * -1 * decrement);
		lefty.Set((pilot.GetY() - pilot.GetX() * 2) * negate * decrement);
		leftyB.Set((pilot.GetY() - pilot.GetX() * 2) * negate * decrement);
	}
	else if (pilot.GetY() > 0 && pilot.GetX() <= 0)//forward left
	{
		righty.Set(pilot.GetY() * negate * decrement);
		rightyB.Set(pilot.GetY() * negate * decrement);
		lefty.Set((pilot.GetX() * 2 + pilot.GetY()) * negate * decrement);
		leftyB.Set((pilot.GetX() * 2 + pilot.GetY()) * negate * decrement);
	}
	else if (pilot.GetY() < 0 && pilot.GetX() < 0)//back right
	{
		lefty.Set(pilot.GetY() * negate * -1 * decrement);//left full back
		leftyB.Set(pilot.GetY() * negate * -1 * decrement);//left full back
		righty.Set(( pilot.GetY() + pilot.GetX() * 2 ) * negate * decrement);//right morots full dec by twice abs X
		rightyB.Set(( pilot.GetY() + pilot.GetX() * 2 ) * negate * decrement);
	}
	else
	{
		righty.Set(pilot.GetY() *negate * decrement);
		rightyB.Set(pilot.GetY() *negate * decrement);
		lefty.Set(pilot.GetY() *negate * decrement);
		leftyB.Set(pilot.GetY() *negate * decrement);
	}
}
