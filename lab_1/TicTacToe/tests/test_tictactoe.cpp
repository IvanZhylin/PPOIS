#include <gtest/gtest.h>
#include <sstream>
#include <memory>
#include "../include/TicTacToe.h"

// ============================================================
// Mock классы для тестирования
// ============================================================

/**
 * @brief Mock класс для InputHandler
 */
class MockInputHandler : public InputHandler {
private:
    std::vector<std::pair<int, int>> moves_;
    size_t currentMove_;
    int boardSize_;

public:
    MockInputHandler(int boardSize = 3) 
        : currentMove_(0), boardSize_(boardSize) {}

    void addMove(int row, int col) {
        moves_.push_back({row, col});
    }

    int getBoardSize() override {
        return boardSize_;
    }

    bool getPlayerMove(Player currentPlayer, int& row, int& col) override {
        if (currentMove_ >= moves_.size()) {
            return false;
        }
        row = moves_[currentMove_].first;
        col = moves_[currentMove_].second;
        currentMove_++;
        return true;
    }

    void reset() {
        currentMove_ = 0;
    }
};

/**
 * @brief Mock класс для OutputHandler
 */
class MockOutputHandler : public OutputHandler {
public:
    int displayBoardCount = 0;
    int displayWinnerCount = 0;
    int displayDrawCount = 0;
    int displayErrorCount = 0;
    Player lastWinner = Player::NONE;
    std::string lastError;

    void displayBoard() const override {
        const_cast<MockOutputHandler*>(this)->displayBoardCount++;
    }

    void displayWelcome() const override {}

    void displayWinner(Player player) const override {
        const_cast<MockOutputHandler*>(this)->displayWinnerCount++;
        const_cast<MockOutputHandler*>(this)->lastWinner = player;
    }

    void displayDraw() const override {
        const_cast<MockOutputHandler*>(this)->displayDrawCount++;
    }

    void displayError(const std::string& message) const override {
        const_cast<MockOutputHandler*>(this)->displayErrorCount++;
        const_cast<MockOutputHandler*>(this)->lastError = message;
    }

    void displayCurrentPlayer(Player player) const override {}

    void reset() {
        displayBoardCount = 0;
        displayWinnerCount = 0;
        displayDrawCount = 0;
        displayErrorCount = 0;
        lastWinner = Player::NONE;
        lastError.clear();
    }
};

// ============================================================
// Тесты GameBoard
// ============================================================

class GameBoardTest : public ::testing::Test {
protected:
    std::shared_ptr<GameBoard> board;

    void SetUp() override {
        board = std::make_shared<GameBoard>();
    }
};

TEST_F(GameBoardTest, InitializeBoard) {
    board->initialize(3);
    EXPECT_EQ(board->getSize(), 3);
    EXPECT_EQ(board->getCell(0, 0), Player::NONE);
    EXPECT_EQ(board->getCell(2, 2), Player::NONE);
}

TEST_F(GameBoardTest, InitializeLargeBoard) {
    board->initialize(5);
    EXPECT_EQ(board->getSize(), 5);
    EXPECT_EQ(board->getCell(4, 4), Player::NONE);
}

TEST_F(GameBoardTest, IsValidMoveEmpty) {
    board->initialize(3);
    EXPECT_TRUE(board->isValidMove(0, 0));
    EXPECT_TRUE(board->isValidMove(1, 1));
    EXPECT_TRUE(board->isValidMove(2, 2));
}

TEST_F(GameBoardTest, IsValidMoveOutOfBounds) {
    board->initialize(3);
    EXPECT_FALSE(board->isValidMove(-1, 0));
    EXPECT_FALSE(board->isValidMove(0, -1));
    EXPECT_FALSE(board->isValidMove(3, 0));
    EXPECT_FALSE(board->isValidMove(0, 3));
    EXPECT_FALSE(board->isValidMove(5, 5));
}

TEST_F(GameBoardTest, IsValidMoveOccupied) {
    board->initialize(3);
    board->makeMove(1, 1, Player::X);
    EXPECT_FALSE(board->isValidMove(1, 1));
}

TEST_F(GameBoardTest, MakeMoveValid) {
    board->initialize(3);
    EXPECT_TRUE(board->makeMove(0, 0, Player::X));
    EXPECT_EQ(board->getCell(0, 0), Player::X);
}

