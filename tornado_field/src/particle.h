#pragma once

class Sphere
{
public:
    Sphere(const double mass, const double density);
    ~Sphere();

    // area of the cross section
    double Sc()const;

    // volume
    double V()const;

    double Radius()const;

    double fastRadius()const;

private:
    const double m_mass;
    const double m_density;// the density of stone: [1.2, 3.3]
    const static double m_K;
    const static double m_Kdiv3;
};
