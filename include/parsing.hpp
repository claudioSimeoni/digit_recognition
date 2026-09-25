#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

extern int rows, cols;
extern vector<unsigned char> lbls; 
extern vector<vector<unsigned char>> imgs;

void little_endian_conversion(unsigned int& n);

void parse_images();

void parse_labels();

void print_number(vector<unsigned char>& img, int x, int y);

// int main(){

//     parse_images();
//     parse_labels();

//     for(int i=0; i<20; i++){
//         cout << int(lbls[i]) << "\n";
//         print_number(imgs[i], x, y);
//     }
// }