#include <iostream>
#include <fstream>
using namespace std;

class Facebook;
class Page;
class User;
class Object;
class Post;
class Memory;
class Date;
class Activity;
class Comment;

class Helper
{
public:
	static char* GetStringFromBuffer(const char*);
	static void StringCopy(char*&, const char*);
	static int StringLenght(const char* str);
	static void StrCat(char*&, const char*);
	static char* IntToString(int id)
	{
		char* ptr = new char[3];
		int temp = 0;
		temp = id / 10;
		ptr[0] = temp + '0';
		temp = id % 10;
		ptr[1] = temp + '0';
		ptr[2] = '\0';
		return ptr;
	}

};

int Helper::StringLenght(const char* str)
{
	int length = 0;
	for (int i = 0; str[i] != '\0'; i++)
		length++;
	return length;
}

void Helper::StringCopy(char*& dest, const char* src) {

	int length = StringLenght(src);

	for (int i = 0; i < length; i++)
		dest[i] = src[i];
	dest[length] = '\0';
}

char* Helper::GetStringFromBuffer(const char* str)
{
	int length = StringLenght(str);
	char* arr = new char[length + 1];
	StringCopy(arr, str);

	return arr;
}

void Helper::StrCat(char*& a, const char* b)
{
	int length = StringLenght(a);
	int i = length;
	for (int j = 0; b[i] != '\0'; i++, j++)
	{
		a[i] = b[j];
	}
	a[i] = '\0';
}

class Object
{
	char* id;
public:
	Object();
	~Object();
	char* getId();
	void setId(char*);
	virtual void PrintName() = 0;
};

Object::Object()
{
	id = 0;
}

Object::~Object()
{
	if (id)
	{
		delete[] id;
		id = 0;
	}
}

char* Object::getId()
{
	return id;
}

void Object::setId(char* temp)
{
	id = Helper::GetStringFromBuffer(temp);
}

class Date
{
	int date;
	int month;
	int year;
public:
	static Date* currentDate;
	static void SetCurrentDate(int d, int m, int y)
	{
		currentDate->date = d;
		currentDate->month = m;
		currentDate->year = y;
	}
	Date();
	void getDateFromFile(ifstream& file);
	void setDate(int, int, int);
	void viewDate();
	bool checkDate()
	{
		if (date == currentDate->date && month == currentDate->month && year == currentDate->year)
			return true;
		if ((date + 1) == currentDate->date && month == currentDate->month && year == currentDate->year)
			return true;
		return false;
	}
	bool checkMemory()
	{
		if (date == currentDate->date && month == currentDate->month && year != currentDate->year)
			return true;
		return false;
	}
	int yearDifference()
	{
		return currentDate->year - year;
	}
	Date& operator= (const Date& d)
	{
		date = d.date;
		month = d.month;
		year = d.year;
		return *this;
	}
	int getYear()
	{
		return year;
	}
};

Date* Date::currentDate = new Date;

Date::Date()
{
	date = 0;
	month = 0;
	year = 0;
}

void Date::getDateFromFile(ifstream & file)
{
	file >> date;
	file >> month;
	file >> year;
}

void Date::setDate(int d, int m, int y)
{
	date = d;
	month = m;
	year = y;
}

void Date::viewDate()
{
	cout << "(" << date << "/" << month << "/" << year << ")";
}

class Activity
{
	int type;
	char* value;
public:
	Activity();
	~Activity()
	{
		if (value)
		{
			delete[] value;
			value = 0;
		}
	}
	void ReadDataFromFile(ifstream&);
	void viewActivity();
};

Activity::Activity()
{
	type = 0;
	value = 0;
}

void Activity::ReadDataFromFile(ifstream & file)
{
	file >> type;
	char temp[80];
	file.getline(temp, 80);
	value = Helper::GetStringFromBuffer(temp);
}

void Activity::viewActivity()
{
	if (type == 1)
		cout << " is Feeling";
	else if (type == 2)
		cout << " is Thinking About";
	else if (type == 3)
		cout << " is Making";
	else if (type == 4)
		cout << " is Celebrating";

	cout << value;
}

