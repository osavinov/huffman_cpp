#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>


#define SIZE_TREE 516
#define BUFFER_SIZE 4096
#define SIZE_CODE 258


unsigned int setbit(unsigned int a, unsigned int pos)
{
	return (1 << pos)&a ? a : a^(1 << pos);
}

class ReadBuffer
{	
private:
	unsigned char buffer[BUFFER_SIZE];
	unsigned int pos;
	unsigned int size;
	FILE* file;
public:
	unsigned int FILE_SIZE;
	ReadBuffer(char* fileName)
	{
		file = fopen(fileName, "rb");
		if(!file)
		{
			std::cout << "Failed to open file \"" << fileName << "\"." << std::endl;
			exit(1);
		}
		pos = 0; size = 0;
		fseek(file, 0, SEEK_END);
		FILE_SIZE = ftell(file);
		fseek(file, 0, SEEK_SET);
	}
	void readFile()
	{
		size = fread(buffer, sizeof(char), BUFFER_SIZE, file);
		pos = 0;
	}
	bool readChar(unsigned char* c)
	{
		if(pos == size && size == BUFFER_SIZE)
			readFile();
		if(pos == size && size != BUFFER_SIZE)
			return 0;

		*c = buffer[pos];
		pos++;
		return 1;
	}
	void closeFile()
	{
		fclose(file);
	}
};

class WritePackBuffer
{
private: 
	unsigned char buffer[BUFFER_SIZE];
	int pos;
	int bitPos;
	FILE* file;
public:
	WritePackBuffer(char* fileName)
	{
		file = fopen(fileName, "wb");
		if(!file)
		{
			std::cout << "Failed to open file \"" << fileName << "\"." << std::endl;
			exit(1);
		}
		pos = 0; bitPos = 0;
		for(int i=0; i<BUFFER_SIZE; i++)
			buffer[i] = 0;
	}
	void writeFile()
	{
		fwrite(buffer, sizeof(char), bitPos ? pos+1 : pos, file);

		pos = 0; bitPos = 0;
		for(int i=0; i<BUFFER_SIZE; i++)
			buffer[i] = 0;
	}
	void write(bool* n, int blockSize)
	{
		for(int i=blockSize-1; i>=0; i--)
		{
			if(n[i])
				buffer[pos] = setbit(buffer[pos], sizeof(char)*8-bitPos-1);
			bitPos++;
			if(bitPos==8)
			{
				bitPos = 0;
				pos++;
				if(pos == BUFFER_SIZE)
					writeFile();
			}
		}
	}
	void writeChar(unsigned char c)
	{
		unsigned char mask = 1 << 7;
		for(int i=0; i<8; i++)
		{
			if(c&mask)
				buffer[pos] = setbit(buffer[pos], sizeof(char)*8-bitPos-1);
			mask = mask >> 1; 
			bitPos++;
			if(bitPos==8)
			{
				bitPos = 0;
				pos++;
				if(pos == BUFFER_SIZE)
					writeFile();
			}
		}
	}
	void closeFile()
	{
		fclose(file);
	}
};

class WriteBuffer
{
private:
	unsigned char buffer[BUFFER_SIZE];
	unsigned int pos;
	FILE* file;
public:
	WriteBuffer(char* fileName)
	{
		file = fopen(fileName, "wb");
		if(!file)
		{
			std::cout << "Failed to open file \"" << fileName << "\"." << std::endl;
			exit(1);
		}
		pos = 0;
	}
	void writeFile()
	{
		fwrite(buffer, sizeof(char), pos, file);
		pos = 0;
	}
	void writeChar(unsigned char c)
	{
		buffer[pos] = c;
		pos++;

		if(pos==BUFFER_SIZE)
			writeFile();
	}
	void closeFile()
	{
		fclose(file);
	}
};

class ReadPackBuffer
{
private:
	unsigned char buffer[BUFFER_SIZE];
	int pos;
	int bitPos;
	int size;
	FILE* file;
public:
	unsigned int FILE_SIZE;
	bool lastBuf;
	void readFile()
	{
		size = fread(buffer, sizeof(char), BUFFER_SIZE, file);
		pos = 0;
		if(size < BUFFER_SIZE)
			lastBuf = 1;
		else if(size == BUFFER_SIZE)
			lastBuf = 0;
	}
	ReadPackBuffer(char* fileName)
	{
		lastBuf = 0;
		file = fopen(fileName, "rb");
		if(!file)
		{
			std::cout << "Failed to open file \"" << fileName << "\"." << std::endl;
			exit(1);
		}

		pos = 0; bitPos = 0; size = 0;
		for(int i=0; i<BUFFER_SIZE; i++)
			buffer[i] = 0;
		fseek(file, 0, SEEK_END);
		FILE_SIZE = ftell(file);
		fseek(file, 0, SEEK_SET);
	}

