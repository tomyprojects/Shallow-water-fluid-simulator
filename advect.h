#ifndef ADVECT_H
#define ADVECT_H

#include "macgrid.h"

template<class Quantity, class Velocity>
class Advect
{
public:
    typedef mac_grid::MacGrid<Quantity, Velocity> MacGridType;
    typedef mac_grid::mg_float FloatType;
    typedef Velocity Position;

    Advect(MacGridType & g, FloatType & dt)
        : mgp(&g), mdt(dt)
    {
    }

    void advect_macgrid2(MacGridType & temp)
    {
        Position xp;

        for (unsigned i=0; i<temp.dimX(); i++)
        {
            for (unsigned k=0; k<temp.dimZ(); k++)
            {
                xp = trace_particle(i, k, &MacGridType::v);
                temp.q(i,k) = mgp->getq(xp[0], xp[1]);

                xp = trace_particle(i, k, &MacGridType::staggered_left);
                temp.staggered_left(i,k) = mgp->getv(xp[0], xp[1]);

                xp = trace_particle(i, k, &MacGridType::staggered_down);
                temp.staggered_down(i, k) = mgp->getv(xp[0], xp[1]);
            }

            unsigned k = temp.dimZ() - 1;

            xp = trace_particle(i, k, &MacGridType::staggered_right);
            temp.staggered_right(i,k) = mgp->getv(xp[0], xp[1]);
        }

        unsigned i=temp.dimX()-1;

        for (unsigned k=0; k<temp.dimZ(); k++)
        {
            xp = trace_particle(i, k, &MacGridType::staggered_top);
            temp.staggered_top(i,k) = mgp->getv(xp[0], xp[1]);
        }
    }

private:
    MacGridType * mgp;
    FloatType mdt;

    void clamp(Position & p)
    {
        if (p[0] < 0)
            p[0] = 0;
        if (p[1] < 0)
            p[1] = 0;
        if (p[0] > (FloatType)mgp->dimX() - 1)
            p[0] = (FloatType)mgp->dimX() - 1;
        if (p[1] > (FloatType)mgp->dimZ() - 1)
            p[1] = (FloatType)mgp->dimZ() - 1;
    }

    Position trace_particle(unsigned x, unsigned z, Velocity& (MacGridType::*v)(unsigned, unsigned))
    {
        Position xg(x,z);
        Position xmid = xg - 0.5 * mdt * (mgp->*v)(x, z);
        clamp(xmid);
        Position xp = xg - mdt * mgp->getv(xmid[0], xmid[1]);
        clamp(xp);
        return xp;
    }
};

#endif // ADVECT_H
