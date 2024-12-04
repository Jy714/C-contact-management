#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH 150
#define MAX_PROPERTY_LENGTH 50
#define MAX_USERS 80
#define ENCRYPT_VAL 3

// user info
typedef struct
{
  char name[50];
  char phone[20];
  char email[50];
} User;

// used for sorting
User list[MAX_USERS];

// used to store user that deleted, so we can restore the value
User restoreList[MAX_USERS];
int count = 0; // used to record the index of restoreList

// use for data encryption and decryption
const char salt[] = "GRPFLU";

// functions prototype
void showMenu();
void toLowerCase(char str[]);
void printList(int userCount);
int isValidEmail(char email[]);
int isValidNumber(char number[]);
int saveToFile(User user);
int saveToFileBatch(User *user, int number);
void addContact();
int readFile(const char *filename, User list[], int maxUsers);
void sortContact();
int displayUser(int includeExit);
void editContact();
int searchContactByName(const char *name, int *indices, int maxIndices);
void searchContact();
void deleteByName();
void clearContact();
void readLine(char *buffer, int size);
void restoreContact();
void dataEncryption(User *user);
void dataDecryption(User *user, int count);
void printLogo();
int countFileUser();
void Exit();

void Exit()
{
  char choice[10];

  while (1)
  {
    printf("\n\n\n\t==> Enter 0 to go back menu: ");
    scanf("%s", choice); // Read input as a string

    // Check if the input is 0
    if (strcmp(choice, "0") == 0)
    {
      system("cls"); // Clear the screen
      return;        // Exit the function
    }

    // Validate the choice
    int isInvalid = 0;
    for (int i = 0; i < strlen(choice); i++)
    {
      if (!isdigit(choice[i]))
      { // If any character is not a number
        isInvalid = 1;
        break;
      }
    }

    if (isInvalid)
    {
      printf("\tInvalid input! Please enter 0 to go back menu. \n");
    }
    else
    {
      printf("\tInvalid input! Please enter 0 to go back menu. \n");
    }
  }
}

// print LOGO when program start
void printLogo()
{
  printf("\n\n");

  // Print the word "FLUTTERSHY" in pink
  printf("\t\t  FFFFFFF  L       U   U  TTTTT  TTTTT  EEEEE  RRRRR    SSSS   H   H   Y   Y\n");
  printf("\t\t  F        L       U   U    T      T    E      R   R   S       H   H    Y Y\n");
  printf("\t\t  FFFFF    L       U   U    T      T    EEEE   RRRRR    SSSS   HHHHH     Y\n");
  printf("\t\t  F        L       U   U    T      T    E      R  R        S   H   H     Y\n");
  printf("\t\t  F        LLLLL   UUUUU    T      T    EEEEE  R   R   SSSS    H   H     Y\n ");
}

// data encryption
void dataEncryption(User *user)
{
  // encryption format: salt + value, then each character + 3(we defined this value ourselves)
  char temp[MAX_PROPERTY_LENGTH];
  // 1. salt + value
  // name
  strcpy(temp, salt);
  strcat(temp, user->name);
  // strcat(temp, salt); // Append name to salt user-> name == *user.name
  for (int i = 0; i < strlen(temp); i++)
    temp[i] = temp[i] + ENCRYPT_VAL;
  strcpy(user->name, temp); // Save back the encrypted name

  // phone
  strcpy(temp, salt);
  strcat(temp, user->phone);
  // strcat(temp, salt);
  for (int i = 0; i < strlen(temp); i++)
    temp[i] = temp[i] + ENCRYPT_VAL;
  strcpy(user->phone, temp); // Save back the encrypted phone

  // email
  strcpy(temp, salt);
  strcat(temp, user->email);
  // strcat(temp, salt);
  for (int i = 0; i < strlen(temp); i++)
    temp[i] = temp[i] + ENCRYPT_VAL;
  strcpy(user->email, temp); // Save back the encrypted email
}