TEST_F(GameBoardTest, MakeMoveInvalid) {
    board->initialize(3);
    board->makeMove(1, 1, Player::X);
    EXPECT_FALSE(board->makeMove(1, 1, Player::O));
    EXPECT_EQ(board->getCell(1, 1), Player::X);
}

TEST_F(GameBoardTest, MakeMoveOutOfBounds) {
    board->initialize(3);
    EXPECT_FALSE(board->makeMove(-1, 0, Player::X));
    EXPECT_FALSE(board->makeMove(3, 3, Player::O));
}

TEST_F(GameBoardTest, IsFullEmpty) {
    board->initialize(3);
    EXPECT_FALSE(board->isFull());
}

TEST_F(GameBoardTest, IsFullPartial) {
    board->initialize(3);
    board->makeMove(0, 0, Player::X);
    board->makeMove(1, 1, Player::O);
    EXPECT_FALSE(board->isFull());
}

TEST_F(GameBoardTest, IsFullComplete) {
    board->initialize(3);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            board->makeMove(i, j, (i + j) % 2 == 0 ? Player::X : Player::O);
        }
    }
    EXPECT_TRUE(board->isFull());
}

TEST_F(GameBoardTest, GetPlayerSymbol) {
    EXPECT_EQ(board->getPlayerSymbol(Player::X), 'X');
    EXPECT_EQ(board->getPlayerSymbol(Player::O), 'O');
    EXPECT_EQ(board->getPlayerSymbol(Player::NONE), '-');
}

TEST_F(GameBoardTest, GetCellValid) {
    board->initialize(3);
    board->makeMove(1, 2, Player::X);
    EXPECT_EQ(board->getCell(1, 2), Player::X);
}

TEST_F(GameBoardTest, GetCellInvalid) {
    board->initialize(3);
    EXPECT_EQ(board->getCell(-1, 0), Player::NONE);
    EXPECT_EQ(board->getCell(0, -1), Player::NONE);
    EXPECT_EQ(board->getCell(3, 3), Player::NONE);
}

// ============================================================
// Тесты StandardGameLogic
// ============================================================

class StandardGameLogicTest : public ::testing::Test {
protected:
    std::shared_ptr<GameBoard> board;
    std::shared_ptr<StandardGameLogic> logic;

    void SetUp() override {
        board = std::make_shared<GameBoard>();
        board->initialize(3);
        logic = std::make_shared<StandardGameLogic>(board);
    }
};

TEST_F(StandardGameLogicTest, GetWinCondition) {
    EXPECT_EQ(logic->getWinCondition(), 3);
}

TEST_F(StandardGameLogicTest, CheckWinHorizontal) {
    board->makeMove(0, 0, Player::X);
    board->makeMove(0, 1, Player::X);
    board->makeMove(0, 2, Player::X);
    EXPECT_TRUE(logic->checkWin(0, 2, Player::X));
}

TEST_F(StandardGameLogicTest, CheckWinVertical) {
    board->makeMove(0, 0, Player::O);
    board->makeMove(1, 0, Player::O);
    board->makeMove(2, 0, Player::O);
    EXPECT_TRUE(logic->checkWin(2, 0, Player::O));
}

TEST_F(StandardGameLogicTest, CheckWinDiagonalTopLeftBottomRight) {
    board->makeMove(0, 0, Player::X);
    board->makeMove(1, 1, Player::X);
    board->makeMove(2, 2, Player::X);
    EXPECT_TRUE(logic->checkWin(1, 1, Player::X));
}

TEST_F(StandardGameLogicTest, CheckWinDiagonalTopRightBottomLeft) {
    board->makeMove(0, 2, Player::O);
    board->makeMove(1, 1, Player::O);
    board->makeMove(2, 0, Player::O);
    EXPECT_TRUE(logic->checkWin(1, 1, Player::O));
}

TEST_F(StandardGameLogicTest, CheckWinNoWin) {
    board->makeMove(0, 0, Player::X);
    board->makeMove(0, 1, Player::X);
    board->makeMove(1, 0, Player::O);
    EXPECT_FALSE(logic->checkWin(0, 1, Player::X));
}

TEST_F(StandardGameLogicTest, CheckWinMixedPlayers) {
    board->makeMove(0, 0, Player::X);
    board->makeMove(0, 1, Player::O);
    board->makeMove(0, 2, Player::X);
    EXPECT_FALSE(logic->checkWin(0, 2, Player::X));
}

