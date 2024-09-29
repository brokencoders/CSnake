#include "model.h"

Model::Model(int size)
{
    l1 = register_module("l1", torch::nn::Linear(size, 100));
    l2 = register_module("l2", torch::nn::Linear(100, 3));
}

torch::Tensor Model::forward(torch::Tensor x) 
{
    x = torch::relu(l1->forward(x));
    x = l2->forward(x);
    return x; 
}