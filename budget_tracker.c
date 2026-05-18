#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_DESC 100
#define MAX_CAT 30
#define MAX_TYPE 10
#define MAX_DATE 11
#define DATA_FILE "budget.dat"

// ANSI color codes (disabled for Windows compatibility)
#define GREEN ""
#define RED ""
#define YELLOW ""
#define CYAN ""
#define RESET ""
#define BOLD ""

typedef struct {
    char date[MAX_DATE];        // YYYY-MM-DD
    char type[MAX_TYPE];        // "income" or "expense"
    char category[MAX_CAT];     // "salary", "food", "transport", etc.
    float amount;               // Always positive
    char description[MAX_DESC];
} Transaction;

typedef struct TransactionNode {
    Transaction data;
    struct TransactionNode* next;
} TransactionNode;

typedef struct {
    TransactionNode* head;
    int transaction_count;
} BudgetSystem;

// ============ UTILITY FUNCTIONS ============

void get_today_date(char* buffer) {
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    strftime(buffer, MAX_DATE, "%Y-%m-%d", tm_info);
}

int validate_amount(float amount) {
    return amount > 0;
}

int validate_date(const char* date) {
    if (strlen(date) != 10) return 0;
    if (date[4] != '-' || date[7] != '-') return 0;
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (!isdigit(date[i])) return 0;
    }
    return 1;
}

int validate_type(const char* type) {
    return strcmp(type, "income") == 0 || strcmp(type, "expense") == 0;
}

int validate_category(const char* category) {
    const char* valid_categories[] = {
        "salary", "bonus", "scholarship", "other_income",
        "rent", "food", "transport", "utilities", "entertainment",
        "shopping", "health", "education", "other_expense"
    };
    int count = sizeof(valid_categories) / sizeof(valid_categories[0]);
    for (int i = 0; i < count; i++) {
        if (strcmp(category, valid_categories[i]) == 0) return 1;
    }
    return 0;
}

void print_header() {
    printf("\n");
    printf(CYAN BOLD "╔════════════════════════════════════════════╗\n" RESET);
    printf(CYAN BOLD "║      📊 BUDGET TRACKER - Student Edition    ║\n" RESET);
    printf(CYAN BOLD "╚════════════════════════════════════════════╝\n" RESET);
    printf("\n");
}

void print_menu() {
    printf(CYAN BOLD "┌─ MAIN MENU ───────────────────────────────┐\n" RESET);
    printf("│ 1. Add Income                               │\n");
    printf("│ 2. Add Expense                              │\n");
    printf("│ 3. View All Transactions                    │\n");
    printf("│ 4. View Financial Summary                   │\n");
    printf("│ 5. View Expenses by Category                │\n");
    printf("│ 6. Exit                                     │\n");
    printf(CYAN BOLD "└────────────────────────────────────────────┘\n" RESET);
    printf("Choose an option (1-6): ");
}

// ============ DATA STRUCTURE FUNCTIONS ============

BudgetSystem* create_budget_system() {
    BudgetSystem* budget = (BudgetSystem*)malloc(sizeof(BudgetSystem));
    if (!budget) {
        perror("Memory allocation failed");
        exit(1);
    }
    budget->head = NULL;
    budget->transaction_count = 0;
    return budget;
}

void add_transaction_to_list(BudgetSystem* budget, Transaction t) {
    TransactionNode* new_node = (TransactionNode*)malloc(sizeof(TransactionNode));
    if (!new_node) {
        perror("Memory allocation failed");
        exit(1);
    }
    
    new_node->data = t;
    new_node->next = budget->head;
    budget->head = new_node;
    budget->transaction_count++;
}

void free_budget(BudgetSystem* budget) {
    TransactionNode* current = budget->head;
    while (current != NULL) {
        TransactionNode* temp = current;
        current = current->next;
        free(temp);
    }
    free(budget);
}

// ============ FILE I/O FUNCTIONS ============