//  data decryption
void dataDecryption(User *user, int count)
{
  char temp[MAX_PROPERTY_LENGTH];
  int saltLength = strlen(salt);

  for (int i = 0; i < count; i++)
  {
    // name
    strcpy(temp, user[i].name);
    for (int j = 0; j < strlen(temp); j++)
    {
      temp[j] = temp[j] - ENCRYPT_VAL; // when encrypt, we + ENCRYPT_VAL at each character before store into the file
    }
    temp[strlen(temp)] = '\0';                                       // put a null terminator at last index
    memmove(temp, temp + saltLength, strlen(temp) - saltLength + 1); // Remove salt exp: salttest -> test
    strncpy(user[i].name, temp, sizeof(user[i].name) - 1);           // copy the value without salt into the target list
    user[i].name[sizeof(user[i].name) - 1] = '\0';                   // put null terminator at last index

    // phone
    strcpy(temp, user[i].phone);
    for (int j = 0; j < strlen(temp); j++)
    {
      temp[j] = temp[j] - ENCRYPT_VAL;
    }
    temp[strlen(temp)] = '\0';
    memmove(temp, temp + saltLength, strlen(temp) - saltLength + 1);
    strncpy(user[i].phone, temp, sizeof(user[i].phone) - 1);
    user[i].phone[sizeof(user[i].phone) - 1] = '\0';

    // email
    strcpy(temp, user[i].email);
    for (int j = 0; j < strlen(temp); j++)
    {
      temp[j] = temp[j] - ENCRYPT_VAL;
    }
    temp[strlen(temp)] = '\0';
    memmove(temp, temp + saltLength, strlen(temp) - saltLength + 1);
    strncpy(user[i].email, temp, sizeof(user[i].email) - 1);
    user[i].email[sizeof(user[i].email) - 1] = '\0';
  }
}

// read a line safety
void readLine(char *buffer, int size)
{
  if (fgets(buffer, size, stdin))
  {
    buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline
  }
}

// Landing page
void showMenu()
{
  printf("\n\n\t\t\t      Welcome to FlutterShy Contact Management System!\n");
  printf("\t\t\t\t============================================\n");
  printf("\t\t\t\t============ 1. Add New Contact ============\n");
  printf("\t\t\t\t============ 2. Edit Contact    ============\n");
  printf("\t\t\t\t============ 3. Delete Contact  ============\n");
  printf("\t\t\t\t============ 4. Search Contact  ============\n");
  printf("\t\t\t\t============ 5. Sort Contact    ============\n");
  printf("\t\t\t\t============ 6. Display Contact ============\n");
  printf("\t\t\t\t============ 7. Restore Contact ============\n");
  printf("\t\t\t\t============ 8. Clear Contact   ============\n");
  printf("\t\t\t\t============ 9. Exit            ============\n");
  printf("\t\t\t\t============================================\n");
}

// toLowerCaseFunction
void toLowerCase(char str[])
{
  for (int i = 0; str[i] != '\0'; i++)
  {
    str[i] = tolower(str[i]);
  }
}

// used to print the list
void printList(int userCount)
{
  // Header
  printf("|********|***********************************************|**********************|********************************|\n");
  printf("| %-6s | %-45s | %-20s | %-30s |\n", "No.", "Name", "Phone", "Email");
  printf("|********|***********************************************|**********************|********************************|\n");

  for (int i = 0; i < userCount; i++)
  {
    printf("| %-6d | %-45s | %-20s | %-30s |\n", i + 1, list[i].name, list[i].phone, list[i].email);
    printf("|________|_______________________________________________|______________________|________________________________|\n");
  }
  printf("\n\n");
}

// function to validate name
int isValidName(char name[])
{
  for (int i = 0; i < strlen(name); i++)
  {
    if (isalnum(name[i]) == 0)
    { // mean is not alphabet or number
      return 0;
    }
    return 1;
  }
}

// Function to validate email
int isValidEmail(char email[])
{
  char *aSymbol = strchr(email, '@'); // check for first occurrence
  char *dot = strrchr(email, '.');    // check for last occurrence
  if (aSymbol && dot && aSymbol < dot && aSymbol != email && dot != email + strlen(email) - 1)
  {
    return 1; // Valid email
  }
  return 0; // Invalid email
}

// Function to validate contact number (based on Malaysia phone number)
int isValidNumber(char number[])
{
  // malaysia phone number is between 9-12 number
  int length = strlen(number);

  if ((length < 9 || length > 12) || (strncmp(number, "601", 3) != 0))
  {
    return 0; // false
  }

  // check if the left part of number array is digit of not
  for (int i = 3; i < length; i++)
  {
    if (!isdigit(number[i]))
    {
      return 0; // not a digit, return false
    }
  }

  // if all passed, return true
  return 1;
}

