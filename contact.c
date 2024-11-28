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
int displayUser();
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

// print LOGO when program start
void printLogo()
{
  printf("\n\n");

  // Print the word "FLUTTERSHY" in pink
  printf("  FFFFFFF  L         U   U  TTTTT   TTTTT  EEEEE  RRRRR    SSSS   H   H   Y   Y\n");
  printf("  F        L         U   U    T       T    E      R   R   S       H   H    Y Y\n");
  printf("  FFFFF    L         U   U    T       T    EEEE   RRRRR    SSSS    HHHH     Y\n");
  printf("  F        L         U   U    T       T    E      R  R        S   H   H     Y\n");
  printf("  F        LLLLL     UUUUU    T       T    EEEEE  R   R   SSSS    H    H    Y\n ");
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
      temp[j] = temp[j] - ENCRYPT_VAL;
    }
    temp[strlen(temp)] = '\0';
    memmove(temp, temp + saltLength, strlen(temp) - saltLength + 1); // Remove salt
    strncpy(user[i].name, temp, sizeof(user[i].name) - 1);
    user[i].name[sizeof(user[i].name) - 1] = '\0';

    // phone
    strcpy(temp, user[i].phone);
    for (int j = 0; j < strlen(temp); j++)
    {
      temp[j] = temp[j] - ENCRYPT_VAL;
    }
    temp[strlen(temp)] = '\0';
    memmove(temp, temp + saltLength, strlen(temp) - saltLength + 1); // Remove salt
    strncpy(user[i].phone, temp, sizeof(user[i].phone) - 1);
    user[i].phone[sizeof(user[i].phone) - 1] = '\0';

    // email
    strcpy(temp, user[i].email);
    for (int j = 0; j < strlen(temp); j++)
    {
      temp[j] = temp[j] - ENCRYPT_VAL;
    }
    temp[strlen(temp)] = '\0';
    memmove(temp, temp + saltLength, strlen(temp) - saltLength + 1); // Remove salt
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
  printf("\nWelcome to FlutterShy Contact Management System!\n");
  printf("============================================\n");
  printf("============ 1. Add New Contact ============\n");
  printf("============ 2. Edit Contact    ============\n");
  printf("============ 3. Delete Contact  ============\n");
  printf("============ 4. Search Contact  ============\n");
  printf("============ 5. Sort Contact    ============\n");
  printf("============ 6. Display Contact ============\n");
  printf("============ 7. Restore Contact ============\n");
  printf("============ 8. Clear Contact   ============\n");
  printf("============ 9. Exit            ============\n");
  printf("============================================\n");
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
  printf("%-5s %-25s %-15s %-30s\n", "no.", "Name", "Phone", "Email");
  printf("-----------------------------------------------------------------------------\n");

  for (int i = 0; i < userCount; i++)
  {
    printf("%-5d %-25s %-15s %-30s\n", i + 1, list[i].name, list[i].phone, list[i].email);
  }
}

// Function to validate email Need to change
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