TEST_F(StandardGameLogicTest, CheckWinLargerBoard) {
    board->initialize(5);
    logic = std::make_shared<StandardGameLogic>(board);
    
    board->makeMove(0, 0, Player::X);
    board->makeMove(0, 1, Player::X);
    board->makeMove(0, 2, Player::X);
    EXPECT_TRUE(logic->checkWin(0, 1, Player::X));
}

TEST_F(StandardGameLogicTest, CheckWinMiddleOfLine) {
    board->makeMove(1, 0, Player::O);
    board->makeMove(1, 1, Player::O);
    board->makeMove(1, 2, Player::O);
    EXPECT_TRUE(logic->checkWin(1, 1, Player::O));
}

// ============================================================
// Тесты ConsoleInputHandler - ИСПРАВЛЕНО
// ============================================================

class ConsoleInputHandlerTest : public ::testing::Test {
protected:
    std::shared_ptr<GameBoard> board;
    std::shared_ptr<ConsoleInputHandler> inputHandler;
    std::stringstream simulatedInput;
    std::stringstream capturedOutput;
    std::streambuf* oldCinBuffer;
    std::streambuf* oldCoutBuffer;

    void SetUp() override {
        board = std::make_shared<GameBoard>();
        board->initialize(3);
        inputHandler = std::make_shared<ConsoleInputHandler>(board);
        
        // Перехват std::cin и std::cout
        oldCinBuffer = std::cin.rdbuf();
        oldCoutBuffer = std::cout.rdbuf();
        std::cin.rdbuf(simulatedInput.rdbuf());
        std::cout.rdbuf(capturedOutput.rdbuf());
    }

    void TearDown() override {
        std::cin.rdbuf(oldCinBuffer);
        std::cout.rdbuf(oldCoutBuffer);
    }
    
    void simulateInput(const std::string& input) {
        simulatedInput.str(input);
        simulatedInput.clear();
    }
};

TEST_F(ConsoleInputHandlerTest, GetBoardSizeValid) {
    simulateInput("3\n");
    int size = inputHandler->getBoardSize();
    EXPECT_EQ(size, 3);
}

TEST_F(ConsoleInputHandlerTest, GetBoardSizeLarge) {
    simulateInput("10\n");
    int size = inputHandler->getBoardSize();
    EXPECT_EQ(size, 10);
}

TEST_F(ConsoleInputHandlerTest, GetBoardSizeInvalidThenValid) {
    simulateInput("2\n5\n"); // Сначала 2 (неверно), потом 5 (верно)
    int size = inputHandler->getBoardSize();
    EXPECT_EQ(size, 5);
}

TEST_F(ConsoleInputHandlerTest, GetBoardSizeNonNumericThenValid) {
    simulateInput("abc\n4\n"); // Сначала текст, потом число
    int size = inputHandler->getBoardSize();
    EXPECT_EQ(size, 4);
}

TEST_F(ConsoleInputHandlerTest, GetBoardSizeZeroThenValid) {
    simulateInput("0\n3\n");
    int size = inputHandler->getBoardSize();
    EXPECT_EQ(size, 3);
}

TEST_F(ConsoleInputHandlerTest, GetPlayerMoveValid) {
    simulateInput("1 1\n");
    int row, col;
    bool result = inputHandler->getPlayerMove(Player::X, row, col);
    EXPECT_TRUE(result);
    EXPECT_EQ(row, 0); // Индексы начинаются с 0
    EXPECT_EQ(col, 0);
}

TEST_F(ConsoleInputHandlerTest, GetPlayerMoveValidDifferentPosition) {
    simulateInput("2 3\n");
    int row, col;
    bool result = inputHandler->getPlayerMove(Player::O, row, col);
    EXPECT_TRUE(result);
    EXPECT_EQ(row, 1);
    EXPECT_EQ(col, 2);
}

TEST_F(ConsoleInputHandlerTest, GetPlayerMoveInvalidFormat) {
    simulateInput("abc\n");
    int row, col;
    bool result = inputHandler->getPlayerMove(Player::X, row, col);
    EXPECT_FALSE(result);
}

TEST_F(ConsoleInputHandlerTest, GetPlayerMoveInvalidFormatSingleNumber) {
    simulateInput("1\n");
    int row, col;
    bool result = inputHandler->getPlayerMove(Player::X, row, col);
    EXPECT_FALSE(result);
}

TEST_F(ConsoleInputHandlerTest, GetPlayerMoveInvalidFormatTooMany) {
    simulateInput("1 2 3\n");
    int row, col;
    bool result = inputHandler->getPlayerMove(Player::X, row, col);
    EXPECT_FALSE(result);
}

