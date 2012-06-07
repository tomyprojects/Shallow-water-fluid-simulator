#ifndef SHALLOWWATER_H
#define SHALLOWWATER_H

#include "macgrid.h"
#include "vec2.h"
#include "advect.h"
#include "heightmap.h"
#include "exception.h"
#include <cmath>
#include <cassert>

class ShallowWaterBase
{
public:
    typedef mac_grid::mg_float sw_float;
    typedef vec2<sw_float> Vec2f;

    virtual void update(sw_float dt, sw_float g) = 0;
    virtual void update(sw_float g) = 0;
    virtual void loadTerrainHeightmap(const char* path, sw_float hmax) = 0;
    virtual void loadWaterDepthmap(const char* path, sw_float max) = 0;
    virtual unsigned width() = 0;
    virtual unsigned height() = 0;
    virtual sw_float& bottom(unsigned i, unsigned j) = 0;
    virtual sw_float& height(unsigned i, unsigned j) = 0;
    virtual sw_float maximumDepth() = 0;
    virtual Vec2f& velocity(unsigned i, unsigned z) = 0;
    virtual sw_float& operator() (unsigned i, unsigned j) = 0;
};

template<unsigned W, unsigned H>
class ShallowWater : public ShallowWaterBase
{
public:
    typedef mac_grid::mg_float sw_float;
    typedef ShallowWaterBase::Vec2f Vec2f;
    typedef mac_grid::MacGrid2<W, H, sw_float, Vec2f> MacGridType;

    ShallowWater()
        : mmaximumDepth(1)
    {
        for (int i=0; i<W; i++)
            for (int j=0; j<H; j++)
                mh[i][j] = mb[i][j] = 0;
    }

    void update(sw_float dt, sw_float g)
    {
        Advect<sw_float,Vec2f>(mgrid, dt).advect_macgrid2(_mgrid);

        for (unsigned i=0; i<W; i++)
            for (unsigned j=0; j<H; j++)
            {
                mh[i][j] = mb[i][j] + _mgrid.q(i,j);
                assert(mh[i][j] == mh[i][j]);
                assert(mh[i][j] < 100);
                assert(mh[i][j] >= 0);
            }

        addPressureAcceleration(dt, g);
        setBoundaryConditions();
        updateVelocities();
        updateVorticities();
        updateDepth(dt);
    }

    void update(sw_float g)
    {
        float dt1 = 0.2 * 1.0 / sqrt(g * maximumDepth());
        float dt2 = 2.0 / sqrt(mmaxvelocity.squaredMagnitude());
        update( (dt1 < dt2 ? dt1 : dt2) , g);
    }

    void loadTerrainHeightmap(const char* path, sw_float hmax)
    {
        HeightMap map(path);
        throwif(map.height() != H);
        throwif(map.width() != W);
        for (unsigned i=0; i<map.width(); i++)
            for (unsigned j=0; j<map.height(); j++)
            {
                mb[i][j] = (map(i,j).h - map.minHeight()) * hmax;
                mgrid.q(i,j) = 0;
                mh[i][j] = mb[i][j];
            }
    }

    void loadWaterDepthmap(const char* path, sw_float max)
    {
        HeightMap map(path);
        throwif(map.height() != H);
        throwif(map.width() != W);
        for (unsigned i=0; i<map.width(); i++)
            for (unsigned j=0; j<map.height(); j++)
            {
                mgrid.q(i,j) = (map(i,j).h - map.minHeight()) * max;
                mh[i][j] = mgrid.q(i,j) + mb[i][j];

                if (mgrid.q(i,j) > mmaximumDepth)
                    mmaximumDepth = mgrid.q(i,j);
            }
    }

    unsigned width() { return W; }
    unsigned height() { return H; }
    sw_float& bottom(unsigned i, unsigned j) { return mb[i][j]; }
    sw_float& height(unsigned i, unsigned j) { return mh[i][j]; }
    sw_float maximumDepth() { return mmaximumDepth; }
    Vec2f& velocity(unsigned i, unsigned z) { return mgrid.v(i, z); }

    sw_float& operator() (unsigned i, unsigned j) { return mh[i][j]; }

private:
    MacGridType mgrid;
    MacGridType _mgrid;
    sw_float mh[W][H];
    sw_float mb[W][H];
    sw_float mmaximumDepth;
    Vec2f mmaxvelocity;