class Comment
{
	char* commentId;
	Object* commentBy;
	char* text;
public:
	Comment();
	~Comment()
	{
		if (commentId)
		{
			delete[] commentId;
			commentId = 0;
		}
		if (text)
		{
			delete[] text;
			text = 0;
		}
	}
	void setValues(char*, Object*, char*);
	void viewComment();
};

Comment::Comment()
{
	commentId = 0;
	commentBy = 0;
	text = 0;
}

void Comment::setValues(char* id, Object * c, char* t)
{
	commentId = Helper::GetStringFromBuffer(id);
	commentBy = c;
	text = Helper::GetStringFromBuffer(t);
}

void Comment::viewComment()
{
	commentBy->PrintName();
	cout << " wrote: " << "\"" << text << "\"" << endl;
}


class Page : public Object
{
	char* title;
	Post** timeLine;
	int postNum;
public:
	Page();
	~Page();
	void ReadDataFromFile(ifstream&);
	void PrintPage();
	char* getId();
	void AddPostToTimeLine(Post*&);
	void PrintName()
	{
		cout << title;
	}
	void viewPage();
	void PageLatest();
};

Page::Page()
{
	title = 0;
	timeLine = 0;
	postNum = 0;
}

Page::~Page()
{
	if (title)
	{
		delete[] title;
		title = 0;
	}
	for (int i = 0; i < postNum; i++)
	{
		if (timeLine[i])
		{
			delete[] timeLine[i];
			timeLine[i] = 0;
		}
	}
	if (timeLine)
	{
		delete[] timeLine;
		timeLine = 0;
	}
}

void Page::ReadDataFromFile(ifstream & file)
{
	char temp[80];
	file >> temp;
	setId(temp);

	file.ignore();

	file.getline(temp, 80);
	title = Helper::GetStringFromBuffer(temp);
}

void Page::PrintPage()
{
	cout << getId() << "\t" << title << endl;
}

char* Page::getId()
{
	return Object::getId();
}

void Page::AddPostToTimeLine(Post * &P)
{
	if (postNum == 0)
	{
		timeLine = new Post * [10];
		timeLine[postNum] = P;
		postNum++;
	}
	else
	{
		if (postNum <= 10)
		{
			timeLine[postNum] = P;
			postNum++;
		}
	}
}

class User : public Object
{
	char* fName;
	char* lName;
	Page** likedPages;
	User** FriendList;
	int pageNum;
	int friendNum;
	Post** timeLine;
	int postNum;
public:
	User();
	~User();
	void ReadDataFromFile(ifstream&);
	void LikePage(Page*&);
	void AddFriend(User*&);
	char* getId();
	char* getFirstName();
	char* getLastName();
	void viewFriendList();
	void viewlikedPages();
	void AddPostToTimeLine(Post*&);
	void viewHomePage();
	void viewTimeLine();
	void PrintName();
	void UserLatest();
	void SeeMemories();
	void ShareMemory(Post*&, char*&, int);
	void PostComment(Post*&, const char*, int);
};

User::User()
{
	fName = 0;
	lName = 0;
	likedPages = 0;
	FriendList = 0;
	pageNum = 0;
	friendNum = 0;
	timeLine = 0;
	postNum = 0;
}

User::~User()
{
	if (fName)
	{
		delete[] fName;
		fName = 0;
	}
	if (lName)
	{
		delete[] lName;
		lName = 0;
	}
	if (likedPages)
	{
		delete[] likedPages;
		likedPages = 0;
	}
	if (FriendList)
	{
		delete[] FriendList;
		FriendList = 0;
	}
	for (int i = 0; i < postNum; i++)
		if (timeLine[i])
		{
			delete timeLine[i];
			timeLine[i] = 0;
		}
	if (timeLine)
	{
		delete[] timeLine;
		timeLine = 0;
	}
}

