#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_LENGTH 150
#define MAX_PROPERTY_LENGTH 50

// user info
typedef struct
{
  char name[50];
  char phone[20];
  char email[50];
} User;

// used for sorting
User list[80];

// functions prototype TODO

// Landing page
void showMenu()
{
  printf("\nWelcome to FlutterShy Contact Management System!\n");
  printf("============================================\n");
  printf("============ 1. Add New Contact ============\n");
  printf("============ 2. Edit Contact    ============\n");
  printf("============ 3. Delete Contact  ============\n");
  printf("============ 4. Search Contact  ============\n");
  printf("============ 5. Sort Contact    ============\n");
  printf("============ 6. Display Contact ============\n");
  printf("============ 7. Clear Contact   ============\n");
  printf("============ 8. Exit            ============\n");
  printf("============================================\n");
}

// Function to validate email Need to change
int isValidEmail(char email[])
{
  int atSymbol = 0, dot = 0;
  char *atLocation = NULL;

  // Check if there is an '@' symbol and at least one '.'
  atLocation = strchr(email, '@'); // Find the first '@' symbol
  if (atLocation != NULL)
  {
    atSymbol = 1;
    // Check if there's a dot after '@'
    if (strchr(atLocation, '.') != NULL)
    {
      dot = 1;
    }
  }
  // Return true if both '@' and '.' exist and '@' comes before '.'
  return (atSymbol && dot);
}

// Function to validate contact number (based on Malaysia phone number)
int isValidNumber(char number[])
{
  // malaysia phone number is between 9-12 number
  int length = strlen(number);

  if (length < 9 || length > 12)
  {
    return 0; // false
  }

  if (number[0] != '6' || number[1] != '0' || number[2] != '1')
  { // malaysia phone number start from 601xxxxxx
    return 0;
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
  FILE *out;

  if ((out = fopen("contact.txt", "a")) == NULL)
  {
    printf("Error when opening the file");
    exit(1);
  }

  // file open successfully, now we need to write into the file
  fprintf(out, "\n%s %s %s", user.name, user.phone, user.email);

  printf("User added successfully\n");

  fclose(out);
}

int saveToFileBatch(User *user, int number)
{
  // declare an output file pointer
  FILE *out;

  if ((out = fopen("contact.txt", "w")) == NULL)
  {
    printf("Error when opening the file");
    exit(1);
  }

  for (int i = 0; i < number; i++)
  {
    // file open successfully, now we need to write into the file
    fprintf(out, "\n%s %s %s", user[i].name, user[i].phone, user[i].email);
  }

  fclose(out);
}

// Add new contact
void addContact()
{
  int num;

  printf("Enter number of user you like to add: ");
  // less than 100 user in a time
  scanf("%2d", &num);

  // Clear the input buffer to avoid issues with fgets
  // Exp: Enter user's name: Enter user's phone number:
  while (getchar() != '\n')
    ;

  User user[80];

  for (int i = 0; i < num; i++)
  {
    printf("Contact %d: \n", i + 1);
    printf("Enter user's name: ");
    fgets(user[i].name, 50, stdin);

    printf("Enter user's phone number (601xxxxxx): ");
    fgets(user[i].phone, 20, stdin);

    printf("Enter user's Email: ");
    fgets(user[i].email, 50, stdin);

    // replace the newline to null terminator
    user[i].name[strcspn(user[i].name, "\n")] = '\0';
    user[i].email[strcspn(user[i].email, "\n")] = '\0';
    user[i].phone[strcspn(user[i].phone, "\n")] = '\0';

    // in order to make the comparison (sort) easier and fair, we decided to convert the strings to lowerCase
    for (int j = 0; j < MAX_PROPERTY_LENGTH; j++)
    { // 50 is the max length of User's property
      user[i].name[j] = tolower(user[i].name[j]);
      user[i].email[j] = tolower(user[i].email[j]);
    }

    // input validate (pass == 1  fail == 0)
    int emailRes = isValidEmail(user[i].email);
    int phoneRes = isValidNumber(user[i].phone);

    if (phoneRes != 1 && emailRes != 1)
    {
      printf("Phone number and email format is not correct!");
      continue;
    }
    if (phoneRes != 1)
    {
      printf("Phone number format is not correct!");
      continue;
    }
    if (emailRes != 1)
    {
      printf("Email format is not correct!");
      continue;
    }
    saveToFile(user[i]);
  }
}

// read file
int readFile(const char *filename, User list[], int maxUsers)
{
  FILE *file = fopen(filename, "r");
  if (file == NULL)
  {
    return -1; // Return error code
  }

  char line[MAX_LENGTH]; // number of char that a line can store
  int count = 0;         // used to record the index of list array

  while (fgets(line, sizeof(line), file) && count < maxUsers)
  {
    // store it in the struct array (list)
    if (sscanf(line, "%49s %19s %49s", list[count].name, list[count].phone, list[count].email) == 3)
    {
      count++;
    }
    else
    {
      printf("Error");
    }
  }

  fclose(file);
  return count; // Return the number of users read from file
}

// Sort
// Function of bubble sort for contact name
void sortContact()
{
  // if we get -1 means error occur
  int userCount = readFile("contact.txt", list, 80);

  // error handling
  if (userCount == 0)
  {
    printf("No users in the file. \n");
    return;
  }

  if (userCount < 0)
  { // error occur
    printf("Some error occurred when open the file");
    return;
  }

  // ask user want sort by using name / phone / email
  printf("The sorting result will permanently store in the database!\n");
  printf("1. Name \n");
  printf("2. Phone Number\n");
  printf("3. Email\n");
  printf("Sort by: ");

  // ask user input
  int res;
  scanf("%d", &res);
  getchar();

  if (res < 1 || res > 3)
  {
    printf("Invalid input");
    exit(1);
  }

  for (int i = 0; i < userCount - 1; i++)
  {
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

      if (compareRes > 0)
      { // j greater than j+1
        User userTemp = list[j + 1];
        list[j + 1] = list[j];
        list[j] = userTemp;
      }
    }
  }

  // update into files
  saveToFileBatch(list, userCount);

  printf("User after sorted\n");
  printf("no. Name Phone Email\n");

  for (int i = 0; i < userCount; i++)
  {
    printf("%d. %s %s %s\n", i + 1, list[i].name, list[i].phone, list[i].email);
  }
}

