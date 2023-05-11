#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <sdsl/int_vector.hpp>

#include "../include/bloom_filter/bloom_filter.hpp"
#include "../include/BooPHF.h"

using namespace std;

//The following class is taken from the BBHash repo's string example found at the link below:
//https://github.com/rizkg/BBHash/blob/alltypes/example_custom_hash_strings.cpp#L18
class Custom_string_Hasher
{
public:
	// the class should have operator () with this signature :
	uint64_t operator ()   (std::string key, uint64_t seed=0) const
	{
		

		uint64_t hash  =  hash_fn(key);
		hash ^= seed;
		
		return hash;
	}
	
     std::hash<std::string> hash_fn;
};

typedef boomphf::mphf<  std::string, Custom_string_Hasher  > boophf_t;

int main()
{
    cout << "--------------Task1: Bloom Filters--------------" << endl;

    //The construction of the following bloom filters follows the Arash Patow repo example found here:
    //https://github.com/ArashPartow/bloom/tree/master#simple-bloom-filter-example
    bloom_parameters parameters11;

    parameters11.projected_element_count = 10000;

    parameters11.false_positive_probability = 0.0078125;

    if (!parameters11)
    {
        std::cout << "Error - Invalid set of bloom filter parameters!" << std::endl;
        return 1;
    }

    parameters11.compute_optimal_parameters();

    // Instantiate Bloom Filter For First Set
    bloom_filter filter11(parameters11);

    //Generate first set K (uses seed to be determininstic)
    std::vector<std::string> k1;
    srand(13); 
    for (int i = 0; i < 10000; ++i)
    {
        std::string s;
        for (int j = 0; j < 31; ++j)
        {
            char letter = 'a' + rand() % 26;
            s += letter;
        }
        //cout << i << ": " << s << endl;
        k1.push_back(s);
    }

    //Generate first set K'
        //For the first set, I have 50% directly taken from K and 50% randomly generated and checked to not be in K
    std::vector<std::string> kprime1; 
    for (int i = 0; i < 10000; ++i)
    {
        if (i >= 5000){
            std::string s;
            for (int j = 0; j < 31; ++j)
            {
                char letter = 'a' + rand() % 26;
                s += letter;
            }
            while(std::find(k1.begin(), k1.end(), s) != k1.end()){
                s = "";
                for (int j = 0; j < 31; ++j)
                {
                    char letter = 'a' + rand() % 26;
                    s += letter;
                }
            }
            kprime1.push_back(s);
        } else {
            kprime1.push_back(k1[i]);
        }
    }

    // Insert some strings
    for (std::size_t i = 0; i < 10000; ++i)
    {
        filter11.insert(k1[i]);
    }
    cout << "Bloom filter 11. FP Rate entered (1/(2^7)). Sets size 10000. String size 31." << endl;
    cout << "K1' is 50% keys present in K1 and 50% keys not in K1." << endl;

    // Query the existence of strings
    auto begin = std::chrono::high_resolution_clock::now();
    int fn = 0;
    for (std::size_t i = 0; i < 5000; ++i)
    {
        if (!filter11.contains(kprime1[i]))
        {
            fn += 1; 
            //std::cout << "BF11 contains" << " " << i << ": " << kprime1[i] << std::endl;
        }
    }

    int fp = 0;
    for (std::size_t i = 5000; i < 10000; ++i)
    {
        if (filter11.contains(kprime1[i]))
        {
            fp += 1; 
            //std::cout << "BF11 contains" << " " << i << ": " << kprime1[i] << std::endl;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "BF11 contains " << fn << " false negatives" << endl;
    std::cout << "BF11 contains " << fp << " false positives with rate " << double(double(fp)/double(5000)) << endl;

    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin);
    cout << "Time taken for querying K1': " << duration.count() << " nanoseconds" << endl;
    cout << "Size of bloom filter: " << filter11.size() << " bits" << endl;


    bloom_parameters parameters12;

    parameters12.projected_element_count = 20000;

    parameters12.false_positive_probability = 0.0078125;

    if (!parameters12)
    {
        std::cout << "Error - Invalid set of bloom filter parameters!" << std::endl;
        return 1;
    }

    parameters12.compute_optimal_parameters();

    // Instantiate Bloom Filter For Second Set
    bloom_filter filter12(parameters12);

    //Generate second set K (uses seed to be determininstic)
    std::vector<std::string> k2;
    srand(9); 
    for (int i = 0; i < 20000; ++i)
    {
        std::string s;
        for (int j = 0; j < 31; ++j)
        {
            char letter = 'a' + rand() % 26;
            s += letter;
        }
        //cout << "k2 " <<  i << ": " << s << endl;
        k2.push_back(s);
    }

    //Generate second set K'
        //For the second set, I have 25% directly taken from K and 75% randomly generated and checked to not be in K
    std::vector<std::string> kprime2; 
    for (int i = 0; i < 20000; ++i)
    {
        if (i < 15000){
            std::string s;
            for (int j = 0; j < 31; ++j)
            {
                char letter = 'a' + rand() % 26;
                s += letter;
            }
            while(std::find(k2.begin(), k2.end(), s) != k2.end()){
                s = "";
                for (int j = 0; j < 31; ++j)
                {
                    char letter = 'a' + rand() % 26;
                    s += letter;
                }
            }
            kprime2.push_back(s);
        } else {
            kprime2.push_back(k2[i]);
        }
    }

    // Insert some strings
    for (std::size_t i = 0; i < 20000; ++i)
    {
        filter12.insert(k2[i]);
    }

    cout << "\nBloom filter 12. FP Rate entered (1/(2^7)). Sets size 20000. String size 31." << endl;
    cout << "K2' is 25% keys present in K2 and 75% keys not in K2." << endl;

    // Query the existence of strings
    begin = std::chrono::high_resolution_clock::now();
    fn = 0;
    for (std::size_t i = 15000; i < 10000; ++i)
    {
        if (!filter12.contains(kprime2[i]))
        {
            fn += 1; 
            //std::cout << "BF11 contains" << " " << i << ": " << kprime1[i] << std::endl;
        }
    }

    fp = 0;
    for (std::size_t i = 0; i < 15000; ++i)
    {
        if (filter12.contains(kprime2[i]))
        {
            fp += 1; 
            //std::cout << "BF11 contains" << " " << i << ": " << kprime1[i] << std::endl;
        }
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "BF12 contains " << fn << " false negatives" << endl;
    std::cout << "BF12 contains " << fp << " false positives with rate " << double(double(fp)/double(15000)) << endl;

    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin);
    cout << "Time taken for querying K2': " << duration.count() << " nanoseconds" << endl;
    cout << "Size of bloom filter: " << filter12.size() << " bits" << endl;

    bloom_parameters parameters13;

    parameters13.projected_element_count = 30000;

    parameters13.false_positive_probability = 0.0078125;

    if (!parameters13)
    {
        std::cout << "Error - Invalid set of bloom filter parameters!" << std::endl;
        return 1;
    }

    parameters13.compute_optimal_parameters();
    // Instantiate Bloom Filter For Third Set
    bloom_filter filter13(parameters13);

    //Generate third set K (uses seed to be determininstic)
    std::vector<std::string> k3;
    srand(18); 
    for (int i = 0; i < 30000; ++i)
    {
        std::string s;
        for (int j = 0; j < 31; ++j)
        {
            char letter = 'a' + rand() % 26;
            s += letter;
        }
        //cout << "k3 " <<  i << ": " << s << endl;
        k3.push_back(s);
    }

    //Generate third set K'
        //For the third set, I have 40% directly taken from K and 60% randomly generated and checked to not be in K
    std::vector<std::string> kprime3; 
    for (int i = 0; i < 30000; ++i)
    {
        if (i >= 12000){
            std::string s;
            for (int j = 0; j < 31; ++j)
            {
                char letter = 'a' + rand() % 26;
                s += letter;
            }
            while(std::find(k3.begin(), k3.end(), s) != k3.end()){
                s = "";
                for (int j = 0; j < 31; ++j)
                {
                    char letter = 'a' + rand() % 26;
                    s += letter;
                }
            }
            kprime3.push_back(s);
        } else {
            kprime3.push_back(k3[i]);
        }
    }

    // Insert some strings
    for (std::size_t i = 0; i < 30000; ++i)
    {
        filter13.insert(k3[i]);
    }

    cout << "\nBloom filter 13. FP Rate entered (1/(2^7)). Sets size 30000. String size 31." << endl;
    cout << "K3' is 40% keys present in K3 and 60% keys not in K3." << endl;

    // Query the existence of strings
    begin = std::chrono::high_resolution_clock::now();
    fn = 0;
    for (std::size_t i = 0; i < 12000; ++i)
    {
        if (!filter13.contains(kprime3[i]))
        {
            fn += 1; 
            //std::cout << "BF11 contains" << " " << i << ": " << kprime1[i] << std::endl;
        }
    }

    fp = 0;
    for (std::size_t i = 12000; i < 30000; ++i)
    {
        if (filter13.contains(kprime3[i]))
        {
            fp += 1; 
            //std::cout << "BF11 contains" << " " << i << ": " << kprime1[i] << std::endl;
        }
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "BF13 contains " << fn << " false negatives" << endl;
    std::cout << "BF13 contains " << fp << " false positives with rate " << double(double(fp)/double(18000)) << endl;

    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin);
    cout << "Time taken for querying K3': " << duration.count() << " nanoseconds" << endl;
    cout << "Size of bloom filter: " << filter13.size() << " bits" << endl;



    bloom_parameters parameters21;

    // How many elements roughly do we expect to insert?
    parameters21.projected_element_count = 10000;

    // Maximum tolerable false positive probability? (0,1)
    parameters21.false_positive_probability = 0.00390625; // 1 in 10000

    if (!parameters21)
    {
        std::cout << "Error - Invalid set of bloom filter parameters!" << std::endl;
        return 1;
    }

    parameters21.compute_optimal_parameters();

    bloom_filter filter21(parameters21);

    // Insert some strings
    for (std::size_t i = 0; i < 10000; ++i)
    {
        //cout << i << ": " << k1[i] << endl;
        filter21.insert(k1[i]);
    }
    cout << "\nBloom filter 21. FP Rate entered (1/(2^8)). Sets size 10000. String size 31." << endl;
    cout << "K1' is 50% keys present in K1 and 50% keys not in K1." << endl;

    // Query the existence of strings
    begin = std::chrono::high_resolution_clock::now();
    fn = 0;
    for (std::size_t i = 0; i < 5000; ++i)
    {
        if (!filter21.contains(kprime1[i]))
        {
            fn += 1; 
            //std::cout << "BF11 contains" << " " << i << ": " << kprime1[i] << std::endl;
        }
    }

    fp = 0;
    for (std::size_t i = 5000; i < 10000; ++i)
    {
        if (filter21.contains(kprime1[i]))
        {
            fp += 1; 
            //std::cout << "BF11 contains" << " " << i << ": " << kprime1[i] << std::endl;
        }
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "BF21 contains " << fn << " false negatives" << endl;
    std::cout << "BF21 contains " << fp << " false positives with rate " << double(double(fp)/double(5000)) << endl;

    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin);
    cout << "Time taken for querying K1': " << duration.count() << " nanoseconds" << endl;
    cout << "Size of bloom filter: " << filter21.size() << " bits" << endl;


    bloom_parameters parameters22;

    // How many elements roughly do we expect to insert?
    parameters22.projected_element_count = 20000;

    // Maximum tolerable false positive probability? (0,1)
    parameters22.false_positive_probability = 0.00390625; // 1 in 10000

    if (!parameters22)
    {
        std::cout << "Error - Invalid set of bloom filter parameters!" << std::endl;
        return 1;
    }

    parameters22.compute_optimal_parameters();
    bloom_filter filter22(parameters22);

    // Insert some strings
    for (std::size_t i = 0; i < 20000; ++i)
    {
        //cout << i << ": " << k2[i] << endl;
        filter22.insert(k2[i]);
    }

    cout << "\nBloom filter 22. FP Rate entered (1/(2^8)). Sets size 20000. String size 31." << endl;
    cout << "K2' is 25% keys present in K2 and 75% keys not in K2." << endl;

    // Query the existence of strings
    begin = std::chrono::high_resolution_clock::now();
    fn = 0;
    for (std::size_t i = 15000; i < 20000; ++i)
    {
        if (!filter22.contains(kprime2[i]))
        {
            fn += 1; 
            //std::cout << "BF11 contains" << " " << i << ": " << kprime1[i] << std::endl;
        }
    }

    fp = 0;
    for (std::size_t i = 0; i < 15000; ++i)
    {
        if (filter22.contains(kprime2[i]))
        {
            fp += 1; 
            //std::cout << "BF11 contains" << " " << i << ": " << kprime1[i] << std::endl;
        }
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "BF22 contains " << fn << " false negatives" << endl;
    std::cout << "BF22 contains " << fp << " false positives with rate " << double(double(fp)/double(15000)) << endl;

    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin);
    cout << "Time taken for querying K2': " << duration.count() << " nanoseconds" << endl;
    cout << "Size of bloom filter: " << filter22.size() << " bits" << endl;

    bloom_parameters parameters23;

    // How many elements roughly do we expect to insert?
    parameters23.projected_element_count = 30000;

    // Maximum tolerable false positive probability? (0,1)
    parameters23.false_positive_probability = 0.00390625; // 1 in 10000

    if (!parameters23)
    {
        std::cout << "Error - Invalid set of bloom filter parameters!" << std::endl;
        return 1;
    }

    parameters23.compute_optimal_parameters();
    bloom_filter filter23(parameters23);

    // Insert some strings
    for (std::size_t i = 0; i < 30000; ++i)
    {
        //cout << i << ": " << k3[i] << endl;
        filter23.insert(k3[i]);
    }

    cout << "\nBloom filter 23. FP Rate entered (1/(2^8)). Sets size 30000. String size 31." << endl;
    cout << "K3' is 40% keys present in K3 and 60% keys not in K3." << endl;

    // Query the existence of strings
    begin = std::chrono::high_resolution_clock::now();
    fn = 0;
    for (std::size_t i = 0; i < 12000; ++i)
    {
        if (!filter23.contains(kprime3[i]))
        {
            fn += 1; 
            //std::cout << "BF11 contains" << " " << i << ": " << kprime1[i] << std::endl;
        }
    }

    fp = 0;
    for (std::size_t i = 12000; i < 30000; ++i)
    {
        if (filter23.contains(kprime3[i]))
        {
            fp += 1; 
            //std::cout << "BF11 contains" << " " << i << ": " << kprime1[i] << std::endl;
        }
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "BF23 contains " << fn << " false negatives" << endl;
    std::cout << "BF23 contains " << fp << " false positives with rate " << double(double(fp)/double(18000)) << endl;

    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin);
    cout << "Time taken for querying K3': " << duration.count() << " nanoseconds" << endl;
    cout << "Size of bloom filter: " << filter23.size() << " bits" << endl;


    bloom_parameters parameters31;

    // How many elements roughly do we expect to insert?
    parameters31.projected_element_count = 10000;

    // Maximum tolerable false positive probability? (0,1)
    parameters31.false_positive_probability = 0.0009765625; // 1 in 10000

    if (!parameters31)
    {
        std::cout << "Error - Invalid set of bloom filter parameters!" << std::endl;
        return 1;
    }

    parameters31.compute_optimal_parameters();

    bloom_filter filter31(parameters31);

    // Insert some strings
    for (std::size_t i = 0; i < 10000; ++i)
    {
        //cout << i << ": " << k1[i] << endl;
        filter31.insert(k1[i]);
    }

    cout << "\nBloom filter 31. FP Rate entered (1/(2^10)). Sets size 10000. String size 31." << endl;
    cout << "K1' is 50% keys present in K1 and 50% keys not in K1." << endl;

    // Query the existence of strings
    begin = std::chrono::high_resolution_clock::now();
    fn = 0;
    for (std::size_t i = 0; i < 5000; ++i)
    {
        if (!filter31.contains(kprime1[i]))
        {
            fn += 1; 
            //std::cout << "BF11 contains" << " " << i << ": " << kprime1[i] << std::endl;
        }
    }

    fp = 0;
    for (std::size_t i = 5000; i < 10000; ++i)
    {
        if (filter31.contains(kprime1[i]))
        {
            fp += 1; 
            //std::cout << "BF11 contains" << " " << i << ": " << kprime1[i] << std::endl;
        }
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "BF31 contains " << fn << " false negatives" << endl;
    std::cout << "BF31 contains " << fp << " false positives with rate " << double(double(fp)/double(5000)) << endl;

    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin);
    cout << "Time taken for querying K1': " << duration.count() << " nanoseconds" << endl;
    cout << "Size of bloom filter: " << filter31.size() << " bits" << endl;

    bloom_parameters parameters32;

    // How many elements roughly do we expect to insert?
    parameters32.projected_element_count = 20000;

    // Maximum tolerable false positive probability? (0,1)
    parameters32.false_positive_probability = 0.0009765625; // 1 in 10000

    if (!parameters32)
    {
        std::cout << "Error - Invalid set of bloom filter parameters!" << std::endl;
        return 1;
    }

    parameters32.compute_optimal_parameters();
    bloom_filter filter32(parameters32);

    // Insert some strings
    for (std::size_t i = 0; i < 20000; ++i)
    {
        //cout << i << ": " << k2[i] << endl;
        filter32.insert(k2[i]);
    }

    cout << "\nBloom filter 32. FP Rate entered (1/(2^10)). Sets size 20000. String size 31." << endl;
    cout << "K2' is 25% keys present in K2 and 75% keys not in K2." << endl;

    // Query the existence of strings
    begin = std::chrono::high_resolution_clock::now();
    fn = 0;
    for (std::size_t i = 15000; i < 20000; ++i)
    {
        if (!filter32.contains(kprime2[i]))
        {
            fn += 1; 
            //std::cout << "BF11 contains" << " " << i << ": " << kprime1[i] << std::endl;
        }
    }

    fp = 0;
    for (std::size_t i = 0; i < 15000; ++i)
    {
        if (filter32.contains(kprime2[i]))
        {
            fp += 1; 
            //std::cout << "BF11 contains" << " " << i << ": " << kprime1[i] << std::endl;
        }
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "BF32 contains " << fn << " false negatives" << endl;
    std::cout << "BF32 contains " << fp << " false positives with rate " << double(double(fp)/double(15000)) << endl;

    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin);
    cout << "Time taken for querying K2': " << duration.count() << " nanoseconds" << endl;
    cout << "Size of bloom filter: " << filter32.size() << " bits" << endl;

    bloom_parameters parameters33;

    // How many elements roughly do we expect to insert?
    parameters33.projected_element_count = 30000;

    // Maximum tolerable false positive probability? (0,1)
    parameters33.false_positive_probability = 0.0009765625; // 1 in 10000

    if (!parameters33)
    {
        std::cout << "Error - Invalid set of bloom filter parameters!" << std::endl;
        return 1;
    }

    parameters33.compute_optimal_parameters();
    bloom_filter filter33(parameters33);

    // Insert some strings
    for (std::size_t i = 0; i < 30000; ++i)
    {
        //cout << i << ": " << k3[i] << endl;
        filter33.insert(k3[i]);
    }

    cout << "\nBloom filter 33. FP Rate entered (1/(2^10)). Sets size 30000. String size 31." << endl;
    cout << "K3' is 40% keys present in K3 and 60% keys not in K3." << endl;

    // Query the existence of strings
    begin = std::chrono::high_resolution_clock::now();
    fn = 0;
    for (std::size_t i = 0; i < 12000; ++i)
    {
        if (!filter33.contains(kprime3[i]))
        {
            fn += 1; 
            //std::cout << "BF11 contains" << " " << i << ": " << kprime1[i] << std::endl;
        }
    }

    fp = 0;
    for (std::size_t i = 12000; i < 30000; ++i)
    {
        if (filter33.contains(kprime3[i]))
        {
            fp += 1; 
            //std::cout << "BF11 contains" << " " << i << ": " << kprime1[i] << std::endl;
        }
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "BF33 contains " << fn << " false negatives" << endl;
    std::cout << "BF33 contains " << fp << " false positives with rate " << double(double(fp)/double(18000)) << endl;

    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin);
    cout << "Time taken for querying K3': " << duration.count() << " nanoseconds" << endl;
    cout << "Size of bloom filter: " << filter33.size() << " bits" << endl;

    cout << "\n\n--------------Task2: MPHF--------------" << endl;

    //u_int64_t nelem = 10000;
	uint nthreads = 1;
    boophf_t * bphf1 = NULL;
    boophf_t * bphf2 = NULL;
    boophf_t * bphf3 = NULL;
	
	// mphf takes as input a c++ range. A std::vector is already a c++ range
	
	double gammaFactor = 2.0; // lowest bit/elem is achieved with gamma=1, higher values lead to larger mphf but faster construction/query
	// gamma = 2 is a good tradeoff (leads to approx 3.7 bits/key )

    cout << "MPHF 1. Sets size 10000. String size 31. Gamma 2." << endl;
    cout << "K1' is the same K1' as above, it is 50% keys present in K1 and 50% keys not in K1." << endl;
    bphf1 = new boomphf::mphf<std::string,Custom_string_Hasher>(10000,k1,nthreads,gammaFactor);

    printf("boophf  bits/elem : %f\n",(float) (bphf1->totalBitSize())/10000);
    fn = 0; 
    begin = std::chrono::high_resolution_clock::now();
    for (u_int64_t i = 0; i < 5000; i++){
		uint64_t  idx = bphf1->lookup(kprime1[i]);
        //cout << idx << endl;
        if (idx == ULLONG_MAX){
            fp += 1; 
        }
	}
    
    fp = 0; 
    for (u_int64_t i = 5000; i < 10000; i++){
		uint64_t  idx = bphf1->lookup(kprime1[i]);
        //cout << idx << endl;
        if (idx != ULLONG_MAX){
            fp += 1; 
        }
	}
    end = std::chrono::high_resolution_clock::now();
    std::cout << "MPHF1 contains " << fn << " false negatives" << endl;
    std::cout << "MPHF1 contains " << fp << " false positives with rate " << double(double(fp)/double(5000)) << endl;

    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin);
    cout << "Time taken for querying K1': " << duration.count() << " nanoseconds" << endl;


    cout << "\nMPHF 2. Sets size 20000. String size 31. Gamma 2." << endl;
    cout << "K2' is the same K2' as above, it is 25% keys present in K2 and 75% keys not in K2." << endl;
    bphf2 = new boomphf::mphf<std::string,Custom_string_Hasher>(20000,k2,nthreads,gammaFactor);

    printf("boophf  bits/elem : %f\n",(float) (bphf2->totalBitSize())/20000);
    begin = std::chrono::high_resolution_clock::now();
    fn = 0; 
    for (u_int64_t i = 15000; i < 20000; i++){
		uint64_t  idx = bphf2->lookup(kprime2[i]);
        //cout << idx << endl;
        if (idx == ULLONG_MAX){
            fn += 1; 
        }
	}
    fp = 0; 
    for (u_int64_t i = 0; i < 15000; i++){
		uint64_t  idx = bphf2->lookup(kprime2[i]);
        //cout << idx << endl;
        if (idx != ULLONG_MAX){
            fp += 1; 
        }
	}
    end = std::chrono::high_resolution_clock::now();
    std::cout << "MPHF2 contains " << fn << " false negatives" << endl;
    std::cout << "MPHF2 contains " << fp << " false positives with rate " << double(double(fp)/double(15000)) << endl;

    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin);
    cout << "Time taken for querying K2': " << duration.count() << " nanoseconds" << endl;


    cout << "\nMPHF 3. Sets size 30000. String size 31. Gamma 2." << endl;
    cout << "K3' is the same K3' as above, it is 40% keys present in K3 and 60% keys not in K3." << endl;
    bphf3 = new boomphf::mphf<std::string,Custom_string_Hasher>(30000,k3,nthreads,gammaFactor);

    printf("boophf  bits/elem : %f\n",(float) (bphf3->totalBitSize())/30000);
    begin = std::chrono::high_resolution_clock::now();
    fn = 0;
    for (u_int64_t i = 0; i < 12000; i++){
		uint64_t  idx = bphf3->lookup(kprime3[i]);
        //cout << idx << endl;
        if (idx == ULLONG_MAX){
            fn += 1; 
        }
	}
    fp = 0; 
    for (u_int64_t i = 12000; i < 30000; i++){
		uint64_t  idx = bphf3->lookup(kprime3[i]);
        //cout << idx << endl;
        if (idx != ULLONG_MAX){
            fp += 1; 
        }
	}
    end = std::chrono::high_resolution_clock::now();
    std::cout << "MPHF3 contains " << fn << " false negatives" << endl;
    std::cout << "MPHF3 contains " << fp << " false positives with rate " << double(double(fp)/double(18000)) << endl;

    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin);
    cout << "Time taken for querying K3': " << duration.count() << " nanoseconds" << endl;

    // std::cout << std::hash<std::string>{}(k1[0]) << '\n';
    // cout << (std::hash<std::string>{}(k1[0]) & 127) << endl;
    // std::cout << std::hash<std::string>{}(k1[1]) << '\n';

    cout << "\n\n--------------Task3: Fingerprint Array--------------" << endl;

    sdsl::int_vector<> f11(10000, 0, 7);
    for (u_int64_t i = 0; i < 10000; i++){
		uint64_t  idx = bphf1->lookup(k1[i]);
        f11[idx] = (std::hash<std::string>{}(k1[i]) & 127);
	}

    cout << "MPHF + FP 11. Fingerprint size 7. Sets size 10000. String size 31. Gamma 2." << endl;
    cout << "K1' is the same K1' as above, it is 50% keys present in K1 and 50% keys not in K1." << endl;
    //query
    begin = std::chrono::high_resolution_clock::now();
    fn = 0; 
    for (u_int64_t i = 0; i < 5000; i++){
		uint64_t  idx = bphf1->lookup(kprime1[i]);
        if (idx == ULLONG_MAX || f11[idx] != (std::hash<std::string>{}(kprime1[i]) & 127)){
            fn += 1; 
        }
	}
    fp = 0; 
    for (u_int64_t i = 5000; i < 10000; i++){
		uint64_t  idx = bphf1->lookup(kprime1[i]);
        if (idx != ULLONG_MAX && f11[idx] == (std::hash<std::string>{}(kprime1[i]) & 127)){
            fp += 1; 
        }
	}
    end = std::chrono::high_resolution_clock::now();
    std::cout << "FP11 contains " << fn << " false negatives" << endl;
    std::cout << "FP11 contains " << fp << " false positives with rate " << double(double(fp)/double(5000)) << endl;

    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin);
    cout << "Time taken for querying K1': " << duration.count() << " nanoseconds" << endl;
    cout << "Size of fingerprint array alone: " << (size_in_bytes(f11)*8) << " bits. Look at MPHF section for its size." << endl;


    sdsl::int_vector<> f12(20000, 0, 7);
    for (u_int64_t i = 0; i < 20000; i++){
		uint64_t  idx = bphf2->lookup(k2[i]);
        f12[idx] = (std::hash<std::string>{}(k2[i]) & 127);
	}

    cout << "\nMPHF + FP 12. Fingerprint size 7. Sets size 20000. String size 31. Gamma 2." << endl;
    cout << "K2' is the same K2' as above, it is 25% keys present in K2 and 75% keys not in K2." << endl;
    //query
    begin = std::chrono::high_resolution_clock::now();
    fn = 0; 
    for (u_int64_t i = 15000; i < 20000; i++){
		uint64_t  idx = bphf2->lookup(kprime2[i]);
        if (idx == ULLONG_MAX || f12[idx] != (std::hash<std::string>{}(kprime2[i]) & 127)){
            fn += 1; 
        }
	}
    fp = 0; 
    for (u_int64_t i = 0; i < 15000; i++){
		uint64_t  idx = bphf2->lookup(kprime2[i]);
        if (idx != ULLONG_MAX && f12[idx] == (std::hash<std::string>{}(kprime2[i]) & 127)){
            fp += 1; 
        }
	}
    end = std::chrono::high_resolution_clock::now();
    std::cout << "FP12 contains " << fn << " false negatives" << endl;
    std::cout << "FP12 contains " << fp << " false positives with rate " << double(double(fp)/double(15000)) << endl;

    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin);
    cout << "Time taken for querying K2': " << duration.count() << " nanoseconds" << endl;
    cout << "Size of fingerprint array alone: " << (size_in_bytes(f12)*8) << " bits. Look at MPHF section for its size." << endl;


    sdsl::int_vector<> f13(30000, 0, 7);
    for (u_int64_t i = 0; i < 30000; i++){
		uint64_t  idx = bphf3->lookup(k3[i]);
        f13[idx] = (std::hash<std::string>{}(k3[i]) & 127);
	}

    cout << "\nMPHF + FP 13. Fingerprint size 7. Sets size 30000. String size 31. Gamma 2." << endl;
    cout << "K3' is the same K3' as above, it is 40% keys present in K3 and 60% keys not in K3." << endl;
    //query
    begin = std::chrono::high_resolution_clock::now();
    fn = 0; 
    for (u_int64_t i = 0; i < 12000; i++){
		uint64_t  idx = bphf3->lookup(kprime3[i]);
        if (idx == ULLONG_MAX || f13[idx] != (std::hash<std::string>{}(kprime3[i]) & 127)){
            fn += 1; 
        }
	}
    fp = 0; 
    for (u_int64_t i = 12000; i < 30000; i++){
		uint64_t  idx = bphf3->lookup(kprime3[i]);
        if (idx != ULLONG_MAX && f13[idx] == (std::hash<std::string>{}(kprime3[i]) & 127)){
            fp += 1; 
        }
	}
    end = std::chrono::high_resolution_clock::now();
    std::cout << "FP13 contains " << fn << " false negatives" << endl;
    std::cout << "FP13 contains " << fp << " false positives with rate " << double(double(fp)/double(18000)) << endl;

    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin);
    cout << "Time taken for querying K3': " << duration.count() << " nanoseconds" << endl;
    cout << "Size of fingerprint array alone: " << (size_in_bytes(f13)*8) << " bits. Look at MPHF section for its size." << endl;





    // std::cout << std::hash<std::string>{}(k1[1]) << '\n';
    // cout << (std::hash<std::string>{}(k1[1]) & 255) << endl;

    sdsl::int_vector<> f21(10000, 0, 8);
    for (u_int64_t i = 0; i < 10000; i++){
		uint64_t  idx = bphf1->lookup(k1[i]);
        f21[idx] = (std::hash<std::string>{}(k1[i]) & 255);
	}

    cout << "\nMPHF + FP 21. Fingerprint size 8. Sets size 10000. String size 31. Gamma 2." << endl;
    cout << "K1' is the same K1' as above, it is 50% keys present in K1 and 50% keys not in K1." << endl;
    //query
    begin = std::chrono::high_resolution_clock::now();
    fn = 0; 
    for (u_int64_t i = 0; i < 5000; i++){
		uint64_t  idx = bphf1->lookup(kprime1[i]);
        if (idx == ULLONG_MAX || f21[idx] != (std::hash<std::string>{}(kprime1[i]) & 255)){
            fn += 1; 
        }
	}
    fp = 0; 
    for (u_int64_t i = 5000; i < 10000; i++){
		uint64_t  idx = bphf1->lookup(kprime1[i]);
        if (idx != ULLONG_MAX && f21[idx] == (std::hash<std::string>{}(kprime1[i]) & 255)){
            fp += 1; 
        }
	}
    end = std::chrono::high_resolution_clock::now();
    std::cout << "FP21 contains " << fn << " false negatives" << endl;
    std::cout << "FP21 contains " << fp << " false positives with rate " << double(double(fp)/double(5000)) << endl;

    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin);
    cout << "Time taken for querying K1': " << duration.count() << " nanoseconds" << endl;
    cout << "Size of fingerprint array alone: " << (size_in_bytes(f21)*8) << " bits. Look at MPHF section for its size." << endl;


    sdsl::int_vector<> f22(20000, 0, 8);
    for (u_int64_t i = 0; i < 20000; i++){
		uint64_t  idx = bphf2->lookup(k2[i]);
        f22[idx] = (std::hash<std::string>{}(k2[i]) & 255);
	}

    cout << "\nMPHF + FP 22. Fingerprint size 8. Sets size 20000. String size 31. Gamma 2." << endl;
    cout << "K2' is the same K2' as above, it is 25% keys present in K2 and 75% keys not in K2." << endl;
    //query
    begin = std::chrono::high_resolution_clock::now();
    fn = 0; 
    for (u_int64_t i = 15000; i < 20000; i++){
		uint64_t  idx = bphf2->lookup(kprime2[i]);
        if (idx == ULLONG_MAX || f22[idx] != (std::hash<std::string>{}(kprime2[i]) & 255)){
            fn += 1; 
        }
	}
    fp = 0; 
    for (u_int64_t i = 0; i < 15000; i++){
		uint64_t  idx = bphf2->lookup(kprime2[i]);
        if (idx != ULLONG_MAX && f22[idx] == (std::hash<std::string>{}(kprime2[i]) & 255)){
            fp += 1; 
        }
	}
    end = std::chrono::high_resolution_clock::now();
    std::cout << "FP22 contains " << fn << " false negatives" << endl;
    std::cout << "FP22 contains " << fp << " false positives with rate " << double(double(fp)/double(15000)) << endl;

    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin);
    cout << "Time taken for querying K2': " << duration.count() << " nanoseconds" << endl;
    cout << "Size of fingerprint array alone: " << (size_in_bytes(f22)*8) << " bits. Look at MPHF section for its size." << endl;

    sdsl::int_vector<> f23(30000, 0, 8);
    for (u_int64_t i = 0; i < 30000; i++){
		uint64_t  idx = bphf3->lookup(k3[i]);
        f23[idx] = (std::hash<std::string>{}(k3[i]) & 255);
	}

    cout << "\nMPHF + FP 23. Fingerprint size 8. Sets size 30000. String size 31. Gamma 2." << endl;
    cout << "K3' is the same K3' as above, it is 40% keys present in K3 and 60% keys not in K3." << endl;
    //query
    begin = std::chrono::high_resolution_clock::now();
    fn = 0; 
    for (u_int64_t i = 0; i < 12000; i++){
		uint64_t  idx = bphf3->lookup(kprime3[i]);
        if (idx == ULLONG_MAX || f23[idx] != (std::hash<std::string>{}(kprime3[i]) & 255)){
            fn += 1; 
        }
	}
    fp = 0; 
    for (u_int64_t i = 12000; i < 30000; i++){
		uint64_t  idx = bphf3->lookup(kprime3[i]);
        if (idx != ULLONG_MAX && f23[idx] == (std::hash<std::string>{}(kprime3[i]) & 255)){
            fp += 1; 
        }
	}
    end = std::chrono::high_resolution_clock::now();
    std::cout << "FP23 contains " << fn << " false negatives" << endl;
    std::cout << "FP23 contains " << fp << " false positives with rate " << double(double(fp)/double(18000)) << endl;

    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin);
    cout << "Time taken for querying K3': " << duration.count() << " nanoseconds" << endl;
    cout << "Size of fingerprint array alone: " << (size_in_bytes(f23)*8) << " bits. Look at MPHF section for its size." << endl;








    // std::cout << std::hash<std::string>{}(k1[3]) << '\n';
    // cout << (std::hash<std::string>{}(k1[3]) & 1023) << endl;

    sdsl::int_vector<> f31(10000, 0, 10);
    for (u_int64_t i = 0; i < 10000; i++){
		uint64_t  idx = bphf1->lookup(k1[i]);
        f31[idx] = (std::hash<std::string>{}(k1[i]) & 1023);
	}

    cout << "\nMPHF + FP 31. Fingerprint size 10. Sets size 10000. String size 31. Gamma 2." << endl;
    cout << "K1' is the same K1' as above, it is 50% keys present in K1 and 50% keys not in K1." << endl;
    //query
    begin = std::chrono::high_resolution_clock::now();
    fn = 0; 
    for (u_int64_t i = 0; i < 5000; i++){
		uint64_t  idx = bphf1->lookup(kprime1[i]);
        if (idx == ULLONG_MAX || f31[idx] != (std::hash<std::string>{}(kprime1[i]) & 1023)){
            fn += 1; 
        }
	}
    fp = 0; 
    for (u_int64_t i = 5000; i < 10000; i++){
		uint64_t  idx = bphf1->lookup(kprime1[i]);
        if (idx != ULLONG_MAX && f31[idx] == (std::hash<std::string>{}(kprime1[i]) & 1023)){
            fp += 1; 
        }
	}
    end = std::chrono::high_resolution_clock::now();
    std::cout << "FP31 contains " << fn << " false negatives" << endl;
    std::cout << "FP31 contains " << fp << " false positives with rate " << double(double(fp)/double(5000)) << endl;

    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin);
    cout << "Time taken for querying K1': " << duration.count() << " nanoseconds" << endl;
    cout << "Size of fingerprint array alone: " << (size_in_bytes(f31)*8) << " bits. Look at MPHF section for its size." << endl;


    sdsl::int_vector<> f32(20000, 0, 10);
    for (u_int64_t i = 0; i < 20000; i++){
		uint64_t  idx = bphf2->lookup(k2[i]);
        f32[idx] = (std::hash<std::string>{}(k2[i]) & 1023);
	}

    cout << "\nMPHF + FP 32. Fingerprint size 10. Sets size 20000. String size 31. Gamma 2." << endl;
    cout << "K2' is the same K2' as above, it is 25% keys present in K2 and 75% keys not in K2." << endl;
    //query
    begin = std::chrono::high_resolution_clock::now();
    fn = 0; 
    for (u_int64_t i = 15000; i < 20000; i++){
		uint64_t  idx = bphf2->lookup(kprime2[i]);
        if (idx == ULLONG_MAX || f32[idx] != (std::hash<std::string>{}(kprime2[i]) & 1023)){
            fn += 1; 
        }
	}
    fp = 0; 
    for (u_int64_t i = 0; i < 15000; i++){
		uint64_t  idx = bphf2->lookup(kprime2[i]);
        if (idx != ULLONG_MAX && f32[idx] == (std::hash<std::string>{}(kprime2[i]) & 1023)){
            fp += 1; 
        }
	}
    end = std::chrono::high_resolution_clock::now();
    std::cout << "FP32 contains " << fn << " false negatives" << endl;
    std::cout << "FP32 contains " << fp << " false positives with rate " << double(double(fp)/double(15000)) << endl;

    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin);
    cout << "Time taken for querying K2': " << duration.count() << " nanoseconds" << endl;
    cout << "Size of fingerprint array alone: " << (size_in_bytes(f32)*8) << " bits. Look at MPHF section for its size." << endl;

    sdsl::int_vector<> f33(30000, 0, 10);
    for (u_int64_t i = 0; i < 30000; i++){
		uint64_t  idx = bphf3->lookup(k3[i]);
        f33[idx] = (std::hash<std::string>{}(k3[i]) & 1023);
	}

    cout << "\nMPHF + FP 33. Fingerprint size 10. Sets size 30000. String size 31. Gamma 2." << endl;
    cout << "K3' is the same K3' as above, it is 40% keys present in K3 and 60% keys not in K3." << endl;
    //query
    begin = std::chrono::high_resolution_clock::now();
    fn = 0; 
    for (u_int64_t i = 0; i < 12000; i++){
		uint64_t  idx = bphf3->lookup(kprime3[i]);
        if (idx == ULLONG_MAX || f33[idx] != (std::hash<std::string>{}(kprime3[i]) & 1023)){
            fn += 1; 
        }
	}
    fp = 0; 
    for (u_int64_t i = 12000; i < 30000; i++){
		uint64_t  idx = bphf3->lookup(kprime3[i]);
        if (idx != ULLONG_MAX && f33[idx] == (std::hash<std::string>{}(kprime3[i]) & 1023)){
            fp += 1; 
        }
	}
    end = std::chrono::high_resolution_clock::now();
    std::cout << "FP33 contains " << fn << " false negatives" << endl;
    std::cout << "FP33 contains " << fp << " false positives with rate " << double(double(fp)/double(18000)) << endl;

    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin);
    cout << "Time taken for querying K3': " << duration.count() << " nanoseconds" << endl;
    cout << "Size of fingerprint array alone: " << (size_in_bytes(f33)*8) << " bits. Look at MPHF section for its size." << endl;


    return 0;
}