// save to file function 1 by 1 (used for adding)
int saveToFile(User user)
{
  // declare an output file pointer
  FILE *out = fopen("contact.txt", "a");

  if (!out)
  { // out == 0
    printf("Error when opening the file");
    return -1;
  }

  // encrypt data before write into the files
  dataEncryption(&user);

  // file open successfully, now we need to write into the file
  fprintf(out, "\n%s %s %s", user.name, user.phone, user.email);
  fclose(out);
  return 1; // task terminate successfully
}

// save to file function batch
int saveToFileBatch(User *user, int number)
{
  // declare an output file pointer
  FILE *out = fopen("contact.txt", "w");

  if (!out)
  {
    printf("Error when opening the file");
    return -1;
  }
  for (int i = 0; i < number; i++)
  {
    dataEncryption(&user[i]);

    // file open successfully, now we need to write into the file
    fprintf(out, "\n%s %s %s", user[i].name, user[i].phone, user[i].email);
  }
  fclose(out);
  return 1;
}

// Count the number in the txt file
int countFileUser()
{
  FILE *file = fopen("contact.txt", "r");
  if (!file)
  {
    return -1; // Return error code if file cannot be opened
  }

  char line[MAX_LENGTH];
  int userCount = 0; // Counter for the number of users in file

  while (fgets(line, sizeof(line), file))
  {
    userCount++; // caused by this, it will increment by 1 at the last line
  }

  // when file is empty, return 0, if not it will return -1( 0-1 )
  if (userCount == 0)
  {
    return 0;
  }

  return userCount - 1; // if not, then 80 users userCount will show 81
}

// Add new contact
void addContact()
{
  system("cls");
  int userCount = countFileUser();
  int num;

  if (userCount < 0)
  {
    printf("Error occur when opening file...\n");
    Exit();
    return;
  }

  printf("\n\t\t\t\t\t****** Add New Contact ******\n\n\n");

  // if userCount >80
  if (userCount >= MAX_USERS)
  {
    printf("\t\t\t\t\tYou have reach maximum amount of contacts!\n");
    Exit();
    return;
  }

  printf("Enter number of user you like to add (max:%d)\n", MAX_USERS);
  printf("(current number available: %d/80): ", MAX_USERS - userCount);
  // less than 100 user in a time
  if (scanf("%d", &num) != 1)
  { // if the scanf return a value which is not 1, that's mean it is not an integer

    getchar();
    printf("\n\n\t\t\t\t\t[ Enter a valid number! ]\n");
    Exit();
    return;
  }

  // Clear the input buffer to avoid issues with fgets
  // Exp: Enter user's name: Enter user's phone number:
  getchar();
  // error handling
  if (userCount + num > MAX_USERS)
  {
    printf("\n\n\t\t\t\t\t[ You have reach maximum amount of contacts! ]\n");
    Exit();
    return;
  }

  // error handling with invalid input
  if (num <= 0 || num > MAX_USERS)
  {
    printf("\n\n\t\t\t\t\t[ Invalid number of users! ]\n");
    Exit();
    return;
  }

  for (int i = 0; i < num; i++)
  {
    User user;
    printf("\n\n\nContact %d: \n", i + 1);

    while (1)
    { // keep asking user input until it meet the format requirements
      printf("Enter user's name: ");
      readLine(user.name, sizeof(user.name));
      // in order to make the comparison (sort) easier and fair, we decided to convert the strings to lowerCase
      toLowerCase(user.name);
      int nameRes = isValidName(user.name); // input validate (pass == 1  fail == 0)

      if (nameRes != 1)
      {
        printf("[ Name format is not correct! ]\n");
      }
      else
      {
        break;
      }
    }

    while (1)
    {
      printf("\nEnter user's phone number (601xxxxxx): ");
      readLine(user.phone, sizeof(user.phone));
      int phoneRes = isValidNumber(user.phone); // input validate (pass == 1  fail == 0)
      if (phoneRes != 1)
      {
        printf("[ Phone number format is not correct! ]\n");
      }
      else
      {
        break;
      }
    }

    while (1)
    {
      printf("\nEnter user's Email: ");
      readLine(user.email, sizeof(user.email));
      // in order to make the comparison (sort) easier and fair, we decided to convert the strings to lowerCase
      toLowerCase(user.email);
      int emailRes = isValidEmail(user.email); // input validate (pass == 1  fail == 0)

      if (emailRes != 1)
      {
        printf("[ Email format is not correct! ]\n");
      }
      else
      {
        break;
      }
    }

    // error handling for saveToFile function
    if (saveToFile(user) > 0)
    {
      printf("\n\t\t\t\t\t[ User added successfully! ]\n");
    }
    else
    {
      printf("\n\n\t\t\t\t\t[ Failed to add user,Please try again... ]\n");
    }
  }
  Exit();
}

