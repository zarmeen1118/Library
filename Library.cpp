#include <iostream>
#include <stdlib.h>
#include <fstream>
using namespace std;

class Helper
{
public:
	static int StringLenght(char* str)	// This function calculates length of a c-string
	{
		int length = 0;
		for (int i = 0; str[i] != '\0'; i++)
		{
			length++;
		}
		return length;
	}

	static char* GetStringFromBuffer(char* str)	// For deep copy and allocation of c-string
	{
		int length = StringLenght(str);
		char* myStr = 0;
		myStr = new char[length + 1];
		int temp = 0;

		for (int i = 0; str[i] != '\0'; i++, temp++)
		{
			myStr[temp] = str[i];
		}
		myStr[temp] = '\0';

		return myStr;
	}

	static bool SearchString(char* str, char* searchString)	// Compares two strings to see if they are the same
	{
		if (StringLenght(str) != StringLenght(searchString))
		{
			return false;
		}
		for (int i = 0; i < StringLenght(str); i++)
		{
			if (str[i] != searchString[i])
			{
				return false;
			}
		}

		return true;
	}
};

class Book
{
private:
	int ID;
	char* Name;
	char* Type;
	char* Author;
	int Issued;
public:
	Book()	// Default constructor
	{
		ID = 0;
		Name = 0;
		Type = 0;
		Author = 0;
		Issued = 0;
	}

	~Book()	// Destructor
	{
		if (Name != 0)
		{
			delete[] Name;
			Name = 0;
		}

		if (Type != 0)
		{
			delete[] Type;
			Type = 0;
		}

		if (Author != 0)
		{
			delete[] Author;
			Author = 0;
		}
	}

	Book(const Book& rhs)	// Copy Constructor
	{
		ID = rhs.ID;
		Name = Helper::GetStringFromBuffer(rhs.Name);
		Type = Helper::GetStringFromBuffer(rhs.Type);
		Author = Helper::GetStringFromBuffer(rhs.Author);
		Issued = rhs.Issued;
	}

	void LoadFromFile(ifstream& fin)	// This function loads the data of a single book from the file
	{
		char name[50];
		char type[20];
		char author[30];

		fin >> ID;
		fin.ignore();

		fin.getline(name, 50);
		Name = Helper::GetStringFromBuffer(name);

		fin.getline(type, 20);
		Type = Helper::GetStringFromBuffer(type);

		fin.getline(author, 30);
		Author = Helper::GetStringFromBuffer(author);

		fin >> Issued;
	}

	int GetID()	// ID getter
	{
		return ID;
	}

	void SetStatus(int status)	// Availability status setter
	{
		Issued = status;
	}

	int GetStatus()	// Availability status getter
	{
		return Issued;
	}

	void PrintBook()	// Function to print details of a single book
	{
		cout << "\tID:\t" << ID;
		cout << "\n\tName:\t" << Name;
		cout << "\n\tGenre:\t" << Type;
		cout << "\n\tAuthor:\t" << Author;

		cout << "\n\tStatus:\t";
		if (Issued == 0)
		{
			cout << "Available";
		}
		else if (Issued == 1)
		{
			cout << "Not Available";
		}
	}

	void EditBook()	// Function to edit book details
	{
		Name = 0;
		Type = 0;
		Author = 0;

		char name[50];
		char type[20];
		char author[30];

		cin.ignore();
		cout << "\nEnter Name:\t";
		cin.getline(name, 50);
		cout << "Enter Genre:\t";
		cin.getline(type, 20);
		cout << "Enter Author:\t";
		cin.getline(author, 30);

		Name = Helper::GetStringFromBuffer(name);
		Type = Helper::GetStringFromBuffer(type);
		Author = Helper::GetStringFromBuffer(author);
	}

	void SaveBook(ofstream& fout)	// This function saves the details of a single book into the file
	{
		fout << ID << endl;
		fout << Name << endl;
		fout << Type << endl;
		fout << Author << endl;
		fout << Issued << endl;
	}

	void SetID(int id)	// ID setter
	{
		ID = id;
	}
};

class Member
{
private:
	char* UserName;
	char* Password;
	Book* IssuedBook;
public:
	Member()	// Default constructor
	{
		UserName = 0;
		Password = 0;
		IssuedBook = 0;
	}

	~Member()	// Destructor
	{
		if (UserName != 0)
		{
			delete[] UserName;
			UserName = 0;
		}
		if (Password != 0)
		{
			delete[] Password;
			Password = 0;
		}
	}

