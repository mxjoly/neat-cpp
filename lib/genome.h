#ifndef GENOME_H
#define GENOME_H

#include <iostream>
#include <vector>
#include <string>
#include "config.h"
#include "connection_gene.h"

class ConnectionHistory; // Forward declaration
class Node;              // Forward declaration

/**
 * @brief Represents a genome in the NEAT (NeuroEvolution of Augmenting Topologies) algorithm.
 */
class Genome
{
public:
    std::string id;                      // Unique identifier for the genome.
    NeatConfig config;                   // Configuration settings for NEAT.
    std::vector<ConnectionGene *> genes; // List of connection genes.
    std::vector<Node *> nodes;           // List of nodes in the genome.
    int inputs;                          // Number of input nodes.
    int outputs;                         // Number of output nodes.
    int layers;                          // Number of layers in the neural network.
    int next_node;                       // ID for the next node to be added.
    int bias_node;                       // ID of the bias node.
    std::vector<Node *> network;         // Fully connected network.
    double fitness;                      // Fitness score of the genome.

    Genome();
    Genome(const NeatConfig &config, bool crossover = false);
    ~Genome();

    /**
     * @brief Fully connects the genome's neural network.
     * @param innovation_history List of connection history for innovation tracking.
     */
    void fully_connect(std::vector<ConnectionHistory *> &innovation_history);

    /**
     * @brief Retrieves a node based on its ID.
     * @param id ID of the node.
     * @return Pointer to the node if found, nullptr otherwise.
     */
    Node *get_node(int id);

    /**
     * @brief Connects nodes based on the genome's genes.
     */
    void connect_nodes();

    /**
     * @brief Performs a feed-forward operation on the neural network.
     * @param input_values List of double values.
     * @return List of output values.
     */
    std::vector<double> feed_forward(std::vector<double> input_values);

    /**
     * @brief Generates the network structure by adding nodes and connections.
     */
    void generate_network();

    /**
     * @brief Adds a node to the genome.
     * @param innovation_history List of connection history for innovation tracking.
     */
    void add_node(std::vector<ConnectionHistory *> &innovation_history);

    /**
     * @brief Removes a node from the genome.
     */
    void remove_node();

    /**
     * @brief Adds a connection to the genome.
     * @param innovation_history List of connection history for innovation tracking.
     */
    void add_connection(std::vector<ConnectionHistory *> &innovation_history);

    /**
     * @brief Removes a connection from the genome.
     */
    void remove_connection();

    /**
     * @brief Generates a new random connection weight based on configuration settings.
     * @return Random connection weight.
     */
    double new_connection_weight() const;

    /**
     * @brief Gets the innovation number for a connection between two nodes.
     * @param innovation_history List of connection history for innovation tracking.
     * @param from_node Pointer to the source node.
     * @param to_node Pointer to the target node.
     * @return Innovation number for the connection.
     */
    int get_innovation_number(std::vector<ConnectionHistory *> &innovation_history, Node *from_node, Node *to_node) const;

    /**
     * @brief Checks if the genome is fully connected.
     * @return True if fully connected, false otherwise.
     */
    bool fully_connected() const;

    /**
     * @brief Mutates the genome based on configuration settings.
     * @param innovation_history List of connection history for innovation tracking.
     */
    void mutate(std::vector<ConnectionHistory *> &innovation_history);

    /**
     * @brief Performs crossover with another genome to create a child genome.
     * @param parent Parent genome for crossover.
     * @return Child genome resulting from crossover.
     */
    Genome *crossover(Genome *parent) const;

    /**
     * @brief Finds the index of a matching gene in another genome.
     * @param parent Genome to compare against.
     * @param innovation Innovation number of the gene to match.
     * @return Index of the matching gene, -1 if not found.
     */
    int matching_gene(Genome *parent, int innovation) const;

    /**
     * @brief Creates a clone of the genome.
     * @return Cloned genome.
     */
    void print_genome() const;

    /**
     * @brief Compare two genomes.
     * @param other The genome to compare with it.
     * @return True if the genomes are the same and connection genes, otherwise false.
     */
    bool is_equal(Genome *other);

    /**
     * @brief Creates a clone of the genome.
     * @return Cloned genome.
     */
    Genome *clone();

    /**
     * @brief Saves the genome to a binary file.
     * @param file_path Path to the file.
     */
    void save(const std::string &file_path);

    /**
     * @brief Loads a genome from a binary file.
     * @param file_path Path to the file.
     * @return Loaded genome.
     */
    static Genome *load(const std::string &file_path);
};

#endif // GENOME_H