// read file
int readFile(const char *filename, User list[], int maxUsers)
{
  FILE *file = fopen(filename, "r");
  if (!file)
  {
    return -1; // Return error code if file cannot be opened
  }

  char line[MAX_LENGTH];
  int count = 0; // Counter for the number of users

  while (fgets(line, sizeof(line), file) && count < maxUsers)
  {
    char tempName[MAX_LENGTH] = "";
    char tempPhone[20] = "";
    char tempEmail[MAX_LENGTH] = "";

    // Split the line into tokens
    char *token = strtok(line, " \n");
    int tokenCount = 0;

    char *tokens[MAX_LENGTH / 2]; // Array to hold tokens

    // Collect all tokens in the line
    while (token != NULL)
    {
      tokens[tokenCount++] = token;
      // Passing NULL tells strtok() to continue from where it left off in the previous call exp: John doe, firstcall: john, secondcall: doe
      token = strtok(NULL, " \n");
    }

    // Ensure there are at least 3 tokens for a valid line
    if (tokenCount < 3)
    {
      continue; // Skip invalid lines
    }

    //  extract phone and email from the line (last two is phone and email)
    strncpy(tempPhone, tokens[tokenCount - 2], sizeof(tempPhone) - 1);
    strncpy(tempEmail, tokens[tokenCount - 1], sizeof(tempEmail) - 1);

    // Combine remaining tokens into the name
    tempName[0] = '\0'; // Start with an empty string
    for (int i = 0; i < tokenCount - 2; i++)
    {
      strncat(tempName, tokens[i], sizeof(tempName) - strlen(tempName) - 1);
      if (i < tokenCount - 3)
      {
        strncat(tempName, " ", sizeof(tempName) - strlen(tempName) - 1);
      }
    }

    tempName[strcspn(tempName, "\n")] = '\0';

    strncpy(list[count].name, tempName, sizeof(list[count].name) - 1);
    strncpy(list[count].phone, tempPhone, sizeof(list[count].phone) - 1);
    strncpy(list[count].email, tempEmail, sizeof(list[count].email) - 1);

    // Confirm null termination
    list[count].name[sizeof(list[count].name) - 1] = '\0';
    list[count].phone[sizeof(list[count].phone) - 1] = '\0';
    list[count].email[sizeof(list[count].email) - 1] = '\0';

    count++;
  }

  dataDecryption(list, count);

  fclose(file);
  return count; // Return the number of users read from file
}

