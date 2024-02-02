#include <stdio.h>
#include <stdlib.h>

// Employee structure
struct Employee {
    int empID;
    char name[50];
    float salary;
};

// Function to add a new employee
void addEmployee(FILE *file) {
    struct Employee emp;

    printf("Enter employee ID: ");
    scanf("%d", &emp.empID);

    printf("Enter employee name: ");
    scanf("%s", emp.name);

    printf("Enter employee salary: ");
    scanf("%f", &emp.salary);

    fseek(file, (emp.empID - 1) * sizeof(struct Employee), SEEK_SET);
    fwrite(&emp, sizeof(struct Employee), 1, file);

    printf("Employee added successfully!\n");
}

// Function to display all employees
void displayAllEmployees(FILE *file) {
    rewind(file);

    struct Employee emp;

    printf("\nEmployee Details:\n");
    printf("%-10s%-20s%-10s\n", "Employee ID", "Name", "Salary");
    while (fread(&emp, sizeof(struct Employee), 1, file) == 1) {
        printf("%-10d%-20s%-10.2f\n", emp.empID, emp.name, emp.salary);
    }
    printf("\n");
}

// Function to search for an employee by ID
void searchEmployee(FILE *file) {
    int empID;
    printf("Enter employee ID to search: ");
    scanf("%d", &empID);

    struct Employee emp;

    fseek(file, (empID - 1) * sizeof(struct Employee), SEEK_SET);
    if (fread(&emp, sizeof(struct Employee), 1, file) == 1) {
        printf("\nEmployee Details:\n");
        printf("%-10s%-20s%-10s\n", "Employee ID", "Name", "Salary");
        printf("%-10d%-20s%-10.2f\n", emp.empID, emp.name, emp.salary);
    } else {
        printf("Employee with ID %d not found!\n", empID);
    }
}

int main() {
    FILE *employeeFile;

    // Open the file in binary mode with read and write permissions
    employeeFile = fopen("employee.dat", "rb+");

    // Check if the file exists, if not, create it
    if (employeeFile == NULL) {
        employeeFile = fopen("employee.dat", "wb+");
        if (employeeFile == NULL) {
            perror("Error creating/opening file");
            exit(EXIT_FAILURE);
        }
    }

    int choice;

    do {
        printf("\nEmployee Record System\n");
        printf("1. Add Employee\n");
        printf("2. Display All Employees\n");
        printf("3. Search Employee\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addEmployee(employeeFile);
                break;
            case 2:
                displayAllEmployees(employeeFile);
                break;
            case 3:
                searchEmployee(employeeFile);
                break;
            case 4:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice! Please enter a valid option.\n");
        }

    } while (choice != 4);

    // Close the file
    fclose(employeeFile);

    return 0;
}

