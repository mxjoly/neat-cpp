#include <gtest/gtest.h>
#include "../config.h"
#include "../node.h"
#include "../connection_gene.h"
#include "../activation_functions.h"

class ConnectionGeneTest : public ::testing::Test
{
protected:
    NeatConfig config;
    std::shared_ptr<Node> fromNode;
    std::shared_ptr<Node> toNode;
    std::shared_ptr<ConnectionGene> gene;

    void SetUp() override
    {
        config = load_config("default_config.txt");
        fromNode = std::make_shared<Node>(1, "sigmoid", 1);
        toNode = std::make_shared<Node>(2, "sigmoid", 2);
        gene = std::make_shared<ConnectionGene>(fromNode, toNode, 0.5, 1, true);
    }

    // Add your test functions for ConnectionGene
    void testInit()
    {
        ASSERT_EQ(gene->from_node, fromNode);
        ASSERT_EQ(gene->to_node, toNode);
        ASSERT_DOUBLE_EQ(gene->weight, 0.5);
        ASSERT_EQ(gene->innovation_nb, 1);
        ASSERT_TRUE(gene->enabled);
    }

    void testMutateWeightReplace()
    {
        config.weight_replace_rate = 1.0;
        gene->mutate(config);
        ASSERT_TRUE(gene->weight >= -1.0 && gene->weight <= 1.0);
    }

    void testMutateWeightMutate()
    {
        config.weight_replace_rate = 0.0;
        config.weight_mutate_rate = 1.0;
        gene->mutate(config);
        ASSERT_NE(gene->weight, 0.5);
    }

    void testMutateEnabledMutate()
    {
        config.enabled_mutate_rate = 1.0;
        gene->mutate(config);
        ASSERT_FALSE(gene->enabled);
    }

    void testClone()
    {
        std::shared_ptr<ConnectionGene> clonedConnectionGene = gene->clone(fromNode, toNode);
        ASSERT_TRUE(gene->is_equal(clonedConnectionGene));
    }
};

TEST_F(ConnectionGeneTest, Init)
{
    testInit();
}

TEST_F(ConnectionGeneTest, MutateWeightReplace)
{
    testMutateWeightReplace();
}

TEST_F(ConnectionGeneTest, MutateWeightMutate)
{
    testMutateWeightMutate();
}

TEST_F(ConnectionGeneTest, MutateEnabledMutate)
{
    testMutateEnabledMutate();
}

TEST_F(ConnectionGeneTest, Clone)
{
    testClone();
}