TEST_F(ConsoleInputHandlerTest, GetPlayerMoveOutOfBounds) {
    simulateInput("10 10\n"); // За пределами доски 3x3
    int row, col;
    bool result = inputHandler->getPlayerMove(Player::X, row, col);
    EXPECT_FALSE(result);
}

TEST_F(ConsoleInputHandlerTest, GetPlayerMoveNegativeCoordinates) {
    simulateInput("-1 1\n");
    int row, col;
    bool result = inputHandler->getPlayerMove(Player::X, row, col);
    EXPECT_FALSE(result);
}

TEST_F(ConsoleInputHandlerTest, GetPlayerMoveOccupiedCell) {
    board->makeMove(0, 0, Player::X);
    simulateInput("1 1\n"); // Клетка уже занята
    int row, col;
    bool result = inputHandler->getPlayerMove(Player::O, row, col);
    EXPECT_FALSE(result);
}

TEST_F(ConsoleInputHandlerTest, GetPlayerMoveEdgeOfBoard) {
    simulateInput("3 3\n"); // Угол доски 3x3
    int row, col;
    bool result = inputHandler->getPlayerMove(Player::X, row, col);
    EXPECT_TRUE(result);
    EXPECT_EQ(row, 2);
    EXPECT_EQ(col, 2);
}

TEST_F(ConsoleInputHandlerTest, GetPlayerMoveZeroCoordinates) {
    simulateInput("0 0\n"); // 0 за пределами (индексы от 1)
    int row, col;
    bool result = inputHandler->getPlayerMove(Player::X, row, col);
    EXPECT_FALSE(result);
}

TEST_F(ConsoleInputHandlerTest, GetPlayerMoveWithSpaces) {
    simulateInput("2  2\n"); // Множественные пробелы
    int row, col;
    bool result = inputHandler->getPlayerMove(Player::X, row, col);
    EXPECT_TRUE(result);
}

TEST_F(ConsoleInputHandlerTest, GetPlayerMoveEmptyInput) {
    simulateInput("\n");
    int row, col;
    bool result = inputHandler->getPlayerMove(Player::X, row, col);
    EXPECT_FALSE(result);
}

TEST_F(ConsoleInputHandlerTest, GetPlayerMoveLargeBoard) {
    board->initialize(10);
    simulateInput("5 5\n");
    int row, col;
    bool result = inputHandler->getPlayerMove(Player::X, row, col);
    EXPECT_TRUE(result);
    EXPECT_EQ(row, 4);
    EXPECT_EQ(col, 4);
}


// ============================================================
// Тесты ConsoleOutputHandler
// ============================================================

class ConsoleOutputHandlerTest : public ::testing::Test {
protected:
    std::shared_ptr<GameBoard> board;
    std::shared_ptr<ConsoleOutputHandler> outputHandler;
    std::stringstream capturedOutput;
    std::streambuf* oldCoutBuffer;

    void SetUp() override {
        board = std::make_shared<GameBoard>();
        board->initialize(3);
        outputHandler = std::make_shared<ConsoleOutputHandler>(board);
        
        // Перехват std::cout
        oldCoutBuffer = std::cout.rdbuf();
        std::cout.rdbuf(capturedOutput.rdbuf());
    }

    void TearDown() override {
        // Восстановление std::cout
        std::cout.rdbuf(oldCoutBuffer);
    }

    std::string getOutput() {
        return capturedOutput.str();
    }

    void clearOutput() {
        capturedOutput.str("");
        capturedOutput.clear();
    }
};

TEST_F(ConsoleOutputHandlerTest, DisplayBoard) {
    outputHandler->displayBoard();
    std::string output = getOutput();
    EXPECT_FALSE(output.empty());
    EXPECT_NE(output.find("1"), std::string::npos);
    EXPECT_NE(output.find("-"), std::string::npos);
}

TEST_F(ConsoleOutputHandlerTest, DisplayBoardWithMoves) {
    board->makeMove(0, 0, Player::X);
    board->makeMove(1, 1, Player::O);
    outputHandler->displayBoard();
    std::string output = getOutput();
    EXPECT_NE(output.find("X"), std::string::npos);
    EXPECT_NE(output.find("O"), std::string::npos);
}

