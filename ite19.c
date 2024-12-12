#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Function to get the value of a Roman numeral character
// Each Roman numeral corresponds to a specific integer value
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
        return 0; // Return 0 for invalid characters
    }
}

// Function to convert a Roman numeral string to its decimal (integer) equivalent
int convertRomanToDecimal(char roman[])
{
    int total = 0, i;

    // Loop through the Roman numeral string
    for (i = 0; roman[i] != '\0'; i++)
    {
        int current = romanValue(roman[i]);       // Get the value of the current Roman numeral
        int next = romanValue(roman[i + 1]);     // Get the value of the next Roman numeral (if any)

        // If the current value is less than the next, subtract it; otherwise, add it
        if (current < next)
            total -= current;
        else
            total += current;
    }
    return total;
}

// Function to perform arithmetic operations based on the provided operator
int performOperation(int num1, int num2, char op)
{
    switch (op)
    {
    case '+': // Addition
        return num1 + num2;
    case '-': // Subtraction
        return num1 - num2;
    case '*': // Multiplication
        return num1 * num2;
    case '/': // Division, ensuring no division by zero
        return num2 != 0 ? num1 / num2 : 0;
    default:  // Return 0 for invalid operators
        return 0;
    }
}

// Function to convert a number to its equivalent English words
void convertNumberToWords(int num, char result[])
{
    // Arrays for word representations of numbers
    const char *ones[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
    const char *teens[] = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
    const char *tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
    const char *thousands[] = {"", "Thousand", "Million", "Billion"};

    char temp[256] = "";       // Temporary buffer for constructing word chunks
    char chunkWords[100] = ""; // Temporary buffer for each chunk
    result[0] = '\0';          // Clear the result buffer before starting
    int place = 0;             // Keeps track of the thousands place (e.g., thousand, million, etc.)

    // Special case for 0
    if (num == 0)
    {
        strcpy(result, "Zero");
        return;
    }

    // Process the number in groups of three digits (chunks)
    while (num > 0)
    {
        int chunk = num % 1000; // Extract the last three digits
        chunkWords[0] = '\0';   // Clear the buffer for the current chunk

        if (chunk > 0)
        {
            if (chunk >= 100) // Process the hundreds place
            {
                int hundreds = chunk / 100;
                sprintf(chunkWords + strlen(chunkWords), "%s Hundred ", ones[hundreds]);
                chunk %= 100;
            }
            if (chunk >= 20) // Process the tens place for numbers >= 20
            {
                int tensPlace = chunk / 10;
                sprintf(chunkWords + strlen(chunkWords), "%s ", tens[tensPlace]);
                chunk %= 10;
            }
            else if (chunk >= 10) // Process the 'teens' numbers (10-19)
            {
                sprintf(chunkWords + strlen(chunkWords), "%s ", teens[chunk - 10]);
                chunk = 0;
            }
            if (chunk > 0) // Process the ones place
            {
                sprintf(chunkWords + strlen(chunkWords), "%s ", ones[chunk]);
            }
            if (place > 0) // Append the appropriate thousand/million/billion suffix
            {
                sprintf(chunkWords + strlen(chunkWords), "%s ", thousands[place]);
            }
        }

        // Prepend the current chunk to the result string
        sprintf(temp, "%s%s", chunkWords, result);
        strcpy(result, temp);

        num /= 1000; // Remove the last three digits
        place++;     // Move to the next place value (thousands, millions, etc.)
    }

    // Remove any trailing space from the result
    size_t len = strlen(result);
    if (len > 0 && result[len - 1] == ' ')
    {
        result[len - 1] = '\0';
    }
}

// Main function to process input file and generate output file
int main()
{
    // Open input and output files
    FILE *input = fopen("C:\\CSU Docs\\Senior Years\\ITE 19\\final_project\\output\\Input.txt", "r");
    FILE *output = fopen("C:\\CSU Docs\\Senior Years\\ITE 19\\final_project\\output\\Output.txt", "w");
    char line[256], roman1[20], roman2[20], op;
    int num1, num2, result;
    char wordResult[100];

    // Check for file errors
    if (!input || !output)
    {
        printf("Error opening file!\n");
        return 1;
    }

    // Read each line from the input file
    while (fgets(line, sizeof(line), input))
    {
        // Parse the line into two Roman numerals and an operator
        if (sscanf(line, " %19s %c %19s ", roman1, &op, roman2) == 3)
        {
            num1 = convertRomanToDecimal(roman1); // Convert the first Roman numeral to decimal
            num2 = convertRomanToDecimal(roman2); // Convert the second Roman numeral to decimal
            result = performOperation(num1, num2, op); // Perform the arithmetic operation

            // Handle negative results
            if (result < 0)
            {
                fprintf(output, "Negative ");
                result = -result; // Convert to positive for word conversion
            }

            convertNumberToWords(result, wordResult); // Convert the result to words
            fprintf(output, "%s\n", wordResult); // Write the result to the output file
        }
        else
        {
            // Handle invalid input lines
            fprintf(output, "Error parsing line: %s\n", line);
        }
    }

    // Close the input and output files
    fclose(input);
    fclose(output);

    printf("Processing complete. Check Output.txt for results.\n");
    return 0;
}
