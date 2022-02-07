double boxit(double x1, double y1, double x2, double y2, int icolor)
{
  double HatchWidth = .1;
  double angle = 45./180.*3.14159;
  double angle2 = 3.14159/2. - angle;
  double slope = tan(angle2);
  cout << slope << endl;
  double xHatch = x1;
  double yHatch = y2;
  TLine line;
  line.SetLineWidth(2.);
  line.SetLineColor(icolor);

  cout << "x1= " << x1 << " x2 = " << x2 << " y1 = " << y1 << " y2= " << y2 << endl;
  for (;;)
    {

      xHatch += HatchWidth*cos(angle);
      yHatch -= HatchWidth*sin(angle);

      cout << "xHatch = " << xHatch << " yHatch= " << yHatch << endl;

      //left intercept
      double Lefty = slope*(x1-xHatch) + yHatch;
      double Leftx = x1;
      cout << "left0 " << Leftx << " " << Lefty << endl;
      if ( Lefty < y1)
	{
          Lefty = y1;
	  Leftx = (y1-yHatch)/slope + xHatch;
          cout << "left1 " << Leftx << " " << Lefty << endl;
          if (Leftx >= x2) break;
	}

      //right intercept
      double Rightx = (y2-yHatch)/slope + xHatch;
      double Righty = y2;
      cout << "Right0 " << Rightx << " " << Righty << endl;
      if (Rightx > x2)
	{
        Righty = slope*(x2-xHatch) + yHatch;
        Rightx = x2;
        cout << "Right1 " << Rightx << " " << Righty << endl;
	}
      line.DrawLine(Leftx,Lefty,Rightx,Righty);  
    }


  return 0;
}
