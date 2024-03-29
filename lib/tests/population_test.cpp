#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <random>
#include "../config.h"
#include "../population.h"

class PopulationTest : public testing::Test
{
protected:
    NeatConfig config;

    void SetUp() override
    {
        // Mock NeatConfig
        config = load_config("default_config.txt");
        config.num_inputs = 5;
        config.num_outputs = 2;
        config.population_size = 10;
        config.species_elitism = 2;
        config.max_stagnation = 5;
        config.bad_species_threshold = 0.5;
        config.no_fitness_termination = false;
        config.min_species_size = 2;
        config.fitness_threshold = 100;
    }
};

TEST_F(PopulationTest, PopulationInitialization)
{
    Population *p = new Population(config);
    ASSERT_EQ(p->genomes.size(), config.population_size);
    ASSERT_EQ(p->species.size(), 0);
    ASSERT_EQ(p->generation, 0);
}

TEST_F(PopulationTest, SetBestGenome)
{
    Population *p = new Population(config);

    // Assume best fitness is set to 10 for simplicity
    p->best_fitness = 10;

    // Mock species and genomes
    Genome *genome = new Genome(config);
    genome->fitness = 20;
    Species *species = new Species(genome);
    species->genomes = {genome};
    p->species = {species};

    // Set best genome
    p->set_best_genome();

    // Assert that the best genome is set
    ASSERT_EQ(p->best_genome, genome);
}

TEST_F(PopulationTest, Speciate)
{
    Population *p = new Population(config);

    // Mock genomes
    Genome *genome1 = new Genome(config);
    Genome *genome2 = new Genome(config);
    p->genomes = {genome1, genome2};

    // Mock species
    Species *species1 = new Species(genome1);
    Species *species2 = new Species(genome2);
    p->species = {species1, species2};

    // Run speciation
    p->speciate();

    // Assert that genomes are grouped into species
    ASSERT_GT(p->species.size(), 0);
}

TEST_F(PopulationTest, ReproduceSpecies)
{
    Population *p = new Population(config);

    // Mock species
    Genome *genome = new Genome(config);
    Species *species = new Species(genome);
    p->species = {species};

    // Run reproduction
    p->reproduce_species();

    // Assert that the p's genomes are updated
    ASSERT_EQ(p->generation, 1);
    ASSERT_EQ(p->best_genome, genome);
    ASSERT_EQ(p->genomes.size(), config.population_size);
}

TEST_F(PopulationTest, SortSpecies)
{
    Population *p = new Population(config);

    // Mock species
    for (int i = 0; i < 5; ++i)
    {
        Genome *genome = new Genome(config);
        genome->fitness = random() % 100;
        Species *species = new Species(genome);
        p->species.push_back(species);
    }

    // Run species sorting
    p->sort_species();

    // Assert species are sorted
    const auto &speciesList = p->species;
    ASSERT_EQ(p->species.size(), 5);

    auto getBestFitness = [](const Species *s)
    { return s->best_fitness; };
    ASSERT_TRUE(std::is_sorted(speciesList.begin(), speciesList.end(), [&getBestFitness](const Species *s1, const Species *s2)
                               { return getBestFitness(s1) > getBestFitness(s2); }));
}

