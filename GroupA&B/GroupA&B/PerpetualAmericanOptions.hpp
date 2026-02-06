//
//  PerpetualAmericanOptions.hpp
//  GroupA&B
// Perpetual American Options declaration
//  Created by Kevin on 10/24/25.
//

#ifndef PerpetualAmericanOptions_hpp
#define PerpetualAmericanOptions_hpp

#include <string>
#include <vector>
#include "Option.hpp"

using namespace std;


class PerpetualAmericanOption: public Option{
private:
    double K;        // strike price
    double sig;      // volatilit
    double r;        // risk-free interest rate
    double b;        // cost of carry
    double S;        // asset price
    string optType;  // option type
    vector<vector<double>> parameter_matrix; // parameter matrix
    vector<double> price_vector; //price vector
    void init();   //initalize american option
    void copy(const PerpetualAmericanOption& o2);   //copy
    // Kernel funtions for option calculations
    double CallPrice() const;
    double PutPrice() const;
    
public:
    PerpetualAmericanOption(); // Default call option
    PerpetualAmericanOption(double r, double sig, double K, double b, double S, string opt_type); //constructor with parameters
    PerpetualAmericanOption(const PerpetualAmericanOption& option2);    // Copy constructor
    PerpetualAmericanOption(const string& option_type);        // Create option type
    PerpetualAmericanOption(const vector<vector<double>> parameter_matrix, string opt_type); // construct by matrix
    virtual ~PerpetualAmericanOption(); //destructor

    PerpetualAmericanOption& operator = (const PerpetualAmericanOption& option2); //assignment overloading
    //Setters
    void setS(double newS);
    void setSig(double newSig);
    void setR(double newR);
    void setB(double newB);
    void setK(double newK);
    // Functions that calculate option price and sensitivities
    double Price() const;
    
    // Modifier functions
    void toggle();        // Change option type (C/P, P/C)
    vector<double> PriceWithMatrix(); //compute price given matrix of paramters
    void PrintPriceResult(); //print price vector
};

#endif /* PerpetualAmericanOptions_hpp */
