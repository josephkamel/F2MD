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

    double DistanceFromLine(double cx, double cy, double ax, double ay ,
                          double bx, double by)
    {

        double distanceSegment = 0;
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
            distanceSegment = distanceLine;
        }
        else
        {

            double dist1 = (cx-ax)*(cx-ax) + (cy-ay)*(cy-ay);
            double dist2 = (cx-bx)*(cx-bx) + (cy-by)*(cy-by);
            if (dist1 < dist2)
            {
                xx = ax;
                yy = ay;
                distanceSegment = sqrt(dist1);
            }
            else
            {
                xx = bx;
                yy = by;
                distanceSegment = sqrt(dist2);
            }


        }

        return distanceSegment;
    }

}

double Link::getDistance(const Coord& pos) const{
    double dist = DBL_MAX;
    const Coord* oldCd = NULL;
    bool firstelem = true;
   for (const Coord& cd : coords) {
       if(firstelem){
           oldCd = &cd;
           firstelem = false;
       }else{
          // double localdist1 = pointToLineDistC(pos, *oldCd, cd);
          // double localdist2 = pDistance(pos.x,pos.y, oldCd->x,oldCd->y,cd.x,cd.y);
           double localdist3 = DistanceFromLine(pos.x,pos.y, oldCd->x,oldCd->y,cd.x,cd.y);
           if(localdist3< dist){
               dist = localdist3;
           }
           oldCd = &cd;
       }
   }
   return dist;
}