void save_budget_to_file(BudgetSystem* budget) {
    FILE* file = fopen(DATA_FILE, "w");
    if (!file) {
        printf(RED "Error: Could not open file for writing.\n" RESET);
        return;
    }
    
    TransactionNode* current = budget->head;
    while (current != NULL) {
        fprintf(file, "%s,%s,%s,%.2f,%s\n",
                current->data.date,
                current->data.type,
                current->data.category,
                current->data.amount,
                current->data.description);
        current = current->next;
    }
    
    fclose(file);
}

BudgetSystem* load_budget_from_file() {
    BudgetSystem* budget = create_budget_system();
    FILE* file = fopen(DATA_FILE, "r");
    
    if (!file) {
        printf(YELLOW "⚠️  No existing budget file. Starting fresh.\n" RESET);
        return budget;
    }
    
    char line[500];
    while (fgets(line, sizeof(line), file)) {
        // Remove newline
        line[strcspn(line, "\n")] = 0;
        
        Transaction t;
        char date[MAX_DATE], type[MAX_TYPE], category[MAX_CAT], desc[MAX_DESC];
        float amount;
        
        if (sscanf(line, "%10[^,],%9[^,],%29[^,],%f,%99[^\n]",
                   date, type, category, &amount, desc) == 5) {
            strcpy(t.date, date);
            strcpy(t.type, type);
            strcpy(t.category, category);
            t.amount = amount;
            strcpy(t.description, desc);
            add_transaction_to_list(budget, t);
        }
    }
    
    fclose(file);
    printf(GREEN "✓ Loaded %d transactions from file.\n" RESET, budget->transaction_count);
    return budget;
}

// ============ CALCULATION FUNCTIONS ============

float calculate_balance(BudgetSystem* budget) {
    float balance = 0;
    TransactionNode* current = budget->head;
    
    while (current != NULL) {
        if (strcmp(current->data.type, "income") == 0) {
            balance += current->data.amount;
        } else {
            balance -= current->data.amount;
        }
        current = current->next;
    }
    
    return balance;
}

float calculate_total_expenses(BudgetSystem* budget) {
    float total = 0;
    TransactionNode* current = budget->head;
    
    while (current != NULL) {
        if (strcmp(current->data.type, "expense") == 0) {
            total += current->data.amount;
        }
        current = current->next;
    }
    
    return total;
}

float calculate_total_income(BudgetSystem* budget) {
    float total = 0;
    TransactionNode* current = budget->head;
    
    while (current != NULL) {
        if (strcmp(current->data.type, "income") == 0) {
            total += current->data.amount;
        }
        current = current->next;
    }
    
    return total;
}

int count_days_elapsed(BudgetSystem* budget) {
    if (budget->transaction_count == 0) return 1;
    
    // Count unique days
    char dates[1000][MAX_DATE];
    int unique_days = 0;
    
    TransactionNode* current = budget->head;
    while (current != NULL) {
        int found = 0;
        for (int i = 0; i < unique_days; i++) {
            if (strcmp(dates[i], current->data.date) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            strcpy(dates[unique_days], current->data.date);
            unique_days++;
        }
        current = current->next;
    }
    
    return unique_days > 0 ? unique_days : 1;
}

float calculate_burn_rate(BudgetSystem* budget) {
    float total_expenses = calculate_total_expenses(budget);
    int days = count_days_elapsed(budget);
    
    if (days == 0) return 0;
    return total_expenses / days;
}

int calculate_days_until_zero(float balance, float daily_burn) {
    if (daily_burn <= 0) return 99999;
    return (int)(balance / daily_burn);
}

float predict_end_of_month(float balance, float daily_burn, int days_remaining) {
    return balance - (daily_burn * days_remaining);
}

int get_days_remaining_in_month() {
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    
    int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int year = tm_info->tm_year + 1900;
    int month = tm_info->tm_mon;
    
    // Check for leap year
    if (month == 1 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))) {
        days_in_month[1] = 29;
    }
    
    return days_in_month[month] - tm_info->tm_mday;
}

// ============ DISPLAY FUNCTIONS ============

