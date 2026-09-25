#include <fstream>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

#include "../include/parsing.hpp"

int rows, cols;
vector<unsigned char> lbls; 
vector<vector<unsigned char>> imgs;

void little_endian_conversion(unsigned int& n){
    n = (n >> 24) | ((n << 8) >> 16) | ((n << 16) >> 8) | (n << 24);
}

void parse_images(){
    ifstream imgs_stream("../data/train-images-idx3-ubyte", ios::binary);
    int magic_number;

    imgs_stream.read((char*)&magic_number, sizeof(magic_number));
    int nd = magic_number >> 24;
    vector<unsigned int> dim(nd);

    for(int i=0; i<nd; i++){
        imgs_stream.read((char*)&dim[i], sizeof(unsigned int));
        little_endian_conversion(dim[i]);
    }

    rows = dim[1];
    cols = dim[2];
    imgs.resize(dim[0], vector<unsigned char>(dim[1] * dim[2]));
    for(int i=0; i<dim[0]; i++){
        for(int j=0; j<dim[1]*dim[2]; j++){
            imgs_stream.read((char*)&imgs[i][j], sizeof(char));
        }
    }
}

void parse_labels(){
    ifstream lbls_stream("../data/train-labels-idx1-ubyte", ios::binary);
    int magic_number;

    lbls_stream.read((char*)&magic_number, sizeof(magic_number));
    int nd = magic_number >> 24;
    vector<unsigned int> dim(nd);

    for(int i=0; i<nd; i++){
        lbls_stream.read((char*)&dim[i], sizeof(unsigned int));
        little_endian_conversion(dim[i]);
    }

    lbls.resize(dim[0]);
    for(int i=0; i<dim[0]; i++){
        lbls_stream.read((char*)&lbls[i], sizeof(char));
    }
}

void print_number(vector<unsigned char>& img, int rows, int cols){
    vector<char> br = {' ', '.', '0', '#'};
    for(int i=0; i<rows; i++){
        for(int j=0; j<cols; j++){
            cout << br[img[rows * i + j] / 64];
        }
        cout << "\n";
    }
}

// int main(){

//     parse_images();
//     parse_labels();

//     for(int i=0; i<20; i++){
//         cout << int(lbls[i]) << "\n";
//         print_number(imgs[i], x, y);
//     }
// }