// Add new contact
void addContact()
{
  int num;

  printf("Enter number of user you like to add (max:%d): ", MAX_USERS);
  // less than 100 user in a time
  scanf("%d", &num);
  // Clear the input buffer to avoid issues with fgets
  // Exp: Enter user's name: Enter user's phone number:
  getchar();

  // error handling with invalid input
  if (num <= 0 || num > MAX_USERS)
  {
    printf("Invalid number of users!\n");
    return;
  }

  for (int i = 0; i < num; i++)
  {
    User user;
    printf("Contact %d: \n", i + 1);
    printf("Enter user's name: ");
    readLine(user.name, sizeof(user.name));

    // in order to make the comparison (sort) easier and fair, we decided to convert the strings to lowerCase
    toLowerCase(user.name);

    while (1)
    {
      printf("Enter user's phone number (601xxxxxx): ");
      readLine(user.phone, sizeof(user.phone));
      int phoneRes = isValidNumber(user.phone); // input validate (pass == 1  fail == 0)
      if (phoneRes != 1)
      {
        printf("Phone number format is not correct!\n");
      }
      else
      {
        break;
      }
    }

    while (1)
    {
      printf("Enter user's Email: ");
      readLine(user.email, sizeof(user.email));
      // in order to make the comparison (sort) easier and fair, we decided to convert the strings to lowerCase
      toLowerCase(user.email);
      int emailRes = isValidEmail(user.email); // input validate (pass == 1  fail == 0)

      if (emailRes != 1)
      {
        printf("Email format is not correct!\n");
      }
      else
      {
        break;
      }
    }

    // error handling for saveToFile function
    if (saveToFile(user) > 0)
    {
      printf("User added successfully!\n");
    }
    else
    {
      printf("Failed to add user,Please try again...\n");
    }
  }
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
    char tempEmail[50] = "";

    // Split the line into tokens
    char *token = strtok(line, " \n");
    int tokenCount = 0;

    char *tokens[MAX_LENGTH / 2]; // Array to hold tokens

    // Collect all tokens in the line
    while (token != NULL)
    {
      tokens[tokenCount++] = token;
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

// Sort
// Function of bubble sort for contact name
void sortContact()
{
  // if we get -1 means error occur
  int userCount = readFile("contact.txt", list, 80);

  // error handling
  if (userCount == 0)
  {
    printf("No users found in the file. \n");
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

  printf("User after sorted\n");
  printList(userCount);

  // update into files
  saveToFileBatch(list, userCount);
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

  printf("%d users found:\n", userCount);

  // loop to print the user
  printList(userCount);

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
    return; // exit with error
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

    toLowerCase(newName);

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
      return;
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

    printf("Enter new user's name: ");
    fgets(newName, 50, stdin);
    printf("Enter new phone number (601xxxxxx): ");
    fgets(newPhone, 50, stdin);
    printf("Enter new user's email: ");
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
      printf("Phone number and email format are not correct!");
      return;
    }
    if (phoneRes != 1)
    {
      printf("Phone number format is not correct!");
      return;
    }
    if (emailRes != 1)
    {
      printf("Email format is not correct!");
      return;
    }

    strcpy(list[userToEdit - 1].name, newName);
    strcpy(list[userToEdit - 1].phone, newPhone);
    strcpy(list[userToEdit - 1].email, newEmail);
    break;
  }
  default:
    printf("Invalid input!\n");
    return;
  }

  // write new result into the file
  saveToFileBatch(list, userCount);

  printf("Edit Successfully!\n");
}

// search name that are use for both search and delete function (it also can do the partial search)
int searchContactByName(const char *name, int *indices, int maxIndices)
{
  int userCount = readFile("contact.txt", list, 80);

  if (userCount <= 0)
  {
    // if 0 print no user found, others must be less than 0
    printf(userCount == 0 ? "No users found!\n" : "Some error occurred when open the file...\n");
    return -1; // Return -1 for errors, 0 for no users
  }

  // ask for user input
  char searchName[MAX_PROPERTY_LENGTH];
  strncpy(searchName, name, MAX_PROPERTY_LENGTH);
  searchName[MAX_PROPERTY_LENGTH - 1] = '\0';

  // Convert the search name to lowercase
  toLowerCase(searchName);

  int found = 0;
  // printf("no. Name Phone Email\n");
  for (int i = 0; i < userCount; i++)
  {
    char lowerName[MAX_PROPERTY_LENGTH];
    strcpy(lowerName, list[i].name);
    lowerName[MAX_PROPERTY_LENGTH - 1] = '\0';

    // here is the key that allow us to do the partial function
    if (strncmp(lowerName, searchName, strlen(searchName)) == 0)
    {
      printf("%d. %s %s %s\n", i + 1, list[i].name, list[i].phone, list[i].email);

      if (indices != NULL && found < maxIndices)
      {
        indices[found] = i; // Store the index of the match
      }
      found++;
    }
  }

  if (found == 0)
  {
    printf("No contacts found with the name '%s'.\n", name);
  }

  return found; // Return the number of matches found
}

// search by name
void searchContact()
{
  char name[MAX_PROPERTY_LENGTH];
  printf("Enter the name you want to search for: ");
  fgets(name, MAX_PROPERTY_LENGTH, stdin);
  name[strcspn(name, "\n")] = '\0';

  int result = searchContactByName(name, NULL, 0); // NULL means we don't need indices

  if (result > 0)
  {
    printf("Total contact found: %d\n", result);
  }
}

