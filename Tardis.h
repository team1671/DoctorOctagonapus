void DoctaEight::drive(void)
{
	GetWatchdog().Kill();
	
	if (pilot.GetDirectionDegrees()/180 <1)
		xCircle = pilot.GetDirectionDegrees()/180;
	else if (pilot.GetDirectionDegrees()/180 >1)
		xCircle = -1+(pilot.GetDirectionDegrees()-180)/180;
	else {xCircle = 0;}
	
	if (pilot.GetTwist() > 0 && xCircle > 0)//forward right
	{
		lefty.Set(pilot.GetTwist() * negate  );//left motors full
		leftyB.Set(pilot.GetTwist() * negate  );//left motors full
		righty.Set(( pilot.GetTwist() - xCircle * 4 ) * negate  );//right motors full dec by twiceX abs X
		rightyB.Set(( pilot.GetTwist() - xCircle * 4 ) * negate  );
	}//(so up to x = 0 right rev and when y negative, backward curve)
	else if (pilot.GetTwist() < 0 && xCircle > 0)//backward left (b/c car drive)
	{
		righty.Set(pilot.GetTwist() * negate);
		rightyB.Set(pilot.GetTwist() * negate);
		lefty.Set((pilot.GetTwist() + (xCircle-1) * 4) * negate  );
		leftyB.Set((pilot.GetTwist() + (xCircle-1) * 4) * negate  );
	}
	else if (pilot.GetTwist() > 0 && xCircle < 0)//forward left
	{
		righty.Set(pilot.GetTwist() * negate  );
		rightyB.Set(pilot.GetTwist() * negate  );
		lefty.Set((pilot.GetTwist() - (1+xCircle)*4) * negate  );
		leftyB.Set((pilot.GetTwist() - (1+xCircle)*4) * negate  );
	}
	else if (pilot.GetTwist() < 0 && xCircle < 0)//back right
	{
		lefty.Set(-pilot.GetTwist() * negate);//left full back
		leftyB.Set(-pilot.GetTwist() * negate);//left full back
		righty.Set(( pilot.GetTwist() + xCircle * 4 ) * negate  );//right morots full dec by twice abs X
		rightyB.Set(( pilot.GetTwist() + xCircle * 4 ) * negate  );
	}
	else
	{
		righty.Set(pilot.GetTwist() *negate  );
		rightyB.Set(pilot.GetTwist() *negate  );
		lefty.Set(pilot.GetTwist() *negate  );
		leftyB.Set(pilot.GetTwist() *negate  );
	}
}
