#include<iostream>
#include<fstream>
#include<string>
using namespace std;
class table
{
public:
	char symbol;
	int freq;
	string code;
	table()
	{
		symbol = NULL;
		freq = 0;
	}
};
class CTNode
{
public:
	int freq;
	string code;
	char symbol;
	CTNode* pright;
	CTNode* pleft;
	CTNode()
	{
		freq = 0;
		pright = NULL;
		pleft = NULL;
		symbol = NULL;
	}
};
class CSNode
{
public:
	char symbol;
	int freq;
	CSNode* pnext;
	//CSNode* pright;
	//CSNode* pleft;
	CTNode* pdown;
	CSNode()
	{
		freq = 0;
		symbol = NULL;
		pnext = NULL;
		//pright = NULL;
		//pleft = NULL;
		pdown = NULL;
	}
};
class CSList
{
public:
	CSNode* phead;
	CSList()
	{
		phead = NULL;
	}
	~CSList()
	{
		CSNode* ptrav = phead;
		while (phead != NULL)
		{
			phead = phead->pnext;
			ptrav->pnext = NULL;
			delete ptrav;
			ptrav = phead;
		}
	}
	void insert(CSNode* pnn)
	{
		if (phead == NULL)
		{
			phead = pnn;
		}
		else
		{
			CSNode* pb = NULL;
			CSNode* ptrav = phead;
			while (ptrav != NULL && ptrav->freq < pnn->freq)
			{
				pb = ptrav;
				ptrav = ptrav->pnext;
			}
			if (pb != NULL)
			{
				pb->pnext = pnn;
				pnn->pnext = ptrav;
			}
			else
			{
				phead = pnn;
				pnn->pnext = ptrav;
			}
		}
	}

};
void incode(CTNode* ptrav, string code)
{
	if (ptrav == NULL)
		return;
	ptrav->code = code;
	incode(ptrav->pleft, code + "0");
	incode(ptrav->pright, code + "1");
}
void display(CTNode* ptrav)
{
	if (ptrav == NULL)
		return;
	if (ptrav->symbol != NULL)
	{
		cout << ptrav->symbol << "," << ptrav->freq << "," << ptrav->code << endl;
	}
	display(ptrav->pleft);
	display(ptrav->pright);
}
void cheatToList(CTNode* ptrav, table frq[256])
{
	if (ptrav == NULL)
		return;
	for (int i = 0; frq[i].symbol != NULL; i++)
	{
		if (ptrav->symbol == frq[i].symbol)
		{
			frq[i].code = ptrav->code;
		}
	}
	cheatToList(ptrav->pright, frq);
	cheatToList(ptrav->pleft, frq);
}
void main()
{
	ifstream file;
	string text;
	CSList frequency;
	CSNode* pnn;
	CSNode* pb;
	CTNode* parent;
	CTNode* temp;
	table frq[256];
	string code;
	string compresion;
	char biteControl = 0;
	char x = 1;
	file.open("test.txt");
	pb = NULL;
	int pos, i;
	pos = 0;
	while (getline(file, text))
	{
		for (i = 0; i < text.length(); i++)
		{
			for (int j = 0; frq[j].symbol != NULL && i > 0; j++)
			{
				if (text[i] == frq[j].symbol && i > 0)
				{
					pos = j;
					break;
				}
				else
				{
					pos = j + 1;
				}
			}
			frq[pos].symbol = text[i];
			frq[pos].freq++;
		}
	}
	//copying from the frequency list into nodes
	for (i = 0; frq[i].symbol != NULL; i++)
	{
		pnn = new CSNode;
		pnn->freq = frq[i].freq;
		pnn->symbol = frq[i].symbol;
		frequency.insert(pnn);
	}
	//Huffman tree
	CSNode* ptrav = frequency.phead;
	while (ptrav != NULL && ptrav->pnext != NULL)
	{
		parent = new CTNode;
		parent->freq = ptrav->freq + ptrav->pnext->freq;
		pnn = new CSNode;
		pnn->freq = parent->freq;
		pnn->pdown = parent;
		frequency.insert(pnn);
		if (ptrav->pdown == NULL)
		{
			temp = new CTNode;
			temp->symbol = ptrav->symbol;
			temp->freq = ptrav->freq;
			parent->pleft = temp;

		}
		else {
			parent->pleft = ptrav->pdown;
		}
		if (ptrav->pnext->pdown == NULL)
		{
			temp = new CTNode;
			temp->symbol = ptrav->pnext->symbol;
			temp->freq = ptrav->pnext->freq;
			parent->pright = temp;
		}
		else {
			parent->pright = ptrav->pnext->pdown;
		}
		frequency.phead = ptrav->pnext->pnext;
		ptrav->pnext->pnext = NULL;
		ptrav->pnext = NULL;
		ptrav->pdown = parent;
		ptrav = frequency.phead;
	}
	incode(frequency.phead->pdown, frequency.phead->pdown->code);
	cheatToList(frequency.phead->pdown, frq);
	//display(frequency.phead->pdown);
	// phase 2
	//compresion phase
	int bitShift = 7;
	int j;
	for (int i = 0; i < text.length(); i++)
	{
		for (j = 0; frq[j].symbol != NULL && frq[j].symbol != text[i]; j++)
		{

		}
		for (int k = 0; k < frq[j].code.length(); k++)
		{
			if (bitShift == -1)
			{
				bitShift = 7;
				compresion += biteControl;
				biteControl = 0;
			}
			if (frq[j].code[k] == '1')
			{
				biteControl |= x << bitShift;
			}
			bitShift--;
		}
	}
	cout << compresion;
	// decode phase
	bitShift = 7;
	biteControl = 0;
	char compare;
	char compare2;
	int flag = 0;
	for (int i = 0; i < text.length(); i++)
	{
		if (bitShift == -1)
		{
			bitShift = 7;
			biteControl = 0;
		}
		compare = biteControl;
		biteControl &= x << bitShift;
		if (biteControl == compare) // if true then the bite = 0
		{
			for (int j=0;j<frq[j].symbol!=NULL;j++)
			{
				if (frq[j].code[0] == '0')
				{
					for (int k = 1; k < frq[j].code.length(); k++)
					{
						// write a code to check rest of the bites
					}
				}
			}
		}
		else  // the bite = 1
		{
			for (int j = 0; j < frq[j].symbol != NULL; j++)
			{
				if (frq[j].code[0] == '1')
				{
					for (int k = 1; k < frq[j].code.length(); k++)
					{
						// write a code to check rest of the bites
					}
				}
			}
		}
	}
}
