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
	CTNode* pdown;
	CSNode()
	{
		freq = 0;
		symbol = NULL;
		pnext = NULL;
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
void main()
{
	ofstream compress("x64\\Debug\\compressedVersion.bmp", ofstream::binary);
	ifstream original("x64\\Debug\\original.bmp", ofstream::binary);
	ofstream code;
	table frq[256];
	int pos=0;
	char ch;
	CSNode* pnn;
	CSList frequency;
	CTNode* parent;
	CTNode* temp;
	char biteControl = 0;
	string compresion;
	char x = 1;
	int nSymbols;
	code.open("code.txt");
	//start of the logic

	//tazbeet el7agat

	original.seekg(0, original.end);
	int h = original.tellg();
	original.seekg(0, original.beg);
	for (int i = 0; i < h; i++)
	{
		original.read(&ch, 1);
		for (int j = 0; frq[j].symbol != NULL ; j++)
		{
			if (ch == frq[j].symbol)
			{
				pos = j;
				break;
			}
			else
			{
				pos = j+1;
			}
		}
		frq[pos].symbol = ch;
		frq[pos].freq++;
	}
	for (nSymbols = 0; frq[nSymbols].symbol != NULL; nSymbols++)
	{
		pnn = new CSNode;
		pnn->freq = frq[nSymbols].freq;
		pnn->symbol = frq[nSymbols].symbol;
		frequency.insert(pnn);
	}
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
	cheatToList(frequency.phead->pdown,frq);
	for (int i = 0; i < nSymbols; i++)
	{
		code << frq[i].symbol << " , " << frq[i].code << endl;
	}
	display(frequency.phead->pdown);

	//compressing

	int bitShift = 7;
	int j;
	original.seekg(0, original.beg);
	for (int i = 0; i < h; i++)
	{
		original.read(&ch, 1);
		for (j = 0; frq[j].symbol != NULL && frq[j].symbol !=ch ; j++)
		{

		}
		for (int k = 0; k < frq[j].code.length(); k++)
		{
			if (bitShift == -1)
			{
				bitShift = 7;
				compress.write(&biteControl, 1);
				//compresion += biteControl;
				biteControl = 0;
			}
			if (frq[j].code[k] == '1')
			{
				biteControl |= x << bitShift;
			}
			bitShift--;
		}
		
	}
	int lastbite = 7 - bitShift;
	if (bitShift > -1)
	{
		compress.write(&biteControl, 1);
		//compresion += biteControl;
	}

}