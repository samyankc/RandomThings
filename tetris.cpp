#include <windows.h>
#include <conio.h>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <array>
#include <cstdlib>
#include <initializer_list>

using std::array, std::cout, std::endl;

constexpr static auto BoardWidth        = 10;
constexpr static auto BoardHeight       = 16;
constexpr static auto ActualBoardHeight = BoardHeight + 4;

using Block = bool;

struct TetrominoConstant : array<array<Block, 4>, 4>
{
    constexpr TetrominoConstant( std::initializer_list<Block> Source )
        : array<array<Block, 4>, 4>{}
    {
        auto it = Source.begin();
        for( auto y = 4; y-- > 0; )
            for( auto x = 0; x < 4; ++x ) ( *this )[ x ][ y ] = *it++;
    }
};

constexpr static array<TetrominoConstant, 4>                     //
TetrominoConstantVariation_I = { TetrominoConstant{ 0, 0, 0, 0,  //
                                                    1, 1, 1, 1,  //
                                                    0, 0, 0, 0,  //
                                                    0, 0, 0, 0 },
                                 TetrominoConstant{ 0, 1, 0, 0,  //
                                                    0, 1, 0, 0,  //
                                                    0, 1, 0, 0,  //
                                                    0, 1, 0, 0 },
                                 TetrominoConstant{ 0, 0, 0, 0,  //
                                                    1, 1, 1, 1,  //
                                                    0, 0, 0, 0,  //
                                                    0, 0, 0, 0 },
                                 TetrominoConstant{ 0, 1, 0, 0,  //
                                                    0, 1, 0, 0,  //
                                                    0, 1, 0, 0,  //
                                                    0, 1, 0, 0 } },
TetrominoConstantVariation_J = { TetrominoConstant{ 0, 1, 0, 0,  //
                                                    0, 1, 1, 1,  //
                                                    0, 0, 0, 0,  //
                                                    0, 0, 0, 0 },
                                 TetrominoConstant{ 0, 1, 1, 0,  //
                                                    0, 1, 0, 0,  //
                                                    0, 1, 0, 0,  //
                                                    0, 0, 0, 0 },
                                 TetrominoConstant{ 1, 1, 1, 0,  //
                                                    0, 0, 1, 0,  //
                                                    0, 0, 0, 0,  //
                                                    0, 0, 0, 0 },
                                 TetrominoConstant{ 0, 0, 1, 0,  //
                                                    0, 0, 1, 0,  //
                                                    0, 1, 1, 0,  //
                                                    0, 0, 0, 0 } },
TetrominoConstantVariation_L = { TetrominoConstant{ 0, 0, 1, 0,  //
                                                    1, 1, 1, 0,  //
                                                    0, 0, 0, 0,  //
                                                    0, 0, 0, 0 },
                                 TetrominoConstant{ 0, 1, 0, 0,  //
                                                    0, 1, 0, 0,  //
                                                    0, 1, 1, 0,  //
                                                    0, 0, 0, 0 },
                                 TetrominoConstant{ 0, 1, 1, 1,  //
                                                    0, 1, 0, 0,  //
                                                    0, 0, 0, 0,  //
                                                    0, 0, 0, 0 },
                                 TetrominoConstant{ 0, 1, 1, 0,  //
                                                    0, 0, 1, 0,  //
                                                    0, 0, 1, 0,  //
                                                    0, 0, 0, 0 } },
TetrominoConstantVariation_S = { TetrominoConstant{ 0, 1, 1, 0,  //
                                                    1, 1, 0, 0,  //
                                                    0, 0, 0, 0,  //
                                                    0, 0, 0, 0 },
                                 TetrominoConstant{ 0, 1, 0, 0,  //
                                                    0, 1, 1, 0,  //
                                                    0, 0, 1, 0,  //
                                                    0, 0, 0, 0 },
                                 TetrominoConstant{ 0, 1, 1, 0,  //
                                                    1, 1, 0, 0,  //
                                                    0, 0, 0, 0,  //
                                                    0, 0, 0, 0 },
                                 TetrominoConstant{ 0, 1, 0, 0,  //
                                                    0, 1, 1, 0,  //
                                                    0, 0, 1, 0,  //
                                                    0, 0, 0, 0 } },
