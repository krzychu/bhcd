#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <utility>

#include <gtest/gtest.h>

#include "special_functions.h"
#include "statistics.h"
#include "forest.h"

using namespace BHCD;

class MockLikelihood : public Likelihood
{
    public:
        virtual double log(const EdgeStatistics& stats) const
        {
            return stats.num_present / (double) (stats.num_absent + stats.num_present);
        }
};


class MockSplit : public Split
{
    public: 
        virtual double probability(int num_children) const
        {
            return 1.0 / num_children; 
        }
};


TEST(SpecialFunctionsTest, LogGamma) 
{
    int fact = 1;
    for(int i = 1; i < 10; i++)
    {
        fact *= i; 
        ASSERT_NEAR(log(fact), log_gamma(i + 1), 1e-6);
    }

    ASSERT_NEAR(0.5 * log(M_PI), log_gamma(0.5), 1e-6);
}


TEST(SpecialFunctionsTest, LogBeta) 
{
    ASSERT_FLOAT_EQ(-log(12), log_beta(2, 3));
}


TEST(StatisticsTest, EdgeStatisticsOperations)
{
    EdgeStatistics a(1, 2);
    EdgeStatistics b(4, 8);

    EdgeStatistics c = a + b;
    ASSERT_EQ(5, c.num_present);
    ASSERT_EQ(10, c.num_absent);

    EdgeStatistics d = b - a;
    ASSERT_EQ(3, d.num_present);
    ASSERT_EQ(6, d.num_absent);
}

/*
TEST(ForestTest, NodeJoiningTest)
{
    Model m(new MockLikelihood(), new MockLikelihood(), new MockSplit());
    Forest f(m, 10);

    const int over = f.add_root();
    const int a = f.add_root();
    const int b = f.add_root();
    const int c = f.add_root();
    const int under = f.add_root();

    f.add_graph_edge(a, over, EdgeStatistics(6, 7));
    f.add_graph_edge(a, c, EdgeStatistics(1, 0));
    f.add_graph_edge(b, c, EdgeStatistics(0, 1));
    f.add_graph_edge(b, under, EdgeStatistics(4, 7));
    
    const Node& n = f.join(a, b);
    ASSERT_EQ(a, n.tree_children[0]);
    ASSERT_EQ(b, n.tree_children[1]);

    ASSERT_EQ(3, n.graph_edges.size());

    ASSERT_EQ(over, n.graph_edges[0].to);
    ASSERT_EQ(EdgeStatistics(6, 7), n.graph_edges[0].stats);

    ASSERT_EQ(c, n.graph_edges[1].to);
    ASSERT_EQ(EdgeStatistics(1, 1), n.graph_edges[1].stats);

    ASSERT_EQ(under, n.graph_edges[2].to);
    ASSERT_EQ(EdgeStatistics(4, 7), n.graph_edges[2].stats);

    const Node& nodec = f.get_node(c);
    ASSERT_EQ(3, nodec.graph_edges.size());
    ASSERT_EQ(EdgeStatistics(1, 1), nodec.graph_edges[2].stats);

    ASSERT_EQ(EdgeStatistics(1, 1), f.get_edge_statistics(nodec, n));
}
*/


TEST(SpecialFunctionsTest, LogCombine)
{
    const double alpha = 0.3;
    const double a = 10;
    const double b = 100;

    const double expected = log(alpha * exp(a) + (1.0 - alpha) * exp(b));
    ASSERT_NEAR(expected, log_combine(alpha, a, b), 1e-4);
}

