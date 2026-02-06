//
//  Option.hpp
//  GroupA&B
//
//  Created by Kevin on 10/24/25.
//
#include <vector>
using namespace std;
#ifndef Option_hpp
#define Option_hpp

#include <stdio.h>
class Option
{
public:
    // constructors
    Option();
    Option(const Option& other);
    virtual ~Option();

    Option& operator = (const Option& option2);
};

//global funuction to generete a mesh array
vector<double> meshArray(double start, double end, double h);

#endif /* Option_hpp */
