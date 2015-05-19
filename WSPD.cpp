#include <math.h>
#include <utility>

#include "WSPD.h"

#define SWAP(x,y) do \
{
    unsigned char swap_temp[sizeof(x) == sizeof(y) ? (signed)sizeof(x) : -1]; \
    memcpy(swap_temp,&y,sizeof(x)); \
    memcpy(&y,&x,       sizeof(x)); \
    memcpy(&x,swap_temp,sizeof(x)); \
} while(0)

WSPD::WSPD(QuadTree tree, Kernel s)
{
    this->s = s;

}

vector<Pair<QuadTree, QuadTree>> WSPD::pairing(QuadTree t1, QuadTree t2)
{
   vector<Pair<QuadTree, QuadTree>> pairs;

   Vector_2 v = t1.get_center() - t2.get_center();
   Kernel dist = sqrt(v.squared_length());
   if (t1.get_radius() < t2.get_radius())
   {
       SWAP(t1, t2);
   }
   // t1 has larger radius by now
   if (s * t1.get_radius() <= dist)
   {
       pairs.push_back(make_pair(t1, t2));
   }
   else
   {
       // pairing the children of t1 with t2

   }


