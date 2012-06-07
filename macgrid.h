#ifndef MACGRID_H
#define MACGRID_H

#include <cassert>

namespace mac_grid {
    typedef float mg_float;

    template <class Value, unsigned NX, unsigned NY, unsigned NZ>
    Value bilinear_interpolation(Value v[NX][NY][NZ], mg_float & i, unsigned j, mg_float & k)
    {
        unsigned inti = unsigned(i);
        unsigned intk = unsigned(k);
        unsigned inti2 = inti + 1;
        unsigned intk2 = intk + 1;

        mg_float floati = i - mg_float(inti);
        mg_float floatk = k - mg_float(intk);

        Value a = v[inti][j][intk] * (1 - floati) + floati * v[inti2][j][intk];
        Value b = v[inti][j][intk2] * (1 - floati) + floati * v[inti2][j][intk2];

        return a * (1 - floatk) + b * floatk;
    }

    template <class Value, unsigned NX, unsigned NY, unsigned NZ>
    Value trilinear_interpolation(Value v[NX][NY][NZ], mg_float & i, mg_float & j, mg_float & k)
    {
        unsigned intj = unsigned(j);
        mg_float floatj = j - mg_float(intj);

        Value a = bilinear_interpolation<Value>(v, i, intj, k);
        Value b = bilinear_interpolation<Value>(v, i, intj+1, k);

        return a * (1 - floatj) + b * floatj;
    }

    template <class Quantity, class Velocity>
    class MacGrid
    {
    public:
        virtual Quantity& q(unsigned, unsigned) = 0;
        virtual Velocity& v(unsigned, unsigned) = 0;
        virtual Quantity& vorticity(unsigned i, unsigned k) = 0;

        virtual Velocity& staggered_left(unsigned i, unsigned k) = 0;
        virtual Velocity& staggered_right(unsigned i, unsigned k) = 0;
        virtual Velocity& staggered_top(unsigned i, unsigned k) = 0;
        virtual Velocity& staggered_down(unsigned i, unsigned k) = 0;

        virtual Quantity getq(mg_float i, mg_float k) = 0;
        virtual Velocity getv(mg_float i, mg_float k) = 0;
        virtual Quantity getVorticity(mg_float i, mg_float k) = 0;

        virtual unsigned dimX() { return 1; }
        virtual unsigned dimY() { return 1; }
        virtual unsigned dimZ() { return 1; }
    };

    template<unsigned NX, unsigned NZ, class Quantity, class Velocity>
    class MacGrid2 : public MacGrid<Quantity, Velocity>
    {
    public:
        MacGrid2()
        {
            for (int i=0; i<NX; i++)
                for (int j=0; j<NZ; j++)
                {
                    mq[i][1][j] = 0;
                    mvorticity[i][1][j] = 0;
                }
        }

        Quantity& q(unsigned i, unsigned k) { assert(mq[i][0][k] == mq[i][0][k]); return mq[i][0][k]; }
        Velocity& v(unsigned i, unsigned k) { return mv[i][0][k]; }
        Quantity& vorticity(unsigned i, unsigned k) { return mvorticity[i][0][k]; }

        Quantity getq(mg_float i, mg_float k) { return mac_grid::bilinear_interpolation<Quantity,NX,1,NZ>(mq,i,0,k); }
        Velocity getv(mg_float i, mg_float k) { return mac_grid::bilinear_interpolation<Velocity,NX,1,NZ>(mv,i,0,k); }
        Quantity getVorticity(mg_float i, mg_float k) { return mac_grid::bilinear_interpolation<Quantity,NX,1,NZ>(mvorticity,i,0,k); }

        Velocity& staggered_left(unsigned i, unsigned k) { return mu[i][k]; }
        Velocity& staggered_right(unsigned i, unsigned k) { return mu[i+1][k]; }
        Velocity& staggered_top(unsigned i, unsigned k) { return mw[i][k+1]; }
        Velocity& staggered_down(unsigned i, unsigned k) { return mw[i][k]; }

        unsigned dimX() { return NX; }
        unsigned dimZ() { return NZ; }

    private:
        Quantity mq[NX][1][NZ];
        Quantity mvorticity[NX][1][NZ];
        Velocity mv[NX][1][NZ];
        Velocity mu[NX+1][NZ];
        Velocity mw[NX][NZ+1];
    };
}
#endif // MACGRID_H
