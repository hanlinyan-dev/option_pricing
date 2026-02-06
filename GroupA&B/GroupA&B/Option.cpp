//
//  Option.cpp
//  GroupA&B
//
//  Created by Kevin on 10/24/25.
//

#include "Option.hpp"

// Default constructor
Option::Option()
{
}

// Copy constructor
Option::Option(const Option& other)
{
}


Option::~Option()
{
}

Option& Option::operator= (const Option& option2)
{
    return *this;
}

//global funuction to generete a mesh array
vector<double> meshArray(double start, double end, double h) {
    vector<double> mesh;
    for (double x = start; x <= end; x += h)
        mesh.push_back(x);
    return mesh;
}
