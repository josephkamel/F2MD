//
// LinkControl - models Links that block radio transmissions
// Copyright (C) 2010 Christoph Sommer <christoph.sommer@informatik.uni-erlangen.de>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//

#include <set>
#include "Link.h"
#include "math.h"
#include "float.h"

Link::Link(){
}

Link::Link(const Link& link){
    for (const Coord& cd : link.coords) {
        coords.push_back(cd);
    }
    bboxP1 = link.bboxP1;
    bboxP2 = link.bboxP2;
}

void Link::setShape(Coords shape) {
	coords = shape;
	bboxP1 = Coord(1e7, 1e7);
	bboxP2 = Coord(-1e7, -1e7);
	for (Coords::const_iterator i = coords.begin(); i != coords.end(); ++i) {
		bboxP1.x = std::min(i->x, bboxP1.x);
		bboxP1.y = std::min(i->y, bboxP1.y);
		bboxP2.x = std::max(i->x, bboxP2.x);
		bboxP2.y = std::max(i->y, bboxP2.y);
	}
}

const Link::Coords& Link::getShape() const {
	return coords;
}

const Coord Link::getBboxP1() const {
	return bboxP1;
}

const Coord Link::getBboxP2() const {
	return bboxP2;
}


namespace {
	bool isPointInLink(Coord point, const Link& o) {
		bool isInside = false;
		const Link::Coords& shape = o.getShape();
		Link::Coords::const_iterator i = shape.begin();
		Link::Coords::const_iterator j = (shape.rbegin()+1).base();
		for (; i != shape.end(); j = i++) {
			bool inYRangeUp = (point.y >= i->y) && (point.y < j->y);
			bool inYRangeDown = (point.y >= j->y) && (point.y < i->y);
			bool inYRange = inYRangeUp || inYRangeDown;
			if (!inYRange) continue;
			bool intersects = point.x < (i->x + ((point.y - i->y) * (j->x - i->x) / (j->y - i->y)));
			if (!intersects) continue;
			isInside = !isInside;
		}
		return isInside;
	}

	void getABCLine(double x1, double y1, double x2, double y2, double &a, double &b, double &c)
	{
	    // (x- p1X) / (p2X - p1X) = (y - p1Y) / (p2Y - p1Y)
	    a = y2 - y1;
	    b = x2 - x1;
	    c = x1 * y2 - x2 * y1;
	}

	double pointToLineDist(double pct1X, double pct1Y, double pct2X, double pct2Y, double pct3X, double pct3Y)
	{
	    double a, b, c;
	    getABCLine(pct2X, pct2Y, pct3X, pct3Y, a, b, c);
	    return abs(a * pct1X + b * pct1Y + c) / sqrt(a * a + b * b);
	}

    double pointToLineDistC(Coord point, Coord line1, Coord line2)
    {
        double a, b, c;
        getABCLine(line1.x, line1.y, line2.x, line2.y, a, b, c);
        return abs(a * point.x + b * point.y + c) / sqrt(a * a + b * b);
    }

    double pDistance(double x, double y,double  x1,double  y1, double x2, double y2) {

      double A = x - x1;
      double B = y - y1;
      double C = x2 - x1;
      double D = y2 - y1;

      double dot = A * C + B * D;
      double len_sq = C * C + D * D;
      double param = -1;
      if (len_sq != 0) //in case of 0 length line
          param = dot / len_sq;

      double xx, yy;

      if (param < 0) {
        xx = x1;
        yy = y1;
      }
      else if (param > 1) {
        xx = x2;
        yy = y2;
      }
      else {
        xx = x1 + param * C;
        yy = y1 + param * D;
      }

      double dx = x - xx;
      double dy = y - yy;
      return sqrt(dx * dx + dy * dy);
    }



    // LUT for fast sqrt of floats. Table will be consist of 2 parts, half for sqrt(X) and half for sqrt(2X).
    const int nBitsForSQRTprecision = 11;                       // Use only 11 most sagnificant bits from the 23 of float. We can use 15 bits instead. It will produce less error but take more place in a memory.
    const int nUnusedBits   = 23 - nBitsForSQRTprecision;       // Amount of bits we will disregard
    const int tableSize     = (1 << (nBitsForSQRTprecision+1)); // 2^nBits*2 because we have 2 halves of the table.
    static short sqrtTab[tableSize];
    static unsigned char is_sqrttab_initialized = false;        // Once initialized will be true