void display_financial_summary(BudgetSystem* budget) {
    printf("\n");
    printf(CYAN BOLD "╔════════════════════════════════════════════╗\n" RESET);
    printf(CYAN BOLD "║      💰 FINANCIAL SUMMARY                  ║\n" RESET);
    printf(CYAN BOLD "╚════════════════════════════════════════════╝\n" RESET);
    
    float income = calculate_total_income(budget);
    float expenses = calculate_total_expenses(budget);
    float balance = calculate_balance(budget);
    float burn_rate = calculate_burn_rate(budget);
    int days_left = get_days_remaining_in_month();
    int days_until_broke = calculate_days_until_zero(balance, burn_rate);
    
    printf(GREEN "💵 Total Income:        %,.2f KES\n" RESET, income);
    printf(RED "💸 Total Expenses:      %,.2f KES\n" RESET, expenses);
    printf(BOLD "💰 Current Balance:     %,.2f KES\n" RESET, balance);
    printf("📊 Daily Burn Rate:     %,.2f KES/day\n", burn_rate);
    printf("📅 Days Remaining:      %d days\n", days_left);
    
    printf("\n");
    printf(CYAN BOLD "⚠️  FINANCIAL PREDICTION\n" RESET);
    printf("├─ Days Until Broke:     %d days\n", days_until_broke);
    
    float predicted_balance = predict_end_of_month(balance, burn_rate, days_left);
    if (predicted_balance < 0) {
        printf(RED "└─ Predicted Balance (month-end): %,.2f KES\n\n" RESET, predicted_balance);
        printf(RED BOLD "🚨 WARNING: At your current burn rate, your remaining %.2f KES\n", balance);
        printf("   will only last for %d days. Consider cutting back on\n", days_until_broke);
        printf("   non-essential categories.\n" RESET);
    } else {
        printf(GREEN "└─ Predicted Balance (month-end): %,.2f KES\n\n" RESET, predicted_balance);
        printf(GREEN "✓ You're on track to finish the month with money left!\n" RESET);
    }
    printf("\n");
}

void display_all_transactions(BudgetSystem* budget) {
    if (budget->transaction_count == 0) {
        printf(YELLOW "\n⚠️  No transactions recorded yet.\n\n" RESET);
        return;
    }
    
    printf("\n");
    printf(CYAN BOLD "┌─ TRANSACTION HISTORY ────────────────────────────────────────┐\n" RESET);
    printf(CYAN "│  DATE       │  TYPE       │  CATEGORY    │  AMOUNT      │\n" RESET);
    printf(CYAN BOLD "├─────────────┼─────────────┼──────────────┼──────────────┤\n" RESET);
    
    TransactionNode* current = budget->head;
    while (current != NULL) {
        if (strcmp(current->data.type, "income") == 0) {
            printf(GREEN "│ %s │ %10s │ %12s │ %+10,.2f │\n" RESET,
                   current->data.date,
                   current->data.type,
                   current->data.category,
                   current->data.amount);
        } else {
            printf(RED "│ %s │ %10s │ %12s │ %+10,.2f │\n" RESET,
                   current->data.date,
                   current->data.type,
                   current->data.category,
                   -current->data.amount);
        }
        current = current->next;
    }
    
    printf(CYAN BOLD "└─────────────┴─────────────┴──────────────┴──────────────┘\n" RESET);
    printf("\n");
}

void display_expenses_by_category(BudgetSystem* budget) {
    printf("\n");
    printf(CYAN BOLD "┌─ EXPENSES BY CATEGORY ────────────────────┐\n" RESET);
    
    const char* expense_categories[] = {
        "rent", "food", "transport", "utilities", "entertainment",
        "shopping", "health", "education", "other_expense"
    };
    int cat_count = sizeof(expense_categories) / sizeof(expense_categories[0]);
    
    int total_expense = 0;
    for (int i = 0; i < cat_count; i++) {
        float category_total = 0;
        TransactionNode* current = budget->head;
        
        while (current != NULL) {
            if (strcmp(current->data.type, "expense") == 0 &&
                strcmp(current->data.category, expense_categories[i]) == 0) {
                category_total += current->data.amount;
            }
            current = current->next;
        }
        
        if (category_total > 0) {
            printf("│ %-25s %,.2f KES │\n", expense_categories[i], category_total);
            total_expense += category_total;
        }
    }
    
    printf(CYAN BOLD "├───────────────────────────────────────────┤\n" RESET);
    printf("│ TOTAL EXPENSES        %,.2f KES │\n", (float)total_expense);
    printf(CYAN BOLD "└───────────────────────────────────────────┘\n" RESET);
    printf("\n");
}

