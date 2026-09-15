#include <fstream>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

struct data_elem{
    unsigned char label;
    unsigned int x, y;
    vector<unsigned char> img;

    data_elem(unsigned int x, unsigned int y) : x(x), y(y) {
        img.resize(x * y);
    }
};

vector<data_elem> dataset; 

void little_endian_conversion(unsigned int& n){
    n = (n >> 24) | ((n << 8) >> 16) | ((n << 16) >> 8) | (n << 24);
}

void parse_images(){
    ifstream ifimg("../data/train-images-idx3-ubyte", ios::binary);
    int mnumber;

    ifimg.read((char*)&mnumber, sizeof(mnumber));
    int ndim = mnumber >> 24;
    vector<unsigned int> dim(ndim);

    for(int i=0; i<ndim; i++){
        ifimg.read((char*)&dim[i], sizeof(unsigned int));
        little_endian_conversion(dim[i]);
    }

    dataset.resize(dim[0], {dim[1], dim[2]});
    for(int i=0; i<dim[0]; i++){
        for(int j=0; j<dim[1]*dim[2]; j++){
            ifimg.read((char*)&dataset[i].img[j], sizeof(char));
        }
    }
}

void parse_labels(){
    ifstream ifimg("../data/train-labels-idx1-ubyte", ios::binary);
    int mnumber;

    ifimg.read((char*)&mnumber, sizeof(mnumber));
    int ndim = mnumber >> 24;
    vector<unsigned int> dim(ndim);

    for(int i=0; i<ndim; i++){
        ifimg.read((char*)&dim[i], sizeof(unsigned int));
        little_endian_conversion(dim[i]);
    }

    for(int i=0; i<dim[0]; i++){
        ifimg.read((char*)&dataset[i].label, sizeof(char));
    }
}

void print_number(vector<unsigned char>& img, int x, int y){
    vector<char> br = {' ', '.', '0', '#'};
    for(int i=0; i<x; i++){
        for(int j=0; j<y; j++){
            if(img[x * i + j] < 64) cout << ' ';
            else if(img[x * i + j] < 128) cout << '.';
            else if(img[x * i + j] < 192) cout << 'o';
            else cout << '#';
        }
        cout << "\n";
    }
}

int main(){

    parse_images();
    parse_labels();

    int x = dataset[0].x;
    int y = dataset[0].y;
    for(int i=0; i<20; i++){
        cout << int(dataset[i].label) << "\n";
        print_number(dataset[i].img, dataset[i].x, dataset[i].y);
    }
}