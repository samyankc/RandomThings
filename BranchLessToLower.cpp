char BranchLessToLower( char c )
{
    return c | ( ( c & 0x40 ) >> ( ( 0x7FFFFFE >> ( c & 0x1F ) ) & 1 ) );
}


char ToLower( char c )
{
    if ('A'<=c&&c<='Z') c+='a'-'A';
    return c;
}
