#include <vector>
#include <cmath>
#include <memory>
#include <iostream>
#include <random>
using namespace std;

#include "../include/matrix.hpp"
#include "../include/parsing.hpp"

struct Layer{
    int in, out;
    vector<float> a;
    Layer(int in, int out) : in(in), out(out) {};
    virtual ~Layer() = default;
    virtual void forw(vector<float>&) = 0;
    virtual void backw(vector<float>&) = 0;
};

struct LinearLayer : Layer{
    float h;
    vector<float> w;
    vector<float> b;
    
    LinearLayer(int in, int out, float h) : Layer(in, out), h(h){
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<float> dist(-0.1f, 0.1f);

        w.resize(in * out);
        b.resize(out);
        a.resize(in);

        for (float& x : w) x = dist(gen);
        for (float& x : b) x = dist(gen);
    }
    
    void forw(vector<float>& f) override{
        fill(f.begin(), f.end(), 0.0f);
        assert(f.size() == out);
        for(int i=0; i<out; i++){
            for(int j=0; j<in; j++){
                f[i] += w[i * out + j] * a[j];
            }
            f[i] += b[i];
        }
    }

    void backw(vector<float>& jacob) override{
        assert(jacob.size() == out);

        vector<float> res(in);
        for(int i=0; i<out; i++){
            for(int j=0; j<in; j++){
                res[j] += w[i * out + j] * jacob[i];
            }
        }

        for(int i=0; i<out; i++){
            for(int j=0; j<in; j++){
                w[i * out + j] -= h * (jacob[i] * a[j]);
            }
            b[i] -= h * (jacob[i]);
        }

        jacob = res;
    }
};

struct SigmoidLayer : Layer{

    SigmoidLayer(int sz) : Layer(sz, sz) {
        a.resize(in);
    }

    void forw(vector<float>& f) override{
        assert(f.size() == out);
        for(int i=0; i<out; i++){
            f[i] = 1 / (1 + exp(-a[i]));
        }
    }

    void backw(vector<float>& jacob) override{
        assert(jacob.size() == out);
        for(int i=0; i<in; i++){
            jacob[i] *= exp(-a[i]) / ((1 + exp(-a[i])) * (1 + exp(-a[i])));
        }
    }
};

struct NeuralNetwork{
    int ldim;
    vector<float> al;
    vector<Layer*> l;

    NeuralNetwork(vector<Layer*>& l) : l(l), ldim(l.size()){
        al.resize(l[ldim - 1]->out);
        // check of sizes compatibility
    }

    void traverse(vector<float>& data){
        assert(data.size() == l[0]->in);

        l[0]->a = data;
        for(int i=1; i<ldim; i++){
            l[i - 1]->forw(l[i]->a);
        }
        l[ldim - 1]->forw(al);
    }

    void backprop(vector<float>& y){
        vector<float> jacob(al.size());
        for(int i=0; i<jacob.size(); i++) jacob[i] = 2 * (al[i] - y[i]);

        for(int i=ldim - 1; i>=0; i--){
            l[i]->backw(jacob);
            // if(typeid(l[i]) == typeid(SigmoidLayer)){
           
            // }
        }
    }

    void recognize(vector<float>& data){
        assert(data.size() == l[0]->in);

        traverse(data);

        cout << "\n";
        for(int i=0; i<al.size(); i++) cout << i << ": " << al[i] << " ";
        cout << "\n";
    }

    void train(vector<float>& data, vector<float>& y){
        assert(data.size() == l[0]->in && y.size() == l[ldim - 1]->out);
        traverse(data);
        backprop(y);
    }
};

int main(){
    vector<Layer*> lrs = {  new LinearLayer(784, 16, 0.1),
                            new SigmoidLayer(16),
                            new LinearLayer(16, 16, 0.1), 
                            new SigmoidLayer(16),
                            new LinearLayer(16, 10, 0.1),
                            new SigmoidLayer(10),
                          };

    cout << lrs[0]->in << "\n";

    NeuralNetwork nn(lrs);
    
    parse_images();
    parse_labels();

    for(int i=0; i<60000; i++){
        vector<float> data(imgs[i].size());
        for(int j=0; j<data.size(); j++) data[j] = static_cast<float>(imgs[i][j]) / 255.0f;

        vector<float> y(10);
        y[lbls[i]] = 1;

        nn.train(data, y);
    }

    for(int i=0; i<10; i++){
        vector<float> data(imgs[i].size());
        for(int j=0; j<data.size(); j++) data[j] = static_cast<float>(imgs[i][j]) / 255.0f;

        vector<float> y(10);
        y[lbls[i]] = 1;

        print_number(imgs[i], rows, cols);
        nn.recognize(data);
    }
}