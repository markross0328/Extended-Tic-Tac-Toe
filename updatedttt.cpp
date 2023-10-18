#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cctype>
#include <limits>

class Player
{
public:
    Player(const std::string &full_name, char piece)
        : full_name(full_name), piece(piece), win_count(0), loss_count(0), draw_count(0) {}

    // Gets the full name of the player
    std::string get_full_name() const
    {
        return full_name;
    }

    // Gets the first name of the player
    std::string get_first_name() const
    {
        return full_name.substr(0, full_name.find(' '));
    }

    // Gets the piece assigned to the player
    char get_piece() const
    {
        return piece;
    }

    // Gets the number of games won by the player
    int get_win_count() const
    {
        return win_count;
    }

    // Increments the win count of the player
    void increment_win_count()
    {
        win_count++;
    }

    // Gets the number of games lost by the player
    int get_loss_count() const
    {
        return loss_count;
    }

    // Increments the loss count of the player
    void increment_loss_count()
    {
        loss_count++;
    }

    // Gets the number of games drawn by the player
    int get_draw_count() const
    {
        return draw_count;
    }

    // Increments the draw count of the player
    void increment_draw_count()
    {
        draw_count++;
    }

private:
    std::string full_name;
    char piece;
    int win_count;
    int loss_count;
    int draw_count;
};

class Board
{
public:
    Board(int rows, int columns)
        : rows(rows), columns(columns)
    {
        board.resize(rows, std::vector<char>(columns, ' '));
    }

    void display_board() const;
    bool make_play(const Player &player, int row, int column);
    bool winner(const Player &player);
    bool check_for_tie();
    void display_game_stats(const std::vector<Player> &players, int total_games_played);

private:
    int moves_made = 0;
    int rows;
    int columns;
    std::vector<std::vector<char>> board;
};

//====================================================//
//                                                    //
// Function to validate player's name (Requirement 2) //
//                                                    //
//====================================================//
bool is_valid_name(const std::string &name)
{
    int word_count = 0;
    bool seen_letter = false;
    for (char character : name)
    {
        if (!std::isalpha(character) && character != ' ')
        {
            return false; // Contains invalid character
        }

        if (character == ' ')
        {
            if (seen_letter)
            {
                // We've reached the end of a word
                word_count++;
                seen_letter = false;
            }
            // Otherwise, we're still at the beginning (leading spaces)
        }
        else
        {
            seen_letter = true;
        }
    }

    // If we've seen a letter in the current word, increase word count
    if (seen_letter)
    {
        word_count++;
    }

    // A valid name should consist of exactly two words (first and last names)
    return word_count == 2;
}

//========================================================//
//                                                        //
// Function to get and validate board size (Requirement 4)//
//                                                        //
//========================================================//
void get_board_size(int &rows, int &columns)
{
    while (true)
    {
        std::cout << "\n"
                  << "Please enter the dimension of the board\n";

        std::cout << "Enter the number of rows (4-13) -> ";
        std::cin >> rows;
        std::cout << "\n";
        if (std::cin.fail() || rows < 4 || rows > 13)
        {
            std::cout << "Invalid input. Please enter a number between 4 and 13.\n";
            std::cin.clear();                                                   // clear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard invalid input
            continue;
        }

        std::cout << "Enter the number of columns (4-16) -> ";
        std::cin >> columns;
        std::cout << "\n";
        if (std::cin.fail() || columns < 4 || columns > 16)
        {
            std::cout << "Invalid input. Please enter a number between 4 and 16.\n";
            std::cin.clear();                                                   // clear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard invalid input
            continue;
        }

        break;
    }
}

//======================================================//
//                                                      //
// Function to initialize players (Requirement 1, 2, 3) //
//                                                      //
//======================================================//
void initialize_players(std::vector<Player> &players)
{
    // Logic to initialize players according to the specifications
    int num_players;
    while (true)
    {
        std::cout << "Enter the number of players (3-7): ";
        std::cin >> num_players;

        if (num_players >= 3 && num_players <= 7)
        {
            break;
        }
        else
        {
            std::cout << "Invalid number of players. Please enter a number between 3 and 7.\n";
        }
    }

    for (int i = 0; i < num_players; ++i)
    {
        std::string full_name;

        while (true)
        {
            std::cout << "Enter player " << (i + 1) << "'s full name (first and last name separated by space(s)):";
            std::cin.ignore();
            std::getline(std::cin, full_name);

            if (is_valid_name(full_name))
            {
                char piece = 'a' + i;
                players.push_back(Player(full_name, piece)); // Create new Player with the proper constructor
                break;
            }
            else
            {
                std::cout << "Invalid name format. Please enter a valid name.\n";
            }
        }
    }
}

