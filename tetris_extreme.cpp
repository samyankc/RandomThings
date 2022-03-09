#include <windows.h>
#include <conio.h>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <array>

constexpr static auto BoardWidth        = 10;
constexpr static auto BoardHeight       = 16;
constexpr static auto ActualBoardHeight = BoardHeight + 4;

constexpr static auto TetrominoVariationSize = 7;

using TetrominoVariation = uint64_t;

constexpr static auto TetrominoVariations =
std::array<TetrominoVariation, TetrominoVariationSize>{
0x22220F0022220F00, 0x4460740062202E00, 0x6440E20022604700, 0x2640630026406300,
0x46206C0046206C00, 0x6600660066006600, 0x2320720026202700 };

struct Action
{
    int8_t RelativeX, RelativeY;
    uint8_t Rotation : 2;
    bool operator==( const Action& ) const = default;
    const static Action NONE, DOWN, LEFT, RIGHT, ROTATE_L, ROTATE_R;
};
constexpr Action Action::NONE     = {};
constexpr Action Action::DOWN     = { .RelativeY = -1 };
constexpr Action Action::LEFT     = { .RelativeX = -1 };
constexpr Action Action::RIGHT    = { .RelativeX = 1 };
constexpr Action Action::ROTATE_L = { .Rotation = 3 };
constexpr Action Action::ROTATE_R = { .Rotation = 1 };

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
    const TetrominoVariation Variation;
    Action State{ BoardWidth / 2 - 2, BoardHeight, 0 };

    Tetromino( int rand_num )
        : Variation{ TetrominoVariations[ rand_num % TetrominoVariationSize ] }
    {}

    bool Solid( int8_t x, int8_t y ) const
    {
        return ( Variation >> ( 16 * State.Rotation + 4 * y + x ) ) & 1;
    }

    void Perform( Action Act )
    {
        State.RelativeX += Act.RelativeX;
        State.RelativeY += Act.RelativeY;
        State.Rotation += Act.Rotation;
    }
};

const auto STD_OUT = GetStdHandle( STD_OUTPUT_HANDLE );

using Column = std::array<bool, ActualBoardHeight>;
struct Board : std::array<Column, BoardWidth>
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
                if( Piece.Solid( x, y ) )
                    ( *this )[ Piece.State.RelativeX + x ]
                             [ Piece.State.RelativeY + y ] = true;
    }

    void Display( const Tetromino& Piece )
    {
        auto DisplayBuffer = *this;
        DisplayBuffer.Embed( Piece );

        SetConsoleCursorPosition( STD_OUT, { 0, 0 } );

        for( auto y = BoardHeight; y-- > 0; )
        {
            std::cout << " |";
            for( auto x = 0; x < BoardWidth; ++x )
                std::cout << ( DisplayBuffer[ x ][ y ] ? '*' : ' ' );
            std::cout << "|\n";
        }
        std::cout << " +" << std::setfill( '-' ) << std::setw( BoardWidth + 1 )
                  << "+" << std::endl;
    }

    bool Collide( const Tetromino& Piece )
    {
        for( auto x = 0; x < 4; ++x )
            for( auto y = 0; y < 4; ++y )
            {
                auto EffectiveX = Piece.State.RelativeX + x;
                auto EffectiveY = Piece.State.RelativeY + y;
                if( Piece.Solid( x, y ) )
                    if( EffectiveX < 0 || EffectiveX >= BoardWidth ||
                        EffectiveY < 0 || ( *this )[ EffectiveX ][ EffectiveY ] )
                        return true;
            }
        return false;
    }

    bool Attempt( Tetromino& Piece, Action Act )
    {
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
            auto Piece = Tetromino{ rand() };
            if( Collide( Piece ) ) break;

            auto LastUpdate = steady_clock::now();
            bool EndTurn    = false;
            while( ! EndTurn )
            {
                auto Act = GetAction();

                if( Act == Action::NONE )
                {
                    if( steady_clock::now() - LastUpdate < 200ms ) continue;
                    Act        = Action::DOWN;
                    LastUpdate = steady_clock::now();
                }

                if( Attempt( Piece, Act ) == false && Act == Action::DOWN )
                {
                    Embed( Piece );
                    EndTurn = true;
                }

                EraseFullRows();
                Display( Piece );
            }
        }
    }
};

void HideConsoleCursor()
{
    CONSOLE_CURSOR_INFO cursorInfo{ 1, 0 };
    SetConsoleCursorInfo( STD_OUT, &cursorInfo );
}

int main()
{
    HideConsoleCursor();
    Board{}.MainLoop();
    return 0;
}