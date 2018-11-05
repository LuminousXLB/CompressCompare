#pragma once

#include "common.h"
#include "ioUtilLZ.h"

using namespace std;

template <class Unit, unsigned USize>
class LZEncoderBase {
protected:
    ioLZEncoder<Unit, USize> io;

    struct Pair {
        size_t prefix_state, state;
    };

public:
    void encode()
    {
#ifdef LZCoderDBG
        __FUNCTION_ENTRANCE__;
#endif // LZCoderDBG

        Unit buf;
        size_t next = 1, mcnt = 2;
        uint8_t len = 1;
        size_t prefix_state;

        Tree<Pair, USize> tree(Pair { 0, 0 });

        TreeNode<Pair, USize>* current = NULL;

        while (io.getUnit(buf)) {

#ifdef LZEncoderInputDBG
            __DBGHeader__;
#endif // LZEncoderInputDBG

            // Maintain the len
            if (next - 1 == mcnt) {
                mcnt *= 2;
                len++;
            }

            // Matching the prefix
            current = tree.getRoot();

            do {
#ifdef LZEncoderInputDBG
                printf("%d ", buf);
#endif // LZEncoderInputDBG

                if (current->get(buf)) {
                    current = current->get(buf);
                } else {
                    break;
                }
            } while (io.getUnit(buf));

#ifdef LZEncoderInputDBG
            cout << endl;
#endif // LZEncoderInputDBG

            prefix_state = current->getValue().state;

            // Code it and write to outfile
            io.putBits(prefix_state, len);

#ifdef LZEncoderOutputDBG
            __DBGHeader__;
            printf("%d :: %d - %d ", next, prefix_state, len);
#endif // LZEncoderOutputDBG

            if (!io.eof()) {
                // Expand the tree with unreconized symbol
                current->addChild(buf, { prefix_state, next++ });
                io.putUnit(buf);

#ifdef LZEncoderOutputDBG
                printf("%d", buf);
#endif // LZEncoderOutputDBG
            }

#ifdef LZEncoderOutputDBG
            cout << endl;
#endif // LZEncoderOutputDBG
        }

        io.dumpLength();

#ifdef LZCoderDBG
        __FUNCTION_EXIT__;
#endif // LZCoderDBG
    }

    void close()
    {
        io.close();
    }

    ~LZEncoderBase()
    {
        io.close();
    }
};

template <class Unit, unsigned USize>
class LZDecoderBase {
protected:
    ioLZDecoder<Unit, USize> io;

    typedef vector<Unit> Code;

    virtual size_t partition_cnt() = 0;

public:
    void decode()
    {
#ifdef LZCoderDBG
        __FUNCTION_ENTRANCE__;
#endif // LZCoderDBG

        vector<Code*> table;
        table.reserve(partition_cnt() + 1);
        table.push_back(nullptr);

        // Bit* prefixbuf = nullptr;
        Unit tail;

        uint8_t len = 1;
        size_t mcnt = 2;
        size_t prefix_state;

        while (io.getBits(prefix_state, len)) {

            //// Extract the prefix state code
#ifdef LZDecoderInputDBG
            __DBGHeader__;
            printf("%d - %d ", prefix_state, len);
#endif // LZDecoderInputDBG

            // Fetch the tail
            io.getUnit(tail);

#ifdef LZDecoderInputDBG
            printf("%d ", tail);
            cout << endl;
#endif // LZDecoderInputDBG

            if (!io.eof()) {
                // if not eof
                Code* ptr = nullptr;

                if (table[prefix_state] != nullptr) {
                    // if prefix exists
                    Code* prefix = table[prefix_state];

                    ptr = new Code(*prefix);
                    ptr->push_back(tail);
                } else {
                    // if prefix does not exist
                    ptr = new Code;
                    ptr->push_back(tail);
                }

                // write to the file
                table.push_back(ptr);
                io.putUnits(*ptr);

            } else if (table[prefix_state] != nullptr) {
                Code* prefix = table[prefix_state];
                io.putUnits(*prefix);
                break;
            }

            // Maintain the len
            if (table.size() - 1 == mcnt) {
                mcnt *= 2;
                len++;
            }
        }

        size_t table_size = table.size();

        for (size_t i = 0; i < table_size; i++) {
            if (table[i]) {
                delete table[i];
            }
        }

        //io.flush();

#ifdef LZCoderDBG
        __FUNCTION_EXIT__;
#endif // LZCoderDBG
    }

    void close()
    {
        io.close();
    }

    ~LZDecoderBase()
    {
        io.close();
    }
};
