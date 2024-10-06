#include "model.h"

Model::Model(int size)
{
    l1 = register_module("l1", torch::nn::Linear(size, 128));
    l2 = register_module("l2", torch::nn::Linear(128, 64));
    l3 = register_module("l3", torch::nn::Linear(64, 3));
}

torch::Tensor Model::forward(torch::Tensor x) 
{
    x = torch::relu(l1->forward(x));
    x = torch::relu(l2->forward(x));
    x = l3->forward(x);
    return x;
}