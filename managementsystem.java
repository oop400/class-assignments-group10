import java.io.*;
import java.util.*;

public class managementsystem {
    static final int MAX_COURSES = 1000;
    static String[] courseIds = new String[MAX_COURSES];
    static String[] titles = new String[MAX_COURSES];
    static int[] creditHours = new int[MAX_COURSES];
    static int courseCount = 0;

    static String[] usernames = new String[MAX_COURSES];
    static String[] passwords = new String[MAX_COURSES];
    static int userCount = 0;

    // ---------------- File Handling ----------------
    static void loadCourses() {
        try (BufferedReader br = new BufferedReader(new FileReader("courses.txt"))) {
            String line;
            courseCount = 0;
            while ((line = br.readLine()) != null) {
                String[] parts = line.split(",");
                if (parts.length == 3) {
                    courseIds[courseCount] = parts[0];
                    titles[courseCount] = parts[1];
                    creditHours[courseCount] = Integer.parseInt(parts[2]);
                    courseCount++;
                }
            }
        } catch (IOException e) {}
    }

    static void saveCourses() {
        try (PrintWriter pw = new PrintWriter(new FileWriter("courses.txt"))) {
            for (int i = 0; i < courseCount; i++) {
                pw.println(courseIds[i] + "," + titles[i] + "," + creditHours[i]);
            }
        } catch (IOException e) {}
    }

    static void loadUsers() {
        try (BufferedReader br = new BufferedReader(new FileReader("users.txt"))) {
            String line;
            userCount = 0;
            while ((line = br.readLine()) != null) {
                String[] parts = line.split(",");
                if (parts.length == 2) {
                    usernames[userCount] = parts[0];
                    passwords[userCount] = parts[1];
                    userCount++;
                }
            }
        } catch (IOException e) {}
    }

    static void saveUsers() {
        try (PrintWriter pw = new PrintWriter(new FileWriter("users.txt"))) {
            for (int i = 0; i < userCount; i++) {
                pw.println(usernames[i] + "," + passwords[i]);
            }
        } catch (IOException e) {}
    }

    // ---------------- Authentication ----------------
    static boolean loginUser(Scanner sc) {
        System.out.print("Enter username: ");
        String u = sc.next();
        System.out.print("Enter password: ");
        String p = sc.next();
        for (int i = 0; i < userCount; i++) {
            if (usernames[i].equals(u) && passwords[i].equals(p)) {
                return true;
            }
        }
        return false;
    }

    static void registerUser(Scanner sc) {
        System.out.print("Choose username: ");
        String u = sc.next();
        System.out.print("Choose password: ");
        String p = sc.next();
        for (int i = 0; i < userCount; i++) {
            if (usernames[i].equals(u)) {
                System.out.println("Username already exists!");
                return;
            }
        }
        usernames[userCount] = u;
        passwords[userCount] = p;
        userCount++;
        saveUsers();
        System.out.println("User registered successfully!");
    }

    // ---------------- Course Functions ----------------
    static void listCourses() {
        if (courseCount == 0) {
            System.out.println("No courses available.");
            return;
        }
        System.out.println("Course List:");
        for (int i = 0; i < courseCount; i++) {
            System.out.println(courseIds[i] + " | " + titles[i] + " | " + creditHours[i] + " credits");
        }
    }

    static void addCourse(Scanner sc) {
        if (courseCount >= MAX_COURSES) {
            System.out.println("Course storage full!");
            return;
        }
        System.out.print("Enter course ID: ");
        String id = sc.next();
        for (int i = 0; i < courseCount; i++) {
            if (courseIds[i].equals(id)) {
                System.out.println("Course ID already exists!");
                return;
            }
        }
        sc.nextLine();
        System.out.print("Enter course title: ");
        String title = sc.nextLine();
        System.out.print("Enter credit hours (1-6): ");
        int credits = sc.nextInt();
        if (credits < 1 || credits > 6) {
            System.out.println("Invalid credit hours!");
            return;
        }
        courseIds[courseCount] = id;
        titles[courseCount] = title;
        creditHours[courseCount] = credits;
        courseCount++;
        saveCourses();
        System.out.println("Course added successfully!");
    }

    static void deleteCourse(Scanner sc) {
        System.out.print("Enter course ID to delete: ");
        String id = sc.next();
        for (int i = 0; i < courseCount; i++) {
            if (courseIds[i].equals(id)) {
                for (int j = i; j < courseCount - 1; j++) {
                    courseIds[j] = courseIds[j + 1];
                    titles[j] = titles[j + 1];
                    creditHours[j] = creditHours[j + 1];
                }
                courseCount--;
                saveCourses();
                System.out.println("Course deleted successfully!");
                return;
            }
        }
        System.out.println("Course not found.");
    }

    static void searchCourse(Scanner sc) {
        sc.nextLine();
        System.out.print("Enter course ID or title keyword: ");
        String keyword = sc.nextLine();
        boolean found = false;
        for (int i = 0; i < courseCount; i++) {
            if (courseIds[i].equals(keyword) || titles[i].contains(keyword)) {
                System.out.println(courseIds[i] + " | " + titles[i] + " | " + creditHours[i] + " credits");
                found = true;
            }
        }
        if (!found) System.out.println("No matching course found.");
    }

    static void updateCourse(Scanner sc) {
        System.out.print("Enter course ID to update: ");
        String id = sc.next();
        for (int i = 0; i < courseCount; i++) {
            if (courseIds[i].equals(id)) {
                sc.nextLine();
                System.out.print("Enter new title: ");
                titles[i] = sc.nextLine();
                System.out.print("Enter new credit hours (1-6): ");
                creditHours[i] = sc.nextInt();
                if (creditHours[i] < 1 || creditHours[i] > 6) {
                    System.out.println("Invalid credit hours!");
                    return;
                }
                saveCourses();
                System.out.println("Course updated successfully!");
                return;
            }
        }
        System.out.println("Course not found.");
    }

    // ---------------- Menu ----------------
    static void menu(Scanner sc) {
        int choice;
        do {
            System.out.println("\n--- Course Management System ---");
            System.out.println("1. Add Course");
            System.out.println("2. Delete Course");
            System.out.println("3. Search Course");
            System.out.println("4. Update Course");
            System.out.println("5. List All Courses");
            System.out.println("6. Logout/Exit");
            System.out.print("Enter choice: ");
            choice = sc.nextInt();

            switch (choice) {
                case 1: addCourse(sc); break;
                case 2: deleteCourse(sc); break;
                case 3: searchCourse(sc); break;
                case 4: updateCourse(sc); break;
                case 5: listCourses(); break;
                case 6: System.out.println("Logging out..."); break;
                default: System.out.println("Invalid choice.");
            }
        } while (choice != 6);
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        loadUsers();
        loadCourses();

        System.out.println("--- Welcome to CMS ---");
        System.out.println("1. Login\n2. Register");
        System.out.print("Choice: ");
        int choice = sc.nextInt();
        boolean loggedIn = false;

        if (choice == 1) {
            if (loginUser(sc)) {
                System.out.println("Login successful!");
                loggedIn = true;
            } else {
                System.out.println("Login failed.");
                return;
            }
        } else if (choice == 2) {
            registerUser(sc);
            System.out.println("Now login to continue.");
            if (loginUser(sc)) {
                System.out.println("Login successful!");
                loggedIn = true;
            } else {
                System.out.println("Login failed.");
                return;
            }
        } else {
            System.out.println("Invalid choice.");
            return;
        }

        if (loggedIn) menu(sc);
    }
}