	Member(const Member& rhs)	// Copy constructor
	{
		UserName = Helper::GetStringFromBuffer(rhs.UserName);
		Password = Helper::GetStringFromBuffer(rhs.Password);
		IssuedBook = rhs.IssuedBook;
	}

	void LoadFromFile(ifstream& fin)	// This function loads data of a single member from file
	{
		char user[15];
		char pass[15];

		fin >> user;
		fin >> pass;
		
		UserName = Helper::GetStringFromBuffer(user);
		Password = Helper::GetStringFromBuffer(pass);
	}

	void SetIssuedBook(Book*& address)	// IssuedBook setter
	{
		IssuedBook = address;
	}

	Book* GetIssuedBook()	// IssuedBook getter
	{
		return IssuedBook;
	}

	char* GetUserName()	// Username getter
	{
		return UserName;
	}

	char* GetPassword()	// Password getter
	{
		return Password;
	}

	void SaveMember(ofstream& ofile)	// This function saves the data of a single member into the file
	{
		ofile << UserName << endl;
		ofile << Password << endl;

		if (IssuedBook == 0)
		{
			ofile << "-1" << endl;	// If the member has not issued any book
		}
		else
		{
			ofile << IssuedBook->GetID() << endl;
		}
	}

	void EditMember()	// To edit the details of a new member created at signing up
	{
		UserName = 0;
		Password = 0;
		IssuedBook = 0;

		char name[20];
		char pass[20];

		cout << "\nWelcome new User! Enter your info for signing up.\n";
		cout << "\nEnter Username:\t";
		cin >> name;
		cout << "Enter Password:\t";
		cin >> pass;

		UserName = Helper::GetStringFromBuffer(name);
		Password = Helper::GetStringFromBuffer(pass);
	}
};

class Librarian
{
private:
	Book** BookList;
	Member** MemberList;
	int totalBooks;
	int totalMembers;
	Member* CurrentUser;
public:
	Librarian()	// Default constructor
	{
		BookList = 0;
		MemberList = 0;
		totalBooks = 0;
		totalMembers = 0;
		CurrentUser = 0;
	}

	Librarian(const Librarian& rhs)	// Copy Constructor
	{
		CurrentUser = rhs.CurrentUser;
		totalBooks = rhs.totalBooks;
		totalMembers = rhs.totalMembers;

		BookList = new Book*[totalBooks];
		MemberList = new Member*[totalMembers];

		for (int i = 0; i < totalBooks; i++)
		{
			BookList[i] = new Book;
			BookList[i] = rhs.BookList[i];
		}

		for (int i = 0; i < totalMembers; i++)
		{
			MemberList[i] = new Member;
			MemberList[i] = rhs.MemberList[i];
		}
	}

	~Librarian()	// Destructor
	{
		if (BookList != 0)
		{
			for (int i = 0; i < totalBooks; i++)
			{
				if (BookList[i] != 0)
				{
					delete BookList[i];
					BookList[i] = 0;
				}
			}
			delete[] BookList;
			BookList = 0;
		}

		if (MemberList != 0)
		{
			for (int i = 0; i < totalMembers; i++)
			{
				if (MemberList[i] != 0)
				{
					delete MemberList[i];
					MemberList[i] = 0;
				}
			}
			delete[] MemberList;
			MemberList = 0;
		}
	}

	void LoadBooksData()	// Function to load data of all books from file
	{
		ifstream fin("BookData.txt");
		fin >> totalBooks;
		BookList = new Book*[totalBooks];

		for (int i = 0; i < totalBooks; i++)
		{
			BookList[i] = new Book;
			BookList[i]->LoadFromFile(fin);	// Calls function of each book to load its respective data
		}

		fin.close();
	}

	void SaveData()	// Function to save all data into the file
	{
		ofstream fout;
		fout.open("BookData.txt", ofstream::out);

		fout << totalBooks << endl;
		for (int i = 0; i < totalBooks; i++)
		{
			BookList[i]->SaveBook(fout);	// Calls save function of each book to save its respective data
		}
		fout.close();

		ofstream ofile;
		ofile.open("Members.txt", ofstream::out);

		ofile << totalMembers << endl;
		for (int i = 0; i < totalMembers; i++)
		{
			MemberList[i]->SaveMember(ofile);	// Similarly with members
		}
		ofile.close();
	}