// delete user
void deleteByName()
{

  int userCount = readFile("contact.txt", list, 80);

  if (userCount == -1)
  {
    printf("Error reading the file.\n");
    return;
  }
  if (userCount == 0)
  {
    printf("No users found.\n");
    return;
  }

  // user input
  char deletedName[MAX_PROPERTY_LENGTH];
  printf("Enter name you would like to delete : ");
  fgets(deletedName, 50, stdin);

  deletedName[strcspn(deletedName, "\n")] = '\0';

  int indices[80]; // use to store the indices if the name match
  int result = searchContactByName(deletedName, indices, 80);

  if (result <= 0)
  {
    return;
  }

  if (result > 1)
  { // means only 1 contact found
    printf("We found %d contact with same name would you like to delete 1 or all? \n", result);
    printf("1. Delete One\n2. Delete All\n");
    printf("User input: ");

    int deleteSelection;
    scanf("%d", &deleteSelection);
    getchar();

    if (deleteSelection == 1)
    { // ask user to choose one number from found contact list
      int decision;
      printf("Which would you like to delete (index): ");
      scanf("%d", &decision);
      getchar();

      int deleteIndex = indices[deleteSelection - 1];

      for (int i = deleteIndex; i < userCount; i++)
      {
        if (indices[i] == decision - 1)
        {
          char returnName[MAX_PROPERTY_LENGTH];
          strcpy(returnName, list[i].name);
          // store the user into the restoreUserList
          restoreList[count++] = list[i];

          // // print the restore List (debug purposes)
          // for (int k = 0; k < count; k++)
          // {
          //   printf("%s %s %s\n", restoreList[k].name, restoreList[k].phone, restoreList[k].email);
          // }

          // Shift the remaining users (delete user from actual list)
          for (int j = decision - 1; j < userCount - 1; j++)
          {
            list[j] = list[j + 1];
          }
          userCount--; // Decrease the count
          printf("User '%s' deleted successfully.\n", returnName);
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
      {
        // Start batch deletion
        int remainingCount = userCount;
        for (int i = 0; i < result; i++)
        {
          int deleteIndex = indices[i];
          // add the deleted user into the restore list
          restoreList[count++] = list[deleteIndex];

          // Adjust the list array to remove this index
          for (int j = deleteIndex; j < remainingCount - 1; j++)
          {
            list[j] = list[j + 1];
          }
          remainingCount--; // Reduce the count after each deletion

          // Adjust subsequent deletedIndex values
          for (int k = i + 1; k < result; k++)
          {
            if (indices[k] > deleteIndex)
            {
              indices[k]--; // Shift to the left for proper indices
            }
          }
        }
        userCount = remainingCount; // Update user count
        printf("Users deleted successfully.\n");

        saveToFileBatch(list, userCount);
      }
      break;
      case 'n':
      case 'N':
        printf("Cancelled");
        break;
      default:
        printf("Invalid Input!");
        return;
      }
      return; // to separate with delete single contact
    }
  }

  int dataToDelete = indices[0];
  // copy the name of deleted user
  char userDeletedName[MAX_PROPERTY_LENGTH];
  strcpy(userDeletedName, list[dataToDelete].name);
  // store the user into the restoreUserList
  restoreList[count++] = list[dataToDelete];
  for (int k = dataToDelete; k < userCount - 1; k++)
  {
    list[k] = list[k + 1];
  }
  userCount--;
  printf("User '%s' deleted successfully.\n", userDeletedName);
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

// restore contact (data will lost if program terminates)
void restoreContact()
{

  if (count <= 0)
  {
    printf("No users found!\n");
    return;
  }
  printf("Total of %d user in the list:\n", count);

  printf("no. Name Phone Email\n");
  for (int i = 0; i < count; i++)
  {
    printf("%d. %s %s %s\n", i + 1, restoreList[i].name, restoreList[i].phone, restoreList[i].email);
  }

  printf("Which user you would like to restore (index, -1 to cancel):");
  int choice;
  scanf("%d", &choice);
  getchar();
  // if user input -1
  if (choice == -1)
  {
    printf("Cancelled\n");
    return;
  }

  if (choice < -1 || choice > count)
  {
    printf("Invalid input!\n");
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
  printf("User '%s' restored successfully.\n", restoredName);
}

int main()
{
  // print out the logo
  printLogo();

  // used to record the decision
  int decision;

  while (1)
  {
    showMenu();

    printf("Enter a number: ");
    scanf("%d", &decision);
    getchar();

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
      searchContact();
      break;
    case 5: // Sort
      sortContact();
      break;
    case 6: // Display
      displayUser();
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
      printf("Invalid choice, please enter again\n");
      return 0;
    }
  }
}