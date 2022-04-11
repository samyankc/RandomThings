char BranchLessToLower( char c )
{
    return c | ( ( c & 0x40 ) >> ( ( 0x7FFFFFE >> ( c & 0x1F ) ) & 1 ) );
}
