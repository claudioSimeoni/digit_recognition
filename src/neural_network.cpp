#include <vector>
#include <cmath>
#include <memory>
using namespace std;

#include "../include/matrix.hpp"


struct Layer{
    int in, out;
    vector<float> a;
    virtual ~Layer() = default;
    virtual void forw(vector<float>&) = 0;
    virtual void backw(vector<float>&) = 0;
};

struct LinearLayer : Layer{
    int in, out;
    vector<float> a;
    vector<float> w;
    vector<float> b;
    
    LinearLayer(int in, int out) : in(in), out(out){
        w.resize(in * out);
        a.resize(in);
        b.resize(out);
    }
    
    void forw(vector<float>& f) override{
        assert(f.size() == out);
        for(int i=0; i<out; i++){
            for(int j=0; j<in; j++){
                f[i] += w[i * out + j] * a[j];
            }
        }
    }

    void backw(vector<float>& jacob) override{
        assert(jacob.size() == in * out);
        jacob = w;
    }
};

struct SigmoidLayer : Layer{
    int in, out;
    vector<float> a;

    SigmoidLayer(int in, int out) : in(in), out(out) {}

    void forw(vector<float>& f) override{
        assert(f.size() == out);
        for(int i=0; i<out; i++){
            f[i] = 1 / (1 + exp(-a[i]));
        }
    }

    void backw(vector<int>& jacob){
        assert(jacob.size() == in * out); // must be initialized with all zeroes
        for(int i=0; i<in; i++){
            jacob[i * in + i] = log(a[i] / 1 - a[i]);
        }
    }
};

struct NeuralNetwork{
    int ldim;
    vector<float> al;
    vector<unique_ptr<Layer>> l;

    NeuralNetwork(vector<unique_ptr<Layer>>& l) : l(l), ldim(l.size()){
        al.resize(l[ldim - 1]->out);
        // check of sizes compatibility
    }

    void traverse(vector<float>& data, vector<float>& y){
        assert(data.size() == l[0]->in && y.size() == l[ldim - 1]->out);

        l[0]->a = data;
        for(int i=1; i<ldim; i++){
            l[i - 1]->forw(l[i]->a);
        }
        l[ldim - 1]->forw(al);

        backprop(y);
    }

    void backprop(vector<float>& y){
        int col = l[ldim - 1]->out;
        int row = 1;

        vector<float> jacob;
        for(int i=0; i<col; i++) jacob[i] = 2 * (al[i] - y[i]);

        for(int i=ldim - 1; i>=0; i--){
            if(typeid(l[i]) == typeid(SigmoidLayer)){
                for(int j=0; j<row; j++){
                    for(int k=0; k<col; k++){
                        jacob[i * row + j] *= l[i]->
                    }
                }
            }
        }
    }
};




struct NeuralNetwork{   

    int l; // number of layers
    vector<int> lsize; // layer sizes 1-indexed
    vector<vector<int>> a; // layer activations
    vector<Matrix> w; // weights
    vector<vector<int>> b; // biases


    NeuralNetwork(){
        l = 4;
        lsize = {784, 16, 16, 10};

        a.resize(l);
        b.resize(l);
        for(int i=0; i<l; i++){
            a[i].resize(lsize[i]);
            if(i > 0) b[i].resize(lsize[i]);
        }

        w.resize(l);
        for(int i=1; i<l; i++){
            w[i] = Matrix(lsize[i], lsize[i - 1]);
        }
    }



};