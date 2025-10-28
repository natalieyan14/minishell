#include "header.h"

int main()
{
    char *input = "echo hi > ./outfiles/outfile01 bye";
    int count;
    t_token *tokens = tokenisation(input, &count);
    
    printf("Input: %s\n", input);
    printf("Token count: %d\n", count);
    
    for (int i = 0; i < count; i++)
    {
        printf("Token %d: '%s' (type: %d)\n", i, tokens[i].str, tokens[i].type);
    }
    
    free_tokens(tokens, count);
    return 0;
}
