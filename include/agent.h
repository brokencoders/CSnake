#ifndef AGENT_H
#define AGENT_H

#include <torch/torch.h>
#include <torch/script.h> 

#include "model.h"
#include <deque>

struct Experience 
{
    torch::Tensor state;
    torch::Tensor next_state;
    int action;
    int reward;
};


struct Agent 
{
public:
    Agent(int size);
    int get_action(std::vector<int> state);
    void update();
    void train(std::vector<int> state, std::vector<int> new_state, int action, int reward);
public:
    Model model;
    Model model_update;

    int buffer_size;
    std::deque<Experience> buffer;
    std::shared_ptr<torch::optim::Adam> optimizer;
};

#endif