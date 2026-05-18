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
    if (days_in_month > 0) {
        average_daily_expenses = total_expenses / days_in_month;
    } else {
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
void budget_prediction(double net_balance, double average_daily_expenses, int days_remaining)
{
    if (average_daily_expenses > 0)
    {
        double days_lasting = net_balance / average_daily_expenses;

        printf("\n--- Prediction ---\n");
        if (net_balance > 0)
        {
            printf("At your current rate, your money will last roughly %.1lf days.\n", days_lasting);

            if (days_lasting < days_remaining)
            {
                printf("Warning: You'll run out %d days before month-end!\n", (int)(days_remaining - days_lasting));
                printf("Consider cutting back on non_essential categories.\n");
            }
            else
            {
                printf("You are on track to finish the month safely.\n");
            }
        }
        else
        {
            printf("Warning: Balance is negative. Cut non-essential spending immediately.\n");
        }
    }
}