//  Sort
//  Function of bubble sort for contact name
void sortContact()
{
  system("cls");
  printf("\n\t\t\t\t\t****** Sort Contact ******\n\n\n");
  // if we get -1 means error occur
  int userCount = readFile("contact.txt", list, 80);

  // error handling
  if (userCount == 0)
  {
    printf("No user found in file. \n");
    Exit();
    return;
  }

  if (userCount < 0)
  { // error occur
    printf("Some error occurred when open the file");
    Exit();
    return;
  }

  // ask user want sort by using name / phone / email
  printf("The sorting result will permanently store in the database!\n\n");
  printf("\t\t\t\t\t====== 1. Name         ======\n");
  printf("\t\t\t\t\t====== 2. Phone Number ======\n");
  printf("\t\t\t\t\t====== 3. Email        ======\n\n");
  printf("\t\t\t\t\tSort by: ");

  // ask user input
  int res;
  scanf("%d", &res);
  getchar();

  if (res < 1 || res > 3)
  {
    printf("\n\t\t\t\t\t[ Invalid input. ]\n");
    Exit();
    return;
  }

  // here we using bubble sort to sort our list (improvement edition)
  // this improvement is used if we have the array like this, is used to reduce execution time
  // exp: 0 1 2 3 4 5 6 7 9 8
  // only last 1 need to compare cuz b4 that all are ascending order
  int flag = 1; // means true

  for (int i = 0; i < userCount - 1 && flag; i++)
  {
    flag = 0; // indicate that this loop is done sorting
    for (int j = 0; j < userCount - i - 1; j++)
    {

      int compareRes;

      switch (res)
      {
        // compare logic for 3 property execute here based on case
      case 1:
        compareRes = strcmp(list[j].name, list[j + 1].name);
        break;
      case 2:
        compareRes = strcmp(list[j].phone, list[j + 1].phone);
        break;
      case 3:
        compareRes = strcmp(list[j].email, list[j + 1].email);
        break;
      }
      // usage of flag: from here if the inner loop didn't enter the if statement (mean j less than j+1)
      // it also means this loop already done sorting
      if (compareRes > 0)
      { // j greater than j+1
        User userTemp = list[j + 1];
        list[j + 1] = list[j];
        list[j] = userTemp;
        flag = 1; // set to true
      }
    }
  }
  system("cls");
  printf("\n\t\t\t\t\t****** Sorted List ******\n\n\n");
  printList(userCount);

  // update into files
  saveToFileBatch(list, userCount);
  Exit();
}

// display user
int displayUser(int includeExit)
{
  system("cls");
  // if we get -1 means error occur
  int userCount = readFile("contact.txt", list, 80);
  printf("\n\t\t\t\t\t****** Contact List ******\n\n\n");

  // error handling
  if (userCount == 0)
  {
    printf("No user found in file. \n");
    if (includeExit)
    {
      Exit(); // Call exit only if the set is flag
    }
    return -1;
  }

  if (userCount < 0)
  { // error occur
    printf("Some error occurred when open the file");
    if (includeExit)
    {
      Exit(); // Call exit only if the set is flag
    }
    return -1;
  }

  // loop to print the user
  printList(userCount);
  printf("%d Users found.\n\n", userCount);

  if (includeExit)
  {
    Exit(); // Call exit only if the set is flag
  }
  return userCount;
}