//==============================================//
//                                              //
// This function takes in the row and columns   //
// and builds the board                         //
//                                              //
//==============================================//
void Board::display_board() const
{
    std::cout << '\n';
    // Print column numbers
    std::cout << " ";
    for (int i = 1; i <= columns; ++i)
    {
        if (i > 9)
        {
            std::cout << "  " << i;
        }
        else
        {
            std::cout << "   " << i;
        }
    }
    std::cout << std::endl;

    // Print separator line
    std::cout << "   ";
    for (int i = 1; i <= columns; ++i)
    {
        std::cout << "--- ";
    }
    std::cout << std::endl;

    // Print rows with row labels and board state
    for (int i = 0; i < rows; ++i)
    {
        char rowLabel = 'A' + i;
        std::cout << rowLabel << " |";
        for (int j = 0; j < columns; ++j)
        {
            std::cout << " " << board[i][j] << " |"; // Displaying the state of the board
        }
        std::cout << " " << rowLabel << std::endl;

        // Print separator line
        std::cout << "   ";
        for (int j = 1; j <= columns; ++j)
        {
            std::cout << "--- ";
        }
        std::cout << std::endl;
    }

    // Print column numbers again
    std::cout << " ";
    for (int i = 1; i <= columns; ++i)
    {
        if (i > 9)
        {
            std::cout << "  " << i;
        }
        else
        {
            std::cout << "   " << i;
        }
    }
    std::cout << std::endl;
}

//=========================================================//
//                                                         //
// Function to handle a player's move (Requirement 10, 11) //
//                                                         //
//=========================================================//
bool Board::make_play(const Player &player, int row, int column)
{
    if (row >= 0 && row < this->rows && column >= 0 && column < this->columns)
    {
        if (this->board[row][column] == ' ')
        {
            this->board[row][column] = player.get_piece();
            moves_made++;
            return true;
        }
        else
        {
            std::cout << "The cell is already occupied. Try a different move." << std::endl;
        }
    }
    else
    {
        std::cout << "Invalid move. Please try again." << std::endl;
    }
    return false;
}

//============================================================//
//                                                            //
// Function to check for a win condition (Requirement 12, 13) //
//                                                            //
//============================================================//
bool Board::winner(const Player &player)
{
    char piece = player.get_piece();
    // Check horizontal wins
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < columns - 2; ++j)
        {
            if (board[i][j] == piece && board[i][j + 1] == piece && board[i][j + 2] == piece)
            {
                board[i][j] = board[i][j + 1] = board[i][j + 2] = toupper(piece);
                return true;
            }
        }
    }

    // Check vertical wins
    for (int j = 0; j < columns; ++j)
    {
        for (int i = 0; i < rows - 2; ++i)
        {
            if (board[i][j] == piece && board[i + 1][j] == piece && board[i + 2][j] == piece)
            {
                board[i][j] = board[i + 1][j] = board[i + 2][j] = toupper(piece);
                return true;
            }
        }
    }

    // Check diagonal wins (from top-left to bottom-right)
    for (int i = 0; i < rows - 2; ++i)
    {
        for (int j = 0; j < columns - 2; ++j)
        {
            if (board[i][j] == piece && board[i + 1][j + 1] == piece && board[i + 2][j + 2] == piece)
            {
                board[i][j] = board[i + 1][j + 1] = board[i + 2][j + 2] = toupper(piece);
                return true;
            }
        }
    }

    // Check diagonal wins (from top-right to bottom-left)
    for (int i = 0; i < rows - 2; ++i)
    {
        for (int j = 2; j < columns; ++j)
        {
            if (board[i][j] == piece && board[i + 1][j - 1] == piece && board[i + 2][j - 2] == piece)
            {
                board[i][j] = board[i + 1][j - 1] = board[i + 2][j - 2] = toupper(piece);
                return true;
            }
        }
    }
    return false;
}

//========================================================//
//                                                        //
// Function to check for a draw condition (Requirement 16)//
//                                                        //
//========================================================//
bool Board::check_for_tie()
{
    return moves_made == (this->rows * this->columns);
}

//=====================================================//
//                                                     //
// Function to display game statistics (Requirement 14)//
//                                                     //
//=====================================================//
void Board::display_game_stats(const std::vector<Player> &players, int total_games_played)
{
    std::cout << '\n';
    std::cout << "Total game played = " << total_games_played << '\n';
    std::cout << std::setw(30) << " "
              << "- - - - - -"
              << " "
              << "- - - - - -"
              << " "
              << "- - - - - -" << '\n';
    std::cout << std::setw(29) << " "
              << "|    WIN    |"
              << "    LOSS   "
              << "|    DRAW   |" << '\n';
    std::cout << std::setw(30) << " "
              << "- - - - - -"
              << " "
              << "- - - - - -"
              << " "
              << "- - - - - -" << '\n';

    for (const auto &player : players)
    {
        std::cout << std::setw(28) << std::right << player.get_full_name() << " "
                  << "|" << std::setw(9) << " " << player.get_win_count() << " "
                  << "|" << std::setw(9) << " " << player.get_loss_count() << " "
                  << "|" << std::setw(9) << " " << player.get_draw_count() << " "
                  << "|" << '\n';
        std::cout << std::setw(30) << " "
                  << "- - - - - -"
                  << " "
                  << "- - - - - -"
                  << " "
                  << "- - - - - -" << '\n';
    }
}

