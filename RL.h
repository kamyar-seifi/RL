//
//  RL.h
//  working
//
//  Created by kamyar Seifi
//  Copyright © 2018 kamyar Seifi. All rights reserved.
//

#ifndef RL_h
#define RL_h
#include <vector>
using namespace std;

namespace Encode { class RL{
    
private:
    vector<unsigned char> dta;
    RL();
    
public:
    //@RL constructor: data loaded and initialized
    //@input: the first argument loading the input into the dta;
    RL(vector<unsigned char>& input);
    
    //@Compressed: the Run-lenght compressor
    //@return: return the size of the data
    int Compress(void);
    
    //@Iscompressed: predicator for checking the compressed data
    bool IsCompressed(void);
    //@Decompress: decompressing the data
    int Decompress(void);
    
    void Disp(void); 
    
    //@ReturnMap: the map of the pair of the count and element
    //@return: allocate the map dynamically, return a pointer to it. user is responsible to free the allocated map
    auto ReturnMap(void)->vector<pair<unsigned char, unsigned char>>*;
};
};

#endif /* RL_h */
