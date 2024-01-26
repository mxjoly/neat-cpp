#ifndef NODE_H
#define NODE_H

#include "activation_functions.h"
#include "config.h"

// Type alias for the function pointer
using ActivationFunctionPointer = double (*)(double);

class ConnectionGene; // Forward declaration

/**
 * @brief Represents a node in a neural network.
 */
class Node
{
public:
    int id;                                           // Unique identifier for the node.
    double input_sum;                                 // Accumulated input sum for the node.
    double output_value;                              // Output value after activation.
    std::vector<ConnectionGene *> output_connections; // List of outgoing connections.
    int layer;                                        // Layer to which the node belongs.
    ActivationFunction activation_function;           // Activation function of the node.

    /**
     * @brief Constructor for the Node class.
     * @param id Unique identifier for the node.
     * @param activation_function Activation function of the node.
     * @param layer Layer to which the node belongs (default is 0).
     */
    Node(int id, ActivationFunction activation_function, int layer = 0);

    /**
     * @brief Activate the node using its activation function.
     */
    void activate();

    /**
     * @brief Propagate the output to connected nodes.
     */
    void propagate_output();

    /**
     * @brief Mutates the node's properties based on the NEAT algorithm.
     * @param config NEAT configuration settings.
     * @param is_bias_node Indicates whether the node is a bias node.
     */
    void mutate(const NeatConfig &config, bool is_bias_node = false);

    /**
     * @brief Checks if the node is connected to another node.
     * @param node Pointer to the target node.
     * @return True if connected, false otherwise.
     */
    bool is_connected_to(Node *node);

    /**
     * @brief Compare two nodes.
     * @param other The node to compare with it.
     * @return True if the nodes are the same and connection genes, otherwise false.
     */
    bool is_equal(Node *other);

    /**
     * @brief Creates a clone of the node.
     * @return Pointer to the cloned node.
     */
    Node *clone();

private:
    /**
     * @brief Retrieves the activation function based on the specified enum.
     * @param activation_function Activation function specified by the enum.
     * @return Pointer to the activation function.
     */
    ActivationFunctionPointer get_function(ActivationFunction function);
};

#endif