#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_STUDENTS 100
#define MAX_NAME 100
#define MAX_ID_LENGTH 20
#define NUM_DEPARTMENTS 11

struct DateOfBirth {
    int day;
    int month;
    int year;
};

struct Student {
    char id[MAX_ID_LENGTH];
    char name[MAX_NAME];
    char first_name[MAX_NAME];
    struct DateOfBirth dob;  // Date of birth
    char department[MAX_NAME];
    float gpa; // General note
};

struct Student students[MAX_STUDENTS];
int student_count = 0;

void edit_student() {
    char id[MAX_ID_LENGTH];
    printf("Enter student ID to edit: ");
    scanf(" %[^\n]", id);

    int found = -1;
    for (int i = 0; i < student_count; i++) {
        if (strcmp(students[i].id, id) == 0) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        printf("Student with ID %s not found.\n", id);
        return;
    }

    printf("Editing student %s:\n", id);

    char buffer[MAX_NAME];
    int new_day, new_month, new_year;
    float temp_float;

    // Edit name
    printf("Enter new name (or press Enter to keep current: %s): ", students[found].name);
    fgets(buffer, MAX_NAME, stdin);  // Safe input to allow empty string
    if (strlen(buffer) > 1) { // Buffer will contain '\n' if Enter was pressed
        buffer[strcspn(buffer, "\n")] = 0; // Remove the newline character
        strcpy(students[found].name, buffer);
        for (int i = 0; students[found].name[i]; i++) {
            students[found].name[i] = tolower((unsigned char)students[found].name[i]);
        }
    }

    // Edit first name
    printf("Enter new first name (or press Enter to keep current: %s): ", students[found].first_name);
    fgets(buffer, MAX_NAME, stdin);
    if (strlen(buffer) > 1) {
        buffer[strcspn(buffer, "\n")] = 0;
        strcpy(students[found].first_name, buffer);
        for (int i = 0; students[found].first_name[i]; i++) {
            students[found].first_name[i] = tolower((unsigned char)students[found].first_name[i]);
        }
    }

    // Edit date of birth
    printf("Enter new date of birth (day month year) or press Enter to keep current: %02d/%02d/%04d:\n",
           students[found].dob.day, students[found].dob.month, students[found].dob.year);
    char dob_input[20];
    fgets(dob_input, sizeof(dob_input), stdin);
    if (sscanf(dob_input, "%d %d %d", &new_day, &new_month, &new_year) == 3) {
        students[found].dob.day = new_day;
        students[found].dob.month = new_month;
        students[found].dob.year = new_year;
    }

    // Edit department
    printf("Enter new department (or press Enter to keep current: %s): ", students[found].department);
    fgets(buffer, MAX_NAME, stdin);
    if (strlen(buffer) > 1) {
        buffer[strcspn(buffer, "\n")] = 0;
        strcpy(students[found].department, buffer);
        for (int i = 0; students[found].department[i]; i++) {
            students[found].department[i] = tolower((unsigned char)students[found].department[i]);
        }
    }

    // Edit GPA
    printf("Enter new GPA (or press Enter to keep current: %.2f): ", students[found].gpa);
    char gpa_input[20];
    fgets(gpa_input, sizeof(gpa_input), stdin);
    if (sscanf(gpa_input, "%f", &temp_float) == 1) {
        students[found].gpa = temp_float;
    }

    printf("Student information updated successfully.\n");
}