	Book* SearchBookByID(int tempID)	// This functions compares book IDs with a given ID and returns the book's address when found
	{
		for (int i = 0; i < totalBooks; i++)
		{
			if (BookList[i]->GetID() == tempID)
			{
				return BookList[i];
			}
		}
		return 0;
	}

	void AddBook()	// Function to add a book in the list
	{
		totalBooks = totalBooks + 1;	// Book count increased
		Book** tempList = new Book*[totalBooks];

		for (int i = 0; i < totalBooks - 1; i++)
		{
			tempList[i] = BookList[i];
		}

		delete[] BookList;
		BookList = tempList;

		BookList[totalBooks - 1] = new Book;
		BookList[totalBooks - 1]->EditBook();	// Setting the details of the new book created in the following lines
		BookList[totalBooks - 1]->SetID(BookList[totalBooks - 2]->GetID() + 1);
		BookList[totalBooks - 1]->SetStatus(0);
	}

	void AddMember()	// Function to add a member in the list of members
	{
		totalMembers = totalMembers + 1;	// Count increases
		Member** tempList = new Member*[totalMembers];

		for (int i = 0; i < totalMembers - 1; i++)
		{
			tempList[i] = MemberList[i];
		}

		delete[] MemberList;
		MemberList = tempList;

		MemberList[totalMembers - 1] = new Member;
		MemberList[totalMembers - 1]->EditMember();	// Setting details of the new member
		SetCurrentUser(MemberList[totalMembers - 1]);	// Signing up so setting as current user
	}

	void RemoveBook()	// This function removes a book from the book list
	{
		int bookID = 0;
		cout << "Enter ID of book you want to remove:\t";
		cin >> bookID;
		Book* bookToRemove = 0;
		bookToRemove = SearchBookByID(bookID);

		if (bookToRemove == 0)	// If ID does not match
		{
			cout << "\nBook not found.";
			return;
		}

		totalBooks = totalBooks - 1;	// Decreasing count
		Book** tempList = new Book*[totalBooks];

		for (int i = 0, j = 0; i < totalBooks; i++, j++)
		{
			if (BookList[j] == bookToRemove)
			{
				delete BookList[j];
				BookList[j] = 0;
				j++;
			}
			tempList[i] = BookList[j];
		}

		delete[] BookList;
		BookList = tempList;
	}

	void ViewIssuedBooks()	// Shows books which have been issued
	{
		for (int i = 0; i < totalMembers; i++)
		{
			if (MemberList[i]->GetIssuedBook() != 0)	// If the member has issued any book then shows its details
			{
				cout << "Issued By:\t" << MemberList[i]->GetUserName();
				cout << "\nDetails of Book Issued:\n";
				MemberList[i]->GetIssuedBook()->PrintBook();
				cout << endl << endl;
			}
		}
	}

	void EditBook()	// Edits deatils of a book
	{
		int bookID = 0;
		cout << "Enter ID of book you want to edit:\t";
		cin >> bookID;
		Book* bookToEdit = 0;
		bookToEdit = SearchBookByID(bookID);

		if (bookToEdit == 0)
		{
			cout << "\nBook not found.";
			return;
		}
		else
		{
			bookToEdit->EditBook();
			cout << "\nSuccessfully edited book!";
		}
	}

	void ReturnBook()	// Returns an issued book
	{
		if (CurrentUser->GetIssuedBook() == 0)
		{
			cout << "\nYou have not issued any book to return it. Issue a book first.";
		}
		else
		{
			CurrentUser->GetIssuedBook()->SetStatus(0);
			Book* temp = NULL;
			CurrentUser->SetIssuedBook(temp);
			cout << "\nSuccessfully returned book!";
		}
	}

	void SearchBook()	// Searches a book by its ID
	{
		int bookID = 0;
		cout << "Enter ID of book you want to search:\t";
		cin >> bookID;
		Book* bookToSearch = 0;
		bookToSearch = SearchBookByID(bookID);

		if (bookToSearch == 0)
		{
			cout << "\nBook not found.";
			return;
		}
		else
		{
			cout << "\nBook Detail:\n";
			bookToSearch->PrintBook();
		}
	}

