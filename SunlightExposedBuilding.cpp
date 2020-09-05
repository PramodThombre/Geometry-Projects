#include <iostream>
#include <cmath>
#include <cfloat>

using namespace std;

struct Point 
{ 
    float x; 
    float y; 
};

inline float
isLeft( Point P0, Point P1, Point P2 )
{
    return (P1.x - P0.x)*(P2.y - P0.y) - (P2.x - P0.x)*(P1.y - P0.y);
}

// tests for polygon vertex ordering relative to a fixed point P
#define above(P,Vi,Vj)  (isLeft(P,Vi,Vj) > 0)   // true if Vi is above Vj
#define below(P,Vi,Vj)  (isLeft(P,Vi,Vj) < 0)   // true if Vi is below Vj
//===================================================================

float
getDist( Point P0, Point P1 )
{
    return sqrt(pow((P1.x - P0.x),2.0) +  pow((P1.y - P0.y),2.0));
}

//P0 and P1 points form Line 1. A1x+B1y=C1
//P2 and P3 points form Line 2. A2x+B2y=C2
bool
areLineIntersecting( Point P0, Point P1, Point P2, Point P3 )
{
    // Coefficients of line 1
    float A1 = P1.y - P0.y;
    float B1 = P0.x - P1.x;
    float C1 = A1*P0.x + B1*P0.y;
    // Coefficients of line 2
    float A2 = P2.y - P0.y;
    float B2 = P0.x - P2.x;
    float C2 = A2*P0.x + B2*P0.y;

    double det = A1*B2 - A2*B1;
    if (det==0)
    {
        return 0;
    } else {
        return 1;
    }
}

Point
getLineIntersection( Point P0, Point P1, Point P2, Point P3 )
{
    // Coefficients of line 1
    // cout << "P0:" << P0.x << " ," << P0.y << endl;
    // cout << "P1:" << P1.x << " ," << P1.y << endl;
    // cout << "P2:" << P2.x << " ," << P2.y << endl;
    // cout << "P3:" << P3.x << " ," << P3.y << endl;

    float A1 = P1.y - P0.y;
    float B1 = P0.x - P1.x;
    float C1 = A1*P0.x + B1*P0.y;
    // Coefficients of line 2
    float A2 = P3.y - P2.y;
    float B2 = P2.x - P3.x;
    float C2 = A2*P2.x + B2*P2.y;

    double det = A1*B2 - A2*B1;
    if (det == 0) 
    { 
        // The lines are parallel. This is simplified 
        // by returning a pair of FLT_MAX 
        Point P = {FLT_MAX, FLT_MAX}; 
        return P; 
    } else
    {
        double x = (C1*B2 - C2*B1)/det ;
        double y = (C2*A1 - C1*A2)/det ;
        Point P = {x,y};
        return P ;
    }
}

// tangent_PointPoly(): find any polygon's exterior tangents
//    Input:  P = a 2D point (exterior to the polygon)
//            n = number of polygon vertices
//            V = array of vertices for any 2D polygon with V[n]=V[0]
//    Output: rtan = index of rightmost tangent point V[*rtan]
//            ltan = index of leftmost tangent point V[*ltan]
auto
tangent_PointPoly( Point P, int n, Point V[] )
{
    struct retVals {        // Declare a local structure 
    int i1, i2;
    };
    float  eprev, enext; 
    bool ertan, eltan;        // V[i] previous and next edge turn direction
    int rtan = 0;
    int ltan = 0;
    // *rtan = *ltan = 0;          // initially assume V[0] = both tangents
    // Point *V = V1
    eprev = isLeft(V[0],V[1], P);
    // cout << " booleprev:" << eprev << endl;
    // cout << "V[0]:" << V[0].x << " V[1]:" << V[1].x << "P:" << P.x << endl;

    for (int i=1; i<n; i++) {
        // cout << "V[i]" << V[i].x << "V[i]" << V[i+1].x << "P" << P.x << endl;
        enext = isLeft(V[i], V[i+1], P);
        if ((eprev <= 0) && (enext > 0)) 
        {
            ertan = isLeft(P, V[i], V[rtan]) < 0;
            if (!ertan)
            {
                rtan = i;
            }
        }
        else if ((eprev > 0) && (enext <= 0)) 
        {
            eltan = isLeft(P, V[i], V[ltan] ) > 0;
            if (!eltan)
            {
                ltan = i;
            }
        }
        eprev = enext;
    }
    // cout << "rtan:" << rtan << " ltan:" << ltan << endl;
    // cout << " V[rtan]:" << V[rtan].x  << "," << V[rtan].y << endl;
    // cout << " V[ltan]:" << V[ltan].x  << "," << V[ltan].y << endl;

    return retVals {rtan,ltan};
}
//===================================================================


