#pragma once

#include <string>

#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"
#include "Demos/NeuralNetwork/NeuralNetwork.hpp"

class NeuralNetworkDemo : public DemoBase
{
    int inputSize = 2;
    int hiddenSize = 8;
    float learningRate = 0.05f;

   public:
    NeuralNetworkDemo()
    {
        mName = "EmptyDemo";
        mNotes = {{{"Neural network information:"},
                   {R"(This is a simple neural network implementation with
2 input neurons, 8 hidden neurons, and 1 output neuron. 
The network is trained to learn the XOR function.)"}}};
    };
    ~NeuralNetworkDemo() = default;

    void ShowExample() override
    {
        PrintNotes();
        NeuralNetwork nn(inputSize, hiddenSize, learningRate);
        std::vector<std::vector<double>> training_data = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};

        // XOR truth table:
        // 0 XOR 0 = 0
        // 0 XOR 1 = 1
        // 1 XOR 0 = 1
        // 1 XOR 1 = 0

        // XOR targets
        std::vector<double> targets = {0, 1, 1, 0};

        // Train the network for 100,000 epochs
        for (int epoch = 0; epoch < 100000; ++epoch)
        {
            for (size_t i = 0; i < training_data.size(); ++i)
                nn.train(training_data[i], targets[i]);
        }

        // Test the trained network
        std::cout << "Testing the trained network:\n";
        for (const auto& data : training_data)
            std::cout << data[0] << " " << data[1] << " => " << nn.feedforward(data) << "\n";
    }

    void ShowDemo() override
    {
        ShowExample();
    };
};
