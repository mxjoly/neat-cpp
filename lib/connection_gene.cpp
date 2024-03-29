#include <iostream>
#include "math_utils.h"
#include "config.h"
#include "node.h"
#include "connection_gene.h"

ConnectionGene::ConnectionGene(std::shared_ptr<Node> from, std::shared_ptr<Node> to, double w, int innovation, bool enabled) : from_node(from), to_node(to), weight(w), innovation_nb(innovation), enabled(enabled) {}

void ConnectionGene::mutate(const NeatConfig &config)
{
    // Change the weight of the connection or disable/enable it
    if (randrange() < config.weight_replace_rate)
        weight = uniform(config.weight_min_value, config.weight_max_value);
    else if (randrange() < config.weight_mutate_rate)
    {
        // Otherwise, slightly change it
        weight += normal(config.weight_init_mean, config.weight_init_stdev) / 50;
        // Keep weight between bounds
        weight = std::min(std::max(weight, config.weight_min_value), config.weight_max_value);
    }

    if (randrange() < config.enabled_mutate_rate)
        enabled = !enabled;
}

bool ConnectionGene::is_equal(std::shared_ptr<ConnectionGene> other)
{
    return from_node == other->from_node && to_node == other->to_node && weight == other->weight && innovation_nb == other->innovation_nb && enabled == other->enabled;
}

std::shared_ptr<ConnectionGene> ConnectionGene::clone(std::shared_ptr<Node> from, std::shared_ptr<Node> to)
{
    return std::make_shared<ConnectionGene>(from, to, weight, innovation_nb, enabled);
}