void delete_student() {
    char id[MAX_ID_LENGTH];
    printf("Enter student ID to delete: ");
    scanf(" %[^\n]", id);

    int found = -1;
    for (int i = 0; i < student_count; i++) {
        if (strcmp(students[i].id, id) == 0) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        printf("Student with ID %s not found.\n", id);
        return;
    }

    // Display student details and ask for confirmation
    printf("Student ID: %s\n", students[found].id);
    printf("Name: %s\n", students[found].name);
    printf("First Name: %s\n", students[found].first_name);
    printf("Date of Birth: %02d/%02d/%04d\n", students[found].dob.day, students[found].dob.month, students[found].dob.year);
    printf("Department: %s\n", students[found].department);
    printf("GPA (grade point average) ../20: %.2f\n", students[found].gpa);

    char confirmation;
    printf("\nAre you sure you want to delete this student? (y/n): ");
    scanf(" %c", &confirmation); // Ensure there's a space before %c to handle previous input

    if (confirmation == 'y' || confirmation == 'Y') {
        // Shift all elements to remove the student from the array
        for (int i = found; i < student_count - 1; i++) {
            students[i] = students[i + 1];
        }
        student_count--;

        printf("Student deleted successfully.\n");
    } else {
        printf("Deletion cancelled.\n");
    }
}

int id_exists(const char *id) {
    for (int i = 0; i < student_count; i++) {
        if (strcmp(students[i].id, id) == 0) {
            return 1; // ID exists
        }
    }
    return 0; // ID does not exist
}

void calculate_overall_average() {
    if (student_count == 0) {
        printf("No students available to calculate.\n");
        return;
    }

    // Departments
    char *departments[NUM_DEPARTMENTS] = {
        "Science and Technology",
        "Engineering",
        "Medicine and Pharmacy",
        "Law",
        "Economics and Management",
        "Humanities and Social Sciences",
        "Education",
        "Literature",
        "Languages",
        "Arts and Design",
        "Agricultural Sciences"
    };

    float department_gpa_sum[NUM_DEPARTMENTS] = {0};
    int department_count[NUM_DEPARTMENTS] = {0};

    float total_gpa = 0.0;
    int total_students = 0;

    // Calculate sums and counts
    for (int i = 0; i < student_count; i++) {
        total_gpa += students[i].gpa;
        total_students++;

        for (int j = 0; j < NUM_DEPARTMENTS; j++) {
            if (strcmp(students[i].department, departments[j]) == 0) {
                department_gpa_sum[j] += students[i].gpa;
                department_count[j]++;
                break;
            }
        }
    }

    // Display overall average for the university
    if (total_students > 0) {
        float overall_avg = total_gpa / total_students;
        printf("Overall GPA for the entire university: %.2f\n", overall_avg);
    }

    // Display average GPA for each department
    for (int i = 0; i < NUM_DEPARTMENTS; i++) {
        if (department_count[i] > 0) {
            float department_avg = department_gpa_sum[i] / department_count[i];
            printf("Department: %s, Average GPA: %.2f\n", departments[i], department_avg);
        } else {
            printf("Department: %s, No students enrolled.\n", departments[i]);
        }
    }
}

void add_student() 
{
    if (student_count >= MAX_STUDENTS) {
        printf("Student list is full. Cannot add more students.\n");
        return;
    }

    struct Student new_student;

    printf("Enter student ID: ");
    scanf(" %[^\n]", new_student.id);
    if (id_exists(new_student.id)) {
        printf("A student with this ID already exists.\n");
        return;
    }

    printf("Enter student's last name: ");
    scanf(" %[^\n]", new_student.name);
    for (int i = 0; new_student.name[i]; i++) {
        new_student.name[i] = tolower((unsigned char)new_student.name[i]);
    }

    printf("Enter student's first name: ");
    scanf(" %[^\n]", new_student.first_name);
    for (int i = 0; new_student.first_name[i]; i++) {
        new_student.first_name[i] = tolower((unsigned char)new_student.first_name[i]);
    }

    printf("Enter student's date of birth (day month year): ");
    scanf("%d %d %d", &new_student.dob.day, &new_student.dob.month, &new_student.dob.year);

    printf("Enter student's department: ");
    scanf(" %[^\n]", new_student.department);
    for (int i = 0; new_student.department[i]; i++) {
        new_student.department[i] = tolower((unsigned char)new_student.department[i]);
    }

    printf("Enter student's GPA (out of 20): ");
    scanf("%f", &new_student.gpa);

    students[student_count] = new_student;
    student_count++;

    printf("Student added successfully.\n");
}
