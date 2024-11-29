#include <gtest/gtest.h>
#include "../src/DFAGenerator.h"

using State = DFAGenerator::state;
using Symbol = DFAGenerator::symbol;
using Class = DFAGenerator::clazz;

TEST(DFAGeneratorTests, ConvertingSimpleNFACorrectly) {
    // Arrange
    unordered_map<State, unordered_map<Symbol, unordered_set<State>>> nfaTransMap;
    unordered_map<State, unordered_set<State>> nfaEpsilonTransMap;
    unordered_map<State, Class> nfaAcceptingStates;
    unordered_set<State> nfaInitialStates;
    unordered_set<Symbol> symbols;
    DFAGenerator* dfaGen = nullptr;


    nfaAcceptingStates = {{1, "id"}};
    nfaInitialStates = {0};
    symbols = {'a', 'b'};
    nfaEpsilonTransMap = {
        {0, {1}},
    };
    nfaTransMap = {
        {
            0,
            {
                {'a', {0}},
                {'b', {1}}
            }
        },
        {
            1,
            {
                {'a', {1}},
                {'b', {0}}
            }
        }
    };

    // Act
    dfaGen = new DFAGenerator(nfaTransMap, nfaEpsilonTransMap, nfaAcceptingStates, nfaInitialStates, symbols);

    // expected data
    unordered_map<State, unordered_map<Symbol, State>>
        expectedDfaTransMap = {
        {
            0,
            {
                {'a', 0},
                {'b', 0}
            }
        },
    };
    unordered_map<State, Class> expectedAcceptingStates{{0, "id"}};
    State expectedInitialState {0};
    int expectedNumberOfStates {1};

    // Assert
    ASSERT_NE(dfaGen, nullptr);
    EXPECT_EQ(dfaGen->getTransMap(), expectedDfaTransMap);
    EXPECT_EQ(dfaGen->getAcceptingStates(), expectedAcceptingStates);
    EXPECT_EQ(dfaGen->getInitialState(), expectedInitialState);
    EXPECT_EQ(dfaGen->getNumberOfStates(), expectedNumberOfStates);
}

// lecture example
TEST(DFAGeneratorTests, ConvertingComplexNFACorrectly) {
    // Arrange
    unordered_map<State, unordered_map<Symbol, unordered_set<State>>> nfaTransMap;
    unordered_map<State, unordered_set<State>> nfaEpsilonTransMap;
    unordered_map<State, Class> nfaAcceptingStates;
    unordered_set<State> nfaInitialStates;
    unordered_set<Symbol> symbols;
    DFAGenerator* dfaGen = nullptr;

    nfaAcceptingStates = {{10, "while"}};
    nfaInitialStates = {0};
    symbols = {'a', 'b'};
    nfaEpsilonTransMap = {
        {0, {1, 7}},
        {1, {2, 4}},
        {3, {6}},
        {5, {6}},
        {6, {1, 7}},
    };
    nfaTransMap = {
        {
            2,
            {
                {'a', {3}},
            }
        },
        {
            4,
            {
                {'b', {5}}
            }
        },
        {
            7,
            {
                {'a', {8}}
            }
        },
        {
            8,
            {
                {'b', {9}}
            }
        },
        {
            9,
            {
                {'b', {10}}
            }
        }
    };

    // Act
    dfaGen = new DFAGenerator(nfaTransMap, nfaEpsilonTransMap, nfaAcceptingStates, nfaInitialStates, symbols);

    // expected data
    unordered_map<State, unordered_map<Symbol, State>>
        expectedDfaTransMap = {
        {
            0,
            {
                {'a', 2},
                {'b', 1}
            }
        },
        {
            2,
            {
                {'a', 2},
                {'b', 3}
            }
        },
        {
            1,
            {
                {'a', 2},
                {'b', 1}
            }
        },
        {
            3,
            {
                {'a', 2},
                {'b', 4}
            }
        },
        {
            4,
            {
                {'a', 2},
                {'b', 1}
            }
        }
    };
    unordered_map<State, Class> expectedAcceptingStates{{4, "while"}};
    State expectedInitialState {0};
    int expectedNumberOfStates {5};

    // Assert
    ASSERT_NE(dfaGen, nullptr);
    EXPECT_EQ(dfaGen->getTransMap(), expectedDfaTransMap);
    EXPECT_EQ(dfaGen->getAcceptingStates(), expectedAcceptingStates);
    EXPECT_EQ(dfaGen->getInitialState(), expectedInitialState);
    EXPECT_EQ(dfaGen->getNumberOfStates(), expectedNumberOfStates);
}

TEST(DFAGeneratorTests, moveNFACorrectly) {
    // Arrange
    set<State> srcStates {
      0, 1, 2, 4, 7
    };
    unordered_map<State, unordered_map<Symbol, unordered_set<State>>> nfaTransMap = {
        {
            2,
            {
                {'a', {3}},
            }
        },
        {
            4,
            {
                {'b', {5}}
            }
        },
        {
            7,
            {
                {'a', {8}}
            }
        },
        {
            8,
            {
                {'b', {9}}
            }
        },
        {
            9,
            {
                {'b', {10}}
            }
        }
    };
    Symbol s = 'a';


    // Act
    unordered_set<State> destinationStates = DFAGenerator::moveNfa(srcStates, s, nfaTransMap);

    // expected data
    unordered_set<State> expectedDestinationStates {
        8, 3
    };

    // Assert
    ASSERT_EQ(destinationStates, expectedDestinationStates);
}

TEST(DFAGeneratorTests, unorderedSetToOrderedSetCorrectly) {
    // Arrange
    unordered_set<State> s2 = {98, 12, 57, 23};

    // Act
    set<State> convertedSet = DFAGenerator::unorderedSetToOrderedSet(s2);

    // expected
    set<State> s1 = {23, 12, 98, 57};

    // Assert
    ASSERT_EQ(s1, convertedSet);
}

TEST(DFAGeneratorTests, computeEpsilonClosureCorrectly) {
    // Arrange
    unordered_map<State, unordered_set<State>> nfaEpsilonTransMap = {
        {0, {1, 7}},
        {1, {2, 4}},
        {3, {6}},
        {5, {6}},
        {6, {1, 7}},
    };
    unordered_set<State> originalStates {
        6, 0, 1
    };

    // Act
    unordered_set<State> destinationStates = DFAGenerator::computeEpsilonClosure(originalStates, nfaEpsilonTransMap);

    // expected
    unordered_set<State> expectedDestinationStates{
        0, 1, 6, 7, 2, 4,
    };

    // Assert
    ASSERT_EQ(expectedDestinationStates, destinationStates);
}