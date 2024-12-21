#include <stdio.h>      // For input and output operations
#include <string.h>     // For string manipulation functions

// Function to get the value of a Roman numeral character 
int romanValue(char c)
{
    // Use a switch-case to map Roman numeral characters to their respective integer values
    switch (c)
    {
    case 'I': return 1;       // I represents 1
    case 'V': return 5;       // V represents 5
    case 'X': return 10;      // X represents 10
    case 'L': return 50;      // L represents 50
    case 'C': return 100;     // C represents 100
    case 'D': return 500;     // D represents 500
    case 'M': return 1000;    // M represents 1000
    default: return 0;        // Return 0 for invalid characters
    }
}

// Function to convert a Roman numeral string to a decimal (integer) value
//iterate each roman character, from left to right
int convertRomanToDecimal(char roman[])
{
    int total = 0, i; // Initialize total to 0, and declare loop variable i

    // Loop through the Roman numeral string until the null character
    for (i = 0; roman[i] != '\0'; i++)
    {
        int current = romanValue(roman[i]);       // Get the value of the current Roman numeral
        int next = romanValue(roman[i + 1]);     // Get the value of the next Roman numeral (if exists)

        // If the current value is smaller than the next, subtract the current value; otherwise, add it
        if (current < next)
            total -= current;
        else
            total += current;
    }
    return total; // Return the final computed decimal value
}

// Function to perform arithmetic operations between two numbers
int performOperation(int num1, int num2, char op)
{
    // Use a switch-case to determine the operation based on the operator character
    switch (op)
    {
    case '+': return num1 + num2;    // Addition
    case '-': return num1 - num2;    // Subtraction
    case '*': return num1 * num2;    // Multiplication
    case '/': return num2 != 0 ? num1 / num2 : 0; // Division, handling division by zero
    default: return 0;               // Return 0 for invalid operators
    }
}

// Function to convert a number to its equivalent English word representation
void convertNumberToWords(int num, char result[])
{
    // Define arrays for number-to-word conversion
    // Define an array for the English word equivalents of numbers 0-9
    const char *ones[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
    // Define an array for English words representing numbers 10-19 (the "teens")
    const char *teens[] = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
    // Define an array for the tens place words (20, 30, ..., 90)
    const char *tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
    // Define an array for place values (e.g., thousand, million, billion)
    const char *thousands[] = {"", "Thousand", "Million", "Billion"};

    char temp[256] = ""; // Temporary buffer for constructing the word result
    char chunkWords[100] = ""; // Temporary buffer for each chunk
    result[0] = '\0'; // Clear the result buffer
    int place = 0; // Keeps track of the place value (e.g., thousand, million)

    // Handle the special case of 0
    if (num == 0)
    {
        strcpy(result, "Zero");
        return;
    }

    // Process the number in chunks of three digits
    while (num > 0)
    {
        int chunk = num % 1000; // Extract the last three digits
        chunkWords[0] = '\0';   // Clear the chunk buffer

        if (chunk > 0)
        {
            if (chunk >= 100) // Process hundreds place
            {
                int hundreds = chunk / 100;
                sprintf(chunkWords + strlen(chunkWords), "%s Hundred ", ones[hundreds]);
                chunk %= 100;
            }
            if (chunk >= 20) // Process tens place for numbers >= 20
            {
                int tensPlace = chunk / 10;
                sprintf(chunkWords + strlen(chunkWords), "%s ", tens[tensPlace]);
                chunk %= 10;
            }
            else if (chunk >= 10) // Process 'teens' numbers
            {
                sprintf(chunkWords + strlen(chunkWords), "%s ", teens[chunk - 10]);
                chunk = 0;
            }
            if (chunk > 0) // Process ones place
            {
                sprintf(chunkWords + strlen(chunkWords), "%s ", ones[chunk]);
            }
            if (place > 0) // Append thousands/millions/billions suffix
            {
                sprintf(chunkWords + strlen(chunkWords), "%s ", thousands[place]);
            }
        }

        // Prepend the current chunk to the result
        sprintf(temp, "%s%s", chunkWords, result);
        strcpy(result, temp);

        num /= 1000; // Move to the next chunk
        place++;     // Move to the next place value
    }

    // Remove any trailing space from the result
    size_t len = strlen(result);
    if (len > 0 && result[len - 1] == ' ')
    {
        result[len - 1] = '\0';
    }
}

// Function to display the arithmetic operation process
void displayOperationProcess(int num1, int num2, char op, int result)
{
    printf("%d %c %d = %d\n", num1, op, num2, result); // Print the process to the console
}

// Main function to read input, process Roman numeral operations, and write results
int main()
{
    // Open the input file in read mode and output file in write mode
    FILE *input = fopen("C:\\CSU Docs\\Senior Years\\ITE 19\\final_project\\Input2.txt", "r");
    FILE *output = fopen("C:\\CSU Docs\\Senior Years\\ITE 19\\final_project\\Output.txt", "w");
    char line[256], roman1[20], roman2[20], op; // Buffers for input line and extracted data
    int num1, num2, result; // Variables for operands and result
    char wordResult[100]; // Buffer for result in words

    // Check if either the input or output file failed to open
    if (!input || !output)
    {
        printf("Error opening file!\n");
        return 1;
    }

    // Read each line from the input file until the end of the file is reached
    while (fgets(line, sizeof(line), input))
    {
        // Parse the line into two Roman numerals and an operator
        if (sscanf(line, " %19s %c %19s ", roman1, &op, roman2) == 3)
        {
            num1 = convertRomanToDecimal(roman1); // Convert first Roman numeral to decimal
            num2 = convertRomanToDecimal(roman2); // Convert second Roman numeral to decimal
            result = performOperation(num1, num2, op); // Perform the operation

            // Display the process in the console
            displayOperationProcess(num1, num2, op, result);

            // Handle negative results
            if (result < 0)
            {
                fprintf(output, "Negative ");
                result = -result; // Convert to positive for word conversion
            }

            // Convert result to words and write to output file
            convertNumberToWords(result, wordResult);
            fprintf(output, "%s\n", wordResult); // Write the English words to output.txt
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

    printf("Processing complete. Check Output.txt for results.\n"); // Notify the user
    return 0;
}