TEST_F(ConsoleOutputHandlerTest, DisplayWelcome) {
    outputHandler->displayWelcome();
    std::string output = getOutput();
    EXPECT_NE(output.find("Добро пожаловать"), std::string::npos);
}

TEST_F(ConsoleOutputHandlerTest, DisplayWinner) {
    outputHandler->displayWinner(Player::X);
    std::string output = getOutput();
    EXPECT_NE(output.find("X"), std::string::npos);
    EXPECT_NE(output.find("победил"), std::string::npos);
}

TEST_F(ConsoleOutputHandlerTest, DisplayDraw) {
    outputHandler->displayDraw();
    std::string output = getOutput();
    EXPECT_NE(output.find("Ничья"), std::string::npos);
}

TEST_F(ConsoleOutputHandlerTest, DisplayError) {
    outputHandler->displayError("Test error");
    std::string output = getOutput();
    EXPECT_NE(output.find("Ошибка"), std::string::npos);
    EXPECT_NE(output.find("Test error"), std::string::npos);
}

TEST_F(ConsoleOutputHandlerTest, DisplayCurrentPlayer) {
    outputHandler->displayCurrentPlayer(Player::X);
    std::string output = getOutput();
    EXPECT_NE(output.find("X"), std::string::npos);
}

// ============================================================
// Тесты Game (интеграционные)
// ============================================================

class GameTest : public ::testing::Test {
protected:
    std::shared_ptr<GameBoard> board;
    std::shared_ptr<StandardGameLogic> logic;
    std::shared_ptr<MockInputHandler> inputHandler;
    std::shared_ptr<MockOutputHandler> outputHandler;
    std::shared_ptr<Game> game;
    std::stringstream capturedOutput;
    std::streambuf* oldCoutBuffer;

    void SetUp() override {
        board = std::make_shared<GameBoard>();
        logic = std::make_shared<StandardGameLogic>(board);
        inputHandler = std::make_shared<MockInputHandler>(3);
        outputHandler = std::make_shared<MockOutputHandler>();
        game = std::make_shared<Game>(board, logic, inputHandler, outputHandler);
        
        // Перехват std::cout для некоторых тестов
        oldCoutBuffer = std::cout.rdbuf();
        std::cout.rdbuf(capturedOutput.rdbuf());
    }

    void TearDown() override {
        std::cout.rdbuf(oldCoutBuffer);
    }
};

TEST_F(GameTest, GameConstructor) {
    EXPECT_NE(game, nullptr);
}

TEST_F(GameTest, GameWinScenarioPlayerX) {
    // X выигрывает по горизонтали
    inputHandler->addMove(0, 0); // X
    inputHandler->addMove(1, 0); // O
    inputHandler->addMove(0, 1); // X
    inputHandler->addMove(1, 1); // O
    inputHandler->addMove(0, 2); // X wins
    
    game->run();
    
    EXPECT_EQ(outputHandler->displayWinnerCount, 1);
    EXPECT_EQ(outputHandler->lastWinner, Player::X);
    EXPECT_GT(outputHandler->displayBoardCount, 0);
}

TEST_F(GameTest, GameWinScenarioPlayerO) {
    // O выигрывает по вертикали
    inputHandler->addMove(0, 0); // X
    inputHandler->addMove(1, 0); // O
    inputHandler->addMove(0, 1); // X
    inputHandler->addMove(1, 1); // O
    inputHandler->addMove(2, 2); // X
    inputHandler->addMove(1, 2); // O wins
    
    game->run();
    
    EXPECT_EQ(outputHandler->displayWinnerCount, 1);
    EXPECT_EQ(outputHandler->lastWinner, Player::O);
}

TEST_F(GameTest, GameDrawScenario) {
    // Ничья - заполняем доску без победителя
    inputHandler->addMove(0, 0); // X
    inputHandler->addMove(0, 1); // O
    inputHandler->addMove(0, 2); // X
    inputHandler->addMove(1, 0); // O
    inputHandler->addMove(1, 1); // X
    inputHandler->addMove(2, 0); // O
    inputHandler->addMove(1, 2); // X
    inputHandler->addMove(2, 2); // O
    inputHandler->addMove(2, 1); // X
    
    game->run();
    
    EXPECT_EQ(outputHandler->displayDrawCount, 1);
    EXPECT_EQ(outputHandler->displayWinnerCount, 0);
}