	bool readBit(bool* bit)
	{
		*bit = 0;
		if( ((size - pos)*8 - bitPos) == 0 && lastBuf)
			return 0;
		unsigned char mask = 1 << (7 - bitPos);
		if(buffer[pos] & mask)
			*bit = 1;
		bitPos++;
		if(bitPos==8)
		{
			bitPos = 0;
			pos++;
			mask = 1 << 7;
			if(pos == size)
				readFile();
		}
		return 1;
	}
	bool read(unsigned char* n, int blockSize)
	{
		*n = 0;
		if( (((size - pos) * 8 - bitPos) < blockSize) && lastBuf)
			return 0;
		else
		{
			unsigned char mask = 1 << (7 - bitPos);
			for(int i=0; i<blockSize; i++)
			{
				if(buffer[pos]&mask)
					*n = setbit(*n, blockSize-i-1);
				
				mask = mask >> 1;
				bitPos++;
				if(bitPos==8)
				{
					bitPos = 0;
					pos++;
					mask = 1 << 7;
					if(pos == size)
						readFile();
				}
			}
			return 1;
		}
	}
	void closeFile()
	{
		fclose(file);
	}
};

struct Code
{
	bool code[SIZE_CODE];
	int lengthCode;
	Code()
	{
		for(int i=0; i<SIZE_CODE; i++)
			code[i]=0;
		lengthCode = 0;
	}
	void setCode(unsigned char c)
	{
		unsigned char mask = 1;
		for(int i=0; i<8; i++)
		{
			if(c&mask)
				code[i] = 1;
		}
	}
};

class Node
{
public:
	unsigned char sym;
	int weight;
	int leftSon;
	int rightSon;
	int parent;
	bool isLeaf;
	void setNode(unsigned char s, int w, int ls, int rs, int p, bool l)
	{
		sym = s;
		weight = w;
		leftSon = ls;
		rightSon = rs;
		parent = p;
		isLeaf = l;
	}
};

class HuffmanTree
{
private:
	int indexESC;
	int indexEOF;
	bool isLeaf;
	unsigned int sizeTree;
public:
	Node tree[SIZE_TREE];
	HuffmanTree()
	{
		tree[1].setNode('\0', 2, 2, 3, 0, 0);
		tree[2].setNode('\0', 1, 0, 0, 1, 1);
		tree[3].setNode('\0', 1, 0, 0, 1, 1);
		indexESC = 3; indexEOF = 2; sizeTree = 3;
	}
	void eraseTree()
	{
		for(int i=0; i<SIZE_TREE; i++)
			tree[i].setNode(0, 0, 0, 0, 0, 0);
		tree[1].setNode('\0', 2, 2, 3, 0, 0);
		tree[2].setNode('\0', 1, 0, 0, 1, 1);
		tree[3].setNode('\0', 1, 0, 0, 1, 1);
		indexESC = 3; indexEOF = 2; sizeTree = 3;
	}
	unsigned int getSizeTree()
	{
		return sizeTree;
	}
	unsigned int getEOFindex()
	{
		return indexEOF;
	}
	unsigned int getESCindex()
	{
		return indexESC;
	}
	int numOfNode(unsigned char c)
	{
		for(int i=1; i<SIZE_TREE; i++)
			if(tree[i].sym == c && tree[i].isLeaf && i!=indexEOF && i!=indexESC)
				return i;
		return 0;
	}

	Code getCode(int pos)
	{
		Code cd;
		int depth = 0;
		while(pos > 1)
		{
			if(pos == tree[tree[pos].parent].rightSon)
				cd.code[depth] = true;
			pos = tree[pos].parent;
			depth++;
		}
		cd.lengthCode = depth;
		return cd;
	}

	void addElem(unsigned char c)
	{
		tree[indexESC].setNode('\0', 1, indexESC+1, indexESC+2, tree[indexESC].parent, 0);
		tree[indexESC+1].setNode(c, 0, 0, 0, indexESC, 1);
		tree[indexESC+2].setNode('\0', 1, 0, 0, indexESC, 1);
		indexESC+=2; sizeTree+=2;
	}