    void updateVorticities()
    {
        for (unsigned i=0; i<W; i++)
            for (unsigned j=0; j<H; j++)
            {
                mgrid.vorticity(i,j) = mgrid.staggered_top(i,j).y() - mgrid.staggered_down(i,j).y()
                        - (mgrid.staggered_right(i,j).x() - mgrid.staggered_left(i,j).x());
            }
    }

    void addPressureAcceleration(sw_float dt, sw_float g)
    {
        for (unsigned i=0; i<W; i++)
            for (unsigned j=0; j<H; j++)
            {
                sw_float h = dt * g * mh[i][j];

                _mgrid.staggered_left(i,j)[0] -= h;
                _mgrid.staggered_right(i,j)[0] += h;

                _mgrid.staggered_down(i,j)[1] -= h;
                _mgrid.staggered_top(i,j)[1] += h;
            }
    }

    void setBoundaryConditions()
    {
        for (unsigned i=0; i<W; i++)
        {
            _mgrid.staggered_top(i,H-1)[1] = 0;
            //_mgrid.staggered_down(i,H-1)[1] = 0;

            //_mgrid.staggered_top(i,0)[1] = 0;
            _mgrid.staggered_down(i,0)[1] = 0;
        }

        for (unsigned j=0; j<H; j++)
        {
            _mgrid.staggered_left(0,j)[0] = 0;
            //_mgrid.staggered_right(0,j)[0] = 0;

            //_mgrid.staggered_left(W-1,j)[0] = 0;
            _mgrid.staggered_right(W-1,j)[0] = 0;
        }
    }

    void updateVelocities()
    {
        for (unsigned i=0; i<W; i++)
            for (unsigned j=0; j<H; j++)
            {
                mgrid.v(i,j)[0] = 0.5 * (_mgrid.staggered_left(i,j)[0] + _mgrid.staggered_right(i,j)[0]);
                mgrid.v(i,j)[1] = 0.5 * (_mgrid.staggered_top(i,j)[1] + _mgrid.staggered_down(i,j)[1]);

                if (mgrid.v(i,j).squaredMagnitude() > mmaxvelocity.squaredMagnitude())
                    mmaxvelocity = mgrid.v(i,j);
            }

        for (unsigned i=1; i<W-1; i++)
            for (unsigned j=1; j<H-1; j++)
            {
                mgrid.staggered_left(i,j)[0] = _mgrid.staggered_left(i,j)[0];
                mgrid.staggered_left(i,j)[1] = 0.5 * (mgrid.v(i,j)[1] + mgrid.v(i-1,j)[1]);

                mgrid.staggered_right(i,j)[0] = _mgrid.staggered_right(i,j)[0];
                mgrid.staggered_right(i,j)[1] = 0.5 * (mgrid.v(i,j)[1] + mgrid.v(i+1,j)[1]);

                mgrid.staggered_top(i,j)[1] = _mgrid.staggered_top(i,j)[1];
                mgrid.staggered_top(i,j)[0] = 0.5 * (mgrid.v(i,j)[0] + mgrid.v(i,j+1)[0]);

                mgrid.staggered_down(i,j)[1] = _mgrid.staggered_down(i,j)[1];
                mgrid.staggered_down(i,j)[0] = 0.5 * (mgrid.v(i,j)[0] + mgrid.v(i,j-1)[0]);
            }

        updateVelocitiesAtBoundaryCells();
    }

    void updateVelocitiesAtBoundaryCells()
    {
        updateBottomBoundary();
        updateTopBoundary();
        updateLeftBoundary();
        updateRightBoundary();

        updateBottomLeftBoundary();
        updateBottomRightBoundary();
        updateUpperLeftBoundary();
        updateUpperRightBoundary();
    }

