//
//  EuropeanOption.cpp
//  GroupA
//  EuropeanOption implementation
//  Created by Kevin on 10/20/25.
//

#include "EuropeanOption.hpp"
#include <iostream>
#include <cmath>
#include <vector>
#include <boost/math/distributions/normal.hpp>

using namespace boost::math;

void EuropeanOption::init()
{    // Initialise all default values

    // Default values
    r = 0.05;
    sig= 0.2;

    K = 110.0;
    T = 0.5;

    b = r;            // Black and Scholes stock option model (1973)
    
    optType = "C";        // European Call Option (this is the default type)
}

void EuropeanOption::copy( const EuropeanOption& o2)
{

    r    = o2.r;
    sig  = o2.sig;
    K    = o2.K;
    S    = o2.S;
    T    = o2.T;
    b    = o2.b;
    
    optType = o2.optType;
    
}

//call price
double EuropeanOption::CallPrice() const{
    double tmp = sig * sqrt(T);
    
    double d1 = ( log(S/K) + (b+ (sig*sig)*0.5 ) * T )/ tmp;
    double d2 = d1 - tmp;
    
    normal_distribution<> normalDist(0, 1);
    
    return (S * exp((b-r)*T) * cdf(normalDist, d1)) - (K * exp(-r * T)* cdf(normalDist, d2));
}

//put price
double EuropeanOption::PutPrice() const{
    double tmp = sig * sqrt(T);
    
    double d1 = ( log(S/K) + (b+ (sig*sig)*0.5 ) * T )/ tmp;
    double d2 = d1 - tmp;
    
    normal_distribution<> normalDist(0, 1);
    
    return (K * exp((-r)*T) * cdf(normalDist, -d2)) - (S * exp((b-r) * T)* cdf(normalDist, -d1));
}

double EuropeanOption::CallDelta() const
{
    double tmp = sig * sqrt(T);

    double d1 = ( log(S/K) + (b+ (sig*sig)*0.5 ) * T )/ tmp;
    
    normal_distribution<> normalDist(0, 1);

    return exp((b-r)*T) * cdf(normalDist, d1);
}

double EuropeanOption::PutDelta() const
{
    double tmp = sig * sqrt(T);

    double d1 = ( log(S/K) + ( b + (sig*sig)*0.5 ) * T )/ tmp;
    
    normal_distribution<> normalDist(0, 1);
    
    return exp((b-r)*T) * (cdf(normalDist, d1) - 1.0);
}

EuropeanOption::EuropeanOption(){
    init();
}; // Default call option

EuropeanOption::EuropeanOption(double T_, double K_, double sig_, double r_, double b_, double S_, string OptType): T(T_), K(K_), sig(sig_), r(r_), b(b_), S(S_), optType(OptType){}; // constructor with parameters

EuropeanOption::EuropeanOption(const EuropeanOption& o2){
    copy(o2);
};    // Copy constructor

EuropeanOption::EuropeanOption (const string& optionType){
    init();
    optType = optionType;

    if (optType == "c")
        optType = "C";
    if (optType == "p")
        optType = "P";
    
};    // Create option type

EuropeanOption::EuropeanOption(const vector<vector<double>> parameter_matrix, string opt_type): parameter_matrix(parameter_matrix), optType(opt_type){};

EuropeanOption::~EuropeanOption(){} //destructor

EuropeanOption& EuropeanOption::operator = (const EuropeanOption& option2)
{

    if (this == &option2) return *this;

    copy (option2);

    return *this;
}

//Setters
void EuropeanOption::setS(double newS) { S = newS; }
void EuropeanOption::setT(double newT) { T = newT; }
void EuropeanOption::setSig(double newSig) { sig = newSig; }
void EuropeanOption::setR(double newR) { r = newR; }
void EuropeanOption::setB(double newB) { b = newB; }
void EuropeanOption::setK(double newK) { K = newK; }

// Functions that calculate option price and sensitivities
double EuropeanOption::Price() const
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

double EuropeanOption::Delta() const
{
    if (optType == "C")
        return CallDelta();
    else
        return PutDelta();
}

double EuropeanOption::Gamma() const{
    double tmp = sig * sqrt(T);
    
    double d1 = ( log(S/K) + (b+ (sig*sig)*0.5 ) * T )/ tmp;
    
    normal_distribution<> normalDist(0, 1);
    
    return pdf(normalDist, d1) * exp((b-r)*T) /(S * sig * sqrt(T));
}

double EuropeanOption::Delta(double h) const{
    EuropeanOption option1(T, K, sig, r, b, S+h, optType);
    EuropeanOption option2(T, K, sig, r, b, S-h, optType);
    return (option1.Price()-option2.Price())/ (2*h);
}//overload delta method using difference method

double EuropeanOption::Gamma(double h) const{
    EuropeanOption option1(T, K, sig, r, b, S+h, optType);
    EuropeanOption option2(T, K, sig, r, b, S, optType);
    EuropeanOption option3(T, K, sig, r, b, S-h, optType);
    return (option1.Price() - 2 * option2.Price() + option3.Price())/ (h*h);
}//overload delta method using difference method

vector<double> EuropeanOption::optionMatrix(string mode){
    vector<double> result = {};
    for (vector<double> row: parameter_matrix)
    {
        EuropeanOption option(row[0], row[1], row[2], row[3], row[4], row[5], optType);
        if (mode == "price") {
            result.push_back(option.Price());
            price_vector = result;
        } else if (mode == "delta") {
            result.push_back(option.Delta());
            delta_vector = result;
        } else if (mode == "gamma") {
            result.push_back(option.Gamma());
            gamma_vector = result;
        }
    }
    return result;
    
};  //return a price vector given a matrix of parameters

double EuropeanOption::CalltoPut(double c) const{
    return c + K * exp(-r*T) - S;
}; //use put-call parity to compute put price

double EuropeanOption::PuttoCall(double p) const{
    return p + S - K * exp(-r*T);
}; //use put-call parity to compute call price

bool EuropeanOption::CheckParity(double c, double p) const{
    double LHS = c + K * exp(-r*T);
    double RHS = p + S;
    return LHS == RHS;
}; //check the put-call parity

// Modifier functions
void EuropeanOption::toggle()
{ // Change option type (C/P, P/C)

    if (optType == "C")
        optType = "P";
    else
        optType = "C";
    
}

void EuropeanOption::PrintOptionResult(string mode)
{
    cout << "----------------------------------------------------------------" << endl;
    cout << "  C/P |   T   |   K   |  sig  |   r   |   b   |   S   |  " << mode << "  " << endl;
    cout << "----------------------------------------------------------------" << endl;
    for (int i = 0; i < parameter_matrix.size(); i++){
        cout << setw(4) << optType << "  |"
             << setw(5) << parameter_matrix[i][0] << "  |"
             << setw(5) << parameter_matrix[i][1] << "  |"
             << setw(5) << parameter_matrix[i][2] << "  |"
             << setw(5) << parameter_matrix[i][3] << "  |"
             << setw(5) << parameter_matrix[i][4] << "  |"
             << setw(5) << parameter_matrix[i][5] << "  |";
        if (mode == "price"){
            cout << setw(8) << price_vector[i] << endl;
        } else if (mode == "delta")  {
            cout << setw(8) << delta_vector[i] << endl;
        } else if (mode == "gamma") {
            cout << setw(8) << gamma_vector[i] << endl;
        }
    }
    cout << "----------------------------------------------------------------" << endl;
}


