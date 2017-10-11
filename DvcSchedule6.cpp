
// Programmer: Max Goshay
// Programmer's ID: 252053
#include "DynamicArray.h"
#include <iostream>
using namespace std;
	
struct voo{
	string sterm = "";
	DynamicArray <string> ssec;
	int scount = 0;
};

struct subj {
	string ssubject;
	int subcount = 0;
};

void sort(DynamicArray <subj> &ssu, int &subjCount) {
	for (int i = 0; i < subjCount; i++)
	{
		if (ssu[i].ssubject == "")break;
		for (int j = i + 1; j < subjCount; j++) 
		{
			if (ssu[j].ssubject == "")break;
			if (ssu[j].ssubject < ssu[i].ssubject) 
				swap(ssu[i], ssu[j]);	
		}
	}
	cout << endl;
}

void outputSubjectsAndTermCounts(int &subjCount, int &dupeCount, int &stermCount, int &count, DynamicArray <subj> &ssu) {
	for (int i = 0; i < subjCount; i++)
		cout << ssu[i].ssubject << "\t" << ssu[i].subcount << endl;
	cout << endl;
	cout << dupeCount << "\tDuplicates" << endl;
	cout << stermCount << "\tTerms" << endl;
	cout << subjCount << "\tSubjects" << endl;
	cout << count << "\tFull Count" << endl;
}

void pushSubject(int& subjCount, string subjectCode, DynamicArray <subj> &ssu)
{
	for (int a = 0; a < subjCount; a++)
	{
		if (subjectCode == ssu[a].ssubject)
		{
			ssu[a].subcount++;
			return;
		}
	}
	ssu[subjCount].ssubject = subjectCode;
	ssu[subjCount].subcount++;
	subjCount++;
}

void checkForDuplicateSection(DynamicArray <voo> &duper, DynamicArray <subj> &ssu, int &i, string const &section, int& subjCount, string const & subjectCode, int & dupeCount) {
	for (int j = 0; j <= duper[i].scount; j++)
	{
		if (section == duper[i].ssec[j])
		{
			dupeCount++;
			return;
		}
	}
	//no dupe add to array
	duper[i].ssec[duper[i].scount] = section;
	duper[i].scount++;
	pushSubject(subjCount, subjectCode, ssu);
}

void checkForDuplicates(DynamicArray <voo> &duper, DynamicArray <subj> &ssu, string const &section, int& subjCount, string const & subjectCode, int & dupeCount, int &stermCount, string const & term) {
	for (int i = 0; i <= stermCount; i++)
	{
		if (term == duper[i].sterm)
		{
			checkForDuplicateSection(duper, ssu, i, section, subjCount, subjectCode, dupeCount);
			return; 
		}
	}
	duper[stermCount].sterm = term;
	duper[stermCount].ssec[0] = section;
	duper[stermCount].scount++;
	stermCount++;
	pushSubject(subjCount, subjectCode, ssu);
}


int main()
{
	DynamicArray <voo> duper(100);
	int stermCount = 0;

	DynamicArray <subj> ssu(115);
	int subjCount = 0;

	int dupeCount = 0;
	int count = 0;

	// for parsing the inputfile
	char* token;
	char buf[1000];
	const char* const tab = "\t";

	// open the input file
	ifstream fin;
	fin.open("dvc-schedule.txt");
	if (!fin.good()) throw "I/O error";

	// read the input file
	while (fin.good())
	{
		// read the line
		string line;
		getline(fin, line);
		strcpy(buf, line.c_str());

		if (buf[0] == 0) continue; // skip blank lines
								   //parse the line
		const string term(token = strtok(buf, tab));
		const string section((token = strtok(0, tab)) ? token : "");
		const string course((token = strtok(0, tab)) ? token : "");
		const string instructor((token = strtok(0, tab)) ? token : "");
		const string whenWhere((token = strtok(0, tab)) ? token : "");
		if (course.find('-') == string::npos) continue; // invalid line: no dash in course name
		const string subjectCode(course.begin(), course.begin() + course.find('-'));

		// if I get this far, then it's a valid record
		//cout << term << '|' << section << '|'
		//	<< course << '|' << instructor << '|'
		//	<< whenWhere << '|' << subjectCode << endl;

		checkForDuplicates(duper, ssu, section, subjCount, subjectCode, dupeCount, stermCount, term);
		count++;

	}
	fin.close();

	sort(ssu, subjCount);
	outputSubjectsAndTermCounts(subjCount, dupeCount, stermCount, count, ssu);

	return 0;
}