    void updateBottomBoundary()
    {
        for (unsigned i=1; i<W-1; i++)
        {
            unsigned j=0;

            mgrid.staggered_left(i,j)[0] = _mgrid.staggered_left(i,j)[0];
            mgrid.staggered_left(i,j)[1] = 0.5 * (mgrid.v(i,j)[1] + mgrid.v(i-1,j)[1]);

            mgrid.staggered_right(i,j)[0] = _mgrid.staggered_right(i,j)[0];
            mgrid.staggered_right(i,j)[1] = 0.5 * (mgrid.v(i,j)[1] + mgrid.v(i+1,j)[1]);

            mgrid.staggered_top(i,j)[1] = _mgrid.staggered_top(i,j)[1];
            mgrid.staggered_top(i,j)[0] = 0.5 * (mgrid.v(i,j)[0] + mgrid.v(i,j+1)[0]);

            mgrid.staggered_down(i,j)[1] = 0;
            mgrid.staggered_down(i,j)[0] = 0.5 * (mgrid.v(i,j)[0] + 0);
        }
    }

    void updateTopBoundary()
    {
        for (unsigned i=1; i<W-1; i++)
        {
            unsigned j = H - 1;

            mgrid.staggered_left(i,j)[0] = _mgrid.staggered_left(i,j)[0];
            mgrid.staggered_left(i,j)[1] = 0.5 * (mgrid.v(i,j)[1] + mgrid.v(i-1,j)[1]);

            mgrid.staggered_right(i,j)[0] = _mgrid.staggered_right(i,j)[0];
            mgrid.staggered_right(i,j)[1] = 0.5 * (mgrid.v(i,j)[1] + mgrid.v(i+1,j)[1]);

            mgrid.staggered_top(i,j)[1] = 0;
            mgrid.staggered_top(i,j)[0] = 0.5 * (mgrid.v(i,j)[0] + 0);

            mgrid.staggered_down(i,j)[1] = _mgrid.staggered_down(i,j)[1];
            mgrid.staggered_down(i,j)[0] = 0.5 * (mgrid.v(i,j)[0] + mgrid.v(i,j-1)[0]);
        }
    }

    void updateLeftBoundary()
    {
        for (unsigned j=1; j<H-1; j++)
        {
            unsigned i = 0;

            mgrid.staggered_left(i,j)[0] = 0;
            mgrid.staggered_left(i,j)[1] = 0.5 * (mgrid.v(i,j)[1] + 0);

            mgrid.staggered_right(i,j)[0] = _mgrid.staggered_right(i,j)[0];
            mgrid.staggered_right(i,j)[1] = 0.5 * (mgrid.v(i,j)[1] + mgrid.v(i+1,j)[1]);

            mgrid.staggered_top(i,j)[1] = _mgrid.staggered_top(i,j)[1];
            mgrid.staggered_top(i,j)[0] = 0.5 * (mgrid.v(i,j)[0] + mgrid.v(i,j+1)[0]);

            mgrid.staggered_down(i,j)[1] = _mgrid.staggered_down(i,j)[1];
            mgrid.staggered_down(i,j)[0] = 0.5 * (mgrid.v(i,j)[0] + mgrid.v(i,j-1)[0]);
        }
    }

    void updateRightBoundary()
    {
        for (unsigned j=1; j<H-1; j++)
        {
            unsigned i = W - 1;

            mgrid.staggered_left(i,j)[0] = _mgrid.staggered_left(i,j)[0];
            mgrid.staggered_left(i,j)[1] = 0.5 * (mgrid.v(i,j)[1] + mgrid.v(i-1,j)[1]);

            mgrid.staggered_right(i,j)[0] = 0;
            mgrid.staggered_right(i,j)[1] = 0.5 * (mgrid.v(i,j)[1] + 0);

            mgrid.staggered_top(i,j)[1] = _mgrid.staggered_top(i,j)[1];
            mgrid.staggered_top(i,j)[0] = 0.5 * (mgrid.v(i,j)[0] + mgrid.v(i,j+1)[0]);

            mgrid.staggered_down(i,j)[1] = _mgrid.staggered_down(i,j)[1];
            mgrid.staggered_down(i,j)[0] = 0.5 * (mgrid.v(i,j)[0] + mgrid.v(i,j-1)[0]);
        }
    }