char* User::getId()
{
	return Object::getId();
}

char* User::getFirstName()
{
	return fName;
}

char* User::getLastName()
{
	return lName;
}

void User::ReadDataFromFile(ifstream & file)
{
	char temp[80];

	file >> temp;
	setId(temp);

	file.ignore();

	file >> temp;
	fName = Helper::GetStringFromBuffer(temp);

	file.ignore();

	file >> temp;
	lName = Helper::GetStringFromBuffer(temp);
}

void User::LikePage(Page * &p)
{
	if (likedPages == 0)
	{
		likedPages = new Page * [10];
		likedPages[0] = p;
		pageNum = 1;
	}
	else
	{
		if (pageNum <= 10)
		{
			likedPages[pageNum] = p;
			pageNum++;
		}
	}
}

void User::AddFriend(User * &u)
{
	if (friendNum == 0)
	{
		FriendList = new User * [10];
		FriendList[0] = u;
		friendNum = 1;
	}
	else
	{
		if (friendNum <= 10)
		{
			FriendList[friendNum] = u;
			friendNum++;
		}
	}
}

void User::viewFriendList()
{
	cout << fName << " " << lName << " - Friend List" << endl << endl;

	for (int i = 0; i < friendNum; i++)
	{
		cout << FriendList[i]->Object::getId() << "\t" << FriendList[i]->fName << "\t" << FriendList[i]->lName << endl;
	}
}

void User::viewlikedPages()
{
	cout << fName << " " << lName << " - Liked Pages" << endl << endl;
	for (int i = 0; i < pageNum; i++)
	{
		likedPages[i]->PrintPage();
	}
}

void User::AddPostToTimeLine(Post * &P)
{
	if (postNum == 0)
	{
		timeLine = new Post * [10];
		timeLine[postNum] = P;
		postNum++;
	}
	else
	{
		if (postNum <= 10)
		{
			timeLine[postNum] = P;
			postNum++;
		}
	}
}

void User::PrintName()
{
	cout << fName << " " << lName;
}

class Facebook
{
	Page** pages;
	User** users;
	Post** posts;
	User* currentUser;
	int totalPosts;
	int totalComments;
public:
	Facebook();
	~Facebook();
	void Load();
	void LoadPagesFromFile();
	void LoadUsersFromFile();
	void LoadPostsFromFile();
	void LoadCommentsFromFile();
	Page* SearchPageById(const char*);
	User* SearchUserById(const char*);
	Post* SearchPostById(const char*);
	void Run();
	void ShareMemory(Post*&, const char*);
};

class Post
{
protected:
	char* postId;
	Date startDate;
	char* text;
	Object* sharedBy;
	Activity* activity;
	Object** likedBy;
	Comment** comments;
	int likes;
	int commentNum;
public:
	Post();
	virtual ~Post()
	{
		if (postId)
		{
			delete[]postId;
			postId = 0;
		}
		if (text)
		{
			delete[]text;
			text = 0;
		}
		if (activity)
		{
			delete[] activity;
			activity = 0;
		}
		if (likedBy)
		{
			delete[] likedBy;
			likedBy = 0;
		}
		for (int i = 0; i < commentNum; i++)
		{
			if (comments[i])
			{
				delete[] comments[i];
				comments[i] = 0;
			}
		}
		if (comments)
		{
			delete[] comments;
			comments = 0;
		}
		if (sharedBy)
			sharedBy = 0;
	}
	Post(char*& c, Object*& o)
	{
		text = Helper::GetStringFromBuffer(c);
		sharedBy = o;
		commentNum = 0;
	}
	void ReadDataFromFile(ifstream&);
	void LikePost(User*& U);
	void LikePost(Page*& P);
	void CreatePost(User*& U);
	void CreatePost(Page*& P);
	char* getId();
	void AddComment(Comment* C);
	virtual void viewPost(bool c = true,  bool d = true);
	void viewLikedList();
	bool checkDate()
	{
		if (startDate.checkDate())
			return true;
	}
	bool isMemory()
	{
		if (startDate.checkMemory())
			return true;
	}
	int yearDifference()
	{
		return startDate.yearDifference();
	}
	void SetPostId(int total)
	{
		postId = Helper::GetStringFromBuffer("post");
		char* temp = new char[3];
		temp = Helper::IntToString(total + 1);
		Helper::StrCat(postId, temp);
	}
	int getYear()
	{
		return startDate.getYear();
	}

};