// ============ INPUT FUNCTIONS ============

void add_income(BudgetSystem* budget) {
    printf("\n" CYAN BOLD "➕ ADD INCOME\n" RESET);
    
    Transaction t;
    strcpy(t.type, "income");
    
    get_today_date(t.date);
    printf("Date (YYYY-MM-DD) [%s]: ", t.date);
    char input[MAX_DATE];
    if (fgets(input, sizeof(input), stdin) && strlen(input) > 1) {
        input[strcspn(input, "\n")] = 0;
        if (!validate_date(input)) {
            printf(RED "Invalid date format.\n" RESET);
            return;
        }
        strcpy(t.date, input);
    }
    
    printf("Income Category (salary/bonus/scholarship/other_income): ");
    fgets(t.category, MAX_CAT, stdin);
    t.category[strcspn(t.category, "\n")] = 0;
    if (!validate_category(t.category)) {
        printf(RED "Invalid category.\n" RESET);
        return;
    }
    
    printf("Amount (positive number, no currency symbol): ");
    if (scanf("%f", &t.amount) != 1 || !validate_amount(t.amount)) {
        printf(RED "Invalid amount. Must be positive.\n" RESET);
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');
    
    printf("Description: ");
    fgets(t.description, MAX_DESC, stdin);
    t.description[strcspn(t.description, "\n")] = 0;
    
    add_transaction_to_list(budget, t);
    save_budget_to_file(budget);
    printf(GREEN "✓ Income added successfully!\n" RESET);
}

void add_expense(BudgetSystem* budget) {
    printf("\n" CYAN BOLD "➖ ADD EXPENSE\n" RESET);
    
    Transaction t;
    strcpy(t.type, "expense");
    
    get_today_date(t.date);
    printf("Date (YYYY-MM-DD) [%s]: ", t.date);
    char input[MAX_DATE];
    if (fgets(input, sizeof(input), stdin) && strlen(input) > 1) {
        input[strcspn(input, "\n")] = 0;
        if (!validate_date(input)) {
            printf(RED "Invalid date format.\n" RESET);
            return;
        }
        strcpy(t.date, input);
    }
    
    printf("Expense Category (rent/food/transport/utilities/entertainment/shopping/health/education/other_expense): ");
    fgets(t.category, MAX_CAT, stdin);
    t.category[strcspn(t.category, "\n")] = 0;
    if (!validate_category(t.category)) {
        printf(RED "Invalid category.\n" RESET);
        return;
    }
    
    printf("Amount (positive number, no currency symbol): ");
    if (scanf("%f", &t.amount) != 1 || !validate_amount(t.amount)) {
        printf(RED "Invalid amount. Must be positive.\n" RESET);
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');
    
    printf("Description: ");
    fgets(t.description, MAX_DESC, stdin);
    t.description[strcspn(t.description, "\n")] = 0;
    
    add_transaction_to_list(budget, t);
    save_budget_to_file(budget);
    printf(GREEN "✓ Expense added successfully!\n" RESET);
}

// ============ MAIN PROGRAM ============

int main() {
    BudgetSystem* budget = load_budget_from_file();
    
    int choice;
    while (1) {
        print_header();
        print_menu();
        
        if (scanf("%d", &choice) != 1) {
            printf(RED "Invalid input. Please enter a number.\n" RESET);
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');
        
        switch (choice) {
            case 1:
                add_income(budget);
                break;
            case 2:
                add_expense(budget);
                break;
            case 3:
                display_all_transactions(budget);
                break;
            case 4:
                display_financial_summary(budget);
                break;
            case 5:
                display_expenses_by_category(budget);
                break;
            case 6:
                printf("\n" GREEN "✓ Budget saved. Goodbye!\n\n" RESET);
                save_budget_to_file(budget);
                free_budget(budget);
                return 0;
            default:
                printf(RED "Invalid choice. Please select 1-6.\n" RESET);
        }
    }
    
    return 0;
}
