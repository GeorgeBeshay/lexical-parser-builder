#include <gtest/gtest.h>
#include <cstdio>
#include "../src/TransitionTableContainer.h"

TEST(TransitionTableContainerTests, ConvertingSimpleDFACorrectly) {
    // Arrange
    unordered_set<symbol> symbols = {'a', 'b'};
    unordered_map<state, unordered_map<symbol, state>>
            dfaTransMap = {
            {
                    0,
                    {
                            {'a', 0}
                    }
            },
    };
    unordered_map<state, clazz> acceptingStates{{0, "id"}};
    state initialState {0};
    int numberOfStates {1};
    string filePath = "transitionTable.txt";

    // Act
    TransitionTableContainer ttContainer(dfaTransMap, acceptingStates, initialState, numberOfStates, symbols);

    // Expected data
    vector<vector<int>> expectedTransitionTable({{-1, 0}});

    // Assert
    EXPECT_EQ(expectedTransitionTable, ttContainer.getTransitionTable());
    EXPECT_EQ(1, ttContainer.getSymbolToIndexMapper()['a']);
    EXPECT_EQ(0, ttContainer.getSymbolToIndexMapper()['b']);
    EXPECT_EQ(true, ttContainer.writeFile(filePath));

    // Act
    TransitionTableContainer ttContainer2(filePath);

    // Assert
    EXPECT_EQ(ttContainer.getTransitionTable(), ttContainer2.getTransitionTable());
    EXPECT_EQ(ttContainer.getSymbolToIndexMapper(), ttContainer2.getSymbolToIndexMapper());
    EXPECT_EQ(ttContainer.getAcceptingStates(), ttContainer2.getAcceptingStates());
    EXPECT_EQ(ttContainer.getInitialState(), ttContainer2.getInitialState());
    EXPECT_EQ(0, remove(filePath.c_str()));
}

TEST(TransitionTableContainerTests, ConvertingComplexDFACorrectly) {
    // Arrange
    unordered_set<symbol> symbols = {'a', 'b'};
    unordered_map<state, unordered_map<symbol, state>>
            dfaTransMap = {
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
    unordered_map<state, clazz> acceptingStates{{0, "id"}};
    state initialState {0};
    int numberOfStates {5};
    string filePath = "transitionTable.txt";

    // Act
    TransitionTableContainer ttContainer(dfaTransMap, acceptingStates, initialState, numberOfStates, symbols);

    // Expected data
    vector<vector<int>> expectedTransitionTable({
        {1, 2},
        {1, 2},
        {3, 2},
        {4, 2},
        {1, 2}});

    // Assert
    EXPECT_EQ(expectedTransitionTable, ttContainer.getTransitionTable());
    EXPECT_EQ(1, ttContainer.getSymbolToIndexMapper()['a']);
    EXPECT_EQ(0, ttContainer.getSymbolToIndexMapper()['b']);
    EXPECT_EQ(true, ttContainer.writeFile(filePath));

    // Act
    TransitionTableContainer ttContainer2(filePath);

    // Assert
    EXPECT_EQ(ttContainer.getTransitionTable(), ttContainer2.getTransitionTable());
    EXPECT_EQ(ttContainer.getSymbolToIndexMapper(), ttContainer2.getSymbolToIndexMapper());
    EXPECT_EQ(ttContainer.getAcceptingStates(), ttContainer2.getAcceptingStates());
    EXPECT_EQ(ttContainer.getInitialState(), ttContainer2.getInitialState());
    EXPECT_EQ(0, remove(filePath.c_str()));
}