void User::viewHomePage()
{
	PrintName();
	cout << " - Home Page" << endl << endl;
	for (int i = 0; i < friendNum; i++)
	{
		FriendList[i]->UserLatest();
		cout << endl;
	}

	for (int i = 0; i < pageNum; i++)
	{
		likedPages[i]->PageLatest();
		cout << endl;
	}
}

void User::viewTimeLine()
{
	PrintName();
	cout << " - Time Line" << endl << endl;
	for (int i = 0; i < postNum; i++)
	{
		timeLine[i]->viewPost();
		cout << endl;
	}
}

void Page::viewPage()
{
	for (int i = 0; i < postNum; i++)
	{
		timeLine[i]->viewPost();
		cout << endl;
	}
}

char* Post::getId()
{
	return postId;
}

void Post::LikePost(User * &U)
{
	if (likes == 0)
	{
		likedBy = new Object * [10];
		likedBy[likes] = U;
		likes++;
	}
	else
	{
		if (likes <= 10)
		{
			likedBy[likes] = U;
			likes++;
		}
	}
}

void Post::LikePost(Page * &P)
{
	if (likes == 0)
	{
		likedBy = new Object * [10];
		likedBy[likes] = P;
		likes++;
	}
	else
	{
		if (likes <= 10)
		{
			likedBy[likes] = P;
			likes++;
		}
	}
}

void Post::CreatePost(Page * &P)
{
	sharedBy = P;
}

void Post::CreatePost(User * &U)
{
	sharedBy = U;
}

Post::Post()
{
	postId = 0;
	text = 0;
	sharedBy = 0;
	activity = 0;
	likedBy = 0;
	comments = 0;
	likes = 0;
	commentNum = 0;
}

void Post::ReadDataFromFile(ifstream & file)
{
	char temp[80];
	int t;

	file >> t;

	file >> temp;
	postId = Helper::GetStringFromBuffer(temp);

	startDate.getDateFromFile(file);
	file.ignore();

	file.getline(temp, 80);
	text = Helper::GetStringFromBuffer(temp);

	if (t == 2)
	{
		activity = new Activity;
		activity->ReadDataFromFile(file);
	}
}

void Post::AddComment(Comment * C)
{
	if (commentNum == 0)
	{
		comments = new Comment * [10];
		comments[0] = C;
		commentNum = 1;
	}
	else
	{
		if (commentNum <= 10)
		{
			comments[commentNum] = C;
			commentNum++;
		}
	}
}

void Post::viewPost(bool c, bool d)
{
	sharedBy->PrintName();
	if (activity)
	{
		activity->viewActivity();
		cout << endl;
	}
	else
		cout << " shared ";

	cout << "\"" << text << "\"" << "...";
	if (d)
	{
		startDate.viewDate();
		cout << endl;
	}

	if (c)
	for (int i = 0; i < commentNum; i++)
	{
		cout << "\t\t";
		comments[i]->viewComment();
	}
}

void Post::viewLikedList()
{
	cout << "Post Liked By:" << endl;
	for (int i = 0; i < likes; i++)
	{
		cout << likedBy[i]->getId();
		cout << " - ";
		likedBy[i]->PrintName();
		cout << endl;
	}
}

class Memory : public Post
{
	Post* originalPost;
public:
	Memory(Post*& original, char* text, Object* ptr, int total);
	~Memory()
	{
		originalPost = 0;
	}
	void viewPost(bool c = true, bool d = true);
};

Memory::Memory(Post * &o, char* text, Object * ptr, int total) : Post(text, ptr)
{
	originalPost = o;
	SetPostId(total);
	startDate = *Date::currentDate;
}