TEST_F(PopulationTest, KillStagnantSpecies)
{
    Population *p = new Population(config);

    // Mock genomes
    Genome *genome1 = new Genome(config);
    Genome *genome2 = new Genome(config);
    Genome *genome3 = new Genome(config);
    Genome *genome4 = new Genome(config);

    // Mock species
    Species *speciesToKeep1 = new Species(genome1);
    Species *speciesToKeep2 = new Species(genome2);
    Species *speciesToRemove1 = new Species(genome3);
    Species *speciesToRemove2 = new Species(genome4);

    // Set the stagnation
    speciesToKeep1->stagnation = 2;
    speciesToKeep2->stagnation = 4;
    speciesToRemove1->stagnation = 6;
    speciesToRemove2->stagnation = 8;

    p->genomes = {genome1, genome2, genome3, genome4};
    p->species = {speciesToKeep1, speciesToKeep2, speciesToRemove1, speciesToRemove2};

    // Run killing stagnant species
    p->kill_stagnant_species();

    // Assert stagnant species are removed
    const auto &remainingSpecies = p->species;
    ASSERT_EQ(remainingSpecies.size(), 2);
    ASSERT_EQ(remainingSpecies[0], speciesToKeep1);
    ASSERT_EQ(remainingSpecies[1], speciesToKeep2);

    // Assert the genomes of the stagnant species are removed
    ASSERT_TRUE(std::find(p->genomes.begin(), p->genomes.end(), genome1) != p->genomes.end());
    ASSERT_TRUE(std::find(p->genomes.begin(), p->genomes.end(), genome2) != p->genomes.end());
    ASSERT_TRUE(std::find(p->genomes.begin(), p->genomes.end(), genome3) == p->genomes.end());
    ASSERT_TRUE(std::find(p->genomes.begin(), p->genomes.end(), genome4) == p->genomes.end());

    // Cleanup
    delete speciesToKeep1;
    delete speciesToKeep2;
    delete speciesToRemove1;
    delete speciesToRemove2;
}

TEST_F(PopulationTest, KillBadSpecies)
{
    Population *p = new Population(config);

    // Mock genomes
    Genome *genome1 = new Genome(config);
    Genome *genome2 = new Genome(config);
    Genome *genome3 = new Genome(config);

    // Mock species
    Species *goodSpecies = new Species(genome1);
    Species *badSpecies1 = new Species(genome2);
    Species *badSpecies2 = new Species(genome3);

    // Mock average fitness
    double average_fitness1 = 100.0;
    double average_fitness2 = 3.0;
    double average_fitness3 = 1.0;

    // Set average fitness
    goodSpecies->average_fitness = average_fitness1;
    badSpecies1->average_fitness = average_fitness2;
    badSpecies2->average_fitness = average_fitness3;

    p->genomes = {genome1, genome2, genome3};
    p->species = {goodSpecies, badSpecies1, badSpecies2};

    // Run killing bad species
    p->kill_bad_species();

    // Assert bad species are removed
    const auto &remainingSpecies = p->species;
    ASSERT_EQ(remainingSpecies.size(), 1);
    ASSERT_EQ(remainingSpecies[0], goodSpecies);

    // Assert genomes are correctly removed
    ASSERT_TRUE(std::find(p->genomes.begin(), p->genomes.end(), genome1) != p->genomes.end());
    ASSERT_TRUE(std::find(p->genomes.begin(), p->genomes.end(), genome2) == p->genomes.end());
    ASSERT_TRUE(std::find(p->genomes.begin(), p->genomes.end(), genome3) == p->genomes.end());

    // Cleanup
    delete goodSpecies;
    delete badSpecies1;
    delete badSpecies2;
}

TEST_F(PopulationTest, UpdateSpecies)
{
    Population *p = new Population(config);

    // Mock species
    Species *species = new Species(new Genome(config));
    for (int i = 0; i < 10; ++i)
        species->add_to_species(new Genome(config));
    p->species.push_back(species);

    // Run updating species
    p->update_species();

    // Assert species is updated
    const auto &updatedSpecies = p->species;
    ASSERT_EQ(updatedSpecies.size(), 1);

    const auto &updatedGenomes = updatedSpecies[0]->genomes;
    ASSERT_EQ(updatedGenomes.size(), config.min_species_size);
}

TEST_F(PopulationTest, Clone)
{
    Population *population = new Population(config);
    Population *clone = population->clone();

    // Asserts the clone is valid

    for (size_t i = 0; i < clone->genomes.size(); ++i)
        ASSERT_TRUE(clone->genomes[i]->is_equal(population->genomes[i]));

    for (size_t i = 0; i < clone->species.size(); ++i)
        ASSERT_TRUE(clone->species[i]->is_equal(population->species[i]));

    if (population->best_genome != NULL)
        ASSERT_TRUE(clone->best_genome->is_equal(population->best_genome));

    ASSERT_EQ(clone->generation, population->generation);
    ASSERT_EQ(clone->average_fitness, population->average_fitness);
    ASSERT_EQ(clone->best_fitness, population->best_fitness);
}
