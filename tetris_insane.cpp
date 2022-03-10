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

struct Tetromino
{
    const TetrominoVariation Variation;
    int8_t RelativeX{ BoardWidth / 2 - 2 };
    int8_t RelativeY{ BoardHeight };
    uint8_t Rotation : 2 { 0 };

    bool Solid( int8_t x, int8_t y ) const
    {
        return ( Variation >> ( 16 * Rotation + 4 * y + x ) ) & 1;
    }
};

namespace Action {
static void NONE( Tetromino& ) {}
static void DOWN( Tetromino& P ) { P.RelativeY += -1; }
};  // namespace Action

auto GetAction()  // quiz: what is the type name of GetAction
{
    if( kbhit() )
    {
        switch( getch() )
        {
            case 'q': return +[]( Tetromino& P ) { P.Rotation += -1; };
            case 'e': return +[]( Tetromino& P ) { P.Rotation += 1; };
            case 0:
            case 224:
                switch( getch() )
                {
                    case 75: return +[]( Tetromino& P ) { P.RelativeX += -1; };
                    case 77: return +[]( Tetromino& P ) { P.RelativeX += 1; };
                    case 80: return Action::DOWN;
                    default: break;
                }
            default: break;
        }
    }
    return Action::NONE;
}

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
                    ( *this )[ Piece.RelativeX + x ]  //
                             [ Piece.RelativeY + y ] = true;
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
                auto EffectiveX = Piece.RelativeX + x;
                auto EffectiveY = Piece.RelativeY + y;
                if( Piece.Solid( x, y ) )
                    if( EffectiveX < 0 || EffectiveX >= BoardWidth ||
                        EffectiveY < 0 || ( *this )[ EffectiveX ][ EffectiveY ] )
                        return true;
            }
        return false;
    }

    // quiz: what is the type name of Act
    bool Attempt( Tetromino& Piece, auto Act )
    {
        auto Pioneer = Piece;
        Act( Pioneer );
        if( Collide( Pioneer ) ) return false;

        Act( Piece );
        return true;
    }

    void MainLoop()
    {
        using namespace std::literals::chrono_literals;
        using std::chrono::steady_clock;
        while( true )
        {
            auto Piece =
            Tetromino{ TetrominoVariations[ rand() % TetrominoVariationSize ] };
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