void Memory::viewPost(bool c, bool d)
{
	sharedBy->PrintName();

	cout << " shared a memory ";

	if (d)
	startDate.viewDate();

	cout << endl;

	cout << "\"" << text << "\"" << "...";

	cout << "(";

	originalPost->getYear();

	cout << startDate.getYear() - originalPost->getYear();

	cout << "  years ago)" << endl;

	originalPost->viewPost(false, d);

	if (c)
	for (int i = 0; i < commentNum; i++)
	{
		cout << "\t\t";
		comments[i]->viewComment();
	}

}

Facebook::Facebook()
{
	pages = 0;
	users = 0;
	posts = 0;
	totalPosts = 0;
	totalComments = 0;
	currentUser = 0;
}

Facebook::~Facebook()
{
	for (int i = 0; pages[i]; i++)
	{
		if (pages[i])
		{
			delete pages[i];
			pages[i] = 0;
		}
	}
	if (pages)
	{
		delete[] pages;
		pages = 0;
	}
	for (int i = 0; users[i]; i++)
	{
		if (users[i])
		{
			delete users[i];
			users[i] = 0;
		}
	}
	if (users)
	{
		delete[] users;
		users = 0;
	}
}

Page* Facebook::SearchPageById(const char* temp)
{
	int i = 0;
	while (pages[i])
	{
		bool flag = true;
		char* id = pages[i]->getId();
		for (int j = 0; temp[j] != '\0'; j++)
		{
			if (temp[j] != id[j])
				flag = false;
		}
		if (flag == true)
			return pages[i];
		i++;
	}
	return 0;
}

User* Facebook::SearchUserById(const char* temp) 
{
	int i = 0;
	while (users[i])
	{
		bool flag = true;
		char* id = users[i]->getId();
		for (int j = 0; temp[j] != '\0'; j++)
		{
			if (temp[j] != id[j])
				flag = false;
		}
		if (flag == true)
			return users[i];
		i++;
	}
	return 0;
}

Post* Facebook::SearchPostById(const char* temp)
{
	int i = 0;
	while (posts[i])
	{
		bool flag = true;
		char* id = posts[i]->getId();
		for (int j = 0; temp[j] != '\0'; j++)
		{
			if (temp[j] != id[j])
				flag = false;
		}
		if (flag == true)
			return posts[i];
		i++;
	}
	return 0;
}

void Facebook::LoadPagesFromFile()
{
	ifstream file;
	file.open("Pages.txt");

	int total;
	file >> total;

	pages = new Page * [total + 1];
	pages[total] = 0;
	for (int i = 0; i < total; i++)
	{
		pages[i] = new Page;
		pages[i]->ReadDataFromFile(file);
	}
	file.close();
}

void Facebook::LoadUsersFromFile()
{
	ifstream file;
	file.open("Users.txt");

	int total;
	file >> total;

	users = new User * [total + 1];
	users[total] = 0;
	char*** tempFriendIds = new char** [total];

	for (int i = 0; i < total; i++)
	{
		users[i] = new User;
		users[i]->ReadDataFromFile(file);

		tempFriendIds[i] = 0;

		char temp[5];
		file >> temp;

		if (temp[0] != '-')
		{
			tempFriendIds[i] = new char* [10];
			tempFriendIds[i][0] = Helper::GetStringFromBuffer(temp);

			file.ignore();
			\
				int j = 1;
			file >> temp;
			while (temp[0] != '-')
			{
				tempFriendIds[i][j] = Helper::GetStringFromBuffer(temp);
				j++;
				file.ignore();
				file >> temp;
			}
			tempFriendIds[i][j] = 0;
		}

		file >> temp;
		while (temp[0] != '-')
		{
			Page* P = SearchPageById(temp);
			users[i]->LikePage(P);
			file >> temp;
		}
		file.ignore(10, '\n');
	}
	file.close();

	for (int i = 0; i < total && tempFriendIds[i]; i++)
	{
		for (int j = 0; tempFriendIds[i][j]; j++)
		{
			User* U = SearchUserById(tempFriendIds[i][j]);
			users[i]->AddFriend(U);
		}
	}
}