    void updateBottomLeftBoundary()
    {
        unsigned j = 0;

        unsigned i = 0;

        mgrid.staggered_left(i,j)[0] = 0;
        mgrid.staggered_left(i,j)[1] = 0.5 * (mgrid.v(i,j)[1] + 0);

        mgrid.staggered_right(i,j)[0] = _mgrid.staggered_right(i,j)[0];
        mgrid.staggered_right(i,j)[1] = 0.5 * (mgrid.v(i,j)[1] + mgrid.v(i+1,j)[1]);

        mgrid.staggered_top(i,j)[1] = _mgrid.staggered_top(i,j)[1];
        mgrid.staggered_top(i,j)[0] = 0.5 * (mgrid.v(i,j)[0] + mgrid.v(i,j+1)[0]);

        mgrid.staggered_down(i,j)[1] = 0;
        mgrid.staggered_down(i,j)[0] = 0.5 * (mgrid.v(i,j)[0] + 0);
    }

    void updateBottomRightBoundary()
    {
        unsigned j = 0;

        unsigned i = W - 1;

        mgrid.staggered_left(i,j)[0] = _mgrid.staggered_left(i,j)[0];
        mgrid.staggered_left(i,j)[1] = 0.5 * (mgrid.v(i,j)[1] + mgrid.v(i-1,j)[1]);

        mgrid.staggered_right(i,j)[0] = 0;
        mgrid.staggered_right(i,j)[1] = 0.5 * (mgrid.v(i,j)[1] + 0);

        mgrid.staggered_top(i,j)[1] = _mgrid.staggered_top(i,j)[1];
        mgrid.staggered_top(i,j)[0] = 0.5 * (mgrid.v(i,j)[0] + mgrid.v(i,j+1)[0]);

        mgrid.staggered_down(i,j)[1] = 0;
        mgrid.staggered_down(i,j)[0] = 0.5 * (mgrid.v(i,j)[0] + 0);
    }

    void updateUpperLeftBoundary()
    {
        unsigned j = H - 1;

        unsigned i = 0;

        mgrid.staggered_left(i,j)[0] = 0;
        mgrid.staggered_left(i,j)[1] = 0.5 * (mgrid.v(i,j)[1] + 0);

        mgrid.staggered_right(i,j)[0] = _mgrid.staggered_right(i,j)[0];
        mgrid.staggered_right(i,j)[1] = 0.5 * (mgrid.v(i,j)[1] + mgrid.v(i+1,j)[1]);

        mgrid.staggered_top(i,j)[1] = 0;
        mgrid.staggered_top(i,j)[0] = 0.5 * (mgrid.v(i,j)[0] + 0);

        mgrid.staggered_down(i,j)[1] = _mgrid.staggered_down(i,j)[1];
        mgrid.staggered_down(i,j)[0] = 0.5 * (mgrid.v(i,j)[0] + mgrid.v(i,j-1)[0]);
    }

    void updateUpperRightBoundary()
    {
        unsigned j = H - 1;

        unsigned i = W - 1;

        mgrid.staggered_left(i,j)[0] = _mgrid.staggered_left(i,j)[0];
        mgrid.staggered_left(i,j)[1] = 0.5 * (mgrid.v(i,j)[1] + mgrid.v(i-1,j)[1]);

        mgrid.staggered_right(i,j)[0] = 0;
        mgrid.staggered_right(i,j)[1] = 0.5 * (mgrid.v(i,j)[1] + 0);

        mgrid.staggered_top(i,j)[1] = 0;
        mgrid.staggered_top(i,j)[0] = 0.5 * (mgrid.v(i,j)[0] + 0);

        mgrid.staggered_down(i,j)[1] = _mgrid.staggered_down(i,j)[1];
        mgrid.staggered_down(i,j)[0] = 0.5 * (mgrid.v(i,j)[0] + mgrid.v(i,j-1)[0]);
    }

    void updateDepth(sw_float dt)
    {
        for (unsigned i=0; i<W; i++)
            for (unsigned j=0; j<H; j++)
            {
                sw_float a = (mgrid.staggered_right(i,j)[0] - mgrid.staggered_left(i,j)[0]
                              + mgrid.staggered_top(i,j)[1] - mgrid.staggered_down(i,j)[1]);
                mgrid.q(i,j) = _mgrid.q(i,j) - dt * _mgrid.q(i,j) * a;
                //assert(mgrid.q(i,j) >= 0);
                if (mgrid.q(i,j) > mmaximumDepth)
                    mmaximumDepth = mgrid.q(i,j);
            }

        assert(mmaximumDepth == mmaximumDepth);

        if (mmaximumDepth < 0.0001)
            mmaximumDepth = 0.0001;
    }
};

#endif // SHALLOWWATER_H