	void swap(unsigned int n1, unsigned int n2)
	{
		if(n1 == indexESC) 
			indexESC = n2;
		else if(n2 == indexESC) 
			indexESC = n1;

		if(n1 == indexEOF) 
			indexEOF = n2;
		else if(n2 == indexEOF) 
			indexEOF = n1;

		if(tree[n1].leftSon)
		{
			tree[tree[n1].leftSon].parent = n2; 
			tree[tree[n1].rightSon].parent = n2;
		}
		if(tree[n2].leftSon)
		{
			tree[tree[n2].leftSon].parent = n1; 
			tree[tree[n2].rightSon].parent = n1;
		}

		Node temp = tree[n1];
		tree[n1] = tree[n2];
		tree[n2] = temp;

		int tempPar = tree[n1].parent;
		tree[n1].parent = tree[n2].parent;
		tree[n2].parent = tempPar;
	}

	void updateTree(unsigned char c)
	{
		int cur = numOfNode(c);
		while(cur>=1)
		{
			tree[cur].weight++;
			unsigned int pos = 0;
			for(int i = cur-1; i>1; i--)
			{
				if(tree[i].weight < tree[cur].weight)
					pos = i;
			}
			if(pos!=0)
			{
				swap(cur, pos);
				cur = pos;
			}
			cur = tree[cur].parent;
		}
	}
};

HuffmanTree h;
void compress(char* inputFile, char* outputFile)
{
	ReadBuffer inp(inputFile);
	inp.readFile();

	WritePackBuffer out(outputFile);
	if(!inp.FILE_SIZE)
		return;
	unsigned char k;
	int count = 1;
	while(inp.readChar(&k))
	{
		if(h.numOfNode(k)==0)
		{
			Code curESC = h.getCode(h.getESCindex());
			out.write(curESC.code, curESC.lengthCode);
			out.writeChar(k);
			h.addElem(k);
		}
		else
		{
			Code curCode = h.getCode(h.numOfNode(k));
			out.write(curCode.code, curCode.lengthCode);
		}
		h.updateTree(k);
		count++;
	}
	Code curEOF = h.getCode(h.getEOFindex());
	out.write(curEOF.code, curEOF.lengthCode);
	out.writeFile();
	inp.closeFile();
	out.closeFile();
	h.eraseTree();
}

void decompress(char* inputFile, char* outputFile)
{
	ReadPackBuffer inp(inputFile);
	WriteBuffer out(outputFile);
	inp.readFile();
	if(!inp.FILE_SIZE)
		return;
	bool cur;
	unsigned int pos = 1;
	unsigned char curChar;
	while(inp.readBit(&cur))
	{
		if(cur)
			pos = h.tree[pos].rightSon;
		else
			pos = h.tree[pos].leftSon;
		
		if(h.tree[pos].isLeaf)
		{
			if(pos==h.getESCindex())
			{
				inp.read(&curChar, 8);
				h.addElem(curChar);
				h.updateTree(curChar);
				pos = 1;
			}
			else if(pos==h.getEOFindex())
			{
				break;
			}
			else
			{
				curChar = h.tree[pos].sym;
				h.updateTree(curChar);
				pos = 1;
			}
			out.writeChar(curChar);
		}
	}

	out.writeFile();
	inp.closeFile();
	out.closeFile();
	h.eraseTree();
}

int main(int argc, char *argv[])
{
    // huffman -c -f test.txt -o test.huf
    int opt;
    char filename[256];
    char archFile[256];
    char mode;

    while((opt = getopt(argc, argv, ":f:o:cd")) != -1)
    {
        switch(opt)
        {
            case 'f':
                printf("filename: %s\n", optarg);
                strcpy(filename, optarg);
                break;
            case 'o':
                printf("output file: %s\n", optarg);
                strcpy(archFile, optarg);
                break;
            case 'c':
                mode = 'c';
                break;
            case 'd':
                mode = 'd';
                break;
        }
    }

    if (mode == 'c') {
        std::cout << "Compression in progress..." << std::endl;
        compress(filename, archFile);
        std::cout << "Compression is completed!" << std::endl;
    }
    else if (mode == 'd') {
        std::cout << "Decompression in progress..." << std::endl;
        decompress(archFile, filename);
        std::cout << "Decompression is completed!" << std::endl;
    }

	return 0;
}