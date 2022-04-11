char BrachLessToLower( char c )
{
    // char Transformer = ( c & 0x40 ) >> ( ( 0x7FFFFFE >> ( c & 0x1F ) ) & 1 );
    return c | ( ( c & 0x40 ) >> ( ( 0x7FFFFFE >> ( c /*& 0x1F*/ ) ) & 1 ) );
}
