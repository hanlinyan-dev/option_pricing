//
//  main.cpp
//  GroupA
//  Exact Solutions of one-factor plain options
//  Created by Kevin on 10/20/25.
//

#include <iostream>
#include "EuropeanOption.hpp"
#include "PerpetualAmericanOptions.hpp"
#include <vector>
#include <iomanip>
#include <random>
using namespace std;


int main(int argc, const char * argv[]) {
    // A Exact Solution of One-factor Plain Options
    // a)
    // Initialize 4 call options
    vector<EuropeanOption> Cbatches = {
        EuropeanOption(0.25, 65, 0.30, 0.08, 0.08, 60, "C"),
        EuropeanOption(1.0, 100, 0.2, 0.0, 0.0, 100, "C"),
        EuropeanOption(1.0, 10, 0.50, 0.12, 0.12, 5, "C"),
        EuropeanOption(30.0, 100.0, 0.30, 0.08, 0.08, 100.0, "C")
    };
    // Initialize 4 put options
    vector<EuropeanOption> Pbatches = {
        EuropeanOption(0.25, 65, 0.30, 0.08, 0.08, 60, "P"),
        EuropeanOption(1.0, 100, 0.2, 0.0, 0.0, 100, "P"),
        EuropeanOption(1.0, 10, 0.50, 0.12, 0.12, 5, "P"),
        EuropeanOption(30.0, 100.0, 0.30, 0.08, 0.08, 100.0, "P")
    };
    // Compute the call and put price using closed-form formula
    for (int i=0; i <Cbatches.size(); i++){
        cout << "Batch " << i+1 << " Call Price = " << Cbatches[i].Price() << " Put Price = " << Pbatches[i].Price() << endl;
    }
    // b)
    // Compute the put option price given call option price using put-call parity
    cout<< endl;
    cout << "Compute Put Price using Put-Call Parity" << endl;
    for (int i=0; i <Cbatches.size(); i++){
        cout << "Batch " << i+1 << " Put Price = " << Pbatches[i].CalltoPut(Cbatches[i].Price()) << endl;
    }
    cout << endl;
    
    // c) test with mesh array of chaning S
    //generate a mesh array of S
    vector<double> S_values = meshArray(10.0, 50.0, 1.0);
    vector<double> prices;
    //use batch 1 as example see how prices changes with S
    cout << "Batch 1" << " Call Prices vs S:" << endl;
    for (vector<double>::iterator it = S_values.begin(); it < S_values.end(); it++){
        Cbatches[0].setS(*it);
        cout << "S=" << *it << "   Price=" << Cbatches[0].Price() <<endl;
        prices.push_back(Cbatches[0].Price());
    }
    
    //d) test with matrix of different parameters
    vector<double> r_values = meshArray(0.00, 0.08, 0.02);
    vector<double> sig_values = meshArray(0.2, 0.6, 0.1);
    vector<double> K_values = meshArray(60, 100, 1);
    vector<double> T_values = meshArray(1, 30, 1);
    
    vector<vector<double>> parameter_matrix;
    int NumOptions = 20;
    parameter_matrix.resize(NumOptions); // allocates 20 empty row vectors

    for (int i = 0; i < NumOptions; ++i) {
        parameter_matrix[i].push_back(T_values[rand() % T_values.size()]);
        parameter_matrix[i].push_back(K_values[rand() % K_values.size()]);
        parameter_matrix[i].push_back(sig_values[rand() % sig_values.size()]);
        parameter_matrix[i].push_back(r_values[rand() % r_values.size()]);
        double b = parameter_matrix[i][3];
        parameter_matrix[i].push_back(b);
        parameter_matrix[i].push_back(S_values[rand() % S_values.size()]);
    }

    //initalize with parameter matrix
    EuropeanOption option_matrix(parameter_matrix, "C");
    //compute the price and print it
    option_matrix.optionMatrix("price");
    option_matrix.PrintOptionResult("price");
    //toggle to compute price of put option
    option_matrix.toggle();
    option_matrix.optionMatrix("price");
    option_matrix.PrintOptionResult("price");
    
    //Option Sensitivities, aka the Greeks
    //a)
    EuropeanOption Option(0.5, 100, 0.36, 0.1, 0, 105, "C");
    cout << "Call Option Delta: " << Option.Delta() << " Gamma: " << Option.Gamma() <<endl;
    Option.toggle();
    cout << "Put Option Delta: " << Option.Delta() << " Gamma: " << Option.Gamma() <<endl;
    
    //b)
    vector<double> delta_values;
    for (vector<double>::iterator it = S_values.begin(); it < S_values.end(); it++){
        Option.setS(*it);
        cout << "S=" << *it << " Put Delta =" << Option.Delta() <<endl;
        delta_values.push_back(Option.Delta());
    }
    //c) incorport delta and gamma into the matrix pricer
    int NumOptions2 = 10;
    parameter_matrix.resize(NumOptions2); // allocates 10 empty row vectors

    for (int i = 0; i < NumOptions2; ++i) {
        parameter_matrix[i].push_back(T_values[rand() % T_values.size()]);
        parameter_matrix[i].push_back(K_values[rand() % K_values.size()]);
        parameter_matrix[i].push_back(sig_values[rand() % sig_values.size()]);
        parameter_matrix[i].push_back(r_values[rand() % r_values.size()]);
        double b = parameter_matrix[i][3];
        parameter_matrix[i].push_back(b);
        parameter_matrix[i].push_back(S_values[rand() % S_values.size()]);
    }

    //initalize with parameter matrix
    EuropeanOption option_matrix2(parameter_matrix, "C");
    //compute the delta and print it
    option_matrix2.optionMatrix("delta");
    option_matrix2.PrintOptionResult("delta");
    //toggle to compute gamma of put option
    option_matrix2.toggle();
    option_matrix2.optionMatrix("gamma");
    option_matrix2.PrintOptionResult("gamma");
    
    //d)we use divided difference to approximate option sensitivities
    //
    Option.toggle();
    vector<double> hs = {1, 0.1, 0.01, 0.001, 0.0001};
    cout << "Call Option Delta: " << Option.Delta() << " Gamma: " << Option.Gamma() <<endl;
    for (double h: hs){
        cout << "h= " << setw(6) << h << " Call Delta: " << Option.Delta(h) << " Gamma: " << Option.Gamma(h) << endl;
    }
    Option.toggle();
    cout << "Put Option Delta: " << Option.Delta() << " Gamma: " << Option.Gamma() <<endl;
    for (double h: hs){
        cout << "h= " << setw(6) << h << " Put Delta: " << Option.Delta(h) << " Gamma: " << Option.Gamma(h) << endl;
    }
    
    for (vector<double>::iterator it = S_values.begin(); it < S_values.end(); it++){
        Option.setS(*it);
        cout << "S=" << setw(6) << *it << " Put Delta =" << std::fixed << std::setprecision(10) << Option.Delta() << std::defaultfloat << endl;
        delta_values.push_back(Option.Delta());
        for (double h: hs)
        {
            cout << "h ="<< setw(6) << h << " Delta= " << std::fixed << std::setprecision(10) << Option.Delta(h) << std::defaultfloat << endl;
        }
        cout << endl;
    }
    
    
    //B. Perpetual American Options
    //a) implemented in PerpetualAmericanOption class
    //b) Test on specific data
    cout << "Perpetual American Option" <<endl;
    PerpetualAmericanOption a_option = PerpetualAmericanOption(100, 0.1, 0.1, 0.02, 110, "C");
    cout << "Call Price: " << a_option.Price() <<endl;
    a_option.toggle();
    cout << "Put Price: " << a_option.Price() <<endl;
    
    //c)
    vector<double> a_prices;
    for (vector<double>::iterator it = S_values.begin(); it < S_values.end(); it++){
        a_option.setS(*it);
        cout << "S=" << *it << "   Price=" << a_option.Price() <<endl;
        a_prices.push_back(a_option.Price());
    }
    
    //d)
    vector<vector<double>> parameter_matrix2;
    parameter_matrix2.resize(NumOptions2); // allocates 20 empty row vectors
    for (int i = 0; i < NumOptions2; i++) {
        parameter_matrix2[i].push_back(K_values[rand() % K_values.size()]);
        parameter_matrix2[i].push_back(sig_values[rand() % sig_values.size()]);
        double rate =r_values[rand() % r_values.size()];
        parameter_matrix2[i].push_back(rate);
        parameter_matrix2[i].push_back(rate/5);
        parameter_matrix2[i].push_back(S_values[rand() % S_values.size()]);
    }
    
    PerpetualAmericanOption a_option_matrix = PerpetualAmericanOption(parameter_matrix2, "C");
    a_option_matrix.PriceWithMatrix();
    a_option_matrix.PrintPriceResult();
    a_option_matrix.toggle();
    a_option_matrix.PriceWithMatrix();
    a_option_matrix.PrintPriceResult();
    

    
}
