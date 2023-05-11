#include <iostream>
#include <string>
#include <algorithm>
//#include <cstdlib>
#include <vector>
#include <sdsl/int_vector.hpp>

#include "../include/bloom_filter/bloom_filter.hpp"
#include "../include/BooPHF.h"

using namespace std;

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
    bloom_parameters parameters1;

    // How many elements roughly do we expect to insert?
    parameters1.projected_element_count = 10000;

    // Maximum tolerable false positive probability? (0,1)
    parameters1.false_positive_probability = 0.0078125; // 1 in 10000

    if (!parameters1)
    {
        std::cout << "Error - Invalid set of bloom filter parameters!" << std::endl;
        return 1;
    }

    parameters1.compute_optimal_parameters();

    // Instantiate Bloom Filter For First Set
    bloom_filter filter11(parameters1);

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

    // Query the existence of strings
    int fn = 0;
    for (std::size_t i = 0; i < 5000; ++i)
    {
        if (!filter11.contains(kprime1[i]))
        {
            fn += 1; 
            //std::cout << "BF11 contains" << " " << i << ": " << kprime1[i] << std::endl;
        }
    }
    std::cout << "BF11 contains " << fn << " false negatives" << endl;

    int fp = 0;
    for (std::size_t i = 5000; i < 10000; ++i)
    {
        if (filter11.contains(kprime1[i]))
        {
            fp += 1; 
            //std::cout << "BF11 contains" << " " << i << ": " << kprime1[i] << std::endl;
        }
    }
    std::cout << "BF11 contains " << fp << " false positives" << endl;



    // Instantiate Bloom Filter For Second Set
    bloom_filter filter12(parameters1);

    //Generate second set K (uses seed to be determininstic)
    std::vector<std::string> k2;
    srand(9); 
    for (int i = 0; i < 10000; ++i)
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
    for (int i = 0; i < 10000; ++i)
    {
        if (i < 7500){
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
    for (std::size_t i = 0; i < 10000; ++i)
    {
        filter12.insert(k2[i]);
    }

    // Query the existence of strings
    fn = 0;
    for (std::size_t i = 7500; i < 10000; ++i)
    {
        if (!filter12.contains(kprime2[i]))
        {
            fn += 1; 
            //std::cout << "BF11 contains" << " " << i << ": " << kprime1[i] << std::endl;
        }
    }
    std::cout << "BF12 contains " << fn << " false negatives" << endl;

    fp = 0;
    for (std::size_t i = 0; i < 7500; ++i)
    {
        if (filter12.contains(kprime2[i]))
        {
            fp += 1; 
            //std::cout << "BF11 contains" << " " << i << ": " << kprime1[i] << std::endl;
        }
    }
    std::cout << "BF12 contains " << fp << " false positives" << endl;


    // Instantiate Bloom Filter For Third Set
    bloom_filter filter13(parameters1);

    //Generate third set K (uses seed to be determininstic)
    std::vector<std::string> k3;
    srand(18); 
    for (int i = 0; i < 10000; ++i)
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
    for (int i = 0; i < 10000; ++i)
    {
        if (i >= 4000){
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
    for (std::size_t i = 0; i < 10000; ++i)
    {
        filter13.insert(k3[i]);
    }

    // Query the existence of strings
    fn = 0;
    for (std::size_t i = 0; i < 4000; ++i)
    {
        if (!filter13.contains(kprime3[i]))
        {
            fn += 1; 
            //std::cout << "BF11 contains" << " " << i << ": " << kprime1[i] << std::endl;
        }
    }
    std::cout << "BF13 contains " << fn << " false negatives" << endl;

    fp = 0;
    for (std::size_t i = 4000; i < 10000; ++i)
    {
        if (filter13.contains(kprime3[i]))
        {
            fp += 1; 
            //std::cout << "BF11 contains" << " " << i << ": " << kprime1[i] << std::endl;
        }
    }
    std::cout << "BF13 contains " << fp << " false positives" << endl;



    bloom_parameters parameters2;

    // How many elements roughly do we expect to insert?
    parameters2.projected_element_count = 10000;

    // Maximum tolerable false positive probability? (0,1)
    parameters2.false_positive_probability = 0.00390625; // 1 in 10000

    if (!parameters2)
    {
        std::cout << "Error - Invalid set of bloom filter parameters!" << std::endl;
        return 1;
    }

    parameters2.compute_optimal_parameters();

    bloom_filter filter21(parameters2);

    // Insert some strings
    for (std::size_t i = 0; i < 10000; ++i)
    {
        //cout << i << ": " << k1[i] << endl;
        filter21.insert(k1[i]);
    }

    // Query the existence of strings
    fn = 0;
    for (std::size_t i = 0; i < 5000; ++i)
    {
        if (!filter21.contains(kprime1[i]))
        {
            fn += 1; 
            //std::cout << "BF11 contains" << " " << i << ": " << kprime1[i] << std::endl;
        }
    }
    std::cout << "BF21 contains " << fn << " false negatives" << endl;

    fp = 0;
    for (std::size_t i = 5000; i < 10000; ++i)
    {
        if (filter21.contains(kprime1[i]))
        {
            fp += 1; 
            //std::cout << "BF11 contains" << " " << i << ": " << kprime1[i] << std::endl;
        }
    }
    std::cout << "BF21 contains " << fp << " false positives" << endl;


    bloom_filter filter22(parameters2);

    // Insert some strings
    for (std::size_t i = 0; i < 10000; ++i)
    {
        //cout << i << ": " << k2[i] << endl;
        filter22.insert(k2[i]);
    }

    // Query the existence of strings
    fn = 0;
    for (std::size_t i = 7500; i < 10000; ++i)
    {
        if (!filter22.contains(kprime2[i]))
        {
            fn += 1; 
            //std::cout << "BF11 contains" << " " << i << ": " << kprime1[i] << std::endl;
        }
    }
    std::cout << "BF22 contains " << fn << " false negatives" << endl;

    fp = 0;
    for (std::size_t i = 0; i < 7500; ++i)
    {
        if (filter22.contains(kprime2[i]))
        {
            fp += 1; 
            //std::cout << "BF11 contains" << " " << i << ": " << kprime1[i] << std::endl;
        }
    }
    std::cout << "BF22 contains " << fp << " false positives" << endl;


    bloom_filter filter23(parameters2);

    // Insert some strings
    for (std::size_t i = 0; i < 10000; ++i)
    {
        //cout << i << ": " << k3[i] << endl;
        filter23.insert(k3[i]);
    }

    // Query the existence of strings
    fn = 0;
    for (std::size_t i = 0; i < 4000; ++i)
    {
        if (!filter23.contains(kprime3[i]))
        {
            fn += 1; 
            //std::cout << "BF11 contains" << " " << i << ": " << kprime1[i] << std::endl;
        }
    }
    std::cout << "BF23 contains " << fn << " false negatives" << endl;

    fp = 0;
    for (std::size_t i = 4000; i < 10000; ++i)
    {
        if (filter23.contains(kprime3[i]))
        {
            fp += 1; 
            //std::cout << "BF11 contains" << " " << i << ": " << kprime1[i] << std::endl;
        }
    }
    std::cout << "BF23 contains " << fp << " false positives" << endl;


    bloom_parameters parameters3;

    // How many elements roughly do we expect to insert?
    parameters3.projected_element_count = 10000;

    // Maximum tolerable false positive probability? (0,1)
    parameters3.false_positive_probability = 0.0009765625; // 1 in 10000

    if (!parameters3)
    {
        std::cout << "Error - Invalid set of bloom filter parameters!" << std::endl;
        return 1;
    }

    parameters3.compute_optimal_parameters();

    bloom_filter filter31(parameters3);

    // Insert some strings
    for (std::size_t i = 0; i < 10000; ++i)
    {
        //cout << i << ": " << k1[i] << endl;
        filter31.insert(k1[i]);
    }

    // Query the existence of strings
    fn = 0;
    for (std::size_t i = 0; i < 5000; ++i)
    {
        if (!filter31.contains(kprime1[i]))
        {
            fn += 1; 
            //std::cout << "BF11 contains" << " " << i << ": " << kprime1[i] << std::endl;
        }
    }
    std::cout << "BF31 contains " << fn << " false negatives" << endl;

    fp = 0;
    for (std::size_t i = 5000; i < 10000; ++i)
    {
        if (filter31.contains(kprime1[i]))
        {
            fp += 1; 
            //std::cout << "BF11 contains" << " " << i << ": " << kprime1[i] << std::endl;
        }
    }
    std::cout << "BF31 contains " << fp << " false positives" << endl;

    bloom_filter filter32(parameters3);

    // Insert some strings
    for (std::size_t i = 0; i < 10000; ++i)
    {
        //cout << i << ": " << k2[i] << endl;
        filter32.insert(k2[i]);
    }

    // Query the existence of strings
    fn = 0;
    for (std::size_t i = 7500; i < 10000; ++i)
    {
        if (!filter32.contains(kprime2[i]))
        {
            fn += 1; 
            //std::cout << "BF11 contains" << " " << i << ": " << kprime1[i] << std::endl;
        }
    }
    std::cout << "BF32 contains " << fn << " false negatives" << endl;

    fp = 0;
    for (std::size_t i = 0; i < 7500; ++i)
    {
        if (filter32.contains(kprime2[i]))
        {
            fp += 1; 
            //std::cout << "BF11 contains" << " " << i << ": " << kprime1[i] << std::endl;
        }
    }
    std::cout << "BF32 contains " << fp << " false positives" << endl;


    bloom_filter filter33(parameters2);

    // Insert some strings
    for (std::size_t i = 0; i < 10000; ++i)
    {
        //cout << i << ": " << k3[i] << endl;
        filter33.insert(k3[i]);
    }

    // Query the existence of strings
    fn = 0;
    for (std::size_t i = 0; i < 4000; ++i)
    {
        if (!filter33.contains(kprime3[i]))
        {
            fn += 1; 
            //std::cout << "BF11 contains" << " " << i << ": " << kprime1[i] << std::endl;
        }
    }
    std::cout << "BF33 contains " << fn << " false negatives" << endl;

    fp = 0;
    for (std::size_t i = 4000; i < 10000; ++i)
    {
        if (filter33.contains(kprime3[i]))
        {
            fp += 1; 
            //std::cout << "BF11 contains" << " " << i << ": " << kprime1[i] << std::endl;
        }
    }
    std::cout << "BF33 contains " << fp << " false positives" << endl;



    u_int64_t nelem = 10000;
	uint nthreads = 1;
    boophf_t * bphf1 = NULL;
    boophf_t * bphf2 = NULL;
    boophf_t * bphf3 = NULL;
	
	// mphf takes as input a c++ range. A std::vector is already a c++ range
	
	double gammaFactor = 2.0; // lowest bit/elem is achieved with gamma=1, higher values lead to larger mphf but faster construction/query
	// gamma = 2 is a good tradeoff (leads to approx 3.7 bits/key )

    bphf1 = new boomphf::mphf<std::string,Custom_string_Hasher>(nelem,k1,nthreads,gammaFactor);

    printf("boophf  bits/elem : %f\n",(float) (bphf1->totalBitSize())/nelem);
    fn = 0; 
    for (u_int64_t i = 0; i < 5000; i++){
		uint64_t  idx = bphf1->lookup(kprime1[i]);
        //cout << idx << endl;
        if (idx == ULLONG_MAX){
            fp += 1; 
        }
	}
    std::cout << "MPHF1 contains " << fn << " false negatives" << endl;
    fp = 0; 
    for (u_int64_t i = 5000; i < 10000; i++){
		uint64_t  idx = bphf1->lookup(kprime1[i]);
        //cout << idx << endl;
        if (idx != ULLONG_MAX){
            fp += 1; 
        }
	}
    std::cout << "MPHF1 contains " << fp << " false positives" << endl;

    bphf2 = new boomphf::mphf<std::string,Custom_string_Hasher>(nelem,k2,nthreads,gammaFactor);

    printf("boophf  bits/elem : %f\n",(float) (bphf2->totalBitSize())/nelem);
    fn = 0; 
    for (u_int64_t i = 7500; i < 10000; i++){
		uint64_t  idx = bphf2->lookup(kprime2[i]);
        //cout << idx << endl;
        if (idx == ULLONG_MAX){
            fn += 1; 
        }
	}
    std::cout << "MPHF2 contains " << fn << " false negatives" << endl;
    fp = 0; 
    for (u_int64_t i = 0; i < 7500; i++){
		uint64_t  idx = bphf2->lookup(kprime2[i]);
        //cout << idx << endl;
        if (idx != ULLONG_MAX){
            fp += 1; 
        }
	}
    std::cout << "MPHF2 contains " << fp << " false positives" << endl;

    bphf3 = new boomphf::mphf<std::string,Custom_string_Hasher>(nelem,k3,nthreads,gammaFactor);

    printf("boophf  bits/elem : %f\n",(float) (bphf3->totalBitSize())/nelem);
    fn = 0;
    for (u_int64_t i = 0; i < 4000; i++){
		uint64_t  idx = bphf3->lookup(kprime3[i]);
        //cout << idx << endl;
        if (idx == ULLONG_MAX){
            fn += 1; 
        }
	}
    std::cout << "MPHF3 contains " << fn << " false negatives" << endl;
    fp = 0; 
    for (u_int64_t i = 4000; i < 10000; i++){
		uint64_t  idx = bphf3->lookup(kprime3[i]);
        //cout << idx << endl;
        if (idx != ULLONG_MAX){
            fp += 1; 
        }
	}
    std::cout << "MPHF3 contains " << fp << " false positives" << endl;

    // std::cout << std::hash<std::string>{}(k1[0]) << '\n';
    // cout << (std::hash<std::string>{}(k1[0]) & 127) << endl;
    // std::cout << std::hash<std::string>{}(k1[1]) << '\n';

    sdsl::int_vector<> f11(10000, 0, 7);
    for (u_int64_t i = 0; i < 10000; i++){
		uint64_t  idx = bphf1->lookup(k1[i]);
        f11[idx] = (std::hash<std::string>{}(k1[i]) & 127);
	}

    //query
    fn = 0; 
    for (u_int64_t i = 0; i < 5000; i++){
		uint64_t  idx = bphf1->lookup(kprime1[i]);
        if (idx == ULLONG_MAX || f11[idx] != (std::hash<std::string>{}(kprime1[i]) & 127)){
            fn += 1; 
        }
	}
    std::cout << "AMQ11 contains " << fn << " false negatives" << endl;
    fp = 0; 
    for (u_int64_t i = 5000; i < 10000; i++){
		uint64_t  idx = bphf1->lookup(kprime1[i]);
        if (idx != ULLONG_MAX && f11[idx] == (std::hash<std::string>{}(kprime1[i]) & 127)){
            fp += 1; 
        }
	}
    std::cout << "AMQ11 contains " << fp << " false positives" << endl;


    sdsl::int_vector<> f12(10000, 0, 7);
    for (u_int64_t i = 0; i < 10000; i++){
		uint64_t  idx = bphf2->lookup(k2[i]);
        f12[idx] = (std::hash<std::string>{}(k2[i]) & 127);
	}

    //query
    fn = 0; 
    for (u_int64_t i = 7500; i < 10000; i++){
		uint64_t  idx = bphf2->lookup(kprime2[i]);
        if (idx == ULLONG_MAX || f12[idx] != (std::hash<std::string>{}(kprime2[i]) & 127)){
            fn += 1; 
        }
	}
    std::cout << "AMQ12 contains " << fn << " false negatives" << endl;
    fp = 0; 
    for (u_int64_t i = 0; i < 7500; i++){
		uint64_t  idx = bphf2->lookup(kprime2[i]);
        if (idx != ULLONG_MAX && f12[idx] == (std::hash<std::string>{}(kprime2[i]) & 127)){
            fp += 1; 
        }
	}
    std::cout << "AMQ12 contains " << fp << " false positives" << endl;

    sdsl::int_vector<> f13(10000, 0, 7);
    for (u_int64_t i = 0; i < 10000; i++){
		uint64_t  idx = bphf3->lookup(k3[i]);
        f13[idx] = (std::hash<std::string>{}(k3[i]) & 127);
	}

    //query
    fn = 0; 
    for (u_int64_t i = 0; i < 4000; i++){
		uint64_t  idx = bphf3->lookup(kprime3[i]);
        if (idx == ULLONG_MAX || f13[idx] != (std::hash<std::string>{}(kprime3[i]) & 127)){
            fn += 1; 
        }
	}
    std::cout << "AMQ13 contains " << fn << " false negatives" << endl;
    fp = 0; 
    for (u_int64_t i = 4000; i < 10000; i++){
		uint64_t  idx = bphf3->lookup(kprime3[i]);
        if (idx != ULLONG_MAX && f13[idx] == (std::hash<std::string>{}(kprime3[i]) & 127)){
            fp += 1; 
        }
	}
    std::cout << "AMQ13 contains " << fp << " false positives" << endl;





    // std::cout << std::hash<std::string>{}(k1[1]) << '\n';
    // cout << (std::hash<std::string>{}(k1[1]) & 255) << endl;

    sdsl::int_vector<> f21(10000, 0, 8);
    for (u_int64_t i = 0; i < 10000; i++){
		uint64_t  idx = bphf1->lookup(k1[i]);
        f21[idx] = (std::hash<std::string>{}(k1[i]) & 255);
	}

    //query
    fn = 0; 
    for (u_int64_t i = 0; i < 5000; i++){
		uint64_t  idx = bphf1->lookup(kprime1[i]);
        if (idx == ULLONG_MAX || f21[idx] != (std::hash<std::string>{}(kprime1[i]) & 255)){
            fn += 1; 
        }
	}
    std::cout << "AMQ21 contains " << fn << " false negatives" << endl;
    fp = 0; 
    for (u_int64_t i = 5000; i < 10000; i++){
		uint64_t  idx = bphf1->lookup(kprime1[i]);
        if (idx != ULLONG_MAX && f21[idx] == (std::hash<std::string>{}(kprime1[i]) & 255)){
            fp += 1; 
        }
	}
    std::cout << "AMQ21 contains " << fp << " false positives" << endl;


    sdsl::int_vector<> f22(10000, 0, 8);
    for (u_int64_t i = 0; i < 10000; i++){
		uint64_t  idx = bphf2->lookup(k2[i]);
        f22[idx] = (std::hash<std::string>{}(k2[i]) & 255);
	}

    //query
    fn = 0; 
    for (u_int64_t i = 7500; i < 10000; i++){
		uint64_t  idx = bphf2->lookup(kprime2[i]);
        if (idx == ULLONG_MAX || f22[idx] != (std::hash<std::string>{}(kprime2[i]) & 255)){
            fn += 1; 
        }
	}
    std::cout << "AMQ22 contains " << fn << " false negatives" << endl;
    fp = 0; 
    for (u_int64_t i = 0; i < 7500; i++){
		uint64_t  idx = bphf2->lookup(kprime2[i]);
        if (idx != ULLONG_MAX && f22[idx] == (std::hash<std::string>{}(kprime2[i]) & 255)){
            fp += 1; 
        }
	}
    std::cout << "AMQ22 contains " << fp << " false positives" << endl;

    sdsl::int_vector<> f23(10000, 0, 8);
    for (u_int64_t i = 0; i < 10000; i++){
		uint64_t  idx = bphf3->lookup(k3[i]);
        f23[idx] = (std::hash<std::string>{}(k3[i]) & 255);
	}

    //query
    fn = 0; 
    for (u_int64_t i = 0; i < 4000; i++){
		uint64_t  idx = bphf3->lookup(kprime3[i]);
        if (idx == ULLONG_MAX || f23[idx] != (std::hash<std::string>{}(kprime3[i]) & 255)){
            fn += 1; 
        }
	}
    std::cout << "AMQ23 contains " << fn << " false negatives" << endl;
    fp = 0; 
    for (u_int64_t i = 4000; i < 10000; i++){
		uint64_t  idx = bphf3->lookup(kprime3[i]);
        if (idx != ULLONG_MAX && f23[idx] == (std::hash<std::string>{}(kprime3[i]) & 255)){
            fp += 1; 
        }
	}
    std::cout << "AMQ23 contains " << fp << " false positives" << endl;








    // std::cout << std::hash<std::string>{}(k1[3]) << '\n';
    // cout << (std::hash<std::string>{}(k1[3]) & 1023) << endl;

    sdsl::int_vector<> f31(10000, 0, 10);
    for (u_int64_t i = 0; i < 10000; i++){
		uint64_t  idx = bphf1->lookup(k1[i]);
        f31[idx] = (std::hash<std::string>{}(k1[i]) & 1023);
	}

    //query
    fn = 0; 
    for (u_int64_t i = 0; i < 5000; i++){
		uint64_t  idx = bphf1->lookup(kprime1[i]);
        if (idx == ULLONG_MAX || f31[idx] != (std::hash<std::string>{}(kprime1[i]) & 1023)){
            fn += 1; 
        }
	}
    std::cout << "AMQ31 contains " << fn << " false negatives" << endl;
    fp = 0; 
    for (u_int64_t i = 5000; i < 10000; i++){
		uint64_t  idx = bphf1->lookup(kprime1[i]);
        if (idx != ULLONG_MAX && f31[idx] == (std::hash<std::string>{}(kprime1[i]) & 1023)){
            fp += 1; 
        }
	}
    std::cout << "AMQ31 contains " << fp << " false positives" << endl;


    sdsl::int_vector<> f32(10000, 0, 10);
    for (u_int64_t i = 0; i < 10000; i++){
		uint64_t  idx = bphf2->lookup(k2[i]);
        f32[idx] = (std::hash<std::string>{}(k2[i]) & 1023);
	}

    //query
    fn = 0; 
    for (u_int64_t i = 7500; i < 10000; i++){
		uint64_t  idx = bphf2->lookup(kprime2[i]);
        if (idx == ULLONG_MAX || f32[idx] != (std::hash<std::string>{}(kprime2[i]) & 1023)){
            fn += 1; 
        }
	}
    std::cout << "AMQ32 contains " << fn << " false negatives" << endl;
    fp = 0; 
    for (u_int64_t i = 0; i < 7500; i++){
		uint64_t  idx = bphf2->lookup(kprime2[i]);
        if (idx != ULLONG_MAX && f32[idx] == (std::hash<std::string>{}(kprime2[i]) & 1023)){
            fp += 1; 
        }
	}
    std::cout << "AMQ32 contains " << fp << " false positives" << endl;

    sdsl::int_vector<> f33(10000, 0, 10);
    for (u_int64_t i = 0; i < 10000; i++){
		uint64_t  idx = bphf3->lookup(k3[i]);
        f33[idx] = (std::hash<std::string>{}(k3[i]) & 1023);
	}

    //query
    fn = 0; 
    for (u_int64_t i = 0; i < 4000; i++){
		uint64_t  idx = bphf3->lookup(kprime3[i]);
        if (idx == ULLONG_MAX || f33[idx] != (std::hash<std::string>{}(kprime3[i]) & 1023)){
            fn += 1; 
        }
	}
    std::cout << "AMQ33 contains " << fn << " false negatives" << endl;
    fp = 0; 
    for (u_int64_t i = 4000; i < 10000; i++){
		uint64_t  idx = bphf3->lookup(kprime3[i]);
        if (idx != ULLONG_MAX && f33[idx] == (std::hash<std::string>{}(kprime3[i]) & 1023)){
            fp += 1; 
        }
	}
    std::cout << "AMQ33 contains " << fp << " false positives" << endl;


    return 0;
}