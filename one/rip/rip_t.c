#include <stdio.h>
#include <stdlib.h>

static void reverse_str(char *s) {
    int i = 0, j;
    while (s[i] != '\0') i++;
    for (j = 0, i = i - 1; j < i; j++, i--) {
        char tmp = s[j];
        s[j] = s[i];
        s[i] = tmp;
    }
}

static void remove_invalid(char *s, int last_i, int last_j, char open, char close) {
    int count = 0, i, j, len;
    
    for (len = 0; s[len] != '\0'; len++);

    for (i = last_i; i < len; i++) {
        if (s[i] == open)  count++;
        if (s[i] == close) count--;
        if (count >= 0) continue;
    
        for (j = last_j; j <= i; j++) {
            
            if (s[j] == close && (j == last_j || s[j-1] != close)) {
                char removed = s[j];
                
                for (int k = j; k < len; k++)
                    s[k] = s[k+1];
                
                remove_invalid(s, i, j, open, close);
                
                for (int k = len; k > j; k--)
                    s[k] = s[k-1];
                s[j] = removed;
            }   
        }   
        return;
    }

    reverse_str(s);
    if (open == '(') {
        
        remove_invalid(s, 0, 0, ')', '(');
    } else {
        
        puts(s);
    }
}

int main(int argc, char **argv) {
    if (argc != 2) return 0;

    
    char *input = argv[1];
    int len = 0;
    while (input[len]) len++;
    
    char *s = malloc(len + 1);
    if (!s) return 1;   

    
    for (int i = 0; i <= len; i++)
        s[i] = input[i];
    
    remove_invalid(s, 0, 0, '(', ')');
    
    free(s);
    return 0;
}
