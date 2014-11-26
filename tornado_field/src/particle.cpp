#include "particle.h"

#include <cassert>
#include <cmath>

const double Sphere::m_K=0.2387324146;
const double Sphere::m_Kdiv3=0.0795774715;
//
Sphere::Sphere(const double mass, const double density)
:m_mass(mass), m_density(density)
{
    assert(m_density >= 0.0 && "density must not be ZERO!");
}
//
Sphere::~Sphere()
{
}
// area of the cross section
double Sphere::Sc()const
{
    const double r = fastRadius();
    return (M_PI * r * r);
}
// volume
double Sphere::V()const
{
    return m_mass / m_density;
}
//
double Sphere::Radius()const
{
    return pow(0.75 * V() / M_PI, 1.0/3.0);
}
// use Taylor's formula to simulate f(x) when x is near zero.
// f(x) = f(a) + f'(a)*(x-a)

// let f(x) be R(V), then
//       R(V) = (3*V/(4*PI))^(1/3)
//            = (3/(4*PI))^(1/3) * V^(1/3)
//            = K * V^(1/3)                 { where K=(3/(4*PI))^(1/3)=0.2387324146 }

//      R'(V) = K/3 * V^(-2/3)
//            = Kdiv3 * V^(-2/3)            { where Kdiv3=K/3=0.0795774715 }
// let a=0.001, then
//      a^(1/3)=0.1; a^(2/3)=0.01;

// so:
// R(V) = R(a)  + R'(a)     *(V-a)
//      = K*0.1 + Kdiv3*0.01*(V-0.001)
double Sphere::fastRadius()const
{
    return (m_K*0.1 + m_Kdiv3*0.01*(V()-0.001));
}