//=================================//
//                                 //
// Function to get the next player //
//                                 //
//=================================//
char get_next_player_piece(char currentPiece, const std::vector<Player> &players)
{
    const std::string play_order = "abcdefg";
    size_t current_index = play_order.find(currentPiece);

    if (current_index != std::string::npos)
    {
        size_t next_index = (current_index + 1) % players.size();
        if (next_index < play_order.size())
        {
            for (const Player &player : players)
            {
                if (player.get_piece() == play_order[next_index])
                {
                    return play_order[next_index];
                }
            }

            for (size_t i = 0; i < play_order.size(); ++i)
            {
                for (const Player &player : players)
                {
                    if (player.get_piece() == play_order[i])
                    {
                        return play_order[i];
                    }
                }
            }
        }
    }

    return ' ';
}

//======================================================//
//                                                      //
// Function to update losers stats                      //
//                                                      //
//======================================================//
void update_other_player_stats(std::vector<Player> &players, char winningPiece, const std::string &result_type)
{
    for (Player &player : players)
    {
        if (player.get_piece() != winningPiece)
        {
            if (result_type == "loss")
            {
                player.increment_loss_count();
            }
            else
            {
                player.increment_draw_count();
            }
        }
    }
}

//====================================//
//                                    //
// Function to update draw case stats //
//                                    //
//====================================//
void update_all_player_stats(std::vector<Player> &players, const std::string &result_type)
{
    for (Player &player : players)
    {
        if (result_type == "draw")
        {
            player.increment_draw_count();
        }
    }
}

//============================================================================================//
//                                                                                            //
//   The main function initializes the game, takes player names, and manages the game loop.   //
//                                                                                            //
//============================================================================================//
int main()
{
    std::vector<Player> players;
    Board game_board(4, 4);
    int total_games_played = 0;

    std::cout << std::setw(30) << "Welcome to this extended 3-7 multiplayer tic-tac-toe game\n\n";

    initialize_players(players);

    while (true)
    {
        int rows = 0, columns = 0;
        get_board_size(rows, columns);
        game_board = Board(rows, columns);

        // game_board.display_board();
        std::cout << "\n";

        char currentPlayerPiece = 'c';
        std::string move_input;

        bool game_won = false;
        while (!game_won && !game_board.check_for_tie())
        {
            for (Player &player : players)
            {
                if (player.get_piece() == currentPlayerPiece)
                {
                    while (true)
                    {
                        game_board.display_board();

                        std::cout << "\n"
                                  << player.get_first_name() << " with peice " << player.get_piece() << ", enter your move (row and column w/ no space in between): ";
                        std::cin >> move_input;

                        if (move_input.size() < 2)
                        {
                            std::cout << "Invalid input. Please enter a valid move." << std::endl;
                            continue;
                        }

                        int row = toupper(move_input[0]) - 'A';
                        int column = std::stoi(move_input.substr(1)) - 1;

                        if (game_board.make_play(player, row, column))
                        {
                            break;
                        }
                    }

                    if (game_board.winner(player))
                    {
                        game_won = true;
                        game_board.display_board();
                        std::cout << player.get_full_name() << " Wins!!" << std::endl;
                        player.increment_win_count();
                        update_other_player_stats(players, player.get_piece(), "loss");
                        break;
                    }
                    else if (game_board.check_for_tie())
                    {
                        game_board.display_board();
                        std::cout << "It's a Draw!!" << std::endl;
                        update_all_player_stats(players, "draw");
                        break;
                    }

                    currentPlayerPiece = get_next_player_piece(currentPlayerPiece, players);
                }
            }
        }

        total_games_played++;

        game_board.display_game_stats(players, total_games_played);

        char choice;
        std::cout << "Do you want to play another game? (y/n): ";
        std::cin >> choice;
        if (choice == 'n' || choice == 'N')
        {
            std::cout << std::endl
                      << "Thanks for playing! :) I appreciate your time" << std::endl;
            break;
        }
    }

    std::cout << '\n';
    std::string delayEndGame;
    std::cin >> delayEndGame;

    return 0;
}