// edit user
void editContact()
{
  system("cls");
  printf("\n\t\t\t\t\t****** Edit Contact ******\n\n\n");
  // display the user so they can choose the contact to edit
  int userCount = displayUser(0);

  // if userCount == 1 (no user)
  if (userCount == -1)
  {
    Exit();
    return;
  }
  // user input to edit the user
  int userToEdit;
  printf("Choose the index of the contact to edit: ");
  scanf("%d", &userToEdit);
  getchar();
  // error handling
  if (userToEdit < 1 || userToEdit > userCount)
  {
    printf("\n\t\t\t\t\t[ Invalid input! ]\n");
    Exit();
    return; // exit with error
  }
  // ask user want to edit name / phone / email / all
  printf("\n\n");
  printf("\t\t\t\t\t====== 1. Name  ======\n");
  printf("\t\t\t\t\t====== 2. Phone ======\n");
  printf("\t\t\t\t\t====== 3. Email ======\n");
  printf("\t\t\t\t\t====== 4. All   ======\n\n");
  printf("\t\t\t\t\tWhich part would you like to edit: ");
  int res; // used to record user answer for edit part
  scanf("%d", &res);
  getchar();
  switch (res)
  {
  case 1: // Name
  {
    char newName[MAX_PROPERTY_LENGTH];
    printf("\nEnter new user's name: ");
    fgets(newName, 50, stdin);

    newName[strcspn(newName, "\n")] = '\0';

    int nameRes = isValidName(newName);
    if (nameRes != 1)
    {
      printf("[ Name format is not correct! ]");
      Exit();
      return;
    }

    toLowerCase(newName);

    strcpy(list[userToEdit - 1].name, newName);
    break;
  }
  case 2: // Phone
  {
    char newPhone[MAX_PROPERTY_LENGTH];
    printf("\nEnter new phone number: ");
    fgets(newPhone, 50, stdin);

    newPhone[strcspn(newPhone, "\n")] = '\0';

    int phoneRes = isValidNumber(newPhone);
    if (phoneRes != 1)
    {
      printf("[ Phone number format is not correct! ]");
      Exit();
      return;
    }

    strcpy(list[userToEdit - 1].phone, newPhone);
    break;
  }
  case 3: // Email
  {
    char newEmail[MAX_PROPERTY_LENGTH];
    printf("\nEnter new user's email: ");
    fgets(newEmail, 50, stdin);

    newEmail[strcspn(newEmail, "\n")] = '\0';

    int emailRes = isValidEmail(newEmail);
    if (emailRes != 1)
    {
      printf("[ Email format is not correct! ]");
      Exit();
      return;
    }

    toLowerCase(newEmail);
    strcpy(list[userToEdit - 1].email, newEmail);
    break;
  }
  case 4: // All
  {
    char newName[MAX_PROPERTY_LENGTH];
    char newPhone[MAX_PROPERTY_LENGTH];
    char newEmail[MAX_PROPERTY_LENGTH];

    printf("\nEnter new user's name: ");
    fgets(newName, 50, stdin);
    printf("\nEnter new phone number (601xxxxxx): ");
    fgets(newPhone, 50, stdin);
    printf("\nEnter new user's email: ");
    fgets(newEmail, 50, stdin);

    newName[strcspn(newName, "\n")] = '\0';
    newPhone[strcspn(newPhone, "\n")] = '\0';
    newEmail[strcspn(newEmail, "\n")] = '\0';

    toLowerCase(newName);
    toLowerCase(newEmail);

    int emailRes = isValidEmail(newEmail);
    int phoneRes = isValidNumber(newPhone);

    if (phoneRes != 1 && emailRes != 1)
    {
      printf("[ Phone number and email format are not correct! ]");
      Exit();
      return;
    }
    if (phoneRes != 1)
    {
      printf("[ Phone number format is not correct! ]");
      Exit();
      return;
    }
    if (emailRes != 1)
    {
      printf("[ Email format is not correct! ]");
      Exit();
      return;
    }

    strcpy(list[userToEdit - 1].name, newName);
    strcpy(list[userToEdit - 1].phone, newPhone);
    strcpy(list[userToEdit - 1].email, newEmail);
    break;
  }
  default:
    printf("\n\t\t\t\t\t[ Invalid input. ]\n");
    Exit();
    return;
  }

  // write new result into the file
  saveToFileBatch(list, userCount);

  printf("\n\t\t\t\t\t[ Edit Successfully! ]\n");
  Exit();
}

// search name that are use for both search and delete function (it also can do the partial search)
int searchContactByName(const char *name, int *indices, int maxIndices)
{
  int userCount = readFile("contact.txt", list, 80);

  if (userCount <= 0)
  {
    // if 0 print no user found, others must be less than 0
    printf(userCount == 0 ? "No user found.\n" : "Some error occurred when open the file...\n");
    Exit();
    return -1; // Return -1 for errors, 0 for no users
  }

  // ask for user input
  char searchName[MAX_PROPERTY_LENGTH];
  strncpy(searchName, name, MAX_PROPERTY_LENGTH);
  searchName[MAX_PROPERTY_LENGTH - 1] = '\0';

  // Convert the search name to lowercase
  toLowerCase(searchName);

  int found = 0;
  int matchedIndex = 1;
  printf("|********|***********************************************|**********************|********************************|\n");
  printf("| %-6s | %-45s | %-20s | %-30s |\n", "No.", "Name", "Phone", "Email");
  printf("|********|***********************************************|**********************|********************************|\n");
  for (int i = 0; i < userCount; i++)
  {
    char lowerName[MAX_PROPERTY_LENGTH];
    strcpy(lowerName, list[i].name);
    lowerName[MAX_PROPERTY_LENGTH - 1] = '\0';

    // here is the key that allow us to do the partial function
    if (strncmp(lowerName, searchName, strlen(searchName)) == 0)
    {
      printf("| %-6d | %-45s | %-20s | %-30s |\n", matchedIndex++, list[i].name, list[i].phone, list[i].email);
      printf("|________|_______________________________________________|______________________|________________________________|\n");

      if (indices != NULL && found < maxIndices)
      {
        indices[found] = i; // Store the index of the match
      }
      found++;
    }
  }

  if (found == 0)
  {
    printf("\n[ No contact found with the name '%s'. ]\n", name);
    Exit();
  }

  return found; // Return the number of matches found
}