// display user
int displayUser()
{
  // if we get -1 means error occur
  int userCount = readFile("contact.txt", list, 80);

  // error handling
  if (userCount == 0)
  {
    printf("No users. \n");
    return 1;
  }

  if (userCount < 0)
  { // error occur
    printf("Some error occurred when open the file");
    return 1;
  }

  printf(" %d users found:\n", userCount);

  printf("Name    Phone    Email\n");
  // loop to print the user
  for (int i = 0; i < userCount; i++)
  {
    printf("%d. %s %s  %s\n", i + 1, list[i].name, list[i].phone, list[i].email);
  }

  return userCount;
}

// edit user
void editContact()
{
  // display the user so they can choose the contact to edit
  int userCount = displayUser();
  // user input to edit the user
  int userToEdit;
  printf("Choose the index of the contact to edit: ");
  scanf("%d", &userToEdit);
  getchar();
  // error handling
  if (userToEdit < 1 || userToEdit > userCount)
  {
    printf("Invalid input!\n");
    exit(1); // exit with error
  }
  // ask user want to edit name / phone / email / all
  printf("1. Name\n");
  printf("2. Phone\n");
  printf("3. Email\n");
  printf("4. All\n");
  printf("Which part would you like to edit: ");
  int res; // used to record user answer for edit part
  scanf("%d", &res);
  getchar();
  switch (res)
  {
  case 1: // Name
  {
    char newName[MAX_PROPERTY_LENGTH];
    printf("Enter new user's name: ");
    fgets(newName, 50, stdin);

    newName[strcspn(newName, "\n")] = '\0';

    for (int j = 0; j < MAX_PROPERTY_LENGTH; j++)
    {
      newName[j] = tolower(newName[j]);
    }
    strcpy(list[userToEdit - 1].name, newName);
    break;
  }
  case 2: // Phone
  {
    char newPhone[MAX_PROPERTY_LENGTH];
    printf("Enter new phone number: ");
    fgets(newPhone, 50, stdin);

    newPhone[strcspn(newPhone, "\n")] = '\0';

    int phoneRes = isValidNumber(newPhone);
    if (phoneRes != 1)
    {
      printf("Phone number format is not correct!");
      exit(1);
    }

    strcpy(list[userToEdit - 1].phone, newPhone);
    break;
  }
  case 3: // Email
  {
    char newEmail[MAX_PROPERTY_LENGTH];
    printf("Enter new user's email: ");
    fgets(newEmail, 50, stdin);

    newEmail[strcspn(newEmail, "\n")] = '\0';

    int emailRes = isValidEmail(newEmail);
    if (emailRes != 1)
    {
      printf("Email format is not correct!");
      exit(1);
    }

    for (int j = 0; j < MAX_PROPERTY_LENGTH; j++)
    {
      newEmail[j] = tolower(newEmail[j]);
    }
    strcpy(list[userToEdit - 1].email, newEmail);
    break;
  }
  case 4: // All
  {
    char newName[MAX_PROPERTY_LENGTH];
    char newPhone[MAX_PROPERTY_LENGTH];
    char newEmail[MAX_PROPERTY_LENGTH];

    printf("Enter new user's name: ");
    fgets(newName, 50, stdin);
    printf("Enter new phone number (601xxxxxx): ");
    fgets(newPhone, 50, stdin);
    printf("Enter new user's email: ");
    fgets(newEmail, 50, stdin);

    newName[strcspn(newName, "\n")] = '\0';
    newPhone[strcspn(newPhone, "\n")] = '\0';
    newEmail[strcspn(newEmail, "\n")] = '\0';

    for (int j = 0; j < MAX_PROPERTY_LENGTH; j++)
    {
      newName[j] = tolower(newName[j]);
      newEmail[j] = tolower(newEmail[j]);
    }

    int emailRes = isValidEmail(newEmail);
    int phoneRes = isValidNumber(newPhone);

    if (phoneRes != 1 && emailRes != 1)
    {
      printf("Phone number and email format are not correct!");
      exit(1);
    }
    if (phoneRes != 1)
    {
      printf("Phone number format is not correct!");
      exit(1);
    }
    if (emailRes != 1)
    {
      printf("Email format is not correct!");
      exit(1);
    }

    strcpy(list[userToEdit - 1].name, newName);
    strcpy(list[userToEdit - 1].phone, newPhone);
    strcpy(list[userToEdit - 1].email, newEmail);
    break;
  }
  default:
    printf("Invalid input!\n");
    exit(1);
  }

  // write new result into the file
  saveToFileBatch(list, userCount);

  printf("Edit Successfully!\n");
}

