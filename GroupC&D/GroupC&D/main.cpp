// HardCoded.cpp
//
// C++ code to price an option, essential algorithms.
//
// We take CEV model with a choice of the elaticity parameter
// and the Euler method. We give option price and number of times
// S hits the origin.
//
// Level 9 D
// Adding SDSE function
// Hanlin Yan
// Oct 25 2025
// (C) Datasim Education BC 2008-2011
//

#include "OptionData.hpp"
#include "NormalGenerator.hpp"
#include "Range.cpp"
#include <cmath>
#include <iostream>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_io.hpp>

template <class T> void print(const std::vector<T>& myList)
{  // A generic print function for vectors
    
    std::cout << std::endl << "Size of vector is " << myList.size() << "\n[";

    // We must use a const iterator here, otherwise we get a compiler error.
    typename std::vector<T>::const_iterator i;
    for (i = myList.begin(); i != myList.end(); ++i)
    {
            std::cout << *i << ",";

    }

    std::cout << "]\n";
}

//helper function to compute SD and SE
template <typename Type>
boost::tuple<Type, Type> SDSE(const std::vector<Type>& price, const Type& r, const Type& T)
{
    Type temp1(0), temp2(0);
    for (int i = 0; i < price.size(); i++)
    {
        temp1 += price[i] * price[i];
        temp2 += price[i];
    }

    int M = price.size();
    Type sd = sqrt((temp1 - 1 / M * temp2 * temp2) / (M - 1)) * exp(- r * T);
    Type se = sd / sqrt(M);

    return boost::make_tuple(sd, se);
}

namespace SDEDefinition
{ // Defines drift + diffusion + data

    OptionData* data;                // The data for the option MC

    double drift(double t, double X)
    { // Drift term
    
        return (data->r)*X; // r - D
    }

    
    double diffusion(double t, double X)
    { // Diffusion term
    
        double betaCEV = 1.0;
        return data->sig * pow(X, betaCEV);
        
    }

    double diffusionDerivative(double t, double X)
    { // Diffusion term, needed for the Milstein method
    
        double betaCEV = 1.0;
        return 0.5 * (data->sig) * (betaCEV) * pow(X, 2.0 * betaCEV - 1.0);
    }
} // End of namespace


int main()
{
    std::cout <<  "1 factor MC with explicit Euler\n";
    
    // Store Batch 1 to Batch 2 data in a vector.
    typedef boost::tuple<double, double, double, double, double> TupleFive;
    vector<TupleFive> vecBatch;
    vecBatch.push_back(boost::make_tuple(0.25, 65.0, 0.30, 0.08, 60.0));
    vecBatch.push_back(boost::make_tuple(1.00, 100.0, 0.20, 0.00, 100.0));
    
    // Vector to store the prices of put and call.
    vector<double> vecCallPrice, vecPutPrice;
    
    for (int i = 0; i<vecBatch.size(); i++){
        OptionData myOption;
        myOption.T = vecBatch[i].get<0>();
        myOption.K = vecBatch[i].get<1>();
        myOption.sig = vecBatch[i].get<2>();
        myOption.r = vecBatch[i].get<3>();
        myOption.type = 1;
        double S_0 = vecBatch[i].get<4>();
        
        long N = 100;
        std::cout << "Number of subintervals in time: ";
        std::cin >> N;
        
        // Create the basic SDE (Context class)
        Range<double> range (0.0, myOption.T);
        double VOld = S_0;
        double VNew;
        
        std::vector<double> x = range.mesh(N);
        
        
        // V2 mediator stuff
        long NSim = 50000;
        std::cout << "Number of simulations: ";
        std::cin >> NSim;
        
        double k = myOption.T / double (N);
        double sqrk = sqrt(k);
        
        // Normal random number
        double dW;
        double price1 = 0.0;    // Call Option price
        double price2 = 0.0;    // Put Option price
        
        
        // NormalGenerator is a base class
        NormalGenerator* myNormal = new BoostNormal();
        
        using namespace SDEDefinition;
        SDEDefinition::data = &myOption;
        
        std::vector<double> res;
        int coun = 0; // Number of times S hits origin
        
        // A.
        for (long i = 1; i <= NSim; ++i)
        { // Calculate a path at each iteration
            
            if ((i/10000) * 10000 == i)
            {// Give status after each 1000th iteration
                
                std::cout << i << std::endl;
            }
            
            VOld = S_0;
            for (unsigned long index = 1; index < x.size(); ++index)
            {
                
                // Create a random number
                dW = myNormal->getNormal();
                
                // The FDM (in this case explicit Euler)
                VNew = VOld  + (k * drift(x[index-1], VOld))
                + (sqrk * diffusion(x[index-1], VOld) * dW);
                
                VOld = VNew;
                
                // Spurious values
                if (VNew <= 0.0) coun++;
            }
            
            double tmp1 = myOption.myPayOffFunction(VNew);
            price1 += (tmp1)/double(NSim);
            vecCallPrice.push_back(tmp1);
            myOption.type = -1;
            double tmp2 = myOption.myPayOffFunction(VNew);
            price2 += (tmp2) / double(NSim);
            vecPutPrice.push_back(tmp2);
            myOption.type = 1;
            
        }
        
        // D. Finally, discounting the average price
        price1 *= exp(-myOption.r * myOption.T);
        price2 *= exp(-myOption.r * myOption.T);
        
        // Cleanup; V2 use scoped pointer
        delete myNormal;
        
        //compute sd and se for batch 1
        boost::tuple<double, double> tupleCall = SDSE<double>(vecCallPrice, myOption.r, myOption.T);
        boost::tuple<double, double> tuplePut = SDSE<double>(vecPutPrice, myOption.r, myOption.T);
        std::cout << "Batch " << i + 1 << ", Call: NT = " << N << ", NSIM = " << NSim
        << ", SD = " << tupleCall.get<0>() << ", SE = " << tupleCall.get<1>() << endl;
       std::cout << "Batch " << i + 1 << ", Put: NT = " << N << ", NSIM = " << NSim
        << ", SD = " << tuplePut.get<0>() << ", SE = " << tuplePut.get<1>() << endl;
        
    }
    
    return 0;
}
