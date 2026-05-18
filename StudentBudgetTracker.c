#include <stdio.h>

void budget_prediction(double net_balance, double average_daily_expenses, int days_remaining);

int main() {
    printf("Welcome to the Student Budget Tracker!\n");

    double total_income, total_expenses, net_balance, average_daily_expenses;
    int days_in_month;
    int days_remaining;

    // Get user input for total income, total expenses, and days in the month
    printf("Enter your total income for the month: ");
    scanf("%lf", &total_income);

    printf("Enter your total expenses for the month: ");
    scanf("%lf", &total_expenses);

    printf("Enter the number of days in the month: ");
    scanf("%d", &days_in_month);

    printf("Enter the number of days remaining in the month: ");
    scanf("%d", &days_remaining);


    // Calculate net balance
    net_balance = total_income - total_expenses;

    // Calculate average daily expenses
    int days_passed = days_in_month - days_remaining;
    if (days_passed > 0)
    {
        average_daily_expenses = total_expenses / days_passed;
    } else 
    {
        average_daily_expenses = 0;
    }

    // Display the results
    printf("\n--- Budget Summary ---\n");
    printf("Total Income: $%.2lf\n", total_income);
    printf("Total Expenses: $%.2lf\n", total_expenses);
    printf("Net Balance: $%.2lf\n", net_balance);
    printf("Average Daily Expenses: $%.2lf\n", average_daily_expenses);

    // Call the function
    budget_prediction(net_balance, average_daily_expenses, days_remaining);

    return 0;
}
// A function for predicting, how long the budget will last
void budget_prediction(double net_balance, double average_daily_expenses, int days_remaining)
{
    // Check if daily expenses are greater than zero, avoids division by zero
    if (average_daily_expenses > 0)
    {
        // Approximates how many days the balance can support
        double days_lasting = net_balance / average_daily_expenses;

        printf("\n--- Prediction ---\n");

        // Check if the user has a positive balance
        if (net_balance > 0)
        {
            // Show the estimated no. of days the money lasts
            printf("At your current rate, your money will last roughly %.lf days.\n", days_lasting);

            // Compare the lasting days with remaining days of the month
            if (days_lasting < days_remaining)
            {
                // Warns the user, money may be finished before end month
                printf("Warning: You'll run out %d days before end month!\n", (int)(days_remaining - days_lasting));
                printf("Consider cutting back on non_essential categories.\n");
            }
            // Informs user that they are safe
            else
            {
                printf("You are on track to finish the month safely.\n");
            }
        }
        else
        // If balance is negative
        {
            printf("Warning: Balance is negative. Cut non-essential spending immediately.\n");
        }
    }
}