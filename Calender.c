#include <stdio.h>
#include <stdbool.h> //To use bool as a Boolean data type

struct Date {
    int year;
    int month;
    int day;
};

//to check whether a given year is a leap year or not (1 or 0)
int is_leap_year(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0); 
}

//to calculate the day of the week for a given date
int get_day_of_week(int year, int month, int day) {
    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4}; //differences in day of the week for each month(Zeller's Congruence algorithm)
    year -= month < 3; //If month is January or February 
    return (year + year / 4 - year / 100 + year / 400 + t[month - 1] + day) % 7;
}

//to get number of days in a given month for a specific year
int get_days_in_month(int year, int month) {
    int days_per_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && is_leap_year(year))
        return 29;
    else
        return days_per_month[month];
}

//To check the date is valid or not 
bool is_valid_date(struct Date date) {
    if (date.year < 1 || date.month < 1 || date.month > 12 || date.day < 1 || date.day > get_days_in_month(date.year, date.month))
        return false;
    return true;
}

//To get the date from the user
struct Date get_date_from_user() {
    struct Date date;
    printf("Enter the year: ");
    scanf("%d", &date.year);
    printf("Enter the month: ");
    scanf("%d", &date.month);
    printf("Enter the day: ");
    scanf("%d", &date.day);
    return date;
}

// to return name of corresponding day of the week as character pointer
char* get_day_name(int day_of_week) {
    char* days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    return days[day_of_week];
}

//to generate calender of a month of given year
void calender_of_month(int year,int month) {
    printf("Enter year : ");
    scanf("%d", &year);
    printf("Enter month (1-12): ");
    scanf("%d", &month);
    // Check if the input month is within the valid range
    if (month < 1 || month > 12) {
        printf("Invalid month input.\n");
        return;
    }

    // Print the calendar header
    printf("\n\tYear: %d \tMonth: %d\n", year, month);
    printf("  Sun  Mon  Tue  Wed  Thu  Fri  Sat\n");

    // Calculate the day of the week for the first day of the month
    int dayOfWeek = get_day_of_week(year, month, 1);

    // Determine the number of days in the month
    int daysInMonth = get_days_in_month(year,month);

    // Print the calendar
    for (int i = 0; i < dayOfWeek; i++) {
        printf("     ");
    }
    for (int day = 1; day <= daysInMonth; day++) {
        printf("%5d", day);
        if (++dayOfWeek > 6) {
            printf("\n");
            dayOfWeek = 0;
        }
    }
    printf("\n");
}

//to find the day of the week for a given date
void find_day_by_date() {
    struct Date date = get_date_from_user();
    if (!is_valid_date(date)) {
        printf("Invalid date\n");
        return;
    }
    int day_of_week = get_day_of_week(date.year, date.month, date.day);
    printf("The day on %02d-%02d-%d is %s\n", date.day, date.month, date.year, get_day_name(day_of_week));
}

//to calculate the number of days between two given dates
int calculate_days(struct Date start_date, struct Date end_date) {
    if (!is_valid_date(start_date) || !is_valid_date(end_date)) {
        printf("Invalid date\n");
        return -1;
    }
    int total_days_start = 0;
    for (int y = 1; y < start_date.year; ++y) {
        total_days_start += is_leap_year(y) ? 366 : 365;
    }
    for (int m = 1; m < start_date.month; ++m) {
        total_days_start += get_days_in_month(start_date.year, m);
    }
    total_days_start += start_date.day;

    int total_days_end = 0;
    for (int y = 1; y < end_date.year; ++y) {
        total_days_end += is_leap_year(y) ? 366 : 365;
    }
    for (int m = 1; m < end_date.month; ++m) {
        total_days_end += get_days_in_month(end_date.year, m);
    }
    total_days_end += end_date.day;

    return total_days_end - total_days_start;
}

//to input a specific date and display information about that date
void jump_to_date() {
    printf("****  Jump to a specific date  ****\n");
    struct Date target_date = get_date_from_user();
    if (!is_valid_date(target_date)) {
        printf("Invalid date\n");
        return;
    }
    int day_of_week = get_day_of_week(target_date.year, target_date.month, target_date.day);
    printf("The day on %d-%02d-%02d is %s\n", target_date.year, target_date.month, target_date.day, get_day_name(day_of_week));
}

//to add a note associated with a specific date and stores the note in a file("notes.txt")
void add_note() {
    printf(" ************  Add a note  ************\n");
    struct Date date = get_date_from_user();
    if (!is_valid_date(date)) {
        printf("Invalid date\n");
        return;
    }
    char note[1000];
    printf("Enter your note: ");
    scanf(" %[^\n]", note);

    FILE *file = fopen("notes.txt", "a");
    if (file) {
        fprintf(file, "%04d-%02d-%02d: %s\n", date.year, date.month, date.day, note);   //fprintf- to format the date, note and append as a line in the file
        fclose(file);
        printf("Note added successfully!\n");
    } else {
        printf("Failed to open the file\n");
    }
}

//to view notes stored in a file ("notes.txt") for a specific date
void view_notes() {
    printf("*** View notes for a particular date ***\n");
    struct Date date = get_date_from_user();
    if (!is_valid_date(date)) {
        printf("Invalid date.\n");
        return;
    }

    FILE *file = fopen("notes.txt", "r");
    if (file) {
        printf("Notes for %d-%02d-%02d:\n", date.year, date.month, date.day);
        char line[1000];
        while (fgets(line, sizeof(line), file)) {   //fgets- read the lines from file
            int year, month, day;
            if (sscanf(line, "%04d-%02d-%02d:", &year, &month, &day) == 3) {    //sscanf- to parse the year, month, and day components from the line
                if (year == date.year && month == date.month && day == date.day) {
                    printf("%s", line);
                }
            }
        }
        fclose(file);
    } else {
        printf("No notes found for this date\n");
    }
}

int main() {
    while (1) {
        printf("\n1.Generate calender of given month and year\n");
        printf("2. Find the day of a specific date\n");
        printf("3. Jump to a specific date\n");
        printf("4. Calculate days between two dates\n");
        printf("5. Add a note to a particular date\n");
        printf("6. View notes for a particular date\n");
        printf("7. Exit\n");

        int choice,year,month;
        printf("\nEnter your choice (1-7): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                calender_of_month(year,month);
                break;
            case 2:
                find_day_by_date();
                break;
            case 3:
                jump_to_date();
                break;
            case 4:
                printf("Calculate days between two dates:\n");
                struct Date start_date = get_date_from_user();
                struct Date end_date = get_date_from_user();
                int days_difference = calculate_days(start_date, end_date);
                if (days_difference >= 0) {
                    printf("There are %d days between %d-%02d-%02d and %d-%02d-%02d\n",
                           days_difference, start_date.year, start_date.month, start_date.day, end_date.year, end_date.month, end_date.day);
                }   
                break;
            case 5:
                add_note();
                break;
            case 6:
                view_notes();
                break;
            case 7:
                printf("Exiting the program.\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }
}