// delete user
void deleteByName()
{
  int userCount = displayUser();

  while (getchar() != '\n')
    ; // Flush leftover characters

  // user input
  char deletedName[MAX_PROPERTY_LENGTH];
  printf("Enter new user's name: ");
  fgets(deletedName, 50, stdin);

  deletedName[strcspn(deletedName, "\n")] = '\0';

  for (int j = 0; j < MAX_PROPERTY_LENGTH; j++)
  {
    deletedName[j] = tolower(deletedName[j]);
  }

  int found = 0; // use to record whether found the name in file,if not then print not found
  // find the name in the array that same as user input's name
  int deletedIndex[userCount]; // used to record the index of deleted contact
  int index = 0;               // used to record deletedIndex array's index
  printf("no. Name Phone Email");
  for (int i = 0; i < userCount; i++)
  {
    if (strcmp(list[i].name, deletedName) == 0)
    { // equal to 0 means same
      printf("%d. %s %s %s\n", i + 1, list[i].name, list[i].phone, list[i].email);
      deletedIndex[index] = i;
      index++;
      found++;
    }
  }

  if (found == 0)
  { // if no contact found
    printf("no contact found...");
    return;
  }

  if (found > 1)
  { // means only 1 contact found
    printf("We found %d contact with same name would you like to delete 1 or all? ");
    printf("1. One");
    printf("2. All");

    int deleteSelection;
    scanf("%d", &deleteSelection);
    getchar();

    if (deleteSelection == 1)
    { // ask user to choose one number from found contact list
      int decision;
      scanf("%d", &decision);
      getchar();

      for (int i = 0; i < index; i++)
      {
        if (deletedIndex[i] == decision - 1)
        {
          // Shift the remaining users (delete user from actual list)
          for (int j = decision - 1; j < userCount - 1; j++)
          {
            list[j] = list[j + 1];
          }
          userCount--; // Decrease the count
          printf("User '%s' deleted successfully.\n", deletedName);
          saveToFileBatch(list, userCount);
          return;
        }
      }
    }
    if (deleteSelection == 2)
    {
      printf("Delete confirmation (y/n): ");
      char res;
      scanf("%c", &res);
      getchar();

      switch (res)
      {
      case 'y':
      case 'Y':
        // delete all function goes here
        break;
      case 'n':
      case 'N':
        printf("Cancelled");
        break;
      default:
        printf("Invalid Input!");
        exit(1);
      }
      return; // to separate with delete single contact
    }
  }

  int dataToDelete = deletedIndex[0];
  for (int k = dataToDelete; k < userCount - 1; k++)
  {
    list[k] = list[k + 1];
  }
  userCount--;
  printf("User '%s' deleted successfully.\n", deletedName);
  saveToFileBatch(list, userCount);
  return;
}

// clear contact
void clearContact()
{

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
      printf("Some error occurred when open the file");
      exit(1);
    }
    else
    {
      printf("Data cleared successfully!\n");
    }
    break;
  case 'n':
  case 'N':
    printf("Cancel...\n");
    break;
  default:
    printf("Invalid input\n");
  }
}

int main()
{
  // used to record the decision
  int decision;

  while (1)
  {
    showMenu();

    printf("Enter a number: ");
    scanf("%d", &decision);

    // all main function going here
    switch (decision)
    {
    case 1: // Add
      addContact();
      break;
    case 2: // Edit
      editContact();
      break;
    case 3: // Delete
      deleteByName();
      break;
    case 4: // Search

      break;
    case 5: // Sort
      sortContact();
      break;
    case 6: // Display
      displayUser();
      break;
    case 7: // Clear
      clearContact();
      break;
    case 8: // exit
      printf("Thank for using. Please visit us again\n");
      exit(0); // exit with successfully (0 mean success | 1 mean failed)
    default:
      printf("Invalid number, please enter again\n");
      return 0;
    }
  }
}
