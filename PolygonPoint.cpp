#include <iostream>
#include <cmath>
using namespace std;


struct Point 
{ 
    double h; 
    double v; 
};
/*
   Return the angle between two vectors
*/
double GetAngle(double x1, double y1, double x2, double y2)
{
    const double pi = 3.14159265358979323846;
    double dtheta,theta1,theta2;

    theta1 = atan2(y1,x1);
    theta2 = atan2(y2,x2);
    dtheta = theta2 - theta1;
    while (dtheta > pi)
        dtheta -= 2*pi;
    while (dtheta < -pi)
        dtheta += 2*pi;

    return(dtheta);
}

int isInsidePolygon(Point polygon[],int n,Point p)
{
    int i;
    double angle=0;
    Point p1,p2;
    const double pi = 3.14159265358979323846;

   for (i=0;i<n;i++) {
      p1.h = polygon[i].h - p.h;
      p1.v = polygon[i].v - p.v;
      p2.h = polygon[(i+1)%n].h - p.h;
      p2.v = polygon[(i+1)%n].v - p.v;
      angle += GetAngle(p1.h,p1.v,p2.h,p2.v);
   }

   if (abs(angle) < pi)
      return(false);
   else
      return(true);
}


int main() {
    // Point polygon[] =   {{1,0}, {8,3}, {8,8}, {1,5}}; 
    // Point p = {3,5}; 
    Point polygon[] =   {{-3,2}, {-2,-0.8}, {0,1.2}, {2.2,0}, {2,4.5}};
    Point p = {0,0}; 

    int n = sizeof(polygon)/sizeof(polygon[0]); 
    // cout << "p[0]: " << polygon[1].h << endl;

    int val = isInsidePolygon( polygon, n, p);
    if (val == 1)
    {
        cout << "Point p is Inside Polygon"<< endl;
    } else {
        cout << "Point p is NOT Inside Polygon"<< endl;
    }
   return 0;
}
