#include <gtest/gtest.h>
#include "../src/dfa/DFAGenerator.h"

TEST(DFAGeneratorTests, ConvertingSimpleNFACorrectly) {
    // Arrange
    unordered_map<state, unordered_map<symbol, unordered_set<state>>> nfaTransMap;
    unordered_map<state, unordered_set<state>> nfaEpsilonTransMap;
    unordered_map<state, clazz> nfaAcceptingStates;
    unordered_set<state> nfaInitialStates;
    unordered_set<symbol> symbols;
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
    unordered_map<state, unordered_map<symbol, state>>
        expectedDfaTransMap = {
        {
            0,
            {
                {'a', 0},
                {'b', 0}
            }
        },
    };
    unordered_map<state, clazz> expectedAcceptingStates{{0, "id"}};
    state expectedInitialState {0};
    int expectedNumberOfStates {1};

    // Assert
    ASSERT_NE(dfaGen, nullptr);
    EXPECT_EQ(dfaGen->getTransMap(), expectedDfaTransMap);
    EXPECT_EQ(dfaGen->getAcceptingStates(), expectedAcceptingStates);
    EXPECT_EQ(dfaGen->getInitialState(), expectedInitialState);
    EXPECT_EQ(dfaGen->getNumberOfStates(), expectedNumberOfStates);
    EXPECT_EQ(dfaGen->getLanguageSymbols(), symbols);
}

// lecture example
TEST(DFAGeneratorTests, ConvertingComplexNFACorrectly) {
    // Arrange
    unordered_map<state, unordered_map<symbol, unordered_set<state>>> nfaTransMap;
    unordered_map<state, unordered_set<state>> nfaEpsilonTransMap;
    unordered_map<state, clazz> nfaAcceptingStates;
    unordered_set<state> nfaInitialStates;
    unordered_set<symbol> symbols;
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
    unordered_map<state, unordered_map<symbol, state>>
        expectedDfaTransMap = {
        {
            0,
            {
                {'a', 1},
                {'b', 0}
            }
        },
        {
            1,
            {
                {'a', 1},
                {'b', 2}
            }
        },
        {
            2,
            {
                {'a', 1},
                {'b', 3}
            }
        },
        {
            3,
            {
                {'a', 1},
                {'b', 0}
            }
        }
    };
    unordered_map<state, clazz> expectedAcceptingStates{{3, "while"}};
    state expectedInitialState {0};
    int expectedNumberOfStates {4};

    // Assert
    ASSERT_NE(dfaGen, nullptr);
    EXPECT_EQ(dfaGen->getTransMap(), expectedDfaTransMap);
    EXPECT_EQ(dfaGen->getAcceptingStates(), expectedAcceptingStates);
    EXPECT_EQ(dfaGen->getInitialState(), expectedInitialState);
    EXPECT_EQ(dfaGen->getNumberOfStates(), expectedNumberOfStates);
    EXPECT_EQ(dfaGen->getLanguageSymbols(), symbols);
}

TEST(DFAGeneratorTests, ConvertingSimpleNFAWithRejectingStatesCorrectly) {
    // Arrange
    unordered_map<state, unordered_map<symbol, unordered_set<state>>> nfaTransMap;
    unordered_map<state, unordered_set<state>> nfaEpsilonTransMap;
    unordered_map<state, clazz> nfaAcceptingStates;
    unordered_set<state> nfaInitialStates;
    unordered_set<symbol> symbols;
    DFAGenerator* dfaGen = nullptr;


    nfaAcceptingStates = {{1, "id"}};
    nfaInitialStates = {0};
    symbols = {'a', 'b', 'c'};
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
    unordered_map<state, unordered_map<symbol, state>>
        expectedDfaTransMap = {
        {
            0,
            {
                {'a', 0},
                {'b', 0},
            }
        },
    };
    unordered_map<state, clazz> expectedAcceptingStates{{0, "id"}};
    state expectedInitialState {0};
    int expectedNumberOfStates {1};

    // Assert
    ASSERT_NE(dfaGen, nullptr);
    EXPECT_EQ(dfaGen->getTransMap(), expectedDfaTransMap);
    EXPECT_EQ(dfaGen->getAcceptingStates(), expectedAcceptingStates);
    EXPECT_EQ(dfaGen->getInitialState(), expectedInitialState);
    EXPECT_EQ(dfaGen->getNumberOfStates(), expectedNumberOfStates);
    EXPECT_EQ(dfaGen->getLanguageSymbols(), symbols);
}

