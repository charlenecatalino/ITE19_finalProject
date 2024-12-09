#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Function to get the value of a Roman numeral character
int romanValue(char c)
{
    switch (c)
    {
    case 'I':
        return 1;
    case 'V':
        return 5;
    case 'X':
        return 10;
    case 'L':
        return 50;
    case 'C':
        return 100;
    case 'D':
        return 500;
    case 'M':
        return 1000;
    default:
        return 0; // Invalid character
    }
}

// Function to convert Roman numeral to decimal
int convertRomanToDecimal(char roman[])
{
    int total = 0, i;
    for (i = 0; roman[i] != '\0'; i++)
    {
        int current = romanValue(roman[i]);
        int next = romanValue(roman[i + 1]);

        if (current < next)
            total -= current; // Subtract if smaller value before larger
        else
            total += current; // Add otherwise
    }
    return total;
}

// Function to perform arithmetic operations
int performOperation(int num1, int num2, char op)
{
    switch (op)
    {
    case '+':
        return num1 + num2;
    case '-':
        return num1 - num2;
    case '*':
        return num1 * num2;
    case '/':
        return num2 != 0 ? num1 / num2 : 0; // Avoid division by zero
    default:
        return 0;
    }
}

// Function to convert number to words (limited to thousands)
void convertNumberToWords(int num, char result[])
{
    const char *ones[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
    const char *teens[] = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
    const char *tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
    const char *thousands[] = {"", "Thousand", "Million", "Billion"};

    char temp[256] = "";       // Temporary buffer for constructing word chunks
    char chunkWords[100] = ""; // Temporary buffer for each chunk
    result[0] = '\0';          // Clear the result buffer before starting
    int place = 0;

    if (num == 0)
    {
        strcpy(result, "Zero");
        return;
    }

    while (num > 0)
    {
        int chunk = num % 1000; // Process last three digits
        chunkWords[0] = '\0';   // Clear chunkWords for each iteration

        if (chunk > 0)
        {
            if (chunk >= 100)
            {
                int hundreds = chunk / 100;
                sprintf(chunkWords + strlen(chunkWords), "%s Hundred ", ones[hundreds]);
                chunk %= 100;
            }
            if (chunk >= 20)
            {
                int tensPlace = chunk / 10;
                sprintf(chunkWords + strlen(chunkWords), "%s ", tens[tensPlace]);
                chunk %= 10;
            }
            else if (chunk >= 10)
            {
                sprintf(chunkWords + strlen(chunkWords), "%s ", teens[chunk - 10]);
                chunk = 0;
            }
            if (chunk > 0)
            {
                sprintf(chunkWords + strlen(chunkWords), "%s ", ones[chunk]);
            }
            if (place > 0)
            {
                sprintf(chunkWords + strlen(chunkWords), "%s ", thousands[place]);
            }
        }

        // Prepend this chunk to the result
        sprintf(temp, "%s%s", chunkWords, result);
        strcpy(result, temp);

        num /= 1000;
        place++;
    }

    // Remove trailing space
    size_t len = strlen(result);
    if (len > 0 && result[len - 1] == ' ')
    {
        result[len - 1] = '\0';
    }
}

// Main function to process input and generate output
int main()
{
    FILE *input = fopen("C:\\CSU Docs\\Senior Years\\ITE 19\\final_project\\output\\Input.txt", "r");
    FILE *output = fopen("C:\\CSU Docs\\Senior Years\\ITE 19\\final_project\\output\\Output.txt", "w");
    char line[256], roman1[20], roman2[20], op;
    int num1, num2, result;
    char wordResult[100];

    if (!input || !output)
    {
        printf("Error opening file!\n");
        return 1;
    }

    while (fgets(line, sizeof(line), input))
    {
        if (sscanf(line, " %19s %c %19s ", roman1, &op, roman2) == 3)
        {
            num1 = convertRomanToDecimal(roman1);
            num2 = convertRomanToDecimal(roman2);
            result = performOperation(num1, num2, op);

            // Debugging statement to check intermediate values
            printf("Roman1: %s (%d), Roman2: %s (%d), Operation: %c, Result: %d\n",
                   roman1, num1, roman2, num2, op, result);

            // Handle negative numbers
            if (result < 0)
            {
                fprintf(output, "Negative ");
                result = -result; // Convert to positive for word conversion
            }

            convertNumberToWords(result, wordResult);
            fprintf(output, "%s\n", wordResult);
        }
        else
        {
            fprintf(output, "Error parsing line: %s\n", line);
        }
    }

    fclose(input);
    fclose(output);

    printf("Processing complete. Check Output.txt for results.\n");
    return 0;
}
