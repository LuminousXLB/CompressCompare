#include "LZCoder.h"
#include "Tree.h"
#include <iostream>

#include <fstream>
#include <stack>
#include <vector>

#include <ctime>

using namespace std;

LZCoder::LZCoder(const char* infile, const char* outfile)
    : io(infile, outfile)
{
}

void LZCoder::encode()
{
    //cout << __FUNCSIG__ << endl;////

    bit buf;
    size_t next = 1;
    size_t len = 1, mcnt = 2;
    size_t prefix_state;

    Tree<Pair> tree(Pair{ 0, 0 });

    TreeNode<Pair>* current = NULL;

    while (io.getBit(buf)) {
        // Maintain the len
        if (next - 1 == mcnt) {
            mcnt *= 2;
            len++;
        }

        // Matching the prefix
        current = tree.getRoot();

        do {
            //cout << unsigned(buf); ////
            if (current->get(buf)) {
                current = current->get(buf);
            } else {
                break;
            }
        } while (io.getBit(buf));
        //cout << endl; ////

        //cout << "rdstate \t" << io.rdstate() << endl;////

        prefix_state = current->getValue().state;

        if (!io.eof()) {
            // Expand the tree with unreconized symbol
            current->addChild(buf, { prefix_state, next++ });
        }

        // Code it and write to outfile
        bit* buffer = new bit[len];

        for (size_t i = 0; i < len; i++) {
            buffer[len - i - 1] = prefix_state % 2;
            prefix_state >>= 1;
        }

        //cout << "\t";////
        for (size_t i = 0; i < len; i++) {
            io.putBit(buffer[i]);
            //cout << unsigned(buffer[i]); ////
        }

        if (!io.eof()) {
            io.putBit(buf);
            //cout << unsigned(buf); ////
        }

        //cout << endl;////

        delete[] buffer;
        buffer = NULL;
    }

    io.flush();

    //cout << io.ocount() << endl;////

    uint64_t count = io.ocount();

    io.getOs().write((char*)&count, sizeof(uint64_t));
    //cout << __FUNCSIG__ << endl;////
    //cout << (io.rdstate() & ios::goodbit) << endl;////
    //cout << (io.rdstate() & ios::eofbit) << endl;////
    //cout << (io.rdstate() & ios::failbit) << endl;////
    //cout << (io.rdstate() & ios::badbit) << endl;////
}

void LZCoder::decode()
{
    //cout << __FUNCSIG__ << endl;////

    clock_t Tdecode = 0, Twrite = 0; //, Tstack_push = 0, Tstack_pop = 0;////

    //unsigned cnt[65536] = { 0 };////
    //unsigned max_size = 0;

    uint64_t count;

    io.getIs().seekg(-9, io.getIs().end);
    io.getIs().read((char*)&count, sizeof(uint64_t));
    io.getIs().seekg(0, io.getIs().beg);

    bit buf;
    size_t len = 1, mcnt = 2;
    size_t prefix_state;

    vector<Code> table;
    table.push_back({ 0, 0 });

    stack<Code*> pool;

    while (io.getBit(buf) && io.icount() <= count) {
        clock_t clock1 = clock(); /////

        // Maintain the len
        if (table.size() - 1 == mcnt) {
            mcnt *= 2;
            len++;
        }

        // extract the prefix
        prefix_state = buf;
        for (size_t i = 1; i < len; i++) {
            io.getBit(buf);
            prefix_state = (prefix_state << 1) + buf;
        }

        io.getBit(buf);

        register Code code = { prefix_state, buf };
        register Code& prefix = table[prefix_state];
        /*

		if (prefix.data) {
			code.data_len = prefix.data_len + 1;
			code.data = new bit[code.data_len];
			memcpy(code.data, prefix.data, prefix.data_len);
			code.data[prefix.data_len] = buf;
		}
		else {
			code.data_len = 1;
			code.data = new bit[code.data_len];
			code.data[0] = buf;
		}
		*/

        table.push_back(code);

        //cout << "len " << len << "\t" << prefix_state << ',' << unsigned(buf) << endl;////

        clock_t clock2 = clock(); /////

        /*

		for (size_t i = 0; i < code.data_len; i++)
		{
			io.putBit(code.data[i]);
		}
		*/

        if (prefix.data) {
            //clock_t Tpush_1 = clock();////

            pool.push(&table[prefix_state]);
            while (pool.top()->prefix_state != 0) {
                pool.push(&table[pool.top()->prefix_state]);
            }

            //clock_t Tpop_1 = clock();////
            //cnt[pool.size()]++;////
            //if (pool.size() > max_size) max_size = pool.size();

            while (!pool.empty()) {
                Code*& top = pool.top();
                io.putBit(top->tail);
                //cout << unsigned(top.tail);////

                pool.pop();
            }

            //clock_t Tpop_2 = clock();////

            if (!io.eof() && io.icount() <= count) {
                io.putBit(buf);
                //cout << unsigned(buf);////
            }

            //Tstack_push += Tpop_1 - Tpush_1;
            //Tstack_pop += Tpop_2 - Tpop_1;
        } else {
            io.putBit(buf);
            //cout << unsigned(buf);////
        }
        //cout << endl;////

        clock_t clock3 = clock(); /////

        Tdecode += clock2 - clock1;
        Twrite += clock3 - clock2;
    }

    cout << "$decode " << Tdecode << "\t$write" << Twrite << "\t$io " << io.timer << endl;
    //cout << "$Tstack_push " << Tstack_push << " $Tstack_pop " << Tstack_pop << endl;

    /*for (int i = 0; i < 65536; i++) {
		if (cnt[i]) {
			cout << "$" << i << "\t: " << cnt[i] << endl;
		}
	}*/

    //cout << max_size << endl;

    //cout << __FUNCSIG__ << endl;////
    //cout << io.rdstate() << endl;////
}

void LZCoder::closeFiles()
{
    io.closeFiles();
}

LZCoder::~LZCoder()
{
}