int main()
{
    // Point V[] =  {{4,0},{4,-5},{7,-5},{7,0}};
    Point P1 = {-3.5,1}; 

    // // int n = 4;
    // Point V1[] =  {{4,0}, {4,-5}, {7,-5}, {7,0}, {4,0}};
    // Point V[] =  {{0.4,-2},{0.4,-5},{2.5,-5},{2.5,-2},{0.4,-2}};
    Point V[2][5] =  {{{0.4,-2},{0.4,-5},{2.5,-5},{2.5,-2},{0.4,-2}},{{4,0}, {4,-5}, {7,-5}, {7,0}, {4,0}}};
    int rows = sizeof(V)/sizeof(V[0]);
    int cols = sizeof(V[0])/sizeof(V[0][0]);
    // cout<<"rows:"<< rows << " cols"<< cols <<endl;
    float totaldist = 0;
    for (int l = 0;l < rows; l++)
    {   
        int n = sizeof(V[l])/sizeof(V[l][0]);; 
        n = n-1;
        Point intersection;
        if (l == 0)
        {
            auto [tan1VaL,tan2VaL] = tangent_PointPoly( P1, n, V[l] );
            // cout << "tan1VaL:" << tan1VaL << "tan2VaL:" << tan2VaL << "l " << l << endl;

            // for counterclockwise polygon
            int j = 0;
            int count = 0;
            for (int i=tan2VaL;i<n;i++) {
                j = i+1;
                if (j == n) {
                    j = 0;
                }
                if (j > tan1VaL) {
                    break;
                }
                // cout << "i:" << i << "j:" << j << endl;
                totaldist += getDist(V[l][i],V[l][j]);
                i = j-1;
                if (count > n+1)
                {
                    break;
                }
                count += 1;
            }
            // cout << "totaldist:" << totaldist << endl;
            bool valIntersection = areLineIntersecting  (P1 , V[l][tan2VaL], V[l+1][0], V[l+1][1]);
            // cout << "valIntersection:" << valIntersection << endl;
            if (valIntersection)
            {
                intersection = getLineIntersection (P1 , V[l][tan2VaL], V[l+1][0], V[l+1][1]);
                // cout << "The intersection of the given lines is: "; 
                // cout << "x:" << intersection.x << "y:" << intersection.y << endl;       
            }
        } else
        {
            auto [tan1VaL,tan2VaL] = tangent_PointPoly( P1, n, V[l] );
            // cout << "tan1VaL:" << tan1VaL << "tan2VaL:" << tan2VaL << "l " << l << endl;

            // for counterclockwise polygon
            int k = 0;
            for (int i=tan2VaL;i<n;i++) {
                k = i+1;
                if (k == n) {
                    k = 0;
                }
                if (k > tan1VaL-1) {
                    break;
                }
                // cout << "i:" << i << "k:" << k << endl;
                totaldist += abs(getDist(V[l][i],V[l][k]));
                i = k-1;
            }
            totaldist += getDist(V[l][tan1VaL-1],intersection);
        }
    }
    cout << "totaldist:" << totaldist << endl;
}
