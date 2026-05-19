#include <math.h>
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
    // Validate that remaining days do not exceed total days in the month
    if (days_remaining > days_in_month) {
        printf("\nError: Remaining days (%d) cannot be greater than total days in the month (%d).\n", days_remaining, days_in_month);
        return 1; // Terminate the program safely with an error code
    }
    
    if (days_remaining < 0 || days_in_month <= 0) {
        printf("\nError: Days cannot be negative, and the month must have at least 1 day.\n", days_remaining, days_in_month);
        return 1;
    }


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
    printf("Total Income: KES%.2lf\n", total_income);
    printf("Total Expenses: KES%.2lf\n", total_expenses);
    printf("Net Balance: KES%.2lf\n", net_balance);
    printf("Average Daily Expenses: KES%.2lf\n", average_daily_expenses);

    // Call the function
    budget_prediction(net_balance, average_daily_expenses, days_remaining);

    return 0;
}
// A function for predicting, how long the budget will last
void budget_prediction(double net_balance, double average_daily_expenses, int days_remaining)
{
    printf("\n--- Prediction ---\n");

    // Catch zero or negative balance immediately
    if (net_balance <= 0) 
    {
        printf("Warning: Balance is zero or negative ($%.2lf). Cut non-essential spending immediately!\n", net_balance);
        return; 
    }

    // Check if daily expenses are greater than zero, avoids division by zero
    if (average_daily_expenses > 0)
    {
        // Approximates how many days the balance can support
        double days_lasting = net_balance / average_daily_expenses;

        // Show the estimated no. of days the money lasts
        printf("At your current rate, your money will last roughly %.0lf days.\n", days_lasting);

        // Compare the lasting days with remaining days of the month
        if (days_lasting < days_remaining)
        {
            // Warns the user, money may be finished before end month
            int days_short = (int)ceil(days_remaining - days_lasting);
            printf("Warning: You'll run out %d days before end month!\n", days_short);
            printf("Consider cutting back on non-essential categories.\n");
        }
        else
        {
            printf("You are on track to finish the month safely.\n");
        }
    }
    else 
    {
        printf("No daily expenses recorded yet. Your balance is secure.\n");
    }
}