	void IssueBook()	// This function issues a book
	{
		int bookID = 0;
		cout << "Enter ID of book you want to issue:\t";
		cin >> bookID;
		Book* bookToIssue = 0;
		bookToIssue = SearchBookByID(bookID);

		if (bookToIssue == 0)
		{
			cout << "\nBook not found.";
			return;
		}
		else
		{
			if (CurrentUser->GetIssuedBook() != 0)	// A member can only issue one book at a time
			{
				cout << "\nYou have already issued a book. First return your issued book.";
			}
			else
			{
				if (bookToIssue->GetStatus() == 1)	// Book already issued by someone
				{
					cout << "\nBook is not available for issue. Issue another book.";
				}
				else
				{
					CurrentUser->SetIssuedBook(bookToIssue);
					bookToIssue->SetStatus(1);
					cout << "\nSuccessfully issued book!";
				}
			}
		}
	}

	void LoadAllMembers()	// Loads the data of all members from file
	{
		ifstream fin("Members.txt");
		fin >> totalMembers;
		MemberList = new Member*[totalMembers];

		for (int i = 0; i < totalMembers; i++)
		{
			MemberList[i] = new Member;
			MemberList[i]->LoadFromFile(fin);	// Calls load function of each member

			int tempID = 0;
			fin >> tempID;
			Book* temp = 0;

			if (tempID != -1)
			{
				temp = SearchBookByID(tempID);
				MemberList[i]->SetIssuedBook(temp);
			}
		}

		fin.close();
	}

	void SetCurrentUser(Member*& user)	// Currentuser setter
	{
		CurrentUser = user;
	}

	Member* GetCurrentUser()	// Currentuser getter
	{
		return CurrentUser;
	}

	bool CompareCredentials(char* user, char* pass)	// Function to verify username and password for member login
	{
		bool u = false;
		bool p = false;

		for (int i = 0; i < totalMembers; i++)
		{
			u = Helper::SearchString(MemberList[i]->GetUserName(), user);
			p = Helper::SearchString(MemberList[i]->GetPassword(), pass);

			if (u == true && p == true)
			{
				SetCurrentUser(MemberList[i]);
				return true;
			}
		}

		return false;
	}
};

