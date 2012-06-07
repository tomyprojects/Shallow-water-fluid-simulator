#ifndef QUATERNION_H
#define QUATERNION_H
/*
class Quaternion
{
    float m[4];
public:
    Quaternion() { m[0] = 0; m[1] = 0; m[2] = 0; m[3] = 0; }

    float& w() { return m[0]; }
    float& x() { return m[1]; }
    float& y() { return m[2]; }
    float& z() { return m[3]; }

    Quaternion(float m0, float m1, float m2, float m3) { m[0] = m0; m[1] = m1; m[2] = m2; m[3] = m3; }

    Quaternion operator* (const Quaternion & q)
    {
        return Quaternion(w() * q.x() + x() * q.w() + y() * q.z() - z() * q.y(),
                          w() * q.y() + y() * q.w() + z() * q.x() - x() * q.z(),
                          w() * q.z() + z() * q.w() + x() * q.y() - y() * q.x(),
                          w() * q.w() - x() * q.x() - y() * q.y() - z() * q.z());
    }
};*/

#endif // QUATERNION_H