TEST_F(GameTest, GameDiagonalWinTopLeftBottomRight) {
    // X выигрывает по диагонали (верхний левый -> нижний правый)
    inputHandler->addMove(0, 0); // X
    inputHandler->addMove(0, 1); // O
    inputHandler->addMove(1, 1); // X
    inputHandler->addMove(0, 2); // O
    inputHandler->addMove(2, 2); // X wins
    
    game->run();
    
    EXPECT_EQ(outputHandler->displayWinnerCount, 1);
    EXPECT_EQ(outputHandler->lastWinner, Player::X);
}

TEST_F(GameTest, GameDiagonalWinTopRightBottomLeft) {
    // O выигрывает по диагонали (верхний правый -> нижний левый)
    inputHandler->addMove(0, 0); // X
    inputHandler->addMove(0, 2); // O
    inputHandler->addMove(0, 1); // X
    inputHandler->addMove(1, 1); // O
    inputHandler->addMove(2, 1); // X
    inputHandler->addMove(2, 0); // O wins
    
    game->run();
    
    EXPECT_EQ(outputHandler->displayWinnerCount, 1);
    EXPECT_EQ(outputHandler->lastWinner, Player::O);
}




TEST_F(GameTest, GameHandlesInvalidMoveGracefully) {
    // Первый ход валидный, второй невалидный (занятая клетка), третий валидный
    inputHandler->addMove(0, 0); // X - валидный
    inputHandler->addMove(0, 0); // O - невалидный (занято)
    inputHandler->addMove(0, 1); // O - валидный после повтора
    
    game->run();
    
    // Проверяем, что первый ход X прошел
    EXPECT_EQ(board->getCell(0, 0), Player::X);
}


TEST_F(GameTest, GameEndsAfterWin) {
    inputHandler->addMove(0, 0); // X
    inputHandler->addMove(1, 0); // O
    inputHandler->addMove(0, 1); // X
    inputHandler->addMove(1, 1); // O
    inputHandler->addMove(0, 2); // X wins
    inputHandler->addMove(2, 2); // Этот ход не должен выполниться
    
    game->run();
    
    EXPECT_EQ(outputHandler->displayWinnerCount, 1);
    EXPECT_EQ(board->getCell(2, 2), Player::NONE); // Последний ход не выполнился
}

TEST_F(GameTest, GameVerticalWin) {
    inputHandler->addMove(0, 0); // X
    inputHandler->addMove(0, 1); // O
    inputHandler->addMove(1, 0); // X
    inputHandler->addMove(1, 1); // O
    inputHandler->addMove(2, 0); // X wins vertically
    
    game->run();
    
    EXPECT_EQ(outputHandler->displayWinnerCount, 1);
    EXPECT_EQ(outputHandler->lastWinner, Player::X);
}

TEST_F(GameTest, GameHorizontalWin) {
    inputHandler->addMove(0, 0); // X
    inputHandler->addMove(1, 0); // O
    inputHandler->addMove(0, 1); // X
    inputHandler->addMove(1, 1); // O
    inputHandler->addMove(0, 2); // X wins horizontally
    
    game->run();
    
    EXPECT_EQ(outputHandler->displayWinnerCount, 1);
    EXPECT_EQ(outputHandler->lastWinner, Player::X);
}


// ============================================================
// Тесты граничных случаев
// ============================================================

TEST(EdgeCaseTest, MinimumBoardSize) {
    auto board = std::make_shared<GameBoard>();
    board->initialize(3);
    EXPECT_EQ(board->getSize(), 3);
}

TEST(EdgeCaseTest, LargeBoardSize) {
    auto board = std::make_shared<GameBoard>();
    board->initialize(10);
    EXPECT_EQ(board->getSize(), 10);
    EXPECT_EQ(board->getCell(9, 9), Player::NONE);
}

TEST(EdgeCaseTest, WinOnLastMove) {
    auto board = std::make_shared<GameBoard>();
    board->initialize(3);
    auto logic = std::make_shared<StandardGameLogic>(board);
    
    // Заполняем доску, оставляя последнюю клетку для победы
    board->makeMove(0, 0, Player::X);
    board->makeMove(0, 1, Player::X);
    board->makeMove(1, 0, Player::O);
    board->makeMove(1, 1, Player::O);
    board->makeMove(2, 1, Player::X);
    board->makeMove(2, 2, Player::O);
    board->makeMove(0, 2, Player::X); // X wins
    
    EXPECT_TRUE(logic->checkWin(0, 2, Player::X));
}

// ============================================================
// Main функция
// ============================================================

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