// search by name
void searchContact()
{
  system("cls");
  printf("\n\t\t\t\t\t****** Search Contact ******\n\n\n");
  char name[MAX_PROPERTY_LENGTH];
  printf("Enter the name you want to search for: ");
  fgets(name, MAX_PROPERTY_LENGTH, stdin);
  name[strcspn(name, "\n")] = '\0';
  printf("\n");

  int result = searchContactByName(name, NULL, 0); // NULL means we don't need indices

  if (result > 0)
  {
    printf("\nTotal contact found: %d\n", result);
    Exit();
  }
}

// remove contact
void deleteContact(int index, int userCount)
{
  // error handling
  if (index < 0 || index >= userCount)
  {
    printf("Invalid index.\n");
    return;
  }

  // Save the contact to the restore list
  restoreList[count++] = list[index];

  // Remove the contact
  for (int i = index; i < userCount - 1; i++)
  {
    list[i] = list[i + 1];
  }

  printf("\n[ Contact '%s' deleted successfully. ]\n", restoreList[count - 1].name);

  // Save updated list to file
  saveToFileBatch(list, userCount - 1);
}

// delete user
void deleteByName()
{
  system("cls");
  printf("\n\t\t\t\t\t****** Delete Contact ******\n\n\n");

  // Read the user list from the file
  int userCount = readFile("contact.txt", list, MAX_USERS);

  if (userCount <= 0)
  {
    printf(userCount == 0 ? "No user found.\n" : "Error reading the file.\n");
    Exit();
    return;
  }

  // Ask for user input for deleted name
  char deletedName[MAX_PROPERTY_LENGTH];
  printf("Enter the name of the contact to delete: ");
  fgets(deletedName, sizeof(deletedName), stdin);
  deletedName[strcspn(deletedName, "\n")] = '\0'; // Remove trailing newline
  printf("\n");

  int indices[MAX_USERS];
  int foundCount = searchContactByName(deletedName, indices, MAX_USERS);

  if (foundCount <= 0)
  {
    // printf(foundCount == 0 ? "\t\t\t[ No contact found. ]" : "Error occurred during the search.\n");
    // Exit();
    return;
  }

  if (foundCount == 1)
  {
    deleteContact(indices[0], userCount);
    Exit();
    return;
  }

  // if multiple contact matched
  printf("\n\n%d contacts found.\n\n", foundCount);
  printf("\t\t\t\t\t====== 1. Delete One ======\n\t\t\t\t\t====== 2. Delete All ======\n\n");
  printf("\t\t\t\t\tYour choice: ");

  int choice;
  scanf("%d", &choice);
  getchar();

  if (choice == 1)
  {
    printf("\n\nEnter the index of the contact to delete (1 to %d): ", foundCount);
    int decision;
    scanf("%d", &decision);
    getchar();

    if (decision < 1 || decision > foundCount)
    {
      printf("\n\t\t\t\t\t[ Invalid input! ]\n");
      Exit();
      return;
    }

    deleteContact(indices[decision - 1], userCount);
    Exit();
    return;
  }
  else if (choice == 2)
  {
    char confirmation[MAX_PROPERTY_LENGTH];
    printf("\n\nAre you sure you want to delete all matching contacts? (y/n): ");
    fgets(confirmation, sizeof(confirmation), stdin);
    confirmation[strcspn(confirmation, "\n")] = '\0';

    // if using scanf, user input yxxx also will execute because scanf only take one character from our input
    if (strcmp(confirmation, "y") == 0 || strcmp(confirmation, "Y") == 0)
    {
      // Proceed with deletion
      int deletedCount = 0;
      for (int i = 0; i < foundCount; i++)
      {
        int index = indices[i] - deletedCount;
        restoreList[count++] = list[index];

        for (int j = index; j < userCount - 1; j++)
        {
          list[j] = list[j + 1];
        }

        userCount--;
        deletedCount++;
      }

      printf("\n\t\t\t\t\t[ Users deleted successfully! ]\n");
      saveToFileBatch(list, userCount);
    }
    else if (strcmp(confirmation, "n") == 0 || strcmp(confirmation, "N") == 0)
    {
      printf("\n\t\t\t\t\t[ Cancelled. ]\n");
    }
    else
    {
      printf("\n\t\t\t\t\t[ Invalid Input. ]\n");
    }
  }
}
// clear contact
void clearContact()
{
  system("cls");
  printf("\n\t\t\t\t\t****** Clear Contact ******\n\n\n");
  printf("You sure you want to clear your contact?\n");
  printf("Your data cannot restore if you proceed this function (y/n): ");

  char result;
  scanf(" %c", &result);

  FILE *file = NULL;

  switch (result)
  {
  case 'y':
  case 'Y':
    if ((file = fopen("contact.txt", "w")) == NULL)
    {
      printf("\n\nSome error occurred when open the file...");
      exit(1);
    }
    else
    {
      printf("\n\n\t\t\t\t\t[ Data cleared successfully!] \n");
      Exit();
    }
    return;
  case 'n':
  case 'N':
    printf("\n\n\t\t\t\t\t[ Cancel...]\n");
    Exit();
    return;
  default:
    printf("\n\n\t\t\t\t\t[ Invalid input. ]\n");
    Exit();
    return;
  }
}

