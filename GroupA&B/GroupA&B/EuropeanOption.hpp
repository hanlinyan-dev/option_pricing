//
//  EuropeanOption.hpp
//  GroupA
//  EuropeanOption declaration
//  Created by Kevin on 10/20/25.
//

#ifndef EuropeanOption_hpp
#define EuropeanOption_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include "Option.hpp"
using namespace std;

class EuropeanOption: public Option {
private:
    double T; //expiry time/maturity
    double K; //strike price
    double sig; //volatility
    double r; //ris-free interest rate
    double b; //cost of carry
    double S; //asset price
    string optType; // Option name (call, put)
    vector<vector<double>> parameter_matrix; //parameter matrix
    vector<double> price_vector; //vector of price
    vector<double> delta_vector; //vector of delta
    vector<double> gamma_vector; // vector of gamma
    void init(); //initalize all default values
    void copy(const EuropeanOption& o2); //copy other options
    // Kernel funtions for option calculations
    double CallPrice() const;
    double PutPrice() const;
    double CallDelta() const;
    double PutDelta() const;

public:
    
    EuropeanOption();                            // Default call option
    EuropeanOption(double T, double K, double sig, double r, double b, double S, string OptType);
    EuropeanOption(const EuropeanOption& o2);    // Copy constructor
    EuropeanOption (const string& optionType);    // Create option type
    EuropeanOption(const vector<vector<double>> parameter_matrix, string opt_type); // construct by matrix
    virtual ~EuropeanOption();
    
    EuropeanOption& operator = (const EuropeanOption& option2); // assignment overload
    //Setters
    void setS(double newS);
    void setT(double newT);
    void setSig(double newSig);
    void setR(double newR);
    void setB(double newB);
    void setK(double newK);
    
    // Functions that calculate option price and sensitivities
    double Price() const;
    double Delta() const;
    double Gamma() const;
    double Delta(double h) const; //overload delta method using difference method
    double Gamma(double h) const; //overload gamma method using difference method
    vector<double> optionMatrix(string mode); //return a vector of price, delta, or gamma given a matrix of parameters
    double CalltoPut(double c) const; //use put-call parity to compute put price
    double PuttoCall(double p) const; //use put-call parity to compute call price
    
    bool CheckParity(double c, double p) const; //check the put-call parity
    
    // Modifier functions
    void toggle();        // Change option type (C/P, P/C)
    
    void PrintOptionResult(string mode); //Print price result
    
};


#endif /* EuropeanOption_hpp */
