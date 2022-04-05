#include <cstdio>

template<std::size_t N>
struct constexpr_string
{
    static constexpr std::size_t Length = N;
    char c[ N ]{};

    constexpr constexpr_string( const char ( &src )[ N ] )
    {
        for( std::size_t i = 0; i < N; ++i ) c[ i ] = src[ i ];
    }

    constexpr const char* c_str() const { return c; }
    constexpr std::size_t size() const { return N; }
};

template<typename T, auto Name>
struct NamedField
{
    using ValueType = T;
    static constexpr auto FieldName{ Name };
    ValueType FieldValue;
};

#define DeclareField( FIELD_TYPE, FIELD_NAME ) \
    NamedField<FIELD_TYPE, constexpr_string( #FIELD_NAME )> FIELD_NAME

struct S
{
    DeclareField( int, Field_1_ );  // ==>  NamedField<int, "Field_1_"> Field_1_;
    DeclareField( double, Field_2_ );
    DeclareField( int*, Field_3_ );
};

int main()
{
    auto s1 = S{};
    std::puts( s1.Field_2_.FieldName.c_str() );
}