// restore contact (data will lost if program terminates)
void restoreContact()
{
  system("cls");
  printf("\n\t\t\t\t\t****** Restore Contact ******\n\n\n");
  if (count <= 0)
  {
    printf("No user found.\n");
    Exit();
    return;
  }
  printf("Total of %d user in the list.\n\n", count);

  printf("|********|***********************************************|**********************|********************************|\n");
  printf("| %-6s | %-45s | %-20s | %-30s |\n", "No.", "Name", "Phone", "Email");
  printf("|********|***********************************************|**********************|********************************|\n");
  for (int i = 0; i < count; i++)
  {
    printf("| %-6d | %-45s | %-20s | %-30s |\n", i + 1, restoreList[i].name, restoreList[i].phone, restoreList[i].email);
    printf("|        |                                               |                      |                                |\n");
  }
  printf("******************************************************************************************************************\n");

  printf("\n\nWhich user you would like to restore (index, -1 to cancel): ");
  int choice;
  scanf("%d", &choice);
  getchar();
  // if user input -1
  if (choice == -1)
  {
    printf("\n\t\t\t\t\t[ Cancelled ]\n");
    Exit();
    return;
  }

  if (choice <= 0 || choice > count)
  {
    printf("\n\t\t\t\t\t[ Invalid input. ]\n");
    Exit();
    return;
  }

  // restore user to file
  int restoreIndex = choice - 1;

  char restoredName[MAX_PROPERTY_LENGTH];
  strcpy(restoredName, restoreList[restoreIndex].name);

  // save the restore user to the file
  saveToFile(restoreList[restoreIndex]);

  // Shift the remaining users (delete user from restore list)
  for (int i = restoreIndex; i < count - 1; i++)
  {
    restoreList[i] = restoreList[i + 1];
  }
  count--; // Decrease the count
  printf("\n\t\t [ User '%s' restored successfully.] \n", restoredName);
  Exit();
}

int main()
{

  // used to record the decision
  int decision;

  while (1)
  {
    // print out the logo
    printLogo();
    showMenu();

    printf("\n\n\nEnter a number: ");
    scanf("%d", &decision);
    getchar();

    // all main function going here
    switch (decision)
    {
    case 1: // Add
      addContact();
      // generateUsers();
      break;
    case 2: // Edit
      editContact();
      break;
    case 3: // Delete
      deleteByName();
      break;
    case 4: // Search
      searchContact();
      break;
    case 5: // Sort
      sortContact();
      break;
    case 6: // Display
      displayUser(1);
      break;
    case 7: // restore
      restoreContact();
      break;
    case 8: // Clear
      clearContact();
      break;
    case 9: // exit
      printf("Thank for using. Please visit us again\n");
      exit(0); // exit with successfully (0 mean success | 1 mean failed)
    default:
      system("cls"); // Clear the screen
      printf("Invalid choice. \n");
      printf("Please enter a number between 1 and 9...\n\n\n");
      getchar(); // Wait for user input to proceed
      break;
    }
  }
  return 0;
}