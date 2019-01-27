#include <iostream>
#include <windows.h>
#include <string>
#include <conio.h>
#include <vector>
#include <fstream>
#include <iomanip>
using namespace std;

void gotoxy(int x, int y)//given, move cursor to a location 
{
	COORD pos = { x, y };
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(output, pos);
}
class node //the start of all linked list 
{
public:
	char c;
	node *next;
	node *prev;
};
class editor
{
public:
	int x, y, p;//x and y are cursor position and vector index, p is menu index
	string meun[4];//string array that represent menu 
	string copy, message;//copy stores the string copied, message display a message by the code
	vector<node*>line;//vector to keep track of line
	node* s, *f, *t;// s is cursor location, t is temporary node and f is the first node of linked list 
	node* f31, *f32; // f31 is location 1 for copy f32 is location 2 
	char input;//user input 
	void run();//runs editor 
	void clear();//delete current text and start from beginning
	void display();//display text 
	void backspace();//function for backspace key 
	void arrowKey();//function for arrow key 
	void enter();//starts a new line 
	void fKey();//special key that does different function
	void text();//normal key 
	void showMenu();//go to menu board 
	void format();//delete lines thats empty
	void deleteNode();//delete current text
	void displayMenu();//display menu and Instructions
	editor()//initialize variable, make three node f->s->t and one element in vector 
	{
		x = 0, y = 0;
		s = new node, f = new node, t = new node;//initialize first line
		meun[0] = meun[1] = meun[2] = "OFF";//initialize menu
		meun[3] = "OK";//initialize variable
		f->prev = t->next = f31 = f32 = nullptr;//initialize first line
		f->next = t->prev = s;
		s->prev = f;
		s->next = t;
		s->c = ' ';
		copy = message = "";
		line.push_back(f);//initialize first element in vector
		display();
	}
	~editor(){ deleteNode(); } // destructor
};
void editor::deleteNode()
{
	for (node *ptr : line)//delete current text 
	{
		while (ptr->next != nullptr)//delete each node usign a for loop
		{
			t = ptr;
			while (t->next != nullptr)
				t = t->next;
			t = t->prev;
			delete t->next;
			t->next = nullptr;
		}
		delete ptr;
	}
	line.clear();//clear vector 
}
void editor::arrowKey()
{
	switch (_getch())
	{
	case 75://left arrow key 
	{
		if (s->prev == f)//if it's out of bound 
		{
			message = "Left arrow key function unavailable: out of bound.";
			break;
		}
		else//insert curser node one node forward 
		{
			s->next->prev = s->prev;
			s->next->prev->next = s->next;
			s->prev = s->prev->prev;
			s->prev->next = s;
			s->next = s->next->prev;
			s->next->prev = s;
			x--;
		}
	}
		break;
	case 77://right arrow key 
	{
		if (s->next->next == nullptr)//if it's out of bound 
		{
			message = "Right arrow key function unavailable: out of bound.";
			break;
		}
		else//insert curser node one node backward 
		{
			s->prev->next = s->next;
			s->prev->next->prev = s->prev;
			s->next = s->next->next;
			s->next->prev = s;
			s->prev = s->prev->next;
			s->prev->next = s;
			x++;
		}
	}
		break;
	case 72://up arrow key 
	{
		if (y == 0)//if it's out of bound 
		{
			message = "Up arrow key function unavailable: out of bound.";
			break;
		}
		else//insert curser node one node up, move to closeest node if couldn't move straight
		{
			int newX = 0;
			f = line[--y];
			t = f;
			while (x-- != 0 && t->next->next != nullptr)
			{
				newX++;
				t = t->next;
			}
			s->prev->next = s->next;
			s->next->prev = s->prev;
			s->next = t->next;
			s->next->prev = s;
			t->next = s;
			s->prev = t;
			x = newX;
		}
	}
		break;
	case 80://down arrow key 
	{
		if (y + 1 == line.size())//if it's out of bound 
		{
			message = "Down arrow key function unavailable: out of bound.";
			break;
		}
		else//insert curser node one node down, move to closeest node if couldn't move straight
		{
			int newX = 0;
			f = line[++y];
			t = f;
			while (x-- != 0 && t->next->next != nullptr)
			{
				newX++;
				t = t->next;
			}
			s->prev->next = s->next;
			s->next->prev = s->prev;
			s->next = t->next;
			s->next->prev = s;
			t->next = s;
			s->prev = t;
			x = newX;
		}
	}
		break;
	default:
		message = "Arrow key function unavailable: unknown key.";//shouldn't be displayed 
		break;
	}
}
void editor::backspace()
{
	if (s->prev != f)//for normal deletion 
	{
		s->prev = s->prev->prev;
		delete s->prev->next;
		s->prev->next = s;
		x--;
	}
	else if (s->prev != line[0])// if is first node in line 
	{
		x = 0;
		f = line[y - 1];
		t = f;
		while (t->next->next != nullptr)
		{
			t = t->next;
			x++;
		}
		line.erase(line.begin() + y);//delete line from vector
		delete t->next;
		delete s->prev;
		s->prev = t;
		t->next = s;
		y--;
	}
	else //out of bound
		message = "Backspace key function unavailable: out of bound.";
}
void editor::enter()//move to next line, make new variables 
{
	t = new node;//new node for the end of old line
	s->prev->next = t;
	t->prev = s->prev;
	t->next = nullptr;
	f = new node;//new node for the start of new line
	f->prev = nullptr;
	f->next = s;
	s->prev = f;
	line.insert(line.begin() + y + 1, f);//insert new element 
	x = 0;
	y++;
}
void editor::fKey()
{
	switch (_getch())
	{
	case (59) ://f1 save text into file 
	{
		system("CLS");
		cout << "enter file name \n(no need to include .txt, we will add it for you)\n: ";
		string fileName = "";
		cin >> fileName;
		fileName += ".txt";
		ofstream save(fileName);//make new file
		if (save.is_open() == true)
		{
			for (node *ptr : line)
			{
				while (ptr->next->next != nullptr)
				{
					if (ptr->next != s)
						save << ptr->next->c;
					ptr = ptr->next;
				}
				save << endl;
			}
			save.close();
			message = "file " + fileName + " saved successfully";
		}
		else
			message = "fail to save file " + fileName;
	}
			   break;
	case (60) ://f2 load file into program
	{
		system("CLS");
		cout << "enter file name you want to load \n(no need to include .txt, we will add it for you)\n: ";
		string fileName;
		cin >> fileName;
		fileName += ".txt";
		ifstream load(fileName);
		if (load.is_open())
		{
			deleteNode();
			string str;
			x = 0, y = 0;
			s = new node;
			s->prev = nullptr;
			s->c = ' ';
			while (getline(load, str))//load text into program 
			{
				t = new node;
				f = new node;
				if (s->prev == nullptr)//for first line
				{
					f->next = s;
					f->prev = nullptr;
					s->prev = f;
					s->next = t;
					t->prev = s;
					t->next = nullptr;
					line.push_back(f);
				}
				else//for other line 
				{
					s->prev->next = t;
					t->prev = s->prev;
					t->next = nullptr;
					f->prev = nullptr;
					f->next = s;
					s->prev = f;
					line.push_back(f);
					y++;
					x = 0;
				}
				for (char c : str)//put charater into each line 
				{
					t = new node;
					t->c = c;
					s->prev->next = t;
					t->prev = s->prev;
					s->prev = t;
					t->next = s;
					x++;
				}
			}
			load.close();
			message = "file " + fileName + " loaded successfully";
		}
		else
			message = "file not found";

	}
			   break;
	case (61) : //f3 save 
	{
		int pos = 0;
		if (f31 == nullptr && f32 == nullptr)//when user press f3 the first time, it sets the first location 
		{
			f31 = s->next;
			pos = y;
			message = "first location acquired, now press f3 again at a later location to record a string";
		}
		else if (f31 != nullptr && f32 == nullptr)// for when user press f3 the second time, it saves the string between the two location as copy
		{
			f32 = s->prev;
			if (f32->prev == nullptr)
				f32 = f32->next;
			while (true)
			{
				if (f31->next == nullptr && pos + 1 == line.size())//reset location if the first location is after second location 
				{
					f31 = nullptr;
					f32 = nullptr;
					message = "Incorrect input: second location must be after first location";
					break;
				}
				else if (f31->next == nullptr)
				{
					copy += "\n";
					f31 = line[++pos]->next;
					if (f31 == f32)
						break;
				}
				else if (f31 == f32)
				{
					copy += f31->c;
					break;
				}
				else
				{
					copy += f31->c;
					f31 = f31->next;
				}
			}
			message = "string recorded, your string is " + copy + " \npress f4 to paste the string copied\nthis string will stay in memory until you press f3 again to reset the string";
		}
		else if (f31 != nullptr && f32 != nullptr)//reset location when user press f3 the third time
		{
			f31 = nullptr;
			f32 = nullptr;
			copy = "";
			message = "string has being reset. now press f3 to set first copy location";
		}
	}
				break;
	case (62) ://f4 paste
	{
		for (char c : copy)//loop through each character in copy string, put each char as node and insert into text
		{
			if (c == '\n')//start new line if character is \n
			{
				f = new node;
				t = new node;
				s->prev->next = t;
				t->prev = s->prev;
				t->next = nullptr;
				f->prev = nullptr;
				f->next = s;
				s->prev = f;
				line.insert(line.begin() + y + 1, f);
				y++;
				x = 0;
			}
			else//for normal character
			{
				t = new node;
				t->c = c;
				s->prev->next = t;
				t->prev = s->prev;
				s->prev = t;
				t->next = s;
				x++;
			}

		}
	}
			   break;
	case (64) :
		showMenu();//go to menu page for f6
	}
}
void editor::displayMenu()//display menu and instruction
{
	system("CLS");
	cout << "\n Meun : use up/down arrow and Enter to navigate the page(press Esc to exit meun)";
	cout << "\n\n     Show message       " << meun[0] << "\n     Show word count    " << meun[1];
	cout << "\n     Clear              " << meun[2] << "\n     Format             " << meun[3];
	cout << "\n\n     Instructions:" << endl;
	cout << "     The arrow keys moves the cursor up,down, right and left." << endl;
	cout << "     Enter starts a new line and backspace deletes a character." << endl;
	cout << "     To copy, select a location where you want to begin, " << endl;
	cout << "     then press F3 and move to another location where you " << endl;
	cout << "     would like to end the copying and press F3 again." << endl;
	cout << "     To paste simply press F4 to paste at any location you want." << endl;
	cout << "     F1 is to save text as txt file and f2 to load text from txt file" << endl;
	cout << "     you can change setting at menu page and do specal function\n";
	cout << "     clear function start a new page and format delete empty line from text";
	gotoxy(23, p + 4);
}
void editor::showMenu()
{
	char inp = ' ';
	p = 0;//cursor index for user
	meun[3] = meun[2] = "OK";
	displayMenu();
	while ((inp = _getch()) != 27)//only allow user to input up, down, enter, and esc
	{
		switch (inp)
		{
		case -32: {
			switch (_getch())
			{
			case 72://when user input up
			{
				if (p == 0)
					break;
				else
					p--;
			}
				break;
			case 80://when user press down
			{
				if (p == 3)
					break;
				else
					p++;
			}
				break;
			}
		}
			break;
		case 13: {//change setting depending on what the user input
			if (meun[p] == "OFF")
				meun[p] = "ON";
			else if (meun[p] == "ON")
				meun[p] = "OFF";
			else if (meun[p] == "OK")
				meun[p] = "DONE";

			if (meun[3] == "DONE")
				format();
			if (meun[2] == "DONE")
				clear();
		}
			break;
		}
		displayMenu();
	}
}
void editor::format()//delete lines that deos not have charater 
{
	for (int i = 0, j = line.size() - 1; i < j; i++)
	{
		if (line[i]->next->next == nullptr)
		{
			line.erase(line.begin() + i--);
			y--;
			j--;
		}
	}
}
void editor::text()//load normal character 
{
	t = new node;
	t->c = input;
	s->prev->next = t;
	t->prev = s->prev;
	s->prev = t;
	t->next = s;
	x++;
}
void editor::run()
{
	while ((input = _getch()) != 27)//take user input and exit out if esc is inputed
	{
		switch (input)
		{
		case -32: arrowKey();//function for arrow key
			break;
		case 8: backspace();//function for backspace
			break;
		case 13: enter();//function ener
			break;
		case 0: fKey();//function for special key
			break;
		default: text();//function for normal input
		}
		display();
	}
}
void editor::clear()
{
	deleteNode();//delete current text
	x = 0, y = 0;//initialize variable for first line
	s = new node, f = new node, t = new node;
	f->prev = t->next = f31 = f32 = nullptr;
	f->next = t->prev = s;//initialize variable for first line
	s->prev = f;
	s->next = t;
	s->c = ' ';//initialize variable for first line
	line.push_back(f);//initialize first element
}
void editor::display()//display board 
{
	string temp = "";//temp string that stores the text for each line
	system("CLS");
	int c = 0;//word count 
	cout << "     F6 to go to menu and instructions, Esc to exit\n\n     ______________________________________________________________________ \n";
	for (node *ptr : line)//goes through each line 
	{
		while (ptr->next->next != nullptr)//goes through each charater 
		{
			temp += ptr->next->c;
			ptr = ptr->next;
			c++;
		}
		cout << "     |" << left << setw(68) << temp << "|\n";//display the line 
		temp = "";
	}
	cout << "     |                                                                    |"
		<< "\n     |                                                                    |"
		<< "\n     ----------------------------------------------------------------------\n";//display the rest of the box
	if (meun[1] == "ON")
		cout << "     Word Count: " << c << " letters " << endl;//display word count if user selete that option 
	if (meun[0] == "ON")
		cout << "\n     message:\n     " << message << endl << endl;//display message count if user selete that option 
	message = "no massage";
	gotoxy(x + 6, y + 3);

}
int main()
{
	editor d;
	d.run();//run editor
	return 0;
}