TetrominoConstantVariation_Z = { TetrominoConstant{ 0, 1, 1, 0,  //
                                                    0, 0, 1, 1,  //
                                                    0, 0, 0, 0,  //
                                                    0, 0, 0, 0 },
                                 TetrominoConstant{ 0, 0, 1, 0,  //
                                                    0, 1, 1, 0,  //
                                                    0, 1, 0, 0,  //
                                                    0, 0, 0, 0 },
                                 TetrominoConstant{ 0, 1, 1, 0,  //
                                                    0, 0, 1, 1,  //
                                                    0, 0, 0, 0,  //
                                                    0, 0, 0, 0 },
                                 TetrominoConstant{ 0, 0, 1, 0,  //
                                                    0, 1, 1, 0,  //
                                                    0, 1, 0, 0,  //
                                                    0, 0, 0, 0 } },
TetrominoConstantVariation_O = { TetrominoConstant{ 0, 1, 1, 0,  //
                                                    0, 1, 1, 0,  //
                                                    0, 0, 0, 0,  //
                                                    0, 0, 0, 0 },
                                 TetrominoConstant{ 0, 1, 1, 0,  //
                                                    0, 1, 1, 0,  //
                                                    0, 0, 0, 0,  //
                                                    0, 0, 0, 0 },
                                 TetrominoConstant{ 0, 1, 1, 0,  //
                                                    0, 1, 1, 0,  //
                                                    0, 0, 0, 0,  //
                                                    0, 0, 0, 0 },
                                 TetrominoConstant{ 0, 1, 1, 0,  //
                                                    0, 1, 1, 0,  //
                                                    0, 0, 0, 0,  //
                                                    0, 0, 0, 0 } },
TetrominoConstantVariation_T = { TetrominoConstant{ 0, 1, 0, 0,  //
                                                    1, 1, 1, 0,  //
                                                    0, 0, 0, 0,  //
                                                    0, 0, 0, 0 },
                                 TetrominoConstant{ 0, 1, 0, 0,  //
                                                    0, 1, 1, 0,  //
                                                    0, 1, 0, 0,  //
                                                    0, 0, 0, 0 },
                                 TetrominoConstant{ 1, 1, 1, 0,  //
                                                    0, 1, 0, 0,  //
                                                    0, 0, 0, 0,  //
                                                    0, 0, 0, 0 },
                                 TetrominoConstant{ 0, 1, 0, 0,  //
                                                    1, 1, 0, 0,  //
                                                    0, 1, 0, 0,  //
                                                    0, 0, 0, 0 } };

constexpr auto& TetrominoConstantVariation( char C )
{
    switch( C )
    {
        default:
        case 'I': return TetrominoConstantVariation_I;
        case 'J': return TetrominoConstantVariation_J;
        case 'L': return TetrominoConstantVariation_L;
        case 'S': return TetrominoConstantVariation_S;
        case 'Z': return TetrominoConstantVariation_Z;
        case 'O': return TetrominoConstantVariation_O;
        case 'T': return TetrominoConstantVariation_T;
    }
}

enum class Action { NONE, DOWN, LEFT, RIGHT, ROTATE_L, ROTATE_R };

Action GetAction()
{
    if( kbhit() )
    {
        switch( getch() )
        {
            case 'q': return Action::ROTATE_L;
            case 'e': return Action::ROTATE_R;
            case 0:
            case 224:
                switch( getch() )
                {
                    case 75: return Action::LEFT;
                    case 77: return Action::RIGHT;
                    case 80: return Action::DOWN;
                    default: break;
                }
            default: break;
        }
    }
    return Action::NONE;
}

struct Tetromino
{
    const array<TetrominoConstant, 4>& Variation;
    unsigned char Rotation : 2 { 0 };
    int RelativeX{ BoardWidth / 2 - 2 };
    int RelativeY{ BoardHeight };

    Tetromino( char C ) : Variation{ TetrominoConstantVariation( C ) } {}
    Tetromino( const Tetromino& ) = default;

    auto& operator[]( auto x ) const { return Variation[ Rotation ][ x ]; }