void Facebook::LoadPostsFromFile()
{
	ifstream file;
	file.open("Posts.txt");

	int total;
	file >> total;

	totalPosts = total;

	posts = new Post * [total + 1];
	posts[total] = 0;

	for (int i = 0; i < total; i++)
	{
		posts[i] = new Post;
		posts[i]->ReadDataFromFile(file);

		char temp[80];
		file >> temp;
		if (temp[0] == 'u')
		{
			User* U = SearchUserById(temp);
			posts[i]->CreatePost(U);
			U->AddPostToTimeLine(posts[i]);

		}
		else if (temp[0] == 'p')
		{
			Page* P = SearchPageById(temp);
			posts[i]->CreatePost(P);
			P->AddPostToTimeLine(posts[i]);
		}

		int j = 0;
		while (temp[0] != '-' && j < 10)
		{
			file >> temp;
			if (temp[0] == '-')
				break;
			if (temp[0] == 'u')
			{
				User* U = SearchUserById(temp);
				posts[i]->LikePost(U);
			}
			else if (temp[0] == 'p')
			{
				Page* P = SearchPageById(temp);
				posts[i]->LikePost(P);
			}
			j++;
		}

	}
}

void Facebook::LoadCommentsFromFile()
{
	ifstream file;
	file.open("Comments.txt");

	int total;
	file >> total;
	totalComments = total;

	char temp[80], id[10], text[100];

	Comment** ptr = new Comment * [total];

	for (int i = 0; i < total; i++)
	{
		ptr[i] = new Comment;
		file >> id;

		file >> temp;
		Post* post = SearchPostById(temp);
		post->AddComment(ptr[i]);

		file >> temp;
		if (temp[0] == 'u')
		{
			User* user = SearchUserById(temp);
			file.ignore();
			file.getline(text, 100);

			ptr[i]->setValues(id, user, text);
		}
		else if (temp[0] == 'p')
		{
			Page* page = SearchPageById(temp);
			file.ignore();
			file.getline(text, 100);

			ptr[i]->setValues(id, page, text);
		}
	}
}

void Facebook::Load()
{
	this->LoadPagesFromFile();
	this->LoadUsersFromFile();
	this->LoadPostsFromFile();
	this->LoadCommentsFromFile();
}

void User::UserLatest()
{
	for (int i = 0; i < postNum; i++)
	{
		if (timeLine[i]->checkDate())
			timeLine[i]->viewPost(true, false);
	}
}

void Page::PageLatest()
{
	for (int i = 0; i < postNum; i++)
	{
		if (timeLine[i]->checkDate())
			timeLine[i]->viewPost(true, false);
	}
}

void User::SeeMemories()
{
	cout << "We hope you enjoy looking back and sharing your memories on Facebook, from the most recent to those long ago.";
	cout << endl << endl;
	for (int i = 0; i < postNum; i++)
	{
		if (timeLine[i]->isMemory())
		{
			int difference = timeLine[i]->yearDifference();
			cout << "On this day " << difference << " years ago" << endl;
			timeLine[i]->viewPost();
			cout << endl;
		}
	}
}

void Facebook::ShareMemory(Post * &post, const char* text)
{
	char* caption = Helper::GetStringFromBuffer(text);
	currentUser->ShareMemory(post, caption, totalPosts);
	totalPosts++;
}

void User::ShareMemory(Post * &p, char*& text, int total)
{
	if (postNum == 0)
	{
		timeLine = new Post * [10];
		timeLine[postNum] = new Memory(p, text, this, total);
		postNum++;
	}
	else if (postNum < 10)
	{
		timeLine[postNum] = new Memory(p, text, this, total);
		postNum++;
	}
}

