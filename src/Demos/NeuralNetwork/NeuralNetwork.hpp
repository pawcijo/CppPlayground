#pragma once

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

/**
 *  Sigmoid activation function
 *  @param x: Input value
 *  @return: Transformed value using sigmoid function
 */
inline double sigmoid(double x)
{
    return 1.0 / (1.0 + std::exp(-x));
}

/**
 *  Derivative of the sigmoid function
 *  @param x: Input value (already passed through sigmoid)
 *  @return: Derivative of the sigmoid function
 */
inline double sigmoid_derivative(double x)
{
    return x * (1.0 - x);
}

class NeuralNetwork
{
   private:
    // warstwa ukryta - przechowuje wartosci neuronow z warstwy ukrytej
    std::vector<double> hidden_layer;
    // wagi pomiedzy warstwa wejsciowa a ukryta
    std::vector<std::vector<double>> weights_input_hidden;
    // wagi pomiedzy warstwa ukryta a wyjsciowa -
    // kazdy neuron z warstwy ukrytej jest polaczony z kazdym neuronem z warstwy wyjsciowej
    std::vector<double> weights_hidden_output;
    // bias dla warstwy ukrytej i wyjsciowej - bias to dodatkowy parametr,
    // ktory pozwala na przesuniecie funkcji aktywacji
    double bias_hidden, bias_output;
    // wspolczynnik uczenia - jest to parametr, ktory okresla jak szybko siec sie uczy
    double learning_rate;

   public:
    /**
     *  Constructor for initializing the neural network
     *  @param input_size: Number of input neurons
     *  @param hidden_size: Number of hidden neurons
     *  @param lr: Learning rate (default is 0.5)
     */
    NeuralNetwork(int input_size, int hidden_size, double lr = 0.5)
    {
        learning_rate = lr;
        hidden_layer.resize(hidden_size);
        weights_input_hidden.resize(input_size, std::vector<double>(hidden_size));
        weights_hidden_output.resize(hidden_size);

        // Initialize weights and biases with random values
        std::srand(std::time(nullptr));
        for (auto &row : weights_input_hidden)
            for (auto &w : row) w = (std::rand() % 100) / 100.0;

        for (auto &w : weights_hidden_output) w = (std::rand() % 100) / 100.0;

        bias_hidden = (std::rand() % 100) / 100.0;
        bias_output = (std::rand() % 100) / 100.0;
    }

    /**
     *  Performs forward propagation through the network
     *  @param inputs: Vector of input values
     *  @return: Output of the neural network
     */
    double feedforward(const std::vector<double> &inputs)
    {
        // Compute hidden layer values
        for (size_t i = 0; i < hidden_layer.size(); ++i)
        {
            double sum = bias_hidden;
            for (size_t j = 0; j < inputs.size(); ++j)
                sum += inputs[j] * weights_input_hidden[j][i];
            hidden_layer[i] = sigmoid(sum);
        }

        // Compute output value
        double sum = bias_output;
        for (size_t i = 0; i < hidden_layer.size(); ++i)
            sum += hidden_layer[i] * weights_hidden_output[i];

        return sigmoid(sum);
    }

    /**
     *  Trains the network using backpropagation
     *  @param inputs: Vector of input values
     *  @param target: Expected output value
     */
    void train(const std::vector<double> &inputs, double target)
    {
        double output = feedforward(inputs);
        double error = target - output;
        double d_output = error * sigmoid_derivative(output);

        // Update hidden-to-output weights
        for (size_t i = 0; i < hidden_layer.size(); ++i)
            weights_hidden_output[i] += learning_rate * d_output * hidden_layer[i];
        bias_output += learning_rate * d_output;

        // Update input-to-hidden weights
        for (size_t i = 0; i < hidden_layer.size(); ++i)
        {
            double d_hidden =
                d_output * weights_hidden_output[i] * sigmoid_derivative(hidden_layer[i]);
            for (size_t j = 0; j < inputs.size(); ++j)
                weights_input_hidden[j][i] += learning_rate * d_hidden * inputs[j];
        }
    }
};
