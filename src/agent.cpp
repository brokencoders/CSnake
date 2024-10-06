#include "agent.h"
#include <random> 

void printTensorDimensions(const torch::Tensor& tensor) {
    // Get the sizes of the tensor
    auto sizes = tensor.sizes();

    // Print the dimensions
    std::cout << "Tensor dimensions: ";
    for (size_t i = 0; i < sizes.size(); ++i) {
        std::cout << sizes[i];
        if (i < sizes.size() - 1) {
            std::cout << " x ";  // Format for better readability
        }
    }
    std::cout << std::endl;
}

Agent::Agent(int size)
	:policy(size), target(size), buffer_size(10000)
{
	optimizer = std::make_shared<torch::optim::Adam>(policy.parameters(), 0.001);
}

int Agent::get_action(std::vector<int> state)
{
    static double epsilon_min = 0.01;
    static double epsilon_decay = 0.99999;
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(0.0, 1.0);
	
    torch::Tensor t_state = torch::tensor(state, torch::kFloat32);
    torch::Tensor result = target.forward(t_state);
    double max = result[0].item<double>();
    int index = 0;
    for(int i = 1; i < 3; i++)
    {
        if(result[i].item<double>() > max)
        {
            max = result[i].item<double>();
            index = i;
        }
    }

    if(dis(gen) < epsilon)
    {
        std::uniform_int_distribution<> random_action(0, 2);
        index = random_action(gen);
    }

    epsilon = std::max(epsilon_min, epsilon * epsilon_decay);

	return index;
}

void Agent::update()
{
    target = policy;
}

void Agent::train(std::vector<int> state, std::vector<int> new_state, int action, int reward)
{
	static double gamma = 0.9;
	static int t_number = 0;
    static int batch_size = 128;

	torch::Tensor t_state = torch::tensor(state, torch::dtype(torch::kFloat32));
    torch::Tensor t_new_state = torch::tensor(state, torch::dtype(torch::kFloat32));

	if (buffer.size() >= buffer_size) 
        buffer.pop_front();

    buffer.push_back(Experience{t_state, t_new_state, action, reward});

    // Train
    if(buffer.size() > batch_size)
    {
    	t_number++;
    	//std::cout << "TRAIN: " << t_number << std::endl;

        std::vector<Experience> mini_batch;
        std::sample(buffer.begin(), buffer.end(), std::back_inserter(mini_batch), batch_size, std::mt19937{std::random_device{}()});

        std::vector<torch::Tensor> states, next_states, rewards, actions;
        for (const auto& exp : mini_batch) {
            states.push_back(exp.state);                    // (25*25)
            next_states.push_back(exp.next_state);          // (25*25)
            rewards.push_back(torch::tensor({exp.reward})); // (1)
            actions.push_back(torch::tensor({exp.action})); // (1)
        }

        torch::Tensor batch_states = torch::stack(states);           // (bs, 25*25)
        torch::Tensor batch_next_states = torch::stack(next_states); // (bs, 25*25)
        torch::Tensor batch_rewards = torch::stack(rewards);         // (bs, 1)
        torch::Tensor batch_actions = torch::stack(actions);         // (bs, 1)

        // LOSS 
        torch::Tensor q_values = policy.forward(batch_states);        // (bs, 3)
        torch::Tensor q_value = q_values.gather(1, batch_actions);    // (bs, 1)

        torch::Tensor next_q_values = target.forward(batch_next_states);        // (bs, 3)
        torch::Tensor next_q_value  = std::get<0>(next_q_values.max(1, true));  // (bs, 1)

        torch::Tensor expected_q_value = batch_rewards + gamma * next_q_value * (batch_rewards < -.5f);
        
        torch::Tensor loss = torch::mse_loss(q_value, expected_q_value);

        optimizer->zero_grad();
        loss.backward();
        optimizer->step();
    }
}