void User::PostComment(Post * &p, const char* text, int total)
{
	char* t = Helper::GetStringFromBuffer(text);
	Comment* C = new Comment;

	char* id = new char[4];
	id = Helper::GetStringFromBuffer("c");

	char* temp = Helper::IntToString(total + 1);
	Helper::StrCat(id, temp);

	Object* o = this;

	C->setValues(id, o, t);

	p->AddComment(C);
}

void Facebook::Run()
{
	Date::SetCurrentDate(15, 11, 2017);
	cout << "System Date:\t";
	Date::currentDate->viewDate();
	cout << endl << endl;

	//load data
	Load();

	//set current user
	currentUser = SearchUserById("u4");
	if (currentUser)
	{
		cout << currentUser->getFirstName() << " " << currentUser->getLastName() << " successfully set as current user" << endl << endl;
		cout << "------------------------------------------------------" << endl << endl;

		//view friend list
		currentUser->viewFriendList();
		cout << endl;
		cout << "------------------------------------------------------" << endl << endl;

		//view liked pages
		currentUser->viewlikedPages();
		cout << endl;
		cout << "------------------------------------------------------" << endl << endl;

		//view home page
		currentUser->viewHomePage();
		cout << endl;
		cout << "------------------------------------------------------" << endl << endl;

		//view timeline
		currentUser->viewTimeLine();
		cout << endl;
		cout << "------------------------------------------------------" << endl << endl;
	}
	else
		cout << "USER NOT FOUND." << endl << endl;

	//view post likes
	Post* post = SearchPostById("post5");
	if (post)
	{
		post->viewLikedList();
		cout << endl;
		cout << "------------------------------------------------------" << endl << endl;

		//like a post and view post likes again
		if (currentUser)
		{
			post->LikePost(currentUser);
		}
		post->viewLikedList();
		cout << endl;
		cout << "------------------------------------------------------" << endl << endl;
	}
	else
		cout << "POST NOT FOUND." << endl << endl;

	//post comment
	Post* post3 = SearchPostById("post4");
	if (post3)
	{
		if (currentUser)
		{
			currentUser->PostComment(post3, "Good Luck for your Result", totalComments);
		}

		////view post after posting comment
		post3->viewPost();
		cout << "------------------------------------------------------" << endl << endl;
	}
	else
		cout << "POST NOT FOUND." << endl << endl;

	//post comment
	Post* post4 = SearchPostById("post8");
	if (post4)
	{
		if (currentUser)
		{
			currentUser->PostComment(post4, "Thanks for the wishes", totalComments);
		}

		//view post after posting comment
		post4->viewPost();
		cout << "------------------------------------------------------" << endl << endl;
	}
	else
		cout << "POST NOT FOUND." << endl << endl;

	//see memories
	if (currentUser)
	{
		currentUser->SeeMemories();
	}

	//share memory
	Post* post2 = SearchPostById("post10");
	if (post2)
	{
		if (currentUser)
		{
			ShareMemory(post2, "Never thouht i will be specialist in this field...");
		}
		cout << "------------------------------------------------------" << endl << endl;
	}
	else
		cout << "POST NOT FOUND." << endl << endl;

	//share memory and view timeline again
	if (currentUser)
	{
		currentUser->viewTimeLine();
		cout << endl;
		cout << "------------------------------------------------------" << endl << endl;
	}
	else cout << "USER NOR FOUND." << endl << endl;

	//view page
	Page* page = SearchPageById("p1");
	if (page)
	{
		page->viewPage();
		cout << "------------------------------------------------------" << endl << endl;
	}
	else
		cout << "PAGE NOT FOUND" << endl << endl;

	currentUser = SearchUserById("u11");
	if (currentUser)
	{
		currentUser->viewHomePage();
		cout << endl;
		cout << "------------------------------------------------------" << endl << endl;

		currentUser->viewTimeLine();
		cout << endl;
	}

	//deallocate only static data
	delete[] Date::currentDate;
	Date::currentDate = 0;
}

int main()
{
	Facebook fb;
	fb.Run();
}