    void Perform( Action Act )
    {
        switch( Act )
        {
            default:
            case Action::NONE: break;
            case Action::DOWN: --RelativeY; break;
            case Action::LEFT: --RelativeX; break;
            case Action::RIGHT: ++RelativeX; break;
            case Action::ROTATE_L: --Rotation; break;
            case Action::ROTATE_R: ++Rotation; break;
        };
    }
};

auto RandomChoice()
{
    constexpr auto Options = std::array{ 'I', 'J', 'L', 'S', 'Z', 'O', 'T' };
    return Options[ rand() % Options.size() ];
}

const auto STD_OUT = GetStdHandle( STD_OUTPUT_HANDLE );

using Column = array<Block, ActualBoardHeight>;
struct Board : array<Column, BoardWidth>
{
    bool FullRow( int y )
    {
        for( auto& col : *this )
            if( ! col[ y ] ) return false;
        return true;
    }

    void EraseFullRows()
    {
        for( auto y = 0; y < ActualBoardHeight; ++y )
            if( FullRow( y ) )
                for( auto& col : *this )
                    for( auto k = y; k < ActualBoardHeight - 1; ++k )
                        col[ k ] = col[ k + 1 ];
    }

    void Embed( const Tetromino& Piece )
    {
        for( auto x = 0; x < 4; ++x )
            for( auto y = 0; y < 4; ++y )
                if( Piece[ x ][ y ] )
                    ( *this )[ Piece.RelativeX + x ][ Piece.RelativeY + y ] = 1;
    }

    void Display( const Tetromino& Piece )
    {
        auto DisplayBuffer = *this;

        DisplayBuffer.Embed( Piece );

        SetConsoleCursorPosition( STD_OUT, { 0, 0 } );

        for( auto y = BoardHeight; y-- > 0; )
        {
            cout << " |";
            for( auto x = 0; x < BoardWidth; ++x )
                cout << ( DisplayBuffer[ x ][ y ] ? '*' : ' ' );
            cout << "|\n";
        }
        cout << " +" << std::setfill( '-' ) << std::setw( BoardWidth + 1 ) << "+";
        cout << endl;
    }

    bool Collide( const Tetromino& Piece )
    {
        for( auto x = 0; x < 4; ++x )
            for( auto y = 0; y < 4; ++y )
            {
                auto EffectiveX = Piece.RelativeX + x;
                auto EffectiveY = Piece.RelativeY + y;
                if( Piece[ x ][ y ] )
                    if( EffectiveX < 0 || EffectiveX >= BoardWidth ||
                        EffectiveY < 0 || ( *this )[ EffectiveX ][ EffectiveY ] )
                        return true;
            }

        return false;
    }

    bool Attempt( Tetromino& Piece, Action Act )
    {
        if( Act == Action::NONE ) return true;

        auto Pioneer = Piece;
        Pioneer.Perform( Act );
        if( Collide( Pioneer ) ) return false;

        Piece.Perform( Act );
        return true;
    }

    void MainLoop()
    {
        using namespace std::literals::chrono_literals;
        using std::chrono::steady_clock;
        while( true )
        {
            auto Piece = Tetromino{ RandomChoice() };
            if( Collide( Piece ) ) break;
            auto LastUpdate = decltype( steady_clock::now() ){};
            bool EndTurn    = false;
            while( ! EndTurn )
            {
                auto Act = GetAction();

                if( steady_clock::now() - LastUpdate > 200ms )
                {
                    Act        = Action::DOWN;
                    LastUpdate = steady_clock::now();
                }

                if( Attempt( Piece, Act ) == false && Act == Action::DOWN )
                {
                    Embed( Piece );
                    EndTurn = true;
                }

                if( Act != Action::NONE )
                {
                    EraseFullRows();
                    Display( Piece );
                }
            }
        }
    }
};

static auto disable_stdio_sync = [] {
    return std::ios_base::sync_with_stdio( false );
}();

void ShowConsoleCursor( bool showFlag )
{
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo( STD_OUT, &cursorInfo );
    cursorInfo.bVisible = showFlag;  // set the cursor visibility
    SetConsoleCursorInfo( STD_OUT, &cursorInfo );
}

int main()
{
    ShowConsoleCursor( false );

    Board{}.MainLoop();

    return 0;
}