    // Table of precalculated sqrt() for future fast calculation. Approximates the exact with an error of about 0.5%
    // Note: To access the bits of a float in C quickly we must misuse pointers.
    // More info in: http://en.wikipedia.org/wiki/Single_precision
    void build_fsqrt_table(void){
        unsigned short i;
        float f;
        uint32_t *fi = (uint32_t*)&f;

        if (is_sqrttab_initialized)
            return;

        const int halfTableSize = (tableSize>>1);
        for (i=0; i < halfTableSize; i++){
             *fi = 0;
             *fi = (i << nUnusedBits) | (127 << 23); // Build a float with the bit pattern i as mantissa, and an exponent of 0, stored as 127

             // Take the square root then strip the first 'nBitsForSQRTprecision' bits of the mantissa into the table
             f = sqrtf(f);
             sqrtTab[i] = (short)((*fi & 0x7fffff) >> nUnusedBits);

             // Repeat the process, this time with an exponent of 1, stored as 128
             *fi = 0;
             *fi = (i << nUnusedBits) | (128 << 23);
             f = sqrtf(f);
             sqrtTab[i+halfTableSize] = (short)((*fi & 0x7fffff) >> nUnusedBits);
        }
        is_sqrttab_initialized = true;
    }

    // Calculation of a square root. Divide the exponent of float by 2 and sqrt() its mantissa using the precalculated table.
    float fsqrt(float n){
        if (n <= 0.f)
            return 0.f;                           // On 0 or negative return 0.
        uint32_t *num = (uint32_t*)&n;
        short e;                                  // Exponent
        e = (*num >> 23) - 127;                   // In 'float' the exponent is stored with 127 added.
        *num &= 0x7fffff;                         // leave only the mantissa

        // If the exponent is odd so we have to look it up in the second half of the lookup table, so we set the high bit.
        const int halfTableSize = (tableSize>>1);
        const int secondHalphTableIdBit = halfTableSize << nUnusedBits;
        if (e & 0x01)
            *num |= secondHalphTableIdBit;
        e >>= 1;                                  // Divide the exponent by two (note that in C the shift operators are sign preserving for signed operands

        // Do the table lookup, based on the quaternary mantissa, then reconstruct the result back into a float
        *num = ((sqrtTab[*num >> nUnusedBits]) << nUnusedBits) | ((e + 127) << 23);
        return n;
    }

    void DistanceFromLine(double cx, double cy, double ax, double ay ,
                            double bx, double by, double * distanceSegment)
      {

          double distanceLine = 0;

          double r_numerator = (cx-ax)*(bx-ax) + (cy-ay)*(by-ay);
          double r_denomenator = (bx-ax)*(bx-ax) + (by-ay)*(by-ay);
          double r = r_numerator / r_denomenator;
      //
          double px = ax + r*(bx-ax);
          double py = ay + r*(by-ay);


      //
          double s =  ((ay-cy)*(bx-ax)-(ax-cx)*(by-ay) ) / r_denomenator;



          distanceLine = fabs(s)*sqrt(r_denomenator);

      //
      // (xx,yy) is the point on the lineSegment closest to (cx,cy)
      //
          double xx = px;
          double yy = py;

          if ( (r >= 0) && (r <= 1) )
          {
              *distanceSegment = distanceLine;

          }
          else
          {
              double dist1 = (cx-ax)*(cx-ax) + (cy-ay)*(cy-ay);
              double dist2 = (cx-bx)*(cx-bx) + (cy-by)*(cy-by);
              if (dist1 < dist2)
              {
                  xx = ax;
                  yy = ay;
                  *distanceSegment = sqrt(dist1);
              }
              else
              {
                  xx = bx;
                  yy = by;
                  *distanceSegment = sqrt(dist2);
              }


          }

         // return 0;

      }

}



double Link::getDistance(const Coord * pos) const{
    double dist = 10000;
    const Coord* oldCd = NULL;
    bool firstelem = true;

    double localdist = 0;
   for (const Coord& cd : coords) {
       if(firstelem){
           oldCd = &cd;
           firstelem = false;
       }else{
          // double localdist1 = pointToLineDistC(pos, *oldCd, cd);
          // double localdist2 = pDistance(pos.x,pos.y, oldCd->x,oldCd->y,cd.x,cd.y);
            DistanceFromLine(pos->x,pos->y, oldCd->x,oldCd->y,cd.x,cd.y, &localdist);

           if(localdist< dist){
               dist = localdist;
           }
           oldCd = &cd;
       }
   }
   return dist;
}