TEST(DFAGeneratorTests, moveNFACorrectly) {
    // Arrange
    set<state> srcStates {
      0, 1, 2, 4, 7
    };
    unordered_map<state, unordered_map<symbol, unordered_set<state>>> nfaTransMap = {
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
    symbol s = 'a';


    // Act
    unordered_set<state> destinationStates = LexicalUtility::makeNfaTransition(srcStates, s, nfaTransMap);

    // expected data
    unordered_set<state> expectedDestinationStates {
        8, 3
    };

    // Assert
    ASSERT_EQ(destinationStates, expectedDestinationStates);
}

TEST(DFAGeneratorTests, unorderedSetToOrderedSetCorrectly) {
    // Arrange
    unordered_set<state> s2 = {98, 12, 57, 23};

    // Act
    set<state> convertedSet = LexicalUtility::unorderedSetToOrderedSet(s2);

    // expected
    set<state> s1 = {23, 12, 98, 57};

    // Assert
    ASSERT_EQ(s1, convertedSet);
}

TEST(DFAGeneratorTests, computeEpsilonClosureCorrectly) {
    // Arrange
    unordered_map<state, unordered_set<state>> nfaEpsilonTransMap = {
        {0, {1, 7}},
        {1, {2, 4}},
        {3, {6}},
        {5, {6}},
        {6, {1, 7}},
    };
    unordered_set<state> originalStates {
        6, 0, 1
    };

    // Act
    unordered_set<state> destinationStates = LexicalUtility::computeEpsilonClosure(originalStates, nfaEpsilonTransMap);

    // expected
    unordered_set<state> expectedDestinationStates{
        0, 1, 6, 7, 2, 4,
    };

    // Assert
    ASSERT_EQ(expectedDestinationStates, destinationStates);
}

TEST(DFAGeneratorTests, computeMinimizeDfaCorrectly) {
    // Arrange
    unordered_map<state, unordered_map<symbol, unordered_set<state>>> nfaTransMap;
    unordered_map<state, unordered_set<state>> nfaEpsilonTransMap;
    unordered_map<state, clazz> nfaAcceptingStates;
    unordered_set<state> nfaInitialStates;
    unordered_set<symbol> symbols;
    DFAGenerator* dfaGen = nullptr;

    nfaAcceptingStates = {
        {2, "id"},
        {4, "id"},
        {1, "id"}
    };
    nfaInitialStates = {0};
    symbols = {'a', 'b',};
    nfaEpsilonTransMap = {};
    nfaTransMap = {
        {
            0,
            {
                {'a', {3}},
                {'b', {1}},
            }
        },
        {
            1,
            {
                {'a', {2}},
                {'b', {5}},
            }
        },
        {
            2,
            {
                {'a', {2}},
                {'b', {5}},
            }
        },
        {
            3,
            {
                {'a', {0}},
                {'b', {4}}
            }
        },
        {
            4,
            {
                {'a', {2}},
                {'b', {5}}
            }
        },
        {
            5,
            {
                {'a', {5}},
                {'b', {5}}
            }
        },
    };

    // Expected
    unordered_map<state, unordered_map<symbol, state>> expectedDfaTransMap {
        {
            0,
            {
                {'a', 0},
                {'b', 1}
            }
        },
        {
            1,
            {
                {'a', 1},
                {'b', 2}
            }
        },
        {
            2,
            {
                {'a', 2},
                {'b', 2}
            }
        }
    };
    state expectedInitialState = 0;
    unordered_map<state, clazz> expectedAcceptingStates = {{1, "id"}};
    int expectedMinimizedDfaStatesCount = 3;

    // Act
    dfaGen = new DFAGenerator(nfaTransMap, nfaEpsilonTransMap, nfaAcceptingStates, nfaInitialStates, symbols);

    // Assert
    ASSERT_EQ(dfaGen->getTransMap(), expectedDfaTransMap);
    ASSERT_EQ(dfaGen->getAcceptingStates(), expectedAcceptingStates);
    ASSERT_EQ(dfaGen->getLanguageSymbols(), symbols);
    ASSERT_EQ(dfaGen->getInitialState(), expectedInitialState);
    ASSERT_EQ(dfaGen->getNumberOfStates(), expectedMinimizedDfaStatesCount);
}

// last example in this article: https://www.gatevidyalay.com/converting-nfa-to-dfa-solved-examples/
TEST(DFAGeneratorTests, computeConvertVeryComplexNfaToDfaCorrectly) {
    // Arrange
    unordered_map<state, unordered_map<symbol, unordered_set<state>>> nfaTransMap;
    unordered_map<state, unordered_set<state>> nfaEpsilonTransMap;
    unordered_map<state, clazz> nfaAcceptingStates;
    unordered_set<state> nfaInitialStates;
    unordered_set<symbol> symbols;
    DFAGenerator* dfaGen = nullptr;

    nfaAcceptingStates = {
        {1, "else"},
    };
    nfaInitialStates = {0};
    symbols = {'a', 'b',};
    nfaEpsilonTransMap = {};
    nfaTransMap = {
        {
            0,
            {
                {'a', {1, 2}},
            }
        },
        {
            1,
            {}
        },
        {
            2,
            {
                {'a', {2, 1}},
                {'b', {2}},
            }
        },
    };

    // Expected
    unordered_map<state, unordered_map<symbol, state>> expectedDfaTransMap {
        {
            0,
            {
                {'a', 1},
            }
        },
        {
            1,
            {
                {'a', 1},
                {'b', 2}
            }
        },
        {
            2,
            {
                {'a', 1},
                {'b', 2}
            }
        },
    };
    state expectedInitialState = 0;
    unordered_map<state, clazz> expectedAcceptingStates = {{1, "else"}};
    int expectedMinimizedDfaStatesCount = 3;

    // Act
    dfaGen = new DFAGenerator(nfaTransMap, nfaEpsilonTransMap, nfaAcceptingStates, nfaInitialStates, symbols);

    // Assert
    ASSERT_EQ(dfaGen->getTransMap(), expectedDfaTransMap);
    ASSERT_EQ(dfaGen->getAcceptingStates(), expectedAcceptingStates);
    ASSERT_EQ(dfaGen->getLanguageSymbols(), symbols);
    ASSERT_EQ(dfaGen->getInitialState(), expectedInitialState);
    ASSERT_EQ(dfaGen->getNumberOfStates(), expectedMinimizedDfaStatesCount);
}