void main()
{
	cout << "\n----------Welcome to Library!----------\n\n";
	char panel = ' ';
	char temp;
	char* username;
	char* password;
	char buffer1[20];
	char buffer2[20];

	while (panel != 'Q' && panel != 'q')
	{
		cout << "\n----------Main Menu----------\n\n";
		cout << "Are you a Member or a Librarian? \nPress M for Member and L for Librarian.\nPress Q if you want to exit the library.\t";
		cin >> panel;

		if (panel == 'M' || panel == 'm')	// Tasks for member
		{
			Librarian L;
			L.LoadBooksData();
			L.LoadAllMembers();

			int option = 0;
			cout << "\n\nEnter 1 to Sign Up and 2 to Log In:\t";	// Member can either login or signup
			cin >> option;

			while (option != 1 && option != 2)	// Validating input
			{
				cout << "\nIncorrect option chosen. Please choose one of the following options. \nEnter 1 to Sign Up and 2 to Log In:\t";
				cin >> option;
			}

			if (option == 1)	// If member chooses to signup
			{
				system("cls");
				L.AddMember();
				cout << "\nSign Up Successful!\n";
			}
			else if (option == 2)	//if member chooses to login
			{
				system("cls");
				cout << "\nEnter Username and Password to login.\nUsername:\t";
				cin >> buffer1;
				username = Helper::GetStringFromBuffer(buffer1);
				cout << "Password:\t";
				cin >> buffer2;
				password = Helper::GetStringFromBuffer(buffer2);
				bool login = L.CompareCredentials(username, password);

				while (login == false)	// Verifies username and password using CompareCredentials() function
				{
					cout << "\nIncorrect credentials. Enter username and password again.\nUsername:\t";
					cin >> buffer1;
					username = Helper::GetStringFromBuffer(buffer1);
					cout << "Password:\t";
					cin >> buffer2;
					password = Helper::GetStringFromBuffer(buffer2);
					login = L.CompareCredentials(username, password);
				}
				cout << "\nLogin Successful!\n";
			}

			cout << "Welcome to the User panel " << L.GetCurrentUser()->GetUserName() << "!\n\n";
			int choice = 0;
			while (choice != 4)
			{
				//system("cls");
				cout << "Choose one of the following options:\n1. Issue a book.\n2. Return a book.\n3. Search for a book.\n4. Exit User panel.\t";
				cin >> choice;

				if (choice == 1)	// Book Issue
				{
					system("cls");
					cout << "Issuing a book for " << L.GetCurrentUser()->GetUserName() << endl << endl;
					L.IssueBook();
					cout << "\n\nPress any key to return to User Panel...\t";
					cin >> temp;
					system("cls");
				}
				else if (choice == 2)	// Book Return
				{
					system("cls");
					cout << "Returning a book for " << L.GetCurrentUser()->GetUserName() << endl << endl;
					L.ReturnBook();
					cout << "\n\nPress any key to return to User Panel...\t";
					cin >> temp;
					system("cls");
				}
				else if (choice == 3)	// Book Search
				{
					system("cls");
					cout << "Searching a book for " << L.GetCurrentUser()->GetUserName() << endl << endl;
					L.SearchBook();
					cout << "\n\nPress any key to return to User Panel...\t";
					cin >> temp;
					system("cls");
				}
				else if (choice == 4)	// Exit
				{
					cout << "\nExiting User panel...";
				}
				else if (choice <= 0 || choice > 4)	// Checking for invalid input
				{
					cout << "\nWrong input. Please choose one of the given options.\n";
				}
			}

			L.SaveData();	// Saves data into files after the member is done

			username = 0;
			password = 0;

			cout << "\n\nPress any key to return to Main Menu...\t";
			cin >> temp;
			system("cls");
		}

		else if (panel == 'L' || panel == 'l')	// Librarian panel
		{
			cout << "\n\nEnter Username and Password to login.\nUsername:\t";
			cin >> buffer1;
			username = Helper::GetStringFromBuffer(buffer1);
			cout << "Password:\t";
			cin >> buffer2;
			password = Helper::GetStringFromBuffer(buffer2);

			bool u = Helper::SearchString("Sohaib38", username);	// Verifies username and password with pre-set data
			bool p = Helper::SearchString("hustler12", password);

			while (u == false || p == false)	// Verifying username and password to enter
			{
				cout << "\nIncorrect credentials. Enter username and password again.\nUsername:\t";
				cin >> buffer1;
				username = 0;
				username = Helper::GetStringFromBuffer(buffer1);
				cout << "Password:\t";
				cin >> buffer2;
				password = 0;
				password = Helper::GetStringFromBuffer(buffer2);

				u = Helper::SearchString("Sohaib38", username);
				p = Helper::SearchString("hustler12", password);
			}
			cout << "\nLogin Successful!\n\n";

			Librarian L;
			L.LoadBooksData();
			L.LoadAllMembers();

			cout << "Welcome to the Librarian panel " << username << "!\n\n";
			int choice = 0;

			while (choice != 5)
			{
				cout << "Choose one of the following options:\n1. Add a book.\n2. Remove a book.\n3. Edit a book.\n4. View issued books.\n5. Exit Librarian panel.\t";
				cin >> choice;

				if (choice == 1)	// Adding a book
				{
					system("cls");
					cout << "Adding a book for " << username << endl << endl;
					L.AddBook();
					cout << "\nSuccessfully Added Book!\n\nPress any key to return to Librarian Panel...\t";
					cin >> temp;
					system("cls");

				}
				else if (choice == 2)	// Removing a book
				{
					system("cls");
					cout << "Removing a book for " << username << endl << endl;
					L.RemoveBook();
					cout << "\nSuccessfully Removed Book!\n\nPress any key to return to Librarian Panel...\t";
					cin >> temp;
					system("cls");
				}
				else if (choice == 3)	// Editing a book
				{
					system("cls");
					cout << "Editing a book for " << username << endl << endl;
					L.EditBook();
					cout << "\n\nPress any key to return to Librarian Panel...\t";
					cin >> temp;
					system("cls");
				}
				else if (choice == 4)	// Viewing list of issued books
				{
					system("cls");
					cout << "List of Issued Books\n\n";
					L.ViewIssuedBooks();
					cout << "\n\nPress any key to return to Librarian Panel...\t";
					cin >> temp;
					system("cls");
				}
				else if (choice == 5)	// Exit
				{
					cout << "\nExiting Librarian panel...";
				}
				else if (choice <= 0 || choice > 5)	// Invalid entry
				{
					cout << "\nWrong input. Please choose one of the given options.\n";
				}
			}

			L.SaveData();	// Saving data into file

			username = 0;
			password = 0;

			cout << "\n\nPress any key to return to Main Menu...\t";
			cin >> temp;
			system("cls");
		}

		else if (panel != 'M' && panel != 'm' && panel != 'L' && panel != 'l' && panel != 'Q' && panel != 'q')	// Validation
		{
			cout << "\nWrong input. Please choose one of the given options.";
		}
	}

	delete[] username;
	username = 0;
	delete[] password;
	password = 0;

	system("pause");
}