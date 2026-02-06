//
//  PerpetualAmericanOptions.cpp
//  GroupA&B
//  Perpetual American Options implementation
//  Created by Kevin on 10/24/25.
//

#include "PerpetualAmericanOptions.hpp"
#include <cmath>
#include <iostream>
#include <vector>
#include <iomanip>

void PerpetualAmericanOption::init()
{    // Initialise all default values
    // Default values
    r = 0.1;
    sig= 0.1;
    K = 100.0;
    b = r;            // Black and Scholes stock option model (1973)
    optType = "C"; // PerpetualAmerican Call Option (this is the default type)
}

void PerpetualAmericanOption::copy( const PerpetualAmericanOption& o2)
{

    r    = o2.r;
    sig  = o2.sig;
    K    = o2.K;
    S    = o2.S;
    b    = o2.b;
    optType = o2.optType;
    
}//copy other option

PerpetualAmericanOption::PerpetualAmericanOption()
{ // Default call option
    init();
}

PerpetualAmericanOption::PerpetualAmericanOption(double K, double sig, double r, double b, double S, string opt_type): K(K), sig(sig), r(r), b(b), S(S), optType(opt_type) {}
//constructor with parameter


PerpetualAmericanOption::PerpetualAmericanOption(const vector<vector<double>> parameter_matrix, string opt_type):parameter_matrix(parameter_matrix), optType(opt_type)
{}
// Constructor with matrix

PerpetualAmericanOption::PerpetualAmericanOption(const PerpetualAmericanOption& o2)
{ // Copy constructor
    copy(o2);
}

PerpetualAmericanOption::PerpetualAmericanOption(const string& option_type)
{    // Create option type
    init();
    optType = option_type;

    if (optType == "c")
        optType = "C";
    if (optType == "p")
        optType = "P";
}

PerpetualAmericanOption& PerpetualAmericanOption::operator = (const PerpetualAmericanOption& option2)
{
    if (this == &option2) return *this;

    copy (option2);

    return *this;
}//assignment overloading

PerpetualAmericanOption::~PerpetualAmericanOption(){}//destructor

//Setters
void PerpetualAmericanOption::setS(double newS) { S = newS; }
void PerpetualAmericanOption::setSig(double newSig) { sig = newSig; }
void PerpetualAmericanOption::setR(double newR) { r = newR; }
void PerpetualAmericanOption::setB(double newB) { b = newB; }
void PerpetualAmericanOption::setK(double newK) { K = newK; }

double PerpetualAmericanOption::CallPrice() const
{
    double sig2 = sig*sig;
    double fac = b/sig2 - 0.5; fac *= fac;
    double y1 = 0.5 - b/sig2 + sqrt(fac + 2.0*r/sig2);
    if (1.0 == y1)
        return S;

    double fac2 = ((y1 - 1.0)*S) / (y1 * K);
    double c = K * pow(fac2, y1) / (y1 - 1.0);

    return c;
}

double PerpetualAmericanOption::PutPrice() const
{
    double sig2 = sig*sig;
    double fac = b/sig2 - 0.5; fac *= fac;
    double y2 = 0.5 - b/sig2 - sqrt(fac + 2.0*r/sig2);
    
    if (0.0 == y2)
        return S;

    double fac2 = ((y2 - 1.0)*S) / (y2 * K);
    double p = K * pow(fac2, y2) / (1.0 - y2);

    return p;

}

// Functions that calculate option price and sensitivities
double PerpetualAmericanOption::Price() const
{
    if (optType == "C")
    {
        return CallPrice();
    }
    else
    {
        return PutPrice();
    }
}

// Calculate option prices using parater matrix
vector<double> PerpetualAmericanOption::PriceWithMatrix()
{
    for (vector<double> row: parameter_matrix)
    {
        PerpetualAmericanOption option(row[0], row[1], row[2], row[3], row[4], optType);
        price_vector.push_back(option.Price());
    }
    return price_vector;
}

void PerpetualAmericanOption::PrintPriceResult()
{
    cout << "----------------------------------------------------------------" << endl;
    cout << "  C/P |   K   |  sig  |   r   |   b   |   S   |  price  " << endl;
    cout << "----------------------------------------------------------------" << endl;
    for (int i = 0; i < parameter_matrix.size(); i++){
        cout << setw(4) << optType << "  |"
             << setw(5) << parameter_matrix[i][0] << "  |"
             << setw(5) << parameter_matrix[i][1] << "  |"
             << setw(5) << parameter_matrix[i][2] << "  |"
             << setw(5) << parameter_matrix[i][3] << "  |"
             << setw(5) << parameter_matrix[i][4] << "  |"
             << setw(8) << price_vector[i] <<  endl;  
    }
    cout << "----------------------------------------------------------------" << endl;
}

// Modifier functions
void PerpetualAmericanOption::toggle()
{ // Change option type (C/P, P/C)
    if (optType == "C")
        optType = "P";
    else
        optType = "C";
}

