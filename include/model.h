#ifndef MODEL_H
#define MODEL_H
#define TORCH_SHOW_CPP_STACKTRACES 1
#include <torch/torch.h>
#include <torch/script.h> 

struct Model : torch::nn::Module
{
    public:
        Model(int size);
        torch::Tensor forward(torch::Tensor x);
    
    public:
        torch::nn::Linear l1{nullptr}, l2{nullptr}, l3{nullptr};
};

#endif
