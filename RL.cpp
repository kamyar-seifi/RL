//
//  RL.cpp
//  
//
//  Created by kamyar Seifi.
// Copyright © 2018 kamyar Seifi. All rights reserved.
//

#include "RL.h"
#include "math.h"
#include <iostream>
Encode::RL::RL(vector<unsigned char>& input):dta(input){
}

int Encode::RL::Compress(){
 
    auto no_rep_seq = ReturnMap(); //Init map

    if(no_rep_seq->empty())
        return -2;
    
    int new_size = ceil(no_rep_seq->size()*1.5 + 0.5) ; //size approx
    
    if(new_size>=dta.size())
        return -1;
    else{
        dta.clear();
        for(auto i : (*no_rep_seq))
            dta.push_back(i.first);  //write the non-repeated data from the map
        
        auto temp_first = no_rep_seq->front().second;  //first load
        temp_first = 0x80 | temp_first;
        dta.push_back((unsigned char)temp_first);
        no_rep_seq->erase(no_rep_seq->begin());
        
        while(!no_rep_seq->empty()){   //loading the rest
            auto temp = no_rep_seq->front().second;
            no_rep_seq->erase(no_rep_seq->begin());
            temp = temp << 4;
            if(!no_rep_seq->empty()){
                temp = temp | (no_rep_seq->front().second);
                no_rep_seq->erase(no_rep_seq->begin());
            }
            dta.push_back(temp);
        }
    }
    delete no_rep_seq;
    return (int)dta.size();
    }


bool Encode::RL::IsCompressed(){
    int header=0;
    for(;( (dta[header] >> 4 ) < 8) && header<dta.size(); header++);
    if(header==dta.size())
        return false;
    else
        return true;
}

auto Encode::RL::ReturnMap(void)->vector<pair<unsigned char, unsigned char>>*{
    
    auto map = new vector<pair<unsigned char, unsigned char>>;
    (*map).clear();
    
    if(!dta.empty()){
        for(int i=0;i<(int)dta.size() ; i++){
            int j=i;
            int count=1;
            for(;dta[j]==dta[j+1]; j++);
        
            count += j-i;
            i=j;
        
            if(count<=15){
                pair<unsigned char, unsigned char> temp = {dta[j], count};
                (*map).push_back(temp);
            }
            else
                (*map).clear();
        }
    }
        return map;
}


int Encode::RL::Decompress(){
    int header=0;

    if(!IsCompressed())
        return -1;
    else
        for(;( (dta[header] >> 4 ) < 8) && header<dta.size(); header++);
    
    int read_header=1;  //helper flag for reading the count bytes;
    vector<pair<unsigned char, unsigned char>> map;
    
    for(int i=0; (dta[i] >> 4) < 0x08; i++)
    {
        unsigned char count = dta[header];
        if(read_header==0){  //reaading the second digit of the count byte
            count = count & 0xf0;
            count = count >> 4;
            read_header=1;
        }else{  //reading the frist digit of the count byte
            count = count & 0x0f;
            read_header=0;
            header++;
        }
        pair<unsigned char, unsigned char> temp = {dta[i], count};
        map.push_back(temp);
    }
    dta.clear();
    
    while(!map.empty()){
        auto a = map.front();
        for(unsigned char i=0; i<a.second; i++)
            dta.push_back(a.first);
        map.erase(map.begin());
    }
    return (int) dta.size();
}


void Encode::RL::Disp(){
    cout << endl;
if(dta.empty())
    cout << "Error No data found" << endl;
    else if(IsCompressed()){
        cout << "Stat: RL-Compressed" << endl;
        for(auto i: dta)
            printf(" %x ", i);
    }else{
        cout << "Stat: Not-Compressed " << endl;
        for(auto i: dta)
            printf(" %x ", i);
    }
}
